#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SORU.H"
#include "CALL.H"

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

    Event_Begin();
    v5 = 3;
    v6 = 2;
    Audio_PlayCue(181);
    Map_CopyCellsTo(16, 28, 21, 3, v5, v6);
    Task_Wait(10);
    Map_CopyCellsTo(16, 30, 21, 3, v5, v6);
    Task_Wait(10);
    Map_CopyCellsTo(16, 32, 21, 3, v5, v6);
    Task_Wait(10);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 98);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
    Event_Wait(10);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(2);
    Event_End();
}

void SceneDialogue_RunFlag81aMessageBranch(void)
{

    Event_Begin();

    if (GameFlag_IsSet(0x81a) != 0) {
        Message_ShowCentered((s32)MsgSoruMinotaurReliefBothEyes, 1);
    } else {
        Message_ShowCentered((s32)MsgSoruMinotaurReliefOneEye, 1);
        if (GameFlag_IsSet(0xf01) != 0) {
            u16 *p = (u16 *)(gWork + 370);
            u16 val = 1;
            *p = val;
        }
    }

    Event_End();
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
            Event_Begin();
            Battle_ResetEffectCounter();
            v5 = 1;
            Audio_PlayCue(182);
            Map_CopyCellsTo(0, 70, 30, 42, v5, v5);
            Map_Redraw();
            Event_Wait(40);
            id = (s32)MsgSoruSetSmallGem;
            Message_ShowCentered(id, 1);
            Event_Wait(20);
            v6 = 3;
            Audio_PlayCue(183);
            Map_CopyCellsTo(0, 29, 3, 1, v6, 2);
            Map_CopyCellAttributes(0, 29, 3, 2, v6, v5);
            Map_CopyCellsTo(1, 109, 4, 81, v5, v5);
            Map_Redraw();
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
            Event_Wait(20);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
            Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
            Event_Wait(20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 40);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
            Actor_Jump(ACTOR_PARTY_LEADER, 4, 20);
            Actor_Jump(ACTOR_PARTY_LEADER, 6, 40);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Event_Wait(40);
            Message_ShowCentered(id + 1, 1);
            GameFlag_Set(0x143);
            GameFlag_Set(0x81a);
            Event_End();
        }
    }
}

void FieldScene_RunFlag821Dialogue(void)
{

    u8 *work;

    Event_Begin();

    if (GameFlag_IsSet(0x821) != 0) {
        Message_ShowCentered((s32)MsgSoruMinotaurReliefBothEyes, 1);
    } else if (GameFlag_IsSet(0xf02) != 0) {
        work = gWork;
        Message_ShowCentered((s32)MsgSoruMinotaurReliefOneEye, 1);
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
        Message_ShowCentered((s32)MsgSoruMinotaurReliefOneEye, 1);
    }

    Event_End();
}

void SoruIriguchi_SetSmallGem(void)
{
    s32 message;

    if (GameFlag_IsSet(3842) != 0 && GameFlag_IsSet(GATE_ID) == 0) {
        Event_Begin();
        Battle_ResetEffectCounter();
        Engine_AudioPlayCue(182);
        Engine_MapCopyCellsTo(0, 71, 100, 71, 1, 1);
        Engine_MapRedraw();
        Engine_EventWait(40);
        message = (s32)MsgSoruSetSmallGem;
        Engine_MessageShowCentered(message, 1);
        Event_Wait(20);
        Engine_AudioPlayCue(183);
        Map_CopyCellsTo(122, 20, 120, 30, 1, 2);
        Map_CopyCellAttributes(122, 20, 1, 2, 120, 30);
        Engine_MapRedraw();
        Value3(Engine_WorkSetValuesIfNonNegative, 65536, 65536, 65536);
        Engine_EventWait(20);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 256, 0);
        Value3(Engine_WorkSetValuesIfNonNegative, 131072, 131072, 65536);
        Engine_EventWait(20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 40);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 32768, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 10);
        Actor_Jump(ACTOR_PARTY_LEADER, 4, 20);
        Actor_Jump(ACTOR_PARTY_LEADER, 6, 40);
        Value3(Engine_WorkSetValuesIfNonNegative, -1, -1, 58982);
        Engine_EventWait(40);
        Engine_MessageShowCentered(message + 1, 1);
        GameFlag_Set(0x143);
        GameFlag_Set(GATE_ID);
        Event_End();
    }
}

void Scene_UpdateOuterActor9Flags(void)
{
    s32 *work = Engine_ActorGet(9);
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
    s32 *work = Engine_ActorGet(10);
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
    s32 *work = Engine_ActorGet(9);
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
    s32 *work = Engine_ActorGet(10);
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
    s32 *work = Engine_ActorGet(11);
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
    s32 *work = Engine_ActorGet(12);
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
    s32 *work = Engine_ActorGet(13);
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
    s32 *work = Engine_ActorGet(14);
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

void SoruIriguchi_PushFacedBlock(void)
{
    struct FieldActor *leader;
    struct FieldActor *block;
    s32 zero;
    s32 step;
    u32 dir;
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;

    leader = Engine_ActorGet(0);
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
            Engine_ObjectSetAnimation(leader, 8);
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
            Engine_ObjectSetAnimation(leader, 1);
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
