#include "types.h"

#define FieldScene_RunScene3b9_020023e0 Func_020023e0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02004fa0();
s32 Func_02004fa2();
void Func_02004fb4();
void Func_02004fd0();
void Func_02004fd2();
void Func_02004ff4();
void Func_0200503a();
s32 Func_02005042();
void Func_02005044();
void Func_02005062();
s32 Func_0200506c();
void Func_02005090();
void Func_020050a2();
void Func_020050a6();
void Func_020050aa();
void Func_020050ac();
void Func_020050c2();
void Func_020050c8();
void Func_020050d6();
void Func_020050f0();
void Func_0200515a();
void Func_020051b6();
void Func_020051be();
void Func_020051ca();
void Func_020051d6();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b9_020023e0(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02004fa2, 5) != 0) {
        Call1(Func_02004fb4, 0x16d);
        Func_02004fd2(5);
        Func_02004fd0(3);
    }
    Func_02004ff4();
    Call3(Func_02005062, 11, 0x2c80000, 0x24c0000);
    Func_02004fa0(1);
    Func_020050f0(11, 1);
    Call3(Func_0200503a, 11, 0x19999, 0xcccc);
    Call3(Func_02005044, 0, 0x19999, 0xcccc);
    record = Func_02005042(11);
    {
        s32 shown = 0;
    
        *(volatile u16 *)(record + 6) = shown;
    }
    Func_0200515a();
    Func_020050a2(0, 2);
    Func_020050aa(11, 2);
    Call3(Func_02005090, 0, 0x30c, 0x24c);
    Call3(Func_020050a6, 11, 0x32c, 0x24c);
    Call3(Func_020050ac, 0, 0x370, 0x24c);
    Call3(Func_020050c2, 11, 0x390, 0x24c);
    Call3(Func_020050c8, 0, 0x3d4, 0x24c);
    Call3(Func_020050d6, 11, 0x3f4, 0x24c);
    Func_020051ca();
    Func_020051d6();
    if (Value1(Func_0200506c, 0x90f) != 0) {
        Func_020051b6(31);
    } else {
        Func_020051be(65);
    }
}
