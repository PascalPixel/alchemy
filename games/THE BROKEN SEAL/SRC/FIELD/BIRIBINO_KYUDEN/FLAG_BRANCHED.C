#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 LinkedMessage_TooYoungForTheJob;

enum {
    FLAG_REWARD_TAKEN = 0x84e
};

extern u8 Data_0200a5c0[];
extern u8 Data_03001ebc[];
s32 Func_02002e68();
s32 Func_02002fae();
s32 Func_02002fc2();
s32 Func_02002fe0();
void Func_0200311c();
s32 Func_020031f6();
s32 Func_02003216();
void Func_0200323c();
s32 Func_02003240();
s32 Func_0200330a();
s32 Func_0200331e();
s32 Func_0200333c();
void Func_02003438();
s32 Func_0200348a();
s32 Func_02003496();
s32 Func_020034a4();
void Func_0200353a();
void Func_020035fc();
void Func_02003786();
void Func_020037f0();
s32 Func_020038ba();
void Func_0200392c();
void Func_02003af2();
void Func_02003afc();
void Func_02003b06();
void Func_02003b10();
s32 Func_02003b20();
s32 Func_02003b7e();
s32 Func_02003ba8();
void Func_02003baa_b();
void Func_02003bd4();

/* FAKEMATCH: Calls through these inline helpers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void AdvanceMessage(s32 amount)
{
    gEventWork->message += amount;
}

void Func_02003c20();
void Func_02003c26();
void Func_02003c44();
void Func_02003c60();
void Func_02003c66();
void Func_02003d84();

#define NULL ((void *)0)
#define CalculateFacingAngle Func_02002462

#include "FACING_OBJECT.H"

s32 Func_02002462(s32, s32);

struct SceneRecord {
    u8 unk_000[166];
    u8 field_166;
    u8 unk_167[23];
    u8 field_190;
    u8 unk_191[23];
    u8 field_214;
    u8 unk_215[23];
    u8 field_238;
};

/* Scene selector and table getters for resource_38d. The two records live in
 * the overlay image, which is writable, so the stores below land in it. */
extern s16 Data_02000240[];     /* Shared scene workspace, not overlay data. */
extern u8 Value_00000021;
extern u8 Data_0200a9b4[];      /* In-image record. */
extern u8 Data_0200a99c[];      /* In-image record. */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
void Func_02002536();           /* Record hand-off. */

extern u8 Data_0200aca8[];
extern u8 Data_0200ac9c[];
extern u8 LinkedMessage_YouWillingGoKolimaForest;

s32 Func_02002ab4(s32, s32, s32);
/* 0x02002996 serves two imports: the two-argument reader that guards the
 * branch and the one-argument flag clear inside its first arm. */

s32 Func_02002878(void);
void Func_02000872(s32);
void Func_0200089c(s32);
void Func_020008c8(s32);
void Func_020008f4(s32);

static __inline__ void FaceActor(s32 actor, s32 direction, s32 frames)
{
    Actor_FaceDirection(actor, direction, frames);
}

s32 Func_020029c8_scene_primary_script_head();

extern u8 Data_0200a5d4[];

void Func_02003748_channels();
void *Func_02003f58();
void *Func_02003fb0();
void *Func_02003fc4();
void *Func_02003fd8();
void Func_020040ac();
void *Func_020044de();
void *Func_020044fe();
void *Func_0200451e();
void Func_0200460e();

enum FlagBranchedMessage {
    MSG_DO_THINK_THESE_BILIBINS_GREAT = 0x1432,
    MSG_WE_REALLY_GIVING_OUR_TREASURE = 0x1434,
    MSG_GOOD_TREASURE_IF_GET_TURNED = 0x1436,
    MSG_YEHVE_HAD_CHANGE_HEART_YEH = 0x1442,
    MSG_WEVE_BROUGHT_WARRIORS_MILORD = 0x1728,
    MSG_HUMBLY_THANK = 0x1737,
    MSG_NAE_YEH_DINNAE_NEED_TAE = 0x1738,
    MSG_WAS_BUT_WORRIED_YEH_MIGHT = 0x1739,
    MSG_PLEASE_TAKE_YOUR_REWARD_BEFORE = 0x1748,
    MSG_ALWAYS_WELCOME_IN_PALACE_LORD = 0x1749,
    MSG_MAY_CHOOSE_ONLY_ONE_ITEM = 0x174b,
    MSG_NONETHELESS_IF_YOUR_LUCK_SOUR = 0x174c,
    MSG_ROBIN_CHECKED_CHEST_BUT_WAS = 0x174d,
    MSG_TREASURE_CHEST_LOCKED = 0x174e,
    MSG_FINE_WARRIOR = 0x1750,
    MSG_SOMETIMES_WE_NEED_CHILDREN_REMIND = 0x1756,
    MSG_AAAAH = 0x1764,
    MSG_ITS_VERY_RECKLESS_FOR_SUCH = 0x1768,
    MSG_WAS_WATCHING_FROM_HERE_AFTER = 0x1769,
    MSG_LOOKED_VERY_COURAGEOUS_WALKING_TOWARD = 0x176c,
    MSG_REWARD_RECEIVED_WAS_INDEED_GREATEST = 0x176e,
    MSG_GOT_PRETTY_NICE_REWARD_BUT = 0x176f
};

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = obj->facing_target;
    if (target != NULL) {
        obj->facing_flags = (u8)(0xFE & obj->facing_flags);
        angle = (u16)CalculateFacingAngle(target->position_z - obj->position_z, target->position_x - obj->position_x);
        old = obj->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            obj->facing = (u16)(old + delta);
        }
    }
    return 1;
}

u8 *SceneData_GetTableA638(void)
{
    return (u8 *)0x0200a638;
}

s32 Func_02000090(void)
{
    return 0;
}

u8 *SceneData_GetTableA920(void)
{
    return (u8 *)0x0200a920;
}

/* Taking a reward changes four entries in this palace placement table. */
s32 SceneData_SelectRecordByScene21(void)
{
    u8 *p;

    /* A signed halfword read. */
    if (gGameState.scene == (s32)&Value_00000021) {
        p = Data_0200a9b4;
        Func_02002536(p);

        if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
            struct SceneRecord *rec = (struct SceneRecord *)p;

            rec->field_166 = 2;
            rec->field_190 = 0;
            rec->field_214 = 3;
            rec->field_238 = 1;
        }

        return (s32)p;
    }
    return (s32)Data_0200a99c;
}

s32 SceneData_SelectTableac9cByState(void)
{
    if (gGameState.scene == (s32)&Value_00000021) {
        return (s32)Data_0200aca8;
    }
    return (s32)Data_0200ac9c;
}

void SceneDialogue_RunActor10Message1420(void)
{
    Event_Begin();
    Event_SetMessage((s32)&LinkedMessage_YouWillingGoKolimaForest);
    Event_AskYesNo(10, 0);
    Event_End();
}

void FieldScene_RunScene38d_02000150(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_ShowEmote(14, 0x102, 0);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(40);
    Event_SetMessage(MSG_AAAAH);
    Event_ShowMessageAndWait(14, 0, 20);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(14, 0, 10);
    Actor_FaceDirection(14, 0xb000, 10);
    Event_End();
}

void FieldScene_RunScene38dSequenceA(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(MSG_FINE_WARRIOR);
    if (GameFlag_IsSet(0x302) != 0) {
        Event_SetMessage(MSG_ITS_VERY_RECKLESS_FOR_SUCH);
    }
    Event_ShowMessage(15, 0);
    GameFlag_Set(0x302);
    Event_End();
}

void SceneDialogue_RunActor16Message1769(void)
{
    Event_Begin();
    Event_SetMessage(MSG_WAS_WATCHING_FROM_HERE_AFTER);
    Event_AskYesNo(16, 0);
    Event_End();
}

void FieldScene_RunActorSeventeenFlaggedDialogue(void)
{
    Event_Begin();

    if (GameFlag_IsSet(0x202) != 0) {
        Event_SetMessage(MSG_MAY_CHOOSE_ONLY_ONE_ITEM);
    } else if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        Event_SetMessage(MSG_REWARD_RECEIVED_WAS_INDEED_GREATEST);
    } else {
        Event_SetMessage(MSG_DO_THINK_THESE_BILIBINS_GREAT);
        if (GameFlag_IsSet(0x84d) != 0) {
            gEventWork->message++;
        }
    }

    Event_ShowMessage(17, 0);
    Event_End();
}

void SceneDialogue_RunActor15Flag303Scene(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(MSG_SOMETIMES_WE_NEED_CHILDREN_REMIND);
    if (GameFlag_IsSet(0x303) != 0) {
        Event_SetMessage(MSG_LOOKED_VERY_COURAGEOUS_WALKING_TOWARD);
    }
    Event_ShowMessage(15, 0);
    GameFlag_Set(0x303);
    Event_End();
}

void FieldScene_RunActorSeventeenFlagDialogue(void)
{
    Event_Begin();

    if (GameFlag_IsSet(0x202) != 0) {
        Event_SetMessage(MSG_NONETHELESS_IF_YOUR_LUCK_SOUR);
    } else if (GameFlag_IsSet(0x845) == 0) {
        Event_SetMessage(MSG_GOOD_TREASURE_IF_GET_TURNED);
    } else {
        Event_SetMessage(MSG_WE_REALLY_GIVING_OUR_TREASURE);
        if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
            Event_SetMessage(MSG_GOT_PRETTY_NICE_REWARD_BUT);
        }
    }

    Event_ShowMessage(17, 0);
    Event_End();
}

void FieldScene_RunStepWithValue174d(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_ROBIN_CHECKED_CHEST_BUT_WAS, 1);
    Event_End();
}

void FieldScene_RunStepWithValue174e(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_TREASURE_CHEST_LOCKED, 1);
    Event_End();
}

void FieldScene_RunBranchedStep(void)
{
    if (Func_02002878() != 0) {
        FieldScene_RunStepWithValue174d();
    } else {
        FieldScene_RunStepWithValue174e();
    }
}

void FieldScene_RunStep210ByFlag84e(void)
{
    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        Func_02000872(0x210);
    } else {
        FieldScene_RunSlotSubjectBranch(21, 182, 0x210);
    }
}

void FieldScene_RunStep211ByFlag84e(void)
{
    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        Func_0200089c(0x211);
    } else {
        FieldScene_RunSlotSubjectBranch(22, 183, 0x211);
    }
}

void FieldScene_RunStep212ByFlag84e(void)
{
    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        Func_020008c8(0x212);
    } else {
        FieldScene_RunSlotSubjectBranch(23, 186, 0x212);
    }
}

void FieldScene_RunStep213ByFlag84e(void)
{
    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        Func_020008f4(0x213);
    } else {
        FieldScene_RunSlotSubjectBranch(24, 189, 0x213);
    }
}

void FieldScene_RunSlotSubjectBranch(s32 actor, s32 item, s32 flag)
{
    s32 record;

    Event_Begin();

    record = Func_02002ab4(0, actor, item);

    if (Party_GiveItem(item, 0) != -1) {
        Actor_SetAnimation(actor, 2);
        GameFlag_Set(FLAG_REWARD_TAKEN);
        GameFlag_Set(flag);
        GameFlag_Clear(0x322);
        GameFlag_Clear(0x202);
    } else {
        Audio_PlayCue(125);
        Actor_SetAnimation(actor, 5);
    }

    Engine_ObjectDispatchRelease(record);
    Event_End();
}

void FieldScene_RunScene38d_02000568(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) == 0) {
        if (Value1(Func_020029c8_scene_primary_script_head, 0x322) != 0) {
            Event_Begin();
            Actor_ShowEmote(19, 0x100, 0);
            Actor_FaceDirection(19, 0x7000, 10);
            Actor_RunRepeatedMotion(19, 2);
            Event_Wait(20);
            Event_SetMessage(MSG_PLEASE_TAKE_YOUR_REWARD_BEFORE);
            Event_ShowMessage(19, 0);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x268, 0x2fa);
            Actor_FaceDirection(19, 0xd000, 10);
            Event_End();
        }
    }
}

void FieldScene_RunScene38d_020005f4(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        Event_Begin();
        Actor_FaceActor(ACTOR_PARTY_LEADER, 19, 0);
        Actor_SetSpeed(19, 0x9999, 0x4ccc);
        Actor_WalkToAndWait(19, 0x26e, 0x2fc);
        Actor_FaceDirection(19, 0xf000, 20);
        Actor_SetAnimationAndWait(19, 3);
        Actor_SetAnimationAndWait(17, 3);
        Event_Wait(20);
        Actor_FaceActor(19, ACTOR_PARTY_LEADER, 0);
        Event_Wait(20);
        Actor_SetAnimationAndWait(19, 3);
        Event_SetMessage(MSG_ALWAYS_WELCOME_IN_PALACE_LORD);
        Event_ShowMessageAndWait(19, 0, 10);
        Actor_SetSpeed(19, 0xcccc, 0x6666);
        Actor_WalkToAndWait(19, 0x23a, 0x2f6);
        Actor_SetPosition(19, 0, 0);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
        GameFlag_Set(0x85e);
        GameFlag_Set(0x333);
        Event_End();
    }
}

void FieldScene_RunLongBranchingChoreography(void)
{
    s32 record;

    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Engine_TaskWait(1);
    *(u8 *)(Func_02002e68() + 85) = 0;
    Call3(Engine_CameraMoveTo, 0x37e0000, -1, 0x2980000);
    Engine_TaskWait(1);
    Engine_ActorSetPosition(0, 0, 0);
    if (Value1(Engine_GameFlagIsSet, 0x85f) != 0) {
        Call4(Engine_CameraMoveTo, 0x37e0000, -1, 0x2ba0000, 0);
        Call3(Engine_ActorSetPosition, 19, 0x36c0000, 0x27a0000);
        Call3(Engine_ActorFaceDirection, 19, 0x3000, 0);
        Call3(Engine_ActorSetPosition, 0, 0x37e0000, 0x31e0000);
    }
    Engine_MapRedraw();
    Engine_TaskWait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 40;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    if (Value1(Engine_GameFlagIsSet, 0x85f) == 0) {
        Engine_EventWait(80);
        Call3(Engine_ActorSetPosition, 19, 0x37e0000, 0x31e0000);
        Call2(Engine_CameraSetSpeed, 0x9999, 0x1333);
        Camera_MoveTo(0x37e0000, -1, 0x2ba0000, 1);
        Call3(Engine_ActorSetSpeed, 19, 0xcccc, 0x6666);
        Actor_WalkTo(19, 0x37e, 0x2b8);
        Engine_EventWait(80);
        Call4(Engine_CameraMoveTo, 0x37e0000, -1, 0x2980000, 1);
        Actor_WaitForMove(19);
        Call3(Engine_ActorWalkToAndWait, 19, 0x34a, 0x2b8);
        Actor_WalkToAndWait(19, 0x34a, 0x27c);
        Actor_FaceDirection(18, 0x7000, 20);
        Call3(Engine_ActorWalkToAndWait, 19, 0x36c, 0x27a);
        Engine_ActorSetAnimationAndWait(19, 3);
        Engine_EventWait(20);
        Actor_SetAnimationAndWait(18, 3);
        Event_Wait(10);
        Call1(Engine_EventSetMessage, 0x1437);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Actor_RunRepeatedMotion(19, 2);
        Engine_EventShowMessageAndWait(19, 0, 20);
        Engine_ActorRunRepeatedMotion(18, 1);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Engine_ActorSetAnimationAndWait(19, 3);
        Engine_EventWait(40);
        Call3(Engine_ActorShowEmote, 18, 0x105, 60);
        Call2(Engine_EventShowMessage, 0x2012, 0);
        Engine_ActorRunRepeatedMotion(18, 1);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Call2(Engine_ActorSetAttachedEffect, 19, 0x102);
        Engine_EventWait(60);
        Call3(Engine_ActorFaceDirection, 19, 0x3000, 10);
        Call3(Engine_ActorFaceDirection, 18, 0x5000, 10);
        Call4(Engine_CameraMoveTo, 0x37e0000, -1, 0x2ba0000, 1);
        Call3(Engine_ActorSetPosition, 0, 0x37e0000, 0x31e0000);
        Call3(Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
        Call3(Engine_ActorWalkToAndWait, 0, 0x37e, 0x2d6);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(0, 3);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(18, 1);
        Call2(Engine_EventShowMessage, 0x2012, 0);
        Camera_MoveTo(0x37e0000, -1, 0x2980000, 1);
        Call3(Engine_ActorWalkToAndWait, 0, 0x37e, 0x2ac);
        record = Value1(Func_02002fae, 0);
        if (record != 0) {
            Engine_ActorSetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        record = Value1(Func_02002fc2, 0);
        if (record != 0) {
            Engine_ActorSetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        if (Value1(Engine_GameFlagIsSet, 3) != 0) {
            record = Value1(Func_02002fe0, 0);
            if (record != 0) {
                Engine_ActorSetPosition(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
            }
        }
        Call3(Engine_ActorSetSpeed, 1, 0x9999, 0x4ccc);
        Call3(Engine_ActorSetSpeed, 2, 0x9999, 0x4ccc);
        Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
        Engine_ActorSetAnimation(1, 2);
        Engine_ActorSetAnimation(2, 2);
        Engine_ActorSetAnimation(3, 2);
        Actor_SetDestinationOffset(ACTOR_GERALD, -16, 16);
        Engine_ActorSetDestinationOffset(2, 16, 16);
        if (Value1(Engine_GameFlagIsSet, 3) != 0) {
            Engine_ActorSetDestinationOffset(3, 32, 16);
        }
        Engine_ActorWaitForMove(2);
        Engine_ActorSetAnimation(1, 1);
        Engine_ActorSetAnimation(2, 1);
        Actor_SetAnimation(ACTOR_MIA, 1);
        Engine_EventWait(10);
        Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
        Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0xc000, 20);
        Func_0200311c(18, 2, 20);
        Call3(Engine_ActorFaceDirection, 18, 0x7000, 10);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Call3(Engine_ActorFaceDirection, 19, 0x1000, 10);
        Actor_SetAnimationAndWait(19, 3);
        Call3(Engine_ActorFaceDirection, 18, 0x5000, 40);
        Call3(Engine_ActorFaceDirection, 18, 0x7000, 10);
        Engine_ActorSetAnimationAndWait(18, 4);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Call2(Engine_ActorSetAttachedEffect, 19, 0x102);
        Event_Wait(40);
        Call3(Engine_ActorFaceDirection, 18, 0x5000, 20);
        Call3(Engine_ActorShowEmote, 18, 0x105, 40);
        Value2(Engine_EventOpenMessage, 0x2012, 0);
        Call3(Engine_ActorFaceDirection, 1, 0xe000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0xa000, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            goto L_02000f86;
        }
    L_02000cb6:
        Engine_EventSetMessage((s32)&LinkedMessage_TooYoungForTheJob);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Call3(Engine_ActorFaceDirection, 19, 0x3000, 0);
        Engine_ActorSetAnimationAndWait(18, 4);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimation(ACTOR_GERALD, 2);
        record = Value1(Func_020031f6, 0);
        if (record != 0) {
            Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorSetAnimation(2, 2);
        record = Value1(Func_02003216, 0);
        if (record != 0) {
            Engine_ActorSetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        if (Value1(Engine_GameFlagIsSet, 3) != 0) {
            Engine_ActorSetAnimation(3, 2);
            record = Value1(Func_02003240, 0);
            if (record != 0) {
                Engine_ActorSetDestination(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
        }
        Engine_ActorWaitForMove(2);
        Engine_ActorSetPosition(1, 0, 0);
        Engine_ActorSetPosition(2, 0, 0);
        Actor_SetPosition(ACTOR_MIA, 0, 0);
        Call1(Func_0200323c, 0x85f);
        Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x37e, 0x2f0);
        gEventWork->transition_frames = 16;
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
        goto L_0200177e;
    }
    Call3(Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
    Call3(Engine_ActorWalkTo, 0, 0x37e, 0x2ac);
    Engine_EventWait(80);
    Call2(Engine_CameraSetSpeed, 0x9999, 0x1333);
    Call4(Engine_CameraMoveTo, 0x37e0000, -1, 0x2980000, 1);
    Engine_ActorWaitForMove(0);
    Engine_ActorSetAnimation(0, 1);
    record = Value1(Func_0200330a, 0);
    if (record != 0) {
        Engine_ActorSetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_0200331e, 0);
    if (record != 0) {
        Engine_ActorSetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    if (Value1(Engine_GameFlagIsSet, 3) != 0) {
        record = Value1(Func_0200333c, 0);
        if (record != 0) {
            Engine_ActorSetPosition(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
    }
    Call3(Engine_ActorSetSpeed, 1, 0x9999, 0x4ccc);
    Call3(Engine_ActorSetSpeed, 2, 0x9999, 0x4ccc);
    Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
    Engine_ActorSetAnimation(1, 2);
    Engine_ActorSetAnimation(2, 2);
    Engine_ActorSetAnimation(3, 2);
    Call3(Engine_ActorSetDestinationOffset, 1, -16, 16);
    Engine_ActorSetDestinationOffset(2, 16, 16);
    if (Value1(Engine_GameFlagIsSet, 3) != 0) {
        Engine_ActorSetDestinationOffset(3, 32, 16);
    }
    Engine_ActorWaitForMove(2);
    Engine_ActorSetAnimation(1, 1);
    Engine_ActorSetAnimation(2, 1);
    Engine_ActorSetAnimation(3, 1);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 20);
    Call3(Engine_ActorShowEmote, 18, 0x101, 60);
    Event_SetMessage(MSG_YEHVE_HAD_CHANGE_HEART_YEH);
    Value2(Engine_EventOpenMessage, 0x2012, 0);
    if (Value2(Engine_EventChooseYesNo, 0, 0) == 1) {
        goto L_02000cb6;
    }
L_02000f86:
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 20);
    Engine_ActorSetAnimation(3, 3);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(2, 3);
    Call3(Engine_ActorShowEmote, 18, 0x105, 60);
    Call1(Engine_EventSetMessage, 0x1443);
    Call2(Engine_EventShowMessage, 0x2012, 0);
    record = Func_0200348a(20);
    Func_02003438(record, 0);
    record = Func_02003496(20);
    *(s32 *)(record + 24) = 0x8000;
    *(s32 *)(record + 28) = 0x8000;
    record = Value1(Func_020034a4, 18);
    if (record != 0) {
        Engine_ActorSetPosition(20, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Engine_TaskWait(1);
    Func_0200353a(20, 6, 0);
    Call3(Engine_ActorSetSpeed, 20, 0x20000, 0x10000);
    Actor_MoveToAndWait(20, 0x37e, 0x29c);
    Event_Wait(40);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Call3(Engine_ActorShowEmote, 3, 0x101, 0);
    Call3(Engine_ActorShowEmote, 0, 0x101, 0);
    Call3(Engine_ActorShowEmote, 1, 0x101, 0);
    Call3(Engine_ActorShowEmote, 2, 0x101, 60);
    Engine_ActorSetAnimationAndWait(18, 4);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Call3(Engine_ActorShowEmote, 1, 0x103, 60);
    Call3(Engine_ActorFaceDirection, 1, 0xe000, 10);
    Value2(Engine_EventOpenMessage, 0x4001, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
    if (Value2(Engine_EventChooseYesNo, 0, 0) == 1) {
        do {
            Func_020035fc(1, 2);
            Engine_ActorRunRepeatedMotion(2, 2);
            Call1(Engine_EventSetMessage, 0x1447);
            Value2(Engine_EventOpenMessage, 0x4001, 0);
        } while (Value2(Engine_EventChooseYesNo, 0, 0) != 1);
    }
    Engine_ActorSetAnimationAndWait(1, 3);
    Call1(Engine_EventSetMessage, 0x1448);
    Call3(Engine_EventShowMessageAndWait, 0x4001, 0, 10);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 10);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Actor_ShowEmote(18, 0x105, 60);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 60);
    Call3(Engine_ActorFaceDirection, 18, 0x3000, 10);
    Call3(Engine_ActorShowEmote, 18, 0x101, 60);
    Call3(Engine_ActorShowEmote, 1, 0x101, 40);
    Engine_ActorFaceDirection(1, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Call3(Engine_EventShowMessageAndWait, 0x4001, 0, 10);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0x2000, 20);
    Call3(Engine_ActorShowEmote, 2, 0x102, 60);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 10);
    Call3(Engine_EventShowMessageAndWait, 0x4002, 0, 10);
    Engine_ActorRunRepeatedMotion(1, 1);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(1, 3);
    Call3(Engine_EventShowMessageAndWait, 0x4001, 0, 10);
    Call3(Engine_ActorFaceDirection, 1, 0xe000, 10);
    Func_02003786(1, 1);
    Value2(Engine_EventOpenMessage, 0x4001, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
    while (Event_ChooseYesNo(0, 0) != 0) {
        Call1(Engine_EventSetMessage, 0x144e);
        Call2(Func_020037f0, 0x4001, 0);
    }
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 0);
    Engine_ActorFaceDirection(1, 0, 10);
    Call3(Engine_ActorFaceDirection, 0, 0x2000, 10);
    Engine_ActorSetAnimationAndWait(1, 3);
    Call3(Engine_ActorShowEmote, 2, 0x105, 60);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 10);
    Engine_ActorSetAnimationAndWait(2, 4);
    Call1(Engine_EventSetMessage, 0x144f);
    Call3(Engine_EventShowMessageAndWait, 0x4002, 0, 20);
    Engine_ActorRunRepeatedMotion(18, 1);
    Call3(Engine_ActorFaceDirection, 18, 0x5000, 10);
    Engine_ActorSetAnimationAndWait(18, 4);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 20);
    Engine_ActorSetAnimation(3, 3);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(40);
    Actor_ShowEmote(18, 0x105, 80);
    Call3(Engine_ActorFaceDirection, 19, 0x1000, 10);
    Call2(Engine_ActorSetAttachedEffect, 19, 0x102);
    Engine_EventWait(40);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Engine_ActorRunRepeatedMotion(18, 1);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 18, 0x7000, 20);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Engine_ActorSetAnimationAndWait(19, 3);
    Engine_EventWait(20);
    Actor_SetAnimationAndWait(18, 4);
    Actor_SetAnimation(18, 4);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Func_0200392c(20, 6, 0);
    record = Value1(Func_020038ba, 18);
    if (record != 0) {
        Engine_ActorSetDestination(20, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(20);
    Engine_ActorSetPosition(20, 0, 0);
    Engine_EventWait(20);
    Call2(Engine_ActorSetAttachedEffect, 3, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 1, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 2, 0x102);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(19, 2);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 20);
    Engine_ActorSetAnimationAndWait(19, 3);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Call3(Engine_EventShowMessageAndWait, 0x4002, 0, 10);
    Call3(Engine_ActorFaceDirection, 19, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 18, 0x3000, 10);
    Engine_ActorSetAnimationAndWait(18, 4);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Call3(Engine_ActorShowEmote, 1, 0x103, 60);
    Call3(Engine_EventShowMessageAndWait, 0x4001, 0, 10);
    Call3(Engine_ActorFaceDirection, 18, 0x5000, 10);
    Engine_ActorSetAnimation(18, 4);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Engine_ActorSetAnimation(2, 4);
    Call3(Engine_EventShowMessageAndWait, 0x4002, 0, 10);
    Call3(Engine_ActorFaceDirection, 18, 0x3000, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Call3(Engine_ActorShowEmote, 3, 0x107, 0);
    Call3(Engine_ActorShowEmote, 0, 0x107, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x107, 0);
    Call3(Engine_ActorShowEmote, 2, 0x107, 60);
    Call3(Engine_ActorFaceDirection, 18, 0x7000, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Engine_ActorRunRepeatedMotion(19, 2);
    Call3(Engine_ActorFaceDirection, 19, 0x1000, 10);
    Engine_ActorSetAnimationAndWait(19, 3);
    Engine_EventWait(20);
    Call3(Func_02003af2, 0, 0x10013, (s32)Data_0200a5c0);
    Call3(Func_02003afc, 1, 0x10013, (s32)Data_0200a5c0);
    Call3(Func_02003b06, 2, 0x10013, (s32)Data_0200a5c0);
    Call3(Func_02003b10, 3, 0x10013, (s32)Data_0200a5c0);
    Call3(Engine_ActorSetSpeed, 19, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(19, 0x354, 0x286);
    Call3(Engine_ActorWalkToAndWait, 19, 0x354, 0x29a);
    Call3(Engine_ActorWalkToAndWait, 19, 0x360, 0x2a0);
    Call3(Engine_ActorFaceDirection, 19, 0x1000, 10);
    Event_ShowMessageAndWait(0x4013, 0, 20);
    Actor_Stop(ACTOR_PARTY_LEADER);
    Engine_ActorStop(1);
    Engine_ActorStop(2);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Call3(Engine_ActorShowEmote, 1, 0x105, 0);
    Call3(Engine_ActorShowEmote, 2, 0x105, 60);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Call3(Func_02003baa_b, 19, 0x10000, (s32)Data_0200a5c0);
    Engine_ActorSetAnimation(1, 2);
    record = Value1(Func_02003b20, 0);
    if (record != 0) {
        Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorSetAnimation(2, 2);
    record = Value1(Func_02003b7e, 0);
    if (record != 0) {
        Engine_ActorSetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    if (Value1(Engine_GameFlagIsSet, 3) != 0) {
        Engine_ActorSetAnimation(3, 2);
        record = Value1(Func_02003ba8, 0);
        if (record != 0) {
            Engine_ActorSetDestination(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
    }
    Engine_ActorWaitForMove(2);
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetPosition(2, 0, 0);
    Engine_ActorSetPosition(3, 0, 0);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, 0, 0x37e, 0x2f0);
    gEventWork->transition_frames = 16;
    Event_CloseScreen();
    Engine_EventWaitForScreen();
    Call1(Func_02003bd4, 0x321);
L_0200177e:
    Event_RequestExit(29);
    Engine_EventEnd();
}

void FieldScene_RunScene38d_0200179c(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Call3(Engine_ActorFaceDirection, 19, 0x3000, 0);
    Call3(Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x100, 0x294);
    Engine_EventWait(20);
    Camera_MoveTo(-1, -1, -1, 0);
    Call1(Func_02003c44, 0x200);
    Audio_PlayCue(188);
    Func_02003c20(1);
    Func_02003c26(2);
    Call3(Engine_ActorSetPosition, 19, 0x1000000, 0x2780000);
    Engine_TaskWait(1);
    Call3(Engine_ActorSetSpeed, 19, 0x9999, 0x4ccc);
    Call3(Engine_ActorWalkToAndWait, 19, 0x100, 0x284);
    Func_02003c60(1);
    Func_02003c66(2);
    Event_Wait(20);
    Actor_RunRepeatedMotion(19, 2);
    Call1(Engine_EventSetMessage, 0x145e);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Call3(Engine_ActorShowEmote, 0, 0x100, 40);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x108, 0x294);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Call3(Engine_ActorWalkToAndWait, 19, 248, 0x294);
    Call3(Engine_ActorFaceDirection, 19, 0x1000, 40);
    Engine_ActorSetAnimationAndWait(19, 4);
    Engine_EventShowMessage(19, 0);
    Engine_ActorSetAnimationAndWait(19, 3);
    Value2(Engine_EventAskYesNo, 19, 0);
    Actor_RunRepeatedMotion(19, 2);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Call3(Engine_ActorShowEmote, 0, 0x101, 60);
    Actor_SetAttachedEffect(19, 0x102);
    Event_Wait(60);
    Engine_ActorRunRepeatedMotion(19, 1);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Engine_ActorSetAnimationAndWait(19, 3);
    Event_ShowMessage(19, 0);
    Actor_SetSpeed(19, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 19, 248, 0x304);
    Engine_ActorSetPosition(19, 0, 0);
    Call1(Engine_GameFlagClear, 0x12f);
    Call1(Func_02003d84, 0x84f);
    Engine_EventEnd();
}

void ConfigurePrimarySceneChannels(void)
{
    FaceActor(1, 0xe000, 0);
    FaceActor(2, 0xa000, 0);
    FaceActor(3, 0x8000, 0);
}

void ConfigureSecondarySceneChannels(void)
{
    FaceActor(1, 0xc000, 0);
    FaceActor(2, 0xc000, 0);
    FaceActor(3, 0xa000, 0);
}

/* FAKEMATCH: The cleared motion byte keeps its own zero local so the
 * following call reloads its separate zero argument. */
void RunEventScript02(void)
{
    u8 *buf;
    struct FieldActor *actor;
    s32 flag;
    u8 clear = 0;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    buf = (u8 *)Func_02003f58();
    buf[85] = clear;
    Camera_MoveTo(0x037e0000, -1, 0x02980000, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Map_Redraw();
    Task_Wait(1);

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    gEventWork->transition_frames = 16;

    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_SetPosition(19, 0x03780000, 0x031e0000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x03880000, 0x031e0000);
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0x037e0000, -1, 0x02ba0000, 1);
    Actor_SetSpeed(19, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Actor_WalkTo(19, 888, 720);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 904, 736);
    Event_Wait(60);
    Actor_WaitForMove(19);
    Actor_SetAnimation(19, 1);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(20);
    Actor_RunRepeatedMotion(19, 2);
    Event_SetMessage(MSG_WEVE_BROUGHT_WARRIORS_MILORD);

    flag = 1;
    if (GameFlag_IsSet(0x84f) == 0) {
        AdvanceMessage(1);
        flag = 0;
    }
    Event_ShowMessage(19, 0);
    if (flag != 0) {
        AdvanceMessage(1);
    }

    Camera_MoveTo(0x037e0000, -1, 0x02980000, 1);
    Actor_EnableActionCallback(19, Data_0200a5d4);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 684);

    actor = (struct FieldActor *)Func_02003fb0(0);
    if (actor != 0) {
        Actor_SetPosition(ACTOR_GERALD, actor->x.fixed, actor->z.fixed);
    }
    actor = (struct FieldActor *)Func_02003fc4(0);
    if (actor != 0) {
        Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
    }
    actor = (struct FieldActor *)Func_02003fd8(0);
    if (actor != 0) {
        Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
    }

    Actor_SetSpeed(ACTOR_GERALD, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_IVAN, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    Actor_SetAnimation(ACTOR_IVAN, 2);
    Actor_SetAnimation(ACTOR_MIA, 2);
    Actor_SetDestinationOffset(ACTOR_GERALD, -16, 16);
    Actor_SetDestinationOffset(ACTOR_IVAN, 16, 16);
    Actor_SetDestinationOffset(ACTOR_MIA, 32, 16);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_SetAnimation(ACTOR_MIA, 1);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 0);
    Func_020040ac(19);
    Event_Wait(20);

    flag = 1;
    if (GameFlag_IsSet(0x84f) == 0) {
        AdvanceMessage(1);
        flag = 0;
    }
    Actor_RunRepeatedMotion(18, 3);
    Event_ShowMessageAndWait(0x2012, 0, 20);
    if (flag != 0) {
        AdvanceMessage(1);
    }

    flag = 1;
    if (GameFlag_IsSet(0x84f) == 0) {
        AdvanceMessage(1);
        flag = 0;
    }
    Actor_RunRepeatedMotion(18, 1);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    if (flag != 0) {
        AdvanceMessage(1);
    }

    ConfigurePrimarySceneChannels();
    Event_Wait(20);
    if (GameFlag_IsSet(0x84f) != 0) {
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
        Actor_ShowEmote(ACTOR_GERALD, 261, 40);
    } else {
        Event_Wait(40);
    }

    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 10);
    Event_ShowMessageAndWait(0x4001, 0, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_ShowMessage(0x4002, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 10);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_ShowMessageAndWait(0x4003, 0, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x2012, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 259, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);

    if (GameFlag_IsSet(0x84f) != 0) {
        Actor_RunRepeatedMotion(18, 1);
        Actor_SetAnimationAndWait(18, 4);
        Event_OpenMessage(0x2012, 0);
        ConfigurePrimarySceneChannels();
        flag = 1;
        if (Event_ChooseYesNo(0, 0) != 0) {
            AdvanceMessage(1);
            flag = 0;
        }
        Actor_FaceDirection(18, 0x5000, 0);
        Func_02003748_channels();
        Event_Wait(10);
        Event_ShowMessageAndWait(0x2012, 0, 10);
        if (flag != 0) {
            AdvanceMessage(1);
        }
        Actor_ShowEmote(18, 258, 60);
    } else {
        AdvanceMessage(4);
    }

    Event_OpenMessage(0x2012, 0);
    ConfigurePrimarySceneChannels();
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage(MSG_HUMBLY_THANK);
    } else {
        Event_SetMessage(MSG_NAE_YEH_DINNAE_NEED_TAE);
    }

    ConfigureSecondarySceneChannels();
    Event_ShowMessageAndWait(0x2012, 0, 20);
    Actor_RunRepeatedMotion(19, 1);
    Event_SetMessage(MSG_WAS_BUT_WORRIED_YEH_MIGHT);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x8000, 40);
    Actor_RunRepeatedMotion(18, 2);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    ConfigureSecondarySceneChannels();
    Event_Wait(10);
    Actor_ShowEmote(18, 261, 60);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 3);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_ShowEmote(18, 264, 60);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_FaceDirection(18, 0x3000, 10);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_SetAnimationAndWait(18, 3);
    Event_OpenMessage(0x2012, 0);

    ConfigurePrimarySceneChannels();
    flag = 1;
    if (Event_ChooseYesNo(0, 0) == 1) {
        AdvanceMessage(1);
        flag = 0;
    }
    ConfigureSecondarySceneChannels();
    Event_ShowMessageAndWait(0x2012, 0, 10);
    if (flag != 0) {
        AdvanceMessage(1);
    }

    Actor_FaceDirection(18, 0x7000, 10);
    Actor_RunRepeatedMotion(19, 1);
    Actor_FaceDirection(19, 0x1000, 20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(19, 3);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(19, 0x3000, 10);
    Actor_FaceDirection(18, 0x3000, 20);
    Actor_RunRepeatedMotion(18, 1);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_SetAnimationAndWait(18, 3);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);

    Actor_SetAnimation(ACTOR_GERALD, 2);
    actor = (struct FieldActor *)Func_020044de(0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_GERALD, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_SetAnimation(ACTOR_IVAN, 2);
    actor = (struct FieldActor *)Func_020044fe(0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_IVAN, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_SetAnimation(ACTOR_MIA, 2);
    actor = (struct FieldActor *)Func_0200451e(0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_MIA, actor->x.part.pixel, actor->z.part.pixel);
    }

    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Event_Wait(20);
    Actor_FaceDirection(18, 0x5000, 0);
    Call3(Func_0200460e, 0, 0x10013, (s32)Data_0200a5c0);
    Actor_WalkToAndWait(19, 852, 646);
    Actor_WalkToAndWait(19, 852, 666);
    Actor_WalkToAndWait(19, 864, 672);
    Actor_FaceDirection(19, 0x1000, 10);
    Actor_RunRepeatedMotion(19, 1);
    Event_Wait(10);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_WalkToAndWait(19, 886, 708);
    Actor_WalkTo(19, 894, 764);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 764);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(802);
    if (GameFlag_IsSet(0x84f) == 0) {
        GameFlag_Set(0x84f);
        GameFlag_Set(0x84a);
    }
    Event_RequestExit(6);
    Event_End();
}
