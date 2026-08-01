/*
 * VENEER AUDIT NOTE (2026-08-01) -- COMMENT ONLY, NO CODE CHANGE.
 *
 * This file is byte-exact, so nothing below is rewritten.  The note records
 * what the call sites actually are, so the next reader is not misled.
 *
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank: fifteen four-byte
 * `bx rN; nop` entries in register order r0..lr, ending at 0x08007320.  A
 * `bl` into that range is an INDIRECT CALL through the named register, not a
 * call to a function at the branch target.  The `Func_080072f*` prototype
 * this file declares is therefore a phantom, and the register load that
 * precedes each site -- which reads like dead code -- is the callee load.
 *
 * Why the file is still byte-identical while being wrong: a direct call to a
 * declared function at 0x080072f0 emits exactly the same `bl` the real
 * indirect call emits.  Converting to a function-pointer call would require
 * the compiler to choose the same register and therefore the same veneer
 * entry, which is a byte-exact source question and is deliberately NOT attempted
 * here.
 *
 * Sites in this owner, resolved with tools/veneer_resolve.ts:
 *
 *   0x0809041a  __call_via_r3  ->  0x03000380
 *     a relocated IWRAM routine. NOT established
 */
#include "types.h"

struct State_080903bc {
    u8 pad_000[0x52a];
    u16 value_52a;
    u8 pad_52c[14];
    s8 start_53a;
    s8 end_53b;
    s8 duration_53c;
    s8 step_53d;
};

struct Display_080903bc {
    u8 pad_000[0x100];
    u16 first_100;
    u16 second_102;
};

extern void Func_08004278(void (*)(void));
extern void Func_0800307c(s32, s32, s32);
extern s32 Func_080072f0(s32, s32, s32, s32);

void Func_080903bc(void)
{
    struct State_080903bc *state =
        *(struct State_080903bc **)0x03001ecc;
    struct Display_080903bc *display =
        *(struct Display_080903bc **)0x03001e70;
    s8 *duration = &state->duration_53c;
    u32 displayValue;

    if (*duration != 0) {
        s8 *step = &state->step_53d;

        if (*step >= *duration) {
            *duration = 0;
            Func_08004278(Func_080903bc);
            Func_0800307c(1, 0, 0);
            return;
        } else {
            s32 delta = state->end_53b - state->start_53a;
            s32 value;

            (*step)++;
            value = Func_080072f0(
                delta * *step, *duration, delta, 0x03000380);
            state->value_52a = state->start_53a + value;
        }
    }

    displayValue = state->value_52a;
    if (displayValue > 79) {
        display->first_100 = 200;
        display->second_102 = 250;
    } else {
        display->first_100 = displayValue;
        display->second_102 = 159 - displayValue;
    }
}
