#include "STORY.H"

/* Stages the scene transition and arms the timed callback that drives it. */
void StoryScene_StartTransition(void)
{
    Event_Begin();
    Audio_PlayCue(141);
    ColorBuffer_ApplySource(0, 0);
    ColorBuffer_ApplyTarget(0, 0);
    ColorBuffer_Interpolate(1);
    Task_Wait(2);
    *(s32 *)(*(u8 **)&gEventWork + 456) = 1;
    Event_OpenScreen();
    Event_WaitForScreen();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Task_Wait(1);
    Camera_SetSpeed(0x40000, 0x8000);
    {
        s32 transition_delay = 3200;
        void *transition_callback = (void *)StoryScene_UpdateTransitionEffect;
        Engine_TaskAddCallback(transition_callback, transition_delay);
    }
    ColorBuffer_ApplySource(0, 0);
    ColorBuffer_ApplyTarget(0x10004, 1);
    ColorBuffer_ApplyTarget(0x10000, 2);
    ColorBuffer_Interpolate(40);
    Event_Wait(240);
    ColorBuffer_ApplyTarget(0, 0);
    ColorBuffer_Interpolate(80);
    Task_Wait(90);
    Event_RequestExit(109);
    GameFlag_Set(282);
    Event_End();
}

enum {
    TRANSITION_EFFECT_TYPE = 222,
    TRANSITION_EFFECT_LEFT = 0x17b0,
    TRANSITION_EFFECT_TOP = 0x0c4c,
    TRANSITION_EFFECT_WIDTH = 40,
    TRANSITION_EFFECT_DEPTH = 30
};

/* The motion script each transition effect runs. */
extern const s32 gTransitionSparkScript[];

s32 Engine_MathModulo(u32 value, s32 divisor);

/*
 * The timed callback the transition schedules. Each run places one effect at
 * a random point of a 40 by 30 pixel area with a random scale, and on every
 * third frame moves the camera to one of four nearby points chosen at random.
 */
void StoryScene_UpdateTransitionEffect(void)
{
    u32 x = (u32)Random_Next() * TRANSITION_EFFECT_WIDTH >> 16;
    u32 z = (u32)Random_Next() * TRANSITION_EFFECT_DEPTH >> 16;
    struct FieldActor *object;

    object = Object_Create(TRANSITION_EFFECT_TYPE, PIXELS(x) + PIXELS(TRANSITION_EFFECT_LEFT), 0,
                           PIXELS(z) + PIXELS(TRANSITION_EFFECT_TOP));
    if (object != NULL) {
        struct FieldSprite *sprite = object->sprite;
        s32 scale = (((u32)Random_Next() << 15) >> 16) + 0x13333;

        sprite->flags = 0;
        sprite->priority = 2;
        object->motion_flags = 0;
        object->scale_x = scale;
        object->scale_y = scale;
        Object_SetAnimation(object, 1);
        Object_SetScript(object, gTransitionSparkScript);
    }
    if (Engine_MathModulo(gFrameCount, 3) == 0) {
        switch (((u32)Random_Next() << 2) >> 16) {
        case 0:
            Camera_MoveTo(PIXELS(0x17c7), -1, PIXELS(0x0c69), 1);
            break;
        case 1:
            Camera_MoveTo(PIXELS(0x17c9), -1, PIXELS(0x0c67), 1);
            break;
        case 2:
            Camera_MoveTo(PIXELS(0x17c9), -1, PIXELS(0x0c69), 1);
            break;
        case 3:
            Camera_MoveTo(PIXELS(0x17c7), -1, PIXELS(0x0c67), 1);
            break;
        }
    }
}

/* Drives actor 8 through a series of position/threshold setup calls and
 * advances the shared scene phase before the scene runs. */
void FieldScene_RunActorEightApproach(void)
{

    u32 i;
    s32 actor;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetAnimation(ACTOR, 2);
    Actor_SetPosition(ACTOR, 0x13080000, 0x3280000);
    actor = Actor_Get(ACTOR);
    {
        /* Write 0xa000 to the halfword at +6 of the actor record. */
        s32 value = 0xa000;

        *(u16 *)(actor + 6) = value;
    }
    Task_Wait(1);
    BattleFx_ScheduleRatioTransition(0x13333, 1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Camera_FollowActor(ACTOR, 1);
    Task_Wait(1);
    SCENE_PHASE = 0x100;
    Event_OpenScreen();
    Actor_SetSpeed(ACTOR, 0x6666, 0x3333);
    Actor_MoveToAndWait(ACTOR, 0x12d8, 0x2c8);
    Actor_MoveToAndWait(ACTOR, 0x12a8, 0x268);
    Actor_SetSpeed(ACTOR, 0x4ccc, 0x2666);
    Actor_MoveToAndWait(ACTOR, 0x12a8, 0x1d8);
    Actor_SetSpeed(ACTOR, 0x3333, 0x1999);
    Actor_MoveToAndWait(ACTOR, 0x1298, 0x1c8);
    Actor_SetSpeed(ACTOR, 0x1999, 0xccc);
    Actor_MoveToAndWait(ACTOR, 0x1298, 0x1b8);
    Actor_SetAnimation(ACTOR, 1);
    Event_Wait(40);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(110);
}

/* Totals slots 0 and 2 against slots 1 and 3 and returns the difference. */
s32 StoryScene_ComputeOpposingSlotDelta(void)
{
    s32 positive_total = StoryReward_LookupBySelection(0);
    s32 negative_total;

    positive_total += StoryReward_LookupBySelection(2);
    negative_total = StoryReward_LookupBySelection(1);
    negative_total += StoryReward_LookupBySelection(3);
    return positive_total - negative_total;
}

/* Complete reference-actor-54 selected-actor setup wrapper. */
s32 StoryReward_LookupBySelection(u32 selection)
{
    s32 flag_base = 0;
    u32 offset;

    switch (selection) {
    case 0:
        flag_base = 0x92C;
        break;
    case 1:
        flag_base = 0x935;
        break;
    case 2:
        flag_base = 0x917;
        break;
    case 3:
        flag_base = 0x990;
        break;
    }
    for (offset = 0; offset < 9; offset++) {
        if (GameFlag_IsSet(flag_base + offset) != 0) return gWorldMapRewards[offset];
    }
    return 0;
}
