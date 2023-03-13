#include "emp-tool/emp-tool.h"

void adder_check() {
    emp::Integer inp1(64, 0, emp::ALICE);
    emp::Integer inp2(64, 0, emp::ALICE);
    emp::Integer out(65, 0, emp::PUBLIC);
		emp::block one = emp::CircuitExecution::circ_exec->public_label(true);
		inp1.bits[0].bit = one;
		inp2.bits[0].bit = one;
    std::string filepath = "/usr/local/include/emp-tool/circuits/files/bristol_format/adder_64bit.txt";
    emp::BristolFormat cf(filepath.c_str());
    cf.compute(out.bits.data(), inp1.bits.data(), inp2.bits.data());
    std::cout << out.reveal<std::string>() << std::endl;

		emp::Integer inp(64 + 64, 0, emp::ALICE);
    emp::Integer out2(64, 0, emp::PUBLIC);
		inp.bits[0].bit = one;
		inp.bits[64].bit = one;
		std::string filepath2 = "/usr/local/include/emp-tool/circuits/files/bristol_fashion/adder64_tlsnotary.txt";
		emp::BristolFashion cf2(filepath2.c_str());
		cf2.compute(out2.bits.data(), inp.bits.data());
    std::cout << out2.reveal<std::string>() << std::endl;

		emp::Integer inp3(64 + 64, 0, emp::ALICE);
    emp::Integer out3(65, 0, emp::PUBLIC);
		inp3.bits[0].bit = one;
		inp3.bits[64].bit = one;
		std::string filepath3 = "/usr/local/include/emp-tool/circuits/files/bristol_fashion/adder_64_custom.txt";
		emp::BristolFashion cf3(filepath3.c_str());
		cf3.compute(out3.bits.data(), inp3.bits.data());
    std::cout << out3.reveal<std::string>() << std::endl;

		emp::Integer inp4_1(64, 0, emp::ALICE);
		emp::Integer inp4_2(64, 0, emp::BOB);
    emp::Integer out4(65, 0, emp::PUBLIC);
		inp4_1.bits[0].bit = one;
		inp4_2.bits[0].bit = one;
		std::string filepath4 = "/usr/local/include/emp-tool/circuits/files/bristol_format/adder64_custom_safe.txt";
		emp::BristolFormat cf4(filepath4.c_str());
		cf4.compute(out4.bits.data(), inp4_1.bits.data(), inp4_2.bits.data());
    std::cout << out4.reveal<std::string>() << std::endl;
}

int main(int argc, char** argv) {
	emp::setup_plain_prot(false, "tmp.txt");
    adder_check();
	emp::finalize_plain_prot();
}


