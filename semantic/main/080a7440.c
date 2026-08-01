#include "types.h"

/*
 * ROM 0x080a7440..0x080a7478: ordinary Thumb leaf/call owner, reviewed from
 * out/decomp/diagnose/080a7440/080a7440.s.  It clears state halfword 186,
 * asks Func_080a77a4 to select an entry, and returns either its -1 sentinel
 * or state byte 0x21a.  src/080a7440.c pins registers/uses inline asm only
 * for byte matching; this is the canonical semantic expression.
 */
s32 Func_080a77a4(s32 selection);

s32 Func_080a7440(void)
{
    u8 *state = *(u8 **)0x03001f2c;
    s32 selection;

    *(s16 *)(state + 0x174) = 0;
    selection = Func_080a77a4(0);
    if (selection != -1) selection = state[0x21a];
    return selection;
}
