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
extern u32 FuneHeya_TurnSteps[];
s32 Object_GetByIdFar();
s32 Object_CheckMovementCollision();
s32 FuneHeya_FindFirstSetFlag();

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

struct FieldActor *FindActorNearPosition(s32 x, s32 y);

/* Actor placement check for resource_3b1. */

/* Bucket offsets, packed as {s16 hi; s16 lo} per entry. */

/*
 * Offset obj+10 and obj+18 by the bucket's packed hi/lo pair, test the
 * candidate point, and on success pack {x << 16, obj+12, z << 16} into a
 * stack struct for a second check.  Returns 1 only if both checks pass.  The
 * owner includes its one pool word, the bucket table base.  Callees are named
 * by the address their call site computes, not by a runtime address.
 */
s32 SceneActor_CheckBucketOffsetPoint(s32 bucket)
{
    u8 *obj = Object_GetByIdFar(0);
    u32 ofs = FuneHeya_TurnSteps[bucket];
    s32 x = *(s16 *)(obj + 10) + ((s32)ofs >> 16);
    s32 z = *(s16 *)(obj + 18) + (s32)(s16)ofs;

    if (FindActorNearPosition(x, z) != 0) {
        return 0;
    }

    {
        s32 point[3];
        point[0] = x << 16;
        point[1] = *(s32 *)(obj + 12);
        point[2] = z << 16;

        if (Object_CheckMovementCollision(obj, point) != 0) {
            return 0;
        }
    }

    return 1;
}

/*
 * Level selection from scene flags, overlay resource_3b1. Each callee name
 * refers to its own call word rather than to a shared runtime address.
 */

/*
 * Actor 8 flag setup for overlay resource_3b1. Each callee name refers to
 * its own call word rather than to a shared runtime address.
 */

/* Scene state helper for overlay resource_3b1. */

/*
 * Picks a level from the highest flag that is set and applies it. The
 * 72-byte owner at 0x020012dc includes its three pool words, which are the
 * addresses taken as Value_0000092b, Value_0000092a and Value_00000929.
 */
s32 SceneState_ApplyLevelFromFlags(void)
{
    s32 ret = 0;

    if (GameFlag_IsSet(0x92b) != 0) {
        ret = 3;
    } else if (GameFlag_IsSet(0x92a) != 0) {
        ret = 2;
    } else if (GameFlag_IsSet(0x929) != 0) {
        ret = 1;
    }

    return FuneHeya_FindFirstSetFlag(ret, 1);
}

/* Story selector owner at 0x02001324, 84 bytes; eight calls. Per-site call
 * veneers (raw asm confirms each callee slot uses a distinct local stub
 * even across the three near-identical "twin" owners at 0x1324/1378/13cc). */
void SceneDialogue_ShowLine1ECETo1ED0(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92c)) Event_SetMessage(MSG_THING_HAS_KAJA_HIS_MEN);
    else if (GameFlag_IsSet(0x935)) Event_SetMessage(MSG_MONSTERS_EVERYWHERE_IM_STUCK_ROWING);
    else Event_SetMessage(MSG_SHIP_STARTING_LIST_IF_WE);
    Event_ShowMessage(0x12, 0); Event_End();
}

/* Story selector owner at 0x02001378, 84 bytes; eight calls. Per-site call
 * veneers (twin of 0x1324/0x13cc with distinct local stub addresses). */
void SceneDialogue_RunActor19TwoFlagLineA(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92d)) Event_SetMessage(MSG_THING_HAS_KAJA_HIS_MEN);
    else if (GameFlag_IsSet(0x936)) Event_SetMessage(MSG_MONSTERS_EVERYWHERE_IM_STUCK_ROWING);
    else Event_SetMessage(MSG_SHIP_STARTING_LIST_IF_WE);
    Event_ShowMessage(0x13, 0); Event_End();
}

/* Story selector owner at 0x020013cc, 84 bytes; eight calls. Per-site call
 * veneers (twin of 0x1324/0x1378 with distinct local stub addresses). */
void SceneDialogue_RunActor20TwoFlagLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92e)) Event_SetMessage(MSG_THING_HAS_KAJA_HIS_MEN);
    else if (GameFlag_IsSet(0x937)) Event_SetMessage(MSG_MONSTERS_EVERYWHERE_IM_STUCK_ROWING);
    else Event_SetMessage(MSG_SHIP_STARTING_LIST_IF_WE);
    Event_ShowMessage(0x14, 0); Event_End();
}

/* Story selector owner at 0x02001420, 60 bytes; six calls. */
void SceneDialogue_ShowLine1ED1Or1ED2(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92f)) Event_SetMessage(MSG_HOW_MANY_MONSTERS_OUT_THERE);
    else Event_SetMessage(MSG_ANOTHER_MONSTER_ISNT_FIRST_CLASS);
    Event_ShowMessage(21, 0); Event_End();
}

/* Story selector owner at 0x0200145c, 84 bytes; eight calls. */
void SceneDialogue_RunActor22TwoFlagLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x930)) Event_SetMessage(MSG_THING_HAS_KAJA_HIS_MEN);
    else if (GameFlag_IsSet(0x939)) Event_SetMessage(MSG_MONSTERS_EVERYWHERE_IM_STUCK_ROWING);
    else Event_SetMessage(MSG_SHIP_STARTING_LIST_IF_WE);
    Event_ShowMessage(22, 0); Event_End();
}

/* Story selector owner at 0x020014b0, 84 bytes; eight calls. */
void SceneDialogue_RunActor23BranchedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x931)) Event_SetMessage(MSG_THING_HAS_KAJA_HIS_MEN);
    else if (GameFlag_IsSet(0x93a)) Event_SetMessage(MSG_MONSTERS_EVERYWHERE_IM_STUCK_ROWING);
    else Event_SetMessage(MSG_SHIP_STARTING_LIST_IF_WE);
    Event_ShowMessage(23, 0); Event_End();
}

/* Story selector owner at 0x02001504, 84 bytes; eight calls. */
void SceneDialogue_RunActor24BranchedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x932)) Event_SetMessage(MSG_THING_HAS_KAJA_HIS_MEN);
    else if (GameFlag_IsSet(0x93b)) Event_SetMessage(MSG_MONSTERS_EVERYWHERE_IM_STUCK_ROWING);
    else Event_SetMessage(MSG_SHIP_STARTING_LIST_IF_WE);
    Event_ShowMessage(24, 0); Event_End();
}

/* Story selector owner at 0x02001558, 60 bytes; six calls. */
void SceneDialogue_RunActor25FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x933)) Event_SetMessage(MSG_HOW_MANY_MONSTERS_OUT_THERE);
    else Event_SetMessage(MSG_ANOTHER_MONSTER_ISNT_FIRST_CLASS);
    Event_ShowMessage(25, 0); Event_End();
}

/* Second-phase story selector at 0x02001594, 84 bytes; eight calls. */
void SceneDialogue_RunActor18TwoFlagLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92c)) Event_SetMessage(MSG_DONT_CARE_TAKES_JUST_HURRY);
    else if (GameFlag_IsSet(0x935)) Event_SetMessage(MSG_IF_THOSE_MONSTERS_COME_BACK);
    else Event_SetMessage(MSG_BOATS_ROCKING_MUCH_IM_CERTAIN);
    Event_ShowMessage(18, 0); Event_End();
}

/* Second-phase story selector at 0x020015e8, 84 bytes; eight calls. */
void SceneDialogue_RunActor19TwoFlagLineB(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92d)) Event_SetMessage(MSG_DONT_CARE_TAKES_JUST_HURRY);
    else if (GameFlag_IsSet(0x936)) Event_SetMessage(MSG_IF_THOSE_MONSTERS_COME_BACK);
    else Event_SetMessage(MSG_BOATS_ROCKING_MUCH_IM_CERTAIN);
    Event_ShowMessage(19, 0); Event_End();
}

/* Second-phase story selector at 0x0200163c, 84 bytes; eight calls. */
void SceneDialogue_ShowLine1EDBTo1EDDActor20(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92e)) Event_SetMessage(MSG_DONT_CARE_TAKES_JUST_HURRY);
    else if (GameFlag_IsSet(0x937)) Event_SetMessage(MSG_IF_THOSE_MONSTERS_COME_BACK);
    else Event_SetMessage(MSG_BOATS_ROCKING_MUCH_IM_CERTAIN);
    Event_ShowMessage(20, 0); Event_End();
}

/* Second-phase story selector at 0x02001690, 60 bytes; six calls. */
void SceneDialogue_RunActor21FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x92f)) Event_SetMessage(MSG_HAD_IDEA_THERE_WERE_MANY);
    else Event_SetMessage(MSG_WERE_SURROUNDED_BY_MONSTERS_STILL);
    Event_ShowMessage(21, 0); Event_End();
}

/* Second-phase story selector at 0x020016cc, 84 bytes; eight calls. */
void SceneDialogue_RunActor22BranchedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x930)) Event_SetMessage(MSG_DONT_CARE_TAKES_JUST_HURRY);
    else if (GameFlag_IsSet(0x939)) Event_SetMessage(MSG_IF_THOSE_MONSTERS_COME_BACK);
    else Event_SetMessage(MSG_BOATS_ROCKING_MUCH_IM_CERTAIN);
    Event_ShowMessage(22, 0); Event_End();
}

/* Second-phase story selector at 0x02001720, 84 bytes; eight calls. */
void SceneDialogue_ShowLine1EDBTo1EDDActor23(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x931)) Event_SetMessage(MSG_DONT_CARE_TAKES_JUST_HURRY);
    else if (GameFlag_IsSet(0x93a)) Event_SetMessage(MSG_IF_THOSE_MONSTERS_COME_BACK);
    else Event_SetMessage(MSG_BOATS_ROCKING_MUCH_IM_CERTAIN);
    Event_ShowMessage(23, 0); Event_End();
}

/* Second-phase story selector at 0x02001774, 84 bytes; eight calls. */
void SceneDialogue_ShowLine1EDBTo1EDDActor24(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x932)) Event_SetMessage(MSG_DONT_CARE_TAKES_JUST_HURRY);
    else if (GameFlag_IsSet(0x93b)) Event_SetMessage(MSG_IF_THOSE_MONSTERS_COME_BACK);
    else Event_SetMessage(MSG_BOATS_ROCKING_MUCH_IM_CERTAIN);
    Event_ShowMessage(24, 0); Event_End();
}

/* Second-phase story selector at 0x020017c8, 60 bytes; six calls. */
void SceneDialogue_ShowLine1EDEOr1EDF(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x933)) Event_SetMessage(MSG_HAD_IDEA_THERE_WERE_MANY);
    else Event_SetMessage(MSG_WERE_SURROUNDED_BY_MONSTERS_STILL);
    Event_ShowMessage(25, 0); Event_End();
}

void FieldScene_RunPrimarySequence(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(a1);
    Event_OpenMessage(a0, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        FieldScene_RunStepThen10(a0);
        Actor_SetAnimation(a0, 2);
        record = Value1(Object_GetByIdFar, 0);
        if (record != 0) {
            Actor_SetDestination(a0, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(a0);
        Actor_SetPosition(a0, 0, 0);
        GameFlag_Set(0x300);
        GameFlag_Set(a2);
    } else {
        bump_step(1);
        FieldScene_RunStepThen10(a0);
    }
    Event_End();
}

void FieldScene_RunScene3b1SequenceC(void)
{
    struct FieldActor *leader;

    leader = (struct FieldActor *)Object_GetByIdFar(0);
    if ((u16)(leader->facing - 0x2000) > 0xc000) {
        if (GameFlag_IsSet(0x928) != 0 && GameFlag_IsSet(0x93e) == 0) {
            Sanctum_Open(17);
        } else {
            Sanctum_Open(15);
        }
    } else {
        Event_Begin();
        if (GameFlag_IsSet(0x93e) != 0) {
            Event_SetMessage(MSG_GOOD_SHIP_HAS_ARRIVED_SAFELY);
        } else if (GameFlag_IsSet(0x8a0) != 0) {
            Event_SetMessage(MSG_HATE_ARGUING);
        } else if (GameFlag_IsSet(0x928) != 0) {
            Event_SetMessage(MSG_SHIPS_CREW_READY_FOR_ANYTHING_2);
        } else if (GameFlag_IsSet(0x925) != 0) {
            Event_SetMessage(MSG_SHIPS_CREW_READY_FOR_ANYTHING);
        } else {
            Event_SetMessage(MSG_IM_SPREADING_GOODWILL_WHEREVER_TRAVEL);
        }
        if (GameFlag_IsSet(0x928) != 0 && GameFlag_IsSet(0x93e) == 0) {
            Event_ShowMessage(17, 0);
        } else {
            Event_ShowMessage(15, 0);
        }
        Event_End();
    }
}
