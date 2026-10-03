#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
#include "HEYA.H"
#include "CALL.H"

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

extern u8 FuneHeya_SceneTable02[];
extern u8 FuneHeya_SceneTable01[];
extern u8 FuneHeya_SceneTable03;
struct SceneActor *Object_GetByIdFar(s32 actor_id);

extern u8 FuneHeya_StepScriptF[];
extern u8 FuneHeya_StepScriptG[];
extern u8 FuneHeya_StepScriptE[];
extern u8 FuneHeya_StepScriptH[];
extern u8 FuneHeya_StepScriptD[];
extern u8 FuneHeya_StepScriptC[];
extern u8 FuneHeya_StepScriptB[];
extern u8 FuneHeya_StepScriptA[];
s32 Engine_GameFlagIsSet();
extern u8 Data_02000240[];

extern u8 MsgFuneBadLuckLosingMyLucky[];
extern u8 MsgFuneButWeCantSendShip[];
extern u8 MsgFuneHaHaHaChosenOarsman[];
extern u8 MsgFuneIfShipFromTolbiHad[];
extern u8 MsgFuneItsTooLateHireMercenaries[];
extern u8 MsgFuneJokingWantRow[];
extern u8 MsgFuneLongerWeSitHereMore[];
extern u8 MsgFuneLooksLikeYouveBeenChosen[];
extern u8 MsgFuneNoooooNotGoing[];
extern u8 MsgFuneNowWantSeeCaptainToo[];
extern u8 MsgFuneOarsmanWasInjured[];
extern u8 MsgFuneTheresWholeBunchReallyMuscular[];
extern u8 MsgFuneTheseProudWarriorsNotGoing[];
extern u8 MsgFuneWeDontKnowMightHappen[];
extern u8 MsgFuneYouCameAskCaptainSet[];
extern u8 MsgFuneYouGoingRow[];
extern u8 FuneHeya_SceneTable04[];
extern u8 FuneHeya_SceneTable05[];
extern u8 FuneHeya_SceneTable06[];
extern u8 FuneHeya_SceneTable07[];
extern u8 FuneHeya_SceneTable08[];
extern u8 FuneHeya_SceneTable09[];
extern u8 FuneHeya_SceneTable10[];
extern u8 FuneHeya_SceneTable11[];
extern u8 FuneHeya_SceneTable12[];
extern u8 FuneHeya_SceneTable13[];
extern u8 FuneHeya_SceneTable14[];
void Battle_ResetEffectCounterFar();
void FieldScene_CallPairWith10(s32 a, u16 b);

extern u8 MsgFuneDontFeelDontTalkMe[];
extern u8 MsgFuneDontTellGoing[];
extern u8 MsgFuneYouFinallyPickedSomeoneDidnt[];
s32 SceneState_ApplyLevelFromFlags(void);
void FuneHeya_TurnActorToOpenSide(s32 actor);
void FieldScene_RunStepThen10(s32 actor);
void FieldScene_RunPrimarySequence(s32 actor, s32 message, s32 flag);

extern u8 MsgFuneGivesMeChillsThinkCould[];
extern u8 MsgFuneHaHaHaRowingFeel[];
extern u8 MsgFuneHoHoPersonGoingGet[];
extern u8 MsgFuneIfYouveChosenOarsmanLets[];
extern u8 MsgFuneNotThinkingMaking[];
extern u8 MsgFuneOarsmanGiveBreak[];
extern u8 MsgFuneOhhhhNooooGoingMakeMe[];
extern u8 MsgFunePeopleAskingFrail[];
extern u8 MsgFunePoorOarsmanFeel[];
extern u8 MsgFuneRobinYouveGotGoodEye[];
extern u8 MsgFuneThatsWhoYouPickedOarsman[];
extern u8 MsgFuneYouveBeenChosenAsOarsman[];
s32 SceneState_ApplyLevelFromFlags();

s32 SceneActor_CheckBucketOffsetPoint();
void FieldScene_CallPairWith10();
void Engine_ActorSetSpeed();
void Engine_ActorSetPosition();
void Engine_ActorSetDestinationOffset();
void Engine_ActorSetAnimation();
void ObjectMotion_SetActionVariant();
void Engine_ActorWaitForMove();
extern s32 FuneHeya_TurnSteps[];

void FuneHeya_TurnActorToOpenSide(s32 a0);

s32 StagedActor_CountdownUntilPositionUnset(u8 *object);
void StagedActor_AdvanceCounter98(u8 *object);

struct Walker {
    u8 unknown_00[0x4c];
    s32 countdown;
    u8 unknown_50[0x16];
    s16 step;
};

/* Ship cabin walker: advance the actor's scripted walk one step, turning, walking and waiting on the leader between steps. */
void FuneHeya_RunWalkerStep(struct FieldActor *obj)
{
    struct FieldActor *leader;
    struct Walker *walker;

    walker = (struct Walker *)obj;
    leader = Object_GetById(8);
    switch (walker->step) {
    case 0:
        obj->facing = 0xb000;
        goto advance;
    case 2:
        obj->facing = 0;
        goto advance;
    case 4:
        Object_SetMode(obj, 2);
        Call4(Engine_ObjectSetPosition, (s32)obj, 0x1d40000, 0x200000, 0x2780000);
        walker->countdown = 60;
        walker->step++;
        break;
    case 5:
        if (StagedActor_CountdownUntilPositionUnset((u8 *)obj) != 0) {
            Object_SetMode(obj, 1);
            obj->rise_counter = 0;
            if (leader->unknown_5b == 0) {
                obj->rise_enabled = 1;
            }
            walker->step++;
        }
        break;
    case 7:
        if (leader->unknown_5b == 0) {
            Object_SetMode(obj, 3);
            obj->rise_enabled = 2;
        }
    advance:
        walker->step++;
        /* FAKEMATCH: the zero is spelled as a 16-bit value so it is loaded after the step store; a plain 0 is loaded before it. */
        *(u8 *)&obj->rise_counter = (u16)0;
        break;
    case 9:
        Object_SetMode(obj, 2);
        Call4(Engine_ObjectSetPosition, (s32)obj, 0x1e00000, 0x200000, 0x2580000);
        walker->countdown = 60;
        walker->step++;
        if (leader->unknown_5b == 0) {
            obj->rise_enabled = 3;
        }
        break;
    case 10:
        if (StagedActor_CountdownUntilPositionUnset((u8 *)obj) != 0) {
            Object_SetMode(obj, 1);
            obj->rise_counter = 0;
            walker->step++;
        }
        break;
    case 1:
    case 3:
    case 6:
    case 8:
    case 11:
        StagedActor_AdvanceCounter98((u8 *)obj);
        break;
    case 12:
        walker->step = 0;
        break;
    }
}

void UpdateActorNineEffectMode(struct EffectRecord *record)
{
    struct SceneActor *actor;

    actor = Object_GetByIdFar(9);
    if (record->state != 0)
        return;
    if (actor->mode == 1) {
        record->angle = 0xd000;
        record->active = 1;
        actor->mode = 0;
    } else if (actor->mode == 2) {
        if (record->active != 0)
            Object_SetMode(record, 3);
        record->active = 0;
        actor->mode = 0;
    } else if (actor->mode == 3) {
        record->angle = 0;
        actor->mode = 0;
    }
}

/* Places both sprite parts behind the foreground and clears automatic priority. */
s32 StagedActor_SetReadyState(struct FieldActor *work)
{
    struct FieldSprite *rec = work->sprite;

    work->collision_flags = 8;
    Engine_ActorSetSpriteFlags(work, 0);
    rec->priority = 1;
    rec->second_priority = 1;
    work->priority_flags = (work->priority_flags & ~1) | 2;
    ObjectGroup_SetChildValue(work, 15);
    return 1;
}

/*
 * Overlay resource_3b1. Picks the scene data table for the current scene
 * index, with two arms further narrowed by story flags.
 */
s32 SceneData_SelectTableByWord224(void)
{
    if (gGameState.scene == (s32)&SceneId_FuneHeya) {
        return (s32)FuneHeya_SceneTable02;
    }
    return (s32)FuneHeya_SceneTable01;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_GetTableEB94(void)
{
    return (s32)&FuneHeya_SceneTable03;
}

s32 FuneHeya_GetStepScript(void)
{
    u8 *script;

    switch (gGameState.entrance) {
    case 1:
    case 2:
    case 11:
        if (Engine_GameFlagIsSet(0x93e) != 0) {
            return (s32)FuneHeya_StepScriptF;
        } else if (Engine_GameFlagIsSet(0x928) != 0) {
            if (Engine_GameFlagIsSet(0x8a0) != 0) {
                script = FuneHeya_StepScriptG;
                script[22] = 2;
                script[70] = 2;
                script[118] = 2;
                script[142] = 2;
                script[214] = 2;
                script[190] = 2;
                script[166] = 1;
                script[94] = 2;
            }
            return (s32)FuneHeya_StepScriptG;
        } else if (Engine_GameFlagIsSet(0x911) != 0) {
            if (Engine_GameFlagIsSet(0x925) != 0) {
                FuneHeya_StepScriptF[22] = 2;
                FuneHeya_StepScriptF[118] = 2;
                FuneHeya_StepScriptF[46] = 2;
                FuneHeya_StepScriptF[94] = 2;
            }
            return (s32)FuneHeya_StepScriptF;
        } else {
            return (s32)FuneHeya_StepScriptE;
        }
        break;
    case 4:
    case 12:
    case 16:
    case 18:
    case 20:
    case 21:
    case 23:
    case 24:
        return (s32)FuneHeya_StepScriptH;
        break;
    case 15:
    case 17:
    case 19:
        script = FuneHeya_StepScriptH;
        script[22] = 2;
        script[46] = 2;
        script[94] = 1;
        script[118] = 2;
        script[142] = 2;
        script[166] = 2;
        script[190] = 2;
        script[214] = 1;
        script[238] = 2;
        return (s32)script;
    case 5:
        if (Engine_GameFlagIsSet(0x93e) != 0) {
            return (s32)FuneHeya_StepScriptD;
        } else if (Engine_GameFlagIsSet(0x911) != 0) {
            if (Engine_GameFlagIsSet(0x922) != 0) {
                if (Value1(Engine_GameFlagIsSet, 0x8a0) != 0) {
                    FuneHeya_StepScriptC[46] = 1;
                }
                if (Engine_GameFlagIsSet(0x925) != 0) {
                    if (!(Engine_GameFlagIsSet(0x8a0) != 0)) {
                        FuneHeya_StepScriptC[22] = 0;
                    }
                }
                return (s32)FuneHeya_StepScriptC;
            } else {
                return (s32)FuneHeya_StepScriptA;
            }
        } else {
            return (s32)FuneHeya_StepScriptB;
        }
        break;
    case 10:
    case 13:
    case 14:
    case 22:
        return (s32)FuneHeya_StepScriptB;
        break;
    default:
        return (s32)FuneHeya_StepScriptA;
        break;
    }
    return (s32)script;
}

/* Message ids. */

/* Scene-step dispatcher for overlay resource 0x3b1.
 *
 * The owner takes a step selector plus two step parameters and jumps through a
 * 26-entry table into one bounded block of scene setup calls per step. Step 18
 * finishes by re-entering the dispatcher with step 14, which the compiler turns
 * into a jump back to the range check.
 *
 * Uncertain: the roles of the two parameters differ per step (actor slot,
 * count, flag, upper loop bound), so they keep neutral names here. The record
 * fields written at +6 (halfword) and +24 (word) are the same scene-object
 * fields the neighbouring scene sources touch; their meaning is not recovered.
 */

/*
 * Field scene beat for overlay resource_3b1.  Each callee is named for its own
 * call site: every call reaches its target through its own local veneer, even
 * where the same logical callee is used from more than one site.
 */
u8 *SceneData_SelectTableBySceneIndexAndFlags(void)
{
    s16 *tbl = ((s16 *)Data_02000240);
    s32 scene = tbl[225];

    switch (scene) {
    case 1:
    case 2:
        if (Engine_GameFlagIsSet(2208) != 0) {
            return FuneHeya_SceneTable07;
        }
        if (Engine_GameFlagIsSet(0x928) != 0 && Engine_GameFlagIsSet(0x93e) == 0) {
            return FuneHeya_SceneTable06;
        }
        return FuneHeya_SceneTable05;
    case 4:
    case 23:
        if (Engine_GameFlagIsSet(0x93e) != 0) {
            return FuneHeya_SceneTable14;
        }
        return FuneHeya_SceneTable11;
    case 5:
        if (Engine_GameFlagIsSet(2208) != 0) {
            return FuneHeya_SceneTable09;
        }
        if (Engine_GameFlagIsSet(0x93e) != 0) {
            return FuneHeya_SceneTable10;
        }
        return FuneHeya_SceneTable08;
    case 15:
    case 17:
    case 19:
        return FuneHeya_SceneTable12;
    case 21:
        return FuneHeya_SceneTable13;
    default:
        break;
    }

    return FuneHeya_SceneTable04;
}

void FieldScene_RunScene3b1_02000670(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Battle_ResetEffectCounterFar();
    if (Engine_GameFlagIsSet(0x921) != 0) {
        Engine_EventSetMessage((s32)MsgFuneIfShipFromTolbiHad);
        Engine_EventShowMessage(10, 0);
    } else {
        if (Engine_GameFlagIsSet(0x922) != 0) {
            Engine_EventSetMessage((s32)MsgFuneNowWantSeeCaptainToo);
            Engine_EventOpenMessage(10, 0);
            if (Engine_EventChooseYesNo(0, 0) == 0) {
                FieldScene_RunExtendedActorChoreography();
                goto L_020006ea;
            }
            Engine_ActorStartRepeatedMotion(10, 2);
            Engine_EventShowMessage(10, 0);
            Engine_ActorFaceDirection(10, 0xd000, 0);
        } else {
            Engine_EventSetMessage((s32)MsgFuneButWeCantSendShip);
            Engine_EventShowMessage(10, 0);
        }
    }
    L_020006ea:;
    Engine_EventEnd();
}

void SceneDialogue_RunActor12Line(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgFuneYouCameAskCaptainSet);
    Engine_EventAskYesNo(12, 0);
    Engine_EventEnd();
}

void FieldScene_RunScene3b1_02000728(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x928) != 0) {
        Engine_EventSetMessage((s32)MsgFuneOarsmanWasInjured);
        FieldScene_RunStepThen10(8);
        Engine_ActorFaceDirection(8, 0xd000, 60);
        Engine_ActorSetAnimationAndWait(8, 4);
        FieldScene_RunStepThen10(8);
        Engine_ActorSetAnimationAndWait(8, 3);
    } else if (Engine_GameFlagIsSet(0x925) != 0) {
        Engine_EventSetMessage((s32)MsgFuneWeDontKnowMightHappen);
        Engine_EventShowMessage(8, 0);
    } else if (Engine_GameFlagIsSet(0x921) != 0) {
        Engine_EventSetMessage((s32)MsgFuneBadLuckLosingMyLucky);
        Engine_EventShowMessage(8, 0);
        if (Engine_GameFlagIsSet(0x925) == 0 && Engine_GameFlagIsSet(0x924) != 0) {
            gEventWork->unknown_172 = 1;
        }
    } else {
        Engine_EventSetMessage((s32)MsgFuneItsTooLateHireMercenaries);
        Engine_EventShowMessage(8, 0);
    }
    Engine_EventEnd();
}

void FieldScene_RunScene3b1_020007f8(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x925) != 0) {
        Engine_ActorStartRepeatedMotion(8, 2);
        Engine_EventSetMessage((s32)MsgFuneTheseProudWarriorsNotGoing);
        FieldScene_RunStepThen10(8);
        Engine_ActorFaceActor(8, ACTOR_PARTY_LEADER, 10);
        Engine_EventOpenMessage(8, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventWait(40);
            FieldScene_RunStepThen10(8);
            FieldScene_CallPairWith10(8, 0x3000);
            Engine_EventShowMessage(8, 0);
            goto L_0200088e;
        }
        bump_step(2);
        Engine_EventShowMessage(8, 0);
        Engine_ActorFaceDirection(8, 0x3000, 0);
    } else {
        Engine_EventSetMessage((s32)MsgFuneLongerWeSitHereMore);
        Engine_EventShowMessage(8, 0);
    }
    L_0200088e:;
    Engine_EventEnd();
}

void SceneDialogue_ShowLine1E19Or1D50(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x925) != 0) {
        Engine_EventSetMessage((s32)MsgFuneYouGoingRow);
        Engine_EventAskYesNo(10, 0);
    } else {
        Engine_EventSetMessage((s32)MsgFuneTheresWholeBunchReallyMuscular);
        Engine_EventShowMessage(10, 0);
    }
    Engine_EventEnd();
}

/* Scene setup for resource_3b1: installs actors 10 through 17. */

/* The pool word, referenced by address so that it is emitted. */

/*
 * Actors 24 and 25 setup for overlay resource_3b1. Each callee slot uses
 * its own local veneer, so the names are per call site and not the shared
 * main-image symbol.
 */
void SceneState_RunFlagBranchedActor8Setup(void)
{
    if (Engine_GameFlagIsSet(0x300) != 0) {
        u8 *obj = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Engine_EventBegin();
        FuneHeya_TurnActorToOpenSide(obj);
        Engine_EventSetMessage((s32)MsgFuneLooksLikeYouveBeenChosen);
        FieldScene_RunStepThen10(8);
        Engine_ActorSetAnimation(obj, 2);
        p = (u8 *)Object_GetByIdFar(0);
        if (p != 0) {
            Engine_ActorSetDestination(obj, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Engine_ActorWaitForMove(obj);
        Engine_ActorSetPosition(obj, 0, 0);
        Engine_EventEnd();
    } else if (Engine_GameFlagIsSet(0x92b) != 0) {
        FieldScene_RunPrimarySequence(8, (s32)MsgFuneNoooooNotGoing, 0x990);
    } else if (Engine_GameFlagIsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(8, (s32)MsgFuneNoooooNotGoing, 0x917);
    } else if (Engine_GameFlagIsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(8, (s32)MsgFuneNoooooNotGoing, 0x935);
    } else {
        FieldScene_RunPrimarySequence(8, (s32)MsgFuneNoooooNotGoing, 0x92c);
    }
}

void SceneDialogue_RunActorTenFlaggedDialogue(void)
{
    if (Engine_GameFlagIsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Engine_EventBegin();
        FuneHeya_TurnActorToOpenSide(o);
        Engine_EventSetMessage((s32)MsgFuneHaHaHaChosenOarsman);
        FieldScene_RunStepThen10(10);
        Engine_ActorSetAnimation(o, 2);
        p = (u8 *)Object_GetByIdFar(0);
        if (p != 0) {
            Engine_ActorSetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Engine_ActorWaitForMove(o);
        Engine_ActorSetPosition(o, 0, 0);
        Engine_EventEnd();
    } else if (Engine_GameFlagIsSet(0x92b) != 0) {
        FieldScene_RunPrimarySequence(10, (s32)MsgFuneJokingWantRow, 0x992);
    } else if (Engine_GameFlagIsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(10, (s32)MsgFuneJokingWantRow, 0x919);
    } else if (Engine_GameFlagIsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(10, (s32)MsgFuneJokingWantRow, 0x937);
    } else {
        FieldScene_RunPrimarySequence(10, (s32)MsgFuneJokingWantRow, 0x92e);
    }
}

/* What actor 11 says aboard, by how far the choice of who rows has come. */

/* Once flag 0x8a0 is set actor 11 will not talk; once someone is picked
 * (flag 0x300) the picked actor walks back to the leader; otherwise actor 11
 * asks, and the flag its yes sets depends on which of 0x92b, 0x92a and 0x929
 * is set. */
void FieldScene_RunActor11FlagDialogue(void)
{
    if (Engine_GameFlagIsSet(0x8A0) != 0) {
        Engine_EventBegin();
        Engine_ActorSetAttachedEffect(11, 0x102);
        Engine_EventWait(40);
        Engine_EventSetMessage((s32)MsgFuneDontFeelDontTalkMe);
        Engine_EventShowMessage(11, 0);
        Engine_EventEnd();
    } else if (Engine_GameFlagIsSet(0x300) != 0) {
        s32 actor = SceneState_ApplyLevelFromFlags();
        u8 *leader;

        Engine_EventBegin();
        FuneHeya_TurnActorToOpenSide(actor);
        Engine_EventSetMessage((s32)MsgFuneYouFinallyPickedSomeoneDidnt);
        FieldScene_RunStepThen10(11);
        Engine_ActorSetAnimation(actor, 2);
        leader = (u8 *)Object_GetById(0);
        if (leader != 0) {
            Engine_ActorSetDestination(actor, *(s16 *)(leader + 10), *(s16 *)(leader + 18));
        }
        Engine_ActorWaitForMove(actor);
        Engine_ActorSetPosition(actor, 0, 0);
        Engine_EventEnd();
    } else if (Engine_GameFlagIsSet(0x92b) != 0) {
        FieldScene_RunPrimarySequence(11, (s32)MsgFuneDontTellGoing, 0x993);
    } else if (Engine_GameFlagIsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(11, (s32)MsgFuneDontTellGoing, 0x91a);
    } else if (Engine_GameFlagIsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(11, (s32)MsgFuneDontTellGoing, 0x938);
    } else {
        FieldScene_RunPrimarySequence(11, (s32)MsgFuneDontTellGoing, 0x92f);
    }
}

/* Message ids. */
void FieldScene_RunScene3b1SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x300) != 0) {
        rec7 = SceneState_ApplyLevelFromFlags();
        ((s32 (*)())FuneHeya_TurnActorToOpenSide)();
        Engine_EventSetMessage((s32)MsgFuneGivesMeChillsThinkCould);
        FieldScene_RunStepThen10(12);
        Engine_ActorSetAnimation(rec7, 2);
        record = (s32)Object_GetByIdFar(0);
        if (record != 0) {
            Engine_ActorSetDestination(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(rec7);
        Engine_ActorSetPosition(rec7, 0, 0);
    } else {
        Engine_ActorRunRepeatedMotion(12, 2);
        Engine_EventWait(20);
        Engine_EventSetMessage((s32)MsgFuneOhhhhNooooGoingMakeMe);
        Engine_EventOpenMessage(12, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            FieldScene_RunStepThen10(12);
            Engine_ActorSetAnimation(12, 2);
            record = (s32)Object_GetByIdFar(0);
            if (record != 0) {
                Engine_ActorSetDestination(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Engine_ActorWaitForMove(12);
            Engine_ActorSetPosition(12, 0, 0);
            Engine_GameFlagSet(0x300);
            if (Engine_GameFlagIsSet(0x92b) != 0) {
                Engine_GameFlagSet(0x994);
                goto L_02000c9a;
            }
            if (Engine_GameFlagIsSet(0x92a) != 0) {
                Engine_GameFlagSet(0x91b);
                goto L_02000c9a;
            }
            if (Engine_GameFlagIsSet(0x929) != 0) {
                Engine_GameFlagSet(0x939);
                goto L_02000c9a;
            }
            Engine_GameFlagSet(0x930);
        } else {
            bump_step(1);
            FieldScene_RunStepThen10(12);
        }
    }
    L_02000c9a:;
    Engine_EventEnd();
}

void FieldScene_RunScene3b1SequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x300) != 0) {
        rec7 = SceneState_ApplyLevelFromFlags();
        ((s32 (*)())FuneHeya_TurnActorToOpenSide)();
        Engine_EventSetMessage((s32)MsgFuneRobinYouveGotGoodEye);
        FieldScene_RunStepThen10(9);
        Engine_ActorSetAnimation(rec7, 2);
        record = (s32)Object_GetByIdFar(0);
        if (record != 0) {
            Engine_ActorSetDestination(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(rec7);
        Engine_ActorSetPosition(rec7, 0, 0);
    } else {
        Engine_EventSetMessage((s32)MsgFuneHaHaHaRowingFeel);
        Engine_EventShowMessageAndWait(9, 0, 60);
        Engine_ActorRunRepeatedMotion(9, 1);
        Engine_EventOpenMessage(9, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            FieldScene_RunStepThen10(9);
            Engine_ActorSetAnimation(9, 2);
            record = (s32)Object_GetByIdFar(0);
            if (record != 0) {
                Engine_ActorSetDestination(9, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Engine_ActorWaitForMove(9);
            Engine_ActorSetPosition(9, 0, 0);
            Engine_GameFlagSet(0x300);
            if (Engine_GameFlagIsSet(0x92b) != 0) {
                Engine_GameFlagSet(0x991);
                goto L_02000de0;
            }
            if (Engine_GameFlagIsSet(0x92a) != 0) {
                Engine_GameFlagSet(0x918);
                goto L_02000de0;
            }
            if (Engine_GameFlagIsSet(0x929) != 0) {
                Engine_GameFlagSet(0x936);
                goto L_02000de0;
            }
            Engine_GameFlagSet(0x92d);
        } else {
            bump_step(1);
            FieldScene_RunStepThen10(9);
        }
    }
    L_02000de0:;
    Engine_EventEnd();
}

void SceneDialogue_RunActorThirteenFlag300Branch(void)
{
    if (Engine_GameFlagIsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Engine_EventBegin();
        FuneHeya_TurnActorToOpenSide(o);
        Engine_EventSetMessage((s32)MsgFuneThatsWhoYouPickedOarsman);
        FieldScene_RunStepThen10(13);
        Engine_ActorSetAnimation(o, 2);
        p = (s32)Object_GetByIdFar(0);
        if (p != 0) {
            Engine_ActorSetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Engine_ActorWaitForMove(o);
        Engine_ActorSetPosition(o, 0, 0);
        Engine_EventEnd();
    } else if (Engine_GameFlagIsSet(0x92b) != 0) {
        FieldScene_RunPrimarySequence(13, (s32)MsgFuneNotThinkingMaking, 0x995);
    } else if (Engine_GameFlagIsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(13, (s32)MsgFuneNotThinkingMaking, 0x91c);
    } else if (Engine_GameFlagIsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(13, (s32)MsgFuneNotThinkingMaking, 0x93a);
    } else {
        FieldScene_RunPrimarySequence(13, (s32)MsgFuneNotThinkingMaking, 0x931);
    }
}

void FieldScene_RunFlag300BranchDialogue(void)
{
    if (Engine_GameFlagIsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Engine_EventBegin();
        FuneHeya_TurnActorToOpenSide(o);
        Engine_EventSetMessage((s32)MsgFuneIfYouveChosenOarsmanLets);
        FieldScene_RunStepThen10(14);
        Engine_ActorSetAnimation(o, 2);
        p = (s32)Object_GetByIdFar(0);
        if (p != 0) {
            Engine_ActorSetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Engine_ActorWaitForMove(o);
        Engine_ActorSetPosition(o, 0, 0);
        Engine_EventEnd();
    } else if (Engine_GameFlagIsSet(0x92b) != 0) {
        FieldScene_RunPrimarySequence(14, (s32)MsgFuneOarsmanGiveBreak, 0x996);
    } else if (Engine_GameFlagIsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(14, (s32)MsgFuneOarsmanGiveBreak, 0x91d);
    } else if (Engine_GameFlagIsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(14, (s32)MsgFuneOarsmanGiveBreak, 0x93b);
    } else {
        FieldScene_RunPrimarySequence(14, (s32)MsgFuneOarsmanGiveBreak, 0x932);
    }
}

void FieldScene_RunActor15FlagDialogue(void)
{
    if (Engine_GameFlagIsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Engine_EventBegin();
        FuneHeya_TurnActorToOpenSide(o);
        Engine_EventSetMessage((s32)MsgFuneYouveBeenChosenAsOarsman);
        FieldScene_RunStepThen10(15);
        Engine_ActorSetAnimation(o, 2);
        p = (s32)Object_GetByIdFar(0);
        if (p != 0) {
            Engine_ActorSetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Engine_ActorWaitForMove(o);
        Engine_ActorSetPosition(o, 0, 0);
        Engine_EventEnd();
    } else if (Engine_GameFlagIsSet(0x92b) != 0) {
        FieldScene_RunPrimarySequence(15, (s32)MsgFunePoorOarsmanFeel, 0x997);
    } else if (Engine_GameFlagIsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(15, (s32)MsgFunePoorOarsmanFeel, 0x91e);
    } else if (Engine_GameFlagIsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(15, (s32)MsgFunePoorOarsmanFeel, 0x93c);
    } else {
        FieldScene_RunPrimarySequence(15, (s32)MsgFunePoorOarsmanFeel, 0x933);
    }
}

void FieldScene_RunActor16FlagDialogue(void)
{
    s32 obj;
    u8 *actor;

    if (Engine_GameFlagIsSet(0x300) != 0) {
        obj = SceneState_ApplyLevelFromFlags();
        Engine_EventBegin();
        FuneHeya_TurnActorToOpenSide(obj);
        Engine_EventSetMessage((s32)MsgFuneHoHoPersonGoingGet);
        FieldScene_RunStepThen10(16);
        Engine_ActorSetAnimation(obj, 2);

        actor = (s32)Object_GetByIdFar(0);
        if (actor != 0) {
            Engine_ActorSetDestination(obj, *(s16 *)(actor + 10),
                          *(s16 *)(actor + 18));
        }

        Engine_ActorWaitForMove(obj);
        Engine_ActorSetPosition(obj, 0, 0);
        Engine_EventEnd();
    } else {
        if (Engine_GameFlagIsSet(0x92b) != 0) {
            FieldScene_RunPrimarySequence(16, (s32)MsgFunePeopleAskingFrail, 0x998);
        } else if (Engine_GameFlagIsSet(0x92a) != 0) {
            FieldScene_RunPrimarySequence(16, (s32)MsgFunePeopleAskingFrail, 0x91f);
        } else if (Engine_GameFlagIsSet(0x929) != 0) {
            FieldScene_RunPrimarySequence(16, (s32)MsgFunePeopleAskingFrail, 0x93d);
        } else {
            FieldScene_RunPrimarySequence(16, (s32)MsgFunePeopleAskingFrail, 0x934);
        }
    }
}

struct FieldActor *FindActorNearPosition(s32 x, s32 y)
{
    struct EventWork *work;
    struct FieldActor **actor;
    struct FieldActor *current;
    u32 i;
    s32 actor_x;
    s32 actor_y;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;

    work = gEventWork;
    i = 8;
    left = x - 12;
    right = x + 12;
    top = y - 12;
    bottom = y + 12;
    actor = work->placed_actors;
    while (i <= 65) {
        current = *actor++;
        actor_x = current->x.part.pixel;
        actor_y = current->z.part.pixel;
        if (left < actor_x && right > actor_x &&
            top < actor_y && bottom > actor_y)
            return current;
        i++;
    }
    return 0;
}

void FuneHeya_TurnActorToOpenSide(s32 a0)
{
    s32 p10;
    s32 rec4;
    u8 *record;
    s32 none;
    s32 v8;
    s32 v6;

    rec4 = (s32)Object_GetById(0);
    v8 = 1;
    ObjectMotion_SetActionVariant(a0, 2);
    {
        u8 *record = (u8 *)Object_GetById(a0);
        /* FAKEMATCH: the flag byte is read through a volatile access. */
        u8 value = *(volatile u8 *)&record[35];
    
        record[35] = (u8)(value | 1);
    }
    v6 = (((*(u16 *)(rec4 + 6) + 0x4000) & 0xf000) >> 12);
    if (SceneActor_CheckBucketOffsetPoint((((*(u16 *)(rec4 + 6) + 0x4000) & 0xf000) >> 12)) != 0) {
        none = 0;
        v8 = none;
    }
    if (v8 != 0) {
        v6 = (((*(u16 *)(rec4 + 6) + -0x4000) & 0xf000) >> 12);
        if (SceneActor_CheckBucketOffsetPoint((((*(u16 *)(rec4 + 6) + -0x4000) & 0xf000) >> 12)) != 0) {
            none = 0;
            v8 = none;
        }
        if (v8 != 0) {
            v6 = (((*(u16 *)(rec4 + 6) + 0x8000) & 0xf000) >> 12);
        }
    }
    record = (s32)Object_GetById(0);
    if ((s32)record != 0) {
        Engine_ActorSetPosition(a0, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
    }
    Call3(Engine_ActorSetSpeed, a0, 0x19999, 0xcccc);
    Engine_ActorSetAnimation(a0, 2);
    {
        s32 w = FuneHeya_TurnSteps[v6];

        Engine_ActorSetDestinationOffset(a0, w >> 16, (w << 16) >> 16);
    }
    Engine_ActorWaitForMove(a0);
    Engine_ActorSetAnimation(a0, 1);
    FieldScene_CallPairWith10(a0, *(u16 *)(rec4 + 6));
    p10 = a0;
}
