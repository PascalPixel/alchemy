#include "types.h"

#define SceneState_ApplyArgMode0AndReturnZero Func_02000030
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000031
#define SecondaryRuntimeSelector Value_00000030
#define TertiaryRuntimeSelector Value_0000002f
#define PrimaryOverlayData Data_020089ec
#define SecondaryOverlayData Data_02008a64
#define TertiaryOverlayData Data_02008b24
#define DefaultOverlayData Data_020089bc
#define SceneData_SelectByRuntimeSelector Func_02000040
#define SceneData_GetTable8bcc Func_02000098
#define SceneData_SelectSecondaryDataByRuntimeSelector Func_020000a0
#define SceneData_SelectDataByRuntimeSelector Func_020000f4
#define SceneState_ConfigureRegion1_0_21x14 Func_02000148
#define SceneState_ConfigureRegion0_0_21x14 Func_02000168
#define SceneState_ApplyTwoRects Func_02000188
#define FieldScene_RunTwoLayoutSteps Func_020001bc
#define FieldScene_RunActor9Flag882Scene Func_020001f0
#define FieldScene_RunActorFifteenScene Func_02000280
#define FieldScene_RunActorSixteenScene Func_020002ac
#define FieldScene_RunActor17Steps28AndD2 Func_020002d8
#define ActorPresentation_SetSceneCell31AndFlag305 Func_02000424
#define SceneState_SetGlobalByte17 Func_0200044c
#define SceneState_ClearRuntimeByte17 Func_0200045c
#define FieldScene_DispatchByScenarioId Func_0200046c
#define RunGuardedSceneSetup Func_020004b4
#define RunSceneCommand Func_02000e04
#define FinishSceneSetup Func_02000e64
#define SceneState_SetRuntimeWord448To516 Func_020004e8
#define SceneActor_FindSlotAtTile Func_020007c4
#define StagedActor_PushActorAhead Func_020007f8
#define Resource398_ImportBankNoOp Func_02000904

#include "select_overlay_data_by_runtime_selector.h"

extern s16 Data_02000240[];
extern unsigned char Value_00000031;
extern unsigned char Value_00000030;
extern unsigned char Value_0000002f;
extern u8 *Data_03001ebc;
extern s32 Data_02009064[]; /* packed direction steps, 16 entries */

void Func_02000a8e(s32, s32, s32, s32, s32, s32);
void Func_02000aae(s32, s32, s32, s32, s32, s32);
void Func_02000ac6(s32, s32, s32, s32, s32, s32);
void Func_02000ae2(s32, s32, s32, s32, s32, s32);
void Func_02000afa(s32, s32, s32, s32, s32, s32);
void Func_02000b16(s32, s32, s32, s32, s32, s32);
void Func_02000b5c(void);
s32 Func_02000b7e(s32, s32, s32);
void Func_02000b54(s32);
void Func_02000b78(void);
void Func_02000bec(void);
s32 Func_02000c24(s32, s32);
void Func_02000bf2(s32);
void Func_02000c30(s32, s32);
void Func_02000c14(void);
void Func_02000c18(void);
s32 Func_02000c50(s32, s32);
void Func_02000c1e(s32);
void Func_02000c5c(s32, s32);
void Func_02000c44(void);
s32 Func_02000c7c(s32, s32);
void Func_02000c4a(s32);
void Func_02000c98(s32);
void Func_02000c88(s32, s32);
s32 Func_02000d6a(s32, s32, s32, s32, s32, s32);
void Func_02000d90(s32);
void Func_02000936(void);
void Func_020009d2(void);
s32 Func_02000e04();
void Func_02000e64();
typedef void(*SceneSetup)(s32, s32, s32, s32, s32, s32);
void Func_02000e86(s32, s32);
void Func_02000e8e(s32, s32);
s32 Func_02000e54(s32);
void Func_02000e9a(s32, s32, s32);
u8 *Func_02000e9a_b(s32);
void Func_02000e68(u8 *, s32);
u8 *Func_02001180();
s32 *Func_02000ff6();
s32 *Func_0200101a();
s32 Func_020011c0();
void Func_020011a4();
void Func_0200124a();
void Func_020011ca();
void Func_020011dc();
void Func_020011ea();
void Func_020011f0();
void Func_02000be8(void);

/*
 * resource_398 owner at 0x02000098, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x200009c holding 0x2008bcc.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000098 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008bcc is image offset
 * 0xbcc under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * Resource 398 map-variant selector at 0x0200046c.
 *
 * Complete owner: `push {lr}` at 0x0200046c and the matching
 * `pop {r1} / bx r1` at 0x0200049e.  72-byte row: 54 bytes of code, an
 * alignment halfword at 0x020004a2, and four pool words (0x02000240,
 * 0x00000031, 0x00000030, 0x0000002f) filling 0x020004a4-0x020004b3.  The pool
 * map is from a control-flow walk: every branch in the body targets
 * 0x02000486, 0x02000492 or 0x0200049c, and nothing reaches 0x020004a2.
 *
 * Signature.  The return address is popped into r1, not r0, so r0 survives and
 * is the result — and `movs r0,#0` immediately precedes the pop, so the owner
 * returns 0.  Same shape as the tracked byte-exact games/gs1/assets/code/
 * resource_398_c_02000030.c, which is written `s32 Func_02000030(...)
 * { ...; return 0; }`.
 *
 * ROOT.  This is entry 0 of the exported-entry veneer table at image offset 0:
 * `ldr r4,[pc,#0] / bx r4 / .word 0x0200846d`, which is 0x0200_046c + the Thumb
 * bit under the 0x02008000 link base.  Working the call graph root-down from
 * here reaches 0x020004b4, 0x020004e8 and 0x02000538 and nothing else.
 *
 * The selector itself is the cross-overlay Data_02000240 idiom: the signed
 * halfword at byte offset 448 (element 224), branched on.  The tracked
 * byte-exact games/gs1/assets/code/resource_398_c_02000040.c reads the same halfword and
 * tests it against the same three values, spelling them `(s32)&Value_00000031`
 * and so on — that is the exact reconstruction's constant-pooling device, and the plain
 * integers below are the same numbers.
 *
 * Three call sites, matching the row's advertised count.  All three are
 * overlay-local prologues, not veneers.
 */

/* 0x02000e9a serves two imports in sibling arms: the three-argument setter in
 * the first and the one-argument record accessor in the second. */

/*
 * Resource 398 push-the-block interaction at 0x020007f8.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the high-register save
 * `mov r7,sl / mov r6,r9 / mov r5,r8 / push {r5, r6, r7}` at 0x020007f8, and
 * the matching unwind `add sp,#12 / pop {r3, r5, r6} / mov r8,r3 / mov r9,r5 /
 * mov sl,r6 / pop {r5, r6, r7} / pop {r0} / bx r0` at 0x020008e6.  268-byte
 * row: 254 bytes of code, an alignment halfword at 0x020008f6, and the three
 * pool words 0x02009064, 0xffff0000 and 0x00003333 filling
 * 0x020008f8-0x02000903.  Control-flow walk: every branch target is
 * 0x020008e6 or below and the bare `bx lr` leaf at 0x02000904 follows the
 * pool.  The return address is popped into r0, so the owner is `void`.
 *
 * Role.  Two interaction records name this address (pool word 0x020087f9 =
 * 0x0200_07f8 + the Thumb bit under the proven 0x02008000 link base), keyed
 * 0x00000202/0xffff000a and 0x00008602/0xffff000b.
 *
 * Direction table.  0x02009064 is in-image data at file offset 0x1064 (even
 * pool word, so data rather than a Thumb entry).  It is the packed-direction
 * family: indexed by `heading >> 12`, X step in the high halfword and Z step
 * in the low halfword, promoted back to 16.16 by `& 0xffff0000` and `<< 16`
 * rather than by multiply.
 *
 * Coordinate views.  The s16 at +0x0a and +0x12 are the integer parts of the
 * 16.16 words at +0x08 and +0x10, so `(integer + (step >> 16)) >> 4` is the
 * neighbouring tile.  That is exactly the pair the tracked byte-exact
 * games/gs1/assets/code/resource_398_c_020007c4.c compares as `p[2] >> 20` and
 * `p[4] >> 20` — the lookup takes tile coordinates.
 *
 * Behaviour: find the actor one step ahead of the player; require the tile
 * beyond it to be empty; stage the pushed position in the three-word frame
 * slot, ask Object_CheckMovementCollision whether the move collides, and if it does not,
 * commit it and hand off to the paired-actor gate scene at 0x02000304.
 *
 * Imports resolved with cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- (an overlay `bl` stores
 * `target_offset - 2`).  Twelve call sites against the row's advertised 11;
 * the manifest's `calls` field is a floor.  Per-target: Scene_GetRecord 1,
 * Func_020007c4 2, Object_CheckMovementCollision 1, Object_SetMode 2, Func_080000c0 1,
 * Audio_PlayCue 1, Object_SetPosition 2, Object_CommitPosition 1, Func_02000304 1.
 *
 * `blocker` is provably zero where it is stored at +0x24 and +0x2c — the guard
 * above returns early otherwise — but it is the register the assembly stores,
 * so it is spelled as itself rather than folded to a literal.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */

                       /* actor record by slot id */

                       /* overlay-local: actor occupying tile (x, z), or 0 */

                       /* collision probe: >0 means the move is blocked */

                       /* set actor motion state */

                       /* audio cue */

                       /* present message by id */

                       /* start a slide to (x, y, z) */

                       /* commit the slide */

/* Deliberate no-op callback published immediately before the import bank. */

s32 SceneState_ApplyArgMode0AndReturnZero(s32 no) {
    s32 Func_02000976(s32, s32);

    Func_02000976(no, 0);
    return 0;
}

s32 SceneData_SelectByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

s32 Func_02000094(void)
{
    return 0;
}

u8 *SceneData_GetTable8bcc(void)
{
    return (u8 *)0x02008bcc;
}

#undef PrimaryOverlayData
#define PrimaryOverlayData Data_02008c2c
#undef SecondaryOverlayData
#define SecondaryOverlayData Data_02008c5c
#undef TertiaryOverlayData
#define TertiaryOverlayData Data_02008cbc
#undef DefaultOverlayData
#define DefaultOverlayData Data_02008c14

extern u8 Data_02008c2c[];
extern u8 Data_02008c5c[];
extern u8 Data_02008cbc[];
extern u8 Data_02008c14[];

s32 SceneData_SelectSecondaryDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

#undef PrimaryOverlayData
#define PrimaryOverlayData Data_02008ea8
#undef SecondaryOverlayData
#define SecondaryOverlayData Data_02008efc
#undef TertiaryOverlayData
#define TertiaryOverlayData Data_02008f80
#undef DefaultOverlayData
#define DefaultOverlayData Data_02008e9c

extern u8 Data_02008ea8[];
extern u8 Data_02008efc[];
extern u8 Data_02008f80[];
extern u8 Data_02008e9c[];

s32 SceneData_SelectDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

void SceneState_ConfigureRegion1_0_21x14(void)
{
    s32 w = 21;
    s32 h = 14;

    Func_02000a8e(1, 0, 1, 1, w, h);
}

void SceneState_ConfigureRegion0_0_21x14(void)
{
    s32 w = 21;
    s32 h = 14;

    Func_02000aae(0, 0, 1, 1, w, h);
}

void SceneState_ApplyTwoRects(void)
{
    {
        s32 a5 = 1;
        s32 a6 = 3;

        Func_02000ac6(111, 37, 97, 21, a5, a6);
    }
    {
        s32 a5 = 32;
        s32 a6 = 24;

        Func_02000ae2(46, 38, 3, 2, a5, a6);
    }
}

void FieldScene_RunTwoLayoutSteps(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 3;

        Func_02000afa(95, 21, 97, 21, fifth, sixth);
    }
    {
        s32 fifth = 32;
        s32 sixth = 25;

        Func_02000b16(46, 38, 3, 1, fifth, sixth);
    }
}

void FieldScene_RunActor9Flag882Scene(void)
{
    Func_02000b5c();
    Func_02000b7e(9, 0, 0);
    Func_02000b54(0x882);
    Func_02000b78();
}

void FieldScene_RunActorFifteenScene(void)
{
    void Func_02000c40(s32);

    Func_02000bec();
    Func_02000c24(0xF, 0);
    Func_02000bf2(0x28);
    Func_02000c40(0xD2);
    Func_02000c30(0xF, 6);
    Func_02000c14();
}

void FieldScene_RunActorSixteenScene(void)
{
    void Func_02000c40(void);
    void Func_02000c6c(s32);

    Func_02000c18();
    Func_02000c50(0x10, 0);
    Func_02000c1e(0x28);
    Func_02000c6c(0xD2);
    Func_02000c5c(0x10, 6);
    Func_02000c40();
}

void FieldScene_RunActor17Steps28AndD2(void)
{
    void Func_02000c6c(void);

    Func_02000c44();
    Func_02000c7c(0x11, 0);
    Func_02000c4a(0x28);
    Func_02000c98(0xD2);
    Func_02000c88(0x11, 6);
    Func_02000c6c();
}

void ActorPresentation_SetSceneCell31AndFlag305(void)
{
    s32 width = 8;
    s32 height = 13;

    Func_02000d6a(31, 0, 1, 1, width, height);
    Func_02000d90(0x305);
}

void SceneState_SetGlobalByte17(void) {
    FIELD_AT_OFFSET(*(void **)0x03001E70, s8 *, 0x17) = 1;
}

void SceneState_ClearRuntimeByte17(void) {
    FIELD_AT_OFFSET(*(void **)0x03001E70, s8 *, 0x17) = 0;
}

s32 FieldScene_DispatchByScenarioId(void)
{
    void Func_02000976(void);

    s16 variant = Data_02000240[224];

    if (variant == (s32)&Value_00000031) {
        Func_02000936();
    } else if (variant == (s32)&Value_00000030) {
        Func_02000976();
    } else if (variant == (s32)&Value_0000002f) {
        Func_020009d2();
    }
    return 0;
}

void RunGuardedSceneSetup(void)
{
    if (RunSceneCommand(0x305) != 0) {
        s32 width = 8;
        s32 height = 13;

        ((SceneSetup)RunSceneCommand)(31, 0, 1, 1, width, height);
        FinishSceneSetup(8, 0);
    }
}

void SceneState_SetRuntimeWord448To516(void)
{
    /* 448 is built as 224 << 1 and the stored 516 as that same register plus
     * 68; reading it as one running offset is the natural mistake. */
    *(s32 *) (Data_03001ebc + 448) = 516;

    Func_02000e86(8, 1);
    Func_02000e8e(10, 2);

    if (Func_02000e54(0x882) != 0) {
        Func_02000e9a(9, 0, 0);
    } else {
        Func_02000e68(Func_02000e9a_b(9), 0);
    }
}

s32 *SceneActor_FindSlotAtTile(s32 x, s32 z) {
    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

void StagedActor_PushActorAhead(void)
{
    u8 *player;
    u8 *target;
    u8 *blocker;
    s32 heading;
    s32 tx;
    s32 tz;
    s32 pos[3];

    player = Func_02001180(0);
    heading = *(u16 *)(player + 6) >> 12;

    tx = (*(s16 *)(player + 10)
        + (Data_02009064[heading] >> 16)) >> 4;
    tz = (*(s16 *)(player + 18)
        + ((Data_02009064[heading] << 16) >> 16)) >> 4;
    target = (u8 *)Func_02000ff6(tx, tz);
    if (target == 0) return;

    tx = (*(s16 *)(target + 10)
        + (Data_02009064[heading] >> 16)) >> 4;
    tz = (*(s16 *)(target + 18)
        + ((Data_02009064[heading] << 16) >> 16)) >> 4;
    blocker = (u8 *)Func_0200101a(tx, tz);
    if (blocker != 0) return;

    target[0x22] = 2;

    pos[0] = *(s32 *)(target + 8)
        + (Data_02009064[heading] & (s32)0xffff0000);
    pos[1] = *(s32 *)(target + 12);
    pos[2] = *(s32 *)(target + 16) + (Data_02009064[heading] << 16);

    if (Func_020011c0(target, pos) > 0) return;

    Func_020011a4(player, 8);
    Func_020011a4(15);
    Func_0200124a(185);

    *(s32 *)(target + 48) = 0x3333;
    *(s32 *)(target + 52) = 0x3333;
    Func_020011ca(target, pos[0], pos[1], pos[2]);

    *(s32 *)(player + 48) = 0x3333;
    *(s32 *)(player + 52) = 0x3333;
    Func_020011dc(player, pos[0], pos[1], pos[2]);

    Func_020011ea(target);

    *(s32 *)(target + 8) = pos[0];
    *(s32 *)(target + 16) = pos[2];
    *(s32 *)(target + 36) = (s32)blocker;
    *(s32 *)(target + 44) = (s32)blocker;

    Func_020011f0(player, 1);
    Func_02000be8();
}

void Resource398_ImportBankNoOp(void)
{
}
