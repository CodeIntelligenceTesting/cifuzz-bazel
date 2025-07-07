#ifndef CIFUZZ_CIFUZZ_H
#define CIFUZZ_CIFUZZ_H

/* Include the headers providing the definitions required to use FUZZ_TEST. */
#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#endif

#ifdef __cplusplus
#define CIFUZZ_C_LINKAGE extern "C"
#else
#define CIFUZZ_C_LINKAGE
#endif

static void LLVMFuzzerTestOneInputNoReturn(const uint8_t *data, size_t size);

#ifdef CIFUZZ_GCOV
#define DEFINE_LLVM_FUZZER_TEST_ONE_INPUT                                      \
  CIFUZZ_C_LINKAGE void __gcov_dump();                                         \
  CIFUZZ_C_LINKAGE void __gcov_reset();                                        \
  CIFUZZ_C_LINKAGE int LLVMFuzzerTestOneInput(const uint8_t *data,             \
                                              size_t size) {                   \
    LLVMFuzzerTestOneInputNoReturn(data, size);                                \
    __gcov_dump();                                                             \
    __gcov_reset();                                                            \
    return 0;                                                                  \
  }
#else
#define DEFINE_LLVM_FUZZER_TEST_ONE_INPUT                                      \
  CIFUZZ_C_LINKAGE int LLVMFuzzerTestOneInput(const uint8_t *data,             \
                                              size_t size) {                   \
    LLVMFuzzerTestOneInputNoReturn(data, size);                                \
    return 0;                                                                  \
  }
#endif

#define FUZZ_TEST                                                              \
  DEFINE_LLVM_FUZZER_TEST_ONE_INPUT;                                           \
  void LLVMFuzzerTestOneInputNoReturn

#ifdef __APPLE__
#define DECLARE_LLVM_FUZZER_INITIALIZE_NO_RETURN                               \
  CIFUZZ_C_LINKAGE void LLVMFuzzerInitializeNoReturn(void)
#else
#define DECLARE_LLVM_FUZZER_INITIALIZE_NO_RETURN                               \
  __attribute__((weak)) void LLVMFuzzerInitializeNoReturn(void)
#endif

#define FUZZ_TEST_SETUP                                                        \
  DECLARE_LLVM_FUZZER_INITIALIZE_NO_RETURN;                                    \
  CIFUZZ_C_LINKAGE int LLVMFuzzerInitialize(int *argc, char ***argv) {         \
    (void)argc;                                                                \
    (void)argv;                                                                \
    LLVMFuzzerInitializeNoReturn();                                            \
    return 0;                                                                  \
  }                                                                            \
  void LLVMFuzzerInitializeNoReturn

#endif // CIFUZZ_CIFUZZ_H
