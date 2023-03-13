#include <emp-tool/emp-tool.h>
#include "emp-agmpc/emp-agmpc.h"
#include <fstream>

#include "emp-agmpc/flexible_input_output.h"

using namespace std;
using namespace emp;

const static int nP = 2;

void test_non_in_out(int party, int port, string circuitFile, string inputs) {
	PRG prg;
	bool delta[128];
	prg.random_bool(delta, 128);	
	NetIOMP<nP> io(party, port);
	NetIOMP<nP> io2(party, port+2*(nP+1)*(nP+1)+1);
	
	NetIOMP<nP> *ios[2] = {&io, &io2};
	ThreadPool pool(2*(nP-1)+2);
	
	// Cannot be BristolFashion because CMPC only accepts BristolFormat
	BristolFormat cf(circuitFile.c_str());
	
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

	bool *in = new bool[cf.n1+cf.n2];
	memset(in, false, cf.n1+cf.n2);

	for (int i = 0; i < cf.n1+cf.n2; i++) {
		in[i] = inputs[i] == 1;
	}
	
	cout << party << "in: ";
	for (auto i = 0; i < cf.n1+cf.n2; i++) {
		 cout << in[i];
		 if (i == cf.n1 - 1) cout << " - ";
	}
	cout << endl;
	
	mpc->online(in, out);
	
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
	int party, port;
	string inputFile;
	
	parse_party_and_port(argv, &party, &port);
	inputFile = argv[3];

	if(party > nP)return 0;
	
	const string circuitFile = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/tlsnotary_safe_c1.txt");
	
	string inputs;
	string line;
	ifstream inputFileStream (inputFile);
	if (inputFileStream.is_open()) {
    while (std::getline(inputFileStream, line)) {
      inputs += line;
    }
    inputFileStream.close();
  }
	
	test_non_in_out(party, port, circuitFile, inputs);

	return 0;
}

