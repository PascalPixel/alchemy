/* NONMATCHING: complete 4708-byte owner; candidate 4704 bytes,
 * 1704 differing halfwords / 610 aligned edits. The reference has a
 * 136-byte frame; this draft has 120. Icon-buffer and particle-origin
 * spills remain unresolved. Prior experiments are preserved in Git. */
#include "TYPES.H"
#include "VINASU.H"
#include "FIELD_EFFECT.H"
#include "FIELD_EVENT.H"
extern u8 MsgVinasuBeatEm[];

extern u8 LinkedValue_Zero;
extern u8 LinkedScene_VinasuChojo;
extern const u8 Data_0200e074[];
extern const u8 Data_0200e088[];
extern const u8 Data_0200e0ac[];
extern const u8 Data_0200e22c[];

void Actor_ParkRecord(struct FieldActor *object);
void Effect_MoveWithDrag(union FieldObject *object);
void SceneTask_020036d0(void);

void Engine_ActorLaunch(s32 actor, s32 speed, s32 frames);
void Engine_ActorSetPositionAndReset(s32 actor, s32 x, s32 z);
void Engine_ActorSetPositionAndCommit(s32 actor, s32 x, s32 z);
void Engine_ObjectSetPosition(struct FieldActor *object, s32 fixed_x, s32 fixed_y, s32 fixed_z);
void Engine_ObjectCommitPosition(struct FieldActor *object);
void Engine_GameStateSetReturn(s32 scene, s32 entrance);
void Engine_GameStateSetWarp(s32 scene, s32 entrance);
void Engine_Import0808a250(s32 first, s32 second);
void Engine_Import08077268(void);

static inline void Actor_Launch(s32 actor, s32 speed, s32 frames)
{
    Engine_ActorLaunch(actor, speed, frames);
}

static inline void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z)
{
    Engine_ActorSetPositionAndReset(actor, x, z);
}

static inline void Actor_SetPositionAndCommit(s32 actor, s32 x, s32 z)
{
    Engine_ActorSetPositionAndCommit(actor, x, z);
}

static inline void Object_SetPosition(struct FieldActor *object, s32 fixed_x, s32 fixed_y,
                                      s32 fixed_z)
{
    Engine_ObjectSetPosition(object, fixed_x, fixed_y, fixed_z);
}

static inline void Object_CommitPosition(struct FieldActor *object)
{
    Engine_ObjectCommitPosition(object);
}

/* Both bursts use one velocity scratch; only its polar construction is
 * shared, leaving the two option records and loop lifetimes in the scene. */
static __inline__ void MakeBurstVelocity(s32 velocity[3], s32 angle)
{
    velocity[0] = Math_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle) * 2;
    velocity[0] *= 3;
}

enum {
    ITEM_VENUS_STAR = 220
};

/* The defeated pair light the beacon, recover, then begin their transformation. */
void Scene_RunPairedActorEffectSequence(void)
{
    struct EffectOptions second_burst;
    s32 velocity[3];
    struct EffectOptions first_burst;
    struct FieldActor *actor;
    struct FieldActor *origin;
    union FieldObject *star;
    struct FieldActor *bird;
    struct FieldSprite *sprite;
    u8 *buffer;
    s32 yes;
    u32 i;

    Event_Begin();
    Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
    Actor_Get(ACTOR_GERALD)->facing = FACING_WEST;
    Actor_SetPosition(ACTOR_GERALD, PIXELS(328), PIXELS(168));
    Actor_Get(ACTOR_IVAN)->facing = FACING_WEST;
    Actor_SetPosition(ACTOR_IVAN, PIXELS(340), PIXELS(196));
    Actor_Get(ACTOR_MIA)->facing = FACING_WEST;
    Actor_SetPosition(ACTOR_MIA, PIXELS(326), PIXELS(204));
    Actor_Get(6)->facing = FACING_SOUTHEAST + FACING_STEP;
    Actor_SetPosition(6, PIXELS(268), PIXELS(154));
    Actor_Get(21)->facing = FACING_SOUTHEAST + FACING_STEP;
    Actor_SetPosition(21, PIXELS(268), PIXELS(164));
    actor = Actor_Get(ACTOR_FIRST_OF_PAIR);
    actor->unknown_64 = 10;
    actor->facing = FACING_NORTH + FACING_STEP;
    Actor_SetPosition(ACTOR_FIRST_OF_PAIR, PIXELS(294), PIXELS(212));
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 9);
    Actor_EnableActionCallback(ACTOR_FIRST_OF_PAIR, Data_0200e074);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_OF_PAIR), 0);
    actor = Actor_Get(ACTOR_SECOND_OF_PAIR);
    actor->unknown_64 = 10;
    actor->facing = FACING_EAST;
    Actor_SetPosition(ACTOR_SECOND_OF_PAIR, PIXELS(286), PIXELS(192));
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 7);
    Actor_EnableActionCallback(ACTOR_SECOND_OF_PAIR, Data_0200e074);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_OF_PAIR), 0);
    Event_GetViewCenter()->motion_flags = 0;
    Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 0);
    Task_Wait(1);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(80);

    Actor_Launch(ACTOR_GERALD, 2, 20);
    VinasuChojo_FaceActor(ACTOR_GERALD, FACING_SOUTHEAST);
    Event_SetMessage((s32)MsgVinasuBeatEm);
    VinasuChojo_ShowMessage(0x1001);
    VinasuChojo_FaceActor(ACTOR_MIA, FACING_NORTHWEST);
    VinasuChojo_ShowMessage(ACTOR_MIA);
    Actor_TurnToAngle(ACTOR_GERALD, FACING_WEST, 0);
    VinasuChojo_FaceActor(ACTOR_IVAN, FACING_NORTHWEST);
    Actor_RunRepeatedMotion(21, 2);
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_Get(21)->unknown_5a &= ~1;
    Actor_SetPositionAndReset(21, 0x118, 164);
    Event_Wait(1);
    Actor_Get(21)->unknown_5a |= 1;
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 40);
    VinasuChojo_ShowMessage(ACTOR_IVAN);
    Actor_SetAnimationAndWait(21, 4);
    VinasuChojo_ShowMessage(21);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    VinasuChojo_ShowMessage(ACTOR_GERALD);
    Actor_RunRepeatedMotion(21, 1);
    VinasuChojo_FaceActor(21, FACING_EAST);
    VinasuChojo_ShowMessage(21);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    VinasuChojo_ShowMessage(ACTOR_MIA);
    Actor_SetAnimation(ACTOR_MIA, 4);
    VinasuChojo_ShowMessage(ACTOR_MIA);
    Actor_ShowEmote(21, 0x105, 40);
    VinasuChojo_FaceActor(21, FACING_SOUTHEAST + FACING_STEP);
    VinasuChojo_ShowMessage(21);
    Actor_Launch(ACTOR_IVAN, 2, 20);
    VinasuChojo_ShowMessage(ACTOR_IVAN);
    Actor_SetAnimationAndWait(21, 3);
    VinasuChojo_ShowMessage(21);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    VinasuChojo_FaceActor(ACTOR_GERALD, FACING_SOUTHEAST);
    Event_OpenMessage(ACTOR_GERALD, 0);
    Actor_TurnToAngle(ACTOR_IVAN, FACING_NORTHEAST, 0);
    Actor_TurnToAngle(ACTOR_MIA, FACING_NORTHEAST, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        yes = TRUE;
    } else {
        gEventWork->message++;
        yes = FALSE;
    }
    Task_Wait(20);
    Actor_TurnToAngle(ACTOR_GERALD, FACING_WEST, 0);
    Actor_TurnToAngle(ACTOR_IVAN, FACING_NORTHWEST, 0);
    VinasuChojo_FaceActor(ACTOR_MIA, FACING_NORTHWEST);
    VinasuChojo_FaceActor(21, FACING_EAST);
    Event_ShowMessage(21, 0);
    Audio_PlayCue(17);
    Event_Wait(40);
    if (yes) {
        gEventWork->message++;
    }

    /* The pair get up and throw the Venus Star into the beacon. */
    Actor_Stop(ACTOR_FIRST_OF_PAIR);
    Actor_Stop(ACTOR_SECOND_OF_PAIR);
    Task_Wait(1);
    actor = Actor_Get(ACTOR_FIRST_OF_PAIR);
    actor->scale_x = 0x10000;
    actor->scale_y = 0x10000;
    actor = Actor_Get(ACTOR_SECOND_OF_PAIR);
    actor->scale_x = 0x10000;
    actor->scale_y = 0x10000;
    Task_Wait(1);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_TurnToAngle(21, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_TurnToAngle(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_TurnToAngle(ACTOR_GERALD, FACING_SOUTHWEST, 0);
    Actor_TurnToAngle(ACTOR_IVAN, FACING_WEST, 0);
    Actor_TurnToAngle(ACTOR_MIA, FACING_WEST, 40);
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 1);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_OF_PAIR), 1);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_FIRST_OF_PAIR, 0x3333, 0x1999);
    Actor_SetPositionAndReset(ACTOR_FIRST_OF_PAIR, 0x12c, 206);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 2);
    Event_Wait(20);
    Actor_Get(ACTOR_FIRST_OF_PAIR)->scale_x = -0x10000;
    Audio_PlayCue(161);
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 8);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 1);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_OF_PAIR), 1);
    Actor_Launch(ACTOR_SECOND_OF_PAIR, 4, 40);
    Actor_SetSpeed(ACTOR_SECOND_OF_PAIR, 0x3333, 0x1999);
    Actor_Get(ACTOR_SECOND_OF_PAIR)->unknown_5a &= ~1;
    Actor_SetPositionAndReset(ACTOR_SECOND_OF_PAIR, 0x128, 186);
    Actor_SetSpeed(ACTOR_SECOND_OF_PAIR, 0x1999, 0xccc);
    Actor_SetPositionAndReset(ACTOR_SECOND_OF_PAIR, 0x124, 186);
    Actor_Get(ACTOR_SECOND_OF_PAIR)->unknown_5a |= 1;
    Actor_Get(ACTOR_SECOND_OF_PAIR)->scale_x = -0x10000;
    Audio_PlayCue(161);
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 5);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 2);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
    Event_Wait(80);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 2);
    Event_Wait(40);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    Actor_TurnToAngle(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Actor_TurnToAngle(ACTOR_GERALD, FACING_SOUTHEAST, 0);
    Actor_TurnToAngle(ACTOR_IVAN, FACING_SOUTHWEST, 0);
    Actor_TurnToAngle(ACTOR_MIA, FACING_NORTHEAST, 40);
    Actor_TurnToAngle(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_TurnToAngle(ACTOR_GERALD, FACING_SOUTHWEST, 0);
    Actor_TurnToAngle(ACTOR_IVAN, FACING_WEST, 0);
    Actor_TurnToAngle(ACTOR_MIA, FACING_WEST, 20);

    actor = Actor_Get(ACTOR_FIRST_OF_PAIR);
    actor->facing = FACING_SOUTHEAST + FACING_STEP;
    actor->scale_x = 0x10000;
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 1);
    Event_Wait(10);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTH + FACING_STEP);
    Actor_Launch(ACTOR_FIRST_OF_PAIR, 6, 0);
    Event_Wait(10);
    Actor_TurnToAngle(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Actor_TurnToAngle(ACTOR_GERALD, FACING_NORTHWEST, 0);
    Actor_TurnToAngle(ACTOR_IVAN, FACING_NORTHWEST, 0);
    Actor_TurnToAngle(ACTOR_MIA, FACING_NORTHWEST, 0);
    Actor_TurnToAngle(21, FACING_NORTH + FACING_STEP, 0);
    Actor_TurnToAngle(6, FACING_EAST, 0);
    star = (union FieldObject *)Object_Create(22, actor->x.fixed, actor->y.fixed + PIXELS(8), actor->z.fixed);
    if (star != NULL) {
        sprite = star->actor.sprite;
        sprite->part_count = 0;
        sprite->full_color = 0;
        sprite->palette = 0;
        sprite->priority = 0;
        star->actor.priority_flags &= ~1;
        star->actor.motion_flags = 0;
        star->actor.unknown_5c = 1;
        star->actor.speed = 0x19999;
        star->actor.acceleration = 0xcccc;
        buffer = Heap_Allocate(17, 0x608);
        Item_LoadIcon(ITEM_VENUS_STAR);
        Vram_Load(sprite->vram_block, 128, buffer + 0x400);
        Heap_Release(17);
    }
    Actor_SetSpritePriority(22, 1);
    bird = Actor_Get(22);
    bird->x.fixed = actor->x.fixed;
    bird->y.fixed = PIXELS(32);
    bird->z.fixed = actor->z.fixed;
    bird->motion_flags = 3;
    bird->speed = 0x19999;
    bird->acceleration = 0xcccc;
    bird->scale_x = 0xc000;
    bird->scale_y = 0xc000;
    if (star != NULL) {
        star->actor.motion_flags = 3;
        star->effect.velocity_y = 0x9999;
        star->effect.velocity_x = 0xcccc;
        star->actor.velocity_y = PIXELS(8);
        Object_SetPosition(&star->actor, PIXELS(308), PIXELS(32), PIXELS(164));
    }
    Actor_SetPositionAndCommit(22, 0x134, 164);
    Actor_SetPosition(22, 0, 0);
    Actor_SetSpritePriority(21, 0);
    if (star != NULL) {
        Audio_PlayCue(0x135);
        Actor_SetSpriteFlags(&star->actor, 0);
        star->actor.velocity_y = PIXELS(4);
        Object_SetPosition(&star->actor, PIXELS(314), PIXELS(32), PIXELS(137));
        Object_CommitPosition(&star->actor);
        Audio_PlayCue(0x135);
        star->actor.sprite->priority = 1;
        star->actor.velocity_y = PIXELS(6);
        Object_SetPosition(&star->actor, PIXELS(285), PIXELS(32), PIXELS(146));
        Object_CommitPosition(&star->actor);
        Audio_PlayCue(0x135);
        star->actor.velocity_y = PIXELS(5);
        Object_SetPosition(&star->actor, PIXELS(300), PIXELS(32), PIXELS(154));
        Object_CommitPosition(&star->actor);
        Task_Wait(6);
        star->actor.x.fixed = 0;
        star->actor.y.fixed = 0;
        star->actor.z.fixed = 0;
        Actor_ParkRecord(&star->actor);
    }
    Actor_ShowEmote(21, 0x100, 0);
    Actor_ShowEmote(6, 0x100, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 30);
    Actor_SetSpritePriority(21, 1);
    Actor_Get(21)->priority_flags |= 1;
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    VinasuChojo_ShowMessage(ACTOR_IVAN);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 40);
    Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
    Event_Wait(20);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_TurnToAngle(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
    Actor_TurnToAngle(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 0);
    Actor_TurnToAngle(ACTOR_IVAN, FACING_WEST, 0);
    Actor_TurnToAngle(ACTOR_MIA, FACING_WEST, 0);
    Actor_TurnToAngle(6, FACING_SOUTHEAST + FACING_STEP, 0);
    VinasuChojo_FaceActor(21, FACING_SOUTHEAST + FACING_STEP);
    Actor_ShowEmote(21, 0x101, 0);
    Actor_ShowEmote(6, 0x101, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x108, 40);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_TurnToAngle(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Actor_TurnToAngle(ACTOR_GERALD, FACING_SOUTHEAST, 40);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimation(ACTOR_GERALD, ANIM_NOD);
        yes = TRUE;
    } else {
        gEventWork->message++;
        Actor_SetAnimation(ACTOR_GERALD, ANIM_SHAKE_HEAD);
        yes = FALSE;
    }
    VinasuChojo_ShowMessage(ACTOR_GERALD);
    if (yes) {
        gEventWork->message++;
    }
    Event_Wait(20);

    /* Raise the two light shafts and brighten the scene in stages. */
    Actor_SetSpriteFlags(Actor_Get(24), 0);
    Actor_SetChildValue(24, 7);
    actor = Actor_Get(24);
    actor->scale_y = -0x10000;
    actor->scale_x = 0x1999;
    actor->motion_flags = 0;
    actor->y.fixed = PIXELS(64);
    actor->z.fixed = PIXELS(158);
    actor->x.fixed = PIXELS(304);
    Actor_SetSpriteFlags(Actor_Get(25), 0);
    Actor_SetChildValue(25, 7);
    actor = Actor_Get(25);
    actor->scale_y = -0x10000;
    actor->scale_x = 0x1999;
    actor->motion_flags = 0;
    actor->y.fixed = PIXELS(96);
    actor->x.fixed = PIXELS(304);
    actor->z.fixed = PIXELS(158);
    Actor_ShowEmote(21, 0x100, 0);
    Actor_ShowEmote(6, 0x100, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 0);
    Actor_TurnToAngle(ACTOR_GERALD, FACING_NORTHWEST, 0);
    Actor_TurnToAngle(ACTOR_IVAN, FACING_NORTHWEST, 0);
    Actor_TurnToAngle(ACTOR_MIA, FACING_NORTHWEST, 0);
    Actor_TurnToAngle(21, FACING_NORTH + FACING_STEP, 0);
    Actor_TurnToAngle(6, FACING_EAST, 0);
    Actor_EnableActionCallback(24, Data_0200e088);
    Actor_EnableActionCallback(25, Data_0200e088);
    Audio_PlayCue(145);
    MapRender_SetValues(0x60000, 0x60000, 0x10000);
    ColorBuffer_ApplyTarget(0x4063ff, 0);
    ColorBuffer_Interpolate(16);
    Task_Wait(20);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(24);
    Task_Wait(60);
    Task_AddCallback(SceneTask_020036d0, TASK_PRIORITY_SCENE);
    Audio_PlayCue(141);
    MapRender_SetValues(0x40000, 0x40000, 0x10000);
    ColorBuffer_ApplyTarget(0x4063ff, 0);
    ColorBuffer_Interpolate(120);
    Actor_EnableActionCallback(24, Data_0200e0ac);
    Actor_EnableActionCallback(25, Data_0200e0ac);
    Task_Wait(120);
    MapRender_SetValues(0x30000, 0x30000, 0x10000);
    ColorBuffer_ApplyTarget(0x203210, 0);
    ColorBuffer_Interpolate(120);
    Task_Wait(120);
    Audio_PlayCue(63);
    MapRender_SetValues(0x20000, 0x20000, 0x10000);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(120);
    Task_Wait(120);
    MapRender_SetValues(0x10000, 0x10000, 0x10000);

    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 1);
    Actor_TurnToAngle(ACTOR_SECOND_OF_PAIR, FACING_EAST, 0);
    Actor_Get(ACTOR_SECOND_OF_PAIR)->scale_x = 0x10000;
    Actor_Launch(ACTOR_SECOND_OF_PAIR, 4, 40);
    Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
    Event_ShowMessageAndWait(ACTOR_SECOND_OF_PAIR, 0, 20);
    Actor_TurnToAngle(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_TurnToAngle(ACTOR_GERALD, FACING_SOUTHWEST, 0);
    Actor_TurnToAngle(ACTOR_IVAN, FACING_WEST, 0);
    Actor_TurnToAngle(ACTOR_MIA, FACING_WEST, 0);
    Actor_TurnToAngle(21, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_TurnToAngle(6, FACING_SOUTHEAST + FACING_STEP, 20);
    Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    VinasuChojo_FaceActor(ACTOR_MIA, FACING_WEST);
    VinasuChojo_ShowMessage(ACTOR_MIA);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_EAST);
    Actor_SetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 4);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x103, 20);
    Actor_StartRepeatedMotion(ACTOR_SECOND_OF_PAIR, 2);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 20);
    VinasuChojo_FaceActor(ACTOR_GERALD, FACING_SOUTHWEST);
    VinasuChojo_ShowMessage(ACTOR_GERALD);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTH + FACING_STEP);
    Actor_SetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
    Event_ShowMessageAndWait(ACTOR_FIRST_OF_PAIR, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    VinasuChojo_ShowMessage(ACTOR_GERALD);
    Actor_TurnToAngle(ACTOR_SECOND_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 20);
    Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
    VinasuChojo_ShowMessage(0x2013);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Actor_TurnToAngle(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_TurnToAngle(ACTOR_GERALD, FACING_WEST, 0);
    Actor_TurnToAngle(ACTOR_IVAN, FACING_NORTHWEST, 0);
    VinasuChojo_FaceActor(ACTOR_MIA, FACING_NORTHWEST);
    Actor_ShowEmote(21, 0x101, 80);
    VinasuChojo_ShowMessage(21);
    Actor_TurnToAngle(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Actor_TurnToAngle(ACTOR_GERALD, FACING_SOUTHEAST, 0);
    Actor_TurnToAngle(ACTOR_IVAN, FACING_SOUTHWEST, 0);
    Actor_TurnToAngle(ACTOR_MIA, FACING_NORTHEAST, 40);
    Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_TurnToAngle(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_TurnToAngle(ACTOR_GERALD, FACING_WEST, 0);
    Actor_TurnToAngle(ACTOR_IVAN, FACING_WEST, 0);
    Actor_TurnToAngle(ACTOR_MIA, FACING_WEST, 20);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP);
    VinasuChojo_ShowMessage(0x2014);
    Actor_SetAnimationAndWait(21, 3);
    Actor_TurnToAngle(21, FACING_NORTHWEST + FACING_STEP, 60);
    Actor_TurnToAngle(21, FACING_SOUTHEAST + FACING_STEP, 40);
    Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
    VinasuChojo_ShowMessage(0x2013);
    Actor_TurnToAngle(21, FACING_EAST, 40);
    VinasuChojo_ShowMessage(21);
    Actor_TurnToAngle(ACTOR_FIRST_OF_PAIR, FACING_NORTH + FACING_STEP, 40);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP);
    VinasuChojo_ShowMessage(0x2014);
    Actor_TurnToAngle(21, FACING_SOUTHEAST + FACING_STEP, 40);
    Actor_TurnToAngle(21, FACING_SOUTHEAST + FACING_STEP, 20);
    Actor_SetAnimationAndWait(21, 3);
    Actor_TurnToAngle(ACTOR_SECOND_OF_PAIR, FACING_EAST, 40);
    Actor_TurnToAngle(ACTOR_SECOND_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 20);
    Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x101, 40);
    VinasuChojo_ShowMessage(0x2013);
    VinasuChojo_FaceActor(21, FACING_SOUTHEAST + FACING_STEP);
    Actor_ShowEmote(21, 0x101, 20);
    VinasuChojo_ShowMessage(21);
    Actor_TurnToAngle(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 20);
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 4);
    VinasuChojo_ShowMessage(0x2013);
    Actor_ShowEmote(21, 0x103, 80);
    VinasuChojo_ShowMessage(21);
    Actor_SetAnimationAndWait(ACTOR_SECOND_OF_PAIR, 3);
    VinasuChojo_ShowMessage(0x2013);
    Actor_ShowEmote(21, 0x103, 20);
    Actor_StartRepeatedMotion(21, 2);
    VinasuChojo_ShowMessage(21);
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 4);
    VinasuChojo_ShowMessage(0x2013);
    Actor_ShowEmote(21, 0x102, 60);
    Actor_RunRepeatedMotion(21, 1);
    VinasuChojo_ShowMessage(21);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x108, 40);
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 4);
    Event_ShowMessageAndWait(0x2014, 0, 40);
    Actor_RunRepeatedMotion(21, 1);
    Event_Wait(20);
    VinasuChojo_ShowMessage(21);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 0);
    Actor_TurnToAngle(ACTOR_IVAN, FACING_NORTHWEST, 0);
    Actor_TurnToAngle(ACTOR_MIA, FACING_NORTHWEST, 20);
    Actor_TurnToAngle(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP, 20);
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 3);
    Actor_SetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
    Actor_TurnToAngle(21, FACING_NORTHWEST + FACING_STEP, 20);
    VinasuChojo_ShowMessage(0xa015);
    Actor_SetSpeed(6, 0x10000, 0x8000);
    Actor_SetSpeed(21, 0x10000, 0x8000);
    Actor_EnableActionCallback(21, Data_0200e22c);
    Event_Wait(20);
    Actor_EnableActionCallback(6, Data_0200e22c);
    VinasuChojo_FaceActor(ACTOR_GERALD, FACING_SOUTHWEST);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    VinasuChojo_ShowMessage(ACTOR_GERALD);
    Audio_PlayCue(17);
    ColorBuffer_ApplyTarget(0x40250d, 1);
    ColorBuffer_Interpolate(40);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTH + FACING_STEP);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_TurnToAngle(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_TurnToAngle(ACTOR_IVAN, FACING_WEST, 0);
    Actor_TurnToAngle(ACTOR_MIA, FACING_WEST, 0);
    VinasuChojo_FlashScreen();
    Actor_SetChildValue(ACTOR_FIRST_OF_PAIR, 7);
    Actor_SetChildValue(ACTOR_SECOND_OF_PAIR, 7);
    Event_Wait(20);
    Actor_SetChildValue(ACTOR_FIRST_OF_PAIR, 0x100);
    Actor_SetChildValue(ACTOR_SECOND_OF_PAIR, 0x100);
    Event_Wait(20);
    VinasuChojo_FlashScreen();
    Actor_Launch(ACTOR_MIA, 2, 20);
    VinasuChojo_ShowMessage(ACTOR_MIA);
    VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_EAST);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    VinasuChojo_FlashScreen();

    /* Draw seventeen-particle rings around each member of the pair. */
    origin = Actor_Get(ACTOR_SECOND_OF_PAIR);
    second_burst.palette = 7;
    second_burst.update = Effect_MoveWithDrag;
    second_burst.start_scale_x = 0x10000;
    second_burst.start_scale_y = 0x10000;
    for (i = 0; i <= 16; i++) {
        MakeBurstVelocity(velocity, i << 12);
        Effect_Spawn(origin->x.fixed, origin->y.fixed, origin->z.fixed, velocity[0], velocity[1],
                     velocity[2], EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                     &second_burst);
    }
    Audio_PlayCue(212);
    Task_Wait(6);
    VinasuChojo_FlashScreen();
    origin = Actor_Get(ACTOR_FIRST_OF_PAIR);
    first_burst.palette = 7;
    first_burst.update = Effect_MoveWithDrag;
    first_burst.start_scale_x = 0x10000;
    first_burst.start_scale_y = 0x10000;
    for (i = 0; i <= 16; i++) {
        MakeBurstVelocity(velocity, i << 12);
        Effect_Spawn(origin->x.fixed, origin->y.fixed, origin->z.fixed, velocity[0], velocity[1],
                     velocity[2], EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                     &first_burst);
    }
    Audio_PlayCue(212);
    Actor_Launch(ACTOR_IVAN, 6, 20);
    Audio_PlayCue(54);
    VinasuChojo_ShowMessage(ACTOR_IVAN);
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 4);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    VinasuChojo_FlashScreen();
    Actor_SetSpeed(ACTOR_FIRST_OF_PAIR, 0x3333, 0x1999);
    Actor_SetSpeed(ACTOR_SECOND_OF_PAIR, 0x3333, 0x1999);
    Actor_Get(ACTOR_FIRST_OF_PAIR)->unknown_5a &= ~1;
    Actor_Get(ACTOR_SECOND_OF_PAIR)->unknown_5a &= ~1;
    Actor_SetDestination(ACTOR_FIRST_OF_PAIR, 0x126, 196);
    Actor_SetDestination(ACTOR_SECOND_OF_PAIR, 0x126, 196);
    Task_AddCallback(VinasuChojo_UpdateBeamActors, TASK_PRIORITY_SCENE);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessage(ACTOR_GERALD, 0);
    GameFlag_Set(FLAG_PAIR_SLIDING);
    Event_ShowMessage(ACTOR_IVAN, 0);
    GameFlag_Set(FLAG_PAIR_GROWING);
    VinasuChojo_FlashScreen();
    Event_Wait(20);
    VinasuChojo_FlashScreen();
    Event_Wait(20);
    ((u8 *)&gGameState)[0x22b] = 3;
    Engine_GameStateSetReturn((s32)&LinkedScene_VinasuChojo, 3);
    Engine_GameStateSetWarp((s32)&LinkedScene_VinasuChojo, 9);
    Engine_Import0808a250(98, 0);
    Engine_Import08077268();
    GameFlag_Set(0x351);
}
