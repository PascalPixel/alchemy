/* NONMATCHING: 772 of 752 bytes, 280 differing halfwords, 124 aligned edits
 * (2026-09-27). Whole owner 02000540..02000830, pool 02000808..02000830;
 * call targets and pool audited against our own ROM. Retained as
 * korosseo-river-actor-map-candidate, without byte credit.
 *
 * Three structural steps, with complete normalized diffs reviewed:
 * 1. Corrected missing indirection at allocated block +0x1e0. The reference
 *    loads the table pointer before ObjectInitFromTableWithArgument; the old
 *    772/305/155 draft passed the address of that pointer. Corrected draft:
 *    776 bytes, 329 differing halfwords, 166 aligned edits.
 * 2. One motion-object local shared across the initial opponent and actors
 *    13/14: 776/317/147. Selector, object and initial acceleration now use
 *    the reference's r5/r7/r8; this is semantic lifetime reuse, not a sweep.
 * 3. Transient allocation expression separated from the later flag-result
 *    local: 772/280/127. The allocation/load/call block now matches, and
 *    flag result moves to sl as in the reference. Retained best corrected C.
 *
 * 2026-09-27 exact KOROSSEO_KAWA/COORDINATOR.C family transfer:
 * H1: existing Map_CopyCells/Map_CopyCellAttributes call scopes give
 * 772/280/124, retained versus 772/280/127. The final rectangle setup is
 * now exact; long-lived destination coordinates do not change allocation.
 * These services take destination column/row as their fifth/sixth arguments:
 * the locals called height/width below actually hold column 34 and row 7/38.
 * H2: also use the neighbour's GameFlag_Set/IsSet wrappers: 780/372/160.
 * Flag 0x301 then reloads at each call as in the ROM, but the selected actor
 * id spills (not its pointer), fp is saved, and motion constants diverge.
 * Rejected; restore H1. Both complete normalized diffs/pools reviewed.
 * No missing calls or new ownership fact warrant another allocation trial.
 *
 * Remaining: actor pointer stays in sl instead of sp+8; destination column
 * 34 lives in r8 rather than r6, adding moves before each map call. Flag
 * 0x301 is shared across calls; later speed/acceleration allocation and pool
 * order differ. Map/flag helper expansion is now bounded and closed; no
 * declaration permutations or register-only variants. No DONE credit.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"

extern u32 gGameState[];
extern s32 Engine_AllocateBlock(s32 slot, s32 size);
extern void Engine_ObjectCommitPosition(struct FieldActor *object);
extern void Engine_ObjectInitFromTableWithArgument(s32 *table, struct FieldActor *object);
extern void Engine_MapCopyCellAttributes(s32, s32, s32, s32, s32, s32);
extern void Engine_MapCopyCells(s32, s32, s32, s32, s32, s32);

void KorosseoKawa_MoveActorsAndMap(void)
{
    s32 selector;
    s32 result;
    s32 width;
    s32 height;
    s32 x;
    struct FieldActor *actor;
    struct FieldActor *object;

    selector = gGameState[125];
    actor = Engine_ActorGet(selector);
    object = Engine_ActorGet(12);
    Engine_GameFlagSet(0x302);
    Engine_EventBegin();
    Engine_ActorSetAnimation(selector, 8);
    Engine_EventWait(6);
    object->speed = 0x8000;
    object->acceleration = 0x3333;
    Engine_AudioPlayCue(239);
    Engine_ObjectSetAnimation(object, 2);
    Engine_ObjectSetPosition(object, object->x.fixed - 0x300000, 0, object->z.fixed);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(selector, 2);
    Engine_ObjectInitFromTableWithArgument(
        *(s32 **)(Engine_AllocateBlock(27, 0xccc) + 0x1e0), object);
    Engine_ActorSetSpeed(selector, 0x4ccc, 0x3333);
    Engine_ObjectSetPosition(actor, actor->x.fixed - 0x180000, 0, actor->z.fixed);
    Engine_ActorWaitForMove(selector);
    Engine_ActorSetAnimation(selector, 1);
    Engine_ObjectCommitPosition(object);
    Engine_ObjectSetAnimation(object, 1);
    Engine_AudioPlayCue(0x120);
    Engine_AudioPlayCue(213);
    Engine_EventWait(15);
    Engine_EventEnd();
    height = 34;
    width = 7;
    Map_CopyCellAttributes(37, 7, 1, 4, height, width);
    Map_CopyCellAttributes(36, 7, 1, 4, 37, width);
    result = Engine_GameFlagIsSet(0x301);
    if (result != 0) {
        Engine_EventBegin();
        Engine_CameraSetSpeed(0x20000, 0x4000);
        Engine_CameraMoveTo(0x2280000, -1, 0xc80000, 1);
        width = 38;
        Engine_CameraWaitForMove();
        Map_CopyCells(96, 29, 1, 3, height, width);
        Engine_EventWait(3);
        Map_CopyCells(97, 29, 1, 3, height, width);
        Engine_EventWait(3);
        Map_CopyCells(98, 29, 1, 3, height, width);
        Engine_EventWait(3);
        Map_CopyCells(99, 29, 1, 3, height, width);
        Engine_EventWait(3);
        Map_CopyCells(100, 29, 1, 3, height, width);
        Engine_EventWait(15);
        Engine_EventEnd();
    } else {
        Engine_GameFlagSet(0x301);
        Engine_EventBegin();
        Engine_CameraSetSpeed(0x20000, 0x4000);
        Engine_CameraMoveTo(0x2580000, -1, 0xc80000, 1);
        Engine_CameraWaitForMove();
        object = Engine_ActorGet(13);
        object->motion_flags = (u8)result;
        object->speed = 0xcccc;
        object->acceleration = 0x6666;
        Engine_ObjectSetPosition(object, object->x.fixed, 0x80000, object->z.fixed);
        width = 38;
        Engine_ObjectSetAnimation(object, 3);
        Map_CopyCells(96, 29, 1, 3, height, width);
        Engine_EventWait(3);
        Map_CopyCells(97, 29, 1, 3, height, width);
        Engine_EventWait(3);
        Map_CopyCells(98, 29, 1, 3, height, width);
        Engine_EventWait(3);
        Map_CopyCells(99, 29, 1, 3, height, width);
        Engine_EventWait(3);
        Map_CopyCells(100, 29, 1, 3, height, width);
        object = Engine_ActorGet(14);
        object->motion_flags = (u8)result;
        object->speed = 0xcccc;
        object->acceleration = 0x6666;
        Engine_ObjectSetPosition(object, object->x.fixed, 0x200000, object->z.fixed);
        Engine_ObjectCommitPosition(object);
        Engine_EventWait(15);
        Engine_EventEnd();
        Map_CopyCellAttributes(43, 12, 1, 1, 41, 12);
    }
}
