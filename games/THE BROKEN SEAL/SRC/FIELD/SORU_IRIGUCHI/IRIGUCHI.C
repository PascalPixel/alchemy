#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SORU.H"
#include "CALL.H"
#include "SCENE_IDS.H"

void Scene_RunActorFormation(s32 a0);

extern const struct SceneEntrance gSoruIriguchiEntrances2[];
extern const struct SceneEntrance gSoruIriguchiEntrances1[];
extern const struct SceneEntrance gSoruIriguchiEntrancesOther[];
extern const u32 SoruIriguchi_Exits[];
extern const struct ScenePlacement gSoruIriguchiPlacementsOther[];
extern const struct ScenePlacement gSoruIriguchiPlacements1[];
extern const struct ScenePlacement gSoruIriguchiPlacements1Entrances11To13[];
extern const struct ScenePlacement gSoruIriguchiPlacements1Entrances14To16[];
extern const struct ScenePlacement gSoruIriguchiPlacements2[];
extern const struct SceneEvent gSoruIriguchiEventsOther[];
extern const struct SceneEvent gSoruIriguchiEvents1[];
extern const struct SceneEvent gSoruIriguchiEvents1Entrances11To13[];
extern const struct SceneEvent gSoruIriguchiEvents1Entrances14To16[];
extern const struct SceneEvent gSoruIriguchiEvents2[];
void FieldScene_PrepareActors(s32 placements);
extern u8 MsgSoruMinotaurReliefBothEyes[];
extern u8 MsgSoruMinotaurReliefOneEye[];
extern u8 MsgSoruSetSmallGem[];

/* Each facing's push step: the x step in the high half, the z step in the low. */
extern s32 gSoruPushSteps[];
s32 Object_CheckMovementCollision(struct FieldActor *actor, union FieldCoordinate *pos);
void Engine_ObjectCommitPosition(struct FieldActor *object);
void Scene_UpdateOuterActor9Flags(void);
void Scene_UpdateOuterActor10Flags(void);
void Scene_UpdateFormationActor9Flags(void);
void Scene_UpdateFormationActor10Flags(void);
void Scene_UpdateFormationActor11Flags(void);
void Scene_UpdateFormationActor12Flags(void);
void Scene_UpdateFormationActor13Flags(void);
void Scene_UpdateFormationActor14Flags(void);
void SoruIriguchi_ApplyEntryState(void);
void FieldScene_RunSceneEntryHook(void);

/* The overlay's work, past its loaded image. */
s32 SoruIriguchi_CueTimer __attribute__((section(".bss")));

void InitializeSceneRecordBuffer();
void BattleFx_SetQueuedSoundAndPlay();
void Scene_RunTransitionCue();
void Scene_EnterSolSanctum();
void Scene_SukuretaSuspectsHiddenPassage();

extern u8 MsgSoruSukuretaFirstTimeAtSol[];

extern u8 MsgSoruCantSeriouslyWant[];
extern u8 MsgSoruDangerousSplitStay[];
extern u8 MsgSoruWrongSukureta[];

extern u8 MsgFieldDoorTightlyLocked[];
extern u8 MsgSoruMoreStatuesOutOfReach[];

/* Where the party appears in the scene it enters. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_SoruIriguchi2) {
        return gSoruIriguchiEntrances2;
    }
    if (scene == (s32)&SceneId_SoruIriguchi1) {
        return gSoruIriguchiEntrances1;
    }
    return gSoruIriguchiEntrancesOther;
}

/* The sanctum entrance's regions and exits, between its scene-dependent
   getters. */
const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return SoruIriguchi_Exits;
}

/* The actors placed at the sanctum's entrance. In the first scene the
   entrances 11 to 13 and 14 to 16 have their own tables; any other entrance
   takes a table that is prepared first. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s32 table;
    s32 lo = 11;

    if (gGameState.scene == (s32)&SceneId_SoruIriguchi1) {
        if (gGameState.entrance >= lo) {
            if (gGameState.entrance > 13) {
                if (gGameState.entrance > 16) {
                    goto other_entrance;
                }
                return gSoruIriguchiPlacements1Entrances14To16;
            }
            return gSoruIriguchiPlacements1Entrances11To13;
        }
    other_entrance:;
        table = (s32)gSoruIriguchiPlacements1;
        FieldScene_PrepareActors(table);
        return (const struct ScenePlacement *)table;
    } else {
        if (gGameState.scene == (s32)&SceneId_SoruIriguchi2) {
            return gSoruIriguchiPlacements2;
        }
    }
    return gSoruIriguchiPlacementsOther;
}

/* What the sanctum's entrance answers, chosen as its placements are. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s32 lo = 11;

    if (gGameState.scene == (s32)&SceneId_SoruIriguchi2) {
        return gSoruIriguchiEvents2;
    } else {
        if (gGameState.scene == (s32)&SceneId_SoruIriguchi1) {
            if (gGameState.entrance >= lo) {
                if (gGameState.entrance > 13) {
                    if (gGameState.entrance > 16) {
                        goto other_entrance;
                    }
                    return gSoruIriguchiEvents1Entrances14To16;
                }
                return gSoruIriguchiEvents1Entrances11To13;
            }
        other_entrance:;
            return gSoruIriguchiEvents1;
        } else {
        }
    }
    return gSoruIriguchiEventsOther;
}

void FieldScene_RunScene37fSequenceA(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    Engine_EventBegin();
    v5 = 3;
    v6 = 2;
    Audio_PlayCue(181);
    Map_CopyCellsTo(16, 28, 21, 3, v5, v6);
    Engine_TaskWait(10);
    Map_CopyCellsTo(16, 30, 21, 3, v5, v6);
    Engine_TaskWait(10);
    Map_CopyCellsTo(16, 32, 21, 3, v5, v6);
    Engine_TaskWait(10);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 98);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
    Engine_EventWait(10);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(2);
    Engine_EventEnd();
}

void SceneDialogue_RunFlag81aMessageBranch(void)
{

    Engine_EventBegin();

    if (GameFlag_IsSet(0x81a) != 0) {
        Engine_MessageShowCentered((s32)MsgSoruMinotaurReliefBothEyes, 1);
    } else {
        Engine_MessageShowCentered((s32)MsgSoruMinotaurReliefOneEye, 1);
        if (GameFlag_IsSet(0xf01) != 0) {
            u16 *p = (u16 *)(gWork + 370);
            u16 val = 1;
            *p = val;
        }
    }

    Engine_EventEnd();
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 id;
    s32 v5;
    s32 v6;

    if (GameFlag_IsSet(0xf01) == 0) {
    } else {
        if (GameFlag_IsSet(0x81a) != 0) {
        } else {
            Engine_EventBegin();
            Battle_ResetEffectCounter();
            v5 = 1;
            Audio_PlayCue(182);
            Map_CopyCellsTo(0, 70, 30, 42, v5, v5);
            Engine_MapRedraw();
            Engine_EventWait(40);
            id = (s32)MsgSoruSetSmallGem;
            Engine_MessageShowCentered(id, 1);
            Engine_EventWait(20);
            v6 = 3;
            Audio_PlayCue(183);
            Map_CopyCellsTo(0, 29, 3, 1, v6, 2);
            Map_CopyCellAttributes(0, 29, 3, 2, v6, v5);
            Map_CopyCellsTo(1, 109, 4, 81, v5, v5);
            Engine_MapRedraw();
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
            Engine_EventWait(20);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
            Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
            Engine_EventWait(20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 40);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
            Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 20);
            Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 40);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Engine_EventWait(40);
            Engine_MessageShowCentered(id + 1, 1);
            GameFlag_Set(0x143);
            GameFlag_Set(0x81a);
            Engine_EventEnd();
        }
    }
}

void FieldScene_RunFlag821Dialogue(void)
{

    u8 *work;

    Engine_EventBegin();

    if (GameFlag_IsSet(0x821) != 0) {
        Engine_MessageShowCentered((s32)MsgSoruMinotaurReliefBothEyes, 1);
    } else if (GameFlag_IsSet(0xf02) != 0) {
        work = gWork;
        Engine_MessageShowCentered((s32)MsgSoruMinotaurReliefOneEye, 1);
        {
            /*
             * The halfword store goes through a pointer local and then an
             * s32 value local, in that order.  Storing the literal straight
             * into the halfword builds the constant in HImode and fetches it
             * from the literal pool, which costs a pool word; splitting the
             * address out first also fixes which register holds it.
             */
            u16 *frame = (u16 *)(work + 370);
            s32 one = 1;
            *frame = (u16)one;
        }
    } else {
        Engine_MessageShowCentered((s32)MsgSoruMinotaurReliefOneEye, 1);
    }

    Engine_EventEnd();
}

/* Setting the small gem into the empty socket opens the gate. */
void SoruIriguchi_SetSmallGem(void)
{
    s32 message;

    if (GameFlag_IsSet(3842) != 0 && GameFlag_IsSet(GATE_ID) == 0) {
        Engine_EventBegin();
        Battle_ResetEffectCounter();
        Engine_AudioPlayCue(182);
        Engine_MapCopyCellsTo(0, 71, 100, 71, 1, 1);
        Engine_MapRedraw();
        Engine_EventWait(40);
        message = (s32)MsgSoruSetSmallGem;
        Engine_MessageShowCentered(message, 1);
        Engine_EventWait(20);
        Engine_AudioPlayCue(183);
        Map_CopyCellsTo(122, 20, 120, 30, 1, 2);
        Map_CopyCellAttributes(122, 20, 1, 2, 120, 30);
        Engine_MapRedraw();
        Call3(Engine_WorkSetValuesIfNonNegative, 65536, 65536, 65536);
        Engine_EventWait(20);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 256, 0);
        Call3(Engine_WorkSetValuesIfNonNegative, 131072, 131072, 65536);
        Engine_EventWait(20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 40);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 32768, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 10);
        Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 20);
        Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 40);
        Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 58982);
        Engine_EventWait(40);
        Engine_MessageShowCentered(message + 1, 1);
        GameFlag_Set(0x143);
        GameFlag_Set(GATE_ID);
        Engine_EventEnd();
    }
}

void Scene_UpdateOuterActor9Flags(void)
{
    s32 *work = Object_GetById(9);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Call1(Engine_GameFlagClear, 0x302);
    Call1(Engine_GameFlagClear, 0x303);
    if (pos == 93) {
        Engine_GameFlagSet(0x303);
    } else if (pos == 95) {
        Engine_GameFlagSet(0x302);
    }
}

void Scene_UpdateOuterActor10Flags(void)
{
    s32 *work = Object_GetById(10);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Call1(Engine_GameFlagClear, 0x300);
    Call1(Engine_GameFlagClear, 0x301);
    if (pos == 115) {
        Engine_GameFlagSet(0x300);
    } else if (pos == 113) {
        Engine_GameFlagSet(0x301);
    }
}

void Scene_UpdateFormationActor9Flags(void)
{
    s32 *work = Object_GetById(9);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Call1(Engine_GameFlagClear, 0x310);
    Call1(Engine_GameFlagClear, 0x311);
    if (pos == 99) {
        Engine_GameFlagSet(0x311);
    } else if (pos == 101) {
        Engine_GameFlagSet(0x310);
    }
    Scene_RunActorFormation(0);
}

void Scene_UpdateFormationActor10Flags(void)
{
    s32 *work = Object_GetById(10);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Call1(Engine_GameFlagClear, 0x312);
    Call1(Engine_GameFlagClear, 0x313);
    if (pos == 103) {
        Engine_GameFlagSet(0x313);
    } else if (pos == 105) {
        Engine_GameFlagSet(0x312);
    }
    Scene_RunActorFormation(0);
}

void Scene_UpdateFormationActor11Flags(void)
{
    s32 *work = Object_GetById(11);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Call1(Engine_GameFlagClear, 0x314);
    Call1(Engine_GameFlagClear, 0x315);
    if (pos == 107) {
        Engine_GameFlagSet(0x315);
    } else if (pos == 109) {
        Engine_GameFlagSet(0x314);
    }
    Scene_RunActorFormation(0);
}

void Scene_UpdateFormationActor12Flags(void)
{
    s32 *work = Object_GetById(12);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Call1(Engine_GameFlagClear, 0x316);
    Call1(Engine_GameFlagClear, 0x317);
    if (pos == 111) {
        Engine_GameFlagSet(0x317);
    } else if (pos == 113) {
        Engine_GameFlagSet(0x316);
    }
    Scene_RunActorFormation(0);
}

void Scene_UpdateFormationActor13Flags(void)
{
    s32 *work = Object_GetById(13);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Call1(Engine_GameFlagClear, 0x318);
    Call1(Engine_GameFlagClear, 0x319);
    if (pos == 115) {
        Engine_GameFlagSet(0x319);
    } else if (pos == 117) {
        Engine_GameFlagSet(0x318);
    }
    Scene_RunActorFormation(0);
}

void Scene_UpdateFormationActor14Flags(void)
{
    s32 *work = Object_GetById(14);
    s32 pos;

    if (work == 0) return;
    pos = work[2] >> 20;
    Call1(Engine_GameFlagClear, 0x31a);
    Call1(Engine_GameFlagClear, 0x31b);
    if (pos == 119) {
        Engine_GameFlagSet(0x31b);
    } else if (pos == 121) {
        Engine_GameFlagSet(0x31a);
    }
    Scene_RunActorFormation(0);
}

s32 *SceneActor_FindSlotByTilePosition(s32 x, s32 z)
{

    s32 **slots = (s32 **)(gWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

/* Push the faced block and leader together, then refresh the current puzzle.
 * The coordinate lookup and position lifetime follow the exact Biribino
 * push-block family; Soru retains its own entrance-dependent flag updates. */
void SoruIriguchi_PushFacedBlock(void)
{
    struct FieldActor *leader;
    struct FieldActor *block;
    s32 zero;
    s32 step;
    u32 dir;
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;

    leader = Object_GetById(0);
    dir = leader->facing >> 12;
    block = (struct FieldActor *)SceneActor_FindSlotByTilePosition(
        (leader->x.part.pixel + (gSoruPushSteps[dir] >> 16)) >> 4,
        (leader->z.part.pixel + (s16)gSoruPushSteps[dir]) >> 4);
    if (block != NULL) {
        zero = 0;
        block->unknown_22 = 2;
        p = pos;
        step = gSoruPushSteps[dir];
        p[0].fixed = block->x.fixed + (step & -0x10000);
        p[1].fixed = block->y.fixed;
        p[2].fixed = block->z.fixed + (step << 16);
        if (((s32 (*)())Object_CheckMovementCollision)((s32)block, (s32)p) <= 0) {
            Object_SetMode(leader, 8);
            Engine_TaskWait(15);
            Engine_AudioPlayCue(185);
            block->speed = 0x3333;
            block->acceleration = 0x3333;
            Engine_ObjectSetPosition(block, p[0].fixed, p[1].fixed, p[2].fixed);
            leader->speed = 0x3333;
            leader->acceleration = 0x3333;
            Engine_ObjectSetPosition(leader, p[0].fixed, p[1].fixed, p[2].fixed);
            Engine_ObjectCommitPosition(block);
            block->x.fixed = p[0].fixed;
            block->z.fixed = p[2].fixed;
            block->velocity_x = zero;
            block->velocity_z = zero;
            Object_SetMode(leader, 1);
            switch (gGameState.entrance) {
            case 11:
            case 12:
            case 13:
                Scene_UpdateOuterActor9Flags();
                Scene_UpdateOuterActor10Flags();
                break;
            case 14:
            case 15:
            case 16:
                Scene_UpdateFormationActor9Flags();
                Scene_UpdateFormationActor10Flags();
                Scene_UpdateFormationActor11Flags();
                Scene_UpdateFormationActor12Flags();
                Scene_UpdateFormationActor13Flags();
                Scene_UpdateFormationActor14Flags();
                break;
            }
        }
    }
}

/* The sanctum entrance's scene start: each of its two scenes runs its own
   entry setup. */
s32 Scene_Initialize(void)
{
    s32 scenario = gGameState.scene;

    if (scenario == (s32)&SceneId_SoruIriguchi2) {
        SoruIriguchi_ApplyEntryState();
    } else if (scenario == (s32)&SceneId_SoruIriguchi1) {
        FieldScene_RunSceneEntryHook();
    }
    return 0;
}

void SoruIriguchi_ApplyEntryState(void)
{
    GameFlag_Set(0x144);
    *(s32 *)((*(u8 *volatile *)&gWork + 0x1c0)) = 0x100;
    if (GameFlag_IsSet(0x814) != 0) {
        s32 *timer = &SoruIriguchi_CueTimer;
        s32 zero = 0;
        *timer = zero;
        Call2(Engine_TaskAddCallback, (s32)Scene_UpdateCueTimer, 0xc80);
    }
    if (GameFlag_IsSet(0x879) != 0) {
        Map_CopyCellAttributes(5, 6, 1, 1, 6, 6);
        Map_CopyCellAttributes(5, 6, 1, 1, 7, 6);
        Map_CopyCellAttributes(5, 6, 1, 1, 8, 6);
        Map_CopyCellAttributes(0, 1, 3, 1, 6, 5);
    }
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Actor_SetPosition(8, 0x780000, 0xe80000);
        Map_CopyCellAttributes(2, 10, 1, 1, 6, 14);
        Map_CopyCellAttributes(2, 10, 1, 1, 7, 14);
        Map_CopyCellAttributes(2, 10, 1, 1, 8, 14);
    }
}

/* Opens the scene with the window transition, playing sound 141 when flag
 * 0x814 is set, then prepares the entrance the party arrived by. Entrances 1
 * and 2 change the map once flag 0x81a is set. Entrance 3 clears actor 9's
 * sprite flags. Entrance 8 stores two game-state halfwords and runs a scene
 * until flag 0x802 is set. Entrances 11 to 13 clear the sprite flags of
 * actors 9 to 19, run a scene until flag 0x804 is set and place actors 9 and
 * 10 by flag. Entrances 14 to 16 clear those of actors 9 to 14, run a scene
 * until flag 0x825 is set, run one more step, set flag 0x234 and change the
 * map once flag 0x821 is set. */
void FieldScene_RunSceneEntryHook(void)
{
    s32 entrance;
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (Engine_GameFlagIsSet(0x814) != 0) {
        BattleFx_SetQueuedSoundAndPlay(141);
        Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
        InitializeSceneRecordBuffer();
    }

    entrance = gGameState.entrance;
    switch (entrance) {
    case 1:
    case 2:
        if (Engine_GameFlagIsSet(0x81a) != 0) {
            Engine_MapCopyCellsTo(1, 109, 4, 81, 1, 1);
            Engine_MapCopyCellsTo(0, 70, 30, 42, 1, 1);
            Engine_MapCopyCellsTo(0, 29, 3, 1, 3, 2);
            Engine_MapCopyCellAttributes(0, 29, 3, 2, 3, 1);
            Engine_MapRedraw();
        }
        break;

    case 3:
        Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
        break;

    case 8:
        gGameState.retreat_scene = (s32)&SceneId_SoruIriguchi1;
        gGameState.retreat_entrance = 8;
        if (GameFlag_IsSet(0x802) == 0)
            Scene_EnterSolSanctum();
        break;

    case 11:
    case 12:
    case 13:
        Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(15), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(16), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(17), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(19), 0);
        if (Engine_GameFlagIsSet(0x804) == 0)
            Scene_SukuretaSuspectsHiddenPassage();
        if (Engine_GameFlagIsSet(0x303) != 0)
            Call3(Engine_ActorSetPosition, 9, 0x5d80000, 0x880000);
        else if (GameFlag_IsSet(0x302) != 0)
            Actor_SetPosition(9, 0x5f80000, 0x880000);
        if (Engine_GameFlagIsSet(0x301) != 0)
            Call3(Engine_ActorSetPosition, 10, 0x7180000, 0x880000);
        else if (Engine_GameFlagIsSet(0x300) != 0)
            Call3(Engine_ActorSetPosition, 10, 0x7380000, 0x880000);
        break;

    case 14:
    case 15:
    case 16:
        Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
        if (Engine_GameFlagIsSet(0x825) == 0)
            Scene_RunTransitionCue();
        Scene_RunActorFormation(1);
        Engine_GameFlagSet(0x234);
        if (GameFlag_IsSet(0x821) != 0) {
            Engine_MapCopyCellsTo(0, 71, 100, 71, 1, 1);
            Map_CopyCellsTo(122, 20, 120, 30, 1, 2);
            Call6(Engine_MapCopyCellAttributes, 122, 20, 1, 2, 120, 30);
            Engine_MapRedraw();
        }
        break;

    }
}

void Scene_EnterSolSanctum(void)
{
    s32 record;

    Engine_EventBegin();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_EventOpenScreen();
    Engine_ActorSetAnimation(0, 0);
    Engine_EventWait(4);
    Camera_MoveTo(-1, -1, -1, 0);
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0x4c80000, -1, 0x880000, 1);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_SUKURETA, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_JASMINE, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_SUKURETA, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_JASMINE, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_GERALD, 0x9999, 0x4ccc);
    Engine_ActorSetAnimation(ACTOR_GERALD, 2);
    Engine_ActorSetAnimation(ACTOR_JASMINE, 2);
    Engine_ActorSetAnimation(ACTOR_SUKURETA, 2);
    Actor_SetDestinationOffset(ACTOR_GERALD, -16, 0);
    Actor_SetDestinationOffset(ACTOR_JASMINE, 16, 0);
    Actor_SetDestinationOffset(ACTOR_SUKURETA, 0, -32);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Engine_ActorSetAnimation(ACTOR_GERALD, 0);
    Engine_ActorSetAnimation(ACTOR_JASMINE, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xc000, 0);
    Engine_ActorWaitForMove(ACTOR_SUKURETA);
    Engine_ActorSetAnimation(ACTOR_SUKURETA, 1);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 20);
    Engine_ActorJump(ACTOR_SUKURETA, 4, 20);
    Engine_EventSetMessage((s32)MsgSoruSukuretaFirstTimeAtSol);
    Event_AskYesNo(0x4008, 0);
    Engine_EventWait(20);
    Camera_MoveTo(0x4c80000, -1, 0x940000, 1);
    Engine_ActorSetAnimation(ACTOR_GERALD, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorSetAnimation(ACTOR_JASMINE, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_JASMINE, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorSetAnimation(ACTOR_SUKURETA, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_SUKURETA, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Engine_ActorWaitForMove(ACTOR_SUKURETA);
    Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Engine_ActorSetAnimation(ACTOR_JASMINE, 1);
    Engine_ActorSetAnimation(ACTOR_SUKURETA, 1);
    GameFlag_Set(FLAG_SOL_SANCTUM_ENTERED);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    Engine_EventEnd();
}

/* Sukureta suspects a hidden passage at the sanctum entrance, and the party
 * decides whether to split up and search. */
void Scene_SukuretaSuspectsHiddenPassage(void)
{
    u8 *record;

    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();

    record = (u8 *)Object_GetById(0);
    if (record != 0)
        Engine_ActorSetPosition(8, RECORD_A32(record), RECORD_B32(record));
    record = (u8 *)Object_GetById(0);
    if (record != 0)
        Engine_ActorSetPosition(5, RECORD_A32(record), RECORD_B32(record));
    record = (u8 *)Object_GetById(0);
    if (record != 0)
        Engine_ActorSetPosition(1, RECORD_A32(record), RECORD_B32(record));

    Actor_SetSpeed(ACTOR_SUKURETA, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_JASMINE, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_GERALD, 0x9999, 0x4ccc);
    Engine_ActorSetAnimation(ACTOR_GERALD, 2);
    Engine_ActorSetAnimation(ACTOR_JASMINE, 2);
    Engine_ActorSetAnimation(ACTOR_SUKURETA, 2);
    Actor_SetDestinationOffset(ACTOR_GERALD, -16, 0);
    Actor_SetDestinationOffset(ACTOR_JASMINE, 16, 0);
    Call3(Engine_ActorSetDestinationOffset, 8, 0, -16);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetAnimation(ACTOR_SUKURETA, 1);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
    Engine_ActorSetAnimation(ACTOR_GERALD, 0);
    Engine_ActorSetAnimation(ACTOR_JASMINE, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 30);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 30);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x4000, 30);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_SUKURETA, 2);
    Engine_ActorSetDestinationOffset(8, 0, -16);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetAnimation(8, 1);
    Engine_EventWait(6);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_SUKURETA, 2);
    Engine_ActorSetDestinationOffset(8, 0, -32);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetAnimation(ACTOR_SUKURETA, 1);

    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x06310000, -1, 0x00960000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(10);
    Camera_SetSpeed(0x13333, 0x2666);
    Camera_MoveTo(0x06550000, -1, 0x00640000, 1);
    Engine_CameraWaitForMove();
    Camera_MoveTo(0x06b60000, -1, 0x00640000, 1);
    Engine_CameraWaitForMove();
    Engine_ActorSetAnimation(ACTOR_SUKURETA, 1);
    Call4(Engine_CameraMoveTo, 0x06d80000, -1, 0x00960000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x06840000, -1, 0x01000000, 1);
    Engine_CameraWaitForMove();
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(10);

    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 10);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 20);
    Engine_EventSetMessage((s32)MsgSoruWrongSukureta);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x102, 60);
    Engine_ActorStartRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 5, 0x102);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_SUKURETA, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 4);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Engine_ActorFaceEachOther(0, 5, 0);
    Engine_EventWait(40);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorRunRepeatedMotion(5, 1);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 4);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 20);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(10);
    Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 60);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x4000, 20);
    Engine_ActorJump(ACTOR_SUKURETA, 2, 20);

    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 40);
    Actor_FaceDirection(ACTOR_GERALD, 0, 20);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventSetMessage((s32)MsgSoruDangerousSplitStay);
        Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
        Engine_EventShowMessageAndWait(1, 0, 10);
    } else {
        Engine_EventSetMessage((s32)MsgSoruCantSeriouslyWant);
        Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 20);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 60);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 40);
        Engine_ActorRunRepeatedMotion(1, 1);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(1, 3);
        Engine_EventWait(10);
        Actor_FaceDirection(ACTOR_GERALD, 0, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 30);
        Engine_ActorRunRepeatedMotion(1, 1);
        Engine_EventWait(10);
        Engine_EventShowMessageAndWait(1, 0, 10);
    }

    Actor_SetSpeed(ACTOR_SUKURETA, 0x9999, 0x4ccc);
    Engine_ActorSetAnimation(ACTOR_SUKURETA, 2);
    Engine_ActorSetDestinationOffset(8, 0, 48);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetAnimation(8, 1);
    Engine_EventWait(6);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Engine_ActorFaceDirection(5, 0xa000, 0);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetAnimation(8, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_JASMINE, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(1, 2);

    record = (u8 *)Object_GetById(0);
    if (record != 0)
        Engine_ActorSetDestination(1, RECORD_A16(record), RECORD_B16(record));
    Engine_ActorSetAnimation(5, 2);
    record = (u8 *)Object_GetById(0);
    if (record != 0)
        Engine_ActorSetDestination(5, RECORD_A16(record), RECORD_B16(record));
    Engine_ActorSetAnimation(8, 2);
    record = (u8 *)Object_GetById(0);
    if (record != 0)
        Engine_ActorSetDestination(8, RECORD_A16(record), RECORD_B16(record));

    Engine_ActorWaitForMove(8);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
    Engine_ActorSetAnimation(ACTOR_SUKURETA, 1);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Engine_ActorSetAnimation(5, 1);
    GameFlag_Set(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE);
    Engine_GameFlagClear(FLAG_ARRIVAL_EVENT_PENDING);
    Engine_EventEnd();
}

void Scene_RunTransitionCue(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x204;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(8, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Engine_ActorSetAnimation(8, 2);
    Actor_SetDestinationOffset(8, 24, -10);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetAnimation(8, 1);
    Engine_EventWait(6);
    Actor_FaceDirection(8, 0xb000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 40);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x6880000, -1, 0x20c0000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x7580000, -1, 0x20c0000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Camera_SetSpeed(0x33333, 0x6666);
    Camera_MoveTo(0x6e90000, -1, 0x2240000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(8, 2);
    Actor_FaceDirection(8, 0, 30);
    Engine_EventSetMessage((s32)MsgSoruMoreStatuesOutOfReach);
    Event_ShowMessageAndWait(0x4008, 0, 10);
    Actor_ShowEmote(8, 0x100, 40);
    Engine_ActorRunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x5000, 20);
    Event_ShowMessageAndWait(0x4008, 0, 10);
    Engine_ActorSetAnimation(8, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(8);
    Actor_SetPosition(8, 0, 0);
    GameFlag_Set(0x825);
    Engine_EventEnd();
}

void Scene_RunActorFormation(s32 a0)
{
    u32 i;
    s32 record;

    Engine_MapCopyCellAttributes(122, 20, 1, 1, 100, 32);
    Map_CopyCellAttributes(122, 20, 1, 1, 104, 32);
    Map_CopyCellAttributes(122, 20, 1, 1, 108, 32);
    Map_CopyCellAttributes(122, 20, 1, 1, 112, 32);
    Map_CopyCellAttributes(122, 20, 1, 1, 116, 32);
    Map_CopyCellAttributes(122, 20, 1, 1, 120, 32);
    if (GameFlag_IsSet(0x311) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 100, 32);
        if (a0 == 0) {
            goto L_02001890;
        }
        Actor_SetPosition(9, 0x6380000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x310) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 100, 32);
            if (a0 != 0) {
                Actor_SetPosition(9, 0x6580000, 0x2080000);
            }
        }
    }
    L_02001890:;
    if (GameFlag_IsSet(0x313) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 104, 32);
        if (a0 == 0) {
            goto L_020018f2;
        }
        Actor_SetPosition(10, 0x6780000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x312) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 104, 32);
            if (a0 != 0) {
                Actor_SetPosition(10, 0x6980000, 0x2080000);
            }
        }
    }
    L_020018f2:;
    if (GameFlag_IsSet(0x315) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 108, 32);
        if (a0 == 0) {
            goto L_02001956;
        }
        Actor_SetPosition(11, 0x6b80000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x314) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 108, 32);
            if (a0 != 0) {
                Actor_SetPosition(11, 0x6d80000, 0x2080000);
            }
        }
    }
    L_02001956:;
    if (GameFlag_IsSet(0x317) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 112, 32);
        if (a0 == 0) {
            goto L_020019b8;
        }
        Actor_SetPosition(12, 0x6f80000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x316) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 112, 32);
            if (a0 != 0) {
                Actor_SetPosition(12, 0x7180000, 0x2080000);
            }
        }
    }
    L_020019b8:;
    if (GameFlag_IsSet(0x319) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 116, 32);
        if (a0 == 0) {
            goto L_02001a1c;
        }
        Actor_SetPosition(13, 0x7380000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x318) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 116, 32);
            if (a0 != 0) {
                Actor_SetPosition(13, 0x7580000, 0x2080000);
            }
        }
    }
    L_02001a1c:;
    if (GameFlag_IsSet(0x31b) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 120, 32);
        if (a0 == 0) {
            goto L_02001a7e;
        }
        Actor_SetPosition(14, 0x7780000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x31a) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 120, 32);
            if (a0 != 0) {
                Actor_SetPosition(14, 0x7980000, 0x2080000);
            }
        }
    }
    L_02001a7e:;
}

void FieldScene_RunScriptedStep953(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgFieldDoorTightlyLocked, 1);
    Engine_EventEnd();
}

void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 record;
    s32 value;
    s32 *timer;
    s32 v3;

    timer = &SoruIriguchi_CueTimer;
    if (*timer != 0) {
        v3 = (*timer - 1);
        *timer = (*timer - 1);
        if (v3 != 40) {
            goto L_02001b14;
        }
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    } else {
        value = Engine_RandomNext();
        if (((u32)(((value << 4) - value) << 3) >> 16) == 0) {
            Audio_PlayCue(138);
            Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
            *timer = 80;
        }
    }
    L_02001b14:;
}
