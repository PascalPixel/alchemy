#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "HEYA.H"

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
extern u8 FuneHeya_EntryActionScript[];
s32 FuneHeya_FindFirstSetFlag();
void FieldScene_RunPositionTransferPresentation();

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

void FieldScene_CallPairWith10(s32 a, u16 b);

void SceneActor_SetFlagBit3ForActors28To35(void);
void FieldScene_RunSceneStep(s32 step, u32 arg, u32 opt);

void SceneState_ApplyActor8FourFlags(void)
{
    SceneActor_RunFirstMatchingSlot(8, 0x92c);
    SceneActor_RunFirstMatchingSlot(8, 0x935);
    SceneActor_RunFirstMatchingSlot(8, 0x917);
    SceneActor_RunFirstMatchingSlot(8, 0x990);
}

/*
 * Scan slots 0 through 8 inclusive.  On the first a1 that Engine_GameFlagIsSet
 * accepts, call Engine_ActorSetPosition and stop.  a0 and a1 advance together.
 */
void SceneActor_RunFirstMatchingSlot(s32 a0, s32 a1)
{
    unsigned int i = 0;

    do {
        if (GameFlag_IsSet(a1)!= 0) {
            Actor_SetPosition(a0, 0, 0);
            break;
        }
        i++;
        a0++;
        a1++;
    } while (i <= 8);
}

void FieldScene_RunScene3b1_02005068(void)
{
    extern u8 FuneHeya_ActionScriptE[];
    u32 i;
    s32 rec8;
    s32 record;
    s32 base5_200e840;
    s32 base5_200e8e4;

    rec8 = Value2(FuneHeya_FindFirstSetFlag, 0, 0);
    Event_Begin();
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 2, 0);
    SceneActor_SetFlagBit3ForActors28To35();
    Value3(FieldScene_RunSceneStep, 19, rec8, 12);
    Actor_SetAnimation(10, 6);
    base5_200e840 = (s32)FuneHeya_ActionScriptE;
    Actor_EnableActionCallback(rec8, base5_200e840);
    Actor_Destroy(11);
    Value2(Engine_ActorEnableActionCallback, 12, base5_200e840);
    base5_200e8e4 = (s32)FuneHeya_EntryActionScript;
    Actor_EnableActionCallback(36, base5_200e8e4);
    Actor_EnableActionCallback(37, base5_200e8e4);
    FieldScene_RunPositionTransferPresentation();
    Event_End();
}
