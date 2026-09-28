#include "KYUDEN.H"

void FieldScene_ShowChestEmpty(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_ROBIN_CHECKED_CHEST_BUT_WAS, 1);
    Event_End();
}

void FieldScene_ShowChestLocked(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_TREASURE_CHEST_LOCKED, 1);
    Event_End();
}

void FieldScene_RunChestStep(s32 flag)
{
    if (Engine_GameFlagIsSet(flag) != 0) {
        FieldScene_ShowChestEmpty();
    } else {
        FieldScene_ShowChestLocked();
    }
}

void FieldScene_RunStep210ByFlag84e(void)
{
    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        FieldScene_RunChestStep(0x210);
    } else {
        FieldScene_RunSlotSubjectBranch(21, 182, 0x210);
    }
}

void FieldScene_RunStep211ByFlag84e(void)
{
    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        FieldScene_RunChestStep(0x211);
    } else {
        FieldScene_RunSlotSubjectBranch(22, 183, 0x211);
    }
}

void FieldScene_RunStep212ByFlag84e(void)
{
    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        FieldScene_RunChestStep(0x212);
    } else {
        FieldScene_RunSlotSubjectBranch(23, 186, 0x212);
    }
}

void FieldScene_RunStep213ByFlag84e(void)
{
    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        FieldScene_RunChestStep(0x213);
    } else {
        FieldScene_RunSlotSubjectBranch(24, 189, 0x213);
    }
}

void FieldScene_RunSlotSubjectBranch(s32 actor, s32 item, s32 flag)
{
    s32 record;

    Event_Begin();

    record = BattleFx_PlayCueAndStartEmitterOnTarget(0, actor, item);

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

void FieldScene_RunRewardReminder(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(FLAG_REWARD_TAKEN) == 0) {
        if (Value1(Engine_GameFlagIsSet, 0x322) != 0) {
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

void FieldScene_RunPalaceFarewell(void)
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
