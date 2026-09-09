#include "types.h"

#define FieldScene_RunCueBySceneSelector Func_02000c28

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001804();
void Func_02001844();
void Func_02004f1c();
void Func_02004f58();
void Func_02004f7e();
void Func_02004fd2();
void Func_020050ae();
void Func_020050d8();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunCueBySceneSelector(void)
{
    u32 i;
    s32 record;
    s32 v5;
    u8 *p6;

    p6 = *(volatile s32 *)Data_03001ebc;
    Func_02004f1c();
    v5 = 0;
    if (*(s16 *)(((s32)p6 + 0x16c)) != 1) {
        if (*(s16 *)(((s32)p6 + 0x16c)) != 3) {
            goto L_02000c56;
        }
        goto L_02000c50;
    } else {
        v5 = 1;
        Func_02001804();
        goto L_02000c56;
        L_02000c50:;
        v5 = 1;
        Func_02001844();
    }
    L_02000c56:;
    if (v5 != 0) {
        Call3(Func_02004f7e, 0, 0x9999, 0x4ccc);
        Call3(Func_02004fd2, 0, 1, -10);
        Func_02004f58(10);
    } else {
        Func_020050d8(123);
    }
    Func_020050ae(*(s16 *)(((s32)p6 + 0x16c)));
}
