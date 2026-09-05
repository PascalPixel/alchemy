#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020036a2();
void Func_02004950();
void Func_02004a74();
void Func_02004a88();
void Func_02004a98();
void Func_02004a9e();
void Func_02004aee();
void Func_02004afc();
void Func_02004b0a();
void Func_02004b28();
void Func_02004b30();
void Func_02004b3a();
void Func_02004b3c();
void Func_02004b42();
void Func_02004be0();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene3bb_02000b38(s32 a0)
{
    u32 i;
    s32 record;

    Func_02004a98(40);
    Func_02004a9e(41);
    Func_02004a74(1);
    Func_02004a88();
    Call3(Func_02004aee, 8, 0x580000, 0x1000000);
    Call3(Func_02004afc, 0, 0x780000, 0x1000000);
    Call3(Func_02004b30, 8, 0x4000, 0);
    Call3(Func_02004b3c, 0, 0x4000, 0);
    if (a0 < 0) {
        Func_02004b28(8, 10);
        Func_02004b30(0, 35);
    } else {
        Func_02004b3a(8, 8);
        Func_02004b42(0, 28);
    }
    Func_02004950(1);
    Call4(Func_02004be0, 0x680000, 0, 0xc00000, 0);
    Func_020036a2(a0);
    Func_02004b0a();
}
