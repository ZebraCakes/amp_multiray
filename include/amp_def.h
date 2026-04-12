#pragma once

#include <cstdint>
#include <sys/types.h>

using u8  = uint8_t;
using u16 = uint16_t;
using u64 = uint64_t;
using u32 = uint32_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

// TODO(apeterson): When implemented in clang, use fixed width fp types
//                  https://cppreference.net/cpp/types/floating-point.html
using f32 = float;
using f64 = double;

using b32 = i32;

using bitset8  = u8;
using bitset16 = u16;
using bitset32 = u32;
using bitset64 = u64;

#define internal static
#define global static
#define local_persist static

#define kilobytes(n) (n)*1024
#define megabytes(n) kilobytes(n)*1024
#define gigabytes(n) megabytes(n)*1024

#define amp_swap(a, b, T){\
                        T temp = (a);\
                      (a) = (b);\
                      (b) = temp;\
}

#ifdef MM_SLOW
#define _ASSERT_GLUE(a, b) a ## b
#define ASSERT_GLUE(a, b) _ASSERT_GLUE(a, b)

#define ASSERT(exp) if(exp){}else{*(int*)0 = 0;}
#define STATIC_ASSERT(exp) enum{ASSERT_GLUE(g_assert_fail_, __LINE__) = 1 / (int)(!!(expr))}
#else
#define ASSERT(exp)
#define STATIC_ASSERT(exp)
#endif
