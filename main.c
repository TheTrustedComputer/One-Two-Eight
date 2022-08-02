/*
    Copyright (C) 2022 TheTrustedComputer
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#include "onetwoeight.c"

#ifdef __SIZEOF_INT128__ // Check compiler support for native 128-bit ints
typedef __uint128_t UInt128b;
#else
#error "This compiler does not support 128-bit integers. Use GCC or Clang to compile this program."
#endif // __SIZEOF_INT128__

// Driver code to debug C operators on the OneTwoEight type
int main(void) {
    OneTwoEight a, b, c;
    UInt128b _a, _b, _c;
    unsigned long long operation, shift, _d, _e, _f[39];
    bool cond, _cond;
    
    // Initialize the PRNG
    srand(time(NULL));
    
    for (;;) {
        // Fill with random numbers
        a.lsb = rand() | (((OneTwoEight_t)(rand()) & 1) << 31) | ((OneTwoEight_t)(rand()) << 32) | (((OneTwoEight_t)(rand()) & 1) << 63);
        a.msb = rand() | (((OneTwoEight_t)(rand()) & 1) << 31) | ((OneTwoEight_t)(rand()) << 32) | (((OneTwoEight_t)(rand()) & 1) << 63);
        b.lsb = rand() | (((OneTwoEight_t)(rand()) & 1) << 31) | ((OneTwoEight_t)(rand()) << 32) | (((OneTwoEight_t)(rand()) & 1) << 63);
        b.msb = rand() | (((OneTwoEight_t)(rand()) & 1) << 31) | ((OneTwoEight_t)(rand()) << 32) | (((OneTwoEight_t)(rand()) & 1) << 63);
        c.lsb = rand() | (((OneTwoEight_t)(rand()) & 1) << 31) | ((OneTwoEight_t)(rand()) << 32) | (((OneTwoEight_t)(rand()) & 1) << 63);
        c.msb = rand() | (((OneTwoEight_t)(rand()) & 1) << 31) | ((OneTwoEight_t)(rand()) << 32) | (((OneTwoEight_t)(rand()) & 1) << 63);
        _a = a.lsb | ((UInt128b)(a.msb) << 64);
        _b = b.lsb | ((UInt128b)(b.msb) << 64);
        _c = c.lsb | ((UInt128b)(c.msb) << 64);
        cond = _cond = false;
        
        // Randomize operations
        operation = rand() % 34;
        shift = rand() % 128;
        
        // Do this operation based on RNG result
        switch (operation) {
        case 0: // Add
            c = OneTwoEight_add(a, b);
            _c = _a + _b;
            break;
        case 1: // Subtract
            c = OneTwoEight_subtract(a, b);
            _c = _a - _b;
            break;
        case 2: // Multiply
            c = OneTwoEight_multiply(a, b);
            _c = _a * _b;
            break;
        case 3: // Divide
            if (OneTwoEight_toBool(b) && _b) { // Ensure to not divide by zero
                c = OneTwoEight_divide(a, b, false, NULL);
                _c = _a / _b;
                break;
            }
            continue;
        case 4: // Modulus
            if (OneTwoEight_toBool(b) && _b) {
                OneTwoEight_divide(a, b, true, &c);
                _c = _a % _b;
                break;
            }
            continue;
        case 5: // Add with assignment
            OneTwoEight_addAssign(&c, a);
            _c += _a;
            break;
        case 6: // Subtract with assignment
            OneTwoEight_subtractAssign(&c, a);
            _c -= _a;
            break;
        case 7: // Multiply with assignment
            OneTwoEight_multiplyAssign(&c, a);
            _c *= _a;
            break;
        case 8: // Divide with assignment
            if (OneTwoEight_toBool(a) && _a) {
                OneTwoEight_divideAssign(&c, a);
                _c /= _a;
                break;
            }
            continue;
        case 9: // Modulus with assignment
            if (OneTwoEight_toBool(a) && _a) {
                OneTwoEight_modulusAssign(&c, a, &c);
                _c %= _a;
                break;
            }
            continue;
        case 10: // Bitwise AND
            c = OneTwoEight_bitwiseAnd(a, b);
            _c = _a & _b;
            break;
        case 11: // Bitwise OR
            c = OneTwoEight_bitwiseOr(a, b);
            _c = _a | _b;
            break;
        case 12: // Bitwise XOR
            c = OneTwoEight_bitwiseXor(a, b);
            _c = _a ^ _b;
            break;
        case 13: // Bitwise NOT
            c = OneTwoEight_bitwiseNot(a);
            _c = ~_a;
            break;
        case 14: // Bitwise AND with assignment
            OneTwoEight_bitwiseAndAssign(&c, a);
            _c &= _a;
            break;
        case 15: // Bitwise OR with assignment
            OneTwoEight_bitwiseOrAssign(&c, a);
            _c |= _a;
            break;
        case 16: // Bitwise XOR with assignment
            OneTwoEight_bitwiseXorAssign(&c, a);
            _c ^= _a;
            break;
        case 17: // Logical AND
            cond = OneTwoEight_logicalAnd(a, b);
            _cond = _a && _b;
            break;
        case 18: // Logical OR
            cond = OneTwoEight_logicalOr(a, b);
            _cond = _a || _b;
            break;
        case 19: // Logical NOT
            cond = OneTwoEight_logicalNot(a);
            _cond = !_a;
            break;
        case 20: // Left shift
            c = OneTwoEight_leftShift(a, shift);
            _c = _a << shift;
            break;
        case 21: // Right shift
            c = OneTwoEight_rightShift(a, shift);
            _c = _a >> shift;
            break;
        case 22: // Left shift with assignment
            OneTwoEight_leftShiftAssign(&c, shift);
            _c <<= shift;
            break;
        case 23: // Right shift with assignment
            OneTwoEight_rightShiftAssign(&c, shift);
            _c >>= shift;
            break;
        case 24: // Pre increment
            c = OneTwoEight_preIncrement(&a);
            _c = ++_a;
            break;
        case 25: // Post increment
            c = OneTwoEight_postIncrement(&a);
            _c = _a++;
            break;
        case 26: // Pre decrement
            c = OneTwoEight_preDecrement(&a);
            _c = --_a;
            break;
        case 27: // Post decrement
            c = OneTwoEight_postDecrement(&a);
            _c = _a--;
            break;
        case 28: // Equal
            cond = OneTwoEight_equal(a, b);
            _cond = _a == _b;
            break;
        case 29: // Not equal
            cond = OneTwoEight_notEqual(a, b);
            _cond = _a != _b;
            break;
        case 30: // Less than
            cond = OneTwoEight_lessThan(a, b);
            _cond = _a < _b;
            break;
        case 31: // Less than or equal
            cond = OneTwoEight_lessThanEqual(a, b);
            _cond = _a <= _b;
            break;
        case 32: // Greater than
            cond = OneTwoEight_greaterThan(a, b);
            _cond = _a > _b;
            break;
        case 33: // Greater than or equal
            cond = OneTwoEight_greaterThanEqual(a, b);
            _cond = _a >= _b;
        }
        
        // Verify the results and error out if answers are different from what is expected.
        // This if statement should never be executed when coded properly, and the hardware is in working order.
        // If this occurs for whatever reason, either the running hardware is faulty, or the compiler is bugged.
        if (!(cond && _cond) && (_c != (c.lsb | ((UInt128b)(c.msb) << 64)))) {
            printf("Error in operation %llu!\nExpected: ", operation);
            for (_d = _c, _e = 0; _d; _d /= 10) {
                _f[_e++] = _d % 10;
            }
            while (_e--) {
                printf("%llu", _f[_e]);
            }
            printf("\nActual: ");
            OneTwoEight_print(c, false);
            puts("");
            return 1;
        }
    }
    return 0;
}