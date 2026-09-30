#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "HEYA.H"

#include "STAGED_ACTOR.H"
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
extern const s32 FuneHeya_CellStepsA[];
extern const s32 FuneHeya_CellStepsB[];
extern const s32 FuneHeya_CellStepsC[];
extern const s32 FuneHeya_CellStepsD[];
void ObjectMotion_SetHorizontalPositionWithTerrain();
void *Object_GetByIdFar();
void FuneHeya_PlaceFoundActors();
void FuneHeya_PoseSceneActor();
void FuneHeya_PlaceAnchorCharm();
void ObjectMotion_WaitForAnimationChange();
void ObjectMotion_ArmCallback();
void Object_SetActionById();
void Event_WaitValue1c8FramesFar();
void Object_SetModeById();
void Battle_WaitMode0();
void Event_CallWithLastActiveObjectId();
u8 *Battle_GetWorkObject1e0();

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

/* The halfword field at +6 of a scene record is written from an int-sized
 * value; storing a plain constant through the cast would make the compiler
 * fetch a halfword literal from the pool instead. */
static __inline__ void SetPose(u8 *rec, s32 pose)
{
    *(u16 *)(rec + 6) = pose;
}

/*
 * Both callees live inside this overlay and are declared without a prototype,
 * so each call site fixes its own arity.
 */

void FieldScene_RunStepThen10(s32 a)
{
    Event_ShowMessage(a, 0);
    Event_Wait(10);
}

void FieldScene_CallPairWith10(s32 a, u16 b)
{
    Actor_FaceDirection(a, b, 10);
}

void OverlayObject_SetPositionAndHeading(void *a, s32 b, s32 c, s32 d)
{
    ObjectMotion_SetHorizontalPositionWithTerrain(a, b << 16, c << 16, d);
    *(s16 *)((u8 *)Object_GetByIdFar(a) + 6) = d;
}

void ConfigureSceneMotionFlags(s32 x, s32 y, s32 z, u32 flags)
{
    u32 selected;

    Camera_MoveTo(x, y, z, ~flags & 1);
    selected = flags & 0x1111;
    if ((flags & 0x10000000) != 0)
        Camera_WaitForMove();
    if ((flags & 0x01000000) != 0)
        Map_Redraw();
    Event_Wait(selected);
}

void FieldScene_RunSceneStep(s32 step, u32 arg, u32 opt)
{
    extern const s32 FuneHeya_ActionScriptE[];

    u32 slot;

    switch (step) {
    case 0:
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
        ObjectMotion_ArmCallback(2, 0, 0);
        Actor_FaceDirection(ACTOR_MIA, 0x8000, opt);
        break;
    case 1:
        Actor_FaceDirection(ACTOR_PARTY_LEADER, arg, 0);
        Actor_FaceDirection(ACTOR_GERALD, arg, 0);
        Actor_FaceDirection(ACTOR_IVAN, arg, 0);
        Actor_FaceDirection(ACTOR_MIA, arg, opt);
        break;
    case 2:
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimation(ACTOR_GERALD, 3);
        Actor_SetAnimation(ACTOR_IVAN, 3);
        Actor_SetAnimation(ACTOR_MIA, 3);
        if (arg != 0) {
            ObjectMotion_WaitForAnimationChange(3);
        }
        if (opt == 0) {
            break;
        }
        Event_Wait(opt);
        break;
    case 3:
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
        Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
        Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
        Event_Wait(opt);
        break;
    case 4:
        for (slot = 0; slot < arg; slot++) {
            Actor_SetPosition(slot + 10, 0, 0);
        }
        break;
    case 5:
        {
            u8 *rec;

            rec = Object_GetByIdFar(arg);
            SetPose(rec, 0x5000);
        }
        Actor_SetAnimation(arg, 5);
        Object_SetActionById(arg, opt);
        break;
    case 6:
        {
            u8 *rec;

            rec = Object_GetByIdFar(arg);
            SetPose(rec, 0x5000);
            *(s32 *)(rec + 24) = -0x10000;
        }
        Actor_SetAnimation(arg, 5);
        Object_SetActionById(arg, opt);
        break;
    case 7:
        {
            u8 *rec;

            rec = Object_GetByIdFar(arg);
            SetPose(rec, 0x5000);
        }
        FuneHeya_PoseSceneActor(arg);
        if (opt == 0) {
            Object_SetActionById(arg, 0);
        }
        break;
    case 8:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
        Event_OpenScreen();
        if (arg != 0) {
            Event_WaitValue1c8FramesFar();
        }
        Event_Wait(0);
        break;
    case 9:
        Event_CloseScreen();
        Event_WaitForScreen();
        if (arg == 0) {
            break;
        }
        Event_RequestExit(arg);
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
            u8 *rec;

            Actor_SetAnimation(13, 1);
            rec = Object_GetByIdFar(13);
            SetPose(rec, 0x3000);
            rec = Object_GetByIdFar(13);
            *(s32 *)(rec + 24) = 0x10000;
        }
        {
            u8 *rec;

            Actor_SetAnimation(14, 1);
            rec = Object_GetByIdFar(14);
            SetPose(rec, 0x5000);
            Actor_SetAnimation(15, 1);
            rec = Object_GetByIdFar(15);
            SetPose(rec, 0x3000);
            rec = Object_GetByIdFar(15);
            *(s32 *)(rec + 24) = 0x10000;
            Actor_SetAnimation(16, 1);
            rec = Object_GetByIdFar(16);
            SetPose(rec, 0x5000);
            Object_SetModeById(17, 1);
            rec = Object_GetByIdFar(17);
            SetPose(rec, 0x3000);
            rec = Object_GetByIdFar(17);
            *(s32 *)(rec + 24) = 0x10000;
        }
        Actor_SetPosition(28, 0x19a0000, 0xae0000);
        Actor_SetPosition(29, 0x1d60000, 0xae0000);
        Actor_SetPosition(30, 0x19a0000, 0xce0000);
        Actor_SetPosition(31, 0x1d60000, 0xce0000);
        Actor_SetPosition(32, 0x19a0000, 0x11e0000);
        Actor_SetPosition(33, 0x1d60000, 0x11e0000);
        Actor_SetPosition(34, 0x19a0000, 0x13c0000);
        Actor_SetPosition(35, 0x1d60000, 0x13c0000);
        Task_Wait(1);
        if (arg != 0) {
            Actor_FaceDirection(13, 0xb000, 0);
        }
        Call3(ObjectMotion_ArmCallback, 14, 0xd000, 0);
        Actor_FaceDirection(15, 0xb000, 0);
        Actor_FaceDirection(16, 0xd000, 0);
        FieldScene_CallPairWith10(17, 0xb000);
        break;
    case 12:
        {
            u8 *rec;

            rec = Object_GetByIdFar(arg);
            Actor_SetAnimation(arg, 1);
            if (opt != 0) {
                SetPose(rec, 0x3000);
            } else {
                SetPose(rec, 0x5000);
            }
            *(s32 *)(rec + 24) = 0x10000;
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
        Event_OpenScreen();
        Event_WaitForScreen();
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
        Task_Wait(1);
        FieldScene_RunSceneStep(23, 0, 0);
        Actor_EnableActionCallback(12, FuneHeya_ActionScriptE);
        break;
    case 23:
        Actor_Destroy(ACTOR_GERALD);
        Actor_Destroy(ACTOR_IVAN);
        Actor_Destroy(ACTOR_MIA);
        break;
    case 24:
        Camera_MoveTo(-1, -1, -1, 0);
        Task_Wait(1);
        if (arg != 0) {
            *(u8 *)(Battle_GetWorkObject1e0() + 0x55) = 0;
        }
        break;
    case 25:
        Event_CallWithLastActiveObjectId(FuneHeya_CellStepsA);
        Task_Wait(1);
        if (arg == 1) {
            Event_CallWithLastActiveObjectId(FuneHeya_CellStepsB);
            Task_Wait(1);
        } else if (arg == 2) {
            Event_CallWithLastActiveObjectId(FuneHeya_CellStepsC);
            Task_Wait(1);
        } else if (arg == 3) {
            Event_CallWithLastActiveObjectId(FuneHeya_CellStepsD);
            Task_Wait(1);
        }
        break;
    }
}
