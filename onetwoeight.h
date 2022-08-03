/*
    Copyright (C) 2022 TheTrustedComputer
*/

#ifndef ONETWOEIGHT_H
#define ONETWOEIGHT_H

/*
    A structure defining a 128-bit integer, using two 64-bit unsigned integers for the halves.
    Some programming languages like Rust has built-in support for it unlike C/C++.
    Additonally some C compilers support 128-bit integers: GCC and Clang do, but MSVC do not.
*/
typedef uint64_t OneTwoEight_t;
typedef struct OneTwoEight {  // Use a little-endian memory mapping
    OneTwoEight_t lsb, msb;
} OneTwoEight;

// Constants for limits of 128-bit integers and other common constants
static const OneTwoEight ONETWOEIGHT_MAX = (OneTwoEight){0xffffffffffffffffull, 0x7fffffffffffffffull};
static const OneTwoEight ONETWOEIGHT_MIN = (OneTwoEight){0x0ull, 0x8000000000000000ull};
static const OneTwoEight ONETWOEIGHT_UMAX = (OneTwoEight){0xffffffffffffffffull, 0xffffffffffffffffull};
static const OneTwoEight ONETWOEIGHT_ZERO = (OneTwoEight){0x0ull, 0x0ull}; // ONETWOEIGHT_UMIN
static const OneTwoEight ONETWOEIGHT_ONE = (OneTwoEight){0x1ull, 0x0ull};

/*
    Functions handling all C operators on 128-bit integers
    Operator overloading is only supported in C++, so use operator names for functions
*/
// Arithmetic
OneTwoEight OneTwoEight_add(const OneTwoEight, const OneTwoEight); // (a + b)
OneTwoEight OneTwoEight_subtract(const OneTwoEight, const OneTwoEight); // (a - b)
OneTwoEight OneTwoEight_multiply(const OneTwoEight, const OneTwoEight); // (a * b)
OneTwoEight OneTwoEight_divide(OneTwoEight, OneTwoEight, const bool, OneTwoEight*); // (a / b)
OneTwoEight OneTwoEight_modulus(OneTwoEight, OneTwoEight, OneTwoEight*); // (a % b)
// Arithmetic with assignment
void OneTwoEight_addAssign(OneTwoEight*, const OneTwoEight); // (a += b)
void OneTwoEight_subtractAssign(OneTwoEight*, const OneTwoEight); // (a -= b)
void OneTwoEight_multiplyAssign(OneTwoEight*, const OneTwoEight); // (a *= b)
void OneTwoEight_divideAssign(OneTwoEight*, OneTwoEight); // (a /= b)
void OneTwoEight_modulusAssign(OneTwoEight*, OneTwoEight, OneTwoEight*); // (a %= b)
// Bitwise
OneTwoEight OneTwoEight_bitwiseAnd(const OneTwoEight, const OneTwoEight); // (a & b)
OneTwoEight OneTwoEight_bitwiseOr(const OneTwoEight, const OneTwoEight); // (a | b)
OneTwoEight OneTwoEight_bitwiseXor(const OneTwoEight, const OneTwoEight); // (a ^ b)
OneTwoEight OneTwoEight_bitwiseNot(const OneTwoEight); // (~a)
OneTwoEight OneTwoEight_leftShift(const OneTwoEight, const int); // (a << b)
OneTwoEight OneTwoEight_rightShift(const OneTwoEight, const int); // (a >> b)
// Bitwise with assignment
void OneTwoEight_bitwiseAndAssign(OneTwoEight*, const OneTwoEight); // (a &= b)
void OneTwoEight_bitwiseOrAssign(OneTwoEight*, const OneTwoEight); // (a |= b)
void OneTwoEight_bitwiseXorAssign(OneTwoEight*, const OneTwoEight); // (a ^= b)
void OneTwoEight_leftShiftAssign(OneTwoEight*, const int); // (a <<= b)
void OneTwoEight_rightShiftAssign(OneTwoEight*, const int); // (a >>= b)
// Increment and decrement
OneTwoEight OneTwoEight_preIncrement(OneTwoEight*); // (++a)
OneTwoEight OneTwoEight_postIncrement(OneTwoEight*); // (a++)
OneTwoEight OneTwoEight_preDecrement(OneTwoEight*); // (--a)
OneTwoEight OneTwoEight_postDecrement(OneTwoEight*); // (a--)
// Relational
bool OneTwoEight_equal(const OneTwoEight, const OneTwoEight); // (a == b)
bool OneTwoEight_notEqual(const OneTwoEight, const OneTwoEight); // (a != b)
bool OneTwoEight_lessThan(const OneTwoEight, const OneTwoEight); // (a < b)
bool OneTwoEight_lessThanEqual(const OneTwoEight, const OneTwoEight); // (a <= b)
bool OneTwoEight_greaterThan(const OneTwoEight, const OneTwoEight); // (a > b)
bool OneTwoEight_greaterThanEqual(const OneTwoEight, const OneTwoEight); // (a >= b)
// Logical
bool OneTwoEight_logicalAnd(const OneTwoEight, const OneTwoEight); // (a && b)
bool OneTwoEight_logicalOr(const OneTwoEight, const OneTwoEight); // (a || b)
bool OneTwoEight_logicalNot(const OneTwoEight); // (!a)

/* Conversations to OneTwoEight */
// Other integral types
OneTwoEight OneTwoEight_fromBool(const bool); // (OneTwoEight)(bool) A bool is technically an int, so is included here
OneTwoEight OneTwoEight_fromChar(const signed char); // (OneTwoEight)(signed char)
OneTwoEight OneTwoEight_fromUChar(const unsigned char); // (OneTwoEight)(unsigned char)
OneTwoEight OneTwoEight_fromShort(const signed short); // (OneTwoEight)(signed short)
OneTwoEight OneTwoEight_fromUShort(const unsigned short); // (OneTwoEight)(unsigned short)
OneTwoEight OneTwoEight_fromInt(const signed); // (OneTwoEight)(signed)
OneTwoEight OneTwoEight_fromUInt(const unsigned); // (OneTwoEight)(unsigned)
OneTwoEight OneTwoEight_fromLong(const signed long); // (OneTwoEight)(signed long)
OneTwoEight OneTwoEight_fromULong(const unsigned long); // (OneTwoEight)(unsigned long)
OneTwoEight OneTwoEight_fromLongLong(const signed long long); // (OneTwoEight)(signed long long)
OneTwoEight OneTwoEight_fromULongLong(const unsigned long long); // (OneTwoEight)(unsigned long long)
// Fixed-widths up to 64-bit
OneTwoEight OneTwoEight_fromInt8(const int8_t); // (OneTwoEight)(int8_t)
OneTwoEight OneTwoEight_fromUInt8(const uint8_t); // (OneTwoEight)(uint8_t)
OneTwoEight OneTwoEight_fromInt16(const int16_t); // (OneTwoEight)(int16_t)
OneTwoEight OneTwoEight_fromUInt16(const uint16_t); // (OneTwoEight)(uint16_t)
OneTwoEight OneTwoEight_fromInt32(const int32_t); // (OneTwoEight)(int32_t)
OneTwoEight OneTwoEight_fromUInt32(const uint32_t); // (OneTwoEight)(uint32_t)
OneTwoEight OneTwoEight_fromInt64(const int64_t); // (OneTwoEight)(int64_t)
OneTwoEight OneTwoEight_fromUInt64(const uint64_t); // (OneTwoEight)(uint64_t)

/* Conversions from OneTwoEight */
// Other integral types
bool OneTwoEight_toBool(const OneTwoEight); // (bool)(OneTwoEight)
signed char OneTwoEight_toChar(const OneTwoEight); // (signed char)(OneTwoEight)
unsigned char OneTwoEight_toUChar(const OneTwoEight); // (unsigned char)(OneTwoEight)
signed short OneTwoEight_toShort(const OneTwoEight); // (signed short)(OneTwoEight)
unsigned short OneTwoEight_toUShort(const OneTwoEight); // (unsigned short)(OneTwoEight)
signed OneTwoEight_toInt(const OneTwoEight); // (signed)(OneTwoEight)
unsigned OneTwoEight_toUInt(const OneTwoEight); // (unsigned)(OneTwoEight)
signed long OneTwoEight_toLong(const OneTwoEight); // (signed long)(OneTwoEight)
unsigned long OneTwoEight_toULong(const OneTwoEight); // (unsigned long)(OneTwoEight)
signed long long OneTwoEight_toLongLong(const OneTwoEight); // (signed long long)(OneTwoEight)
unsigned long long OneTwoEight_toULongLong(const OneTwoEight); // (unsigned long long)(OneTwoEight)
// Fixed-widths up to 64-bit
int8_t OneTwoEight_toInt8(const OneTwoEight); // (int8_t)(OneTwoEight)
uint8_t OneTwoEight_toUInt8(const OneTwoEight); // (uint8_t)(OneTwoEight)
int16_t OneTwoEight_toInt16(const OneTwoEight); // (int16_t)(OneTwoEight)
uint16_t OneTwoEight_toUInt16(const OneTwoEight); // (uint16_t)(OneTwoEight)
int32_t OneTwoEight_toInt32(const OneTwoEight); // (int32_t)(OneTwoEight)
uint32_t OneTwoEight_toUInt32(const OneTwoEight); // (uint32_t)(OneTwoEight)
int64_t OneTwoEight_toInt64(const OneTwoEight); // (int64_t)(OneTwoEight)
uint64_t OneTwoEight_toUInt64(const OneTwoEight); // (uint64_t)(OneTwoEight)

// Generic print function
void OneTwoEight_print(const OneTwoEight, const bool);

#endif // ONETWOEIGHT_H