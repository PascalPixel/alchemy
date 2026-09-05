#include "types.h"

/*
 * Double-precision packer from the GCC soft-float runtime shape.  This overlay
 * copy calls the shared runtime predicates and shift helper at its witnessed
 * targets.  The final word swap is the runtime's FLOAT_WORD_ORDER_MISMATCH
 * path and preserves the packed-word order observed by its callers.
 */
typedef u64 SoftDouble;

enum SoftFloatClass {
    CLASS_SNAN,
    CLASS_QNAN,
    CLASS_ZERO,
    CLASS_NUMBER,
    CLASS_INFINITY
};

typedef struct SoftFloatRecord {
    enum SoftFloatClass cls;
    u32 sign;
    s32 exponent;
    union {
        u64 ll;
        u32 words[2];
    } fraction;
} SoftFloatRecord;

typedef union SoftDoubleUnion {
    SoftDouble value;
    u64 raw;
    u32 words[2];
    struct {
        u64 fraction : 52 __attribute__((packed));
        u32 exponent : 11 __attribute__((packed));
        u32 sign : 1 __attribute__((packed));
    } bits;
} SoftDoubleUnion;

s32 Func_0200ba24(SoftFloatRecord *record);
s32 Func_0200ba4c(SoftFloatRecord *record);
s32 Func_0200ba66(SoftFloatRecord *record);
u64 Func_0200b8c2(u64 value, u32 count);

#define PackSoftDouble Func_02005c38
SoftDouble PackSoftDouble(SoftFloatRecord *src)
{
    SoftDoubleUnion dst;
    u64 fraction = src->fraction.ll;
    s32 sign = (s32)src->sign;
    s32 exp = 0;

    if (Func_0200ba24(src)) {
        exp = 0x7ff;
        if (src->cls == CLASS_QNAN || 1) {
            fraction |= 0x8000000000000ULL;
        }
    } else if (Func_0200ba4c(src)) {
        exp = 0x7ff;
        fraction = 0;
    } else if (Func_0200ba66(src)) {
        exp = 0;
        fraction = 0;
    } else if (fraction == 0) {
        exp = 0;
    } else {
        if (src->exponent < -1022) {
            s32 shift = -1022 - src->exponent;

            exp = 0;
            if (shift > 56) {
                fraction = 0;
            } else {
                s32 lowbit = (fraction & ((1 << shift) - 1)) ? 1 : 0;
                fraction = Func_0200b8c2(fraction, (u32)shift) | (u32)lowbit;
            }
            if ((fraction & 0xff) == 0x80) {
                if (fraction & (1 << 8)) {
                    fraction += 0x80;
                }
            } else {
                fraction += 0x7f;
            }
            if (fraction >= (1ULL << 60)) {
                exp += 1;
            }
            fraction >>= 8;
        } else if (src->exponent > 1023) {
            exp = 0x7ff;
            fraction = 0;
        } else {
            exp = src->exponent + 1023;
            if ((fraction & 0xff) == 0x80) {
                if (fraction & (1 << 8)) {
                    fraction += 0x80;
                }
            } else {
                fraction += 0x7f;
            }
            if (fraction >= (1ULL << 61)) {
                fraction >>= 1;
                exp += 1;
            }
            fraction >>= 8;
        }
    }

    dst.bits.fraction = fraction;
    dst.bits.exponent = (u32)exp;
    dst.bits.sign = (u32)sign;

    {
        u32 tmp = dst.words[0];
        dst.words[0] = dst.words[1];
        dst.words[1] = tmp;
    }

    return dst.value;
}
