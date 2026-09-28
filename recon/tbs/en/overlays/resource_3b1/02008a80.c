/* Draft: FieldScene_RunActor11FlagDialogue, resource_3b1 at 0x02008a80 (listing 0x02000a80).
 * Not linked: its flag and message ids load from the literal pool as link-time constants (Value_0000092b, Value_00001e7e and siblings) that the main image does not define; spelled as plain constants GCC derives one from another and the owner comes out 4 bytes short.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
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
extern u8 LinkedMessage_DontFeelDontTalkMe;
extern u8 LinkedMessage_YouFinallyPickedSomeoneDidnt;
extern u8 Value_0000092b;
extern u8 Value_00001e7e;
extern u8 Value_00000993;
extern u8 Value_0000092a;
extern u8 Value_0000091a;
extern u8 Value_00000929;
extern u8 Value_00000938;
extern u8 Value_0000092f;
void Func_02001c60(u8 *o);
u8 *Func_02006fd2(s32 n);

void FieldScene_RunActor11FlagDialogue(void)
{
    if (GameFlag_IsSet(0x8A0) != 0) {
        Event_Begin();
        Actor_SetAttachedEffect(11, 0x102);
        Event_Wait(40);
        Event_SetMessage((s32)&LinkedMessage_DontFeelDontTalkMe);
        Event_ShowMessage(11, 0);
        Event_End();
    } else if (GameFlag_IsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        Func_02001c60(o);
        Event_SetMessage((s32)&LinkedMessage_YouFinallyPickedSomeoneDidnt);
        FieldScene_RunStepThen10(11);
        Actor_SetAnimation(o, 2);
        p = Func_02006fd2(0);
        if (p != 0) {
            Actor_SetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Actor_WaitForMove(o);
        Actor_SetPosition(o, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet((s32)&Value_0000092b) != 0) {
        FieldScene_RunPrimarySequence(11, (s32)&Value_00001e7e, (s32)&Value_00000993);
    } else if (GameFlag_IsSet((s32)&Value_0000092a) != 0) {
        FieldScene_RunPrimarySequence(11, (s32)&Value_00001e7e, (s32)&Value_0000091a);
    } else if (GameFlag_IsSet((s32)&Value_00000929) != 0) {
        FieldScene_RunPrimarySequence(11, (s32)&Value_00001e7e, (s32)&Value_00000938);
    } else {
        FieldScene_RunPrimarySequence(11, (s32)&Value_00001e7e, (s32)&Value_0000092f);
    }
}
