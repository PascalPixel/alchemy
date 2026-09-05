#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define WORKSPACE (*(u8 **) 0x03001EBC)

#include "resource_38c.h"

extern u8 Data_03001ebc[];

void Func_02000574(s32);
u8 *Func_0200059a(s32);
void Func_02000580(void);
void Func_02000602(s32, s32);
s32 Func_02000582(s32);
void Func_020005dc(s32);
void Func_020005e4(s32);
void Func_020005fc(s32, s32);
void Func_020005c0(void);
u8 *Func_020005fa(s32);
void Func_020005e0(void);
s32 Func_020005e2(s32);
void Func_0200063c(s32);
void Func_02000644(s32);
void Func_0200065c(s32, s32);
void Func_02000620(void);
void Func_02000648();
s32 Func_0200064a();
void Func_02000674();
void Func_020006a4();
void Func_020006a6();
void Func_020006ac();
void Func_020006ca();
void Func_020006d0();
s32 Func_020006dc();
void Func_020006e8();
void Func_020006f0();
void Func_020006c4(void);
void Func_02000702(s32);
void Func_02000704(s32, s32, s32);
void Func_02000724(s32, s32);
void Func_02000748(s32, s32, s32);
void Func_020006f4(void);
void Func_020006fc();
void Func_02000714();
void Func_0200073a();
s32 Func_02000744();
void Func_02000746();
void Func_0200075c();
void Func_02000760();
s32 Func_02000764();
void Func_02000790();
void Func_020007b4();
void Func_0200076c();
void Func_0200077a();
s32 Func_020007aa();
void Func_020007ac();
void Func_020007c6();
s32 Func_020007ca();
void Func_020007f6();
void Func_020007d0(void);
void Func_0200080e(s32);
s32 Func_02000836(s32, s32);
void Func_020007ea(void);
void Func_020007f0(void);
void Func_0200082e(s32);
s32 Func_02000856(s32, s32);
void Func_0200080a(void);
u8 *Func_02000832(s32);
void Func_02000818(void);
void Func_020008a2(s32, s32);
void Func_02000874(s32);
void Func_02000884(s32);
void Func_0200089c(s32, s32);
void Func_02000860(void);
s32 Func_02000868();
void Func_02000878();
void Func_020008b6();
void Func_020008c2();
void Func_020008c4();
void Func_020008d6();
void Func_020008da();
void Func_020008ee();
void Func_020008fe();
void Func_02000904();
void Func_0200090e();
void Func_0200091c();
void Func_02000920();
void Func_02000934();
void Func_02000910(void);
void Func_0200094e(s32);
void Func_02000966(s32, s32);
void Func_02000978(s32, s32);
void Func_02000946(void);
void Func_0200094c(void);
void Func_0200098a(s32);
void Func_020009a2(s32, s32);
void Func_02000994(s32, s32, s32);
void Func_020009b4(s32, s32);
void Func_02000984(void);
void Func_0200098c(void);
void Func_020009ca(s32);
s32 Func_020009f2(s32, s32);
void Func_020009a6(void);
void Func_020009ac(void);
void Func_020009f4(s32);
void Func_020009fc(s32);

/*
 * resource_38c owner at 0x02000030, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000034 holding 0x2008598.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000030 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008598 is image offset
 * 0x598 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_38c owner at 0x02000038, 4 bytes: `movs r0, #0 / bx lr`.
 *
 * LEAF RESIDUE. Published at image offset 0x2c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 * A leaf never does -- it saves no register and returns with `bx lr`.
 *
 * Complete owner: both instructions. No prologue, no stack frame, no
 * literal pool, no callees, no argument read.
 *
 * One of the 70 rows sharing this exact body across the tree. The body is
 * shared; the identity is not -- this row is bounded by ITS overlay's
 * neighbours and published from ITS overlay's table.
 */

/* Contiguous unnamed leaf-owner run for resource_38c. */

/*
 * resource_38c owner at 0x0200003c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000040 holding 0x2008688.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200003c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008688 is image offset
 * 0x688 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/* Contiguous unnamed state-owner run for resource_38c. */

/*
 * resource_38c owner at 0x0200011c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000120 holding 0x20088f0.
 *
 * LEAF RESIDUE. Published at image offset 0x24; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200011c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20088f0 is image offset
 * 0x8f0 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Both arms reach this one name from sites 0x10 apart, so it stands for two
 * different veneers -- 0x570 from the first arm and 0x560 from the second. */

/* The workspace pointer the overlay's own rows all reach through. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    s32 Func_02000662();

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    s32 Func_02000662();

    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    s32 Func_02000662();

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    s32 Func_02000662();

    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    s32 Func_02000662();

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void SetScale(s32 actor, s32 scale, s32 duration)
{
    Func_02000748(actor, scale, duration);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    void Func_02000958();

    f(a0, a1);
}

static __inline__ void SetScale_020009d8(s32 actor, s32 scale, s32 duration)
{
    void Func_020009d8(s32, s32, s32);

    Func_020009d8(actor, scale, duration);
}

u8 *SceneData_GetPrimaryTable(void)
{
    return (u8 *)RESOURCE38C_PRIMARY_TABLE_ADDRESS;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetSecondaryTable(void)
{
    return (u8 *)RESOURCE38C_SECONDARY_TABLE_ADDRESS;
}

s32 SceneData_PrepareTable86b0(void) {
    Func_02000574(0x020086B0);
    return 0x020086B0;
}

void FieldScene_RunActor16MessageBranch(void)
{
    void Func_02000662(s32, s32);

    u32 dir;

    dir = *(u16 *) (Func_0200059a(0) + 6);
    Func_02000580();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000602(7, 16);
    } else {
        if (Func_02000582(0x845) == 0) {
            Func_020005dc(0x13E3);
        } else {
            Func_020005e4(0x16F5);
        }
        Func_020005fc(16, 0);
    }

    Func_020005c0();
}

void FieldScene_RunActor18MessageBranch(void)
{
    void Func_02000662(s32, s32);

    u32 dir;

    dir = *(u16 *) (Func_020005fa(0) + 6);
    Func_020005e0();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000662(9, 18);
    } else {
        if (Func_020005e2(0x845) == 0) {
            Func_0200063c(0x13E9);
        } else {
            Func_02000644(0x16F9);
        }
        Func_0200065c(18, 0);
    }

    Func_02000620();
}

u8 *SceneData_GetTertiaryTable(void)
{
    return (u8 *)RESOURCE38C_TERTIARY_TABLE_ADDRESS;
}

void FieldScene_RunActor17MessageBranch(void)
{
    s32 Func_02000662();

    u32 i;
    s32 record;
    u8 *dir;

    record = Value1(Func_02000662, 0);
    dir = *(volatile u16 *)(record + 6);
    Func_02000648();
    if ((u32)((s32)dir + -0xa001) <= 0x3ffe) {
        Func_020006ca(8, 17);
    } else {
        if (Value1(Func_0200064a, 0x845) == 0) {
            Call1(Func_020006a4, 0x13e5);
            Func_020006a6(17, 0, 0);
            Func_02000674(10);
            Value2(Func_020006dc, 17, 0);
            Call3(Func_020006f0, 17, 0x3000, 10);
        } else {
            Call1(Func_020006d0, 0x16f7);
            Func_020006e8(17, 0);
        }
    }
    Func_020006ac();
}

void FieldScene_ConfigureActor21Scene(void)
{
    Func_020006c4();
    Func_02000702(0x13ed);
    Func_02000704(21, 0, 0);
    Func_02000724(21, 0);
    SetScale(21, 0xc000, 10);
    Func_020006f4();
}

void FieldScene_RunActor24Sequence(void)
{
    u32 i;
    s32 record;

    Func_020006fc();
    Call1(Func_0200073a, 0x13f0);
    Func_0200075c(24, 0, 20);
    Func_02000746(24, 0, 0);
    Func_02000714(10);
    Value2(Func_02000764, 24, 0);
    if (Value2(Func_02000744, 0, 0) != 0) {
        bump_step(1);
    }
    Func_02000790(24, 0);
    Call3(Func_020007b4, 24, 0x4000, 10);
    Func_02000760();
}

void FieldScene_RunActor27Sequence(void)
{
    void Func_0200081a();

    u32 i;
    s32 record;

    Func_0200076c();
    Call1(Func_020007aa, 0x13f6);
    Func_020007ac(27, 0, 0);
    Func_0200077a(10);
    Value2(Func_020007ca, 27, 0);
    if (Value2(Func_020007aa, 0, 0) != 0) {
        bump_step(1);
    }
    Func_020007f6(27, 0);
    Call3(Func_0200081a, 27, 0x4000, 10);
    Func_020007c6();
}

void FieldScene_RunActor8Message(void)
{
    Func_020007d0();
    Func_0200080e(0x16E1);
    Func_02000836(8, 0);
    Func_020007ea();
}

void FieldScene_RunActor13Message(void)
{
    Func_020007f0();
    Func_0200082e(0x16EC);
    Func_02000856(13, 0);
    Func_0200080a();
}

void FieldScene_RunActor19MessageBranch(void)
{
    s32 Func_0200081a(s32);

    u32 dir;

    dir = *(u16 *) (Func_02000832(0) + 6);
    Func_02000818();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Func_020008a2(2, 19);
    } else if (Func_0200081a(0x845) != 0) {
        Func_02000874(0x16FB);
        Func_0200089c(19, 0);
    } else {
        Func_02000884(0x13EB);
        Func_0200089c(19, 0);
    }

    Func_02000860();
}

void FieldScene_RunActor21SequenceOnFlag300(void)
{
    void Func_02000958();

    u32 i;
    s32 record;

    Func_02000878();
    if (Value1(Func_02000868, 0x300) == 0) {
        Call1(Func_020008c2, 0x16ff);
        Func_020008da(21, 0);
        Call3(Func_020008fe, 21, 0x8000, 20);
        Func_020008ee(21, 0);
        Func_020008d6(22, 2);
        Call2(Func_02000920, 22, 0x102);
        Func_020008b6(60);
        Func_0200090e(22, 0);
        Func_020008c4(10);
        Call1(Func_020008c4, 0x300);
    }
    Func_0200090e(21, 0, 0);
    Call1(Func_0200091c, 0x1702);
    Func_02000934(21, 0);
    Call3(Func_02000958, 21, 0xc000, 10);
    Func_02000904();
}

void FieldScene_ConfigureActor22Scene(void)
{
    void Func_02000958(s32, s32, s32);
    void Func_0200099a(s32, s32, s32);

    Func_02000910();
    Func_0200094e(0x1703);
    Func_02000966(0x16, 0);
    Func_02000958(0x16, 0, 0);
    Func_02000978(0x16, 0);
    Func_0200099a(0x16, 0, 0xA);
    Func_02000946();
}

void FieldScene_ConfigureActor23Scene(void)
{
    void Func_020009d8(s32, s32, s32);

    Func_0200094c();
    Func_0200098a(0x1705);
    Func_020009a2(23, 0);
    Func_02000994(23, 0, 0);
    Func_020009b4(23, 0);
    SetScale_020009d8(23, 0xc000, 10);
    Func_02000984();
}

void FieldScene_RunActor27Message(void)
{
    s32 Func_0200099a(s32);
    void Func_020009d8(void);
    void Func_02000a14(s32, s32);

    Func_0200098c();
    Func_020009ca(0x170A);
    Func_020009f2(27, 0);
    Func_020009a6();
}

void FieldScene_RunActor10MessageBranch(void)
{
    s32 Func_0200099a(s32);
    void Func_020009d8(void);
    void Func_02000a14(s32, s32);

    Func_020009ac();
    if (Func_0200099a(3) != 0) {
        Func_020009f4(0x146F);
    } else {
        Func_020009fc(0x13D9);
    }
    Func_02000a14(10, 0);
    Func_020009d8();
}

s32 FieldScene_SetupActor27OnEntry(void)
{
    u8 *Func_02000a14(s32);

    u8 *actor;
    u8 *record;
    s32 bits;

    *(s32 *) (WORKSPACE + 448) = 521;
    actor = Func_02000a14(27);
    /* The stored zero is the mask's starting value: the reference builds
     * -13 by subtracting from the register the `strb` already set to 0
     * rather than materialising 0xf3 or negating 13. */
    actor[0x23] = bits = 0;
    record = *(u8 **) (actor + 0x50);
    bits -= 13;
    bits &= record[9];
    bits |= 8;
    record[9] = bits;
    return 0;
}
