#include "types.h"

#define Scene_DispatchPuzzleEvent Func_02000890
#define Scene_UpdatePuzzleActors Func_020009cc

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
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02001a82();
void Func_02001a96();
void Func_02001aa8();
s32 Func_02001aac();
s32 Func_02001ab4();
void Func_02001aba();
void Func_02001ad6();
void Func_02001b26();
void Func_02001b60();
void Func_02001b82();
void Func_02001b8c();
void Func_02001b9c();
void Func_02001baa();
void Func_02001bda();
void Func_02001bfe();
void Func_02001c18();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
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

void Scene_UpdatePuzzleActors(void)
{
    s32 p10;
    s32 p9;
    s32 rec7;
    s32 record;
    s32 p6;
    s32 row;

    rec7 = Value1(Func_02001aac, 0);
    record = Value1(Func_02001ab4, 20);
    row = *(s32 *)(record + 16) >> 20;
    p9 = (*(s32 *)(rec7 + 8) >> 20);
    p10 = (*(s32 *)(rec7 + 16) >> 20);
    p6 = *(s32 *)(record + 8);
    Call6(Func_02001a82, 15, 11, 3, 1, 15, 12);
    Call6(Func_02001a96, 15, 11, 3, 1, 15, 13);
    Call6(Func_02001aa8, 15, 11, 3, 1, 15, 14);
    Call6(Func_02001aba, 1, 0, 1, 1, (p6 >> 20), row);
    if (((s32)p6 >> 20) == 16) {
        if (row == 13) {
            goto L_02000a60;
        }
    }
    Call6(Func_02001ad6, 0, 0, 1, 1, 16, 13);
    L_02000a60:;
    if (p9 == 16) {
        if (p10 == 13) {
            Func_02001b26();
            Call3(Func_02001bda, 0, 0x100, 20);
            Call3(Func_02001b60, 0, 0x20000, 0x10000);
            Func_02001baa(0, 6, 0);
            if (row == 13) {
                Call3(Func_02001b82, 0, 0x106, 196);
                Call3(Func_02001bfe, 0, 0x4000, 20);
            } else {
                Call3(Func_02001b9c, 0, 0x11e, 218);
                Call3(Func_02001c18, 0, 0x8000, 20);
            }
            Func_02001b8c();
        }
    }
}
