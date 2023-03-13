#include "emp-tool/emp-tool.h"

void tls_check() {
		emp::Integer inp(512 + 512, 0, emp::ALICE);
    emp::Integer out(512, 0, emp::PUBLIC);
		std::string filepath = "/usr/local/include/emp-tool/circuits/files/bristol_fashion/tlsnotary_c1_original.txt";
		emp::BristolFashion cf(filepath.c_str());
		cf.compute(out.bits.data(), inp.bits.data());
    std::cout << out.reveal<std::string>() << std::endl;

		emp::Integer in1(512, 0, emp::ALICE);
		emp::Integer in2(512, 0, emp::BOB);
    emp::Integer out2(512, 0, emp::PUBLIC);
		std::string filepath2 = "/usr/local/include/emp-tool/circuits/files/bristol_format/tlsnotary_c1.txt";
		emp::BristolFormat cf2(filepath2.c_str());
		cf2.compute(out2.bits.data(), in1.bits.data(), in2.bits.data());
    std::cout << out2.reveal<std::string>() << std::endl;
}

int main(int argc, char** argv) {
	emp::setup_plain_prot(false, "tmp.txt");
    tls_check();
	emp::finalize_plain_prot();
}


