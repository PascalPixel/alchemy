#include "types.h"

#define SceneState_InitEntryWorkspaceAndFlag96f Func_0200022c

#define SceneData_SelectTable8614ByFlag96f Func_02000208

#define SceneEffect_ConfigureIndexedEffect85fe Func_02000194

#define FieldScene_RunLayoutStepThenSet201 Func_020000f8

#define FieldScene_RunActorCue25dcBranch Func_020000b0

#define FieldScene_RunActorCue25b8Branch Func_02000068

#define SceneData_SelectActorTableByFlag96f Func_02000044

#define SceneData_GetMessageTable Func_0200003c

#define SceneData_ReturnZero Func_02000038

#define SceneData_GetScriptTable Func_02000030

#define SceneEffect_ConfigureIndexedEffect85e8 Func_02000120

extern u8 Value_000025b8;
extern u8 Value_000025dc;
extern s16 Data_020085d0[];
extern s16 Data_02000240[];

s32 Func_020002e2(s32);
s32 Func_02000332();
s32 Func_02000342(s32, s32);
void Func_02000334(s32);
void Func_02000352(s32);
void Func_0200035a(s32);
void Func_02000372(s32, s32);
s32 Func_0200037a();
s32 Func_0200038a(s32, s32);
void Func_0200037c(s32);
void Func_0200039a(s32);
void Func_020003a2(s32);
void Func_020003ba(s32, s32);
s32 Func_0200039e(s32, s32, s32, s32, s32, s32);
void Func_020003b4(s32);
void Func_02000438();
void Func_020003e2();
void Func_02000446(s32, s32, s32);
void Func_02000452();
void Func_020004ac();
void Func_02000456();
void Func_020004ba(s32, s32, s32);
void Func_020004c6();
s32 Func_020004a6(s32);
void Func_020004e2();
s32 Func_020004f6();
void Func_0200035e();
void Func_02000526();

/*
 * resource_3c1 owner at 0x02000030, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000034 holding 0x20082f0.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000030 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20082f0 is image offset
 * 0x2f0 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/* Contiguous unnamed leaf-owner run for resource_3c1. */

/*
 * resource_3c1 owner at 0x0200003c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000040 holding 0x20083c8.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200003c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20083c8 is image offset
 * 0x3c8 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/* Contiguous unnamed state-owner run for resource_3c1. */

static __inline__ void SetOffset(s32 actor, s32 axis, s32 offset)
{
    Func_02000446(actor, axis, offset);
}

static __inline__ void SetOffset_020004ba(s32 actor, s32 axis, s32 offset)
{
    Func_020004ba(actor, axis, offset);
}

u8 *SceneData_GetScriptTable(void)
{
    return (u8 *)0x020082f0;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return (u8 *)0x020083c8;
}

s32 SceneData_SelectActorTableByFlag96f(void) {
    if (Func_020002e2(0x96f) != 0) {
        return 0x020084e0;
    }
    return 0x020083f0;
}

void FieldScene_RunActorCue25b8Branch(s32 obj)
{
    s32 cue = (s32) &Value_000025b8;
    Func_02000332(cue);
    Func_02000342(obj, 0);
    if (Func_02000332(0, 0) == 0) {
        Func_02000334(10);
        Func_02000352(cue + 1);
    } else {
        Func_0200035a(cue + 2);
    }
    Func_02000372(obj, 0);
}

void FieldScene_RunActorCue25dcBranch(s32 obj)
{
    s32 cue = (s32) &Value_000025dc;
    Func_0200037a(cue);
    Func_0200038a(obj, 0);
    if (Func_0200037a(0, 0) == 0) {
        Func_0200037c(10);
        Func_0200039a(cue + 1);
    } else {
        Func_020003a2(cue + 2);
    }
    Func_020003ba(obj, 0);
}

void FieldScene_RunLayoutStepThenSet201(void)
{
    s32 width = 4;
    s32 height = 9;

    Func_0200039e(25, 9, 1, 1, width, height);
    Func_020003b4(0x201);
}

void SceneEffect_ConfigureIndexedEffect85e8(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    s32 no = *(s16 *)(work + 364);
    u16 x = Data_020085d0[no * 2];
    u16 y = Data_020085d0[no * 2 + 1];

    Func_02000438(158);
    Func_020003e2(0x020085e8, x, y);
    SetOffset(0, 0, -16);
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 16;
    Func_02000452(no);
}

void SceneEffect_ConfigureIndexedEffect85fe(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    s32 no = *(s16 *)(work + 364);
    u16 x = Data_020085d0[no * 2];
    u16 y = Data_020085d0[no * 2 + 1];

    Func_020004ac(158);
    Func_02000456(0x020085fe, x, y);
    SetOffset_020004ba(0, 0, -16);
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 16;
    Func_020004c6(no);
}

s32 SceneData_SelectTable8614ByFlag96f(void)
{
    if (Func_020004a6(0x96F) != 0) {
        return 0x02008758;
    }
    return 0x02008614;
}

s32 SceneState_InitEntryWorkspaceAndFlag96f(void)
{
    u8 *work;

    /* Record arrival on map 90, then publish the initial scene phase/timer. */
    if (Data_02000240[225] == 90) {
        Func_020004e2(0x96f);
    }

    work = *(u8 **)0x03001ebc;
    *(s32 *)(work + 448) = 256;
    *(s32 *)(work + 456) = 24;

    /* The dressing sequence and cue are unlocked by the shared event flag. */
    if (Func_020004f6(0x201) != 0) {
        Func_0200035e();
        Func_02000526(16, 4);
    }
    return 0;
}
