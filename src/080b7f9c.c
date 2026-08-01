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
 *   0x080b7fec  __call_via_r3  ->  0x03000250
 *     a relocated IWRAM routine. NOT established
 */
#include "types.h"

struct State_080b7f9c {
    u8 filler0[12];
    s32 field0c;
    s32 field10;
    s32 field14;
    s32 field18;
    s32 field1c;
    s32 field20;
    u8 filler24[16];
    s16 field34;
    s16 field36;
};

struct Local_080b7f9c {
    s32 first;
    s32 second;
    s32 third;
};

extern struct State_080b7f9c *Data_03001e80;

void Func_080049ac(void);
void Func_08004cb4(void *);
void Func_08004c1c(s32);
void Func_08004bd4(s32);
void Func_080072f0(struct Local_080b7f9c *, struct State_080b7f9c *);

void Func_080b7f9c(void) {
    struct State_080b7f9c *state = Data_03001e80;
    struct Local_080b7f9c transfer;

    state->field36 = 192 << 6;
    state->field34 = 254 << 8;
    state->field20 = 255 << 17;
    state->field0c = 0;
    state->field10 = 0;
    state->field14 = 0;
    state->field1c = 0;
    state->field18 = 0;

    Func_080049ac();
    Func_08004cb4(&state->field0c);
    Func_08004c1c(state->field36);
    Func_08004bd4(state->field34);

    transfer.first = 0;
    transfer.second = 0;
    transfer.third = state->field20;
    ((void (*)(struct Local_080b7f9c *, struct State_080b7f9c *))0x03000250)(&transfer, state);
}
