#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/*
 * The Elemental Star chamber collapses around the party leader and Gerald.
 * The Wise One appears, has the Elemental Star put back in its bag, and
 * helps them out as the volcano erupts.
 */

enum {
    ACTOR_WISE_ONE = 15,
    /* The actors Scene_UpdateFallingRocks moves. */
    ACTOR_FIRST_ROCK = 16,
    ROCK_COUNT = 16
};

enum {
    OBJECT_ELEMENTAL_STAR = 22,
    ITEM_ELEMENTAL_STAR = 222
};

enum {
    MSG_FRIENDS_GONE = 0x10fb,
    MSG_THANKS_A_LOT = 0x10fd,
    MSG_THEYLL_BE_SAFE = 0x10fe,
    MSG_THIS_IS_TERRIBLE = 0x10ff,
    MSG_QUIT_ACTING_TOUGH = 0x1103,
    MSG_OVER_HERE = 0x1104,
    MSG_CANNOT_RESIST = 0x110c,
    MSG_RETURN_STAR_TO_BAG = 0x110d
};

/* "This turned out badly", followed by the two answers to it. */
extern u8 SceneMessage_ActorOneChoiceBase;

struct QuakeWork {
    u8 unknown_000[0x40c];
    s32 unknown_40c;
};

extern struct QuakeWork *gQuakeWork;
extern s32 gRockfallFrameMask;

void State_UpdateRandomTimerLevel(void);
void Scene_UpdateFallingRocks(void);
void Actor_MoveTo232_125AndFace4000(s32 actor);
void State_ConfigureEightCornerRegions(void);
void Scene_RunVariantStep(s32 variant, s32 frames, s32 wait);
void Scene_RunStepByRuntimeBits(struct FieldActor *actor);
void State_SetValue140Mode0(void);
void State_ApplyRectsByCondition(s32 lit);
void State_ApplyRectPairByFlag(s32 lit);
void Scene_RunRandomHalfBranch(void);
void Scene_RunLateRandomHalfBranch(void);
void Scene_RunActor15TwoStep(void);
void Scene_RunFourWayEffectSequence(s32 corner);
void Actor_PlaceAtTileAndRunSteps(s32 x, s32 z);
void Scene_CallHelper6620(void);

void Scene_RunExtendedEffectPresentation(void)
{
    struct QuakeWork *quake;
    struct FieldActor *wise_one;
    struct FieldActor *center;
    struct FieldActor *leader;
    struct FieldActor *actor;
    struct FieldActor *garet;
    struct FieldSprite *leader_sprite;
    struct FieldSprite *garet_sprite;
    struct FieldActor *star;
    struct FieldSprite *sprite;
    u8 *buf;
    u8 i;
    u32 cnt;
    s32 wait;

    quake = gQuakeWork;
    wise_one = Actor_Get(ACTOR_WISE_ONE);
    Task_RemoveCallback(State_UpdateRandomTimerLevel);
    gRockfallFrameMask = 3;
    Event_Wait(80);
    Audio_PlayCue(17);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(40);
    Event_Wait(40);
    Task_RemoveCallback(Scene_UpdateFallingRocks);
    for (i = 0; i < ROCK_COUNT; i++) {
        Actor_SetPosition(ACTOR_FIRST_ROCK + i, 0, 0);
    }
    Task_Wait(1);
    Actor_SetPosition(ACTOR_WISE_ONE, 0, 0);
    Actor_MoveTo232_125AndFace4000(ACTOR_PARTY_LEADER);
    Actor_MoveTo232_125AndFace4000(ACTOR_GERALD);
    quake->unknown_40c = 0;
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Event_Wait(80);
    State_ConfigureEightCornerRegions();
    center = Event_GetViewCenter();
    center->motion_flags = 0;
    center->x.fixed = PIXELS(231);
    center->z.fixed = PIXELS(144);
    center->target_x = ACTOR_NO_TARGET;
    center->target_y = ACTOR_NO_TARGET;
    center->target_z = ACTOR_NO_TARGET;
    center->y.fixed = 0;
    center->velocity_x = 0;
    center->velocity_z = 0;
    Task_Wait(4);
    Map_Redraw();
    Task_Wait(4);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Actor_SetSpritePriority(ACTOR_GERALD, 3);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(40);
    Event_Wait(40);

    /* The two spin apart. */
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    garet = Actor_Get(ACTOR_GERALD);
    leader_sprite = leader->sprite;
    garet_sprite = garet->sprite;
    for (cnt = 0; cnt < 20; cnt++) {
        leader_sprite->rotation += 0x100;
        garet_sprite->rotation -= 0x100;
        leader->x.fixed += 0x6000;
        garet->x.fixed -= 0x6000;
        Task_Wait(1);
    }
    Event_Wait(40);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_SetSpeed(ACTOR_GERALD, 0x20000, 0x10000);
    Actor_Get(ACTOR_PARTY_LEADER)->sprite->rotation = 0;
    Actor_Get(ACTOR_GERALD)->sprite->rotation = 0;
    Engine_ObjectMotionLaunch(ACTOR_PARTY_LEADER, 6, 0);
    Engine_ObjectMotionLaunch(ACTOR_GERALD, 6, 0);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 246, 150);
    Engine_ObjectMotionSetPositionAndCommit(ACTOR_GERALD, 220, 150);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Actor_SetSpritePriority(ACTOR_GERALD, 2);
    Actor_Get(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_Get(ACTOR_GERALD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_Stop(ACTOR_PARTY_LEADER);
    Actor_Stop(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHEAST, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST + FACING_STEP, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 0);
    Event_Wait(80);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, FACING_EAST + FACING_STEP, 0);
    Event_Wait(60);

    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_SetMessage((s32)&SceneMessage_ActorOneChoiceBase);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
        Event_SetMessage((s32)&SceneMessage_ActorOneChoiceBase + 1);
    } else {
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        Event_SetMessage((s32)&SceneMessage_ActorOneChoiceBase + 2);
    }
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 60);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
    Event_Wait(40);
    Engine_ObjectMotionLaunch(ACTOR_GERALD, 2, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 0);
    Event_Wait(40);
    Event_SetMessage(MSG_FRIENDS_GONE);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Engine_ObjectMotionLaunch(ACTOR_PARTY_LEADER, 2, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Event_Wait(60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Event_Wait(80);
    Actor_FaceDirection(ACTOR_GERALD, FACING_EAST + FACING_STEP, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Event_Wait(10);
        Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
        Event_Wait(60);
        Actor_FaceDirection(ACTOR_GERALD, FACING_EAST + FACING_STEP, 0);
        Event_Wait(10);
        Actor_SetAnimation(ACTOR_GERALD, ANIM_SHAKE_HEAD);
        Event_SetMessage(MSG_THANKS_A_LOT);
    } else {
        Event_Wait(10);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Event_Wait(10);
        Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
        Event_Wait(60);
        Actor_FaceDirection(ACTOR_GERALD, FACING_EAST + FACING_STEP, 0);
        Event_Wait(10);
        Actor_SetAnimation(ACTOR_GERALD, ANIM_SHAKE_HEAD);
        Event_SetMessage(MSG_THEYLL_BE_SAFE);
    }
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
    Event_Wait(40);

    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHWEST + FACING_STEP, 0);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(PIXELS(284), -1, PIXELS(92), 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 0);
    Camera_SetSpeed(0x18000, 0x3000);
    Camera_MoveTo(PIXELS(127), -1, PIXELS(162), 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(PIXELS(304), -1, PIXELS(294), 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_EAST + FACING_STEP, 0);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(PIXELS(400), -1, PIXELS(215), 1);
    Camera_WaitForMove();
    Event_Wait(60);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(PIXELS(273), -1, PIXELS(145), 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_SetMessage(MSG_THIS_IS_TERRIBLE);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH + FACING_STEP, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHEAST + FACING_STEP, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHEAST + FACING_STEP, 0);
    Event_Wait(10);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(10);

    Audio_PlayCue(23);
    Scene_RunVariantStep(1, 4, 0);
    quake->unknown_40c = 0;
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
    Event_Wait(10);
    Scene_RunVariantStep(0, 40, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_SetSpeed(ACTOR_GERALD, 0x20000, 0x10000);
    Engine_ObjectMotionLaunch(ACTOR_PARTY_LEADER, 6, 0);
    Engine_ObjectMotionLaunch(ACTOR_GERALD, 6, 0);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 243, 144);
    Actor_SetDestination(ACTOR_GERALD, 202, 144);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Event_Wait(20);
    quake->unknown_40c = 1;
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Event_Wait(60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 0);
    Event_Wait(80);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Actor_SetSpeed(ACTOR_GERALD, 0x6666, 0x3333);
    Actor_WalkTo(ACTOR_GERALD, 220, 150);
    Engine_ObjectMotionSetPositionAndReset(ACTOR_PARTY_LEADER, 246, 150);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_STAND);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHWEST + FACING_STEP, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
    Event_Wait(40);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_FaceDirection(ACTOR_GERALD, FACING_EAST + FACING_STEP, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
    } else {
        Actor_FaceDirection(ACTOR_GERALD, FACING_EAST + FACING_STEP, 0);
        Event_Wait(10);
        Actor_SetAnimation(ACTOR_GERALD, ANIM_SHAKE_HEAD);
        Event_SetMessage(MSG_QUIT_ACTING_TOUGH);
    }
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);

    /* The Wise One arrives. */
    Actor_SetPosition(ACTOR_WISE_ONE, PIXELS(110), PIXELS(152));
    Task_Wait(1);
    Actor_SetSpeed(ACTOR_WISE_ONE, 0x13333, 0x9999);
    Actor_SetDestination(ACTOR_WISE_ONE, 171, 152);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST, 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_WISE_ONE), 1);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_GERALD, 217, 182);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST + FACING_STEP, 0);
    Event_Wait(10);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(PIXELS(217), -1, PIXELS(176), 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
    Event_Wait(20);
    Engine_ObjectMotionLaunch(ACTOR_PARTY_LEADER, 2, 0);
    Event_Wait(10);
    Engine_ObjectMotionLaunch(ACTOR_PARTY_LEADER, 4, 0);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT, 0);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_STAND);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST, 0);
    Event_Wait(4);
    Engine_ObjectMotionSetPositionAndReset(ACTOR_GERALD, 231, 175);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHEAST, 0);
    Event_SetMessage(MSG_OVER_HERE);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 1, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHWEST, 0);
    Event_Wait(60);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_SetAttachedEffect(ACTOR_GERALD, EMOTE_IN_FRONT | 2);
    Event_Wait(40);

    Scene_RunVariantStep(1, 20, 0);
    quake->unknown_40c = 0;
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Event_Wait(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Audio_PlayCue(0x121);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(40);
    quake->unknown_40c = 1;
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(120);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 0);
    Event_Wait(100);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(20);
    Scene_RunVariantStep(1, 10, 0);
    quake->unknown_40c = 0;
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_SetSpeed(ACTOR_GERALD, 0x20000, 0x10000);
    Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a &= ~1;
    Actor_Get(ACTOR_GERALD)->unknown_5a &= ~1;
    Engine_ObjectMotionLaunch(ACTOR_PARTY_LEADER, 4, 0);
    Engine_ObjectMotionLaunch(ACTOR_GERALD, 4, 0);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 256, 150);
    Actor_SetDestination(ACTOR_GERALD, 231, 180);
    Actor_WaitForMove(ACTOR_GERALD);
    Scene_RunVariantStep(0, 40, 0);
    Event_Wait(20);
    Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a |= 1;
    Actor_Get(ACTOR_GERALD)->unknown_5a |= 1;
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 3, 0);
    Event_Wait(40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_WISE_ONE, EMOTE_IN_FRONT | 1, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH + FACING_STEP, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHWEST + FACING_STEP, 0);
    Event_Wait(10);

    Audio_PlayCue(107);
    Task_AddCallback(Scene_RunRandomHalfBranch, TASK_PRIORITY_SCENE);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_WISE_ONE, EMOTE_IN_FRONT, 0);
    Event_Wait(40);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(PIXELS(186), -1, PIXELS(166), 1);
    Actor_SetDestination(ACTOR_WISE_ONE, 130, 113);
    Actor_WaitForMove(ACTOR_WISE_ONE);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTHEAST + FACING_STEP, 0);
    Event_Wait(20);
    quake->unknown_40c = 0;
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    ColorBuffer_ApplyTarget(0x20119e, 1);
    ColorBuffer_Interpolate(20);
    Event_Wait(20);
    State_SetValue140Mode0();
    Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 2);
    for (cnt = 0; cnt < 40; cnt++) {
        Scene_RunStepByRuntimeBits(wise_one);
        Task_Wait(1);
    }
    Task_AddCallback(Scene_RunActor15TwoStep, TASK_PRIORITY_SCENE);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(60);
    Event_Wait(30);
    Audio_PlayCue(0x121);
    Task_RemoveCallback(Scene_RunRandomHalfBranch);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    wait = 16;
    while (wait--) {
        State_ApplyRectsByCondition(0);
        Task_Wait(wait);
        State_ApplyRectsByCondition(1);
        Task_Wait(wait);
    }
    State_ApplyRectsByCondition(0);
    quake->unknown_40c = 1;
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_MapWaitWorkValuesBelow256();
    Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 3);
    Task_RemoveCallback(Scene_RunActor15TwoStep);
    Task_Wait(1);
    Actor_SetChildValue(ACTOR_WISE_ONE, 0);
    Event_Wait(60);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 0);
    Event_Wait(60);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(PIXELS(218), -1, PIXELS(181), 1);
    Actor_SetSpeed(ACTOR_WISE_ONE, 0x10000, 0x8000);
    Actor_SetDestination(ACTOR_WISE_ONE, 169, 151);
    Actor_WaitForMove(ACTOR_WISE_ONE);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST + FACING_STEP, 0);
    Event_Wait(40);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH + FACING_STEP, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_NORTHEAST + FACING_STEP, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHWEST + FACING_STEP, 0);
    Camera_MoveTo(PIXELS(224), -1, PIXELS(158), 1);
    Camera_WaitForMove();
    Scene_RunFourWayEffectSequence(0);
    Actor_RunRepeatedMotion(ACTOR_WISE_ONE, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(40);
    Event_OpenMessage(ACTOR_WISE_ONE, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(40);
    } else {
        Message_ShowCentered(MSG_CANNOT_RESIST, 1);
        Event_Wait(40);
    }

    /* The Elemental Star rises from the party leader. */
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    star = Object_Create(OBJECT_ELEMENTAL_STAR, actor->x.fixed, actor->y.fixed + PIXELS(36),
                         actor->z.fixed);
    if (star != NULL) {
        buf = Heap_Allocate(17, 0x608);
        sprite = star->sprite;
        sprite->flags = 0;
        sprite->part_count = 0;
        sprite->full_color = 0;
        sprite->palette = 0;
        sprite->priority = 1;
        Item_LoadIcon(ITEM_ELEMENTAL_STAR);
        Vram_Load(sprite->vram_block, 128, buf + 0x400);
        Heap_Release(17);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 28);
        Engine_RunRisingObjectSequence(star, 3);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 28);
    }
    Scene_RunVariantStep(1, 20, 0);
    for (cnt = 0; cnt < 24; cnt++) {
        Scene_RunStepByRuntimeBits(wise_one);
        Task_Wait(1);
        Scene_RunStepByRuntimeBits(wise_one);
        Task_Wait(1);
        star->scale_x = 0x6666;
        star->scale_y = 0x6666;
        Scene_RunStepByRuntimeBits(wise_one);
        Task_Wait(1);
        Scene_RunStepByRuntimeBits(wise_one);
        Task_Wait(1);
        star->scale_x = 0x10000;
        star->scale_y = 0x10000;
    }
    Actor_SetChildValue(ACTOR_WISE_ONE, 0);
    Scene_RunVariantStep(0, 20, 0);
    Event_SetMessage(MSG_RETURN_STAR_TO_BAG);
    Event_ShowMessageAndWait(ACTOR_WISE_ONE, 0, 20);
    if (star != NULL) {
        Engine_ObjectDispatchRelease(star);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHWEST, 60);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(40);

    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
    Camera_SetSpeed(0x40000, 0x8000);
    Actor_PlaceAtTileAndRunSteps(232, 464);
    Scene_RunFourWayEffectSequence(1);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Actor_PlaceAtTileAndRunSteps(711, 144);
    Scene_RunFourWayEffectSequence(2);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Actor_PlaceAtTileAndRunSteps(711, 464);
    Scene_RunFourWayEffectSequence(3);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST + FACING_STEP, 0);
    Actor_SetPosition(ACTOR_GERALD, PIXELS(582), PIXELS(345));
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_SetPosition(ACTOR_GERALD, PIXELS(231), PIXELS(180));
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHWEST + FACING_STEP, 0);
    Task_Wait(20);
    Actor_PlaceAtTileAndRunSteps(219, 171);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(40);

    quake->unknown_40c = 0;
    Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    ColorBuffer_ApplyTarget(0x20119e, 1);
    ColorBuffer_Interpolate(20);
    Event_Wait(20);
    Audio_PlayCue(107);
    Task_AddCallback(Scene_RunLateRandomHalfBranch, TASK_PRIORITY_SCENE);
    Event_Wait(20);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(PIXELS(184), -1, PIXELS(132), 1);
    Engine_ObjectMotionLaunch(ACTOR_PARTY_LEADER, 6, 0);
    Engine_ObjectMotionLaunch(ACTOR_GERALD, 6, 0);
    Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a &= ~1;
    Actor_Get(ACTOR_GERALD)->unknown_5a &= ~1;
    Actor_SetDestination(ACTOR_PARTY_LEADER, 245, 145);
    Actor_SetDestination(ACTOR_GERALD, 215, 168);
    Actor_WaitForMove(ACTOR_GERALD);
    Event_Wait(80);
    Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a |= 1;
    Actor_Get(ACTOR_GERALD)->unknown_5a |= 1;
    Actor_SetDestination(ACTOR_WISE_ONE, 184, 87);
    Actor_WaitForMove(ACTOR_WISE_ONE);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 2);
    for (cnt = 0; cnt < 40; cnt++) {
        Scene_RunStepByRuntimeBits(wise_one);
        Task_Wait(1);
    }
    Task_AddCallback(Scene_RunActor15TwoStep, TASK_PRIORITY_SCENE);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(60);
    Event_Wait(30);
    Audio_PlayCue(0x121);
    Task_RemoveCallback(Scene_RunLateRandomHalfBranch);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    wait = 8;
    while (wait--) {
        State_ApplyRectPairByFlag(0);
        Task_Wait(wait);
        State_ApplyRectPairByFlag(1);
        Task_Wait(wait);
    }
    State_ApplyRectPairByFlag(0);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 3);
    Task_RemoveCallback(Scene_RunActor15TwoStep);
    Task_Wait(1);
    Actor_SetChildValue(ACTOR_WISE_ONE, 0);
    Event_Wait(60);

    Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    ColorBuffer_ApplyTarget(0x20119e, 1);
    ColorBuffer_Interpolate(20);
    Event_Wait(20);
    Audio_PlayCue(107);
    Task_AddCallback(Scene_RunRandomHalfBranch, TASK_PRIORITY_SCENE);
    Event_Wait(40);
    Engine_ObjectMotionSetPositionAndCommit(ACTOR_WISE_ONE, 127, 110);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 2);
    for (cnt = 0; cnt < 40; cnt++) {
        Scene_RunStepByRuntimeBits(wise_one);
        Task_Wait(1);
    }
    Task_AddCallback(Scene_RunActor15TwoStep, TASK_PRIORITY_SCENE);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(60);
    Audio_PlayCue(0x121);
    Event_Wait(30);
    Task_RemoveCallback(Scene_RunRandomHalfBranch);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    wait = 8;
    while (wait--) {
        State_ApplyRectsByCondition(0);
        Task_Wait(wait);
        State_ApplyRectsByCondition(1);
        Task_Wait(wait);
    }
    State_ApplyRectsByCondition(0);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Audio_PlayCue(107);
    Audio_PlayCue(63);
    Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    ColorBuffer_ApplyTarget(0x20119e, 1);
    ColorBuffer_Interpolate(20);
    Event_Wait(20);
    Audio_PlayCue(107);
    Task_AddCallback(Scene_RunLateRandomHalfBranch, TASK_PRIORITY_SCENE);
    Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 3);
    Task_RemoveCallback(Scene_RunActor15TwoStep);
    Task_Wait(1);
    Actor_SetChildValue(ACTOR_WISE_ONE, 0);
    Event_Wait(60);
    Engine_ObjectMotionSetPositionAndCommit(ACTOR_WISE_ONE, 184, 87);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 3);
    Task_RemoveCallback(Scene_RunActor15TwoStep);
    Task_Wait(1);
    Actor_SetChildValue(ACTOR_WISE_ONE, 0);
    Audio_PlayCue(141);
    Event_Wait(100);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 0);
    Event_Wait(60);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(20);
    Audio_PlayCue(0x121);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTHEAST + FACING_STEP, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(20);
    for (cnt = 0; cnt < 40; cnt++) {
        Scene_RunStepByRuntimeBits(wise_one);
        Task_Wait(1);
    }
    Task_AddCallback(Scene_RunActor15TwoStep, TASK_PRIORITY_SCENE);
    Event_Wait(20);
    ColorBuffer_ApplyTarget(0x7fff, 2);
    ColorBuffer_Interpolate(60);
    Task_Wait(100);
    ColorBuffer_ApplyTarget(0x7fff, 1);
    ColorBuffer_Interpolate(60);
    Task_Wait(60);
    Task_RemoveCallback(Scene_RunActor15TwoStep);
    quake->unknown_40c = 1;
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_MapWaitWorkValuesBelow256();
    Scene_CallHelper6620();
    GameFlag_Set(0x814);
    GameFlag_Set(0x83f);
    Event_RequestExit(5);
    GameFlag_Set(0x100);
}
