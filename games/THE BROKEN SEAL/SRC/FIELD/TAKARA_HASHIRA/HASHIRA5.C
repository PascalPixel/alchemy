#include "HASHIRA.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "DMA.H"

extern u8 MsgFieldFlippedSwitch[];

/* The four background scroll pairs; the shaken copy starts at the second. */
extern u16 gBgScroll[];

#define SPRITE_OF(actor) ((struct FieldSprite *)*(s32 *)((u8 *)(actor) + 0x50))

s32 TakaraHashira_SyncPriorityIfBehind(struct FieldActor *actor, struct FieldActor *other);

struct PillarSlot {
    u8 unknown_00[16];
    s32 flag;
};

extern struct PillarSlot TakaraHashira_PillarSlots[];
#define DMA3 ((volatile u32 *)0x040000d4)

static __inline__ void Dma_Wait(volatile u32 *dma)
{
    while (dma[2] & 0x80000000)
        ;
}

s32 TakaraHashira_SyncPriorityIfAhead(struct FieldActor *front, struct FieldActor *actor);

void CopyAndOffsetCoordinatePreset(void)
{
    u32 *destination;
    const u32 *source;
    u16 *coordinates;

    source = (const u32 *)&gBgScroll[2];
    destination = (u32 *)TakaraHashira_ShakenScroll;
    *destination++ = *source++;
    *destination++ = *source++;
    *destination = *source;
    coordinates = (u16 *)TakaraHashira_ShakenScroll;
    coordinates[1] += 0xc0;
    coordinates[3] += 0xc0;
    coordinates[5] += 0xc0;
}

void FieldScene_RunScene3b3SequenceA(void)
{
    s32 record;

    record = GameFlag_IsSet(0x200);
    if (record == 0) {
        TakaraHashira_CopyCellBlock(10, 19, 16, 5, record, 10, 31);
        TakaraHashira_CopyCellBlock(10, 51, 16, 5, 1, 10, 31);
        TakaraHashira_CopyCellBlock(42, 51, 16, 5, 2, 10, 31);
    } else {
        TakaraHashira_CopyCellBlock(10, 19, 16, 5, 0, 10, 31);
        TakaraHashira_CopyCellBlock(10, 83, 16, 5, 1, 10, 31);
        TakaraHashira_CopyCellBlock(42, 83, 16, 5, 2, 10, 31);
    }
    TakaraHashira_ShakeChance = 0;
    Engine_TaskAddCallback(CopyAndOffsetCoordinatePreset, 0xc80);
    WaitFrames(1);
    Runtime_SetIrqHandler(1, 0, TakaraHashira_JitterBackgroundScroll);
    Audio_PlayCue(231);
    TakaraHashira_ShakeChance = 0;
    do {
        WaitFrames(1);
    } while (++TakaraHashira_ShakeChance <= 100);
    Audio_PlayCue(0x121);
    if (GameFlag_IsSet(0x200) == 0) {
        Map_CopyCellsTo(0, 32, 32, 0, 32, 32);
        Map_CopyCellsTo(32, 32, 64, 0, 32, 32);
    } else {
        Map_CopyCellsTo(0, 64, 32, 0, 32, 32);
        Map_CopyCellsTo(32, 64, 64, 0, 32, 32);
    }
    WaitFrames(1);
    Runtime_SetIrqHandler(1, 0, 0);
    WaitFrames(1);
    Engine_TaskRemoveCallback((s32)CopyAndOffsetCoordinatePreset);
    Engine_MapRedraw();
    WaitFrames(30);
}

/* Runs one of two near-identical setup sequences for record REC_ID and
 * records 9-15, chosen by the query call's return value; each sequence ends
 * with its own closing call carrying QUERY_FLAG. */
void FieldScene_RunFlaggedDisplayScene(void)
{
    u32 i;
    u8 *queried;
    u8 *record;

    Engine_EventBegin();
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x1190000, -1, 0x1b00000, 1);
    Engine_CameraWaitForMove();
    Engine_MessageShowCentered((s32)MsgFieldFlippedSwitch, 1);
    queried = GameFlag_IsSet(QUERY_FLAG);
    if (queried == 0) {
        Audio_PlayCue(232);
        Map_AnimateCells(TakaraHashira_ShiftSteps1, 84, 24);
        Engine_EventWait(30);
        Audio_PlayCue(240);
        Engine_ActorSetSpritePriority(REC_ID, 1);
        /* Flag byte at +85: cleared, since queried is zero here. */
        *((u8 *)Object_GetById(REC_ID) + 85) = queried;
        record = Actor_Get(REC_ID);
        *(s32 *)(record + 12) = -0x200000;
        Actor_SetPosition(REC_ID, 0x1100000, 0x1a00000);
        Engine_ActorSetAnimation(REC_ID, 1);
        Map_AnimateCells(TakaraHashira_ShiftSteps3, 80, 24);
        Map_AnimateCells(TakaraHashira_ShiftSteps5, 80, 28);
        Map_CopyCellsTo(65, 40, 16, 27, 2, 4);
        FieldScene_RunScene3b3SequenceA();
        SceneActor_ApplyPlacementQueryAndTag(9);
        SceneActor_ApplyPlacementQueryAndTag(10);
        SceneActor_ApplyPlacementQueryAndTag(11);
        SceneActor_ApplyPlacementQueryAndTag(12);
        SceneActor_ApplyPlacementQueryAndTag(13);
        SceneActor_ApplyPlacementQueryAndTag(14);
        SceneActor_ApplyPlacementQueryAndTag(15);
        Map_CopyCellAttributes(24, 3, 1, 1, 24, 8);
        GameFlag_Set(QUERY_FLAG);
    } else {
        Audio_PlayCue(232);
        Map_AnimateCells(TakaraHashira_ShiftSteps2, 84, 24);
        Engine_EventWait(30);
        Audio_PlayCue(230);
        /* Flag byte at +85: cleared unconditionally in this branch. */
        *((u8 *)Object_GetById(REC_ID) + 85) = 0;
        record = Actor_Get(REC_ID);
        *(s32 *)(record + 12) = -0x200000;
        Actor_SetPosition(REC_ID, 0x1100000, 0x1b40000);
        Engine_ActorSetAnimation(REC_ID, 2);
        Map_CopyCellsTo(65, 45, 16, 27, 2, 4);
        Map_AnimateCells(TakaraHashira_ShiftSteps4, 80, 24);
        FieldScene_RunScene3b3SequenceA();
        SceneActor_ApplyPlacementQuery(9);
        SceneActor_ApplyPlacementQuery(10);
        SceneActor_ApplyPlacementQuery(11);
        SceneActor_ApplyPlacementQuery(12);
        SceneActor_ApplyPlacementQuery(13);
        SceneActor_ApplyPlacementQuery(14);
        SceneActor_ApplyPlacementQuery(15);
        Map_CopyCellAttributes(24, 4, 1, 1, 24, 8);
        GameFlag_Clear(QUERY_FLAG);
    }
    Engine_EventEnd();
}

/* Whether actor stands directly in front of front (the mirror of
 * TakaraHashira_SyncPriorityIfAhead); when front draws in a higher sprite priority
 * number, actor takes front's priorities. */
s32 TakaraHashira_SyncPriorityIfBehind(struct FieldActor *actor, struct FieldActor *front)
{
    s32 result = 0;

    if (front->x.fixed == actor->x.fixed && front->y.fixed == actor->y.fixed
        && front->z.fixed == actor->z.fixed) {
        return result;
    }
    if (front->x.fixed - 0x100000 < actor->x.fixed && actor->x.fixed < front->x.fixed + 0x100000
        && front->y.fixed / 0x10000 == actor->y.fixed / 0x10000
        && front->z.fixed > actor->z.fixed && front->z.fixed - 0x200000 < actor->z.fixed) {
        if (SPRITE_OF(actor)->priority < SPRITE_OF(front)->priority) {
            actor->priority_flags &= ~1;
            SPRITE_OF(actor)->priority = SPRITE_OF(front)->priority;
            SPRITE_OF(actor)->second_priority = SPRITE_OF(front)->second_priority;
        }
        result = 1;
    }
    return result;
}

/* Whether actor stands directly in front of front: within a cell across,
 * on the same height and up to two cells nearer. When it does and front
 * draws in a lower sprite priority, actor takes front's priorities. */
s32 TakaraHashira_SyncPriorityIfAhead(struct FieldActor *front, struct FieldActor *actor)
{
    s32 result = 0;

    if (actor->x.fixed == front->x.fixed && actor->y.fixed == front->y.fixed
        && actor->z.fixed == front->z.fixed) {
        return result;
    }
    if (front->x.fixed - 0x100000 < actor->x.fixed && actor->x.fixed < front->x.fixed + 0x100000
        && actor->y.fixed / 0x10000 == front->y.fixed / 0x10000
        && front->z.fixed > actor->z.fixed && front->z.fixed - 0x200000 < actor->z.fixed) {
        if (SPRITE_OF(front)->priority > SPRITE_OF(actor)->priority) {
            actor->priority_flags &= ~1;
            SPRITE_OF(actor)->priority = SPRITE_OF(front)->priority;
            SPRITE_OF(actor)->second_priority = SPRITE_OF(front)->second_priority;
        }
        result = 1;
    }
    return result;
}

/* The sprite read through an s32 view of actor->sprite, so the pointer reloads
 * after each bitfield store. */
s32 TakaraHashira_UpdateActorPriority(struct FieldActor *actor)
{
    struct FieldActor *leader = Object_GetById(0);
    s32 hit;
    u32 id;

    if (((s8 *)gEventWork)[0xcc7] == 1) {
        actor->sprite->priority = leader->sprite->priority;
        actor->collision_flags |= 1;
        return 0;
    }
    hit = TakaraHashira_SyncPriorityIfBehind(actor, leader);
    for (id = 8; id <= 11; id++) {
        hit += TakaraHashira_SyncPriorityIfBehind(actor, Object_GetById(id));
    }
    if (hit != 0) {
        for (id = 8; id <= 11; id++) {
            TakaraHashira_SyncPriorityIfAhead(actor, Object_GetById(id));
        }
    }
    if (actor->y.fixed < leader->y.fixed) {
        actor->priority_flags |= 2;
        actor->collision_flags &= 254;
        if (actor->sprite->priority < leader->sprite->priority) {
            actor->priority_flags &= 254;
            SPRITE_OF(actor)->priority = SPRITE_OF(leader)->priority;
            SPRITE_OF(actor)->second_priority = SPRITE_OF(leader)->second_priority;
            hit = 1;
        }
    } else {
        actor->priority_flags &= 253;
        actor->collision_flags |= 1;
    }
    if (hit == 0) {
        actor->priority_flags |= 1;
    }
    return 0;
}

void TakaraHashira_SortPillarActors(void)
{
    u32 i;
    u32 j;
    struct FieldActor *a;
    struct FieldActor *b;
    u8 actor[112];
    /* Holds the first 16 bytes of a slot; the copy runs over into actor, which is no longer needed. */
    s32 slot[3];

    for (i = 0; i <= 2; i++) {
        a = Object_GetById(i + 8);
        for (j = i; j <= 3; j++) {
            b = Object_GetById(j + 8);
            if (a->y.fixed <= b->y.fixed && a->z.fixed < b->z.fixed) {
                continue;
            }
            Dma_Set(b, actor, 0x8400001c, DMA3);
            Dma_Wait(DMA3);
            Dma_Set(a, b, 0x8400001c, DMA3);
            Dma_Wait(DMA3);
            Dma_Set(actor, a, 0x8400001c, DMA3);
            Dma_Wait(DMA3);
            Dma_Set(&TakaraHashira_PillarSlots[j], slot, 0x84000004, DMA3);
            Dma_Wait(DMA3);
            Dma_Set(&TakaraHashira_PillarSlots[i], &TakaraHashira_PillarSlots[j], 0x84000004, DMA3);
            Dma_Wait(DMA3);
            Dma_Set(slot, &TakaraHashira_PillarSlots[i], 0x84000004, DMA3);
            Dma_Wait(DMA3);
            if (Engine_GameFlagIsSet(TakaraHashira_PillarSlots[i].flag) && !Engine_GameFlagIsSet(TakaraHashira_PillarSlots[j].flag)) {
                Engine_GameFlagClear(TakaraHashira_PillarSlots[i].flag);
                Engine_GameFlagSet(TakaraHashira_PillarSlots[j].flag);
            } else if (!Engine_GameFlagIsSet(TakaraHashira_PillarSlots[i].flag) && Engine_GameFlagIsSet(TakaraHashira_PillarSlots[j].flag)) {
                Engine_GameFlagSet(TakaraHashira_PillarSlots[i].flag);
                Engine_GameFlagClear(TakaraHashira_PillarSlots[j].flag);
            }
        }
    }
}
