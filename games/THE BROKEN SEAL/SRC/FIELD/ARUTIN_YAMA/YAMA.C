#include "YAMA.H"
/* Arutin mountain: the leader is carried with actor 8 until it has come
   level with it, then the rolling object starts. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

extern const struct SceneEntrance gArutinYamaEntrancesOther[];
extern const struct SceneEntrance gArutinYamaEntrances1[];
extern const struct SceneEntrance gArutinYamaEntrances2[];
extern const struct SceneEntrance gArutinYamaEntrances3[];
extern const struct SceneEntrance gArutinYamaEntrances4[];
extern const struct SceneEntrance gArutinYamaEntrances5[];
extern const struct SceneEntrance gArutinYamaEntrances6[];
extern const struct SceneEntrance gArutinYamaEntrances7[];
extern const struct SceneEntrance gArutinYamaEntrances8[];
extern const struct SceneEntrance gArutinYamaEntrances9[];
extern const struct SceneEntrance gArutinYamaEntrances10[];
extern const struct SceneEntrance gArutinYamaEntrances11[];
extern const struct SceneRegion gArutinYamaRegions9[];
extern const struct SceneRegion gArutinYamaRegions10[];

extern const struct ScenePlacement gArutinYamaPlacementsOther[];
extern const struct ScenePlacement gArutinYamaPlacements1[];
extern const struct ScenePlacement gArutinYamaPlacements3[];
extern const struct ScenePlacement gArutinYamaPlacements5[];
extern const struct ScenePlacement gArutinYamaPlacements6[];
extern const struct ScenePlacement gArutinYamaPlacements7[];
extern const struct ScenePlacement gArutinYamaPlacements8[];
extern const struct ScenePlacement gArutinYamaPlacements9[];
extern const struct ScenePlacement gArutinYamaPlacements10[];
extern const struct ScenePlacement gArutinYamaPlacements11[];

void ArutinYama_RunRollingObject(s32 id, s32 heading);
void Battle_ResetEffectCounter(void);
void BattleFx_PlayQueuedSound(void);

extern u8 MsgFieldFlippedSwitch[];

void SceneState_SetValue14Mode23(void)
{
    extern s32 Data_03001e40;

    BattleFx_SetPhaseRequest(0xE, 0x17);
}

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        angle = (u16)ArcTan2(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

s32 SceneActor_CopyActor8PositionWithFixedY(struct Frame *dst)
{
    struct Frame *src = Actor_Get(8);

    dst->f08 = src->f08;
    dst->f0c = 0xFFF40000;
    dst->f10 = src->f10;
    return 0;
}

/*
 * Per-frame integrator for one actor record in resource_3a4. Advances the
 * position pair at +8 and +12, advances +24 and +28 by one shared velocity,
 * damps that velocity, and returns 0.
 *
 * The damping subtracts +72 from the value of +40 already held in a register,
 * not from a fresh load; v28 and v2c carry those earlier reads and must stay
 * locals rather than become repeated loads.
 */
s32 OverlayObject_IntegrateAndDamp(u8 *p)
{
    s32 v28;
    s32 v2c;

    *(s32 *)(p + 8) = *(s32 *)(p + 8) + *(s32 *)(p + 36);

    v28 = *(s32 *)(p + 40);
    *(s32 *)(p + 12) = *(s32 *)(p + 12) + v28;

    v2c = *(s32 *)(p + 44);
    *(s32 *)(p + 24) = *(s32 *)(p + 24) + v2c;
    *(s32 *)(p + 28) = *(s32 *)(p + 28) + v2c;

    *(s32 *)(p + 40) = v28 - *(s32 *)(p + 72);

    return 0;
}

/* Where the party appears in each of Altin Peak's eleven areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_ArutinYama1) {
        return gArutinYamaEntrances1;
    }
    if (scene == (s32)&SceneId_ArutinYama2) {
        return gArutinYamaEntrances2;
    }
    if (scene == (s32)&SceneId_ArutinYama3) {
        return gArutinYamaEntrances3;
    }
    if (scene == (s32)&SceneId_ArutinYama4) {
        return gArutinYamaEntrances4;
    }
    if (scene == (s32)&SceneId_ArutinYama5) {
        return gArutinYamaEntrances5;
    }
    if (scene == (s32)&SceneId_ArutinYama6) {
        return gArutinYamaEntrances6;
    }
    if (scene == (s32)&SceneId_ArutinYama7) {
        return gArutinYamaEntrances7;
    }
    if (scene == (s32)&SceneId_ArutinYama8) {
        return gArutinYamaEntrances8;
    }
    if (scene == (s32)&SceneId_ArutinYama9) {
        return gArutinYamaEntrances9;
    }
    if (scene == (s32)&SceneId_ArutinYama10) {
        return gArutinYamaEntrances10;
    }
    if (scene == (s32)&SceneId_ArutinYama11) {
        return gArutinYamaEntrances11;
    }
    return gArutinYamaEntrancesOther;
}

/* Only the ninth and tenth areas have map regions. */
const struct SceneRegion *Scene_GetRegions(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_ArutinYama9) {
        return gArutinYamaRegions9;
    }
    if (scene == (s32)&SceneId_ArutinYama10) {
        return gArutinYamaRegions10;
    }
    return 0;
}

u8 *SceneData_GetTableC85c(void)
{
    return ArutinYama_StatueTable;
}

/* The actors placed in Altin Peak's areas; the second and fourth areas
   take the table the other scenes take. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_ArutinYama1) {
        return gArutinYamaPlacements1;
    }
    if (scene == (s32)&SceneId_ArutinYama3) {
        return gArutinYamaPlacements3;
    }
    if (scene == (s32)&SceneId_ArutinYama5) {
        return gArutinYamaPlacements5;
    }
    if (scene == (s32)&SceneId_ArutinYama6) {
        return gArutinYamaPlacements6;
    }
    if (scene == (s32)&SceneId_ArutinYama7) {
        return gArutinYamaPlacements7;
    }
    if (scene == (s32)&SceneId_ArutinYama8) {
        return gArutinYamaPlacements8;
    }
    if (scene == (s32)&SceneId_ArutinYama9) {
        return gArutinYamaPlacements9;
    }
    if (scene == (s32)&SceneId_ArutinYama10) {
        return gArutinYamaPlacements10;
    }
    if (scene == (s32)&SceneId_ArutinYama11) {
        return gArutinYamaPlacements11;
    }
    return gArutinYamaPlacementsOther;
}

void ArutinYama_BeginRollingRide(s32 a0)
{
    u32 i;
    struct FieldActor *rec7;
    struct FieldActor *rec8;
    s32 record;
    s32 v3;

    rec8 = Object_GetById(0);
    rec7 = Object_GetById(8);
    Battle_ResetEffectCounter();
    Engine_EventBegin();
    Engine_ActorSetAnimation(0, 22);
    Engine_EventWait(10);
    Engine_AudioPlayCue(152);
    Engine_ActorSetSpeed(0, 0x33333, 0x19999);
    v3 = rec7->y.fixed - rec8->y.fixed;
    if ((rec7->y.fixed - rec8->y.fixed) < 0) {
        v3 = rec8->y.fixed - rec7->y.fixed;
    }
    {
        s32 speed = 0x80;

        asm("lsl %0, %0, #11" : "+l"(speed)); /* FAKEMATCH: builds the base speed after the height step */
        rec8->velocity_y = ((v3 >> 14) << 14) + speed;
    }
    Engine_ActorSetAnimation(0, 7);
    Engine_ObjectSetPosition(rec8, rec7->x.fixed, rec7->y.fixed, rec7->z.fixed);
    Engine_TaskWait(10);
    rec8->sprite->priority = 3;
    Engine_ActorWaitForMove(0);
    for (;;) {
        if (!((rec7->y.fixed >> 14) < (rec8->y.fixed >> 14))) break;
        Engine_TaskWait(1);
    }
    Engine_EventEnd();
    Engine_AudioPlayCue(159);
    ArutinYama_RunRollingObject(a0, 0);
    Engine_TaskWait(20);
    BattleFx_PlayQueuedSound();
}

void SceneState_SetWorkByte22bTo3(void)
{
    gGameState.unknown_200[0x22b - 0x200] = 3;
    Party_SetFields1ceAnd1d0((s32)&SceneId_ArutinYama1, 99);
    BattleFx_SetWeightedResult(53, 2);
}

void SceneState_SetByte22bTo3(void)
{
    gGameState.unknown_200[0x22b - 0x200] = 3;
    Party_SetFields1ceAnd1d0((s32)&SceneId_ArutinYama3, 99);
    BattleFx_SetWeightedResult(53, 2);
}

void SceneState_SetByte22bTo3AndSend51(void)
{
    gGameState.unknown_200[0x22b - 0x200] = 3;
    Party_SetFields1ceAnd1d0((s32)&SceneId_ArutinYama5, 99);
    BattleFx_SetWeightedResult(53, 2);
}

void SceneActor_UpdateSlot10ByTileX(void)
{
    s32 *a = Actor_Get(10);

    if (a != 0) {
        s32 x = 24;
        s32 y = 26;
        s32 t;

        Map_CopyCellAttributes(x, 27, 2, 1, x, y);
        t = a[2] >> 20;
        if (t == 25) {
            Map_CopyCellAttributes(0, 0, 1, 1, t, y);
        } else {
            Map_CopyCellAttributes(0, 0, 1, 1, x, y);
        }
        Actor_SetSpriteFlags(a, 0);
        ((u8 *)a)[0x55] = 0;
        Map_Redraw();
        Task_Wait(1);
    }
}

/*
 * Presentation setup in resource_3a4: clear a record byte, adjust two
 * handle flag bits, run two presentation primitives, then stamp a fixed
 * rate into the record.
 */

/* Declared without prototypes -- call sites vary in argument shape. */

/*
 * resource_3a4: a published callback that sets the mode of actor record 8.
 */

/*
 * Presentation callback for resource_3a4, published rather than called
 * directly from this overlay.
 */
void ActorPresentation_SetCellAndLowerActorEight(void)
{
    extern u32 Data_03001e40;

    s32 *p;
    s32 s0;
    s32 s1;

    p = Actor_Get(8);
    s0 = 9;
    s1 = 13;
    Map_CopyCellAttributes(7, 13, 1, 1, s0, s1);
    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    GameFlag_Set(0x200);
}

void SceneState_ApplyRectAndSetActor9Byte55(void)
{
    extern s32 Data_03001e40;

    s32 *p;
    s32 s0;
    s32 s1;

    p = Actor_Get(9);
    s0 = 17;
    s1 = 13;
    Map_CopyCellAttributes(29, 1, 3, 1, s0, s1);
    if (p != 0) {
        ((u8 *)p)[0x55] = 2;
    }
    GameFlag_Set(0x201);
}

void SceneActor_RaiseSlot9StepA(void)
{
    s32 *p;
    s32 s0;

    p = Actor_Get(9);
    s0 = 26;
    Map_CopyCellAttributes(0, 0, 1, 1, s0, s0);
    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    GameFlag_Set(0x200);
}

void SceneActor_RaiseSlot9StepB(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Actor_Get(9);
    s0 = 25;
    s1 = 13;
    Map_CopyCellAttributes(23, 13, 1, 1, s0, s1);
    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    GameFlag_Set(0x200);
}

void SceneState_ApplyRectAndLowerActor9(void)
{
    extern s32 Data_03001e40;

    s32 *rec;
    s32 s0;
    s32 s1;

    rec = Actor_Get(9);
    s0 = 43;
    s1 = 41;
    Map_CopyCellAttributes(45, 41, 1, 1, s0, s1);
    if (rec != 0) {
        Actor_SetSpriteFlags(rec, 0);
        rec[3] += 0xffe00000;
        ((u8 *)rec)[0x23] = 2;
    }
    GameFlag_Set(0x200);
}

void SceneActor_RaiseSlot11AndSetFlag201(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Actor_Get(11);
    s0 = 17;
    s1 = 10;
    Map_CopyCellAttributes(1, 0, 1, 1, s0, s1);
    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    GameFlag_Set(0x201);
}

void SceneActor_AdjustSlot12AndSetFlag204(void)
{
    extern s32 Data_03001e40;

    s32 *p;
    s32 s0;
    s32 s1;

    p = Actor_Get(12);
    s0 = 26;
    s1 = 15;
    Map_CopyCellAttributes(1, 0, 1, 1, s0, s1);
    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    GameFlag_Set(0x204);
}

void SceneState_SetDispcntBit9ByThreshold(void)
{
    extern u16 ArutinYama_RiseTimer;

    volatile u16 *reg = (volatile u16 *)0x04000000;
    s16 v = *reg & 0xfdff;

    if ((u32)(Random_Next() * 100) >> 16 >= ArutinYama_RiseTimer) {
        s32 k = 0x200;

        v |= k;
    }
    {
        u32 t = (u16)v;

        *reg = t;
    }
}

void FieldScene_RunEarlySequence(void)
{
    extern u8 ArutinYama_RiseTimer[];

    s32 i;
    s32 record;
    u8 *p5;
    u8 *rec;
    s32 v;

    p5 = *(u8 **)&gMapWork;
    Audio_PlayCue(230);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Event_Wait(10);
    rec = p5 + 356;
    for (i = 0; i <= 23; i++) {
        *(s32 *)(rec + 12) -= 0x10000;
        Task_Wait(4);
        if (i == 8) {
            record = Object_GetById(8);
            *(s32 *)(record + 24) = 0x1999;
            record = Actor_Get(8);
            *(s32 *)(record + 28) = 0x1999;
            Actor_SetPosition(8, 0x980000, 0xd80000);
            Actor_EnableActionCallback(8, ArutinYama_EarlyActorScript);
        }
    }
    Runtime_SetIrqHandler(1, 0, SceneState_SetDispcntBit9ByThreshold);
    *(u16 *)ArutinYama_RiseTimer = 0;
    do {
        Task_Wait(1);
        v = *(u16 *)ArutinYama_RiseTimer + 1;
        *(u16 *)ArutinYama_RiseTimer = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Task_Wait(1);
    Runtime_SetIrqHandler(1, 0, 0);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(30);
    Map_CopyCellAttributes(0, 0, 1, 2, 3, 14);
    GameFlag_Set(0x8fd);
}

void FieldScene_RunScene3a4SequenceH(void)
{
    extern u8 ArutinYama_RiseTimer[];

    s32 i;
    u8 *p5;
    u8 *rec;
    s32 v;

    p5 = *(u8 **)&gMapWork;
    Map_CopyCells(93, 41, 16, 4, 77, 28);
    Audio_PlayCue(230);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Event_Wait(10);
    rec = p5 + 356;
    for (i = 23; i >= 0; i--) {
        *(s32 *)(rec + 12) -= 0x10000;
        Task_Wait(4);
    }
    Runtime_SetIrqHandler(1, 0, SceneState_SetDispcntBit9ByThreshold);
    *(u16 *)ArutinYama_RiseTimer = 0;
    do {
        Task_Wait(1);
        v = *(u16 *)ArutinYama_RiseTimer + 1;
        *(u16 *)ArutinYama_RiseTimer = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Task_Wait(1);
    Runtime_SetIrqHandler(1, 0, 0);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(30);
    Map_CopyCells(77, 41, 16, 4, 77, 28);
    GameFlag_Set(0x8fe);
}

void FieldScene_RunScene3a4SequenceI(void)
{
    extern u8 ArutinYama_RiseTimer[];

    s32 i;
    u8 *p8;
    u8 *rec;
    s32 v;

    p8 = *(u8 **)&gMapWork;
    Map_CopyCellsTo(113, 31, 103, 17, 1, 1);
    Map_CopyCellsTo(111, 32, 104, 18, 3, 2);
    Map_CopyCellsTo(64, 32, 103, 18, 1, 2);
    Audio_PlayCue(230);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Event_Wait(10);
    rec = p8 + 356;
    for (i = 23; i >= 0; i--) {
        *(s32 *)(rec + 12) -= 0x10000;
        Task_Wait(4);
    }
    Runtime_SetIrqHandler(1, 0, SceneState_SetDispcntBit9ByThreshold);
    *(u16 *)ArutinYama_RiseTimer = 0;
    do {
        Task_Wait(1);
        v = *(u16 *)ArutinYama_RiseTimer + 1;
        *(u16 *)ArutinYama_RiseTimer = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Task_Wait(1);
    Runtime_SetIrqHandler(1, 0, 0);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(30);
    Map_CopyCellsTo(103, 14, 103, 17, 4, 3);
    GameFlag_Set(0x907);
}

void FieldScene_RunScene3a4SequenceB(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x323) != 0) {
        Map_CopyCellAttributes(2, 0, 1, 1, 24, 80);
        Map_CopyCellsTo(2, 1, 24, 11, 1, 2);
        GameFlag_Clear(0x323);
    } else {
        Map_CopyCellAttributes(0, 0, 1, 1, 24, 80);
        Map_CopyCellsTo(0, 1, 24, 11, 1, 2);
        GameFlag_Set(0x323);
    }
}

void FieldScene_RunValue1528Scene(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgFieldFlippedSwitch, 1);
    Audio_PlayCue(125);
    FieldScene_RunScene3a4SequenceB();
    Task_Wait(20);
    BattleFx_PlayQueuedSound();
    Event_End();
}

void FieldScene_RunScene3a4SequenceA(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x325) != 0) {
        Map_CopyCellAttributes(12, 72, 1, 1, 11, 73);
        Map_CopyCellsTo(48, 32, 11, 4, 1, 2);
        GameFlag_Clear(0x325);
    } else {
        Map_CopyCellAttributes(10, 72, 1, 1, 11, 73);
        Map_CopyCellsTo(49, 32, 11, 4, 1, 2);
        GameFlag_Set(0x325);
    }
}

void FieldScene_RunLine1528Sequence(void)
{
    extern u8 ArutinYama_RiseTimer[];

    Event_Begin();
    Message_ShowCentered((s32)MsgFieldFlippedSwitch, 1);
    Audio_PlayCue(125);
    FieldScene_RunScene3a4SequenceA();
    Task_Wait(20);
    BattleFx_PlayQueuedSound();
    Event_End();
}

void FieldScene_RunScene3a4SequenceC(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    s32 record;

    Event_Begin();
    Message_ShowCentered((s32)MsgFieldFlippedSwitch, 1);
    Audio_PlayCue(125);
    if (GameFlag_IsSet(0x326) != 0) {
        Map_CopyCellAttributes(15, 93, 1, 1, 16, 92);
        Map_CopyCellsTo(47, 29, 16, 28, 1, 2);
        GameFlag_Clear(0x326);
    } else {
        Map_CopyCellAttributes(17, 93, 1, 1, 16, 92);
        Map_CopyCellsTo(46, 29, 16, 28, 1, 2);
        GameFlag_Set(0x326);
    }
    Task_Wait(20);
    BattleFx_PlayQueuedSound();
    Event_End();
}

void FieldScene_RunScene3a4SequenceD(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    s32 record;

    Event_Begin();
    Message_ShowCentered((s32)MsgFieldFlippedSwitch, 1);
    Audio_PlayCue(125);
    if (GameFlag_IsSet(0x327) != 0) {
        Map_CopyCellAttributes(28, 82, 1, 1, 29, 81);
        Map_CopyCellsTo(47, 28, 29, 17, 1, 2);
        GameFlag_Clear(0x327);
    } else {
        Map_CopyCellAttributes(30, 82, 1, 1, 29, 81);
        Map_CopyCellsTo(46, 28, 29, 17, 1, 2);
        GameFlag_Set(0x327);
    }
    Task_Wait(20);
    BattleFx_PlayQueuedSound();
    Event_End();
}

void SceneActor_SetActor10Byte23To3(void)
{
    extern u32 Data_03001e40;

    ((u8 *)Object_GetById(10))[0x23] = 3;
}

void SceneActor_SetActor10Byte23To1(void)
{
    extern u32 Data_03001e40;

    ((u8 *)Object_GetById(10))[0x23] = 1;
}

void FieldScene_RunScene3a4_02000c9c(void)
{
    extern u8 ArutinYama_RiseTimer[];

    u32 i;
    s32 record;

    Event_Begin();
    Map_CopyCellAttributes(24, 27, 2, 1, 24, 26);
    Audio_PlayCue(185);
    Actor_SetSpeed(10, 0x3333, 0x1999);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
    *(u8 *)((u8 *)Object_GetById(10) + 90) &= 254;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 8);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x190, 0x1a8);
    Actor_SetDestination(10, 0x198, 0x1a8);
    Actor_WaitForMove(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    SceneActor_UpdateSlot10ByTileX();
    Event_End();
}
