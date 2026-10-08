#include "TYPES.H"
#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "HEYA.H"
#include "STAGED_ACTOR.H"
#include "SCENE_IDS.H"
#include "INVENTORY.H"
#include "text/MSG_IDS.H"

struct SceneActor {
    u8 pad00[99];
    u8 mode;
};

struct EffectRecord {
    u8 pad00[6];
    u16 angle;
    u8 pad08[83];
    u8 state;
    u8 pad5c[6];
    u8 active;
};

s32 Object_CheckMovementCollision();

void Battle_ResetEffectCounter();
void Party_SetFields1ceAnd1d0();
void BattleFx_SetWeightedResult();

void UiText_ShowCenteredMessage();
void Ui_SetRenderResultFromObject();
void ConfigureSceneMotionFlags();
void Object_RefreshSelectorById();

void Audio_PlayCueFromEventWork();

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
};

void FuneHeya_RunWalkerStep();
void UpdateActorNineEffectMode();

struct Half {
    u16 v;
};

enum ExtendedChoreographyMessage {
    MSG_WONDER_COULD_HAVE_HAPPENED = 0x1d26,
    MSG_ITS_TOO_LATE_HIRE_MERCENARIES = 0x1d30,
    MSG_BUT_WE_CANT_SEND_SHIP = 0x1d31,
    MSG_LONGER_WE_SIT_HERE_MORE = 0x1d4e,
    MSG_IF_WE_ARENT_GOING_SET = 0x1d56,
    MSG_NOW_WANT_SEE_CAPTAIN_TOO = 0x1d91,
    MSG_YOURE_TRYING_LAUNCH_SHIP = 0x1d93,
    MSG_BAD_LUCK_LOSING_MY_LUCKY = 0x1dcd,
    MSG_IF_SHIP_FROM_TOLBI_HAD = 0x1dd4,
    MSG_ITS_MY_LUCKY_ANCHOR = 0x1ddb,
    MSG_WE_DONT_KNOW_MIGHT_HAPPEN = 0x1e06,
    MSG_THESE_PROUD_WARRIORS_NOT_GOING = 0x1e13,
    MSG_OUR_REPLACEMENT_NEVER_ARRIVED_BUT = 0x1e27,
    MSG_CAST_OFF = 0x1e3b,
    MSG_ROW_THOSE_OARS = 0x1e3c,
    MSG_WERE_OFF = 0x1e3d,
    MSG_IM_TURNING = 0x1e43,
    MSG_HEY_ARE_YOU_OK = 0x1e6e,
    MSG_OHHHH_NOOOO_GOING_MAKE_ME = 0x1e81,
    MSG_HA_HA_HA_ROWING_FEEL = 0x1e84,
    MSG_GIVES_ME_CHILLS_THINK_COULD = 0x1ea1,
    MSG_ROBIN_YOUVE_GOT_GOOD_EYE = 0x1ea2,
    MSG_HO_HO_PERSON_GOING_GET = 0x1ea6,
    MSG_OARSMAN_WAS_INJURED = 0x1eb2,
    MSG_WONDER_WHATS_WRONG_SHIP_SHOULDNT = 0x1ec1,
    MSG_THING_HAS_KAJA_HIS_MEN = 0x1ece,
    MSG_MONSTERS_EVERYWHERE_IM_STUCK_ROWING = 0x1ecf,
    MSG_SHIP_STARTING_LIST_IF_WE = 0x1ed0,
    MSG_HOW_MANY_MONSTERS_OUT_THERE = 0x1ed1,
    MSG_ANOTHER_MONSTER_ISNT_FIRST_CLASS = 0x1ed2,
    MSG_DONT_CARE_TAKES_JUST_HURRY = 0x1edb,
    MSG_IF_THOSE_MONSTERS_COME_BACK = 0x1edc,
    MSG_BOATS_ROCKING_MUCH_IM_CERTAIN = 0x1edd,
    MSG_HAD_IDEA_THERE_WERE_MANY = 0x1ede,
    MSG_WERE_SURROUNDED_BY_MONSTERS_STILL = 0x1edf,
    MSG_HATE_ARGUING = 0x1f48,
    MSG_SORRY_EVERYONE_BUT_WE_NEED = 0x1f78,
    MSG_IM_SPREADING_GOODWILL_WHEREVER_TRAVEL = 0x1f7b,
    MSG_SHIPS_CREW_READY_FOR_ANYTHING = 0x1f7d,
    MSG_SHIPS_CREW_READY_FOR_ANYTHING_2 = 0x1f7f,
    MSG_GOOD_SHIP_HAS_ARRIVED_SAFELY = 0x1f81
};

void Scene_RunConditionalActorPresentation();

void SceneState_ApplyActor8FourFlags();

void FieldScene_RunPositionTransferPresentation();
void SceneActor_SetFlagBit3ForActors28To35(void);

void FuneHeya_PlaceFoundActors();
void FieldScene_RunStepThen10();

enum { CabinSpeakerRequest = 0xa01b };

void Object_SetActionById();

void ObjectMotion_SetHorizontalPositionWithTerrain();
void FuneHeya_PoseSceneActor();
void ObjectMotion_WaitForAnimationChange();
void ObjectMotion_ArmCallback();
void Engine_EventWaitForScreen();
void Event_CallWithLastActiveObjectId();
u8 *Battle_GetWorkObject1e0();
void FieldScene_CallPairWith10(s32 a, u16 b);

static __inline__ void SetPose(u8 *rec, s32 pose)
{
    ((struct FieldActor *)rec)->facing = pose;
}

void Object_SetActionCallbackAndRefreshById();
void FieldScene_RunStepThen10(s32 a);
void ConfigureSceneMotionFlags(s32 x, s32 y, s32 z, u32 flags);

void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2);
void FieldScene_InstallFlaggedActors10To17(u8 *src);
void FieldScene_RunPositionTransferPresentation(void);

extern struct EventWork *gEventWork;
void SceneActor_SetFlagBit3ForActors28To35();
void FieldScene_InstallFlaggedActors10To17();

void FieldScene_CallPairWith10(s32 a, u16 b);
void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2);

s32 SceneActor_CheckBucketOffsetPoint(s32 bucket)
{
    struct FieldActor *obj = Object_GetById(0);
    u32 ofs = FuneHeya_TurnSteps[bucket];
    s32 x = obj->x.part.pixel + ((s32)ofs >> 16);
    s32 z = obj->z.part.pixel + (s32)(s16)ofs;

    if (FindActorNearPosition(x, z) != 0) {
        return 0;
    }

    {
        s32 point[3];
        point[0] = x << 16;
        point[1] = obj->y.fixed;
        point[2] = z << 16;

        if (Object_CheckMovementCollision(obj, point) != 0) {
            return 0;
        }
    }

    return 1;
}

s32 SceneState_ApplyLevelFromFlags(void)
{
    s32 ret = 0;

    if (Engine_GameFlagIsSet(0x92b) != 0) {
        ret = 3;
    } else if (Engine_GameFlagIsSet(0x92a) != 0) {
        ret = 2;
    } else if (Engine_GameFlagIsSet(0x929) != 0) {
        ret = 1;
    }

    return FuneHeya_FindFirstSetFlag(ret, 1);
}

void SceneDialogue_ShowLine1ECETo1ED0(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x92c)) Engine_EventSetMessage((s32)MsgFuneThingHasKajaHisMen);
    else if (Engine_GameFlagIsSet(0x935)) Engine_EventSetMessage((s32)MsgFuneMonstersEverywhereImStuckRowing);
    else Engine_EventSetMessage((s32)MsgFuneShipStartingListIfWe);
    Engine_EventShowMessage(0x12, 0); Engine_EventEnd();
}

void SceneDialogue_RunActor19TwoFlagLineA(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x92d)) Engine_EventSetMessage((s32)MsgFuneThingHasKajaHisMen);
    else if (Engine_GameFlagIsSet(0x936)) Engine_EventSetMessage((s32)MsgFuneMonstersEverywhereImStuckRowing);
    else Engine_EventSetMessage((s32)MsgFuneShipStartingListIfWe);
    Engine_EventShowMessage(0x13, 0); Engine_EventEnd();
}

void SceneDialogue_RunActor20TwoFlagLine(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x92e)) Engine_EventSetMessage((s32)MsgFuneThingHasKajaHisMen);
    else if (Engine_GameFlagIsSet(0x937)) Engine_EventSetMessage((s32)MsgFuneMonstersEverywhereImStuckRowing);
    else Engine_EventSetMessage((s32)MsgFuneShipStartingListIfWe);
    Engine_EventShowMessage(0x14, 0); Engine_EventEnd();
}

void SceneDialogue_ShowLine1ED1Or1ED2(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x92f)) Engine_EventSetMessage((s32)MsgFuneHowManyMonstersOutThere);
    else Engine_EventSetMessage((s32)MsgFuneAnotherMonsterIsntFirstClass);
    Engine_EventShowMessage(21, 0); Engine_EventEnd();
}

void SceneDialogue_RunActor22TwoFlagLine(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x930)) Engine_EventSetMessage((s32)MsgFuneThingHasKajaHisMen);
    else if (Engine_GameFlagIsSet(0x939)) Engine_EventSetMessage((s32)MsgFuneMonstersEverywhereImStuckRowing);
    else Engine_EventSetMessage((s32)MsgFuneShipStartingListIfWe);
    Engine_EventShowMessage(22, 0); Engine_EventEnd();
}

void SceneDialogue_RunActor23BranchedDialogue(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x931)) Engine_EventSetMessage((s32)MsgFuneThingHasKajaHisMen);
    else if (Engine_GameFlagIsSet(0x93a)) Engine_EventSetMessage((s32)MsgFuneMonstersEverywhereImStuckRowing);
    else Engine_EventSetMessage((s32)MsgFuneShipStartingListIfWe);
    Engine_EventShowMessage(23, 0); Engine_EventEnd();
}

void SceneDialogue_RunActor24BranchedDialogue(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x932)) Engine_EventSetMessage((s32)MsgFuneThingHasKajaHisMen);
    else if (Engine_GameFlagIsSet(0x93b)) Engine_EventSetMessage((s32)MsgFuneMonstersEverywhereImStuckRowing);
    else Engine_EventSetMessage((s32)MsgFuneShipStartingListIfWe);
    Engine_EventShowMessage(24, 0); Engine_EventEnd();
}

void SceneDialogue_RunActor25FlaggedLine(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x933)) Engine_EventSetMessage((s32)MsgFuneHowManyMonstersOutThere);
    else Engine_EventSetMessage((s32)MsgFuneAnotherMonsterIsntFirstClass);
    Engine_EventShowMessage(25, 0); Engine_EventEnd();
}

void SceneDialogue_RunActor18TwoFlagLine(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x92c)) Engine_EventSetMessage((s32)MsgFuneDontCareTakesJustHurry);
    else if (Engine_GameFlagIsSet(0x935)) Engine_EventSetMessage((s32)MsgFuneIfThoseMonstersComeBack);
    else Engine_EventSetMessage((s32)MsgFuneBoatsRockingMuchImCertain);
    Engine_EventShowMessage(18, 0); Engine_EventEnd();
}

void SceneDialogue_RunActor19TwoFlagLineB(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x92d)) Engine_EventSetMessage((s32)MsgFuneDontCareTakesJustHurry);
    else if (Engine_GameFlagIsSet(0x936)) Engine_EventSetMessage((s32)MsgFuneIfThoseMonstersComeBack);
    else Engine_EventSetMessage((s32)MsgFuneBoatsRockingMuchImCertain);
    Engine_EventShowMessage(19, 0); Engine_EventEnd();
}

void SceneDialogue_ShowLine1EDBTo1EDDActor20(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x92e)) Engine_EventSetMessage((s32)MsgFuneDontCareTakesJustHurry);
    else if (Engine_GameFlagIsSet(0x937)) Engine_EventSetMessage((s32)MsgFuneIfThoseMonstersComeBack);
    else Engine_EventSetMessage((s32)MsgFuneBoatsRockingMuchImCertain);
    Engine_EventShowMessage(20, 0); Engine_EventEnd();
}

void SceneDialogue_RunActor21FlaggedLine(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x92f)) Engine_EventSetMessage((s32)MsgFuneHadIdeaThereWereMany);
    else Engine_EventSetMessage((s32)MsgFuneWereSurroundedByMonstersStill);
    Engine_EventShowMessage(21, 0); Engine_EventEnd();
}

void SceneDialogue_RunActor22BranchedDialogue(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x930)) Engine_EventSetMessage((s32)MsgFuneDontCareTakesJustHurry);
    else if (Engine_GameFlagIsSet(0x939)) Engine_EventSetMessage((s32)MsgFuneIfThoseMonstersComeBack);
    else Engine_EventSetMessage((s32)MsgFuneBoatsRockingMuchImCertain);
    Engine_EventShowMessage(22, 0); Engine_EventEnd();
}

void SceneDialogue_ShowLine1EDBTo1EDDActor23(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x931)) Engine_EventSetMessage((s32)MsgFuneDontCareTakesJustHurry);
    else if (Engine_GameFlagIsSet(0x93a)) Engine_EventSetMessage((s32)MsgFuneIfThoseMonstersComeBack);
    else Engine_EventSetMessage((s32)MsgFuneBoatsRockingMuchImCertain);
    Engine_EventShowMessage(23, 0); Engine_EventEnd();
}

void SceneDialogue_ShowLine1EDBTo1EDDActor24(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x932)) Engine_EventSetMessage((s32)MsgFuneDontCareTakesJustHurry);
    else if (Engine_GameFlagIsSet(0x93b)) Engine_EventSetMessage((s32)MsgFuneIfThoseMonstersComeBack);
    else Engine_EventSetMessage((s32)MsgFuneBoatsRockingMuchImCertain);
    Engine_EventShowMessage(24, 0); Engine_EventEnd();
}

void SceneDialogue_ShowLine1EDEOr1EDF(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x933)) Engine_EventSetMessage((s32)MsgFuneHadIdeaThereWereMany);
    else Engine_EventSetMessage((s32)MsgFuneWereSurroundedByMonstersStill);
    Engine_EventShowMessage(25, 0); Engine_EventEnd();
}

void FieldScene_RunPrimarySequence(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    struct FieldActor *record;

    Engine_EventBegin();
    Engine_EventSetMessage(a1);
    Engine_EventOpenMessage(a0, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        FieldScene_RunStepThen10(a0);
        Engine_ActorSetAnimation(a0, 2);
        record = Object_GetById(0);
        if (record != 0) {
            Engine_ActorSetDestination(a0, record->x.part.pixel, record->z.part.pixel);
        }
        Engine_ActorWaitForMove(a0);
        Engine_ActorSetPosition(a0, 0, 0);
        Engine_GameFlagSet(0x300);
        Engine_GameFlagSet(a2);
    } else {
        gEventWork->message++;
        FieldScene_RunStepThen10(a0);
    }
    Engine_EventEnd();
}

void FieldScene_RunScene3b1SequenceC(void)
{
    struct FieldActor *leader;

    leader = (struct FieldActor *)Object_GetById(0);
    if ((u16)(leader->facing - 0x2000) > 0xc000) {
        if (Engine_GameFlagIsSet(0x928) != 0 && Engine_GameFlagIsSet(0x93e) == 0) {
            Engine_SanctumOpen(17);
        } else {
            Engine_SanctumOpen(15);
        }
    } else {
        Engine_EventBegin();
        if (Engine_GameFlagIsSet(0x93e) != 0) {
            Engine_EventSetMessage((s32)MsgFuneGoodShipHasArrivedSafely);
        } else if (Engine_GameFlagIsSet(0x8a0) != 0) {
            Engine_EventSetMessage((s32)MsgFuneHateArguing);
        } else if (Engine_GameFlagIsSet(0x928) != 0) {
            Engine_EventSetMessage((s32)MsgFuneShipsCrewReadyAnything);
        } else if (Engine_GameFlagIsSet(0x925) != 0) {
            Engine_EventSetMessage((s32)MsgFuneShipsCrewReadyForAnything);
        } else {
            Engine_EventSetMessage((s32)MsgFuneImSpreadingGoodwillWhereverTravel);
        }
        if (Engine_GameFlagIsSet(0x928) != 0 && Engine_GameFlagIsSet(0x93e) == 0) {
            Engine_EventShowMessage(17, 0);
        } else {
            Engine_EventShowMessage(15, 0);
        }
        Engine_EventEnd();
    }
}

void FuneHeya_RunFlagBranchSequence(void)
{

    Engine_EventBegin();
    Battle_ResetEffectCounter();
    gGameState.battle_start = 3;
    /* FAKEMATCH: the do/while orders the flag store before this call. */
    do {
        Engine_GameFlagClear(0x8f0);
    } while (0);
    if (Engine_GameFlagIsSet(0x928) == 0) {
        Party_SetFields1ceAnd1d0((s32)&SceneId_FuneHeya, 16);
        BattleFx_SetWeightedResult(62, 0);
    } else {
        if (Engine_GameFlagIsSet(0x929) == 0) {
            Party_SetFields1ceAnd1d0((s32)&SceneId_FuneHeya, 18);
            BattleFx_SetWeightedResult(62, 1);
        } else {
            if (Engine_GameFlagIsSet(0x92a) == 0) {
                Party_SetFields1ceAnd1d0((s32)&SceneId_FuneHeya, 20);
                BattleFx_SetWeightedResult(62, 2);
            }
        }
    }
    Engine_EventEnd();
}

void FieldScene_RunScene3b1SequenceD(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x301) != 0) {
        Engine_EventBegin();
        Ui_SetRenderResultFromObject(8);
        UiText_ShowCenteredMessage((s32)MsgFuneWhereGoingSlay, 1, 8);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x198, 134);
        FieldScene_CallPairWith10(0, 0x4000);
        Engine_EventEnd();
    }
}

void FieldScene_RunFlagGatedThreeActorSetup(void)
{
    if (Engine_GameFlagIsSet(0x922) == 0)
        return;

    Engine_EventBegin();
    Battle_ResetEffectCounter();
    Engine_CameraSetSpeed(0x19999, 0x3333);
    ConfigureSceneMotionFlags(0xe0 << 17, -1, 0x027e0000, 0x10000028u);
    Engine_EventSetMessage((s32)MsgFuneWonderCouldHaveHappened);

    FieldScene_RunStepThen10(8);
    FieldScene_RunStepThen10(10);
    FieldScene_CallPairWith10(8, 0x3000);
    FieldScene_RunStepThen10(8);
    FieldScene_CallPairWith10(10, 0xd000);
    FieldScene_RunStepThen10(10);
    FieldScene_CallPairWith10(9, 0x5000);
    FieldScene_RunStepThen10(9);

    Engine_ActorFaceDirection(8, FACING_EAST, 20);
    FieldScene_RunStepThen10(8);
    FieldScene_CallPairWith10(9, 0x8000);
    FieldScene_RunStepThen10(9);
    FieldScene_RunStepThen10(10);
    FieldScene_RunStepThen10(8);
    FieldScene_CallPairWith10(10, 0xb000);
    FieldScene_RunStepThen10(8);

    Engine_GameFlagSet(0x920);
    Engine_EventEnd();
}

void FieldScene_RunThreeActorPresentation(void)
{
    s32 request_a;
    s32 request_b;
    s32 action;

    if (GameFlag_IsSet(0x911) == 0) {
    } else {
        Engine_EventBegin();
        Battle_ResetEffectCounter();
        Camera_SetSpeed(0x26666, 0x4ccc);
        ConfigureSceneMotionFlags( 0x5b70000, -1, 0x1d00000, 0x10000014);
        Engine_ActorRunRepeatedMotion(13, 1);
        Engine_EventSetMessage((s32)MsgFuneIfWeArentGoingSet);
        FieldScene_RunStepThen10(0x200d);
        FieldScene_CallPairWith10(12, 0xd000);
        request_a = 0x800c;
        Actor_ShowEmote(12, 0x102, 20);
        Engine_ActorStartRepeatedMotion(12, 2);
        FieldScene_RunStepThen10(request_a);
        Engine_ActorRunRepeatedMotion(14, 1);
        Event_ShowMessageAndWait(0xa00e, 0, 20);
        FieldScene_CallPairWith10(12, 0);
        request_b = 0xa00e;
        Actor_ShowEmote(12, 0x101, 40);
        Actor_ShowEmote(14, 0x103, 40);
        Engine_ActorStartRepeatedMotion(14, 3);
        FieldScene_RunStepThen10(request_b);
        Actor_SetAttachedEffect(12, 0x102);
        Engine_EventWait(40);
        Engine_ActorStartRepeatedMotion(12, 3);
        FieldScene_RunStepThen10(request_a);
        Engine_ActorRunRepeatedMotion(14, 1);
        FieldScene_RunStepThen10(request_b);
        FieldScene_CallPairWith10(14, 0xb000);
        FieldScene_RunStepThen10(request_b);
        FieldScene_CallPairWith10(12, 0xd000);
        Actor_ShowEmote(12, 0x100, 30);
        Engine_ActorStartRepeatedMotion(12, 1);
        FieldScene_RunStepThen10(request_a);
        Engine_ActorSetAnimationAndWait(13, 4);
        FieldScene_RunStepThen10(0x200d);
        Engine_ActorStartRepeatedMotion(13, 2);
        FieldScene_RunStepThen10(0x200d);
        Engine_ActorSetAnimation(12, 4);
        FieldScene_RunStepThen10(request_a);
        Engine_ActorSetAnimationAndWait(14, 4);
        FieldScene_RunStepThen10(request_b);
        FieldScene_CallPairWith10(14, 0x8000);
        Engine_ActorStartRepeatedMotion(14, 2);
        Event_ShowMessageAndWait(request_b, 0, 20);
        Actor_FaceDirection(12, FACING_EAST, 0);
        Actor_ShowEmote(12, 0x102, 80);
        Event_ShowMessageAndWait(request_a, 0, 20);
        Actor_ShowEmote(14, 0x103, 0);
        Actor_ShowEmote(13, 0x103, 60);
        Engine_ActorStartRepeatedMotion(14, 2);
        FieldScene_RunStepThen10(request_b);
        FieldScene_CallPairWith10(14, 0xb000);
        Engine_ActorRunRepeatedMotion(14, 1);
        FieldScene_RunStepThen10(request_b);
        FieldScene_CallPairWith10(13, 0x3000);
        Actor_ShowEmote(13, 0x101, 0);
        Actor_ShowEmote(12, 0x101, 60);
        Engine_ActorRunRepeatedMotion(13, 1);
        FieldScene_RunStepThen10(13);
        Actor_ShowEmote(14, 0x103, 40);
        Engine_ActorRunRepeatedMotion(14, 1);
        FieldScene_RunStepThen10(request_b);
        Actor_FaceDirection(12, 0xd000, 0);
        Actor_FaceDirection(13, 0x5000, 40);
        Actor_FaceDirection(12, FACING_EAST, 0);
        FieldScene_CallPairWith10(13, 0x3000);
        Engine_ActorRunRepeatedMotion(12, 2);
        Event_ShowMessageAndWait(request_a, 0, 20);
        Actor_FaceDirection(14, FACING_SOUTH, 40);
        FieldScene_RunStepThen10(request_b);
        Engine_ActorStartRepeatedMotion(12, 2);
        Engine_ActorRunRepeatedMotion(13, 2);
        Engine_EventWait(60);
        Engine_ActorRunRepeatedMotion(13, 1);
        FieldScene_RunStepThen10(13);
        Engine_ActorSetAnimationAndWait(14, 3);
        FieldScene_RunStepThen10(request_b);
        Actor_ShowEmote(12, 0x102, 40);
        Engine_ActorRunRepeatedMotion(12, 2);
        FieldScene_RunStepThen10(request_a);
        Engine_ActorSetAnimationAndWait(13, 3);
        FieldScene_RunStepThen10(13);
        Actor_FaceDirection(14, 0xb000, 40);
        Engine_ActorSetAnimation(14, 3);
        Engine_ActorSetAnimationAndWait(13, 3);
        Actor_SetSpeed(14, 0x19999, 0xcccc);
        Actor_SetSpeed(13, 0x19999, 0xcccc);
        action = (s32)FuneHeya_ActionScriptA;
        Engine_ActorEnableActionCallback(14, action);
        Engine_ActorEnableActionCallback(13, action);
        Engine_EventWait(20);
        Actor_FaceDirection(12, FACING_SOUTH, 0);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x26666, 0x13333);
        ((struct FieldActor *)Object_GetById(0))->steering_flags &= 254;
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 184, 0x208);
        Engine_EventWait(1);
        {
            struct FieldActor *record = Object_GetById(0);
            record->steering_flags = ACTOR_FACES_MOTION | record->steering_flags;
        }
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
        Engine_ActorJump(12, 4, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
        Engine_ActorStartRepeatedMotion(12, 2);
        FieldScene_RunStepThen10(12);
        Actor_SetSpeed(12, 0x19999, 0xcccc);
        Engine_ActorEnableActionCallback(12, action);
        Engine_EventWait(40);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
        Object_RefreshSelectorById(12);
        GameFlag_Set(0x922);
        Engine_EventEnd();
    }
}

void FieldScene_RunExtendedActorChoreography(void)
{
    s32 request_a;
    s32 request_b;
    s32 request_c;

    Audio_PlayCue(28);
    Camera_SetSpeed(0x26666, 0x4ccc);
    ConfigureSceneMotionFlags(0x1c80000, -1, 0x2880000, 0x10000014);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_EventSetMessage((s32)MsgFuneYoureTryingLaunchShip);
    FieldScene_RunStepThen10(9);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    Actor_FaceDirection(10, 0xd000, 0);
    Actor_FaceDirection(11, FACING_EAST, 0);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_FaceDirection(13, FACING_WEST, 40);
    Actor_ShowEmote(9, 0x103, 40);
    Engine_ActorStartRepeatedMotion(9, 2);
    FieldScene_RunStepThen10(9);
    Actor_FaceDirection(12, FACING_EAST, 0);
    Actor_FaceDirection(11, 0xd000, 0);
    Actor_FaceDirection(13, 0xd000, 20);
    Engine_ActorRunRepeatedMotion(11, 1);
    FieldScene_RunStepThen10(0x100b);
    Actor_ShowEmote(13, 0x102, 20);
    Engine_ActorStartRepeatedMotion(13, 2);
    FieldScene_RunStepThen10(13);
    Actor_ShowEmote(9, 0x105, 60);
    FieldScene_RunStepThen10(9);
    Actor_ShowEmote(12, 0x104, 20);
    FieldScene_RunStepThen10(0x900c);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_ActorSetAnimation(8, 3);
    FieldScene_RunStepThen10(8);
    FieldScene_CallPairWith10(12, 0x3000);
    FieldScene_RunStepThen10(0x900c);
    FieldScene_CallPairWith10(11, 0xb000);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(13, 1);
    Engine_ActorSetAnimationAndWait(13, 3);
    FieldScene_RunStepThen10(13);
    Actor_FaceDirection(13, FACING_WEST, 0);
    Actor_FaceDirection(12, 0x5000, 0);
    FieldScene_CallPairWith10(11, 0x5000);
    Actor_SetSpeed(13, 0x6666, 0x3333);
    Actor_SetSpeed(12, 0xcccc, 0x6666);
    Actor_WalkTo(12, 0x1bc, 0x29c);
    Actor_WalkToAndWait(13, 0x1d8, 0x29c);
    Engine_ActorWaitForMove(12);
    Engine_ActorSetAnimation(12, 1);
    Engine_EventWait(80);
    FieldScene_CallPairWith10(12, 0xd000);
    Actor_ShowEmote(12, 0x101, 60);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x400b, 0, 40);
    Engine_ActorRunRepeatedMotion(11, 2);
    Actor_FaceDirection(11, 0xd000, 0);
    FieldScene_RunStepThen10(0x100b);
    Actor_FaceDirection(12, 0xd000, 0);
    Actor_ShowEmote(9, 0x101, 60);
    Engine_ActorSetAnimation(11, 4);
    Engine_EventWait(20);
    FieldScene_RunStepThen10(0x100b);
    Engine_ActorSetAnimation(9, 3);
    FieldScene_RunStepThen10(9);
    Actor_FaceDirection(13, 0xd000, 0);
    Actor_SetAttachedEffect(13, 0x102);
    Engine_ActorJump(13, 2, 20);
    FieldScene_RunStepThen10(13);
    Engine_ActorSetAnimationAndWait(9, 3);
    request_a = 0x100c;
    FieldScene_RunStepThen10(9);
    FieldScene_CallPairWith10(11, 0xd000);
    Engine_ActorRunRepeatedMotion(12, 1);
    FieldScene_RunStepThen10(request_a);
    Actor_ShowEmote(8, 0x105, 40);
    Engine_ActorSetAnimation(8, 3);
    FieldScene_RunStepThen10(8);
    Actor_ShowEmote(13, 0x102, 40);
    Engine_ActorJump(13, 4, 0);
    FieldScene_RunStepThen10(13);
    Engine_ActorSetAnimation(9, 3);
    FieldScene_RunStepThen10(9);
    Engine_ActorRunRepeatedMotion(11, 1);
    FieldScene_RunStepThen10(0x100b);
    Actor_ShowEmote(8, 0x102, 40);
    Event_ShowMessage(8, 0);
    Engine_ActorStartRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(0x100b, 0, 40);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_FaceDirection(9, 0x5000, 20);
    Engine_ActorStartRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(9, 0, 20);
    Engine_ActorSetAnimation(11, 3);
    Engine_EventWait(20);
    Actor_ShowEmote(12, 0x100, 40);
    Engine_ActorStartRepeatedMotion(12, 2);
    request_b = 0x400b;
    FieldScene_RunStepThen10(request_a);
    Actor_FaceDirection(11, 0x5000, 20);
    FieldScene_RunStepThen10(request_b);
    Engine_ActorStartRepeatedMotion(12, 2);
    FieldScene_RunStepThen10(request_a);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_ActorRunRepeatedMotion(11, 1);
    FieldScene_RunStepThen10(request_b);
    Actor_ShowEmote(12, 0x102, 60);
    Engine_ActorRunRepeatedMotion(9, 1);
    FieldScene_RunStepThen10(9);
    Actor_ShowEmote(11, 0x101, 40);
    Actor_FaceDirection(11, 0xd000, 20);
    Engine_ActorSetAnimation(9, 3);
    FieldScene_RunStepThen10(9);
    Actor_ShowEmote(11, 0x103, 20);
    Engine_ActorStartRepeatedMotion(11, 2);
    FieldScene_RunStepThen10(0x100b);
    Actor_ShowEmote(9, 0x108, 40);
    FieldScene_RunStepThen10(9);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_ActorSetAnimation(8, 3);
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(9, 0xd000, 40);
    FieldScene_RunStepThen10(9);
    Engine_ActorRunRepeatedMotion(12, 1);
    Engine_EventWait(20);
    FieldScene_RunStepThen10(request_a);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_FaceDirection(9, 0x5000, 0);
    Actor_FaceDirection(13, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(10, 0xb000, 40);
    Engine_ActorRunRepeatedMotion(11, 1);
    FieldScene_RunStepThen10(request_b);
    Engine_ActorSetAnimation(12, 3);
    Event_ShowMessageAndWait(request_a, 0, 20);
    Engine_ActorRunRepeatedMotion(9, 2);
    FieldScene_RunStepThen10(9);
    Actor_ShowEmote(12, 0x108, 40);
    Engine_ActorSetAnimation(12, 3);
    FieldScene_RunStepThen10(request_a);
    Engine_ActorSetAnimationAndWait(8, 3);
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(8, FACING_WEST, 20);
    Audio_PlayCue(19);
    Engine_ActorStartRepeatedMotion(8, 2);
    request_c = 0x8008;
    Actor_ShowEmote(8, 0x100, 80);
    FieldScene_RunStepThen10(request_c);
    Actor_ShowEmote(12, 0x101, 0);
    Actor_ShowEmote(11, 0x101, 0);
    Actor_ShowEmote(13, 0x101, 0);
    Actor_ShowEmote(10, 0x101, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 40);
    Actor_FaceDirection(12, 0xd000, 0);
    Actor_FaceDirection(11, 0xd000, 0);
    Actor_FaceDirection(13, 0xb000, 0);
    Actor_FaceDirection(10, 0xb000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
    Actor_SetAttachedEffect(8, 0x102);
    Engine_ActorJump(8, 4, 40);
    Engine_ActorStartRepeatedMotion(8, 2);
    Event_ShowMessage(request_c, 0);
    Actor_SetSpeed(8, 0x19999, 0xcccc);
    Actor_WalkToAndWait(8, 0x1db, 0x256);
    Actor_FaceDirection(8, FACING_WEST, 0);
    Actor_SetSpeed(9, 0x10000, 0x8000);
    Actor_WalkToAndWait(9, 0x1ce, 0x26a);
    FieldScene_CallPairWith10(9, 0xb000);
    Actor_ShowEmote(9, 0x100, 40);
    Engine_ActorStartRepeatedMotion(9, 2);
    FieldScene_RunStepThen10(0x8009);
    Actor_ShowEmote(11, 0x101, 60);
    FieldScene_RunStepThen10(11);
    Actor_ShowEmote(12, 0x102, 20);
    FieldScene_RunStepThen10(request_a);
    Actor_ShowEmote(8, 0x103, 20);
    Engine_ActorJump(8, 4, 0);
    Actor_FaceDirection(8, 0x5000, 20);
    FieldScene_RunStepThen10(8);
    Audio_PlayCue(28);
    Engine_ActorStartRepeatedMotion(8, 3);
    FieldScene_RunStepThen10(8);
    Actor_ShowEmote(13, 0x101, 60);
    FieldScene_RunStepThen10(13);
    FieldScene_CallPairWith10(8, 0x3000);
    Engine_ActorSetAnimationAndWait(8, 4);
    FieldScene_RunStepThen10(8);
    Actor_WalkToAndWait(12, 0x1bc, 0x274);
    FieldScene_CallPairWith10(12, 0xd000);
    FieldScene_RunStepThen10(0x900c);
    Actor_FaceDirection(8, 0x5000, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    FieldScene_RunStepThen10(8);
    Actor_ShowEmote(11, 0x102, 60);
    FieldScene_RunStepThen10(0x100b);
    Actor_ShowEmote(13, 0x107, 40);
    Engine_ActorStartRepeatedMotion(13, 2);
    FieldScene_RunStepThen10(13);
    FieldScene_CallPairWith10(9, 0x3000);
    Engine_ActorSetAnimationAndWait(9, 4);
    FieldScene_RunStepThen10(0x1009);
    FieldScene_CallPairWith10(12, 0);
    Engine_ActorRunRepeatedMotion(8, 1);
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(11, FACING_EAST, 0);
    Actor_ShowEmote(12, 0x105, 0);
    Actor_ShowEmote(9, 0x105, 60);
    Camera_SetSpeed(0x13333, 0x2666);
    ConfigureSceneMotionFlags(0x1d00000, -1, 0x2a80000, 0x10000000);
    Engine_ActorRunRepeatedMotion(10, 1);
    FieldScene_CallPairWith10(10, 0);
    FieldScene_RunStepThen10(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_ShowEmote(10, 0x102, 40);
    FieldScene_RunStepThen10(10);
    Actor_FaceDirection(10, FACING_WEST, 20);
    Actor_ShowEmote(10, 0x100, 0);
    Engine_ActorJump(10, 4, 40);
    FieldScene_RunStepThen10(10);
    Engine_ActorRunRepeatedMotion(10, 1);
    FieldScene_RunStepThen10(10);
    Actor_SetSpeed(13, 0x10000, 0x8000);
    Actor_WalkTo(13, 0x1b6, 0x293);
    Actor_FaceDirection(8, FACING_WEST, 0);
    Actor_FaceDirection(9, 0xb000, 0);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_FaceDirection(11, 0xb000, 0);
    Audio_PlayCue(17);
    Actor_SetSpeed(10, 0x10000, 0x8000);
    Actor_WalkToAndWait(10, 0x1e8, 0x2ae);
    Actor_FaceDirection(10, 0xb000, 0);
    Engine_ActorWaitForMove(13);
    Engine_ActorSetAnimation(13, 1);
    Actor_FaceDirection(13, 0xd000, 0);
    Audio_PlayCueFromEventWork();
    GameFlag_Set(0x921);
}

void FieldScene_RunBranchingActorPresentation(void)
{
    struct FieldActor *record;
    s32 request_a;
    s32 request_b;
    s32 value;
    s32 request_c;
    s32 action;

    Engine_EventBegin();
    Battle_ResetEffectCounter();
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Actor_ShowEmote(8, 0x100, 40);
    request_a = 0x1008;
    Engine_ActorStartRepeatedMotion(8, 3);
    Engine_EventSetMessage((s32)MsgFuneItsMyLuckyAnchor);
    FieldScene_RunStepThen10(request_a);
    Engine_ActorStartRepeatedMotion(9, 1);
    Engine_ActorStartRepeatedMotion(12, 1);
    Engine_ActorStartRepeatedMotion(11, 1);
    Engine_ActorStartRepeatedMotion(13, 1);
    Engine_ActorRunRepeatedMotion(10, 1);
    Actor_FaceDirection(9, 0xd000, 0);
    Actor_FaceDirection(12, 0xd000, 0);
    Actor_FaceDirection(11, 0xd000, 0);
    Actor_FaceDirection(13, 0xd000, 0);
    Actor_FaceDirection(10, 0xb000, 20);
    Engine_ActorRunRepeatedMotion(8, 1);
    Event_OpenMessage(request_a, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorRunRepeatedMotion(9, 2);
        FieldScene_RunStepThen10(0x9009);
        Actor_ShowEmote(8, 0x108, 40);
        FieldScene_RunStepThen10(request_a);
        gEventWork->message += 2;
    } else {
        gEventWork->message += 2;
        Engine_ActorRunRepeatedMotion(9, 1);
        FieldScene_RunStepThen10(0x9009);
        Engine_ActorStartRepeatedMotion(8, 2);
        FieldScene_RunStepThen10(0x9008);
    }
    Actor_ShowEmote(13, 0x105, 40);
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x1d80000, -1, 0x27c0000, 1);
    Actor_SetSpeed(13, 0x10000, 0x8000);
    Actor_WalkToAndWait(13, 0x1d8, 0x296);
    FieldScene_CallPairWith10(13, 0xb000);
    FieldScene_RunStepThen10(13);
    FieldScene_CallPairWith10(8, 0x5000);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_ActorSetAnimationAndWait(9, 3);
    Actor_FaceDirection(11, FACING_EAST, 0);
    Actor_FaceDirection(13, FACING_WEST, 20);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimationAndWait(13, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(12, 1);
    FieldScene_CallPairWith10(12, 0x3000);
    Event_ShowMessageAndWait(0x100c, 0, 20);
    Actor_FaceDirection(11, 0xb000, 20);
    Actor_ShowEmote(11, 0x101, 40);
    FieldScene_RunStepThen10(11);
    request_b = 0x900c;
    FieldScene_CallPairWith10(12, 0xd000);
    Engine_ActorSetAnimation(12, 4);
    FieldScene_RunStepThen10(request_b);
    FieldScene_CallPairWith10(13, 0xb000);
    Engine_ActorRunRepeatedMotion(13, 1);
    FieldScene_RunStepThen10(13);
    Actor_ShowEmote(9, 0x100, 20);
    FieldScene_CallPairWith10(9, 0x3000);
    Engine_ActorRunRepeatedMotion(9, 1);
    FieldScene_RunStepThen10(9);
    Engine_ActorSetAnimationAndWait(12, 3);
    FieldScene_RunStepThen10(request_b);
    Engine_ActorRunRepeatedMotion(8, 2);
    FieldScene_RunStepThen10(8);
    FieldScene_CallPairWith10(12, 0xd000);
    Engine_ActorSetAnimationAndWait(12, 3);
    FieldScene_RunStepThen10(request_b);
    Engine_ActorRunRepeatedMotion(11, 2);
    FieldScene_CallPairWith10(11, 0xb000);
    FieldScene_RunStepThen10(11);
    FieldScene_CallPairWith10(12, 0);
    FieldScene_RunStepThen10(request_b);
    Actor_FaceDirection(8, 0x3000, 0);
    Actor_FaceDirection(9, FACING_EAST, 0);
    Actor_FaceDirection(11, 0xd000, 0);
    Actor_FaceDirection(13, 0xd000, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1e6, 0x260);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(GERALD, record->x.fixed, record->z.fixed);
    }
    Actor_SetSpeed(GERALD, 0x10000, 0x8000);
    Actor_WalkToAndWait(GERALD, 0x1e6, 0x270);
    Actor_FaceDirection(GERALD, FACING_WEST, 0);
    record = Object_GetById(1);
    if (record != 0) {
        Actor_SetPosition(IWAN, record->x.fixed, record->z.fixed);
    }
    Actor_SetSpeed(IWAN, 0x10000, 0x8000);
    Actor_WalkToAndWait(IWAN, 0x1e6, 0x280);
    Actor_FaceDirection(IWAN, FACING_WEST, 0);
    record = Object_GetById(2);
    if (record != 0) {
        Actor_SetPosition(MEARI, record->x.fixed, record->z.fixed);
    }
    Actor_SetSpeed(MEARI, 0x10000, 0x8000);
    Actor_WalkToAndWait(MEARI, 0x1e6, 0x290);
    Actor_FaceDirection(MEARI, FACING_WEST, 20);
    Actor_ShowEmote(12, 0x108, 40);
    FieldScene_RunStepThen10(request_b);
    Engine_ActorRunRepeatedMotion(9, 1);
    FieldScene_RunStepThen10(0x1009);
    Engine_ActorSetAnimationAndWait(8, 3);
    FieldScene_CallPairWith10(8, 0x5000);
    FieldScene_RunStepThen10(8);
    FieldScene_CallPairWith10(8, 0x3000);
    Event_OpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        Engine_ActorStartRepeatedMotion(8, 2);
        FieldScene_RunStepThen10(8);
        Engine_ActorSetAnimationAndWait(12, 3);
        FieldScene_RunStepThen10(request_b);
        Engine_ActorStartRepeatedMotion(9, 1);
        Event_ShowMessageAndWait(0x9009, 0, 40);
        gEventWork->message += 1;
    } else {
        gEventWork->message += 3;
        Engine_ActorStartRepeatedMotion(8, 3);
        Event_ShowMessageAndWait(8, 0, 40);
    }
    Engine_ActorRunRepeatedMotion(13, 1);
    FieldScene_RunStepThen10(13);
    Engine_ActorRunRepeatedMotion(8, 1);
    value = 176;
    FieldScene_CallPairWith10(8, 0x5000);
    FieldScene_RunStepThen10(8);
    Engine_ActorRunRepeatedMotion(13, 1);
    FieldScene_CallPairWith10(13, (value << 8));
    Event_ShowMessageAndWait(13, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    request_c = 0x4008;
    Actor_WalkToAndWait(8, 0x1d8, 0x278);
    FieldScene_RunStepThen10(request_c);
    Actor_ShowEmote(13, 0x103, 40);
    Engine_ActorStartRepeatedMotion(13, 2);
    FieldScene_RunStepThen10(13);
    Engine_ActorSetAnimation(8, 4);
    Event_ShowMessageAndWait(request_c, 0, 40);
    Engine_ActorRunRepeatedMotion(11, 1);
    FieldScene_CallPairWith10(11, (value << 8));
    FieldScene_RunStepThen10(0x100b);
    Actor_ShowEmote(10, 0x102, 20);
    Actor_SetSpeed(10, 0x26666, 0x13333);
    Engine_ActorJump(10, 2, 0);
    Actor_WalkToAndWait(10, 0x1ce, 0x2a2);
    FieldScene_CallPairWith10(10, (value << 8));
    Engine_ActorStartRepeatedMotion(10, 2);
    FieldScene_RunStepThen10(10);
    FieldScene_CallPairWith10(9, 0x5000);
    Engine_ActorSetAnimationAndWait(9, 4);
    FieldScene_RunStepThen10(9);
    Engine_ActorSetAnimationAndWait(8, 3);
    FieldScene_RunStepThen10(request_c);
    Actor_ShowEmote(13, 0x102, 40);
    Event_ShowMessageAndWait(13, 0, 40);
    FieldScene_CallPairWith10(9, 0x3000);
    Engine_ActorStartRepeatedMotion(9, 2);
    FieldScene_RunStepThen10(0x1009);
    FieldScene_CallPairWith10(12, 0);
    Actor_FaceDirection(8, FACING_WEST, 0);
    Actor_FaceDirection(9, 0x5000, 0);
    Actor_FaceDirection(11, (value << 8), 0);
    Actor_FaceDirection(13, (value << 8), 0);
    Actor_FaceDirection(10, (value << 8), 20);
    Engine_ActorRunRepeatedMotion(12, 1);
    Event_ShowMessageAndWait(0x100c, 0, 20);
    Actor_ShowEmote(8, 0x101, 40);
    FieldScene_CallPairWith10(8, 0xd000);
    Event_OpenMessage(0x1008, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(8, 3);
        FieldScene_RunStepThen10(0x1008);
        gEventWork->message += 1;
    } else {
        gEventWork->message += 1;
        FieldScene_RunStepThen10(0x1008);
    }
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(8, 3);
    FieldScene_RunStepThen10(0x1008);
    FieldScene_CallPairWith10(8, 0x8000);
    FieldScene_RunStepThen10(0x4008);
    FieldScene_RunSceneStep(2, 0, 0);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_ActorRunRepeatedMotion(13, 2);
    Engine_EventWait(20);
    action = (s32)FuneHeya_ActionScriptB;
    Engine_ActorEnableActionCallback(10, action);
    Engine_EventWait(4);
    Engine_ActorEnableActionCallback(11, action);
    Engine_EventWait(4);
    Engine_ActorEnableActionCallback(12, action);
    Engine_EventWait(4);
    Engine_ActorEnableActionCallback(9, action);
    Engine_ActorSetAnimation(MEARI, 2);
    record = Object_GetById(2);
    if (record != 0) {
        Actor_SetDestination(MEARI, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(MEARI);
    Actor_SetPosition(MEARI, 0, 0);
    Engine_ActorSetAnimation(IWAN, 2);
    record = Object_GetById(1);
    if (record != 0) {
        Actor_SetDestination(IWAN, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(IWAN);
    Actor_SetPosition(IWAN, 0, 0);
    Engine_ActorSetAnimation(GERALD, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(GERALD, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(GERALD);
    Actor_SetPosition(GERALD, 0, 0);
    Engine_ActorEnableActionCallback(13, action);
    Actor_WalkToAndWait(8, 0x1c8, 0x288);
    FieldScene_CallPairWith10(8, 0);
    PartyInventory_Discard(232);
    GameFlag_Set(0x925);
    Engine_EventEnd();
}

s32 FuneHeya_ApplyEntryState(void)
{
    Engine_TaskWait(1);
    Engine_GameFlagSet(0x144);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    switch (((union GameStateRows *)&gGameState)->halves[225][0]) {
    case 1:
    case 2:
    case 11:
        if (Engine_GameFlagIsSet(0x93e) == 0 && Engine_GameFlagIsSet(0x928) != 0) {
            Engine_ActorSetChildValue(9, 2);
        } else if (Engine_GameFlagIsSet(0x911) != 0) {
            Engine_ActorSetChildValue(12, 2);
        }
        break;
    case 4:
    case 12:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 23:
    case 24:
        Engine_ActorSetChildValue(19, 2);
        break;
    case 5:
        if (Engine_GameFlagIsSet(0x93e) == 0 && Engine_GameFlagIsSet(0x911) != 0) {
            Engine_ActorSetChildValue(13, 2);
        }
        break;
    }
    switch (((union GameStateRows *)&gGameState)->halves[225][0]) {
    case 1:
    case 2:
        SceneState_RunFlagGatedSetupCascade();
        break;
    case 4:
        RunSceneSelectionChain();
        break;
    case 5:
        FuneHeya_ApplyFlaggedLayout();
        break;
    case 10:
        if (Engine_GameFlagIsSet(0x928) != 0) {
            FuneHeya_AnnounceDeparture();
        } else {
            FuneHeya_CastOff();
        }
        break;
    case 11:
        if (Engine_GameFlagIsSet(0x928) != 0) {
            FuneHeya_CallActorEighteenToRow();
        } else {
            FuneHeya_CallActorSixteenToRow();
        }
        break;
    case 12:
        FieldScene_RunFlagBranchedSetupCascade();
        break;
    case 13:
        FuneHeya_AskAboutMonsters();
        break;
    case 14:
        FuneHeya_AnnounceTurn();
        break;
    case 15:
        if (Engine_GameFlagIsSet(0x109) != 0) {
            Engine_EventBegin();
            FieldScene_RunSceneStep(25, 1, 0);
            FieldScene_RunSceneStep(22, 0, 0);
            Engine_ActorEnableActionCallback(36, FuneHeya_EntryActionScript);
            Engine_ActorEnableActionCallback(37, FuneHeya_EntryActionScript);
            Engine_ActorEnableActionCallback(38, FuneHeya_EntryActionScript);
            Engine_ActorSetChildValue(36, 3);
            Engine_ActorSetChildValue(37, 3);
            Engine_ActorSetChildValue(38, 3);
            Engine_EventEnd();
        } else {
            FuneHeya_SetupFirstRowingPair();
        }
        break;
    case 16:
        FieldScene_RunActors24And25Setup();
        break;
    case 17:
        if (Engine_GameFlagIsSet(0x109) != 0) {
            Engine_EventBegin();
            FieldScene_RunSceneStep(25, 2, 0);
            FieldScene_RunSceneStep(22, 0, 0);
            Engine_ActorEnableActionCallback(36, FuneHeya_EntryActionScript);
            Engine_ActorEnableActionCallback(37, FuneHeya_EntryActionScript);
            Engine_EventEnd();
        } else {
            FuneHeya_SetupReplacementRower();
        }
        break;
    case 18:
        FieldScene_RunActors24And25SetupWithValue929();
        break;
    case 19:
        if (Engine_GameFlagIsSet(0x109) != 0) {
            Engine_EventBegin();
            FieldScene_RunSceneStep(25, 3, 0);
            FieldScene_RunSceneStep(22, 0, 0);
            Engine_ActorEnableActionCallback(36, FuneHeya_EntryActionScript);
            Engine_ActorEnableActionCallback(37, FuneHeya_EntryActionScript);
            Engine_ActorSetChildValue(36, 3);
            Engine_ActorSetChildValue(37, 3);
            Engine_EventEnd();
        } else {
            FuneHeya_SetupReplacementPair();
        }
        break;
    case 20:
        FieldScene_RunActors24And25SetupWithValue92a();
        break;
    case 21:
        if (Engine_GameFlagIsSet(0x109) != 0) {
            if (Engine_GameFlagIsSet(0x302) != 0) {
                {
                    u16 zero = 0;

                    FuneHeya_CueTimer = zero;
                }
                Engine_TaskAddCallback(Scene_UpdateCueTimer, 0xc80);
                Engine_ActorSetAnimation(9, 5);
            }
        } else {
            FieldScene_RunExtendedFormationPresentation();
        }
        break;
    case 22:
        RunActorsEightAndNineMapEvent();
        break;
    case 23:
        if (Engine_GameFlagIsSet(0x109) == 0) {
            Scene_RunFourActorProgressPresentation();
        }
        break;
    case 24:
        FuneHeya_DismissReplacementRowers();
        break;
    case 30:
        FieldScene_RunSceneStep(20, 0x926, 0x92b);
        FieldScene_RunSceneStep(21, 0, 0);
        Engine_GameFlagSet(0x902);
        Engine_EventRequestExit(1);
        break;
    }
    return 0;
}

void FuneHeya_ApplyFlaggedLayout(void)
{
    struct FieldActor *actor;
    struct FieldActor *other;
    s32 done;
    s32 x;
    s32 heading;
    struct Half zero;

    if (!Engine_GameFlagIsSet(0x911)) {
        FuneHeya_PlaceAnchorCharm();
        return;
    }
    if (GameFlag_IsSet(0x928)) {
        FuneHeya_PlaceAnchorCharm();
    }
    done = Engine_GameFlagIsSet(0x93e);
    if (done != 0) {
        return;
    }
    if (Engine_GameFlagIsSet(0x8a0)) {
        actor = Object_GetById(9);
        FieldScene_RunSceneStep(13, 0, 0);
        OverlayObject_SetPositionAndHeading(8, 0x1c8, 0x28c, 0);
        x = 0x1e0;
        OverlayObject_SetPositionAndHeading(9, x, 0x258, 0xb000);
        Actor_SetSpeed(9, 0xcccc, 0x6666);
        actor->work = done;
        zero.v = 0;
        actor->rise_enabled = zero.v;
        actor->collision_flags |= 128;
        actor->update = (void (*)(union FieldObject *))&FuneHeya_RunWalkerStep;
        other = Object_GetById(8);
        other->rise_counter = zero.v;
        other->update = (void (*)(union FieldObject *))&UpdateActorNineEffectMode;
        if (Engine_GameFlagIsSet(0x109)) {
            OverlayObject_SetPositionAndHeading(0, x, 0x29a, 0xa000);
        }
    } else if (Engine_GameFlagIsSet(0x928)) {
        OverlayObject_SetPositionAndHeading(8, 0x1bc, 0x266, 0xd000);
        FieldScene_RunSceneStep(13, 0, 0);
    } else if (Engine_GameFlagIsSet(0x925)) {
        OverlayObject_SetPositionAndHeading(8, 0x1c8, 0x288, 0);
        FieldScene_RunSceneStep(13, 0, 0);
    } else if (Engine_GameFlagIsSet(0x921)) {
        heading = 0xb000;
        OverlayObject_SetPositionAndHeading(8, 0x1db, 0x256, 0x8000);
        OverlayObject_SetPositionAndHeading(9, 0x1ce, 0x26a, heading);
        Object_GetById(12)->facing = 0x3000;
        Object_GetById(11)->facing = heading;
        OverlayObject_SetPositionAndHeading(13, 0x1b6, 0x293, 0xd000);
        OverlayObject_SetPositionAndHeading(10, 0x1e8, 0x2b0, heading);
    }
}

void SceneState_RunFlagGatedSetupCascade(void)
{
    if (Engine_GameFlagIsSet(0x93e) != 0) {
        Engine_ActorSetPosition(8, 0, 0);
        Engine_ActorSetPosition(9, 0, 0);
        Engine_ActorSetPosition(10, 0, 0);
        Engine_ActorSetPosition(11, 0, 0);
        Engine_ActorSetPosition(12, 0, 0);
        FieldScene_RunSceneStep(14, 0, 0);
        return;
    }

    if (Engine_GameFlagIsSet(0x8a0) != 0) {
        OverlayObject_SetPositionAndHeading(8, 0x98, 0x1bc, 0x3000);
        Engine_ActorEnableActionCallback(8, FuneHeya_ActionScriptF);
        OverlayObject_SetPositionAndHeading(10, 0xb8, 0x1e0, 0xb000);
        OverlayObject_SetPositionAndHeading(12, 0xaa, 0x1e8, 0xb000);
        OverlayObject_SetPositionAndHeading(13, 0x88, 0x1e8, 0xd000);
        OverlayObject_SetPositionAndHeading(15, 0x78, 0x1e0, 0xd000);
        OverlayObject_SetPositionAndHeading(14, 0xb8, 0x20e, 0xb000);
        OverlayObject_SetPositionAndHeading(11, 0x88, 0x248, 0x8000);
        Engine_ActorEnableActionCallback(11, FuneHeya_ActionScriptE);
        return;
    }

    {
        s32 t = Engine_GameFlagIsSet(0x928);
        if (t != 0) {
            SceneState_ApplyActor8FourFlags(t);
            return;
        }
    }

    if (Engine_GameFlagIsSet(0x925) != 0) {
        FieldScene_RunSceneStep(18, 0, 0);
        return;
    }

    if (Engine_GameFlagIsSet(0x911) != 0 &&
        Engine_GameFlagIsSet(0x922) != 0) {
        FieldScene_RunSceneStep(14, 0, 0);
        Engine_ActorSetPosition(12, 0, 0);
    }
}

void RunSceneSelectionChain(void)
{
    Engine_TaskWait(1);
    SceneActor_SetFlagBit3ForActors28To35();
    if (GameFlag_IsSet(2366) != 0) {
        FieldScene_RunSceneStep(4, 4, 0);
        OverlayObject_SetPositionAndHeading(8, 412, 222, 12288);
        OverlayObject_SetPositionAndHeading(9, 458, 161, 32768);
    } else {
        if (GameFlag_IsSet(2208) != 0) {
            Actor_SetPosition(8, 30932992, 9961472);
            Engine_ActorSetAnimation(9, 5);
            FieldScene_RunSceneStep(4, 4, 0);
        } else {
            if (GameFlag_IsSet(2347) != 0) {
                FieldScene_RunSceneStep(16, 0, 0);
                FieldScene_RunSceneStep(4, 4, 0);
                Scene_RunConditionalActorPresentation(3);
            } else {
                if (GameFlag_IsSet(2346) != 0) {
                    FieldScene_RunSceneStep(16, 0, 0);
                    FieldScene_RunSceneStep(4, 3, 0);
                    Scene_RunConditionalActorPresentation(2);
                } else {
                    if (GameFlag_IsSet(2345) != 0) {
                        FieldScene_RunSceneStep(16, 0, 0);
                        FieldScene_RunSceneStep(4, 2, 0);
                        Scene_RunConditionalActorPresentation(1);
                    } else {
                        if (GameFlag_IsSet(2344) != 0) {
                            FieldScene_RunSceneStep(16, 0, 0);
                            Actor_SetPosition(10, 0, 0);
                            Scene_RunConditionalActorPresentation(0);
                        } else {
                            Engine_ActorSetAnimation(9, 5);
                            if (GameFlag_IsSet(2341) != 0 && GameFlag_IsSet(2342) == 0) {
                                FieldScene_RunFourActorCoordinatePresentation();
                            }
                        }
                    }
                }
            }
        }
    }
}

void SceneActor_SetFlagBit3ForActors28To35(void)
{
    u32 i;
    u32 bit;
    u32 zero;

    i = 28;
    bit = 8;
    zero = 0;
    for (; i <= 35; i++) {
        struct FieldActor *obj = Object_GetById(i);
        u32 v = obj->collision_flags;
        obj->collision_flags = (u8)((v | bit) | zero);
    }
}

void FuneHeya_PlaceFoundActors(s32 gated)
{
    s32 actor;

    if (gated && !Engine_GameFlagIsSet(0x929)) {
        return;
    }
    if ((actor = FuneHeya_FindFirstSetFlag(0, 0)) != 0) {
        OverlayObject_SetPositionAndHeading(actor, 410, 172, 0xd000);
        Engine_ActorSetPosition(10, 0, 0);
    }
    if (gated && !Engine_GameFlagIsSet(0x92a)) {
        return;
    }
    if ((actor = FuneHeya_FindFirstSetFlag(1, 0)) != 0) {
        OverlayObject_SetPositionAndHeading(actor, 470, 172, 0xb000);
        Engine_ActorSetPosition(11, 0, 0);
    }
    if (gated && !Engine_GameFlagIsSet(0x92b)) {
        return;
    }
    if ((actor = FuneHeya_FindFirstSetFlag(2, 0)) != 0) {
        OverlayObject_SetPositionAndHeading(actor, 410, 204, 0xd000);
        Engine_ActorSetPosition(12, 0, 0);
    }
    if (gated) {
        return;
    }
    if ((actor = FuneHeya_FindFirstSetFlag(3, 0)) != 0) {
        OverlayObject_SetPositionAndHeading(actor, 470, 204, 0xb000);
        Engine_ActorSetPosition(13, 0, 0);
    }
}

void FieldScene_RunFourActorCoordinatePresentation(void)
{
    struct FieldActor *record;
    s32 mode;

    Engine_EventBegin();
    FieldScene_RunSceneStep(25, 0, 0);
    FieldScene_RunSceneStep(24, 1, 0);
    ConfigureSceneMotionFlags(0x1b80000, -1, 0xa80000, 0x1000001);
    OverlayObject_SetPositionAndHeading(27, 0x1b8, 164, 0x5000);
    OverlayObject_SetPositionAndHeading(8, 0x1ac, 190, 0xd000);
    OverlayObject_SetPositionAndHeading(9, 0x1c4, 190, 0xb000);
    Engine_ActorSetAnimation(9, 1);
    mode = 128;
    OverlayObject_SetPositionAndHeading(0, 0x1b8, 134, 0x8000);
    gEventWork->start_transition = (mode << 1);
    Engine_EventOpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x198, 134);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x198, 148);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1a8, 148);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
    Engine_ActorRunRepeatedMotion(27, 1);
    Engine_EventSetMessage((s32)MsgFuneOurReplacementNeverArrivedBut);
    FieldScene_RunStepThen10(27);
    Engine_ActorRunRepeatedMotion(8, 1);
    FieldScene_RunStepThen10(8);
    Engine_ActorSetAnimationAndWait(27, 3);
    FieldScene_RunStepThen10(27);
    FieldScene_CallPairWith10(27, 0xd000);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(GERALD, record->x.fixed, record->z.fixed);
    }
    Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
    Actor_WalkToAndWait(GERALD, 0x1b8, 148);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    record = Object_GetById(1);
    if (record != 0) {
        Actor_SetPosition(IWAN, record->x.fixed, record->z.fixed);
    }
    Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(IWAN, 0x1c8, 148);
    Actor_FaceDirection(IWAN, FACING_SOUTH, 0);
    record = Object_GetById(2);
    if (record != 0) {
        Actor_SetPosition(MEARI, record->x.fixed, record->z.fixed);
    }
    Actor_SetSpeed(MEARI, 0xcccc, 0x6666);
    Actor_WalkToAndWait(MEARI, 0x1d8, 148);
    Actor_FaceDirection(MEARI, FACING_SOUTH, 20);
    FieldScene_RunSceneStep(0, 0, 60);
    FieldScene_RunSceneStep(1, 0x4000, 20);
    FieldScene_RunSceneStep(2, 1, 20);
    Actor_FaceDirection(27, 0x5000, 20);
    FieldScene_RunStepThen10(27);
    Engine_ActorStartRepeatedMotion(9, 1);
    Actor_ShowEmote(9, (mode << 1), 40);
    FieldScene_RunStepThen10(9);
    Engine_ActorStartRepeatedMotion(GERALD, 3);
    Actor_ShowEmote(GERALD, 0x103, 60);
    Engine_ActorSetAnimationAndWait(27, 3);
    FieldScene_RunStepThen10(27);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_ActorSetAnimation(10, 3);
    FieldScene_RunStepThen10(10);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimationAndWait(13, 3);
    FieldScene_RunSceneStep(0, 0, 40);
    FieldScene_RunSceneStep(2, 1, 0);
    FieldScene_RunSceneStep(1, 0x4000, 20);
    Engine_ActorSetAnimationAndWait(27, 4);
    FieldScene_RunStepThen10(27);
    Actor_ShowEmote(8, 0x102, 60);
    Engine_ActorStartRepeatedMotion(8, 1);
    FieldScene_RunStepThen10(8);
    Engine_ActorSetAnimationAndWait(27, 3);
    FieldScene_RunStepThen10(27);
    Actor_FaceDirection(8, FACING_EAST, 0);
    Actor_FaceDirection(9, FACING_WEST, 40);
    Actor_ShowEmote(8, 0x102, 0);
    Actor_ShowEmote(8, 0x102, 40);
    Engine_ActorRunRepeatedMotion(27, 1);
    Engine_ActorSetAnimation(27, 3);
    Event_ShowMessageAndWait(27, 0, 20);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimationAndWait(9, 3);
    Engine_EventWait(40);
    Actor_ShowEmote(9, (mode << 1), 20);
    FieldScene_CallPairWith10(9, 0xb000);
    FieldScene_RunStepThen10(9);
    FieldScene_CallPairWith10(27, 0x3000);
    Actor_ShowEmote(27, 0x101, 60);
    Event_ShowMessageAndWait(27, 0, 60);
    Actor_ShowEmote(27, 0x106, 20);
    FieldScene_CallPairWith10(27, 0xb000);
    Engine_ActorSetAnimationAndWait(27, 3);
    FieldScene_RunStepThen10(27);
    FieldScene_RunSceneStep(3, 2, 80);
    FieldScene_CallPairWith10(8, 0xd000);
    Engine_ActorStartRepeatedMotion(8, 2);
    FieldScene_RunStepThen10(8);
    Engine_ActorSetAnimationAndWait(9, 3);
    Engine_ActorStartRepeatedMotion(9, 2);
    FieldScene_RunStepThen10(9);
    FieldScene_CallPairWith10(27, 0x5000);
    Engine_ActorSetAnimationAndWait(27, 3);
    Engine_ActorRunRepeatedMotion(27, 1);
    FieldScene_RunStepThen10(27);
    Actor_SetSpeed(27, 0xcccc, 0x6666);
    Actor_WalkToAndWait(27, 0x198, 158);
    Actor_WalkToAndWait(27, 0x198, 148);
    Actor_FaceDirection(27, FACING_EAST, 20);
    Engine_ActorRunRepeatedMotion(27, 1);
    FieldScene_RunStepThen10(27);
    FieldScene_RunSceneStep(1, 0x8000, 20);
    FieldScene_RunSceneStep(2, 1, 0);
    Actor_WalkToAndWait(27, 0x198, 134);
    Actor_WalkTo(27, 0x1b8, 134);
    Engine_EventWait(40);
    FieldScene_RunSceneStep(9, 10, 0);
    GameFlag_Set(0x926);
}

void FuneHeya_CastOff(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Engine_EventBegin();
    FieldScene_RunSceneStep(15, 0, 1);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_WalkToAndWait(8, 0x1d4, 0x266);
    Actor_WalkToAndWait(8, 0x1d8, 0x254);
    Actor_FaceDirection(8, FACING_WEST, 20);
    Engine_ActorJump(8, 4, 20);
    rec7 = (s32)FuneHeya_PlaceAnchorCharm();
    Engine_EventWait(20);
    Audio_PlayCue(214);
    Engine_ObjectSetScript(rec7, FuneHeya_Script01);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Actor_WalkToAndWait(8, 0x1d2, 0x270);
    FieldScene_CallPairWith10(8, 0x5000);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventSetMessage((s32)MsgFuneCastOff);
    Event_ShowMessageAndWait(8, 0, 20);
    FieldScene_RunSceneStep(9, 11, 0);
}

void FuneHeya_AnnounceDeparture(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    FieldScene_RunSceneStep(15, 1, 1);
    Actor_FaceDirection(8, 0x5000, 40);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventSetMessage((s32)MsgFuneWereOff);
    Event_ShowMessageAndWait(8, 0, 20);
    FieldScene_RunSceneStep(9, 11, 0);
}

void FuneHeya_CallActorSixteenToRow(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    FieldScene_RunSceneStep(24, 0, 0);
    FieldScene_RunSceneStep(18, 0, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(16, 0x960000, 0x24a0000);
    ConfigureSceneMotionFlags(0x9c0000, -1, 0x2180000, 0x1000001);
    FieldScene_RunSceneStep(8, 0, 0);
    Actor_SetSpeed(16, 0xcccc, 0x6666);
    Actor_WalkToAndWait(16, 168, 0x242);
    Actor_WalkToAndWait(16, 168, 0x22a);
    Actor_FaceDirection(16, FACING_WEST, 20);
    Engine_ActorStartRepeatedMotion(16, 2);
    Engine_EventSetMessage((s32)MsgFuneRowThoseOars);
    Event_ShowMessageAndWait(16, 0, 20);
    FieldScene_RunSceneStep(9, 12, 0);
}

void FuneHeya_CallActorEighteenToRow(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    SceneState_ApplyActor8FourFlags();
    Actor_SetPosition(18, 0x960000, 0x24a0000);
    ConfigureSceneMotionFlags(0x9c0000, -1, 0x2180000, 0x1000001);
    FieldScene_RunSceneStep(8, 0, 0);
    Actor_SetSpeed(18, 0xcccc, 0x6666);
    Actor_WalkToAndWait(18, 168, 0x242);
    Actor_WalkToAndWait(18, 168, 0x22a);
    Actor_FaceDirection(18, FACING_WEST, 20);
    Engine_ActorStartRepeatedMotion(18, 2);
    Engine_EventSetMessage((s32)MsgFuneRowThoseOars);
    Event_ShowMessageAndWait(18, 0, 20);
    FieldScene_RunSceneStep(9, 12, 0);
}

void FieldScene_RunFlagBranchedSetupCascade(void)
{
    Engine_EventBegin();
    Engine_ActorSetAnimation(9, 5);
    FieldScene_RunSceneStep(24, 1, 0);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
    FieldScene_RunSceneStep(17, 0, 0);
    FieldScene_InstallFlaggedActors10To17(0);
    FieldScene_RunSceneStep(8, 1, 20);
    Engine_CameraSetSpeed(0x6666, 0xccc);
    Engine_CameraMoveTo(0x1b80000, -1, 0xb00000, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(9, 7);
    Engine_EventWait(30);
    Engine_AudioPlayCue(0xbc);
    Engine_EventWait(30);
    FieldScene_InstallFlaggedActors10To17(16);
    Engine_EventWait(0x50);
    FieldScene_InstallFlaggedActors10To17(0);
    Engine_EventWait(0x3c);
    Engine_ActorSetAnimation(9, 7);
    Engine_EventWait(30);
    Engine_AudioPlayCue(0xbc);
    Engine_EventWait(30);
    FieldScene_InstallFlaggedActors10To17(16);
    Engine_EventWait(0x50);
    FieldScene_InstallFlaggedActors10To17(0);
    Engine_EventWait(0x5a);
    Engine_AudioPlayCue(0xbc);
    Engine_EventWait(30);

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);

    FieldScene_RunSceneStep(9, 0, 0);

    if (Engine_GameFlagIsSet(0x92b) != 0) {
        Engine_EventRequestExit(20);
    } else if (Engine_GameFlagIsSet(0x92a) != 0) {
        Engine_EventRequestExit(18);
    } else if (Engine_GameFlagIsSet(0x929) != 0) {
        Engine_EventRequestExit(17);
    } else if (Engine_GameFlagIsSet(0x928) != 0) {
        Engine_EventRequestExit(16);
    } else {
        Engine_EventRequestExit(13);
    }
}

void FuneHeya_AskAboutMonsters(void)
{
    Engine_EventBegin();
    FieldScene_RunSceneStep(15, 1, 1);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(10);
    Actor_FaceDirection(8, 0x3000, 20);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventSetMessage((s32)MsgFuneMonsters);
    Event_ShowMessageAndWait(8, 0, 20);
    FieldScene_RunSceneStep(9, 14, 0);
}

void FuneHeya_AnnounceTurn(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    FieldScene_RunSceneStep(15, 1, 1);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventSetMessage((s32)MsgFuneImTurning);
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(8, 0xd000, 40);
    FieldScene_RunSceneStep(9, 15, 0);
}

void FuneHeya_SetupFirstRowingPair(void)
{
    u32 i;
    s32 record;
    s32 base5_200e8e4;

    Engine_EventBegin();
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 1, 0);
    SceneActor_SetFlagBit3ForActors28To35();
    FieldScene_RunSceneStep(19, 11, 12);
    Engine_ActorSetAnimation(10, 6);
    Engine_ActorEnableActionCallback(12, FuneHeya_ActionScriptE);
    base5_200e8e4 = (s32)((u8 *)FuneHeya_EntryActionScript);
    Engine_ActorEnableActionCallback(36, base5_200e8e4);
    Engine_ActorEnableActionCallback(37, base5_200e8e4);
    Engine_ActorEnableActionCallback(38, base5_200e8e4);
    Engine_ActorSetChildValue(36, 3);
    Engine_ActorSetChildValue(37, 3);
    Engine_ActorSetChildValue(38, 3);
    FieldScene_RunPositionTransferPresentation();
    Engine_EventEnd();
}

void FieldScene_RunActors24And25Setup(void)
{
    Engine_EventBegin();
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 0, 0);
    FieldScene_RunSceneStep(19, 11, 12);
    FieldScene_RunFormationAndEffectPresentation();
    Engine_GameFlagSet(0x928);
    Engine_EventEnd();
}

void Scene_RunConditionalActorPresentation(s32 a0)
{
    u32 i;
    s32 rec7;
    struct FieldActor *record;
    s32 v6;
    s32 v7;
    s32 base5_a01b;
    s32 x;

    Engine_EventBegin();
    FieldScene_RunSceneStep(24, 0, 0);
    x = 0x1b0;
    v6 = 0x8000;
    OverlayObject_SetPositionAndHeading(0, x, 134, v6);
    FuneHeya_PlaceFoundActors(1);
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_WalkToAndWait(0, 0x196, 134);
    Actor_WalkToAndWait(0, 0x196, 152);
    Actor_WalkToAndWait(0, 0x1a5, 152);
    Engine_ActorRunRepeatedMotion(27, 1);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(27, 0, 10);
    if (Engine_GameFlagIsSet(0x300) == 0) {
    } else {
        rec7 = FuneHeya_FindFirstSetFlag(a0, 0);
        Engine_ActorRunRepeatedMotion(27, 1);
        Engine_EventWait(20);
        Engine_ActorFaceEachOther(27, 0, 10);
        Engine_EventSetMessage((s32)MsgFuneBackBroughtOarsman);
        FieldScene_RunStepThen10(CabinSpeakerRequest);
        Engine_ActorSetAnimationAndWait(0, 3);
        v7 = 0;
        Actor_SetSpeed(0, 0x10000, v6);
        Engine_ActorWalkToAndWait(0, x, 168);
        Engine_ActorFaceDirection(0, FACING_NORTH, 0);
        record = Object_GetById(0);
        if (record != 0) {
            Engine_ActorSetPosition(rec7, record->x.fixed, record->z.fixed);
        }
        Actor_SetSpeed(rec7, 0x10000, v6);
        Actor_WalkToAndWait(rec7, 0x1c0, 168);
        Actor_FaceDirection(rec7, 0xb000, 20);
        Actor_FaceDirection(27, 0x3000, 20);
        Engine_ActorSetAnimation(27, 3);
        FieldScene_RunStepThen10(27);
        Actor_ShowEmote(rec7, 0x102, 60);
        Engine_ActorRunRepeatedMotion(27, 1);
        Engine_ActorSetAnimation(27, 3);
        FieldScene_RunStepThen10(27);
        Engine_ActorSetAnimationAndWait(rec7, 3);
        if (Engine_GameFlagIsSet(0x92b) != 0) {
            Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
            Actor_FaceDirection(27, 0x3000, 0);
            Actor_WalkToAndWait(rec7, 0x1d6, 204);
            FieldScene_CallPairWith10(rec7, 0xb000);
        } else if (Engine_GameFlagIsSet(0x92a) != 0) {
            Actor_WalkToAndWait(0, 0x1a6, 154);
            Actor_FaceDirection(0, FACING_SOUTHWEST, 0);
            Actor_FaceDirection(27, 0x5000, 0);
            Actor_WalkToAndWait(rec7, 0x19a, 204);
            v7 = 1;
            FieldScene_CallPairWith10(rec7, 0xd000);
        } else if (Engine_GameFlagIsSet(0x929) != 0) {
            Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
            Actor_FaceDirection(27, 0x3000, 0);
            Actor_WalkToAndWait(rec7, 0x1d6, 172);
            FieldScene_CallPairWith10(rec7, 0xb000);
        } else {
            Actor_WalkToAndWait(0, 0x1a6, 154);
            Actor_FaceDirection(0, FACING_SOUTHWEST, 0);
            Actor_FaceDirection(27, 0x5000, 0);
            Actor_WalkToAndWait(rec7, 0x19a, 172);
            v7 = 1;
            FieldScene_CallPairWith10(rec7, 0xd000);
        }
        Engine_ActorFaceEachOther(27, 0, 20);
        FieldScene_RunStepThen10(0x201b);
        Engine_ActorSetAnimationAndWait(0, 3);
        Engine_ActorSetAnimationAndWait(27, 3);
        Actor_SetSpeed(27, 0x10000, 0x8000);
        if (v7 != 0) {
            Actor_WalkToAndWait(27, 0x1ac, 164);
            Actor_WalkToAndWait(27, 0x198, 164);
        }
        Actor_WalkToAndWait(27, 0x198, 134);
        Engine_ActorWalkTo(27, 0x1b8, 134);
        Engine_EventWait(40);
        FieldScene_RunSceneStep(9, 10, 0);
        goto L_02004592;
    }
    Engine_EventSetMessage((s32)MsgFuneBack);
    Event_ShowMessageAndWait(CabinSpeakerRequest, 0, 40);
    Engine_ActorShowEmote(27, 0x101, 60);
    /* FAKEMATCH: start the saved speaker at repeated dialogue; the initial call must retain its independent load from the same literal word. */
    base5_a01b = CabinSpeakerRequest;
    FieldScene_RunStepThen10(base5_a01b);
    Engine_ActorSetAttachedEffect(0, 0x102);
    Engine_EventWait(60);
    Actor_ShowEmote(27, 0x103, 40);
    Engine_ActorStartRepeatedMotion(27, 2);
    FieldScene_RunStepThen10(base5_a01b);
    Engine_ActorShowEmote(27, 0x105, 40);
    FieldScene_RunStepThen10(base5_a01b);
    Engine_ActorSetAnimationAndWait(27, 4);
    FieldScene_RunStepThen10(base5_a01b);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Engine_EventRequestExit(4);
    L_02004592:;
}

struct FieldActor *FuneHeya_PlaceAnchorCharm(void)
{
    s32 set = GameFlag_IsSet(0x200);
    struct FieldActor *object;
    struct FieldSprite *sprite;
    u8 *buffer;

    if (set != 0) {
        return FuneHeya_AnchorObject;
    }
    object = Engine_ObjectCreate(22, 0x1c70000, 0x40000, 0x2200000);
    object->motion_flags = set;
    object->unknown_5c = 1;
    sprite = object->sprite;
    sprite->part_count = set;
    sprite->full_color = 0;
    sprite->palette = 0;
    buffer = Engine_HeapAllocate(17, 0x608);
    Engine_ItemLoadIcon(232);
    Engine_VramLoad(sprite->vram_block, 128, buffer + 0x400);
    Engine_HeapRelease(17);
    Engine_GameFlagSet(0x200);
    FuneHeya_AnchorObject = object;
    return object;
}

void FieldScene_InstallFlaggedActors10To17(u8 *src)
{
    if (Engine_GameFlagIsSet(0x928) != 0) {
        u8 *obj = (u8 *)FuneHeya_FindFirstSetFlag(0, 0);
        Engine_ActorSetPosition(obj, 0xcd << 17, 0xac << 16);
        FieldScene_RunSceneStep(7, obj, src);
        Engine_ActorSetPosition(10, 0, 0);
    } else {
        FieldScene_RunSceneStep(5, 10, src);
    }

    if (Engine_GameFlagIsSet(0x929) != 0) {
        u8 *obj = (u8 *)FuneHeya_FindFirstSetFlag(1, 0);
        Engine_ActorSetPosition(obj, 0xeb << 17, 0xac << 16);
        ((struct FieldActor *)Object_GetById(obj))->scale_x = 0xffff0000;
        FieldScene_RunSceneStep(7, obj, src);
        Engine_ActorSetPosition(11, 0, 0);
    } else {
        FieldScene_RunSceneStep(6, 11, src);
    }

    if (Engine_GameFlagIsSet(0x92a) != 0) {
        u8 *obj = (u8 *)FuneHeya_FindFirstSetFlag(2, 0);
        Engine_ActorSetPosition(obj, 0xcd << 17, 0xcc << 16);
        FieldScene_RunSceneStep(7, obj, src);
        Engine_ActorSetPosition(12, 0, 0);
    } else {
        FieldScene_RunSceneStep(5, 12, src);
    }

    if (Engine_GameFlagIsSet(0x92b) != 0) {
        u8 *obj = (u8 *)FuneHeya_FindFirstSetFlag(3, 0);
        Engine_ActorSetPosition(obj, 0xeb << 17, 0xcc << 16);
        ((struct FieldActor *)Object_GetById(obj))->scale_x = 0xffff0000;
        FieldScene_RunSceneStep(7, obj, src);
        Engine_ActorSetPosition(13, 0, 0);
    } else {
        FieldScene_RunSceneStep(6, 13, src);
    }

    FieldScene_RunSceneStep(5, 14, src);
    FieldScene_RunSceneStep(6, 15, src);
    FieldScene_RunSceneStep(5, 16, src);
    FieldScene_RunSceneStep(6, 17, src);
}

void FuneHeya_PoseSceneActor(s32 id)
{
    switch (id) {
    case 19:
        Engine_ActorSetAnimation(id, 6);
        Object_SetActionById(id, 8);
        break;
    case 18:
    case 20:
        Engine_ActorSetAnimation(id, 5);
        Object_SetActionById(id, 16);
        break;
    case 22:
    case 23:
        Engine_ActorSetAnimation(id, 5);
        Object_SetActionById(id, 20);
        break;
    case 24:
        Engine_ActorSetAnimation(id, 10);
        Object_SetActionById(id, 8);
        break;
    case 21:
    case 25:
        Engine_ActorSetAnimation(id, 5);
        Object_SetActionById(id, 4);
        break;
    case 26:
        Engine_ActorSetAnimation(id, 9);
        Object_SetActionById(id, 4);
        break;
    }
}

void FieldScene_RunStepThen10(s32 a)
{
    Engine_EventShowMessage(a, 0);
    Engine_EventWait(10);
}

void FieldScene_CallPairWith10(s32 a, u16 b)
{
    Engine_ActorFaceDirection(a, b, 10);
}

void OverlayObject_SetPositionAndHeading(void *a, s32 b, s32 c, s32 d)
{
    ObjectMotion_SetHorizontalPositionWithTerrain(a, b << 16, c << 16, d);
    ((struct FieldActor *)Object_GetById(a))->facing = d;
}

void ConfigureSceneMotionFlags(s32 x, s32 y, s32 z, u32 flags)
{
    u32 selected;

    Engine_CameraMoveTo(x, y, z, ~flags & 1);
    selected = flags & 0x1111;
    if ((flags & 0x10000000) != 0)
        Engine_CameraWaitForMove();
    if ((flags & 0x01000000) != 0)
        Engine_MapRedraw();
    Engine_EventWait(selected);
}

void FieldScene_RunSceneStep(s32 step, u32 arg, u32 opt)
{
    extern const s32 FuneHeya_ActionScriptE[];

    u32 slot;

    switch (step) {
    case 0:
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
        Actor_FaceDirection(GERALD, FACING_WEST, 0);
        ObjectMotion_ArmCallback(2, 0, 0);
        Actor_FaceDirection(MEARI, FACING_WEST, opt);
        break;
    case 1:
        Actor_FaceDirection(ACTOR_PARTY_LEADER, arg, 0);
        Actor_FaceDirection(GERALD, arg, 0);
        Actor_FaceDirection(IWAN, arg, 0);
        Actor_FaceDirection(MEARI, arg, opt);
        break;
    case 2:
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimation(GERALD, 3);
        Engine_ActorSetAnimation(IWAN, 3);
        Engine_ActorSetAnimation(MEARI, 3);
        if (arg != 0) {
            ObjectMotion_WaitForAnimationChange(3);
        }
        if (opt == 0) {
            break;
        }
        Engine_EventWait(opt);
        break;
    case 3:
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Actor_SetAttachedEffect(GERALD, 0x102);
        Actor_SetAttachedEffect(IWAN, 0x102);
        Actor_SetAttachedEffect(MEARI, 0x102);
        Engine_EventWait(opt);
        break;
    case 4:
        for (slot = 0; slot < arg; slot++) {
            Actor_SetPosition(slot + 10, 0, 0);
        }
        break;
    case 5:
        {
            struct FieldActor *rec;

            rec = Object_GetById(arg);
            SetPose(rec, 0x5000);
        }
        Engine_ActorSetAnimation(arg, 5);
        Object_SetActionById(arg, opt);
        break;
    case 6:
        {
            struct FieldActor *rec;

            rec = Object_GetById(arg);
            SetPose(rec, 0x5000);
            rec->scale_x = -0x10000;
        }
        Engine_ActorSetAnimation(arg, 5);
        Object_SetActionById(arg, opt);
        break;
    case 7:
        {
            struct FieldActor *rec;

            rec = Object_GetById(arg);
            SetPose(rec, 0x5000);
        }
        FuneHeya_PoseSceneActor(arg);
        if (opt == 0) {
            Object_SetActionById(arg, 0);
        }
        break;
    case 8:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
        Engine_EventOpenScreen();
        if (arg != 0) {
            Engine_EventWaitForScreen();
        }
        Engine_EventWait(0);
        break;
    case 9:
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
        if (arg == 0) {
            break;
        }
        Engine_EventRequestExit(arg);
        break;
    case 10:
        FieldScene_RunSceneStep(24, 1, 0);
        FieldScene_RunSceneStep(25, 0, 0);
        FuneHeya_PlaceFoundActors(0);
        OverlayObject_SetPositionAndHeading(0, 0x1b0, 168, 0x4000);
        OverlayObject_SetPositionAndHeading(1, 0x1c0, 168, 0x4000);
        OverlayObject_SetPositionAndHeading(2, 0x1a8, 152, 0x4000);
        OverlayObject_SetPositionAndHeading(3, 0x1ca, 152, 0x4000);
        break;
    case 11:
        if (arg != 0) {
            struct FieldActor *rec;

            Engine_ActorSetAnimation(13, 1);
            rec = Object_GetById(13);
            SetPose(rec, 0x3000);
            rec = Object_GetById(13);
            rec->scale_x = 0x10000;
        }
        {
            struct FieldActor *rec;

            Engine_ActorSetAnimation(14, 1);
            rec = Object_GetById(14);
            SetPose(rec, 0x5000);
            Engine_ActorSetAnimation(15, 1);
            rec = Object_GetById(15);
            SetPose(rec, 0x3000);
            rec = Object_GetById(15);
            rec->scale_x = 0x10000;
            Engine_ActorSetAnimation(16, 1);
            rec = Object_GetById(16);
            SetPose(rec, 0x5000);
            Object_SetModeById(17, 1);
            rec = Object_GetById(17);
            SetPose(rec, 0x3000);
            rec = Object_GetById(17);
            rec->scale_x = 0x10000;
        }
        Actor_SetPosition(28, 0x19a0000, 0xae0000);
        Actor_SetPosition(29, 0x1d60000, 0xae0000);
        Actor_SetPosition(30, 0x19a0000, 0xce0000);
        Actor_SetPosition(31, 0x1d60000, 0xce0000);
        Actor_SetPosition(32, 0x19a0000, 0x11e0000);
        Actor_SetPosition(33, 0x1d60000, 0x11e0000);
        Actor_SetPosition(34, 0x19a0000, 0x13c0000);
        Actor_SetPosition(35, 0x1d60000, 0x13c0000);
        Engine_TaskWait(1);
        if (arg != 0) {
            Actor_FaceDirection(13, 0xb000, 0);
        }
        Actor_FaceDirection(14, 0xd000, 0);
        Actor_FaceDirection(15, 0xb000, 0);
        Actor_FaceDirection(16, 0xd000, 0);
        FieldScene_CallPairWith10(17, 0xb000);
        break;
    case 12:
        {
            struct FieldActor *rec;

            rec = Object_GetById(arg);
            Engine_ActorSetAnimation(arg, 1);
            if (opt != 0) {
                SetPose(rec, 0x3000);
            } else {
                SetPose(rec, 0x5000);
            }
            rec->scale_x = 0x10000;
        }
        break;
    case 13:
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(12, 0, 0);
        Actor_SetPosition(11, 0, 0);
        Actor_SetPosition(13, 0, 0);
        Actor_SetPosition(10, 0, 0);
        break;
    case 14:
        Actor_SetPosition(14, 0, 0);
        Actor_SetPosition(13, 0, 0);
        break;
    case 15:
        FieldScene_RunSceneStep(24, 1, 0);
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(10, 0, 0);
        OverlayObject_SetPositionAndHeading(8, 0x1bc, 0x266, 0xd000);
        Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
        if (arg != 0) {
            FuneHeya_PlaceAnchorCharm();
        }
        ConfigureSceneMotionFlags(0x1c00000, 0x200000, 0x2700000, 0x1000001);
        if (opt == 0) {
            break;
        }
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Battle_WaitMode0(20);
        break;
    case 16:
        Actor_SetPosition(8, 0, 0);
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(27, 0x1b60000, 0x980000);
        break;
    case 17:
        slot = 0;
        do {
            Actor_SetPosition(slot + 28, 0, 0);
            slot++;
        } while (slot <= 7);
        break;
    case 18:
        OverlayObject_SetPositionAndHeading(12, 152, 0x214, 0xb000);
        OverlayObject_SetPositionAndHeading(8, 134, 0x1ea, 0x3000);
        OverlayObject_SetPositionAndHeading(9, 166, 0x1ea, 0x5000);
        OverlayObject_SetPositionAndHeading(10, 182, 0x1f8, 0x5000);
        OverlayObject_SetPositionAndHeading(11, 118, 0x1f8, 0x3000);
        FieldScene_RunSceneStep(14, 0, 0);
        break;
    case 19:
        OverlayObject_SetPositionAndHeading(8, 0x1a0, 0x148, 0);
        OverlayObject_SetPositionAndHeading(9, 0x1c0, 0x160, 0xd000);
        OverlayObject_SetPositionAndHeading(10, 0x1c6, 248, 0x3000);
        OverlayObject_SetPositionAndHeading(arg, 0x198, 0x122, 0);
        OverlayObject_SetPositionAndHeading(opt, 0x198, 0x156, 0);
        OverlayObject_SetPositionAndHeading(13, 0x1a4, 0x164, 0xd000);
        OverlayObject_SetPositionAndHeading(14, 0x198, 0x130, 0);
        OverlayObject_SetPositionAndHeading(15, 0x1a2, 0x17a, 0xd000);
        OverlayObject_SetPositionAndHeading(16, 0x1b8, 0x106, 0x3000);
        OverlayObject_SetPositionAndHeading(17, 0x1c0, 0x17a, 0xd000);
        break;
    case 20:
        for (slot = arg; slot <= opt; slot++) {
            GameFlag_Clear(slot);
        }
        break;
    case 21:
        FieldScene_RunSceneStep(20, 0x92c, 0x93d);
        FieldScene_RunSceneStep(20, 0x917, 0x91f);
        FieldScene_RunSceneStep(20, 0x990, 0x998);
        GameFlag_Clear(0x300);
        GameFlag_Clear(0x301);
        GameFlag_Clear(0x302);
        break;
    case 22:
        Engine_TaskWait(1);
        FieldScene_RunSceneStep(23, 0, 0);
        Engine_ActorEnableActionCallback(12, FuneHeya_ActionScriptE);
        break;
    case 23:
        Engine_ActorDestroy(GERALD);
        Engine_ActorDestroy(IWAN);
        Engine_ActorDestroy(MEARI);
        break;
    case 24:
        Camera_MoveTo(-1, -1, -1, 0);
        Engine_TaskWait(1);
        if (arg != 0) {
            ((struct FieldActor *)Battle_GetWorkObject1e0())->motion_flags = 0;
        }
        break;
    case 25:
        Event_CallWithLastActiveObjectId(FuneHeya_CellStepsA);
        Engine_TaskWait(1);
        if (arg == 1) {
            Event_CallWithLastActiveObjectId(FuneHeya_CellStepsB);
            Engine_TaskWait(1);
        } else if (arg == 2) {
            Event_CallWithLastActiveObjectId(FuneHeya_CellStepsC);
            Engine_TaskWait(1);
        } else if (arg == 3) {
            Event_CallWithLastActiveObjectId(FuneHeya_CellStepsD);
            Engine_TaskWait(1);
        }
        break;
    }
}

s32 FuneHeya_FindFirstSetFlag(u32 group, s32 alternate)
{
    s32 flag = 0;
    s32 id = 8;
    u32 i;

    if (alternate == 0) {
        id = 18;
    }
    switch (group) {
    case 0:
        flag = 0x92c;
        break;
    case 1:
        flag = 0x935;
        break;
    case 2:
        flag = 0x917;
        break;
    case 3:
        flag = 0x990;
        break;
    }
    for (i = 0; i <= 8; i++, flag++, id++) {
        if (Engine_GameFlagIsSet(flag) != 0) {
            return id;
        }
    }
    return 0;
}

void SceneState_ApplyActor8FourFlags(void)
{
    SceneActor_RunFirstMatchingSlot(8, 0x92c);
    SceneActor_RunFirstMatchingSlot(8, 0x935);
    SceneActor_RunFirstMatchingSlot(8, 0x917);
    SceneActor_RunFirstMatchingSlot(8, 0x990);
}

void SceneActor_RunFirstMatchingSlot(s32 a0, s32 a1)
{
    unsigned int i = 0;

    do {
        if (Engine_GameFlagIsSet(a1)!= 0) {
            Engine_ActorSetPosition(a0, 0, 0);
            break;
        }
        i++;
        a0++;
        a1++;
    } while (i <= 8);
}

void FuneHeya_SetupReplacementRower(void)
{
    u32 i;
    s32 rec8;
    s32 record;
    s32 base5_200e840;
    s32 base5_200e8e4;

    rec8 = FuneHeya_FindFirstSetFlag(0, 0);
    Engine_EventBegin();
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 2, 0);
    SceneActor_SetFlagBit3ForActors28To35();
    FieldScene_RunSceneStep(19, rec8, 12);
    Engine_ActorSetAnimation(10, 6);
    base5_200e840 = (s32)((u8 *)FuneHeya_ActionScriptE);
    Engine_ActorEnableActionCallback(rec8, base5_200e840);
    Engine_ActorDestroy(11);
    Engine_ActorEnableActionCallback(12, base5_200e840);
    base5_200e8e4 = (s32)((u8 *)FuneHeya_EntryActionScript);
    Engine_ActorEnableActionCallback(36, base5_200e8e4);
    Engine_ActorEnableActionCallback(37, base5_200e8e4);
    FieldScene_RunPositionTransferPresentation();
    Engine_EventEnd();
}

void FieldScene_RunPositionTransferPresentation(void)
{
    struct FieldActor *record;
    s32 message;

    ConfigureSceneMotionFlags(0x1b80000, -1, 0xb00000, 0x1000001);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1b80000, 0x860000);
    Engine_EventOpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 5);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x198, 134);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x198, 152);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x1b0, 166);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(IWAN, record->x.fixed, record->z.fixed);
    }
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(GERALD, record->x.fixed, record->z.fixed);
    }
    record = Object_GetById(1);
    if (record != 0) {
        Actor_SetPosition(MEARI, record->x.fixed, record->z.fixed);
    }
    Engine_TaskWait(1);
    Actor_SetSpeed(IWAN, 0x19999, 0xcccc);
    Actor_WalkTo(IWAN, 0x1a8, 152);
    Actor_SetSpeed(GERALD, 0x19999, 0xcccc);
    Actor_WalkTo(GERALD, 0x1c0, 168);
    Actor_SetSpeed(MEARI, 0x20000, 0x10000);
    Actor_WalkToAndWait(MEARI, 0x1ca, 152);
    Engine_ActorSetAnimation(GERALD, 1);
    Engine_ActorSetAnimation(IWAN, 1);
    Actor_FaceDirection(IWAN, FACING_SOUTH, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    Actor_FaceDirection(MEARI, FACING_SOUTH, 40);
    Ui_SetRenderResultFromObject(10);
    message = (s32)MsgFuneMonstersBelowdecks;
    UiText_ShowCenteredMessage(message, 1, 10);
    Engine_EventWait(10);
    FieldScene_RunSceneStep(0, 0, 40);
    FieldScene_RunSceneStep(1, 0x4000, 20);
    Camera_SetSpeed(0x39999, 0x7333);
    ConfigureSceneMotionFlags(0x1b80000, -1, 0x1400000, 0x10000014);
    Engine_ActorRunRepeatedMotion(8, 2);
    FieldScene_CallPairWith10(8, 0xd000);
    Engine_EventSetMessage(message + 1);
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(8, FACING_EAST, 20);
    ConfigureSceneMotionFlags(0x1b80000, -1, 0x860000, 0x10000000);
    Engine_ActorEnableActionCallback(GERALD, FuneHeya_ProgressTableA);
    Engine_ActorEnableActionCallback(2, FuneHeya_ProgressTableA);
    Object_SetActionCallbackAndRefreshById(3, (s32)FuneHeya_ProgressTableA);
    Engine_EventWait(40);
    GameFlag_Set(0x301);
    FieldScene_RunSceneStep(23, 0, 0);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
}

void FieldScene_RunFormationAndEffectPresentation(void)
{
    s32 action;

    OverlayObject_SetPositionAndHeading(0, 0x1bc, 0x12c, 0);
    OverlayObject_SetPositionAndHeading(1, 0x1ca, 0x136, 0);
    OverlayObject_SetPositionAndHeading(2, 0x1bc, 0x14a, 0);
    OverlayObject_SetPositionAndHeading(3, 0x1b0, 0x136, 0);
    OverlayObject_SetPositionAndHeading(27, 0x1b8, 134, 0x8000);
    OverlayObject_SetPositionAndHeading(10, 0x1c6, 248, 0x3000);
    Engine_ActorSetAnimation(10, 6);
    ConfigureSceneMotionFlags(0x1b80000, -1, 0x1340000, 0x1000001);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(GERALD, FACING_NORTHWEST, 0);
    Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_SOUTHEAST, 40);
    FieldScene_RunSceneStep(2, 1, 20);
    Engine_EventSetMessage((s32)MsgFuneHeyAreYouOk);
    FieldScene_RunStepThen10(27);
    FieldScene_RunSceneStep(1, 0xc000, 0);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x1b80000, -1, 0xb00000, 1);
    Actor_SetSpeed(27, 0x19999, 0xcccc);
    Actor_WalkToAndWait(27, 0x198, 134);
    Actor_WalkToAndWait(27, 0x198, 152);
    Actor_WalkToAndWait(27, 0x1a8, 164);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x1b80000, -1, 0x12c0000, 1);
    Actor_WalkToAndWait(27, 0x1a8, 222);
    Actor_WalkToAndWait(27, 0x1a8, 0x106);
    Actor_FaceDirection(27, 0x3000, 20);
    Engine_ActorRunRepeatedMotion(27, 1);
    FieldScene_RunStepThen10(27);
    FieldScene_RunSceneStep(2, 1, 20);
    Engine_ActorSetAnimationAndWait(27, 3);
    Engine_ActorRunRepeatedMotion(27, 1);
    FieldScene_RunStepThen10(27);
    FieldScene_RunSceneStep(3, 2, 60);
    FieldScene_RunSceneStep(1, 0xe000, 60);
    Actor_FaceDirection(27, FACING_EAST, 40);
    Engine_ActorRunRepeatedMotion(27, 1);
    Engine_ActorSetAnimation(27, 2);
    Actor_MoveToAndWait(27, 0x1b0, 0x10c);
    Actor_MoveToAndWait(27, 0x1c4, 0x10c);
    Engine_ActorSetAnimation(27, 1);
    FieldScene_CallPairWith10(27, 0xd000);
    Engine_ActorStartRepeatedMotion(27, 2);
    Event_ShowMessageAndWait(27, 0, 20);
    FieldScene_RunSceneStep(1, 0xc000, 20);
    Engine_ActorSetAnimationAndWait(27, 4);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(27, 0, 80);
    Engine_ActorRunRepeatedMotion(27, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(27, 3);
    Engine_EventWait(10);
    FieldScene_CallPairWith10(27, 0x5000);
    Event_OpenMessage(27, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(27, 3);
        FieldScene_RunStepThen10(27);
    } else {
        Engine_ActorSetAnimationAndWait(27, 4);
        gEventWork->message += 1;
        FieldScene_RunStepThen10(27);
        FieldScene_RunSceneStep(3, 2, 40);
        Engine_ActorRunRepeatedMotion(27, 1);
        Engine_ActorSetAnimation(27, 3);
        FieldScene_RunStepThen10(27);
    }
    FieldScene_RunSceneStep(2, 1, 20);
    action = (s32)FuneHeya_ActionScriptC;
    Engine_ActorEnableActionCallback(GERALD, action);
    Engine_ActorEnableActionCallback(IWAN, action);
    Object_SetActionCallbackAndRefreshById(3, action);
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0x1b80000, -1, 0xb00000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1a8, 0x110);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1a8, 164);
    Engine_EventWait(60);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    FieldScene_RunSceneStep(9, 0, 0);
    GameFlag_Clear(0x301);
    GameFlag_Clear(0x927);
    Engine_EventRequestExit(4);
}

void FieldScene_RunActors24And25SetupWithValue929(void)
{
    s32 handle = FuneHeya_FindFirstSetFlag(0, 0);

    Engine_EventBegin();
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 0, 0);
    FuneHeya_PlaceFoundActors(0);
    FieldScene_RunSceneStep(19, handle, 12);
    Engine_ActorSetPosition(11, 0, 0);
    FieldScene_RunFormationAndEffectPresentation();
    Engine_GameFlagSet(0x929);
    Engine_EventEnd();
}

void FuneHeya_SetupReplacementPair(void)
{
    u32 i;
    s32 rec2;
    s32 rec8;
    s32 record;
    s32 base5_200e840;
    s32 base5_200e8e4;

    rec8 = FuneHeya_FindFirstSetFlag(0, 0);
    rec2 = FuneHeya_FindFirstSetFlag(1, 0);
    Engine_EventBegin();
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 3, 0);
    SceneActor_SetFlagBit3ForActors28To35();
    FieldScene_RunSceneStep(19, rec8, rec2);
    Engine_ActorSetAnimation(10, 6);
    base5_200e840 = (s32)((u8 *)FuneHeya_ActionScriptE);
    Engine_ActorEnableActionCallback(rec8, base5_200e840);
    Engine_ActorDestroy(11);
    Engine_ActorEnableActionCallback(rec2, base5_200e840);
    Engine_ActorDestroy(12);
    base5_200e8e4 = (s32)((u8 *)FuneHeya_EntryActionScript);
    Engine_ActorEnableActionCallback(36, base5_200e8e4);
    Engine_ActorEnableActionCallback(37, base5_200e8e4);
    Engine_ActorSetChildValue(36, 3);
    Engine_ActorSetChildValue(37, 3);
    FieldScene_RunPositionTransferPresentation();
    Engine_EventEnd();
}

void FieldScene_RunActors24And25SetupWithValue92a(void)
{
    s32 handle = FuneHeya_FindFirstSetFlag(0, 0);
    s32 other = FuneHeya_FindFirstSetFlag(1, 0);

    Engine_EventBegin();
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 0, 0);
    FuneHeya_PlaceFoundActors(0);
    FieldScene_RunSceneStep(19, handle, other);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
    FieldScene_RunFormationAndEffectPresentation();
    Engine_GameFlagSet(0x92a);
    Engine_EventEnd();
}

void FieldScene_RunExtendedFormationPresentation(void)
{
    s32 slot_b;
    s32 slot_c;
    s32 slot_a;
    struct FieldActor *record;
    s32 action;

    slot_a = FuneHeya_FindFirstSetFlag(0, 0);
    slot_b = FuneHeya_FindFirstSetFlag(1, 0);
    slot_c = FuneHeya_FindFirstSetFlag(2, 0);
    Engine_EventBegin();
    FieldScene_RunSceneStep(10, 0, 0);
    FieldScene_RunSceneStep(17, 0, 0);
    Actor_SetPosition(8, 0x1d80000, 0x980000);
    Engine_ActorSetAnimation(9, 5);
    Actor_SetPosition(27, 0x1b80000, 0x860000);
    Actor_SetChildValue(27, 15);
    record = Object_GetById(27);
    Engine_ActorSetSpriteFlags(record, 0);
    FieldScene_InstallFlaggedActors10To17(16);
    ConfigureSceneMotionFlags(0x1b60000, -1, 0xae0000, 0x1000001);
    FieldScene_RunSceneStep(8, 1, 20);
    Audio_PlayCue(19);
    Audio_PlayCue(181);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Engine_EventWait(10);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(80);
    Audio_PlayCue(181);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Engine_EventWait(10);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Audio_PlayCue(63);
    GameFlag_Set(0x11a);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Engine_EventWait(40);
    FieldScene_CallPairWith10(3, 0x6000);
    Engine_EventSetMessage((s32)MsgFuneWonderWhatsWrongShipShouldnt);
    Event_ShowMessageAndWait(MEARI, 0, 40);
    FieldScene_RunStepThen10(27);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(GERALD, FACING_NORTHWEST, 0);
    Actor_FaceDirection(IWAN, FACING_EAST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHEAST, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
    Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_WEST, 40);
    Actor_FaceDirection(IWAN, FACING_NORTHEAST, 0);
    Actor_ShowEmote(IWAN, 0x100, 60);
    FieldScene_CallPairWith10(2, 0x2000);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    FieldScene_RunStepThen10(2);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTHEAST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHWEST, 20);
    Actor_SetChildValue(27, 0);
    record = Object_GetById(27);
    Engine_ActorSetSpriteFlags(record, 1);
    Actor_SetSpeed(27, 0x10000, 0x8000);
    Actor_WalkToAndWait(27, 0x1ae, 134);
    FieldScene_CallPairWith10(27, 0x3000);
    Engine_ActorStartRepeatedMotion(27, 2);
    FieldScene_RunStepThen10(27);
    Engine_ActorStartRepeatedMotion(slot_a, 1);
    Engine_ActorStartRepeatedMotion(slot_b, 1);
    Engine_ActorStartRepeatedMotion(slot_c, 1);
    Engine_ActorRunRepeatedMotion(13, 1);
    Actor_SetAttachedEffect(slot_a, 0x102);
    Actor_SetAttachedEffect(slot_b, 0x102);
    Actor_SetAttachedEffect(slot_c, 0x102);
    Actor_SetAttachedEffect(13, 0x102);
    Engine_EventWait(40);
    FieldScene_RunSceneStep(12, slot_a, 0);
    FieldScene_RunSceneStep(12, slot_b, 1);
    FieldScene_RunSceneStep(12, slot_c, 0);
    FieldScene_RunSceneStep(11, 1, 0);
    Actor_FaceDirection(slot_a, 0xd000, 0);
    Actor_FaceDirection(slot_b, 0xb000, 0);
    Actor_FaceDirection(slot_c, 0xd000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(GERALD, FACING_WEST, 0);
    Actor_FaceDirection(IWAN, FACING_EAST, 0);
    Actor_FaceDirection(MEARI, FACING_WEST, 40);
    Engine_ActorStartRepeatedMotion(27, 2);
    Event_ShowMessage(27, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    Actor_FaceDirection(MEARI, FACING_NORTH, 20);
    Actor_WalkToAndWait(27, 0x1b8, 134);
    Actor_SetPosition(27, 0, 0);
    FieldScene_CallPairWith10(1, 0x8000);
    Engine_ActorRunRepeatedMotion(GERALD, 1);
    FieldScene_RunStepThen10(1);
    Actor_FaceDirection(IWAN, FACING_EAST, 0);
    FieldScene_CallPairWith10(3, 0x8000);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Actor_SetSpeed(GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(IWAN, 0x10000, 0x8000);
    Actor_SetSpeed(MEARI, 0x10000, 0x8000);
    action = (s32)FuneHeya_ActionScriptD;
    Engine_ActorEnableActionCallback(GERALD, action);
    Engine_ActorEnableActionCallback(IWAN, action);
    Object_SetActionCallbackAndRefreshById(3, action);
    GameFlag_Set(0x302);
    FuneHeya_CueTimer = 0;
    Scheduler_AddOrUpdateCallback((s32)Scene_UpdateCueTimer, 0xc80);
    FieldScene_RunSceneStep(23, 0, 0);
    Engine_ActorDestroy(27);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    GameFlag_Clear(0x927);
    Engine_EventEnd();
}

void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2)
{
    s32 value;
    s32 *timer = &FuneHeya_CueTimer;
    s32 remaining;
    if (*timer != 0) {
        remaining = *timer - 1;
        *timer = *timer - 1;
        if (remaining == 70)
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    } else {
        value = Engine_RandomNext();
        if (((u32)(((value << 4) - value) << 3) >> 16) == 0) {
            Engine_AudioPlayCue(181);
            Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
            *timer = 80;
        }
    }
}

void RunActorsEightAndNineMapEvent(void)
{
    Engine_EventBegin();
    FieldScene_RunSceneStep(15, 1, 0);
    OverlayObject_SetPositionAndHeading(9, 468, 616, 32768);
    FieldScene_RunSceneStep(8, 1, 20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(8, 53248, 80);
    Actor_FaceDirection(8, FACING_EAST, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    FieldScene_RunSceneStep(9, 21, 0);
}

void Scene_RunFourActorProgressPresentation(void)
{
    s32 rec;
    s32 rec2;
    s32 rec4;
    s32 rec7;
    struct FieldActor *record;
    s32 v6 = 0;
    s32 base6_200e904;
    s32 base5_200e938;
    s32 base5_200e7c8;

    rec7 = FuneHeya_FindFirstSetFlag(0, 0);
    rec2 = FuneHeya_FindFirstSetFlag(1, 0);
    rec4 = FuneHeya_FindFirstSetFlag(2, 0);
    rec = FuneHeya_FindFirstSetFlag(3, 0);
    Engine_EventBegin();
    SceneActor_SetFlagBit3ForActors28To35();
    FieldScene_RunSceneStep(10, 0, 0);
    FieldScene_RunSceneStep(17, 0, 0);
    Actor_SetPosition(8, 0x1d80000, 0x980000);
    Actor_SetPosition(27, 0x1b80000, 0x860000);
    Engine_ActorSetChildValue(27, 15);
    record = Object_GetById(27);
    Engine_ActorSetSpriteFlags(record, 0);
    FieldScene_InstallFlaggedActors10To17(16);
    Engine_ActorSetAnimation(9, 5);
    ConfigureSceneMotionFlags(0x1b60000, -1, 0xae0000, 0x1000001);
    FieldScene_RunSceneStep(8, 1, 20);
    Engine_ActorSetChildValue(27, 0);
    record = Object_GetById(27);
    Engine_ActorSetSpriteFlags(record, 1);
    Actor_SetSpeed(27, 0x10000, 0x8000);
    Actor_WalkToAndWait(27, 0x198, 132);
    Actor_WalkToAndWait(27, 0x198, 142);
    Actor_FaceDirection(27, 0x3000, 20);
    Engine_ActorStartRepeatedMotion(27, 2);
    Engine_EventSetMessage((s32)MsgFuneReachedIslandRowers);
    FieldScene_RunStepThen10(27);
    Engine_EventWait(120);
    FieldScene_RunSceneStep(12, rec7, 0);
    FieldScene_RunSceneStep(12, rec2, 1);
    FieldScene_RunSceneStep(12, rec4, 0);
    FieldScene_RunSceneStep(12, rec, 1);
    FieldScene_RunSceneStep(11, 0, 0);
    Actor_FaceDirection(rec7, 0xd000, 0);
    Actor_FaceDirection(rec2, 0xb000, 0);
    Actor_FaceDirection(rec4, 0xd000, 0);
    Engine_ActorFaceDirection(rec, 0xb000, 60);
    if (Engine_GameFlagIsSet(0x934)) {
        v6 = 2;
    } else if (Engine_GameFlagIsSet(0x933) || Engine_GameFlagIsSet(0x92f)) {
        v6 = 1;
    }
    Engine_ActorRunRepeatedMotion(rec7, 1);
    if (v6 == 1) {
        gEventWork->message += 1;
    } else if (v6 == 2) {
        gEventWork->message += 2;
    }
    Engine_ActorStartRepeatedMotion(rec7, 2);
    FieldScene_RunStepThen10(rec7);
    Engine_EventSetMessage((s32)MsgFuneShipWentOff);
    Engine_ActorSetAnimationAndWait(27, 4);
    FieldScene_RunStepThen10(27);
    Actor_ShowEmote(rec7, 0x102, 0);
    Actor_ShowEmote(rec2, 0x102, 0);
    Actor_ShowEmote(rec4, 0x102, 0);
    Engine_ActorShowEmote(rec, 0x102, 60);
    FieldScene_RunStepThen10(27);
    Actor_WalkToAndWait(27, 0x198, 132);
    Actor_WalkToAndWait(27, 0x1bc, 132);
    Engine_ActorDestroy(27);
    Engine_EventWait(40);
    if (v6 == 0 && (Engine_GameFlagIsSet(0x92c) || Engine_GameFlagIsSet(0x92d))) {
        v6 = 3;
    }
    if (v6 == 0) {
        gEventWork->message += 1;
    } else if (v6 == 1) {
        gEventWork->message += 2;
    } else if (v6 == 2) {
        gEventWork->message += 3;
    }
    FieldScene_CallPairWith10(rec7, 0);
    FieldScene_RunStepThen10(rec7);
    Actor_SetSpeed(rec7, 0x10000, 0x8000);
    base6_200e904 = (s32)&FuneHeya_ProgressTableB;
    Object_SetActionCallbackAndRefreshById(rec7, base6_200e904);
    Actor_FaceDirection(rec2, 0x5000, 0);
    Engine_ActorFaceDirection(rec4, FACING_EAST, 0);
    Actor_FaceDirection(rec, FACING_WEST, 40);
    Engine_ActorFaceDirection(rec2, 0xd000, 0);
    Engine_ActorFaceDirection(rec4, 0xb000, 0);
    Engine_ActorFaceDirection(rec, 0x5000, 20);
    Actor_SetSpeed(rec2, 0x10000, 0x8000);
    Actor_SetSpeed(rec4, 0x10000, 0x8000);
    Actor_SetSpeed(rec, 0x10000, 0x8000);
    Engine_ActorEnableActionCallback(rec4, base6_200e904);
    Engine_EventWait(40);
    base5_200e938 = (s32)&FuneHeya_ProgressTableC;
    Object_SetActionCallbackAndRefreshById(rec2, base5_200e938);
    ((void (*)())Engine_ActorEnableActionCallback)(rec2, base6_200e904);
    Object_SetActionCallbackAndRefreshById(rec, base5_200e938);
    Object_SetActionCallbackAndRefreshById(rec, base6_200e904);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_SetSpeed(2, 0x10000, 0x8000);
    Actor_SetSpeed(3, 0x10000, 0x8000);
    base5_200e7c8 = (s32)&FuneHeya_ProgressTableA;
    Engine_ActorEnableActionCallback(1, base5_200e7c8);
    Engine_ActorEnableActionCallback(2, base5_200e7c8);
    Object_SetActionCallbackAndRefreshById(3, base5_200e7c8);
    FieldScene_RunSceneStep(23, 0, 0);
    Engine_GameFlagClear(0x927);
    Engine_GameFlagSet(0x8a0);
    Engine_GameFlagClear(0x12f);
    Engine_EventEnd();
}

void FuneHeya_DismissReplacementRowers(void)
{
    s32 rec2;
    s32 rec4;
    s32 rec7;
    s32 rec8;

    rec2 = FuneHeya_FindFirstSetFlag(0, 0);
    rec8 = FuneHeya_FindFirstSetFlag(1, 0);
    rec7 = FuneHeya_FindFirstSetFlag(2, 0);
    rec4 = FuneHeya_FindFirstSetFlag(3, 0);
    Engine_EventBegin();
    FieldScene_RunSceneStep(10, 0, 0);
    OverlayObject_SetPositionAndHeading(8, 0x1d8, 144, 0x5000);
    OverlayObject_SetPositionAndHeading(27, 0x198, 142, 0x3000);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(27, 1);
    Engine_EventSetMessage((s32)MsgFuneSorryEveryoneButWeNeed);
    FieldScene_RunStepThen10(27);
    Engine_ActorStartRepeatedMotion(rec2, 2);
    Engine_ActorStartRepeatedMotion(rec8, 2);
    Engine_ActorStartRepeatedMotion(rec7, 2);
    Engine_ActorRunRepeatedMotion(rec4, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(rec2, FACING_EAST, 0);
    Actor_FaceDirection(rec8, FACING_WEST, 0);
    Actor_FaceDirection(rec7, FACING_EAST, 0);
    Actor_FaceDirection(rec4, FACING_WEST, 40);
    Actor_SetSpeed(rec2, 0x10000, 0x8000);
    Actor_SetSpeed(rec8, 0x10000, 0x8000);
    Actor_SetSpeed(rec7, 0x10000, 0x8000);
    Actor_SetSpeed(rec4, 0x10000, 0x8000);
    Actor_WalkTo(rec2, 0x1d6, 172);
    Actor_WalkTo(rec8, 0x19a, 172);
    Actor_WalkTo(rec7, 0x1d6, 204);
    Actor_WalkToAndWait(rec4, 0x19a, 204);
    Engine_ActorSetAnimation(rec2, 1);
    Engine_ActorSetAnimation(rec8, 1);
    Engine_ActorSetAnimation(rec7, 1);
    Actor_FaceDirection(rec8, 0xd000, 0);
    Actor_FaceDirection(rec2, 0xb000, 0);
    Actor_FaceDirection(rec4, 0xd000, 0);
    Actor_FaceDirection(rec7, 0xb000, 20);
    Engine_ActorRunRepeatedMotion(27, 1);
    FieldScene_RunStepThen10(27);
    Engine_ActorSetAnimation(rec2, 3);
    Engine_ActorSetAnimation(rec8, 3);
    Engine_ActorSetAnimation(rec7, 3);
    Engine_ActorSetAnimationAndWait(rec4, 3);
    FieldScene_RunStepThen10(27);
    Engine_ActorSetAnimation(rec2, 3);
    Engine_ActorSetAnimation(rec8, 3);
    Engine_ActorSetAnimation(rec7, 3);
    Engine_ActorSetAnimationAndWait(rec4, 3);
    Actor_FaceDirection(27, FACING_EAST, 0);
    FieldScene_CallPairWith10(0, 0x8000);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(27, 3);
    Actor_SetSpeed(27, 0x10000, 0x8000);
    Actor_WalkToAndWait(27, 0x198, 132);
    Actor_WalkToAndWait(27, 0x1bc, 132);
    Actor_SetPosition(27, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    SceneState_ScanTwoArraysAndCrossNotify(0x92c, 0x935);
    SceneState_ScanTwoArraysAndCrossNotify(0x917, 0x990);
    GameFlag_Clear(0x8a0);
    Engine_EventRequestExit(10);
}

void SceneState_ScanTwoArraysAndCrossNotify(u8 *a, u8 *b)
{
    s32 cnt_a = 0;
    s32 cnt_b = 0;
    u32 i;

    for (i = 0; i <= 8; i++) {
        u8 *p = a + i;
        if (Engine_GameFlagIsSet(p)!= 0) {
            Engine_GameFlagClear(p);
            break;
        }
        cnt_a++;
    }

    for (i = 0; i <= 8; i++) {
        u8 *p = b + i;
        if (Engine_GameFlagIsSet(p)!= 0) {
            Engine_GameFlagClear(p);
            break;
        }
        cnt_b++;
    }

    Engine_GameFlagSet(b + cnt_a);
    Engine_GameFlagSet(a + cnt_b);
}
