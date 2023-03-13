# EMP-agmpc
![arm](https://github.com/emp-toolkit/emp-agmpc/workflows/arm/badge.svg)
![x86](https://github.com/emp-toolkit/emp-agmpc/workflows/x86/badge.svg)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/emp-toolkit/emp-agmpc.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/emp-toolkit/emp-agmpc/alerts/)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/emp-toolkit/emp-agmpc.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/emp-toolkit/emp-agmpc/context:cpp)

<img src="https://raw.githubusercontent.com/emp-toolkit/emp-readme/master/art/logo-full.jpg" width=300px/>

## Global-Scale Secure Multiparty Computation

More details of the protocol can be found in the [paper](https://eprint.iacr.org/2017/189).

<img src="https://raw.githubusercontent.com/emp-toolkit/emp-readme/master/art/logo-full.jpg" width=300px/>


# Installation
1. `wget https://raw.githubusercontent.com/emp-toolkit/emp-readme/master/scripts/install.py`
2. `python install.py -install -tool -ot -agmpc`
    1. By default it will build for Release. `-DCMAKE_BUILD_TYPE=[Release|Debug]` option is also available.
    2. No sudo? Change [`CMAKE_INSTALL_PREFIX`](https://cmake.org/cmake/help/v2.8.8/cmake.html#variable%3aCMAKE_INSTALL_PREFIX).

## Usage
Our custom circuits present in `circuits/` should be copied into `/usr/local/include/emp-tool/circuits/files/bristol_format/`.
The circuits either come from EMP or [TLSNotary](https://github.com/tlsnotary/circuits/tree/36e12884328a79f3f8f0dda1f0da15ce42914c5d).
Then you can run the following to run some tests:

```
./run bin/test_sha256
./run2 bin/test_test_in_out_sha256
./bin/test_sha256_plain

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
./bin/test_generate_handshake_circuits ${SCRIPT_DIR}/circuits/n-for-1-auth/

./run2Inputs bin/test_test_in_out_tls_n_for_1
```

We also have scripts to calculate some SHA/TLS test vectors, some adapted from [illustrated TLS](https://tls13.ulfheim.net/):
```
python3 scripts/sha_test_vectors.py
shared_secret=df4a291baa1eb7cfa6934b29b474baad2697e29f1f920dcc77c8a0a088447624 // Just an example value which also happens to be used in one of the n-for-1-auth tests
zero_key=0000000000000000000000000000000000000000000000000000000000000000
early_secret=$(./hkdf-256.sh extract 00 $zero_key)
empty_hash=$(openssl sha256 < /dev/null | sed -e 's/.* //')
derived_secret=$(./hkdf-256.sh expandlabel $early_secret "derived" $empty_hash 32)
handshake_secret=$(./hkdf-256.sh extract $derived_secret $shared_secret)
csecret=$(./hkdf-256.sh expandlabel $handshake_secret "c hs traffic" $hello_hash 32)
ssecret=$(./hkdf-256.sh expandlabel $handshake_secret "s hs traffic" $hello_hash 32)
client_handshake_key=$(./hkdf-256.sh expandlabel $csecret "key" "" 32)
server_handshake_key=$(./hkdf-256.sh expandlabel $ssecret "key" "" 32)
client_handshake_iv=$(./hkdf-256.sh expandlabel $csecret "iv" "" 12)
server_handshake_iv=$(./hkdf-256.sh expandlabel $ssecret "iv" "" 12)
```

For `hello_hash` and `finished_hash`, you'll really have to realy on openssl's code.

### Question
Please send email to wangxiao1254@gmail.com

# Generating GO wrapper

1. Run `./swig_wrap.sh`
2. Add to `swigmpc/swigmpc.go` the following lines right before `import "C"`:
    - `// #cgo CXXFLAGS: -mavx2 -maes -ggdb2 -fpic`
    - `// #cgo LDFLAGS: -lcrypto -lboost_system`
3. Build Go app as usual
```
go build -o bin/mpc_server server.go
```
