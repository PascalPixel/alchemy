#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "text/MSG_IDS.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALLBACK_SCHEDULER.H"
#include "CALL.H"

extern const struct SceneEntrance gKuupuappuDouEntrances1[];
extern const struct SceneEntrance gKuupuappuDouEntrances2[];
extern const struct SceneEntrance gKuupuappuDouEntrances3[];
extern const struct SceneEntrance gKuupuappuDouEntrancesOther[];

void Vector_AddPolarOffset(s32, s32, s32 *);
void Object_SetMoveTarget(s32 *, s32, s32, s32);
void KuupuappuDou_PushBlockAhead();
s32 SceneActor_LiftLowActorOnSubjectTile(s32 subject_actor);
void KuupuappuDou_SpawnPuffs();
s32 Math_RemainderUnsigned();

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
};

extern void KuupuappuDou_RaiseActorPriorities(void);

TEXT_MESSAGE_ENUM(MsgFieldDoorTightlyLocked);
TEXT_MESSAGE_ENUM(MsgFieldFlippedSwitch);

extern 

extern const u32 gKuupuappuDouExits1[];
extern const u32 gKuupuappuDouExits2[];
extern const u32 gKuupuappuDouExits3[];
extern const u32 gKuupuappuDouExitsOther[];
extern const struct ScenePlacement gKuupuappuDouPlacements1[];
extern const struct ScenePlacement gKuupuappuDouPlacements2[];
extern const struct ScenePlacement gKuupuappuDouPlacements3[];
extern const struct ScenePlacement gKuupuappuDouPlacementsOther[];

s32 Object_CheckMovementCollision(struct FieldActor *actor, union FieldCoordinate *pos);
void Engine_ObjectCommitPosition(struct FieldActor *object);

/* One cell's step for each sixteenth of a turn: pixels across in the high
 * half, pixels along in the low half. */
extern s32 KuupuappuDou_DirectionSteps[];

extern const struct SceneEvent gKuupuappuDouEvents1[];
extern const struct SceneEvent gKuupuappuDouEvents2[];
extern const struct SceneEvent gKuupuappuDouEvents3[];
extern const struct SceneEvent gKuupuappuDouEventsOther[];

void SceneState_SetEntries16To21Byte35(void);

struct MapShake {
    u8 unknown_00[0x18];
    s32 direction_x;
    s32 direction_z;
    u8 unknown_20[4];
    s32 scroll;
};

extern u32 gFrameCount;
void QueueIoWriteDelay2(u32 address, s32 value);
void Engine_MapRenderWaitForValues(void);

void KuupuappuDou_RunRumble(void);

struct SceneActorRecord {
    u8 reserved_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[20];
    s32 field_28;
    u8 reserved_2c[28];
    s32 field_48;
};

#define TILE(position) ((position) / 0x100000)

extern s32 KuupuappuDou_ScheduleFrames[];
extern s32 KuupuappuDou_ScheduleTimer;
extern s32 KuupuappuDou_ScheduleIndex;
extern s32 KuupuappuDou_TickCounter;
extern s32 KuupuappuDou_TickValue;

extern const s32 KuupuappuDou_PuffScript[];

void FieldScene_RunOpeningAuxiliarySequence(void);
void FieldScene_RunScene3a7SequenceD(void);

void SceneActor_InitSlots10To15AndStartTask(void);
void SceneState_ApplyRectAndMarkActor16(void);
void SceneState_ConfigureRegion26_30AndMarkActor17(void);
void SceneState_ConfigureRegion26_30AndClearActor18Mode(void);
void SceneState_ApplyRectAndSetupActor19(void);
void SceneActor_SetupSlotTwenty(void);
void SceneActor_MarkSlot21AndSetFlag205(void);
void SceneState_ApplyThreeRects(void);
void SceneActor_SetupActors11To14AndInstallTask(void);
void SceneState_ApplyThreeRectsRows9And10(void);
void SceneState_DispatchByActorZeroDepth(void);

/* Where the party appears in each of the cave's three areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KuupuappuDou1) {
        return gKuupuappuDouEntrances1;
    }
    if (scene == (s32)&SceneId_KuupuappuDou2) {
        return gKuupuappuDouEntrances2;
    }
    if (scene == (s32)&SceneId_KuupuappuDou3) {
        return gKuupuappuDouEntrances3;
    }
    return gKuupuappuDouEntrancesOther;
}

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Deliberate no-op callback. */
u8 *SceneData_GetTable9C5C(void)
{
    return (u8 *)0x02009c5c;
}

/* Where each area's exits lead. */
const u32 *Scene_GetExits(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KuupuappuDou1) {
        return gKuupuappuDouExits1;
    }
    if (scene == (s32)&SceneId_KuupuappuDou2) {
        return gKuupuappuDouExits2;
    }
    if (scene == (s32)&SceneId_KuupuappuDou3) {
        return gKuupuappuDouExits3;
    }
    return gKuupuappuDouExitsOther;
}

/* The actors placed in each area. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KuupuappuDou1) {
        return gKuupuappuDouPlacements1;
    }
    if (scene == (s32)&SceneId_KuupuappuDou2) {
        return gKuupuappuDouPlacements2;
    }
    if (scene == (s32)&SceneId_KuupuappuDou3) {
        return gKuupuappuDouPlacements3;
    }
    return gKuupuappuDouPlacementsOther;
}

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Deliberate no-op callback. */
s32 *SceneData_FindActiveSlotAtCell(s32 cx, s32 cz)
{
    s32 **slots = (s32 **)((u8 *)gEventWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (cx == (p[2] >> 20) && cz == (p[4] >> 20) && *((u8 *)p + 0x59) != 0) {
            return p;
        }
    }
    return 0;
}

/* The leader pushes the block in the cell ahead one cell further, unless it
 * is one of actors 11 to 14, something solid lies beyond it or the landing
 * cell is higher; leader and block then move together. */
void KuupuappuDou_PushBlockAhead(void)
{
    struct FieldActor *leader = Object_GetById(0);
    s32 direction = leader->facing >> 12;
    struct FieldActor *block;
    struct FieldActor *beyond;
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;
    s32 zero;
    s32 i;
    s32 cx;
    s32 cz;

    cx = (leader->x.part.pixel + (KuupuappuDou_DirectionSteps[direction] >> 16)) >> 4;
    cz = (leader->z.part.pixel + (s16)KuupuappuDou_DirectionSteps[direction]) >> 4;
    block = (struct FieldActor *)SceneData_FindActiveSlotAtCell(cx, cz);
    if (block->collision_flags == 0 || block == 0) {
        return;
    }
    for (i = 0; i <= 3; i++) {
        if (block == Object_GetById(i + 11)) {
            return;
        }
    }
    cx = (block->x.part.pixel + (KuupuappuDou_DirectionSteps[direction] >> 16)) >> 4;
    cz = (block->z.part.pixel + (s16)KuupuappuDou_DirectionSteps[direction]) >> 4;
    beyond = (struct FieldActor *)SceneData_FindActiveSlotAtCell(cx, cz);
    if (beyond != 0 && (beyond->collision_flags & 1)) {
        return;
    }
    block->unknown_22 = 2;
    zero = 0;
    p = pos;
    p[0].fixed = block->x.fixed + (KuupuappuDou_DirectionSteps[direction] & 0xffff0000);
    p[1].fixed = block->y.fixed;
    p[2].fixed = block->z.fixed + (KuupuappuDou_DirectionSteps[direction] << 16);
    if (Object_CheckMovementCollision(block, p) > 0) {
        return;
    }
    Object_SetMode(leader, 8);
    Engine_TaskWait(15);
    Engine_AudioPlayCue(238);
    block->speed = 0x3333;
    block->acceleration = 0x3333;
    Engine_ObjectSetPosition(block, p[0].fixed, p[1].fixed, p[2].fixed);
    leader->speed = 0x3333;
    leader->acceleration = 0x3333;
    Engine_ObjectSetPosition(leader, p[0].fixed, p[1].fixed, p[2].fixed);
    Engine_ObjectCommitPosition(block);
    Engine_AudioPlayCue(288);
    block->x.fixed = p[0].fixed;
    block->z.fixed = p[2].fixed;
    block->velocity_x = zero;
    block->velocity_z = zero;
    leader->velocity_x = zero;
    leader->velocity_z = zero;
    leader->target_x = ACTOR_NO_TARGET;
    leader->target_z = ACTOR_NO_TARGET;
    Object_SetMode(leader, 1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Deliberate no-op callback. */
void SceneState_ApplyFlag300(void)
{
    Engine_GameFlagSet(0x300);
}

void SceneState_SetFlag953(void)
{
    Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
}

/* What each area answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KuupuappuDou1) {
        return gKuupuappuDouEvents1;
    }
    if (scene == (s32)&SceneId_KuupuappuDou2) {
        return gKuupuappuDouEvents2;
    }
    if (scene == (s32)&SceneId_KuupuappuDou3) {
        return gKuupuappuDouEvents3;
    }
    return gKuupuappuDouEventsOther;
}

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Deliberate no-op callback. */
s32 IsActor9AtTile15x54(void)
{
    s32 *actor = Object_GetById(9);
    s32 z = actor[4];
    s32 x;
    s32 z_tile;
    s32 x_tile;

    if (z < 0) {
        z += 0x000FFFFF;
    }
    x = actor[2];
    z_tile = z >> 20;
    if (x < 0) {
        x += 0x000FFFFF;
    }
    x_tile = x >> 20;
    if (x_tile == 15 && z_tile == 54) {
        return 1;
    }
    return 0;
}

void FieldScene_RunFlag9a9GuardedScene(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x9a9) == 0) {
        KuupuappuDou_PushBlockAhead();
        if (IsActor9AtTile15x54()!= 0) {
            GameFlag_Set(0x9a9);
            Audio_PlayCue(80);
            SceneState_ApplyThreeRects();
        }
    }
}

void SceneState_ApplyThreeRects(void)
{
    s32 strip = 16;

    {
        s32 fifth = 80;
        s32 sixth = 50;

        Engine_MapCopyCells(87, 50, 2, 4, fifth, sixth);
    }
    Engine_MapCopyCells(23, 52, 1, 2, strip, 52);
    Engine_MapCopyCellAttributes(16, 52, 1, 1, strip, 53);
}

void FieldScene_RunScene3a7SequenceA(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x9a9) == 0) {
        if (IsActor9AtTile15x54()!= 0) {
            GameFlag_Set(0x9a9);
            Audio_PlayCue(80);
            SceneState_ApplyThreeRects();
        }
    }
}

void FieldScene_NoOp(void) {}

void SceneState_ApplyThreeRectsRows9And10(void)
{
    s32 strip = 17;

    {
        s32 p5 = 80;
        s32 p6 = 9;

        Engine_MapCopyCells(90, 9, 2, 3, p5, p6);
    }
    Engine_MapCopyCells(27, 10, 1, 2, strip, 10);
    Engine_MapCopyCellAttributes(17, 10, 1, 1, strip, 11);
}

s32 SceneActor_IsActor10AtTile16x12(void)
{
    s32 *p = Object_GetById(10);
    s32 z = p[4];
    s32 x;
    s32 cz;
    s32 cx;

    if (z < 0) {
        z += 0x000FFFFF;
    }
    x = p[2];
    cz = z >> 20;
    if (x < 0) {
        x += 0x000FFFFF;
    }
    cx = x >> 20;
    if (cx == 16 && cz == 12) {
        return 1;
    }
    return 0;
}

void FieldScene_RunGuardedStep9AAAfterSetup(void)
{
    u32 i;
    s32 record;

    KuupuappuDou_PushBlockAhead();
    if (GameFlag_IsSet(0x9aa) == 0) {
        if (SceneActor_IsActor10AtTile16x12()!= 0) {
            if (GameFlag_IsSet(0x207) == 0) {
                Audio_PlayCue(80);
                SceneState_ApplyThreeRectsRows9And10();
                GameFlag_Set(0x9aa);
            }
        }
    }
}

void Resource3a7_NoOpCallback(void)
{
}

void FieldScene_RunGuardedStep9AA(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x9aa) == 0) {
        if (SceneActor_IsActor10AtTile16x12()!= 0) {
            if (GameFlag_IsSet(0x207) == 0) {
                Audio_PlayCue(80);
                SceneState_ApplyThreeRectsRows9And10();
                GameFlag_Set(0x9aa);
            }
        }
    }
}

void SceneState_ApplyRectAndMarkActor16(void)
{
    u8 *rec = Object_GetById(16);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 23;
    s32 sixth = 32;

    Engine_MapCopyCellAttributes(26, 30, 1, 1, fifth, sixth);

    if (rec != 0) {
        /* The rec is reloaded with the same selector before this store. */
        ((u8 *)Object_GetById(16))[85] = 0;
        rec[35] = 1;
    }

    Engine_GameFlagSet(0x200);
}

void SceneState_ConfigureRegion26_30AndMarkActor17(void)
{
    u8 *rec = Object_GetById(17);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 p5 = 23;
    s32 p6 = 34;

    Engine_MapCopyCellAttributes(26, 30, 1, 1, p5, p6);

    if (rec != 0) {
        /* The record is reloaded with the same selector before this store. */
        ((u8 *)Object_GetById(17))[85] = 0;
        rec[35] = 1;
    }

    Engine_GameFlagSet(0x201);
}

void SceneState_ConfigureRegion26_30AndClearActor18Mode(void)
{
    u8 *record = Object_GetById(18);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a = 24;
    s32 b = 34;

    Engine_MapCopyCellAttributes(26, 30, 1, 1, a, b);

    if (record != 0) {
        /* The record is reloaded with the same selector before this store. */
        ((u8 *)Object_GetById(18))[85] = 0;
        record[35] = 1;
    }

    Engine_GameFlagSet(0x202);
}

void SceneState_ApplyRectAndSetupActor19(void)
{
    u8 *p = Object_GetById(19);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a5 = 26;
    s32 a6 = 32;

    Engine_MapCopyCellAttributes(26, 30, 1, 1, a5, a6);

    if (p != 0) {
        Engine_ActorSetSpriteFlags(p, 0);
        /* The record is reloaded with the same selector before this store. */
        ((u8 *)Object_GetById(19))[85] = 0;
        p[35] = 1;
    }

    Engine_GameFlagSet(0x203);
}

void SceneActor_SetupSlotTwenty(void)
{
    u8 *rec = Object_GetById(20);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 26;
    s32 sixth = 34;

    Engine_MapCopyCellAttributes(26, 30, 1, 1, fifth, sixth);

    if (rec != 0) {
        Engine_ActorSetSpriteFlags(rec, 0);
        /* The rec is reloaded with the same selector before this store. */
        ((u8 *)Object_GetById(20))[85] = 0;
        rec[35] = 1;
    }

    Engine_GameFlagSet(0x204);
}

void SceneActor_MarkSlot21AndSetFlag205(void)
{
    u8 *record = Object_GetById(21);
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 28;
    s32 sixth = 33;

    Engine_MapCopyCellAttributes(26, 30, 1, 1, fifth, sixth);

    if (record != 0) {
        Engine_ActorSetSpriteFlags(record, 0);
        /* The record is reloaded with the same selector before this store. */
        ((u8 *)Object_GetById(21))[85] = 0;
        record[35] = 1;
    }

    Engine_GameFlagSet(0x205);
}

void SceneState_DispatchByActorZeroDepth(void)
{
    struct Actor *p = Object_GetById(ACTOR_PARTY_LEADER);

    if (p->f0c >= 0x100000) {
        KuupuappuDou_RaiseActorPriorities();
    } else {
        SceneState_SetEntries16To21Byte35();
    }
}

void KuupuappuDou_RaiseActorPriorities(void)
{
    s32 i;
    s32 id;

    id = 16;
    for (i = 0; i < 6; i++) {
        Object_GetById(id)->priority_flags |= 2;
        id++;
    }
}

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Deliberate no-op callback. */
void SceneState_SetEntries16To21Byte35(void)
{
    s32 index = 16;
    s32 flag = 1;
    s32 remaining = 5;

    do {
        u8 *entry = Object_GetById(index);

        remaining--;
        entry[35] = flag;
        index++;
    } while (remaining >= 0);
}

/* The cave rumbles: the map shakes for 480 frames, drifting its scroll by
   a random amount each frame, then the second layer fades out, the opened
   passage is copied into the map and the chime plays. */
void KuupuappuDou_RunRumble(void)
{
    struct MapShake *shake = (struct MapShake *)(((u8 *)gMapWork[0]) + 0x164);
    s32 frames;
    s32 eva;
    s32 evb;
    s32 top;

    Engine_EventBegin();
    if (gFrameCount & 1) {
        shake->direction_x = 1;
        shake->direction_z = 1;
    } else {
        shake->direction_x = -1;
        shake->direction_z = -1;
    }
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Audio_PlayCue(163);
    for (frames = 0x1df; frames >= 0; frames--) {
        u32 drift = (u32)(Random_Next() << 11) >> 16;

        shake->scroll = (double)shake->scroll - (4718.592 - (double)drift);
        Engine_EventWait(1);
    }
    eva = 6;
    evb = 6;
    frames = 0;
    top = eva << 10;
    do {
        QueueIoWriteDelay2(0x4000052, top | (eva << 5) | evb);
        Engine_EventWait(1);
        if (frames % 20 == 0) {
            evb--;
            eva--;
        }
        frames++;
    } while (frames <= 69);
    Map_CopyCells(19, 83, 15, 8, 19, 91);
    Audio_PlayCue(0x120);
    Engine_MapRedraw();
    Engine_MapRenderWaitForValues();
    Engine_EventEnd();
}

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Deliberate no-op callback. */
void FieldScene_RunScene3a7SequenceB(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    if (GameFlag_IsSet(0x9a8) == 0) {
        Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
        GameFlag_Set(0x9a8);
        v5 = 27;
        v6 = 92;
        Audio_PlayCue(155);
        Map_CopyCells(107, 27, 1, 1, v6, v5);
        Engine_EventWait(39);
        Map_CopyCells(108, 27, 1, 1, v6, v5);
        Engine_EventWait(50);
        v6 = 25;
        Audio_PlayCue(156);
        Map_CopyCells(1, 24, 1, 2, v6, v5);
        Engine_EventWait(40);
        Map_CopyCells(2, 24, 1, 2, v6, v5);
        Engine_EventWait(40);
        KuupuappuDou_RunRumble();
    }
}

/* Checks actors 11 to 14 in turn against the subject actor. The first one
 * whose y lies above 0 and below one tile, and that stands on the subject's
 * tile, is moved up to y 255 with fields 0x28 and 0x48 cleared, and the check
 * returns 1; otherwise it returns 0. */
s32 SceneActor_LiftLowActorOnSubjectTile(s32 subject_actor)
{
    struct SceneActorRecord *subject = (struct SceneActorRecord *)Object_GetById(subject_actor);
    s32 index = 0;

    do {
        struct SceneActorRecord *actor = (struct SceneActorRecord *)Object_GetById(index + 11);

        if ((u32)(actor->y - 1) <= 0x000ffffe) {
            s32 actor_z = TILE(actor->z);
            s32 actor_x = TILE(actor->x);
            s32 subject_z = TILE(subject->z);
            s32 subject_x = TILE(subject->x);
            s32 z_delta = subject_z - actor_z;
            s32 x_delta = subject_x - actor_x;

            if (x_delta == 0 && z_delta == 0) {
                actor->y = 0x00ff0000;
                actor->field_48 = 0;
                actor->field_28 = 0;
                return 1;
            }
        }

        index++;
    } while (index <= 3);

    return 0;
}

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Deliberate no-op callback. */
void FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 i;
    u8 *rec7;
    s32 flag;
    s32 index;

    flag = *(u8 *)((u8 *)Object_GetById(10) + 91);
    if (flag == 0) {
        if (++KuupuappuDou_ScheduleTimer > 190) {
            KuupuappuDou_ScheduleTimer = 0;
        }
        index = KuupuappuDou_ScheduleIndex;
        if (KuupuappuDou_ScheduleFrames[index] == KuupuappuDou_ScheduleTimer) {
            rec7 = Actor_Get((index + 11));
            *(s32 *)(rec7 + 72) = 0xa3d;
            if (++KuupuappuDou_ScheduleIndex > 3) {
                KuupuappuDou_ScheduleIndex = 0;
            }
        }
        for (i = 0; i <= 3; i++) {
            rec7 = Actor_Get((i + 11));
            if (*(s32 *)(rec7 + 40) >= 0) {
                if (*(s32 *)(rec7 + 12) <= 0xffff) {
                    KuupuappuDou_SpawnPuffs();
                    *(s32 *)(rec7 + 12) = 0xff0000;
                    *(s32 *)(rec7 + 72) = 0;
                    *(s32 *)(rec7 + 40) = 0;
                    rec7[91] = 0;
                    Audio_PlayCue(106);
                }
            }
        }
        if (SceneActor_LiftLowActorOnSubjectTile(10) != 0) {
            Engine_ActorSetAnimation(10, 1);
            if (GameFlag_IsSet(0x207) == 0) {
                GameFlag_Set(0x207);
                Audio_PlayCue(204);
            } else {
                Audio_PlayCue(106);
            }
        }
        if (SceneActor_LiftLowActorOnSubjectTile(9) != 0) {
            Audio_PlayCue(106);
        }
    }
}

void FieldScene_RunScene3a7SequenceD(void)
{

    s32 i;
    u8 *rec7;
    s32 record;
    s32 *selected;

    rec7 = (u8 *)Object_GetById(10);
    if (rec7[91] == 0) {
        if ((++KuupuappuDou_TickCounter & 63) == 0) {
            selected = &KuupuappuDou_TickValue;
            record = Engine_RandomNext();
            record = Math_RemainderUnsigned(record, 6);
            *selected = record;
            rec7 = Object_GetById((record + 10));
            *(s32 *)(rec7 + 72) = 0xa3d;
        }
        for (i = 0; i <= 5; i++) {
            rec7 = Object_GetById((i + 10));
            record = Engine_GameFlagIsSet((i + 0x200));
            if (record != 0) {
                if (*(s32 *)(rec7 + 40) <= 0) {
                    if (*(s32 *)(rec7 + 12) > 0x20ffff) {
                        continue;
                    }
                }
                *(s32 *)(rec7 + 12) = 0xff0000;
                *(s32 *)(rec7 + 72) = 0;
                *(s32 *)(rec7 + 40) = 0;
                Engine_AudioPlayCue(106);
            } else {
                if (*(s32 *)(rec7 + 40) <= 0) {
                    if (*(s32 *)(rec7 + 12) > 0xffff) {
                        continue;
                    }
                }
                *(s32 *)(rec7 + 72) = record;
                *(s32 *)(rec7 + 40) = record;
                *(s32 *)(rec7 + 12) = 0xff0000;
                Engine_AudioPlayCue(106);
            }
        }
    }
}

void SceneActor_TransformAndApplyRecordPosition(s32 *rec, s32 v0, s32 v1)
{
    s32 pos[3];

    if (rec == 0) {
        return;
    }
    pos[0] = rec[2];
    pos[1] = rec[3];
    pos[2] = rec[4];
    Vector_AddPolarOffset(v0, v1, pos);
    Object_SetMoveTarget(rec, pos[0], pos[1], pos[2]);
}

/* Spawn up to four small type-240 objects at the source, rising with a random speed and a random turn, each running the puff script. */
void KuupuappuDou_SpawnPuffs(struct FieldActor *source)
{
    struct FieldActor *puff;
    s32 i;

    for (i = 0; i < 4; i++) {
        puff = Engine_ObjectCreate(240, source->x.fixed, source->y.fixed, source->z.fixed);
        if (puff == 0)
            break;
        puff->scale_y = 0x8ccc;
        puff->scale_x = 0x8ccc;
        puff->motion_flags = 2;
        puff->velocity_y = -0x10000;
        puff->speed = Engine_RandomNext() + 0xcccc;
        puff->collision_flags = 1;
        SceneActor_TransformAndApplyRecordPosition(puff, 0x200000, Engine_RandomNext());
        {
            u16 *timer = (u16 *)&puff->unknown_5d[1];
            s32 frames = 8;

            *timer = frames;
        }
        Engine_ObjectSetScript(puff, KuupuappuDou_PuffScript);
    }
}

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Deliberate no-op callback. */
void SceneActor_InitSlots10To15AndStartTask(void)
{
    s32 selector = 10;
    s32 remaining = 5;

    do {
        s32 *record;

        Engine_ActorSetSpriteFlags(Object_GetById(selector), 0);
        record = Object_GetById(selector);
        record[17] = 0x1999;
        record[18] = 0;
        remaining--;
        record[3] = 0x00ff0000;
        selector++;
    } while (remaining >= 0);

    {
        s32 rank = 0xc80;

        Scheduler_AddOrUpdateCallback((s32)FieldScene_RunScene3a7SequenceD, rank);
    }
}

void SceneActor_SetupActors11To14AndInstallTask(void)
{
    s32 no = 11;
    s32 i = 0;

    do {
        s32 *rec;

        Engine_ActorSetSpriteFlags(Object_GetById(no), 0);
        rec = Object_GetById(no);
        rec[17] = 0x1999;
        rec[18] = 0;
        rec[3] = 0x00ff0000;
        Engine_ActorSetSpritePriority(i + 11, 1);
        i++;
        no++;
    } while (i <= 3);

    {
        s32 rate = 0xc80;

        Scheduler_AddOrUpdateCallback((s32)FieldScene_RunOpeningAuxiliarySequence, rate);
    }
}

/*
 * Kuupuappu Cave entry: open the screen with the window transition, then
 * for each of the cave's three areas set its actors and map cells for the
 * story so far. Retreat returns the party to the first area.
 */
s32 KuupuappuDou_ApplyEntryState(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_KuupuappuDou1) {
        switch (gGameState.entrance) {
        case 5:
        case 6:
        case 7:
        case 8:
        case 13:
            if (Engine_GameFlagIsSet(0x9a8) == 0) {
                Call6((void (*)())Engine_MapCopyCellAttributes, 22, 29, 1, 1, 21, 29);
            } else {
                Engine_MapCopyCells(108, 27, 1, 1, 92, 27);
                Engine_TaskWait(1);
                Call6((void (*)())Engine_MapCopyCells, 19, 83, 15, 8, 19, 91);
                Engine_TaskWait(1);
                Engine_MapCopyCells(2, 24, 1, 2, 25, 27);
            }
            Engine_MapRedraw();
            Engine_TaskWait(1);
            break;
        case 10:
            Engine_GameFlagClear(0x9a8);
            break;
        }
    }
    if (gGameState.scene == (s32)&SceneId_KuupuappuDou2) {
        if (Engine_GameFlagIsSet(0x300) == 0) {
            Object_GetById(22)->scale_y = 0x18000;
        }
        switch (gGameState.entrance) {
        case 1:
        case 2:
        case 3:
        case 4:
            if (Engine_GameFlagIsSet(0x9a8) == 0) {
                Call6((void (*)())Engine_MapCopyCells, 5, 81, 11, 7, 5, 73);
            } else {
                Call6((void (*)())Engine_MapCopyCellAttributes, 5, 12, 1, 1, 6, 12);
                Engine_MapCopyCellAttributes(12, 10, 1, 1, 12, 11);
            }
            break;
        case 8:
        case 9:
        case 14:
            SceneActor_InitSlots10To15AndStartTask();
            if (Engine_GameFlagIsSet(0x200) != 0) {
                Engine_ActorSetAnimation(16, 5);
                SceneState_ApplyRectAndMarkActor16();
            }
            if (Engine_GameFlagIsSet(0x201) != 0) {
                Engine_ActorSetAnimation(17, 5);
                SceneState_ConfigureRegion26_30AndMarkActor17();
            }
            if (Engine_GameFlagIsSet(0x202) != 0) {
                Engine_ActorSetAnimation(18, 5);
                SceneState_ConfigureRegion26_30AndClearActor18Mode();
            }
            if (Engine_GameFlagIsSet(0x203) != 0) {
                Engine_ActorSetAnimation(19, 5);
                SceneState_ApplyRectAndSetupActor19();
            }
            if (Engine_GameFlagIsSet(0x204) != 0) {
                Engine_ActorSetAnimation(20, 5);
                SceneActor_SetupSlotTwenty();
            }
            if (Engine_GameFlagIsSet(0x205) != 0) {
                Engine_ActorSetAnimation(21, 5);
                SceneActor_MarkSlot21AndSetFlag205();
            }
            Scheduler_AddOrUpdateCallback((s32)SceneState_DispatchByActorZeroDepth, 0xc80);
            break;
        case 10:
        case 11:
            if (Engine_GameFlagIsSet(0x9a9) != 0) {
                SceneState_ApplyThreeRects();
                Call3((void (*)())Engine_ActorSetPosition, 9, 0xf80000, 0x36c0000);
            }
            Object_GetById(8)->priority_flags = 2;
            break;
        }
        Engine_ActorSetAnimation(8, 2);
        Engine_ActorSetAnimation(9, 2);
        Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
        Object_GetById(9)->collision_flags = 1;
    }
    if (gGameState.scene == (s32)&SceneId_KuupuappuDou3) {
        Engine_ActorSetAnimation(8, 2);
        if (Engine_GameFlagIsSet(0x207) == 0) {
            Engine_ActorSetAnimation(10, 2);
        }
        Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
        Object_GetById(10)->collision_flags |= 0x80;
        Object_GetById(9)->collision_flags |= 0x80;
        switch (gGameState.entrance) {
        case 5:
        case 6:
            SceneActor_SetupActors11To14AndInstallTask();
            Object_GetById(11)->collision_flags = 2;
            Object_GetById(12)->collision_flags = 2;
            Object_GetById(13)->collision_flags = 2;
            Object_GetById(14)->collision_flags = 2;
            Object_GetById(8)->collision_flags = 1;
            Object_GetById(10)->collision_flags = 1;
            Object_GetById(9)->collision_flags = 1;
            if (Engine_GameFlagIsSet(0x9aa) != 0) {
                SceneState_ApplyThreeRectsRows9And10();
                Call3((void (*)())Engine_ActorSetPosition, 10, 0x1080000, 0xcc0000);
            }
            break;
        }
    }
    gGameState.retreat_entrance = 10;
    gGameState.retreat_scene = (s32)&SceneId_KuupuappuDou1;
    return 0;
}
