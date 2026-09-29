#include "SORU.H"
extern u8 MsgSoruSukuretaFirstTimeAtSol[];

void Scene_EnterSolSanctum(void)
{
    s32 record;

    Event_Begin();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Event_OpenScreen();
    ((void (*)())Engine_ActorSetAnimation)(0, 0);
    Event_Wait(4);
    Camera_MoveTo(-1, -1, -1, 0);
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0x4c80000, -1, 0x880000, 1);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_SUKURETA, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_JASMINE, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_SUKURETA, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_JASMINE, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_GERALD, 0x9999, 0x4ccc);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    Actor_SetAnimation(ACTOR_JASMINE, 2);
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    Actor_SetDestinationOffset(ACTOR_GERALD, -16, 0);
    Actor_SetDestinationOffset(ACTOR_JASMINE, 16, 0);
    Actor_SetDestinationOffset(ACTOR_SUKURETA, 0, -32);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetAnimation(ACTOR_GERALD, 0);
    Actor_SetAnimation(ACTOR_JASMINE, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xc000, 0);
    Actor_WaitForMove(ACTOR_SUKURETA);
    Actor_SetAnimation(ACTOR_SUKURETA, 1);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 20);
    Actor_Jump(ACTOR_SUKURETA, 4, 20);
    Event_SetMessage((s32)MsgSoruSukuretaFirstTimeAtSol);
    Event_AskYesNo(0x4008, 0);
    Event_Wait(20);
    Camera_MoveTo(0x4c80000, -1, 0x940000, 1);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_SetAnimation(ACTOR_JASMINE, 2);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_JASMINE, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    record = Value1(Engine_ActorGet, 0);
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
    GameFlag_Set(FLAG_SOL_SANCTUM_ENTERED);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    Event_End();
}

