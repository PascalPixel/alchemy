#include "types.h"

#define FieldScene_RunScene3b8_02003d40 Func_02003d40

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020080e0();
void Func_020080fe();
void Func_02008118();
void Func_02008128();
void Func_0200814c();
void Func_02008152();
void Func_02008156();
void Func_02008158();
void Func_02008164();
void Func_02008174();
void Func_0200818e();
void Func_020081a0();
void Func_020081c4();
void Func_020081d2();
void Func_020081d4();
void Func_020081e8();
void Func_02008202();
void Func_02008296();
void Func_020082b6();

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

void FieldScene_RunScene3b8_02003d40(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    Func_02008128();
    Func_02008296(158);
    Call3(Func_02008164, 0, 0x8000, 0x4000);
    Func_020081c4(0, 2);
    if (*(s16 *)(((s32)p5 + 0x16c)) == 32) {
        Func_020080e0(1);
        Func_02008156(10);
        Call3(Func_020081d2, 0, 0, -16);
    } else {
        if (*(s16 *)(((s32)p5 + 0x16c)) == 30) {
            Func_020080fe(4);
            Func_02008174(10);
            Call3(Func_020081e8, 0, 3, -16);
        } else {
            Func_02008118(2);
            Func_0200818e(10);
            Call3(Func_02008202, 0, 3, -16);
        }
    }
    Func_020081a0(16);
    Func_020082b6(*(s16 *)(((s32)p5 + 0x16c)));
    Func_0200814c(1);
    Func_02008152(2);
    Func_02008158(4);
    Func_020081d4();
}
