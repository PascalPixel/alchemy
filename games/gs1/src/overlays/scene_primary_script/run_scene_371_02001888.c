#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02005a30();
void Func_02005a54();
void Func_02005a7e();
void Func_02005b70();
s32 Func_02005b8a();
void Func_02005ba4();
void Func_02005bf2();
void Func_02005bfe();
void Func_02005c08();
void Func_02005c12();
void Func_02005c1e();
void Func_02005c9a();
void Func_02005cbc();
void Func_02005d18();
void Func_02005d22();
void Func_02005d4a();
void Func_02005d56();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene371_02001888(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005b8a, 8);
    Func_02005b70();
    Call4(Func_02005c9a, -1, -1, -1, 0);
    Func_02005a30(1);
    Func_02005c12(0, 0, 0);
    Call3(Func_02005c1e, 8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Func_02005a54(1);
    Func_02005cbc(8, 1);
    Func_02005d18();
    Call3(Func_02005bf2, 8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Func_02005c08(8, 0x200d158);
    do {
        Func_02005a7e(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Func_02005d4a();
    Func_02005d56();
    Call1(Func_02005ba4, 0x927);
    Func_02005d22(103);
    Func_02005bfe();
}
