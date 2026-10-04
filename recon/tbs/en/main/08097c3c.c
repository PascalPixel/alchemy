/* 2026-10-04 third wave, finisher 2 (still 115): CSE's skip-block path
 * carries the first test's mask into the nearby test. A volatile asm with
 * an empty operand list before the nearby test flushes CSE's table, and a
 * mask local with __asm__("" : "+r"(mask)) at the same point does the same
 * more narrowly; both give the ROM's second mask load (161), but global
 * allocation then gives cell_x r2 and the first mask r1 (ROM r0 and r2), so
 * the second mask takes r0 rather than ip and the reload rotation shifts
 * below. Moving cell_x to r0 by reusing keys (pseudo in r0), or pinning
 * cell_x to r0 or the mask to r2 (even in an inner block), spills p and
 * grows the frame to 56 (265-275); pinning p to r9 is worse (336+). An
 * unused user label before the cell loads is deleted and changes nothing.
 * Devsearch (300 iterations, seed 301) reached 101 with two empty asm
 * barriers in the move path, "+r" on moved before AudioCommand_PlayFar(175)
 * and on p after the loop, and swapped nearby/moved declarations; none of
 * them touches the cell tests, so they are not kept here.
 *
 * 2026-10-04 finisher 5 (score 115, was 183; z before x and speed/lift before
 * unknown_5b in the move path take 121 to 115): a tagged r11 binding for the
 * loop's 0x100000 makes global allocation leave source on the stack as the
 * ROM does ([sp+20], 52-byte frame, every stack offset now right); a tagged
 * r4 binding for the source cell z gives 121. Remaining: the cell tests.
 * CSE follows the jump from the first cell test into the nearby test
 * (label 520 has one use), so one 0xfff00000 pseudo serves both tests and
 * takes r0; the ROM loads the mask again for the nearby test and parks it
 * in ip (a second pseudo), and reload's register rotation after that point
 * differs throughout the move path. Rewriting the tests with ||, nested
 * ifs, >> 20 or without the cell locals: 121 to 278, no gain.
 * (lreg dump: source 19 weighted refs/258 insns outranks the 0x100000 copy,
 * 16/354, so without the binding source takes r11.)
 * The ROM also reloads source twice ([sp+20] into r2 and r3) for the cell
 * loads instead of inheriting the r1 of the hit compare, which reload does
 * only after a used label; no label sits there in any shape tried (if/else,
 * while, nested hit tests, declaration order: 115 to 162). A devsearch seed
 * reached 74 only by "+r" barriers on the spilled source and moved, which
 * insert ldr/str pairs; that confirms the reload register rotation but is
 * not admissible.
 */
/* DRAFT of FunctionHead_08097c3c: the effect that lets the player push a
   target one step: a preview object shows the step tried, and a step that
   fits moves the target (and the source object with it when the source
   stands in the way and its next cell is free).
   Not exact: 836 of 836 bytes, the blocks in the reference's order, and
   r5 to r10 in its roles. Remaining: the reference keeps the source pointer
   on the stack ([sp+20]) and gives r11 to the loop's copy of 0x100000, with
   that constant in r5 before the loop; here the source takes r11 and the
   constant is rebuilt at each use. The reference also reloads the cell mask
   after the first cell test and parks it in ip.
 * 2026-10-04 (score 4219): loop.c does hoist 0x100000 here (pseudo 106,
 * cse2 turns it into a copy of the pre-loop r5 value, as the reference's
 * "mov r11, r5"), but global allocation ranks source (19 weighted refs
 * over 258 insns) above it (16 over 354), so the copy spills and reload
 * rebuilds the constant. A while-loop head with WaitFrames in the
 * condition compiles identically; a named 0x100000 local scores 4554;
 * seven minutes of permutation reached 3180 with the same 14 inserted
 * constant loads, not kept.
 * API context measured 2026-10-02 with ordinary target flags:
 * all six prior successful objects retain every allocated byte and
 * normalized call/pool operand; no new match or adoption is claimed.
 * The shared void DispatchObject/u32 contract uses ordinary data casts;
 * every original matching-body and trial annotation is retained.
 */
#include "TYPES.H"
#include "IO_REG.H"
#include "OBJDISP.H"

struct FxPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct FxObject {
    u8 unknown_00[8];
    s32 x;                      /* 0x08 */
    s32 y;
    s32 z;
    s32 ground;                 /* 0x14 */
    u8 unknown_18[10];
    u8 mode;                    /* 0x22 */
    u8 unknown_23;
    s32 velocity_x;             /* 0x24 */
    s32 velocity_y;             /* 0x28 */
    s32 velocity_z;             /* 0x2c */
    s32 speed;                  /* 0x30 */
    s32 lift;                   /* 0x34 */
    u8 unknown_38[12];
    s32 gravity;                /* 0x44 */
    u8 unknown_48[13];
    u8 flag;                    /* 0x55 */
    u8 unknown_56[4];
    u8 unknown_5a;
    u8 unknown_5b;
    u8 unknown_5c[12];
    struct FxObject *linked;    /* 0x68 */
    void *callback;             /* 0x6c */
};

struct EffectWork {
    s32 direction;
    s32 x;
    s32 y;
    s32 z;
    struct FxObject *source;    /* 0x10 */
    struct FxObject *target;    /* 0x14 */
    s16 source_id;              /* 0x18 */
    u8 unknown_1a[0x1e];
    void *callback;             /* 0x38 */
    const u8 *script;           /* 0x3c */
    u8 unknown_40[4];
    u8 palette;                 /* 0x44 */
};

extern struct EffectWork *gEffectWork;
extern const u8 BattleFx_SourceHoldScript[];
extern const u8 Data_0809f118[];
extern volatile u32 gKeysHeld;
extern volatile u32 gKeyState;
extern volatile u32 gFrameCount;

void BattleEffect_InitializeSharedScene(void);
struct FxObject *BattleFx_StartItemBreak(struct FxObject *source);
void BattleFx_PrepareBufferInterpolation(void);
void BattleFx_SnapScaleToFull(struct FxObject *object);
void WaitFrames(s32 frames);
void Object_SetMoveTargetFar(struct FxObject *object, s32 x, s32 y, s32 z);
void Object_CommitPosition(struct FxObject *object);
void ObjectGroup_ApplyRandomChildValues(void);
void AudioCommand_PlayFar(s32 cue);
s32 BattleFx_GetCycledTableWord(u32 keys);
void Object_SetMode(struct FxObject *object, s32 mode);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct FxPosition *position);
struct FxObject *ScriptObject_FindOverlappingEntryFar(struct FxObject *object, struct FxPosition *position);
s32 Object_CheckMovementCollision(struct FxObject *object, struct FxPosition *position);
s32 Map_GetCellAttributeLowNibbleFar(s32 layer, s32 x, s32 z);
struct FxObject *Object_GetById(s32 id);
void Animation_ApplyChildValuesFar(struct FxObject *object, s32 value);
void EffectRuntime_StopCurrentObject(void);
void UpdateRisingParticleBurst(struct FxObject *object);

void FunctionHead_08097c3c(void)
{
    struct FxPosition position;
    struct FxPosition nearby;
    struct EffectWork *work;
    struct FxObject *source;
    struct FxObject *target;
    struct FxObject *preview;
    struct FxObject *hit;
    s32 x;
    s32 z;
    s32 angle;
    s32 moved;
    s32 direction;
    s32 keys;
    s32 cell_x;
    /* FAKEMATCH: the native's source cell z sits in r4 (127 -> 121). */
    register s32 cell_z asm("r4");
    struct FxPosition *p;

    work = gEffectWork;
    source = work->source;
    target = work->target;
    angle = work->direction + 0x8000;
    moved = 0;
    if (target == NULL)
        return;
    BattleEffect_InitializeSharedScene();
    source->linked = target;
    ObjectDispatch_InitializeFar((struct DispatchObject *)source, (u32)BattleFx_SourceHoldScript);
    preview = BattleFx_StartItemBreak(source);
    if (preview == NULL) {
        BattleFx_PrepareBufferInterpolation();
        return;
    }
    preview->linked = target;
    p = &position;
    p->x = target->x;
    p->y = target->y + 0x100000;
    p->z = target->z;
    Vector_AddPolarOffset(0x100000, angle, p);
    Object_SetMoveTargetFar(preview, p->x, p->y, p->z);
    BattleFx_SnapScaleToFull(preview);
    preview->speed = 0x40000;
    preview->lift = 0x8000;
    preview->flag = 4;
    target->callback = ObjectGroup_ApplyRandomChildValues;
    target->speed = 0x6666;
    target->lift = 0x3333;
    target->unknown_5a = moved;
    target->mode = 2;
    {
    /* FAKEMATCH: the native gives r11 to the loop's 0x100000 and leaves the source object on the stack; global allocation otherwise ranks source first (183 -> 127). */
    register s32 step asm("r11") = 0x100000;
    for (;;) {
        WaitFrames(1);
        keys = gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS);
        if (keys != 0)
            break;
        direction = (u16)BattleFx_GetCycledTableWord(gKeysHeld);
        if (direction == 0xffff) {
            position.x = target->x;
            position.y = target->y + step;
            position.z = target->z;
            Vector_AddPolarOffset(step, angle, &position);
            Object_SetMoveTargetFar(preview, position.x, position.y, position.z);
            Object_SetMode(preview, 1);
            preview->velocity_x = 0;
            preview->velocity_y = 0;
            preview->velocity_z = 0;
            continue;
        }
        position.x = target->x;
        position.y = target->y + step;
        position.z = target->z;
        Vector_AddPolarOffset(step, angle, &position);
        Vector_AddPolarOffset(0x20000, direction, &position);
        Object_SetMoveTargetFar(preview, position.x, position.y, position.z);
        Object_CommitPosition(preview);
        position.x = target->x;
        position.y = target->y;
        position.z = target->z;
        Vector_AddPolarOffset(step, direction, &position);
        nearby.x = target->x;
        nearby.y = target->y;
        nearby.z = target->z;
        Vector_AddPolarOffset(0x200000, direction, &nearby);
        if (Object_CheckMovementCollision(target, &position) > 0)
            goto blocked;
        hit = ScriptObject_FindOverlappingEntryFar(target, p);
        if (hit == NULL)
            goto move;
        if (hit != source)
            goto blocked;
        cell_x = source->x & 0xfff00000;
        cell_z = source->z & 0xfff00000;
        if (cell_x == (p->x & 0xfff00000) && cell_z == (p->z & 0xfff00000))
            goto blocked;
        if (cell_x != (nearby.x & 0xfff00000) || cell_z != (nearby.z & 0xfff00000))
            goto move;
        if (Map_GetCellAttributeLowNibbleFar(source->mode, nearby.x, nearby.z) == 0)
            goto move_source;
    blocked:
        Object_SetMode(preview, 4);
        if ((gFrameCount & 15) == 0)
            AudioCommand_PlayFar(114);
        continue;
    move_source:
        moved = 1;
    move:
        AudioCommand_PlayFar(175);
        z = position.z;
        x = position.x;
        Object_SetMode(preview, Data_0809f118[(u16)(angle - direction) >> 14]);
        WaitFrames(15);
        target->speed = 0x3333;
        target->lift = 0x3333;
        target->unknown_5b = 0;
        Object_SetMoveTargetFar(target, position.x, position.y, position.z);
        preview->flag = 0;
        preview->speed = 0x3333;
        preview->lift = 0x3333;
        Vector_AddPolarOffset(step, direction, &position);
        Object_SetMoveTargetFar(preview, position.x, position.y + step, position.z);
        if (moved == 1) {
            Object_GetById(work->source_id)->unknown_5a &= 0xfe;
            source->speed = 0x3333;
            source->lift = 0x3333;
            Object_SetMoveTargetFar(source, nearby.x, nearby.y, nearby.z);
        }
        Object_CommitPosition(target);
        target->x = x;
        target->z = z;
        target->velocity_x = 0;
        target->velocity_z = 0;
        break;
    }
    }
    Animation_ApplyChildValuesFar(target, work->palette);
    ObjectDispatch_InitializeFar((struct DispatchObject *)target, (u32)(work->script));
    target->callback = work->callback;
    EffectRuntime_StopCurrentObject();
    if (moved == 1)
        Object_GetById(work->source_id)->unknown_5a |= 1;
    BattleFx_PrepareBufferInterpolation();
    UpdateRisingParticleBurst(preview);
}
