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
#include "gs1_edition.h"

#if defined(GS1_EDITION_DE)
#define STATE_CELL_ADDR 0x03001EDC
#define DISPLAY_CELL_ADDR 0x03001E80
#else
#define STATE_CELL_ADDR 0x03001ECC
#define DISPLAY_CELL_ADDR 0x03001E70
#endif

struct DisplayTransitionState {
    u8 pad_000[0x52a];
    u16 transition_value;
    u8 pad_52c[14];
    s8 transition_start;
    s8 transition_end;
    s8 transition_duration;
    s8 transition_step;
};

struct DisplayTransitionRegisters {
    u8 pad_000[0x100];
    u16 primary_value;
    u16 secondary_value;
};

extern s32 ScheduleCallback(void (*)(void));
extern void Func_0800307c(s32, s32, s32);
extern s32 Func_080072f0(s32, s32, s32, s32);

void DisplayTransition_Update(void)
{
    struct DisplayTransitionState *state =
        *(struct DisplayTransitionState **)STATE_CELL_ADDR;
    struct DisplayTransitionRegisters *display =
        *(struct DisplayTransitionRegisters **)DISPLAY_CELL_ADDR;
    s8 *duration = &state->transition_duration;
    u32 display_value;

    if (*duration != 0) {
        s8 *step = &state->transition_step;

        if (*step >= *duration) {
            *duration = 0;
            ScheduleCallback(DisplayTransition_Update);
            Func_0800307c(1, 0, 0);
            return;
        } else {
            s32 delta = state->transition_end - state->transition_start;
            s32 value;

            (*step)++;
            value = Func_080072f0(
                delta * *step, *duration, delta, 0x03000380);
            state->transition_value = state->transition_start + value;
        }
    }

    display_value = state->transition_value;
    if (display_value > 79) {
        display->primary_value = 200;
        display->secondary_value = 250;
    } else {
        display->primary_value = display_value;
        display->secondary_value = 159 - display_value;
    }
}
