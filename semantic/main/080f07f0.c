/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * Callee signatures here are established, not guessed: 0x03001388 is the
 * word copy declared in the EXACT src/080d40ec.c, and 0x03000168 is the fill
 * documented in semantic/main/080e15e8.c as (destination, size, value).
 */
#include "types.h"

typedef void *(*WordCopy)(void *destination, const void *source, s32 size);
typedef void (*ArmFill)(void *destination, u32 size, u32 value);

#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08002df0(void *);
u8 *Func_08004970(s32);
s32 Func_080770c0(s32);
void Func_080770c8(s32);

s32 Func_080f07f0(u8 *arg0, s32 arg1, s32 arg2) {
    s32 sp0;
    u8 *sp4;
    s32 sp8;
    u8 *var_r0_3;
    u8 *var_r1_2;
    u8 *var_r2_2;
    u8 *var_r4_3;
    s32 temp_r2;
    s32 var_ip;
    s32 var_ip_2;
    s32 var_ip_3;
    s32 var_r1_3;
    s32 var_r5;
    s32 var_r5_2;
    s32 var_r8;
    s32 var_r8_2;
    s8 *var_r1;
    s8 *var_r2;
    u32 temp_r3;
    u32 var_r6;
    u8 *var_r4;
    u8 *var_r4_2;
    u8 *var_sl;
    u8 temp_r7;
    u8 var_r0;
    u8 var_r0_2;
    u8 var_r3;

    sp8 = arg1;
    sp4 = Func_08004970(0x900);
    sp0 = 0;
    if (arg0 == NULL) {
        return -1;
    }
    if (Func_080770c0(0x200) == 0) {
        ((ArmFill)0x03000168)(sp4, 0x900, 0);
        Func_080770c8(0x200);
    } else {
        ((WordCopy)0x03001388)(sp4, sp4 + 0x800, 0x100);
        ((ArmFill)0x03000168)(sp4 + 0x100, 0x800, 0);
    }
    var_r0 = *arg0;
    var_r8 = 0;
    var_r4 = arg0 + 1;
    if (var_r0 != 0) {
        do {
            if ((u32) var_r0 > 0x1FU) {
                var_r8 += M2C_FIELD((var_r0 - 0x20), u8 *, 0x080F11BD);
            }
            var_r0 = *var_r4;
            var_r4 += 1;
        } while (var_r0 != 0);
    }
    if (arg2 == 2) {
        sp0 = 0xC0 - var_r8;
    } else if (arg2 == 1) {
        temp_r3 = 0xC0 - var_r8;
        sp0 = (s32) (temp_r3 + (temp_r3 >> 0x1F)) >> 1;
    }
    var_r0_2 = *arg0;
    var_r8_2 = 0;
    var_sl = arg0 + 1;
    if (var_r0_2 != 0) {
        do {
            if ((u32) var_r0_2 > 0x1FU) {
                temp_r2 = var_r0_2 - 0x20;
                var_r4_2 = (u8 *)0x080F1770 + (temp_r2 * 8);
                var_r1 = sp4 + sp0 + var_r8_2;
                var_ip = 0;
                do {
                    temp_r7 = *var_r4_2;
                    var_r6 = 0x80;
                    var_r4_2 += 1;
                    var_r5 = 7;
                    var_r2 = var_r1 + 0x101;
loop_18:
                    if (temp_r7 & var_r6) {
                        *var_r2 = 1;
                        *var_r1 = 0xF;
                    }
                    var_r5 -= 1;
                    var_r2 += 1;
                    var_r1 += 1;
                    var_r6 = var_r6 >> 1;
                    if (var_r5 >= 0) {
                        goto loop_18;
                    }
                    var_ip += 1;
                    var_r1 += 0xF8;
                } while (var_ip <= 7);
                var_r3 = 1;
                if ((u32) var_r0_2 > 0x1FU) {
                    var_r3 = M2C_FIELD(temp_r2, u8 *, 0x080F11BD);
                }
                var_r8_2 += var_r3;
            }
            var_r0_2 = *var_sl;
            var_sl += 1;
        } while (var_r0_2 != 0);
    }
    var_r4_3 = sp4;
    var_r1_2 = var_r4_3;
    var_ip_2 = 7;
    do {
        if (0x60 != 0) {
            var_r5_2 = 0x60;
            var_r2_2 = var_r4_3;
            do {
                var_r5_2 -= 1;
                *var_r1_2 = (s8) (M2C_FIELD(var_r2_2, u8 *, 0) | (M2C_FIELD(var_r2_2, u8 *, 1) * 0x10));
                var_r2_2 += 2;
                var_r4_3 += 2;
                var_r1_2 += 1;
            } while (var_r5_2 != 0);
        }
        var_r1_2 = (var_r1_2 - 0x60) + 0x100;
        var_r4_3 = (var_r4_3 - 0xC0) + 0x100;
        var_ip_2 -= 1;
    } while (var_ip_2 >= 0);
    if (0x18 != 0) {
        var_r0_3 = sp4;
        var_r1_3 = sp8 << 5;
        var_ip_3 = 0x18;
        do {
            M2C_FIELD(var_r1_3, s32 *, 0x06010000) = (s32) M2C_FIELD(var_r0_3, s32 *, 0);
            M2C_FIELD(var_r1_3, s32 *, 0x06010004) = (s32) M2C_FIELD(var_r0_3, s32 *, 0x100);
            M2C_FIELD(var_r1_3, s32 *, 0x06010008) = (s32) M2C_FIELD(var_r0_3, s32 *, 0x200);
            M2C_FIELD(var_r1_3, s32 *, 0x0601000C) = (s32) M2C_FIELD(var_r0_3, s32 *, 0x300);
            M2C_FIELD(var_r1_3, s32 *, 0x06010010) = (s32) M2C_FIELD(var_r0_3, s32 *, 0x400);
            M2C_FIELD(var_r1_3, s32 *, 0x06010014) = (s32) M2C_FIELD(var_r0_3, s32 *, 0x500);
            M2C_FIELD(var_r1_3, s32 *, 0x06010018) = (s32) M2C_FIELD(var_r0_3, s32 *, 0x600);
            M2C_FIELD(var_r1_3, s32 *, 0x0601001C) = (s32) M2C_FIELD(var_r0_3, s32 *, 0x700);
            var_ip_3 -= 1;
            var_r1_3 += 0x20;
            var_r0_3 += 4;
        } while (var_ip_3 != 0);
    }
    Func_08002df0(sp4);
    return 0;
}
