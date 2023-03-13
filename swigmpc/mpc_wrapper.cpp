#include "mpc_wrapper.h"

#include <emp-tool/emp-tool.h>
#include "emp-agmpc/emp-agmpc.h"
#include "emp-agmpc/flexible_input_output.h"

using namespace std;

constexpr const int PARTIES = 2;

void setCustomInput(int party, bool* in, int start, unsigned char *customInput) {
	bool customInputParsed[256];
	for (int i = 0; i < 32; i++) {
		int w = customInput[i];
		for (int j = 0; j < 8; j++) {
			customInputParsed[i * 8 + j] = w & 1 ? true : false;
			w >>= 1;
		}
	}

	if (party == ALICE) {
		for (auto i = 0; i < 256; i=i+2) {
			in[start + i] = customInputParsed[i];
		}
	} else if (party == BOB) {
		for (auto i = 1; i < 256; i=i+2) {
			in[start + i] = customInputParsed[i];
		}
	} else {
		std::cout << "ERROR: This circuit is currently set up to just handle 2 parties..." << std::endl;
	}
}

std::pair<bool*, size_t> run_in_out(int party, int port, const std::string& filename, bool *previousOut, unsigned char *customInput) {
	emp::PRG prg;
	bool delta[128];
	prg.random_bool(delta, 128);
	NetIOMP<PARTIES> io(party, port);
	NetIOMP<PARTIES> io2(party, port+2*(PARTIES+1)*(PARTIES+1)+1);

	NetIOMP<PARTIES> *ios[2] = {&io, &io2};
	ThreadPool pool(2*(PARTIES-1)+2);

	// Cannot be BristolFashion because CMPC only accepts BristolFormat
	emp::BristolFormat cf(filename.c_str());

	CMPC<PARTIES>* mpc = new CMPC<PARTIES>(ios, &pool, party, &cf, delta);
	ios[0]->flush();
	ios[1]->flush();

	mpc->function_independent();
	ios[0]->flush();
	ios[1]->flush();

	mpc->function_dependent();
	ios[0]->flush();
	ios[1]->flush();

	bool *in = new bool[cf.n1+cf.n2];
	memset(in, false, cf.n1+cf.n2);

	if (previousOut) {
		if (party == ALICE) { // Only setting this for one party, easier for now than fixing the sum of all parties
			for (auto i = 0; i < 256; i++) {
				in[i] = previousOut[i];
			}
		}
		if (customInput) {
			setCustomInput(party, in, 256, customInput);
		}
	} else {
		setCustomInput(party, in, 0, customInput);
	}

	bool *out = new bool[cf.n3];
	mpc->online(in, out);
	ios[0]->flush();
	ios[1]->flush();

	if (party == ALICE) {
		std::string outbitstr;
		for (auto i = 0; i < cf.n3; i++) {
      outbitstr += out[i] ? "1" : "0";
		}
		std::reverse(outbitstr.begin(), outbitstr.end());
		std::stringstream reader_reverse(outbitstr);
		std::stringstream result_reverse;
		while (reader_reverse) {
			std::bitset<8> digit;
			reader_reverse >> digit;
			result_reverse << std::hex << digit.to_ulong();
		}
		std::cout << party << " - out: " << result_reverse.str() << std::endl;
	}

	delete mpc;

	return { out, cf.n3 };
}

std::vector<bool> perform_mpc(int party, int port, std::string inputFolder) {
	if (party == 0 || party > PARTIES) {
		return {};
	}

	string filepath1 = inputFolder + "/circuits/n-for-1-auth/key-derivation/DeriveHandshakeSecret_PreMasterSecret.txt";
	unsigned char test_shared_secret[] = { // also known as premaster secret
		0xf3, 0xc5, 0xf4, 0x5a, 0xb5, 0xda, 0x15, 0x05,
		0x4e, 0x78, 0xc3, 0xa8, 0x7b, 0x6b, 0x9f, 0x29,
		0x2e, 0x84, 0xd2, 0x9e, 0x00, 0xdc, 0x97, 0x11,
		0x83, 0x1f, 0x5a, 0xb4, 0x1f, 0xf9, 0xec, 0x6b
	};
	auto out1 = run_in_out(party, port, filepath1, nullptr, test_shared_secret);

	unsigned char test_hello_hash[] = { // also known as handshake_traffic_hash
		0xd7, 0xc1, 0x09, 0xf6, 0xd2, 0x33, 0xa0, 0x1b,
		0xb3, 0x61, 0x67, 0x40, 0x4e, 0xde, 0xa6, 0xb6,
		0xad, 0xaa, 0x14, 0xa2, 0x4f, 0xb0, 0x84, 0x81,
		0xcf, 0xb9, 0x86, 0x12, 0x4a, 0x9f, 0x06, 0x59
	};
	string filepath2 = inputFolder + "/circuits/n-for-1-auth/key-derivation/DeriveClientHandshakeSecret.txt";
	auto out2 = run_in_out(party, port, filepath2, out1.first, test_hello_hash);

	string filepath3 = inputFolder + "/circuits/n-for-1-auth/key-derivation/DeriveServerHandshakeSecret.txt";
	auto out3 = run_in_out(party, port, filepath3, out1.first, test_hello_hash);

	string filepath4 = inputFolder + "/circuits/n-for-1-auth/key-derivation/DeriveClientHandshakeKey.txt";
	auto out4 = run_in_out(party, port, filepath4, out2.first, nullptr);

	string filepath5 = inputFolder + "/circuits/n-for-1-auth/key-derivation/DeriveClientHandshakeIV.txt";
	auto out5 = run_in_out(party, port, filepath5, out2.first, NULL);

	string filepath6 = inputFolder + "/circuits/n-for-1-auth/key-derivation/DeriveMasterSecret.txt";
	auto out6 = run_in_out(party, port, filepath6, out1.first, NULL);

	unsigned char test_finished_hash[] = { // also known as server_finished_hash
		0x6f, 0xe8, 0x69, 0x3c, 0x2a, 0xab, 0xc4, 0x41,
		0xe5, 0xc4, 0x32, 0x09, 0xd3, 0x63, 0xe8, 0x07,
		0xbe, 0xf5, 0x27, 0x9b, 0x9a, 0x11, 0xbd, 0x28,
		0x2a, 0x84, 0x8d, 0x2f, 0x4f, 0x6e, 0x9a, 0x58
	};
	string filepath7 = inputFolder + "/circuits/n-for-1-auth/key-derivation/DeriveClientTrafficSecret.txt";
	auto out7 = run_in_out(party, port, filepath7, out6.first, test_finished_hash);
	// expected: ee23f537c6949bd1e966ce7376a2141fcd5e36d21fe8936f87383c3982ab8ef0

	string filepath8 = inputFolder + "/circuits/n-for-1-auth/key-derivation/DeriveClientTrafficKey.txt";
	auto out8 = run_in_out(party, port, filepath8, out7.first, NULL);

	std::string filepath9 = inputFolder + "/circuits/n-for-1-auth/key-derivation/DeriveClientTrafficIV.txt";
	auto out9 = run_in_out(party, port, filepath9, out7.first, NULL);

	std::vector<bool> result(out9.second);
	for (auto i = 0; i < out9.second; ++i) {
		result[i] = out9.first[i];
	}

	return result;
}
