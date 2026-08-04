#include "types.h"

/*
 * resource_3b1 owner at 0x02001a08, 88 bytes.  `push {lr}` starts the
 * owner; `pop {r0} / bx r0` returns at 0x02001a4c-0x02001a4f.  The code is
 * already four-byte aligned at the pool boundary, so four pool words begin
 * immediately at 0x02001a50 and run through 0x02001a5f; the next owner's
 * `push {lr}` is 0x02001a60.
 */

extern void Func_02004880(s32 id, unsigned short value);
extern void Func_08015210(s32 id, s32 mode, s32 arg2);
extern s32 Func_080770c0(s32 flag);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1d8(s32 id);

void Func_02001a08(void)
{
    if (Func_080770c0(0x301) == 0)
        return;

    Func_0808a018();
    Func_0808a1d8(8);
    Func_08015210(0, 0x198, 0x86);
    Func_0808a090(0x1e48, 1, 8);
    Func_0808a0d0(0, 0x19999, 0xcccc);
    Func_02004880(0, 0x4000);
    Func_0808a020();
}
