/*
    Copyright (C) 2022 TheTrustedComputer
*/

#include "onetwoeight.h"

OneTwoEight_t OneTwoEight_numberOfBits(OneTwoEight NUM) {
    OneTwoEight_t loggo = 1;
    while (OneTwoEight_greaterThan(NUM, ONETWOEIGHT_ONE)) {
        OneTwoEight_rightShiftAssign(&NUM, 1);
        ++loggo;
    }
    return loggo;
}

OneTwoEight OneTwoEight_add(const OneTwoEight LEFT, const OneTwoEight RIGHT) {
    OneTwoEight_t sumLSB, sumMSB;

    // Add
    sumLSB = LEFT.lsb + RIGHT.lsb;
    sumMSB = LEFT.msb + RIGHT.msb + (sumLSB < LEFT.lsb); // Check LSB overflow, then carry to MSB; in C, a true is 1; a false is 0.

    // Return sum
    return (OneTwoEight){sumLSB, sumMSB};
}

OneTwoEight OneTwoEight_subtract(const OneTwoEight LEFT, const OneTwoEight RIGHT) {
    OneTwoEight_t diffLSB, diffMSB;

    // Subtract
    diffLSB = LEFT.lsb - RIGHT.lsb;
    diffMSB = LEFT.msb - RIGHT.msb - (diffLSB > LEFT.lsb); // Check LSB underflow, then borrow from MSB

    // Return difference
    return (OneTwoEight){diffLSB, diffMSB};
}

OneTwoEight OneTwoEight_multiply(const OneTwoEight LEFT, const OneTwoEight RIGHT) {
    OneTwoEight_t leftLSBL, leftLSBM, rightLSBL, rightLSBM;
    OneTwoEight_t firstProd, secondProd, thirdProd, fourthProd;
    OneTwoEight_t firstPart, secondPart;
    OneTwoEight_t least64ProdBits, most64ProdBits;

    // The result of multiplication of N bit integers is in 2N bits
    // 64x64 = 128-bit product
    // 128x128 = 256-bit product
    // Break apart 64-bit LSB integers into padded 32-bit integers
    leftLSBL = LEFT.lsb & UINT32_MAX;
    leftLSBM = LEFT.lsb >> 32;
    rightLSBL = RIGHT.lsb & UINT32_MAX;
    rightLSBM = RIGHT.lsb >> 32;

    // Multiply partial products
    firstProd = leftLSBL * rightLSBL;
    secondProd = leftLSBL * rightLSBM;
    thirdProd = leftLSBM * rightLSBL;
    fourthProd = leftLSBM * rightLSBM;

    // Get lower and higher 32-bit parts of 64-bit products and add them, taking into account carries and overflows
    firstPart = (firstProd >> 32) + thirdProd;
    secondPart = (firstPart & UINT32_MAX) + secondProd;

    // Combine them together; we now have a truncated 128-bit product
    least64ProdBits = (firstProd & UINT32_MAX) + (secondPart << 32);
    most64ProdBits = (fourthProd + (firstPart >> 32) + (secondPart >> 32)) + (LEFT.lsb * RIGHT.msb) + (LEFT.msb * RIGHT.lsb);

    // Return that product
    return (OneTwoEight){least64ProdBits, most64ProdBits};
}

OneTwoEight OneTwoEight_divide(const OneTwoEight LEFT, const OneTwoEight RIGHT, const bool USE_REM, OneTwoEight *REM_128) {
    // Division is usually the slowest basic operation of any integer type
    OneTwoEight quot, rem;
    OneTwoEight_t bitsLeft;

    // Check for zero divisor, a classic undefined mathematical operation
    if (!OneTwoEight_toBool(RIGHT)) {
        fprintf(stderr, "Division by zero.\n");
        exit(EXIT_FAILURE);
    }

    // Prepare quotient and remainder for binary long division
    quot = rem = ONETWOEIGHT_ZERO;
    
    // Perform unsigned binary long division algorithm, based on Wikipedia's article on division algorithms
    // It is adapted to work with this data structure, see https://en.wikipedia.org/wiki/Division_algorithm#Long_division
    for (bitsLeft = OneTwoEight_numberOfBits(LEFT); bitsLeft-- > 0;) {
        // rem <<= 1;
        OneTwoEight_leftShiftAssign(&rem, 1);
        // rem |= (LEFT >> bitsLeft) & 1;
        OneTwoEight_bitwiseOrAssign(&rem, OneTwoEight_bitwiseAnd(OneTwoEight_rightShift(LEFT, bitsLeft), ONETWOEIGHT_ONE));
        // if (rem >= RIGHT) {
        if (OneTwoEight_greaterThanEqual(rem, RIGHT)) {
            // quot |= 1 << bitsLeft;
            OneTwoEight_bitwiseOrAssign(&quot, OneTwoEight_leftShift(ONETWOEIGHT_ONE, bitsLeft));
            // rem -= RIGHT;
            OneTwoEight_subtractAssign(&rem, RIGHT);
        }
    }
    
    // See if the caller is requesting the remainder, and avoid null pointer dereferencing
    if (USE_REM && REM_128) {
        *REM_128 = rem;
    }

    // Return the quotient
    return quot;
}

void OneTwoEight_addAssign(OneTwoEight *assigner, const OneTwoEight RIGHT) {
    *assigner = OneTwoEight_add(*assigner, RIGHT);
}

void OneTwoEight_subtractAssign(OneTwoEight *assigner, const OneTwoEight RIGHT) {
    *assigner = OneTwoEight_subtract(*assigner, RIGHT);
}

void OneTwoEight_multiplyAssign(OneTwoEight *assigner, const OneTwoEight RIGHT) {
    *assigner = OneTwoEight_multiply(*assigner, RIGHT);
}

void OneTwoEight_divideAssign(OneTwoEight *assigner, const OneTwoEight RIGHT) {
    *assigner = OneTwoEight_divide(*assigner, RIGHT, false, NULL);
}

void OneTwoEight_modulusAssign(OneTwoEight *assigner, const OneTwoEight RIGHT, OneTwoEight *REM_MOD) {
    OneTwoEight_divide(*assigner, RIGHT, true, REM_MOD);
}

OneTwoEight OneTwoEight_bitwiseAnd(const OneTwoEight LEFT, const OneTwoEight RIGHT) {
    return (OneTwoEight){(LEFT.lsb & RIGHT.lsb), (LEFT.msb & RIGHT.msb)};
}

OneTwoEight OneTwoEight_bitwiseOr(const OneTwoEight LEFT, const OneTwoEight RIGHT) {
    return (OneTwoEight){(LEFT.lsb | RIGHT.lsb), (LEFT.msb | RIGHT.msb)};
}

OneTwoEight OneTwoEight_bitwiseXor(const OneTwoEight LEFT, const OneTwoEight RIGHT) {
    return (OneTwoEight){(LEFT.lsb ^ RIGHT.lsb), (LEFT.msb ^ RIGHT.msb)};
}

OneTwoEight OneTwoEight_bitwiseNot(const OneTwoEight NUM) {
    return (OneTwoEight){~NUM.lsb, ~NUM.msb};
}

OneTwoEight OneTwoEight_leftShift(const OneTwoEight NUM, const int SHIFT_AMOUNT) {
    // The standard states undefined behavior occurs when bit-shifting more than the size of the integer, or in negative amounts
    // My behavior is to return zero as per the condition from above, since it is trying to bit-shift beyond word sizes anyway
    if (!SHIFT_AMOUNT) { // Not shifting any bits
        return NUM;
    }
    if ((SHIFT_AMOUNT <= 0) || (SHIFT_AMOUNT >= 128)) { // More than 128 bits, zero, or negative shift
        return ONETWOEIGHT_ZERO;
    }
    if (SHIFT_AMOUNT >= 64) { // More than 64 bits but less than 128 bits
        return (OneTwoEight){0, (NUM.lsb << (SHIFT_AMOUNT - 64))};
    }
    // Less than 64 bits but greater than zero
    return (OneTwoEight){(NUM.lsb << SHIFT_AMOUNT), (NUM.msb << SHIFT_AMOUNT) | (NUM.lsb >> (64 - SHIFT_AMOUNT))};
}

OneTwoEight OneTwoEight_rightShift(const OneTwoEight NUM, const int SHIFT_AMOUNT) {
    if (!SHIFT_AMOUNT) {
        return NUM;
    }
    if ((SHIFT_AMOUNT <= 0) || (SHIFT_AMOUNT >= 128)) {
        return ONETWOEIGHT_ZERO;
    }
    if (SHIFT_AMOUNT >= 64) {
        return (OneTwoEight){(NUM.msb >> (SHIFT_AMOUNT - 64)), 0};
    }
    return (OneTwoEight){(NUM.lsb >> SHIFT_AMOUNT) | (NUM.msb << (64 - SHIFT_AMOUNT)), (NUM.msb >> SHIFT_AMOUNT)};
}

void OneTwoEight_bitwiseAndAssign(OneTwoEight *assigner, const OneTwoEight RIGHT) {
    *assigner = OneTwoEight_bitwiseAnd(*assigner, RIGHT);
}

void OneTwoEight_bitwiseOrAssign(OneTwoEight *assigner, const OneTwoEight RIGHT) {
    *assigner = OneTwoEight_bitwiseOr(*assigner, RIGHT);
}

void OneTwoEight_bitwiseXorAssign(OneTwoEight *assigner, const OneTwoEight RIGHT) {
    *assigner = OneTwoEight_bitwiseXor(*assigner, RIGHT);
}

void OneTwoEight_leftShiftAssign(OneTwoEight *assigner, const int SHIFT_AMOUNT) {
    *assigner = OneTwoEight_leftShift(*assigner, SHIFT_AMOUNT);
}

void OneTwoEight_rightShiftAssign(OneTwoEight *assigner, const int SHIFT_AMOUNT) {
    *assigner = OneTwoEight_rightShift(*assigner, SHIFT_AMOUNT);
}

OneTwoEight OneTwoEight_preIncrement(OneTwoEight *NUM) {
    // Increment LSB then check overflow
    if (!(++NUM->lsb)) {
        ++NUM->msb;
    }
    // Return incremented value
    return *NUM;
}

OneTwoEight OneTwoEight_postIncrement(OneTwoEight *NUM) {
    // Save old values
    OneTwoEight oldNum = *NUM;
    if (!(++NUM->lsb)) {
        ++NUM->msb;
    }
    // Return old value
    return oldNum;
}

OneTwoEight OneTwoEight_preDecrement(OneTwoEight *NUM) {
    // Decrement LSB then check underflow
    if ((--NUM->lsb) == UINT64_MAX) {
        --NUM->msb;
    }
    // Return decremented value
    return *NUM;
}

OneTwoEight OneTwoEight_postDecrement(OneTwoEight *NUM) {
    OneTwoEight oldNum = *NUM;
    if ((--NUM->lsb) == UINT64_MAX) {
        --NUM->msb;
    }
    return oldNum;
}

bool OneTwoEight_equal(const OneTwoEight LEFT, const OneTwoEight RIGHT) {
    return (LEFT.lsb == RIGHT.lsb) && (LEFT.msb == RIGHT.msb);
}

bool OneTwoEight_notEqual(const OneTwoEight LEFT, const OneTwoEight RIGHT) {
    return (LEFT.lsb != RIGHT.lsb) || (LEFT.msb != RIGHT.msb);
}

bool OneTwoEight_lessThan(const OneTwoEight LEFT, const OneTwoEight RIGHT) {
    return (LEFT.msb < RIGHT.msb) || ((LEFT.msb == RIGHT.msb) && (LEFT.lsb < RIGHT.lsb));
}

bool OneTwoEight_lessThanEqual(const OneTwoEight LEFT, const OneTwoEight RIGHT) {
    return (LEFT.msb < RIGHT.msb) || ((LEFT.msb == RIGHT.msb) && (LEFT.lsb <= RIGHT.lsb));
}

bool OneTwoEight_greaterThan(const OneTwoEight LEFT, const OneTwoEight RIGHT) {
    return (LEFT.msb > RIGHT.msb) || ((LEFT.msb == RIGHT.msb) && (LEFT.lsb > RIGHT.lsb));
}

bool OneTwoEight_greaterThanEqual(const OneTwoEight LEFT, const OneTwoEight RIGHT) {
    return (LEFT.msb > RIGHT.msb) || ((LEFT.msb == RIGHT.msb) && (LEFT.lsb >= RIGHT.lsb));
}

bool OneTwoEight_logicalAnd(const OneTwoEight LEFT, const OneTwoEight RIGHT)  {
    return (LEFT.lsb && RIGHT.lsb) && (LEFT.msb && RIGHT.msb);
}

bool OneTwoEight_logicalOr(const OneTwoEight LEFT, const OneTwoEight RIGHT) {
    return (LEFT.lsb || RIGHT.lsb) || (LEFT.msb || RIGHT.msb);
}

bool OneTwoEight_logicalNot(const OneTwoEight NUM) {
    return !(NUM.lsb || NUM.msb);
}

OneTwoEight OneTwoEight_fromBool(const bool BOOL) {
    return (OneTwoEight){BOOL, 0};
}

OneTwoEight OneTwoEight_fromChar(const signed char S_CHAR) {
    return (OneTwoEight){S_CHAR, 0};
}

OneTwoEight OneTwoEight_fromUChar(const unsigned char U_CHAR) {
    return (OneTwoEight){U_CHAR, 0};
}

OneTwoEight OneTwoEight_fromShort(const signed short S_SHORT) {
    return (OneTwoEight){S_SHORT, 0};
}

OneTwoEight OneTwoEight_fromUShort(const unsigned short U_SHORT) {
    return (OneTwoEight){U_SHORT, 0};
}

OneTwoEight OneTwoEight_fromInt(const signed S_INT) {
    return (OneTwoEight){S_INT, 0};
}

OneTwoEight OneTwoEight_fromUInt(const unsigned U_INT) {
    return (OneTwoEight){U_INT, 0};
}

OneTwoEight OneTwoEight_fromLong(const signed long S_LONG) {
    return (OneTwoEight){S_LONG, 0};
}

OneTwoEight OneTwoEight_fromULong(const unsigned long U_LONG) {
    return (OneTwoEight){U_LONG, 0};
}

OneTwoEight OneTwoEight_fromLongLong(const signed long long S_LONG_LONG) {
    return (OneTwoEight){S_LONG_LONG, 0};
}

OneTwoEight OneTwoEight_fromULongLong(const unsigned long long U_LONG_LONG) {
    return (OneTwoEight){U_LONG_LONG, 0};
}

OneTwoEight OneTwoEight_fromInt8(const int8_t SINT8_T) {
    return (OneTwoEight){SINT8_T, 0};
}

OneTwoEight OneTwoEight_fromUInt8(const uint8_t UINT8_T) {
    return (OneTwoEight){UINT8_T, 0};
}

OneTwoEight OneTwoEight_fromInt16(const int16_t SINT16_T) {
    return (OneTwoEight){SINT16_T, 0};
}

OneTwoEight OneTwoEight_fromUInt16(const uint16_t UINT16_T) {
    return (OneTwoEight){UINT16_T, 0};
}

OneTwoEight OneTwoEight_fromInt32(const int32_t SINT32_T) {
    return (OneTwoEight){SINT32_T, 0};
}

OneTwoEight OneTwoEight_fromUInt32(const uint32_t UINT32_T) {
    return (OneTwoEight){UINT32_T, 0};
}

OneTwoEight OneTwoEight_fromInt64(const int64_t SINT64_T) {
    return (OneTwoEight){SINT64_T, 0};
}

OneTwoEight OneTwoEight_fromUInt64(const uint64_t UINT64_T) {
    return (OneTwoEight){UINT64_T, 0};
}

bool OneTwoEight_toBool(const OneTwoEight ONETWOEIGHT) {
    return (ONETWOEIGHT.lsb || ONETWOEIGHT.msb);
}

signed char OneTwoEight_toChar(const OneTwoEight ONETWOEIGHT) {
    return (signed char)(ONETWOEIGHT.lsb);
}

unsigned char OneTwoEight_toUChar(const OneTwoEight ONETWOEIGHT) {
    return (unsigned char)(ONETWOEIGHT.lsb);
}

signed short OneTwoEight_toShort(const OneTwoEight ONETWOEIGHT) {
    return (signed short)(ONETWOEIGHT.lsb);
}

unsigned short OneTwoEight_toUShort(const OneTwoEight ONETWOEIGHT) {
    return (unsigned short)(ONETWOEIGHT.lsb);
}

signed OneTwoEight_toInt(const OneTwoEight ONETWOEIGHT) {
    return (signed)(ONETWOEIGHT.lsb);
}

unsigned OneTwoEight_toUInt(const OneTwoEight ONETWOEIGHT) {
    return (unsigned)(ONETWOEIGHT.lsb);
}

signed long OneTwoEight_toLong(const OneTwoEight ONETWOEIGHT) {
    return (signed long)(ONETWOEIGHT.lsb);
}

unsigned long OneTwoEight_toULong(const OneTwoEight ONETWOEIGHT) {
    return (unsigned long)(ONETWOEIGHT.lsb);
}

signed long long OneTwoEight_toLongLong(const OneTwoEight ONETWOEIGHT) {
    return (signed long long)(ONETWOEIGHT.lsb);
}

unsigned long long OneTwoEight_toULongLong(const OneTwoEight ONETWOEIGHT) {
    return (unsigned long long)(ONETWOEIGHT.lsb);
}

int8_t OneTwoEight_toInt8(const OneTwoEight ONETWOEIGHT) {
    return (int8_t)(ONETWOEIGHT.lsb);
}

uint8_t OneTwoEight_toUInt8(const OneTwoEight ONETWOEIGHT) {
    return (uint8_t)(ONETWOEIGHT.lsb);
}

int16_t OneTwoEight_toInt16(const OneTwoEight ONETWOEIGHT) {
    return (int16_t)(ONETWOEIGHT.lsb);
}

uint16_t OneTwoEight_toUInt16(const OneTwoEight ONETWOEIGHT) {
    return (uint16_t)(ONETWOEIGHT.lsb);
}

int32_t OneTwoEight_toInt32(const OneTwoEight ONETWOEIGHT) {
    return (int32_t)(ONETWOEIGHT.lsb);
}

uint32_t OneTwoEight_toUInt32(const OneTwoEight ONETWOEIGHT) {
    return (uint32_t)(ONETWOEIGHT.lsb);
}

int64_t OneTwoEight_toInt64(const OneTwoEight ONETWOEIGHT) {
    return (int64_t)(ONETWOEIGHT.lsb);
}

uint64_t OneTwoEight_toUInt64(const OneTwoEight ONETWOEIGHT) {
    return (uint64_t)(ONETWOEIGHT.lsb);
}

void OneTwoEight_print(const OneTwoEight NUM, const bool SIGN) {
    OneTwoEight basePrint = NUM, baseDigit;
    int digit128Index = 0, digits128[39];
    bool negative = SIGN && (NUM.msb & 0x8000000000000000ull);

    // Print only LSB if MSB is non-zero
    if (NUM.msb) {
        // Print negative sign when the highest bit of MSB is set
        if (negative) {
            printf("-");
             // Do a two's complement on the number to get original value
            basePrint = OneTwoEight_add(OneTwoEight_bitwiseNot(basePrint), ONETWOEIGHT_ONE);
        }
        // Compute decimal digits
        while (OneTwoEight_toBool(basePrint)) {
            OneTwoEight_modulusAssign(&basePrint, OneTwoEight_fromInt(10), &baseDigit);
            digits128[digit128Index++] = OneTwoEight_toInt(baseDigit);
        }
        // Print the digits
        while (digit128Index--) {
            printf("%d", digits128[digit128Index]);
        }
    }
    else {
        printf((negative ? "%" PRIi64 : "%" PRIu64), NUM.lsb);
    }
}