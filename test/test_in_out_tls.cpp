#include <emp-tool/emp-tool.h>
#include "emp-agmpc/emp-agmpc.h"

#include "emp-agmpc/flexible_input_output.h"

using namespace std;
using namespace emp;

const static int nP = 2;
int party, port;

void test_non_in_out(int party, int port, string filename, bool customInput = false) {
	PRG prg;
	bool delta[128];
	prg.random_bool(delta, 128);	
	NetIOMP<nP> io(party, port);
	NetIOMP<nP> io2(party, port+2*(nP+1)*(nP+1)+1);
	
	NetIOMP<nP> *ios[2] = {&io, &io2};
	ThreadPool pool(2*(nP-1)+2);
	
	// Cannot be BristolFashion because CMPC only accepts BristolFormat
	BristolFormat cf(filename.c_str());
	
	CMPC<nP>* mpc = new CMPC<nP>(ios, &pool, party, &cf, delta);
	ios[0]->flush();
	ios[1]->flush();
	
	mpc->function_independent();
	ios[0]->flush();
	ios[1]->flush();
	
	mpc->function_dependent();
	ios[0]->flush();
	ios[1]->flush();
	
	
	bool *out = new bool[cf.n3];

	if(party == ALICE && customInput) {
		bool in[] = {
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
			false, false, true, true, false, false, false, true, false, false, false, false, false, true, false, false, false, true, false, true, false, false, true, false, true, true, false, true, true, false, false, true, true, true, true, false, true, false, true, true, false, false, true, false, true, false, true, true, false, true, true, false, true, true, true, true, true, false, true, true, true, false, true, false, false, false, true, true, false, false, true, false, false, true, true, false, true, false, true, false, true, true, true, true, true, false, false, true, true, true, false, false, false, false, false, true, true, true, false, true, true, true, false, false, true, false, true, false, false, true, false, false, false, true, false, false, true, true, true, false, true, true, false, false, false, false, false, true, false, false, true, true, false, false, false, true, true, true, false, false, true, true, false, false, true, false, true, true, false, false, false, false, true, true, false, false, false, false, false, true, false, false, true, true, true, false, false, true, false, false, true, false, false, false, false, true, false, false, true, false, true, false, true, true, true, false, true, false, false, true, true, true, true, true, true, false, false, true, false, true, false, false, false, false, true, false, false, false, true, false, false, true, true, true, true, false, true, true, true, true, false, false, true, true, false, false, true, false, false, true, false, true, false, false, false, true, false, false, true, true, true, false, false, false, true, false, false, true, false, true, false, true, false, true, false, true, false, true, true, true, true, false, true, true, false, true, true, false, false, false, true, false, true, true, false, true, false, true, false, false, true, true, false, true, true, false, false, true, true, false, false, false, true, true, true, true, false, false, true, true, false, true, false, false, true, false, false, false, false, true, true, true, false, true, true, false, false, true, true, false, true, false, true, false, true, true, true, false, false, true, false, true, true, true, true, false, true, true, false, false, true, true, true, true, false, false, true, false, false, false, true, true, false, true, false, true, true, false, true, false, true, true, true, false, true, true, true, false, false, true, false, true, true, true, true, true, false, false, true, false, true, false, true, true, true, true, false, false, true, true, false, false, false, false, true, false, true, true, true, false, false, true, true, true, false, false, true, false, true, false, true, true, false, true, false, false, true, true, false, true, true, false, false, true, false, true, true, true, false, true, true, false, false, false, true, false, false, false, true, true, false, true, true, true, true, true, false, false, true, true, true, true, true, false, false, true, false, false, false, true, true, true, true, false, false, false, true, false, true, false, false, true, true, true, false, false, false, false, true, true, true, false, true, false, false, true, false, false, true, false, true, false, true, true, true, true, false, true, true, true, true, false, true, false, true, false, true, false, true, false, false, false
		};
		cout << party << "in: ";
		for (auto i = 0; i < cf.n1+cf.n2; i++) {
			 cout << in[i];
			 if (i == cf.n1 - 1) cout << " - ";
		}
		cout << endl;
	
		mpc->online(in, out);
	} else if (party == BOB && customInput){
		bool in[] = { 
			true, true, false, false, false, true, true, true, false, false, false, false, true, false, false, false, true, true, false, true, true, true, true, true, true, true, true, false, false, true, false, true, true, false, false, true, false, false, false, true, true, false, true, false, false, true, true, false, true, false, true, false, false, false, false, false, true, false, false, false, false, true, false, true, false, true, false, false, true, false, false, false, false, true, false, true, false, true, true, true, true, true, true, false, true, true, true, true, true, true, false, true, true, true, false, true, true, true, false, true, true, true, true, false, false, true, false, false, false, true, false, false, true, false, true, false, false, false, false, true, false, false, true, false, false, true, true, false, false, true, true, true, false, false, true, false, false, true, false, false, true, true, true, false, false, false, true, false, true, false, false, false, true, false, false, false, false, false, true, true, false, true, true, false, true, true, false, true, false, true, false, true, true, true, false, false, false, true, false, false, true, true, true, true, false, false, false, true, true, true, false, true, true, false, false, false, false, true, true, false, false, false, false, true, false, true, true, true, false, true, false, true, true, false, false, true, true, false, true, false, false, false, true, true, false, true, true, true, false, false, false, false, true, true, false, true, false, false, true, true, true, false, false, false, true, true, true, true, false, false, true, true, false, true, true, false, true, false, true, true, true, false, true, false, true, false, false, false, false, true, true, false, false, false, false, false, false, true, true, false, true, true, true, true, false, false, true, false, true, true, true, true, true, false, false, false, true, false, true, false, false, false, true, false, false, false, true, false, true, false, true, true, true, true, true, true, true, false, true, true, false, false, false, true, true, true, true, false, true, false, true, false, true, false, false, false, false, false, true, false, true, false, true, true, true, true, true, false, true, false, false, false, true, true, false, true, false, false, false, true, false, true, false, false, false, true, false, true, false, false, false, true, true, false, false, false, false, false, true, true, true, false, true, false, false, true, true, false, true, true, false, false, true, false, false, true, true, false, true, true, true, true, true, true, false, false, true, true, false, true, false, true, true, false, true, true, true, true, false, false, false, true, false, false, true, false, true, true, false, true, false, false, false, false, false, true, true, true, false, false, true, false, true, true, true, true, false, true, true, false, false, false, true, false, true, true, false, true, true, true, true, true, false, true, true, false, true, true, false, false, true, false, false, false, false, true, true, false, false, true, true, true, true, false, true, true, true, true, false, false, true, false, false, true, true, true, false, false, false, true, true, true, true, true, false, false, true, true, true, false, true, false,
			false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false
		};
		cout << party << "in: ";
		for (auto i = 0; i < cf.n1+cf.n2; i++) {
			 cout << in[i];
			 if (i == cf.n1 - 1) cout << " - ";
		}
		cout << endl;
		
		mpc->online(in, out);
	} else {
		bool *in = new bool[cf.n1+cf.n2];
		memset(in, false, cf.n1+cf.n2);
		mpc->online(in, out);
	}
	
	ios[0]->flush();
	ios[1]->flush();

	cout << party << "out1: ";
	for (auto i = 0; i < cf.n3; i++) {
		 cout << out[i];
	}
	cout << endl;
	cout << "mpc 1 done" << endl;
	
	delete mpc;
}

int main(int argc, char** argv) {
	parse_party_and_port(argv, &party, &port);
	if(party > nP)return 0;
	
	// This is failing, so we're making a custom version
	// const string filename1 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/tlsnotary_c1.txt");
	// test_non_in_out(party, port, filename1);
	// const string filename7 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/tlsnotary_combine_pms_shares.txt");
	// test_non_in_out(party, port, filename7);

	const string filename2 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/tlsnotary_getP256prime.txt");
	test_non_in_out(party, port, filename2);
	
	const string filename3 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/compare-lt-32-bit-unsigned-old.txt");
	test_non_in_out(party, port, filename3);
	
	const string filename4 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/tlsnotary_getP256primeOnesComplement.txt");
	test_non_in_out(party, port, filename4);
	
	const string filename5 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/tlsnotary_mult256by1.txt");
	test_non_in_out(party, port, filename5);

	const string filename6 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/tlsnotary_safe_combine_pms_shares.txt");
	test_non_in_out(party, port, filename6);

	const string filename8 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/tlsnotary_safe_shaPmsXorPadding.txt");
	test_non_in_out(party, port, filename8);

	cout << "------------------------------------------------------------\n\n\n" << endl;

	const string filename9 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/tlsnotary_safe_c1.txt");
	test_non_in_out(party, port, filename9, true);

	const string filename10 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/tlsnotary_c2.txt");
	test_non_in_out(party, port, filename10);

	const string filename11 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/tlsnotary_c3.txt");
	test_non_in_out(party, port, filename11);

	const string filename12 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/tlsnotary_c4.txt");
	test_non_in_out(party, port, filename12);

	const string filename13 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/tlsnotary_c5.txt");
	test_non_in_out(party, port, filename13);

	const string filename14 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/tlsnotary_c6.txt");
	test_non_in_out(party, port, filename14);

	const string filename15 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/tlsnotary_c7.txt");
	test_non_in_out(party, port, filename15);

	cout << "------------------------------------------------------------\n\n\n" << endl;

	return 0;
}