#include "ENTRY_SETUP.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"

struct SwitchCell {
    u32 x;
    u32 z;
};

struct SwitchEffect {
    s32 active;
    u8 unknown_04[0x5f];
    u8 finished;
};

struct FieldActor *OverlayObject_PrepareObjectWithCommand15(s32 x, s32 y, s32 z, s32 kind);
void OverlayObject_WaitUntilIdle(struct FieldActor *object);
struct SwitchEffect *SceneEffect_SpawnEffect284AtCell(s32 x, s32 z, const void *script);
extern struct SwitchCell gVinasuSwitchCells[];
extern u8 gVinasuSettleScriptA[];
extern u8 gVinasuSettleScriptB[];
extern u16 gVinasuSettleCells[];

extern s32 gVinasuBlockHeights[];

void FieldScene_RunGuardedRectStep(void)
{
    s32 x;
    s32 y;

    Event_Begin();
    if (SceneActor_TryMoveActorZeroTwoTilesAhead() == 0) {
        x = 45;
        y = 43;
        Map_CopyCellAttributes(109, 43, 7, 5, x, y);
        RunStagedActorTransition();
    }
    Event_End();
    VinasuHeya_RunCellPushScene();
}

/* Venus Lighthouse room: after actors 9 to 11 are pushed, each block that
 * rests on one of the four switch cells and shares its cell with no other
 * block sinks into it and marks it. Once all three are down, two effects
 * play and flag 0x302 (block 9 on the first switch) or 0x303 is set, then
 * 0x304. */
void VinasuHeya_SettlePushedBlocks(void)
{
    struct FieldActor *marker = 0;
    struct FieldActor *leader = Object_GetById(0);
    struct FieldActor *block;
    struct FieldActor *other;
    struct SwitchEffect *first;
    struct SwitchEffect *second;
    u8 *flags;
    u32 id;
    u32 i;
    u32 slot;
    s32 priority;

    Engine_EventBegin();
    Call6((void (*)())Engine_MapCopyCellAttributes, 108, 39, 13, 7, 44, 39);
    for (id = 9; id <= 11; id++) {
        block = Object_GetById(id);
        flags = &block->priority_flags;
        if (*flags != 2) {
            Call6((void (*)())Engine_MapCopyCellAttributes, 47, 39, 1, 1, block->x.fixed >> 20, block->z.fixed >> 20);
        } else {
            Call6((void (*)())Engine_MapCopyCellAttributes, 46, 39, 1, 1, block->x.fixed >> 20, block->z.fixed >> 20);
        }
        slot = 5;
        for (i = 0; i <= 3; i++) {
            if (block->x.fixed >> 20 == gVinasuSwitchCells[i].x && block->z.fixed >> 20 == gVinasuSwitchCells[i].z
                && block->y.fixed >= 0) {
                slot = i;
                break;
            }
        }
        if (slot == 5) {
            continue;
        }
        for (i = 9; i <= 11; i++) {
            other = Object_GetById(i);
            if (id != i && block->x.fixed >> 20 == other->x.fixed >> 20
                && block->z.fixed >> 20 == other->z.fixed >> 20) {
                slot = 5;
                break;
            }
        }
        if (slot == 5) {
            continue;
        }
        priority = leader->sprite->priority;
        if (leader->z.fixed >> 20 <= gVinasuSwitchCells[slot].z) {
            marker = OverlayObject_PrepareObjectWithCommand15(block->x.fixed, block->y.fixed,
                                                              block->z.fixed - 0x40000, 20);
            Engine_ActorSetSpritePriority(0, 3);
        }
        Engine_ActorSetSpriteFlags(Object_GetById(id), 0);
        block->unknown_22 = 0;
        block->motion_flags = 3;
        *(s32 *)&block->unknown_44[0] = 0;
        *(s32 *)&block->unknown_44[4] = 0x1999;
        Call6((void (*)())Engine_MapCopyCellAttributes, 42, 41, 1, 1, gVinasuSwitchCells[slot].x, gVinasuSwitchCells[slot].z);
        OverlayObject_WaitUntilIdle(block);
        Engine_AudioPlayCue(188);
        block->collision_flags = 0;
        block->motion_flags = 0;
        block->y.fixed = -0x100000;
        Engine_ActorSetSpritePriority(id, 3);
        {
            s32 two = 2; /* FAKEMATCH: a block-local word keeps the 2 from being held for the later & 2 */

            *flags = two;
        }
        Call6((void (*)())Engine_MapCopyCellAttributes, 46, 39, 1, 1, gVinasuSwitchCells[slot].x, gVinasuSwitchCells[slot].z);
        Engine_ActorSetSpritePriority(0, priority);
        Object_GetById(0)->priority_flags |= 1;
        if (marker != 0) {
            Engine_ObjectDispatchRelease(marker);
        }
        if (Engine_GameFlagIsSet(0x304)) {
            Engine_EventEnd();
            return;
        }
        if ((Object_GetById(9)->priority_flags & Object_GetById(10)->priority_flags
             & Object_GetById(11)->priority_flags & 2) == 0) {
            continue;
        }
        first = SceneEffect_SpawnEffect284AtCell(888, 680, gVinasuSettleScriptA);
        second = SceneEffect_SpawnEffect284AtCell(888, 680, gVinasuSettleScriptB);
        while (first->active != 0 || second->active != 0) {
            if (first->finished != 0 || second->finished != 0) {
                Engine_EventWait(30);
                Engine_AudioPlayCue(158);
                Engine_MapAnimateCells(gVinasuSettleCells, 109, 37);
                Call6((void (*)())Engine_MapCopyCellAttributes, 45, 37, 1, 1, 45, 38);
                if (Object_GetById(9)->x.fixed >> 20 == gVinasuSwitchCells[0].x
                    && Object_GetById(9)->z.fixed >> 20 == gVinasuSwitchCells[0].z) {
                    Engine_GameFlagSet(0x302);
                } else {
                    Engine_GameFlagSet(0x303);
                }
                Engine_GameFlagSet(0x304);
                break;
            }
            Engine_TaskWait(1);
        }
    }
    Engine_EventEnd();
}

void SceneState_RunConditionalStep(void)
{
    Event_Begin();
    if (SceneActor_TryMoveActorZeroTwoTilesAhead() == 0) {
        s32 k5 = 44, k6 = 39;
        Map_CopyCellAttributes(108, 39, 13, 7, k5, k6);
        RunStagedActorTransition();
    }
    Event_End();
    VinasuHeya_SettlePushedBlocks();
}

s32 SceneActor_SetHeightAboveLinkedRecord(Struct_22a4 *obj)
{
    Struct_22a4b *rec;

    rec = Actor_Get(((s16 *)obj)[50]);
    ((s32 *)obj)[3] = rec->unkC + 0x100000;
    return 0;
}

/* Moves actors 8 and 9 to the heights their table rows name, then marks the
 * cell under each of actors 8 to 12 that has sunk below the floor. */
void VinasuHeya_LowerFloatingBlocks(s32 wait)
{
    struct FieldActor *a = Object_GetById(8);
    struct FieldActor *b = Object_GetById(9);
    u32 i;

    Call3((void (*)())Engine_ActorSetSpeed, 8, 0x8000, 0x4000);
    ((void (*)())Engine_ActorSetSpeed)(9, 0x8000, 0x4000);
    if (wait != 0) {
        Engine_AudioPlayCue(180);
    }
    Call4((void (*)())Engine_ObjectSetPosition, (s32)a, a->x.fixed, gVinasuBlockHeights[(s16)a->unknown_64], a->z.fixed);
    Call4((void (*)())Engine_ObjectSetPosition, (s32)b, b->x.fixed, gVinasuBlockHeights[(s16)b->unknown_64], b->z.fixed);
    Engine_ActorWaitForMove(8);
    Engine_ActorWaitForMove(9);
    a->y.fixed = gVinasuBlockHeights[(s16)a->unknown_64];
    b->y.fixed = gVinasuBlockHeights[(s16)b->unknown_64];
    if (wait != 0) {
        Engine_AudioPlayCue(0x121);
    }
    for (i = 0; i < 5; i++) {
        struct FieldActor *actor = Object_GetById(i + 8);

        if (actor->y.fixed / 0x10000 < 0 && actor->y.fixed / 0x10000 > -30) {
            Call6((void (*)())Engine_MapCopyCellAttributes, 4, 19, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
        }
    }
    Engine_EventWait(wait);
}

/*
 * resource_3c8 owner at 0x020023d4, 168 bytes: among scene slots 8-13,
 * locate candidates sharing the selected slot's x/z tile, retain the highest
 * candidate at least one 16.16 unit above its y value, store that candidate id
 * at selected+100, then move/release the selected slot and run its local effect.
 *
 * Complete owner: high-register prologue and four-byte frame at 0x020023d4
 * through the sole interworking return at 0x02002468-0x02002474, followed by
 * alignment and one referenced pool word through 0x0200247b.  Eight static
 * calls across seven targets match independently; the two scene-accessor call
 * sites sit inside the bounded six-iteration loop.
 */
void SceneActor_PickHighestSlotAtSameTileAndRelease(s32 selector)
{

    u8 *cand;
    s32 highest = (s32)0xffb00000;
    u8 *sel = 0;
    u32 i;

    for (i = 0; i <= 5; i++) {
        s32 no = i + 8;
        s32 cand_y;

        if (no == selector) {
            continue;
        }

        cand = Actor_Get(no);
        sel = Actor_Get(selector);

        if ((*(s32 *)(cand + 8) >> 20)
                != (*(s32 *)(sel + 8) >> 20)
            || (*(s32 *)(cand + 16) >> 20)
                != (*(s32 *)(sel + 16) >> 20)) {
            continue;
        }

        cand_y = *(s32 *)(cand + 12) + 0x100000;
        if (highest <= cand_y) {
            *(u16 *)(sel + 100) = (u16)no;
            highest = cand_y;
        }
    }

    Actor_SetSpeed(selector, 0x40000, 0x20000);
    Engine_ObjectSetPosition(sel,
                  *(s32 *)(sel + 8),
                  highest,
                  *(s32 *)(sel + 16));
    Actor_WaitForMove(selector);
    Audio_PlayCue(188);
    SceneEffect_SpawnNineRadialEffects(selector);
    Event_Wait(30);
}
