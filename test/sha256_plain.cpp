#include "emp-tool/emp-tool.h"

// expected out: 7ca51614425c3ba8ce54dd2fc2020ae7b6e574d198136d0fae7e26ccbf0be7a6
void sha2_check() {
    emp::Integer inp1(512, 0, emp::ALICE);
    emp::Integer inp2(0, 0, emp::ALICE);
    emp::Integer out(256, 0, emp::PUBLIC);
		emp::block one = emp::CircuitExecution::circ_exec->public_label(true);
		inp1.bits[0].bit = one; // custom padding
    std::string filepath = "/usr/local/include/emp-tool/circuits/files/bristol_format/sha-256-big.txt";
    emp::BristolFormat cf(filepath.c_str());
    cf.compute(out.bits.data(), inp1.bits.data(), inp2.bits.data());
    std::cout << out.reveal<std::string>() << std::endl;
    
		emp::Integer inp(512 + 256, 0, emp::ALICE);
    emp::Integer out2(256, 0, emp::PUBLIC);
		std::string filepath2 = "/usr/local/include/emp-tool/circuits/files/bristol_fashion/sha256.txt";
		emp::BristolFashion cf2(filepath2.c_str());
		cf2.compute(out2.bits.data(), inp.bits.data());
    std::cout << out2.reveal<std::string>() << std::endl;

    emp::Integer inp3_1(512, 0, emp::ALICE);
    emp::Integer inp3_2(0, 0, emp::ALICE);
    emp::Integer out3(256, 0, emp::PUBLIC);
    cf.compute(out3.bits.data(), inp3_1.bits.data(), inp3_2.bits.data());
    std::cout << out3.reveal<std::string>() << std::endl;
}

int main(int argc, char** argv) {
	emp::setup_plain_prot(false, "tmp.txt");
    sha2_check();
	emp::finalize_plain_prot();
}

