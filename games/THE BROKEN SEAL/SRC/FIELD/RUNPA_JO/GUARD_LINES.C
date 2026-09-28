/* The Lunpa fortress: the guards' lines and the first searchable objects. */
#include "FORTRESS.H"

void RunActorScriptedSequenceC(s32 actor_id)
{
#if defined(TBS_EDITION_JA)
    u8 *t = 0x25aa;
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    u8 *t = 0x2403;
#else
    u8 *t = 0x2421;
#endif

    Event_SetMessage((s32)t);
    Event_ShowMessage(actor_id, 0);
    Actor_RunRepeatedMotion(actor_id, 1);
    Event_SetMessage((s32)(t + 1));
    Event_ShowMessage(actor_id, 0);
    Actor_SetAnimationAndWait(actor_id, 4);
    Event_SetMessage((s32)(t + 2));
    Event_ShowMessage(actor_id, 0);
}

void FieldScene_RunScene3bf_02001cf0(s32 a0)
{
    u32 i;
    s32 record;
    s32 base6_2424;

    base6_2424 = 0x2424;
    Event_SetMessage(base6_2424);
    Event_ShowMessage(a0, 0);
    Event_Wait(120);
    Actor_ShowEmote(a0, 0x101, 60);
    Event_SetMessage((base6_2424 + 1));
    Event_ShowMessage(a0, 0);
    Actor_RunRepeatedMotion(a0, 1);
    Event_SetMessage((base6_2424 + 2));
    Event_ShowMessage(a0, 0);
    Actor_SetAnimationAndWait(a0, 4);
    Event_SetMessage((base6_2424 + 3));
    Event_ShowMessage(a0, 0);
}

void RunActorScriptedSequenceD(s32 actor_id)
{
    u8 *t = 0x2428;

    Event_SetMessage((s32)t);
    Event_ShowMessage(actor_id, 0);
    Actor_SetAnimationAndWait(actor_id, 4);
    Event_SetMessage((s32)(t + 1));
    Event_ShowMessage(actor_id, 0);
    Actor_RunRepeatedMotion(actor_id, 1);
    Event_SetMessage((s32)(t + 2));
    Event_ShowMessage(actor_id, 0);
    Actor_SetAnimationAndWait(actor_id, 3);
    Event_SetMessage((s32)(t + 3));
    Event_ShowMessage(actor_id, 0);
}

void InspectOrdinaryObject(void)
{

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(15, 256, 60);
    TurnActorToSceneDirection(15);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gGameState.unknown_1f8[0x22b - 0x1f8] = 3;
    BattleFx_SetWeightedResult(98, 2);
    Actor_SetPosition(15, 0, 0);
    Event_End();
    GameFlag_Set(2380);
}

void InspectEmptyChest(void)
{

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(11, 256, 60);
    TurnActorToSceneDirection(11);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gGameState.unknown_1f8[0x22b - 0x1f8] = 3;
    BattleFx_SetWeightedResult(98, 2);
    Actor_SetPosition(11, 0, 0);
    Event_End();
    GameFlag_Set(2377);
}
