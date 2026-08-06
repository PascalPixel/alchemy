#include "types.h"

/*
 * Apply the current world-event selection.
 *
 * The owner starts with `push {lr}` at 0x02001894 and returns through
 * `pop {r0} / bx r0` at 0x0200194e-0x02001950.  The halfword at 0x02001952 is
 * alignment, followed by the nine literal words at 0x02001954-0x02001977;
 * the next owner's `push {lr}` is 0x02001978.  The complete measured span is
 * therefore 0x02001894-0x02001977 (228 bytes).
 *
 * The first test is an unsigned fixed-point comparison: the record state is
 * adjusted by 0xffffe000, shifted into the high halfword, and compared with
 * 0xc0000000.  Keeping the arithmetic in u32 preserves the machine's wrap
 * and unsigned `bls` condition.
 */









extern u8 * Func_02007d82(s32 id);
extern s32 Func_02007d50(s32 flag);
extern s32 Func_02007d5a(s32 flag);
extern void Func_02007f1c(s32 value);
extern void Func_02007f24(s32 value);
extern void Func_02007d9a(void);
extern s32 Func_02007d78(s32 flag);
extern void Func_02007e8a(s32 value);
extern s32 Func_02007d8c(s32 flag);
extern void Func_02007e9e(s32 value);
extern s32 Func_02007d9e(s32 flag);
extern void Func_02007eb0(s32 value);
extern s32 Func_02007db0(s32 flag);
extern void Func_02007ec2(s32 value);
extern void Func_02007eca(s32 value);
extern s32 Func_02007dc8(s32 flag);
extern s32 Func_02007dd2(s32 flag);
extern void Func_02007ef6(s32 value, s32 mode);
extern void Func_02007f00(s32 value, s32 mode);
extern void Func_02007e1c(void);
void Func_02001894(void)
{
    u8 *record = Func_02007d82(0);
    u32 adjustedState = ((u32)*(u16 *)(record + 6) + 0xffffe000u) << 16;

    if (adjustedState > 0xc0000000u) {
        if (Func_02007d50(0x928) != 0 &&
            Func_02007d5a(0x93e) == 0) {
            Func_02007f1c(17);
        } else {
            Func_02007f24(15);
        }
    } else {
        Func_02007d9a();

        if (Func_02007d78(0x93e) != 0) {
            Func_02007e8a(0x1f81);
        } else if (Func_02007d8c(0x8a0) != 0) {
            Func_02007e9e(0x1f48);
        } else if (Func_02007d9e(0x925) != 0) {
            Func_02007eb0(0x1f7f);
        } else if (Func_02007db0(0x1f7d) != 0) {
            Func_02007ec2(0x1f7d);
        } else {
            Func_02007eca(0x1f7b);
        }

        if (Func_02007dc8(0x928) != 0 &&
            Func_02007dd2(0x93e) == 0) {
            Func_02007ef6(17, 0);
        } else {
            Func_02007f00(15, 0);
        }
    }

    Func_02007e1c();
}
