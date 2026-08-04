#include "types.h"

struct ComparisonStats_080a15f0 {
    u8 padding_00[0x3c];
    u16 first;
    u16 second;
    u16 third;
};

void Func_08015080(s32 message, s32 window, s32 style, s32 y);
void Func_080150a8(s32 value, s32 digits, s32 window, s32 x, s32 y);
void Func_080ae99c(s32 window, s32 x, s32 y, s32 direction);

void Func_080a15f0(
    const struct ComparisonStats_080a15f0 *before,
    const struct ComparisonStats_080a15f0 *after,
    s32 window)
{
    Func_08015080(0xb1c, window, 0, 0x20);
    Func_080150a8(after->first, 3, window, 0x10, 0x28);
    if (before->first != after->first) {
        Func_080150a8(before->first, 3, window, 0x40, 0x28);
        Func_080ae99c(
            window, 0x2c, 0x24, before->first > after->first ? 0 : 1);
    }

    Func_08015080(0xb1d, window, 0, 0x30);
    Func_080150a8(after->second, 3, window, 0x10, 0x38);
    if (before->second != after->second) {
        Func_080150a8(before->second, 3, window, 0x40, 0x38);
        Func_080ae99c(
            window, 0x2c, 0x34, before->second > after->second ? 0 : 1);
    }

    Func_08015080(0xb20, window, 0, 0x40);
    Func_080150a8(after->third, 3, window, 0x10, 0x48);
    if (before->third != after->third) {
        Func_080150a8(before->third, 3, window, 0x40, 0x48);
        Func_080ae99c(
            window, 0x2c, 0x44, before->third > after->third ? 0 : 1);
    }
}
