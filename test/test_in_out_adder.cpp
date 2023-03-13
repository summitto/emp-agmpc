#include <emp-tool/emp-tool.h>
#include "emp-agmpc/emp-agmpc.h"

#include "emp-agmpc/flexible_input_output.h"

using namespace std;
using namespace emp;

const static int nP = 2;
int party, port;

void test_non_in_out(int party, int port, string filename) {
	cout << party << "Standard in/out without using FlexIn/FlexOut" << endl;
	cout << party << "compute: K = E_{010101...}(101010...); E_{010101...}(K)" << endl;
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
	
	bool *in = new bool[cf.n1+cf.n2];
	// bool in[cf.n1+cf.n2];
	memset(in, false, cf.n1+cf.n2);
	// memset(in, true, 1);

	bool *out = new bool[cf.n3];

	if(party == ALICE) {
		memset(in, true, cf.n1);
		cout << party << "in: ";
		for (auto i = 0; i < cf.n1+cf.n2; i++) {
			 cout << in[i];
			 if (i == cf.n1 - 1) cout << " - ";
		}
		cout << endl;
	} else if (party == BOB){
		// for (auto i = 0; i < cf.n2; i++) {
		//   in[i+cf.n1] = i % 2 == 0;
		// }
		memset(in+cf.n1, false, cf.n2);
		cout << party << "in: ";
		for (auto i = 0; i < cf.n1+cf.n2; i++) {
			 cout << in[i];
			 if (i == cf.n1 - 1) cout << " - ";
		}
		cout << endl;
	} else {
		for (auto i = 0; i < cf.n2; i++) {
			in[i+cf.n1] = i % 2 == 1;
		}
		// memset(in+cf.n1, true, cf.n2);
		cout << party << "in: ";
		for (auto i = 0; i < cf.n1+cf.n2; i++) {
			 cout << in[i];
			 if (i == cf.n1 - 1) cout << " - ";
		}
		cout << endl;
	}
	// cout << party << "in: ";
	// for (auto i = 0; i < cf.n1+cf.n2; i++) {
	//    cout << in[i];
	//    if (i == cf.n1 - 1) cout << " - ";
	// }
	// cout << endl;
	
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
	parse_party_and_port(argv, &party, &port);
	if(party > nP)return 0;
	
	const string filename1 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/adder64_custom_safe.txt");
	test_non_in_out(party, port, filename1);

	const string filename2 = macro_xstr(EMP_CIRCUIT_PATH) + string("bristol_format/adder64_tlsnotary.txt");
	test_non_in_out(party, port, filename2);
	
	return 0;
}

