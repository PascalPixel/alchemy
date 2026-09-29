#include "SUKURETA.H"
extern u8 MsgHaidiaSukuretaOurBestBet[];

void Scene_LeaveForMtAleph(void)
{
    s32 record;
    s32 facing;

    Event_Begin();
    Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
    Actor_SetPosition(ACTOR_GERALD, 0xd80000, 0x1080000);
    Actor_SetPosition(ACTOR_JASMINE, 0xf80000, 0x1080000);
    record = (s32)Engine_ActorGet(1);
    facing = 0xc000;
    *(u16 *)(record + 6) = facing;
    record = (s32)Engine_ActorGet(5);
    *(u16 *)(record + 6) = facing;
    Map_AnimateCells(Sukureta_GateCells, 43, 8);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_SetSpeed(ACTOR_SUKURETA, 0xcccc, 0x6666);
    Actor_SetPosition(ACTOR_SUKURETA, 0xe60000, 0xdc0000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 230, 232);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
    Event_SetMessage((s32)MsgHaidiaSukuretaOurBestBet);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 10);
    Actor_SetAnimation(ACTOR_JASMINE, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_JASMINE, 0xcccc, 0x6666);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Value1((s32 (*)())Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_SetAnimation(ACTOR_JASMINE, 2);
    record = Value1((s32 (*)())Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_JASMINE, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    record = Value1((s32 (*)())Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_SUKURETA, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Actor_WaitForMove(ACTOR_SUKURETA);
    Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetAnimation(ACTOR_JASMINE, 1);
    Actor_SetAnimation(ACTOR_SUKURETA, 1);
    Actor_SetPosition(ACTOR_SATUROS, 0, 0);
    Actor_SetPosition(ACTOR_MENARDI, 0, 0);
    GameFlag_Set(FLAG_SANCTUM_VISIT_PLANNED);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    ColorBuffer_ApplySource(0x10000, 0);
    GameFlag_Set(0x242);
    Event_End();
}

void FieldScene_SetupWithDescriptorA0ACWhenFlag242Clear(void)
{
    if (GameFlag_IsSet(0x242) == 0) {
        Audio_PlayCue(0x9E);
        Map_AnimateCells(Sukureta_GateCells, 0x2B, 8);
    }
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0xE5, 0xD9);
    Event_RequestExit(3);
}
