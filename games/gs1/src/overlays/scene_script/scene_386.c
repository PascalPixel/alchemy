#include "types.h"

#define WORKSPACE (*(u8 **)0x03001ebc)

#include "resource_386_state.h"

extern u8 Data_03001ebc[];
extern s16 Data_02000240[];

void Func_020006d4(u8 *);
void Func_020006da(s32);
void Func_0200071a(s32, s32);
void Func_02000752(s32, s32);
void Func_020006f6(void);
void Func_02000756(s32);
void Func_02000758(s32, s32, s32);
void Func_0200016a(s32);
void Func_02000776(s32);
void Func_02000778(s32, s32, s32);
void Func_0200018a(s32);
void Func_02000796(s32);
void Func_02000798(s32, s32, s32);
void Func_020001aa(s32);
void Func_0200075c();
void Func_02000784();
void Func_020007ac();
void Func_020007ba();
void Func_020007bc();
void Func_020007c2();
void Func_020007c6();
void Func_020007c8();
s32 Func_020007dc();
void Func_020007ee();
void Func_020007f4();
void Func_02000806();
s32 Func_02000814();
void Func_0200081c();
void Func_02000822();
void Func_0200084a();
void Func_0200085e(s32);
void Func_02000860(s32, s32, s32);
void Func_02000272(s32);
void Func_0200087e(s32);
void Func_02000292(s32);
s32 Func_0200082c();
void Func_02000844();
void Func_0200088e();
s32 Func_02000898();
void Func_0200089c();
s32 Func_0200089e();
void Func_020008b2();
void Func_020008b6();
void Func_020008c0();
void Func_020008c6();
void Func_020008d6();
void Func_020008e6();
void Func_020008f0();
void Func_020008f2();
void Func_020008f4();
void Func_020008f8();
void Func_020008fe();
void Func_02000918();
void Func_02000926();
void Func_02000930();
void Func_02000940();
void Func_0200095a();
void Func_02000992();
void Func_0200093c(void);
void Func_0200098c(s32, s32);
void Func_02000950(void);
void Func_02000438(void);
void Func_0200099c(s32, s32);
void Func_02000978(void);
s32 Func_0200096c(void);
void Func_020009ec(s32);
void Func_02000a04(s32, s32);
void Func_02000a2e(s32, s32);
void Func_020009c6(s32, s32);
void Func_020009ba(void);
u8 *Func_020009ea(s32);
void Func_02000a6a(s32, s32);
void Func_02000a3a(s32);
void Func_02000a52(s32, s32);
void Func_020009f6(void);
u8 *Func_02000a32(s32);
void Func_02000a10(void);
void Func_02000ab2(s32, s32);
void Func_02000a82(s32);
void Func_02000a9a(s32, s32);
void Func_02000a3e(void);
u8 *Func_02000a7a(s32);
void Func_02000a58(void);
void Func_02000afa(s32, s32);
void Func_02000aca(s32);
void Func_02000ae2(s32, s32);
void Func_02000a86(void);
u8 *Func_02000ac2(s32);
void Func_02000aa0(void);
void Func_02000b4a(s32, s32);
void Func_02000b12(s32);
void Func_02000b2a(s32, s32);
void Func_02000ace(void);
void Func_02000ae0(void);
void Func_02000b48(s32);
void Func_02000b50(s32);
void Func_02000b68(s32, s32);
void Func_02000b0c(void);
void Func_02000b1a(s32, s32, s32, s32, s32, s32);
u8  *Func_02000b80();
u8  *Func_02000b8c();
u8  *Func_02000b94();
void Func_02000b34();
u8 *Func_02000b76(s32);
u8 *Func_02000b98(s32, s32);
void Func_02000bc8(s32);
void Func_02000bc0(s32, s32, u8 *);
void Func_02000bbe(s32);

/*
 * resource_386 owner at 0x0200006c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000070 holding 0x20086dc.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200006c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20086dc is image offset
 * 0x6dc under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_386 owner at 0x02000074, 4 bytes: `movs r0, #0 / bx lr`.
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

/*
 * resource_386 owner at 0x02000078, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x200007c holding 0x20087cc.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000078 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20087cc is image offset
 * 0x7cc under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/* Contiguous unnamed leaf-owner run for resource_386. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/*
 * resource_386 owner at 0x020004dc, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x20004e0 holding 0x200898c.
 *
 * LEAF RESIDUE. Published at image offset 0x24; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x020004dc reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x200898c is image offset
 * 0x98c under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * Resource 386 overlay initialiser at 0x020004e4.
 *
 * This is the overlay's ROOT: entry 0 of the exported-entry veneer table at
 * file offset 0 is `ldr r4,[pc,#0] / bx r4 / .word 0x020084e5`, and
 * 0x020084e5 - 0x8000 = 0x4e5 = `FieldScene_InitSceneStateByStep + 1`.  Everything else in the
 * overlay is reached from here or from the handler pool this owner's mode
 * selects.
 *
 * Complete owner: `push {r5, lr}` at 0x020004e4 with `sub sp, #8` at
 * 0x020004fe for the two stacked arguments of Func_08009180; interworking
 * return `add sp, #8 / pop {r5} / pop {r1} / bx r1` at 0x0200055c-0x02000562.
 * The control-flow walk reaches every halfword to 0x02000562;
 * 0x02000564-0x0200056f is the literal pool (0x03001ebc, 0x02000240,
 * 0x02008031).  Next owner at 0x02000570.
 *
 * Signature.  The return address is popped into r1, NOT r0, so r0 survives
 * and IS the result (HANDOVER section 0).  r0 is set to 0 immediately before
 * the epilogue on every path, so the owner returns a constant 0 status.
 *
 * LINK-BASE PROOF, and the strongest one available here.  The pool word
 * 0x02008031 is odd; under `offset = value - 0x8000` it is `0x30 + 1`, that
 * is `Func_02000030` plus the Thumb bit — and games/gs1/assets/code/resource_386_c_
 * 02000030.c is a tracked byte-exact source for exactly that address.  It is
 * passed to Func_080000d0, the per-frame task installer, so this word is a
 * task callback and not data.  That single word proves the base against
 * already-tracked material and names 0x02000030's role at the same time.
 *
 * The displacement/value trap, additive form (HANDOVER).  r2 is built as
 * 224 << 1 = 448 and used as the workspace displacement; `adds r2, #73`
 * turns the SAME register into the stored value 521; `subs r2, #71` then
 * turns it into the displacement 450 of the following load.  Reading it as
 * 448 - 71 or as a single running offset is the natural mistake, and nothing
 * else in the owner would catch it.
 *
 * Data_02000240 is the cross-overlay RAM global block, not an in-image
 * address: 0x02000240 lies BELOW the 0x02008000 link band, so it cannot be a
 * file offset here.  The signed halfword read out of it selects the scene.
 */

/* Veneer declarations. OverlayObject_InitObject22 is called with three further registers
 * set that it does not read — see that file. */

                       /* veneer 0x664 — actor record accessor */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value0(s32 (*f)())
{
    void Func_02000880();
    void Func_02000976();

    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    void Func_02000880();
    void Func_02000976();

    return f(a0);
}

void SceneState_CheckPositionWindow(void) {
    s32 v1;
    s32 v0;

    v0 = ((struct Resource386FirstView *)Resource386_GetFirstView(0))->sample_08;
    v1 = (s32) ((struct Resource386SecondView *)Resource386_GetSecondView(0))->sample_10 >> 0x14;
    if (((u32) ((v0 >> 0x14) - 0x22) <= 1U) && (v1 > 0x28) && (v1 <= 0x2A)) {
        Resource386_OnWindowMatch(0x250);
        return;
    }
    Resource386_OnWindowMiss(0x250);
}

u8 *SceneData_GetScriptTable(void)
{
    return (u8 *)0x020086dc;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return (u8 *)0x020087cc;
}

u8 *SceneData_InitAndGetTable87f4(void)
{
    u8 *slot = (u8 *)0x020087F4;

    Func_020006d4(slot);
    return slot;
}

void SceneActor_RunActorStep(s32 arg0)
{
    Func_020006da(arg0);
    Func_0200071a(arg0, 1);
    Func_02000752(arg0, 0);
    Func_020006f6();
}

void SceneDialogue_RunActor9Line(void)
{
    Func_02000756(0x1CC9);
    Func_02000758(9, 0, 2);
    Func_0200016a(9);
}

void SceneDialogue_RunActor11Line(void)
{
    Func_02000776(0x1CCD);
    Func_02000778(11, 0, 2);
    Func_0200018a(11);
}

void SceneDialogue_RunActor12Line(void)
{
    Func_02000796(0x1CD0);
    Func_02000798(12, 0, 2);
    Func_020001aa(12);
}

void FieldScene_RunActor16Sequence(void)
{
    u32 i;
    s32 record;

    Func_0200075c();
    Call1(Func_020007ba, 0x1cd4);
    Func_020007bc(16, 0, 2);
    Func_020007ac(16, 1);
    Func_020007ee(16, 0, 20);
    Func_020007c6(16, 4);
    Func_02000784(20);
    Func_02000806(16, 0, 20);
    Call3(Func_02000822, 16, 0x102, 60);
    Func_0200081c(16, 0, 30);
    Value2(Func_02000814, 16, 0);
    if (Value2(Func_020007dc, 0, 0) != 0) {
        bump_step(1);
    }
    Func_0200084a(16, 0, 20);
    Call1(Func_020007c2, 0x300);
    Call1(Func_020007c8, 0x868);
    Func_020007f4();
}

void SceneDialogue_RunActor16Line(void)
{
    void Func_02000880(s32, s32, s32);

    Func_0200085e(0x1CDA);
    Func_02000860(16, 0, 2);
    Func_02000272(16);
}

void SceneDialogue_RunActor23Line(void)
{
    void Func_02000880(s32, s32, s32);

    Func_0200087e(0x1CEE);
    Func_02000880(23, 0, 2);
    Func_02000292(23);
}

void FieldScene_RunActor18FlaggedSequence(void)
{
    void Func_02000880();
    void Func_02000976();

    u32 i;
    s32 record;

    Func_02000844();
    ((void (*)())Func_0200089e)(18, 0, 0);
    if (Value1(Func_0200082c, 0x85b) == 0) {
        Call1(Func_020008b6, 0x137c);
        Func_020008c6(18, 0);
    } else {
        Call1(Func_020008c6, 0x1385);
        Func_020008d6(18, 0);
    }
    if (Value2(Func_0200089e, 0, 0) == 0) {
        Func_02000880(20);
        Func_020008f8(18, 0);
        Func_0200088e(20);
        Func_020008e6(18, 2);
        Func_0200089c(20);
        if (Value0(Func_02000898) == 0) {
            Func_020008f4(18, 4);
            Func_020008b2(20);
            Call1(Func_02000918, 0x1384);
            Func_02000930(18, 0);
            goto L_020002d4;
        }
        Func_0200095a(231, 3);
        Func_020008f2(231, 0);
        Call1(Func_020008c0, 0x85b);
    } else {
        bump_step(1);
        Func_020008f0(20);
        Func_02000940(18, 3);
        Func_020008fe(20);
        Func_02000976(18, 0);
    }
    L_020002d4:;
    Call3(Func_02000992, 18, 0x4000, 0);
    Func_02000926();
}

void SceneActor_RunActor16StepWithFlag91(void)
{
    u8 *Func_02000976(s32);
    u8 *Func_02000986(s32);

    u8 *slot;
    u8 clear = 0;

    Func_0200093c();
    Func_0200098c(16, 1);
    Func_02000950();
    slot = Func_02000976(16) + 91;
    *slot = 1;
    Func_02000438();
    slot = Func_02000986(16) + 91;
    *slot = clear;
    Func_0200099c(16, 2);
}

void FieldScene_RunActor18ConditionalCue(void)
{
    void Func_02000986(s32);
    void Func_020009c8(s32, s32);

    Func_02000978();

    if (Func_0200096c() == 0) {
        Func_020009c8(18, 4);
        Func_02000986(20);
        Func_020009ec(0x1384);
        Func_02000a04(18, 0);
    } else {
        Func_02000a2e(0xE7, 3);
        Func_020009c6(0xE7, 0);
    }

    Func_020009ba();
}

void FieldScene_RunActor19StepByPlace(void)
{
    void Func_020009c8(void);

    u32 place;

    place = *(u16 *) (Func_020009ea(0) + 6);
    Func_020009c8();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000a6a(4, 19);
    } else {
        Func_02000a3a(0x1CE2);
        Func_02000a52(19, 0);
    }

    Func_020009f6();
}

void FieldScene_RunActor20StepByPlace(void)
{
    u32 place;

    place = *(u16 *) (Func_02000a32(0) + 6);
    Func_02000a10();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000ab2(5, 20);
    } else {
        Func_02000a82(0x1CE4);
        Func_02000a9a(20, 0);
    }

    Func_02000a3e();
}

void FieldScene_RunActor21StepByPlace(void)
{
    u32 place;

    place = *(u16 *) (Func_02000a7a(0) + 6);
    Func_02000a58();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000afa(6, 21);
    } else {
        Func_02000aca(0x1CE6);
        Func_02000ae2(21, 0);
    }

    Func_02000a86();
}

void FieldScene_RunActor22StepByPlace(void)
{
    u32 place;

    place = *(u16 *) (Func_02000ac2(0) + 6);
    Func_02000aa0();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000b4a(1, 22);
    } else {
        Func_02000b12(0x1CEC);
        Func_02000b2a(22, 0);
    }

    Func_02000ace();
}

void SceneDialogue_RunActor18FlaggedLine(void)
{
    s32 Func_02000abe(s32);

    Func_02000ae0();
    if (Func_02000abe(0x85B) == 0) {
        Func_02000b48(0x1382);
    } else {
        Func_02000b50(0x1CF4);
    }
    Func_02000b68(18, 0);
    Func_02000b0c();
}

u8 *SceneData_GetEffectTable(void)
{
    s32 Func_02000abe(s32);

    return (u8 *)0x0200898c;
}

s32 FieldScene_InitSceneStateByStep(void)
{
    void Func_02000abe();

    s32 scene;
    s32 zero;

    *(s32 *)(WORKSPACE + 448) = 521;
    scene = Data_02000240[225];

    if (scene == 5) {
        s32 fifth = 4;
        s32 sixth = 3;

        /* Two arguments are passed on the stack: 4 then 3. */
        Func_02000b1a(0, 120, 8, 67, fifth, sixth);
        zero = 0;
        Func_02000b80(8)[0x55] = zero;
        *(s32 *)(Func_02000b8c(8) + 12) = zero;
        *(s32 *)(Func_02000b94(8) + 20) = zero;
    } else if (scene == 7 || scene == 11) {
        /* Built by shifts: 142 << 18, 128 << 13, 168 << 18. */
        Func_02000abe(0xe7, 0x02380000, 0x00100000, 0x02a00000);
        /* 0x02008031 is Func_02000030 + 1; 200 << 4 is the period. */
        Func_02000b34(0x02008031, 0xc80);
    }

    return 0;
}

void OverlayObject_InitObject22(s32 a)
{
    u8 *o;
    u8 *q;
    u8 *p;
    u8 *v;
    s32 z;
    s32 m;

    z = 0;
    o = Func_02000b76(22);
    if (o != 0) {
        q = *(u8 **)(o + 0x50);
        p = q + 38;
        *p = z;
        p += 1;
        *p = z;
        m = 33;
        m = -m;
        q[5] &= m;
        q[9] &= 15;
        o[0x55] = z;
        o[0x5c] = 1;
        v = Func_02000b98(17, 0x608);
        Func_02000bc8(a);
        v += 0x400;
        Func_02000bc0(q[28], 0x80, v);
        Func_02000bbe(17);
    }
}
