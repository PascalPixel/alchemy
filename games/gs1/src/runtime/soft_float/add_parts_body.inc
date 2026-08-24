/*
 * SPDX-License-Identifier: GPL-2.0-or-later WITH GCC-exception-2.0
 *
 * Shared body reconstructed from the matching `_fpadd_parts` routine in the
 * approved local GCC 2.96 runtime source. The wrapper supplies the
 * overlay-local entry points used by this embedded copy of the routine.
 */
    SoftFloatSignedFraction signedFraction;
    int leftExponent;
    int rightExponent;
    SoftFloatFraction leftFraction;
    SoftFloatFraction rightFraction;

    if (SoftFloatLeftIsNaN(left)) {
        return left;
    }
    if (SoftFloatRightIsNaN(right)) {
        return right;
    }
    if (SoftFloatLeftIsInfinity(left)) {
        if (SoftFloatRightIsInfinityWithLeftInfinity(right) &&
            left->sign != right->sign) {
            return GetSoftFloatNaNRecord();
        }
        return left;
    }
    if (SoftFloatRightIsInfinity(right)) {
        return right;
    }
    if (SoftFloatRightIsZero(right)) {
        if (SoftFloatLeftIsZeroWithRightZero(left)) {
            *result = *left;
            result->sign = left->sign & right->sign;
            return result;
        }
        return left;
    }
    if (SoftFloatLeftIsZero(left)) {
        return right;
    }

    {
        int exponentDifference;

        leftExponent = left->exponent;
        rightExponent = right->exponent;
        leftFraction = left->fraction.whole;
        rightFraction = right->fraction.whole;

        exponentDifference = leftExponent - rightExponent;
        if (exponentDifference < 0) {
            exponentDifference = -exponentDifference;
        }
        if (exponentDifference < SOFT_FLOAT_FRACTION_BITS) {
            while (leftExponent > rightExponent) {
                rightExponent++;
                SOFT_FLOAT_SHIFT_RIGHT_STICKY(rightFraction);
            }
            while (rightExponent > leftExponent) {
                leftExponent++;
                SOFT_FLOAT_SHIFT_RIGHT_STICKY(leftFraction);
            }
        } else {
            if (leftExponent > rightExponent) {
                rightExponent = leftExponent;
                rightFraction = 0;
            } else {
                leftExponent = rightExponent;
                leftFraction = 0;
            }
        }
    }

    if (left->sign != right->sign) {
        if (left->sign) {
            signedFraction = -leftFraction + rightFraction;
        } else {
            signedFraction = leftFraction - rightFraction;
        }
        if (signedFraction >= 0) {
            result->sign = 0;
            result->exponent = leftExponent;
            result->fraction.whole = signedFraction;
        } else {
            result->sign = 1;
            result->exponent = leftExponent;
            result->fraction.whole = -signedFraction;
        }
        while (result->fraction.whole < SOFT_FLOAT_IMPLICIT_ONE &&
               result->fraction.whole) {
            result->fraction.whole <<= 1;
            result->exponent--;
        }
    } else {
        result->sign = left->sign;
        result->exponent = leftExponent;
        result->fraction.whole = leftFraction + rightFraction;
    }

    result->classification = SOFT_FLOAT_NUMBER;
    if (result->fraction.whole >= SOFT_FLOAT_IMPLICIT_TWO) {
        SOFT_FLOAT_SHIFT_RIGHT_STICKY(result->fraction.whole);
        result->exponent++;
    }
    return result;
