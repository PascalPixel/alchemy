#ifndef GS1_SOFT_FLOAT_ADD_PARTS_RECORD_H
#define GS1_SOFT_FLOAT_ADD_PARTS_RECORD_H

typedef unsigned int SoftFloatHalfFraction __attribute__((mode(SI)));
typedef unsigned int SoftFloatFraction __attribute__((mode(DI)));
typedef int SoftFloatSignedFraction __attribute__((mode(DI)));

enum SoftFloatClass {
    SOFT_FLOAT_SIGNALING_NAN,
    SOFT_FLOAT_QUIET_NAN,
    SOFT_FLOAT_ZERO,
    SOFT_FLOAT_NUMBER,
    SOFT_FLOAT_INFINITY
};

typedef struct SoftFloatRecord {
    enum SoftFloatClass classification;
    unsigned int sign;
    int exponent;
    union {
        SoftFloatFraction whole;
        SoftFloatHalfFraction halves[2];
    } fraction;
} SoftFloatRecord;

#define SOFT_FLOAT_FRACTION_BITS 64
#define SOFT_FLOAT_GUARD_BITS 8L
#define SOFT_FLOAT_VALUE_BITS 52
#define SOFT_FLOAT_IMPLICIT_ONE \
    (1LL << (SOFT_FLOAT_VALUE_BITS + SOFT_FLOAT_GUARD_BITS))
#define SOFT_FLOAT_IMPLICIT_TWO \
    (1LL << (SOFT_FLOAT_VALUE_BITS + 1 + SOFT_FLOAT_GUARD_BITS))
#define SOFT_FLOAT_SHIFT_RIGHT_STICKY(value) \
    { (value) = ((value) & 1) | ((value) >> 1); }

#endif
