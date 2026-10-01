#include "REGION.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gBiribinoDouEntrances3[];
extern const struct SceneEntrance gBiribinoDouEntrances2[];
extern const struct SceneEntrance gBiribinoDouEntrances1[];
extern const struct SceneEntrance gBiribinoDouEntrancesOther[];

/* The exits, in the overlay's read-only data. */
extern u8 gBiribinoDouExits[];

extern const struct ScenePlacement gBiribinoDouPlacements3[];
extern const struct ScenePlacement gBiribinoDouPlacements2[];
extern const struct ScenePlacement gBiribinoDouPlacements1[];
extern const struct ScenePlacement gBiribinoDouPlacementsOther[];
extern const struct SceneEvent gBiribinoDouEvents3[];
extern const struct SceneEvent gBiribinoDouEvents2[];
extern const struct SceneEvent gBiribinoDouEvents1[];
extern const struct SceneEvent gBiribinoDouEventsOther[];

void RunGuardedSceneSetup(void);
void SceneState_SetRuntimeWord448To516(void);
void FieldScene_RunScene398SequenceC(void);

/* Moving a pushed statue: the collision probe, the move target and the
 * commit, through the overlay's import veneers. */
s32 Object_CheckMovementCollision();
void Object_SetPosition();
void Object_CommitPosition();

/* Hides an actor by clearing its sprite flags. */
s32 SceneState_ApplyArgMode0AndReturnZero(s32 no)
{
    Engine_ActorSetSpriteFlags(no, 0);
    return 0;
}

/* Where the party appears in each of the cave's three areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_BiribinoDou3) {
        return gBiribinoDouEntrances3;
    }
    if (selector == (s32)&SceneId_BiribinoDou2) {
        return gBiribinoDouEntrances2;
    }
    if (selector == (s32)&SceneId_BiribinoDou1) {
        return gBiribinoDouEntrances1;
    }
    return gBiribinoDouEntrancesOther;
}

/* Two of the scene hooks the entry veneers export: no follow-up, and the
 * cave's exits. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *BiribinoDou_GetExits(void)
{
    return gBiribinoDouExits;
}

/* The actors placed in each area. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_BiribinoDou3) {
        return gBiribinoDouPlacements3;
    }
    if (selector == (s32)&SceneId_BiribinoDou2) {
        return gBiribinoDouPlacements2;
    }
    if (selector == (s32)&SceneId_BiribinoDou1) {
        return gBiribinoDouPlacements1;
    }
    return gBiribinoDouPlacementsOther;
}

/* What each area answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_BiribinoDou3) {
        return gBiribinoDouEvents3;
    }
    if (selector == (s32)&SceneId_BiribinoDou2) {
        return gBiribinoDouEvents2;
    }
    if (selector == (s32)&SceneId_BiribinoDou1) {
        return gBiribinoDouEvents1;
    }
    return gBiribinoDouEventsOther;
}

/* The cave's map regions and its gate, switch and statue scenes, up to the
 * overlay's exported entry. */
void SceneState_ConfigureRegion1_0_21x14(void)
{
    s32 w = 21;
    s32 h = 14;

    Map_CopyCellAttributes(1, 0, 1, 1, w, h);
}

void SceneState_ConfigureRegion0_0_21x14(void)
{
    s32 w = 21;
    s32 h = 14;

    Map_CopyCellAttributes(0, 0, 1, 1, w, h);
}

void SceneState_ApplyTwoRects(void)
{
    {
        s32 a5 = 1;
        s32 a6 = 3;

        Map_CopyCellsTo(111, 37, 97, 21, a5, a6);
    }
    {
        s32 a5 = 32;
        s32 a6 = 24;

        Map_CopyCellAttributes(46, 38, 3, 2, a5, a6);
    }
}

void FieldScene_RunTwoLayoutSteps(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 3;

        Map_CopyCellsTo(95, 21, 97, 21, fifth, sixth);
    }
    {
        s32 fifth = 32;
        s32 sixth = 25;

        Map_CopyCellAttributes(46, 38, 3, 1, fifth, sixth);
    }
}

void FieldScene_RunActor9Flag882Scene(void)
{
    Engine_EventBegin();
    Actor_SetPosition(9, 0, 0);
    GameFlag_Set(0x882);
    Engine_EventEnd();
}

void FieldScene_RunScene398SequenceA(void)
{
    Engine_EventBegin();
    Actor_SetPosition(8, 0, 0);
    GameFlag_Set(0x883);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(15, 2);
    Actor_Get(15)->motion_flags = 0;
    Actor_Get(15)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    Engine_ActorSetSpritePriority(15, 2);
    Map_CopyCellAttributes(0, 0, 1, 1, 18, 14);
    Engine_EventEnd();
}

void FieldScene_RunActorFifteenScene(void)
{
    void Audio_PlayCue(s32);

    Engine_EventBegin();
    Actor_SetChildValue(0xF, 0);
    Engine_EventWait(0x28);
    Audio_PlayCue(0xD2);
    Engine_ActorSetAnimationAndWait(0xF, 6);
    Engine_EventEnd();
}

void FieldScene_RunActorSixteenScene(void)
{
    void Engine_EventEnd(void);
    void Audio_PlayCue(s32);

    Engine_EventBegin();
    Actor_SetChildValue(0x10, 0);
    Engine_EventWait(0x28);
    Audio_PlayCue(0xD2);
    Engine_ActorSetAnimationAndWait(0x10, 6);
    Engine_EventEnd();
}

void FieldScene_RunActor17Steps28AndD2(void)
{
    void Engine_EventEnd(void);

    Engine_EventBegin();
    Actor_SetChildValue(0x11, 0);
    Engine_EventWait(0x28);
    Audio_PlayCue(0xD2);
    Engine_ActorSetAnimationAndWait(0x11, 6);
    Engine_EventEnd();
}

void FieldScene_RunScene398SequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 v5;

    rec7 = Object_GetById(11);
    rec8 = Actor_Get(12);
    if ((*(s32 *)(rec7 + 8) >> 20) == 35) {
        if ((*(s32 *)(rec7 + 16) >> 20) != 23) {
            goto L_02000330;
        }
        GameFlag_Set(0x303);
    } else {
        L_02000330:;
        GameFlag_Clear(0x303);
    }
    if ((*(s32 *)(rec8 + 8) >> 20) == 35) {
        if ((*(s32 *)(rec8 + 16) >> 20) != 23) {
            goto L_02000350;
        }
        GameFlag_Set(0x304);
    } else {
        L_02000350:;
        GameFlag_Clear(0x304);
    }
    if (GameFlag_IsSet(0x303) == 0) {
        record = GameFlag_IsSet(0x304);
        if (record == 0) {
            goto L_020003c2;
        }
    }
    if (GameFlag_IsSet(0x302) == 0) {
        Engine_EventBegin();
        Engine_EventWait(40);
        Audio_PlayCue(210);
        v5 = 36;
        Engine_ActorSetAnimationAndWait(17, 6);
        Map_CopyCellAttributes(0, 1, 1, 1, v5, 22);
        Map_CopyCellAttributes(0, 2, 1, 1, v5, 24);
        Engine_EventEnd();
    }
    GameFlag_Set(0x302);
    goto L_02000414;
    L_020003c2:;
    if (GameFlag_IsSet(0x302) != 0) {
        Engine_EventBegin();
        Engine_EventWait(40);
        Audio_PlayCue(220);
        v5 = 36;
        Engine_ActorSetAnimationAndWait(17, 2);
        Map_CopyCellAttributes(1, 1, 1, 1, v5, 22);
        Map_CopyCellAttributes(1, 2, 1, 1, v5, 24);
        Engine_EventEnd();
    }
    GameFlag_Clear(0x302);
    L_02000414:;
}

void ActorPresentation_SetSceneCell31AndFlag305(void)
{
    s32 width = 8;
    s32 height = 13;

    Map_CopyCellAttributes(31, 0, 1, 1, width, height);
    GameFlag_Set(0x305);
}

void SceneState_SetGlobalByte17(void)
{
    FIELD_AT_OFFSET(*(void **)&gMapWork, s8 *, 0x17) = 1;
}

void SceneState_ClearRuntimeByte17(void)
{
    FIELD_AT_OFFSET(*(void **)&gMapWork, s8 *, 0x17) = 0;
}

/* The cave's scene start: each of its three areas runs its own setup. */
s32 Scene_Initialize(void)
{
    s16 variant = gGameState.scene;

    if (variant == (s32)&SceneId_BiribinoDou3) {
        RunGuardedSceneSetup();
    } else if (variant == (s32)&SceneId_BiribinoDou2) {
        SceneState_SetRuntimeWord448To516();
    } else if (variant == (s32)&SceneId_BiribinoDou1) {
        FieldScene_RunScene398SequenceC();
    }
    return 0;
}

/* The entrance setups the exported entry dispatches to, and pushing a statue
 * one tile ahead of the leader. */
void RunGuardedSceneSetup(void)
{
    if (GameFlag_IsSet(0x305) != 0) {
        s32 width = 8;
        s32 height = 13;

        Map_CopyCellAttributes(31, 0, 1, 1, width, height);
        Engine_ActorSetAnimation(8, 0);
    }
}

void SceneState_SetRuntimeWord448To516(void)
{
    /* 448 is built as 224 << 1 and the stored 516 as that same register plus
     * 68; the two are not one running offset. */
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);

    Engine_ActorSetAnimation(8, 1);
    Engine_ActorSetAnimation(10, 2);

    if (GameFlag_IsSet(0x882) != 0) {
        Actor_SetPosition(9, 0, 0);
    } else {
        Engine_ActorSetSpriteFlags(Actor_Get(9), 0);
    }
}

void FieldScene_RunScene398SequenceC(void)
{
    u32 i;
    u8 *record;
    s32 v5;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    record = Actor_Get(18);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Actor_Get(19);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Actor_Get(20);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Actor_Get(21);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Actor_Get(22);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Actor_Get(23);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Actor_Get(24);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Actor_Get(25);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Actor_Get(26);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    Engine_ActorSetAnimation(18, 5);
    Engine_ActorSetAnimation(19, 5);
    Engine_ActorSetAnimation(20, 5);
    Engine_ActorSetAnimation(21, 5);
    Engine_ActorSetAnimation(22, 5);
    Engine_ActorSetAnimation(23, 3);
    Engine_ActorSetAnimation(24, 3);
    Engine_ActorSetAnimation(25, 3);
    Engine_ActorSetAnimation(26, 3);
    Engine_ActorSetAnimation(9, 2);
    Engine_ActorSetAnimation(10, 2);
    Engine_ActorSetAnimation(11, 2);
    Engine_ActorSetAnimation(12, 2);
    Engine_ActorSetAnimation(13, 2);
    Engine_ActorSetAnimation(14, 2);
    Resource398_ImportBankNoOp(18);
    Resource398_ImportBankNoOp(19);
    Resource398_ImportBankNoOp(20);
    Resource398_ImportBankNoOp(21);
    Resource398_ImportBankNoOp(22);
    Resource398_ImportBankNoOp(23);
    Resource398_ImportBankNoOp(24);
    Resource398_ImportBankNoOp(25);
    Resource398_ImportBankNoOp(26);
    Resource398_ImportBankNoOp(9);
    Resource398_ImportBankNoOp(10);
    Resource398_ImportBankNoOp(11);
    Resource398_ImportBankNoOp(12);
    Resource398_ImportBankNoOp(13);
    Resource398_ImportBankNoOp(14);
    if (GameFlag_IsSet(0x883) != 0) {
        Actor_SetPosition(8, 0, 0);
        Engine_ActorSetAnimation(15, 5);
        Actor_Get(15)->motion_flags = 0;
        Actor_Get(15)->y.fixed = -0x40000;
        Actor_Get(15)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetSpritePriority(15, 2);
        Map_CopyCellAttributes(0, 0, 1, 1, 18, 14);
    } else {
        Engine_ActorSetAnimation(8, 2);
        record = Actor_Get(8);
        Engine_ActorSetSpriteFlags((s32)record, 0);
        Engine_ActorSetAnimation(15, 1);
    }
    Engine_ActorSetAnimation(16, 1);
    if (GameFlag_IsSet(0x302) != 0) {
        v5 = 36;
        Engine_ActorSetAnimation(17, 1);
        Map_CopyCellAttributes(0, 1, 1, 1, v5, 22);
        Map_CopyCellAttributes(0, 2, 1, 1, v5, 24);
    } else {
        v5 = 36;
        Engine_ActorSetAnimation(17, 5);
        Map_CopyCellAttributes(1, 1, 1, 1, v5, 22);
        Map_CopyCellAttributes(1, 2, 1, 1, v5, 24);
    }
    if (GameFlag_IsSet(0x303) != 0) {
        Actor_SetPosition(11, 0x23a0000, 0x1780000);
    }
    if (GameFlag_IsSet(0x304) != 0) {
        Actor_SetPosition(12, 0x23a0000, 0x1780000);
    }
}

s32 *SceneActor_FindSlotAtTile(s32 x, s32 z)
{
    s32 **slots = (s32 **)((u8 *)gEventWork + 0x14);
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

    player = Actor_Get(ACTOR_PARTY_LEADER);
    heading = *(u16 *)(player + 6) >> 12;

    tx = (*(s16 *)(player + 10)
        + (StagedActor_DirectionSteps[heading] >> 16)) >> 4;
    tz = (*(s16 *)(player + 18)
        + ((StagedActor_DirectionSteps[heading] << 16) >> 16)) >> 4;
    target = (u8 *)SceneActor_FindSlotAtTile(tx, tz);
    if (target == 0) return;

    tx = (*(s16 *)(target + 10)
        + (StagedActor_DirectionSteps[heading] >> 16)) >> 4;
    tz = (*(s16 *)(target + 18)
        + ((StagedActor_DirectionSteps[heading] << 16) >> 16)) >> 4;
    blocker = (u8 *)SceneActor_FindSlotAtTile(tx, tz);
    if (blocker != 0) return;

    target[0x22] = 2;

    pos[0] = *(s32 *)(target + 8)
        + (StagedActor_DirectionSteps[heading] & (s32)0xffff0000);
    pos[1] = *(s32 *)(target + 12);
    pos[2] = *(s32 *)(target + 16) + (StagedActor_DirectionSteps[heading] << 16);

    if (Object_CheckMovementCollision(target, pos) > 0) return;

    Object_SetMode(player, 8);
    Engine_TaskWait(15);
    Audio_PlayCue(185);

    *(s32 *)(target + 48) = 0x3333;
    *(s32 *)(target + 52) = 0x3333;
    Object_SetPosition(target, pos[0], pos[1], pos[2]);

    *(s32 *)(player + 48) = 0x3333;
    *(s32 *)(player + 52) = 0x3333;
    Object_SetPosition(player, pos[0], pos[1], pos[2]);

    Object_CommitPosition(target);

    *(s32 *)(target + 8) = pos[0];
    *(s32 *)(target + 16) = pos[2];
    *(s32 *)(target + 36) = (s32)blocker;
    *(s32 *)(target + 44) = (s32)blocker;

    Object_SetMode(player, 1);
    FieldScene_RunScene398SequenceB();
}

void Resource398_ImportBankNoOp(void)
{
}
