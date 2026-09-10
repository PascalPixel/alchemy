#include "types.h"

#define SceneState_RunRepaintSequenceWithWaits Func_020000d4
#define SceneActor_MarkSlot10AndRunFlag202Step Func_020001b0
#define SceneState_ApplyFlag203 Func_02000200
extern u8 Data_03001ebc[];

u8 *Func_02001c18();
void Func_02001c5a();
void Func_02001be6();
void Func_02001bf2();
void Func_02001be8();
void Func_02001c0c();
void Func_02001c1e();
void Func_02001c30();
void Func_02001c26();
void Func_02001c46();
void Func_02001c3c();
void Func_02001c42();
void Func_02001c6c();
void Func_02001c80();
void Func_02001c92();
void Func_020002bc();
void Func_02001cc0();
void Func_02001ccc();
s32 Func_02001cde();
s32 Func_02001cf2();
void Func_02001d00();
void Func_02001e10();
void Func_02001e1a();
void Func_02001d12(s32);

/*
 * Overlay resource_3ad. Map repaint sequence: mark the record for slot ten,
 * then run the tile and collision repaints with frame waits between them.
 */

/* Old-style declarations: overlay import arities vary per call site. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/*
 * The owner runs from 0x020000d4 for 220 bytes and holds no literal pool.
 * Two of the calls spell their trailing argument pair as locals; that is how
 * the reference puts those two values on the stack, so keep the spelling.
 * Every call here goes through an import veneer in this overlay.
 */
void SceneState_RunRepaintSequenceWithWaits(void)
{
    u8 *obj;
    int row;           /* Assigned at its first use, as the reference does. */

    obj = Func_02001c18(10);
    Func_02001c5a(10, 5);

    if (obj != 0) {
        Func_02001be6(obj, 0);
        obj[0x23] = 1;
    }

    row = 21;
    Func_02001bf2(41, 87, 2, 5, row, 59);
    Func_02001be8(4);

    { s32 f1 = 24; s32 g1 = 62; Func_02001c0c(3, 93, 1, 1,  f1, g1); }
    Func_02001c1e(1, 94, 1, 1, row, 55);
    Func_02001c30(43, 87, 2, 5, row, 58);
    Func_02001c26(4);

    Func_02001c46(41, 87, 2, 5, row, 58);
    Func_02001c3c(4);
    Func_02001c42(4);

    Func_02001c6c(21, 11, 2, 2, row, 13);
    { s32 f2 = 22; s32 g2 = 15; Func_02001c80(21, 11, 1, 1,  f2, g2); }
    Func_02001c92(19, 17, 1, 1, row, 14);
}

void SceneActor_MarkSlot10AndRunFlag202Step(void)
{
    u32 i;
    u8 *rec7;
    s32 record;

    rec7 = Value1(Func_02001cf2, 10);
    Call1(Func_02001ccc, 0x200);
    if ((s32)rec7 != 0) {
        Func_02001cc0((s32)rec7, 0);
        rec7[35] = 1;
    }
    if (Value1(Func_02001cde, 0x202) == 0) {
        Func_02001e10(157);
        Func_020002bc();
        Func_02001e1a(80);
        Call1(Func_02001d00, 0x202);
    }
}

void SceneState_ApplyFlag203(void)
{
    Func_02001d12(0x203);
}
