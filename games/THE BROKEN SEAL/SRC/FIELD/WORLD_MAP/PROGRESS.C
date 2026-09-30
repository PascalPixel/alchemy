#include "STORY.H"

void FieldScene_RunScene371_0200357c(void)
{
    struct FieldActor *actor;
    s32 record;

    actor = (struct FieldActor *)Engine_ActorGet(8);
    Event_Wait(60);
    Event_Begin();
    BattleFx_ScheduleRatioTransition(0x9999, 1);
    actor->scale_x = 0x13333;
    actor->scale_y = 0x13333;
    Camera_FollowActor(8, 1);
    Task_Wait(1);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpriteFlags(record, 0);
    record = Actor_Get(8);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetSpeed(8, 0x6666, 0x3333);
    actor->unknown_64 = 0;
    Engine_ActorEnableActionCallback(8, (s32)gOpeningLeaderRise);
    Engine_TaskAddCallback((s32)WorldMap_SpawnActorEightPuff, 0xc80);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    ColorBuffer_ApplyTarget(0x10003, 1);
    gEventWork->transition_frames = 32;
    Event_OpenScreen();
    Event_Wait(120);
    BattleFx_ScheduleRatioTransition(0x16666, 0x12c);
    Event_Wait(0x10e);
    gEventWork->transition_frames = 16;
    *(u16 *)0x05000000 = 0x7fff;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(111);
}

void StoryScene_UpdateSelectedActorProgress(void)
{

    struct StorySelectionActor *actor;
    struct StoryProgressWork *scene;
    s32 progress;

    actor = Actor_Get(gGameState.selected_actor);
    scene = (struct StoryProgressWork *)gEventWork;
    actor->presentation = (u16)(*(volatile s32 *)&gFrameCount << 12);

    progress = GameFlag_GetByte(0x2f8);
    if (progress != 0) {
        if (progress == 1) {
            scene->state_one_marker = 99;
        } else if (GameFlag_IsSet(0x106) == 0) {
            progress -= 1;
        }
    }
    GameFlag_SetByte(0x2f8, progress);
}

void FieldScene_RunOpeningAuxiliarySequence(s32 a0)
{
    s32 rec2;
    struct FieldActor *actor;
    s32 record;
    u8 *p6;
    u8 *base;

    base = (u8 *)&gGameState;
    p6 = *(s32 *)(base + 500);
    actor = (struct FieldActor *)Engine_ActorGet((s32)p6);
    rec2 = GameFlag_IsSet(0x2f0);
    if (rec2 == 0) {
        Event_Begin();
        Actor_SetAttachedEffect((s32)p6, 0x101);
        Actor_SetAnimation((s32)p6, 9);
        record = Engine_ActorGet(a0);
        if (record != 0) {
            Actor_SetDestination((s32)p6, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove((s32)p6);
        Audio_PlayCue(244);
        Engine_TaskAddCallback((s32)StoryScene_UpdateSelectedActorProgress, 0xc80);
        actor->motion_flags = rec2;
        Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed + 0x200000, actor->z.fixed);
        Actor_WaitForMove((s32)p6);
        actor->velocity_y = rec2;
        actor->motion_flags = 4;
        *(u8 *)(base + 498) = 2;
        GameFlag_Set(0x2f0);
        GameFlag_SetByte(0x2f8, 180);
        Event_End();
        *(u16 *)((u8 *)gEventWork + 0x17c) = rec2;
    }
}

void StoryScene_SetReferenceActor(void)
{
    FieldScene_RunOpeningAuxiliarySequence(54);
}

void StoryScene_ActivateSharedState(void)
{
    gEffectWork->active = 1;
}

/* Publish the actor-98 scene state and restore its selected actor. */
void StoryScene_CompleteActor98(void)
{
    u8 *state;
    u8 *selected_actor;

    if (((struct StoryCompletionWork *)gEventWork)->scene_value == 99) {
        ((struct StoryCompletionWork *)gEventWork)->scene_value = 0;
    }
    GameFlag_Clear(0x2f0);
    GameFlag_Set(0x2f1);
    GameFlag_SetByte(0x2f8, 0);
    BattleFx_SetWeightedResult(98, 5);
    state = (u8 *)&gGameState;
    state[0x22b] = 3;
    BattleFx_SetWeightedResult(98, 7);
    selected_actor = Actor_Get(*(s32 *)(state + 500));
    selected_actor[85] = 2;
}
