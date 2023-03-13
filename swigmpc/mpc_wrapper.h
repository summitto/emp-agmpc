#ifndef MPC_WRAPPER
#define MPC_WRAPPER

#include <vector>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

std::vector<bool> perform_mpc(int party, int port, std::string inputFolder);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MPC_WRAPPER
