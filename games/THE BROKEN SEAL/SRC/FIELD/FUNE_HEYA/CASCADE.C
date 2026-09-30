#include "TYPES.H"
#include "FIELD_EVENT.H"

extern const s32 FuneHeya_ActionScriptE[];
extern const s32 FuneHeya_ActionScriptF[];
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR.H"

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
u8 *Object_GetByIdFar();
void OverlayObject_SetPositionAndHeading();
void FieldScene_RunSceneStep();
void Scene_RunConditionalActorPresentation();

void SceneState_RunFlagGatedSetupCascade(void)
{
    if (GameFlag_IsSet(0x93e) != 0) {
        Actor_SetPosition(8, 0, 0);
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(10, 0, 0);
        Actor_SetPosition(11, 0, 0);
        Actor_SetPosition(12, 0, 0);
        FieldScene_RunSceneStep(14, 0, 0);
        return;
    }

    if (GameFlag_IsSet(0x8a0) != 0) {
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
        s32 t = GameFlag_IsSet(0x928);
        if (t != 0) {
            SceneState_ApplyActor8FourFlags(t);
            return;
        }
    }

    if (GameFlag_IsSet(0x925) != 0) {
        FieldScene_RunSceneStep(18, 0, 0);
        return;
    }

    if (GameFlag_IsSet(0x911) != 0 &&
        GameFlag_IsSet(0x922) != 0) {
        FieldScene_RunSceneStep(14, 0, 0);
        Actor_SetPosition(12, 0, 0);
    }
}

void RunSceneSelectionChain(void)
{
    Task_Wait(1);
    SceneActor_SetFlagBit3ForActors28To35();
    if (GameFlag_IsSet(2366) != 0) {
        FieldScene_RunSceneStep(4, 4, 0);
        OverlayObject_SetPositionAndHeading(8, 412, 222, 12288);
        OverlayObject_SetPositionAndHeading(9, 458, 161, 32768);
    } else {
        if (GameFlag_IsSet(2208) != 0) {
            Actor_SetPosition(8, 30932992, 9961472);
            Actor_SetAnimation(9, 5);
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
                            Actor_SetAnimation(9, 5);
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
        u8 *obj = Object_GetByIdFar(i);
        u32 v = obj[0x59];
        obj[0x59] = (u8)((v | bit) | zero);
    }
}
