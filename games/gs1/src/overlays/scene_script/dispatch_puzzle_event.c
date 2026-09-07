#include "types.h"

#define Scene_DispatchPuzzleEvent Func_02000890

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Value_0000001e;
extern u8 Value_00000020;
extern u8 Value_00000023;
extern u8 Data_03001ebc[];
extern s16 Data_02000240[];
void Func_0200119c();
void Func_02001398();
void Func_020015de();
void Func_020018e2();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

s32 Scene_DispatchPuzzleEvent(void)
{
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x100;
    if (Data_02000240[224] == (s32)&Value_0000001e) {
        Func_0200119c();
    } else {
        if (Data_02000240[224] == (s32)&Value_00000023) {
            Func_02001398();
            Call2(Func_020018e2, 0x2008ed9, 0xc80);
        } else {
            if (Data_02000240[224] == (s32)&Value_00000020) {
                Func_020015de();
            }
        }
    }
    return 0;
}
