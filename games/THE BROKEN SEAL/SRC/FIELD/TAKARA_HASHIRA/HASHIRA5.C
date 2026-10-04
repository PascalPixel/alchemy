#include "HASHIRA.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "DMA.H"

extern u8 MsgFieldFlippedSwitch[];

/* The four background scroll pairs; the shaken copy starts at the second. */
extern u16 gBgScroll[];

#define SPRITE_OF(actor) ((struct FieldSprite *)*(s32 *)((u8 *)(actor) + 0x50))

s32 TakaraHashira_SyncPriorityIfBehind(struct FieldActor *actor, struct FieldActor *other);

struct MapCell {
    u32 tile : 12;
    u32 layer : 2;
    u32 kind : 2;
    u32 height : 8;
    u32 event : 8;
};

/* A pillar's last cell position, the map cell it covered and the flag it
   raises when it stands on another pillar. */
struct PillarSlot {
    s32 x;
    s32 y;
    s32 z;
    struct MapCell cell;
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

    record = Engine_GameFlagIsSet(0x200);
    if (record == 0) {
        Map_DrawCellPanel(10, 19, 16, 5, record, 10, 31);
        Map_DrawCellPanel(10, 51, 16, 5, 1, 10, 31);
        Map_DrawCellPanel(42, 51, 16, 5, 2, 10, 31);
    } else {
        Map_DrawCellPanel(10, 19, 16, 5, 0, 10, 31);
        Map_DrawCellPanel(10, 83, 16, 5, 1, 10, 31);
        Map_DrawCellPanel(42, 83, 16, 5, 2, 10, 31);
    }
    TakaraHashira_ShakeChance = 0;
    Engine_TaskAddCallback(CopyAndOffsetCoordinatePreset, 0xc80);
    WaitFrames(1);
    Runtime_SetIrqHandler(1, 0, TakaraHashira_JitterBackgroundScroll);
    Engine_AudioPlayCue(231);
    TakaraHashira_ShakeChance = 0;
    do {
        WaitFrames(1);
    } while (++TakaraHashira_ShakeChance <= 100);
    Engine_AudioPlayCue(0x121);
    if (Engine_GameFlagIsSet(0x200) == 0) {
        Engine_MapCopyCellsTo(0, 32, 32, 0, 32, 32);
        Engine_MapCopyCellsTo(32, 32, 64, 0, 32, 32);
    } else {
        Engine_MapCopyCellsTo(0, 64, 32, 0, 32, 32);
        Engine_MapCopyCellsTo(32, 64, 64, 0, 32, 32);
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
    struct FieldActor *record;

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
        Object_GetById(REC_ID)->motion_flags = queried;
        record = Actor_Get(REC_ID);
        record->y.fixed = -0x200000;
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
        Object_GetById(REC_ID)->motion_flags = 0;
        record = Actor_Get(REC_ID);
        record->y.fixed = -0x200000;
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

/* A pillar block's object record, as the pillar update reads it. */
struct PillarActor {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[14];
    u8 unknown_22;
    u8 priority_flags;
    s32 velocity_x;
    s32 velocity_y;
    u8 unknown_2c[0x29];
    u8 motion_flags;
};

/* Where a moved pillar's position is copied to test it for collisions. */
extern s32 TakaraHashira_ProbePosition[];

s32 GetMapCellCollision(s32 layer, s32 x, s32 z);
void TakaraHashira_SetCellAttributes(s32 layer, s32 x, s32 y, struct MapCell *src);
void TakaraHashira_ReadMapCell(s32 layer, s32 x, s32 y, struct MapCell *dst);
s32 TakaraHashira_LowerActorToLedge(s32 id, s32 far);
void SceneActor_WaitHeightBelowLimit(struct FieldActor *actor, s32 limit);
void StagedActor_StepDownUntilClamp(s32 id);
void OverlayObject_WaitUntilSettledAndReset(struct FieldActor *actor);

/* Each frame, for pillars 8 to 11: a pillar that moved since its slot was
 * written restores the cells it covered, drops onto a ledge or into a pit
 * when its move left it unsupported, takes the cells where it now stands,
 * and raises the flag of each pillar it stands on. Last the pillars are
 * sorted. */
void TakaraHashira_UpdatePillarActors(void)
{
    u32 id;
    struct PillarActor *actor;
    u8 *flags;
    u32 i;
    u32 k;
    struct PillarActor *other;
    struct MapCell cell;

    for (id = 8; id <= 11; id++) {
        actor = (struct PillarActor *)Object_GetById(id);
        actor->unknown_22 = 2;
        i = id - 8;
        if ((actor->x >> 20) == TakaraHashira_PillarSlots[i].x && (actor->z >> 20) == TakaraHashira_PillarSlots[i].z
            && actor->velocity_y == 0) {
            continue;
        }
        Dma_Set(&actor->x, TakaraHashira_ProbePosition, 0x84000003, DMA3);
        Dma_Wait(DMA3);
        if (Object_CheckMovementCollision(actor, TakaraHashira_ProbePosition) == -1) {
            actor->motion_flags = 3;
        }
        flags = &actor->motion_flags;
        TakaraHashira_SetCellAttributes(0, TakaraHashira_PillarSlots[i].x, TakaraHashira_PillarSlots[i].z,
                                        &TakaraHashira_PillarSlots[i].cell);
        TakaraHashira_SetCellAttributes(2, TakaraHashira_PillarSlots[i].x, TakaraHashira_PillarSlots[i].z,
                                        &TakaraHashira_PillarSlots[i].cell);
        if (*flags & 1) {
            if (GetMapCellCollision(2, actor->x, actor->z) == 50) {
                Engine_AudioPlayCue(189);
                /* FAKEMATCH: the byte view keeps these flag updates out of the record's alias set; field stores reallocate the loop (score 2573). */
                *(u8 *)&actor->priority_flags &= 254;
                TakaraHashira_LowerActorToLedge(id, 1);
                *(u8 *)&actor->priority_flags |= 1;
            } else if (GetMapCellCollision(2, actor->x, actor->z) == 51) {
                SceneActor_WaitHeightBelowLimit((struct FieldActor *)actor, 0);
                Engine_AudioPlayCue(189);
                actor->y = 0;
                /* FAKEMATCH: the byte view keeps these flag updates out of the record's alias set; field stores reallocate the loop (score 2573). */
                *(u8 *)&actor->priority_flags &= 254;
                StagedActor_StepDownUntilClamp(id);
                actor->x = 0;
                actor->y = 0;
                actor->z = 0;
                *(u8 *)&actor->priority_flags |= 1;
            } else {
                OverlayObject_WaitUntilSettledAndReset((struct FieldActor *)actor);
            }
            *flags = 0;
        }
        TakaraHashira_ReadMapCell(0, actor->x >> 20, actor->z >> 20, &TakaraHashira_PillarSlots[i].cell);
        if (actor->y >= 0) {
            TakaraHashira_ReadMapCell(0, 27, (actor->y >> 20) + 6, &cell);
            TakaraHashira_SetCellAttributes(0, actor->x >> 20, actor->z >> 20, &cell);
            cell.kind = TakaraHashira_PillarSlots[i].cell.kind;
            TakaraHashira_SetCellAttributes(2, actor->x >> 20, actor->z >> 20, &cell);
        }
        TakaraHashira_PillarSlots[i].x = actor->x >> 20;
        TakaraHashira_PillarSlots[i].y = actor->y >> 20;
        TakaraHashira_PillarSlots[i].z = actor->z >> 20;
        for (k = 0; k <= 3; k++) {
            if (k == i) {
                continue;
            }
            Engine_GameFlagClear(TakaraHashira_PillarSlots[k].flag);
            other = (struct PillarActor *)Object_GetById(k + 8);
            if ((actor->x >> 20) == (other->x >> 20) && (actor->z >> 20) == (other->z >> 20)
                && actor->y > other->y) {
                Engine_GameFlagSet(TakaraHashira_PillarSlots[k].flag);
            }
        }
    }
    TakaraHashira_SortPillarActors();
}
