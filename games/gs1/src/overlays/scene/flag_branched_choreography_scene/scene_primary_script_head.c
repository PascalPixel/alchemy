#include "types.h"

#define FieldScene_RunScene38d_02000150 Func_02000150
#define FieldScene_RunScene38dSequenceA Func_020001b4
#define FieldScene_RunActorSeventeenFlaggedDialogue Func_02000214
#define FieldScene_RunScene38d_02000568 Func_02000568
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];

void Func_020025c4();
void Func_020025d6();
void Func_020025f6();
void Func_02002620();
void Func_02002688();
void Func_020026ac();
void Func_020026b0();
void Func_020026c8();
void Func_020026ce();
void Func_020026e8();
void Func_02002704();
s32 Func_02002614();
void Func_02002628();
void Func_02002634();
void Func_02002658();
void Func_020026f6();
void Func_02002706();
void Func_0200271e();
s32 Func_02002680();
void Func_02002772();
void Func_0200277a();
s32 Func_02002698();
s32 Func_020029be();
s32 Func_020029c8();
void Func_020029f0();
void Func_02002a0e();
void Func_02002a56();
void Func_02002a62();
void Func_02002aa6();
void Func_02002ac0();
void Func_02002ae4();
void Func_02002af4();
void Func_02002af8();
void Func_02002afc();
void Func_02002b3a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void Func_02002688_a();           /* scripted-scene bracket: open */

s32 Func_0200266e();            /* story-flag test; used in a condition */

void Func_02002760();           /* show a dialogue line by id */

void Func_020027ac();           /* wait for the slot's action to finish */

void Func_020026e0();           /* scripted-scene bracket: close */

void FieldScene_RunScene38d_02000150(void)
{
    u32 i;
    s32 record;

    Func_020025c4();
    Call3(Func_020026c8, 14, 0x102, 0);
    Func_02002688(14, 2);
    Func_020025d6(40);
    Call1(Func_020026ac, 0x1764);
    Func_020026ce(14, 0, 20);
    Func_020026b0(14, 0, 0);
    Func_020025f6(20);
    Func_020026e8(14, 0, 10);
    Call3(Func_02002704, 14, 0xb000, 10);
    Func_02002620();
}

void FieldScene_RunScene38dSequenceA(void)
{
    u32 i;
    s32 record;

    Func_02002628();
    Call1(Func_020026f6, 0x1750);
    if (Value1(Func_02002614, 0x302) != 0) {
        Call1(Func_02002706, 0x1768);
    }
    Func_0200271e(15, 0);
    Call1(Func_02002634, 0x302);
    Func_02002658();
}

void FieldScene_RunActorSeventeenFlaggedDialogue(void)
{
    u8 *work;

    Func_02002688_a();

    if (Func_0200266e(0x202) != 0) {
        Func_02002760(0x174b);
    } else if (Func_02002680(0x84e) != 0) {
        Func_02002772(0x176e);
    } else {
        Func_0200277a(0x1432);
        if (Func_02002698(0x84d) != 0) {
            work = *(u8 **)0x03001ebc;
            *(u16 *)(work + 472) = (u16)(*(u16 *)(work + 472) + 1);
        }
    }

    Func_020027ac(17, 0);
    Func_020026e0();
}

void FieldScene_RunScene38d_02000568(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_020029be, 0x84e) == 0) {
        if (Value1(Func_020029c8, 0x322) != 0) {
            Func_020029f0();
            Call3(Func_02002af4, 19, 0x100, 0);
            Call3(Func_02002af8, 19, 0x7000, 10);
            Func_02002ac0(19, 2);
            Func_02002a0e(20);
            Call1(Func_02002ae4, 0x1748);
            Func_02002afc(19, 0);
            Call3(Func_02002a62, 0, 0x10000, 0x8000);
            Call3(Func_02002aa6, 0, 0x268, 0x2fa);
            Call3(Func_02002b3a, 19, 0xd000, 10);
            Func_02002a56();
        }
    }
}
