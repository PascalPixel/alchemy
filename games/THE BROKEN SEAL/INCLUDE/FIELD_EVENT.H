#ifndef ALCHEMY_FIELD_EVENT_H
#define ALCHEMY_FIELD_EVENT_H

#include "LAYOUT_GUARD.H"

/*
 * What a field scene's event scripts ask of the engine: holding the party
 * still while an event plays, dialogue, actor motion, the camera, the map,
 * flags, sound, exits and the shop, inn and sanctum services.
 */

/* The part of the saved game that records where the party is. */
struct GameState {
    u8 unknown_000[0x1c0];
    s16 scene;
    s16 entrance;
    s16 saved_scene;
    s16 saved_entrance;
};

LAYOUT_OFFSET_GUARD(GameState_Entrance, struct GameState, entrance, 0x1c2);

extern struct GameState gGameState;

/* The engine's event work. */
struct EventWork {
    u8 unknown_000[0x1c0];
    /* How the screen opens when the scene starts. */
    s32 start_transition;
};

LAYOUT_OFFSET_GUARD(EventWork_StartTransition, struct EventWork, start_transition, 0x1c0);

extern struct EventWork *gEventWork;

/* A scene transition is a style in the high byte and its variant in the low byte. */
enum SceneTransitionStyle {
    TRANSITION_FADE = 0,
    TRANSITION_WINDOW = 2
};

#define SCENE_TRANSITION(style, variant) ((style) << 8 | (variant))

/* A 16.16 fixed-point map coordinate; its high half is the whole pixel. */
union FieldCoordinate {
    s32 fixed;
    struct {
        u16 fraction;
        s16 pixel;
    } part;
};

/* The runtime actor as event scripts see it. */
struct FieldActor {
    u8 unknown_00[6];
    u16 facing;
    union FieldCoordinate x;
    union FieldCoordinate y;
    union FieldCoordinate z;
    u8 unknown_14[4];
    /* The sprite's scale on each axis; 0x10000 draws it at full size. */
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[3];
    /* Bit 0 clears when a script sets the sprite priority. */
    u8 priority_flags;
    u8 unknown_24[0x31];
    /* Bit 0 keeps the actor standing on the terrain height. */
    u8 motion_flags;
    u8 unknown_56[3];
    u8 collision_flags;
};

LAYOUT_OFFSET_GUARD(FieldActor_Facing, struct FieldActor, facing, 0x06);
LAYOUT_OFFSET_GUARD(FieldActor_X, struct FieldActor, x, 0x08);
LAYOUT_OFFSET_GUARD(FieldActor_Z, struct FieldActor, z, 0x10);
LAYOUT_OFFSET_GUARD(FieldActor_ScaleY, struct FieldActor, scale_y, 0x1c);
LAYOUT_OFFSET_GUARD(FieldActor_PriorityFlags, struct FieldActor, priority_flags, 0x23);
LAYOUT_OFFSET_GUARD(FieldActor_MotionFlags, struct FieldActor, motion_flags, 0x55);
LAYOUT_OFFSET_GUARD(FieldActor_CollisionFlags, struct FieldActor, collision_flags, 0x59);

/*
 * Animations a person's sprite plays. The party leader nods when the player
 * answers yes to a question and shakes its head at no.
 */
enum ActorAnimation {
    ANIM_STAND = 1,
    ANIM_WALK = 2,
    ANIM_NOD = 3,
    ANIM_SHAKE_HEAD = 4
};

/* Emote and attached-effect sprites draw over the scenery with this bit. */
#define EMOTE_IN_FRONT 0x100

/* A speaker id with this bit opens the message window above the speaker. */
#define SPEAKER_WINDOW_ABOVE 0x4000

/*
 * The engine entry points a scene overlay imports. Scripts do not call them
 * directly; they use the inline event services that follow.
 */
void Engine_EventBegin(void);
void Engine_EventEnd(void);
void Engine_EventWait(s32 frames);
void Engine_TaskWait(s32 frames);
void Engine_EventSetMessage(s32 message);
void Engine_EventShowMessage(s32 speaker, s32 flags);
s32 Engine_EventOpenMessage(s32 speaker, s32 flags);
s32 Engine_EventAskYesNo(s32 speaker, s32 flags);
s32 Engine_EventChooseYesNo(s32 actor, s32 flags);
void Engine_MessageShowCentered(s32 message, s32 flags);
void Engine_EventRequestExit(s32 exit);
void Engine_EventOpenScreen(void);
void Engine_EventCloseScreen(void);
void Engine_EventWaitForScreen(void);
struct FieldActor *Engine_ActorGet(s32 actor);
void Engine_ActorSetPosition(s32 actor, s32 fixed_x, s32 fixed_z);
void Engine_ActorSetSpeed(s32 actor, s32 speed, s32 acceleration);
void Engine_ActorSetDestination(s32 actor, s32 x, s32 z);
void Engine_ActorSetDestinationOffset(s32 actor, s32 dx, s32 dz);
void Engine_ActorWalkTo(s32 actor, s32 x, s32 z);
void Engine_ActorWalkBy(s32 actor, s32 dx, s32 dz);
void Engine_ActorWaitForMove(s32 actor);
void Engine_ActorFaceDirection(s32 actor, s32 facing, s32 frames);
void Engine_ActorFaceActor(s32 actor, s32 target, s32 frames);
void Engine_ActorFaceEachOther(s32 actor, s32 other, s32 frames);
void Engine_ActorSetAnimation(s32 actor, s32 animation);
void Engine_ActorSetAnimationAndWait(s32 actor, s32 animation);
void Engine_ActorStartRepeatedMotion(s32 actor, s32 repeats);
void Engine_ActorRunRepeatedMotion(s32 actor, s32 repeats);
void Engine_ActorShowEmote(s32 actor, s32 emote, s32 frames);
void Engine_ActorSetAttachedEffect(s32 actor, s32 effect);
void Engine_ActorSetSpritePriority(s32 actor, s32 priority);
void Engine_ActorSetSpriteFlags(struct FieldActor *actor, s32 flags);
void Engine_CameraFollowActor(s32 actor, s32 keep_position);
void Engine_CameraSetSpeed(s32 speed, s32 acceleration);
void Engine_CameraMoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan);
void Engine_CameraMoveToActor(s32 actor, s32 pan);
void Engine_CameraWaitForMove(void);
void Engine_MapCopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Engine_MapCopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x,
                                  s32 dest_y);
void Engine_MapRedraw(void);
s32 Engine_GameFlagIsSet(s32 flag);
s32 Engine_GameFlagSet(s32 flag);
void Engine_GameFlagClear(s32 flag);
void Engine_AudioPlayCue(s32 cue);
void Engine_ShopOpen(s32 shop, s32 keeper);
void Engine_InnOpen(s32 inn, s32 keeper);
void Engine_SanctumOpen(s32 priest);

/* Holding the party still while an event plays; waits count frames. */
static inline void Event_Begin(void)
{
    Engine_EventBegin();
}

static inline void Event_End(void)
{
    Engine_EventEnd();
}

static inline void Event_Wait(s32 frames)
{
    Engine_EventWait(frames);
}

static inline void Task_Wait(s32 frames)
{
    Engine_TaskWait(frames);
}

/*
 * Dialogue: choose the message, then let an actor say it. Each message shown
 * advances to the next one. An opened message stays on screen for the
 * question that follows it, and choosing yes returns 0.
 */
static inline void Event_SetMessage(s32 message)
{
    Engine_EventSetMessage(message);
}

static inline void Event_ShowMessage(s32 speaker, s32 flags)
{
    Engine_EventShowMessage(speaker, flags);
}

static inline s32 Event_OpenMessage(s32 speaker, s32 flags)
{
    return Engine_EventOpenMessage(speaker, flags);
}

static inline s32 Event_AskYesNo(s32 speaker, s32 flags)
{
    return Engine_EventAskYesNo(speaker, flags);
}

static inline s32 Event_ChooseYesNo(s32 actor, s32 flags)
{
    return Engine_EventChooseYesNo(actor, flags);
}

static inline void Message_ShowCentered(s32 message, s32 flags)
{
    Engine_MessageShowCentered(message, flags);
}

/* Leaving the scene, and opening or closing the screen around a change. */
static inline void Event_RequestExit(s32 exit)
{
    Engine_EventRequestExit(exit);
}

static inline void Event_OpenScreen(void)
{
    Engine_EventOpenScreen();
}

static inline void Event_CloseScreen(void)
{
    Engine_EventCloseScreen();
}

static inline void Event_WaitForScreen(void)
{
    Engine_EventWaitForScreen();
}

/*
 * Actors. Positions set directly are fixed-point; destinations and offsets
 * are whole pixels. Walking plays the walk animation on the way.
 */
static inline struct FieldActor *Actor_Get(s32 actor)
{
    return Engine_ActorGet(actor);
}

static inline void Actor_SetPosition(s32 actor, s32 fixed_x, s32 fixed_z)
{
    Engine_ActorSetPosition(actor, fixed_x, fixed_z);
}

static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
    Engine_ActorSetSpeed(actor, speed, acceleration);
}

static inline void Actor_SetDestination(s32 actor, s32 x, s32 z)
{
    Engine_ActorSetDestination(actor, x, z);
}

static inline void Actor_SetDestinationOffset(s32 actor, s32 dx, s32 dz)
{
    Engine_ActorSetDestinationOffset(actor, dx, dz);
}

static inline void Actor_WalkTo(s32 actor, s32 x, s32 z)
{
    Engine_ActorWalkTo(actor, x, z);
}

static inline void Actor_WalkBy(s32 actor, s32 dx, s32 dz)
{
    Engine_ActorWalkBy(actor, dx, dz);
}

/* Waits until the actor reaches its destination, then stands it still. */
static inline void Actor_WaitForMove(s32 actor)
{
    Engine_ActorWaitForMove(actor);
}

static inline void Actor_FaceDirection(s32 actor, s32 facing, s32 frames)
{
    Engine_ActorFaceDirection(actor, facing, frames);
}

static inline void Actor_FaceActor(s32 actor, s32 target, s32 frames)
{
    Engine_ActorFaceActor(actor, target, frames);
}

static inline void Actor_FaceEachOther(s32 actor, s32 other, s32 frames)
{
    Engine_ActorFaceEachOther(actor, other, frames);
}

static inline void Actor_SetAnimation(s32 actor, s32 animation)
{
    Engine_ActorSetAnimation(actor, animation);
}

static inline void Actor_SetAnimationAndWait(s32 actor, s32 animation)
{
    Engine_ActorSetAnimationAndWait(actor, animation);
}

static inline void Actor_StartRepeatedMotion(s32 actor, s32 repeats)
{
    Engine_ActorStartRepeatedMotion(actor, repeats);
}

static inline void Actor_RunRepeatedMotion(s32 actor, s32 repeats)
{
    Engine_ActorRunRepeatedMotion(actor, repeats);
}

/* Shows an emote over the actor and waits the given frames. */
static inline void Actor_ShowEmote(s32 actor, s32 emote, s32 frames)
{
    Engine_ActorShowEmote(actor, emote, frames);
}

/* Attaches a lasting effect sprite to the actor; effect 0 removes it. */
static inline void Actor_SetAttachedEffect(s32 actor, s32 effect)
{
    Engine_ActorSetAttachedEffect(actor, effect);
}

static inline void Actor_SetSpritePriority(s32 actor, s32 priority)
{
    Engine_ActorSetSpritePriority(actor, priority);
}

static inline void Actor_SetSpriteFlags(struct FieldActor *actor, s32 flags)
{
    Engine_ActorSetSpriteFlags(actor, flags);
}

/* The camera. A pan of 0 moves it at once; -1 keeps a coordinate. */
static inline void Camera_FollowActor(s32 actor, s32 keep_position)
{
    Engine_CameraFollowActor(actor, keep_position);
}

static inline void Camera_SetSpeed(s32 speed, s32 acceleration)
{
    Engine_CameraSetSpeed(speed, acceleration);
}

static inline void Camera_MoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan)
{
    Engine_CameraMoveTo(fixed_x, fixed_y, fixed_z, pan);
}

static inline void Camera_MoveToActor(s32 actor, s32 pan)
{
    Engine_CameraMoveToActor(actor, pan);
}

static inline void Camera_WaitForMove(void)
{
    Engine_CameraWaitForMove();
}

/*
 * The map, in cells. Copying cells redraws them; copying attributes keeps
 * each destination cell's tile and takes the rest from the source.
 */
static inline void Map_CopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x,
                                 s32 dest_y)
{
    Engine_MapCopyCells(src_x, src_y, width, height, dest_x, dest_y);
}

static inline void Map_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height,
                                          s32 dest_x, s32 dest_y)
{
    Engine_MapCopyCellAttributes(src_x, src_y, width, height, dest_x, dest_y);
}

static inline void Map_Redraw(void)
{
    Engine_MapRedraw();
}

static inline s32 GameFlag_IsSet(s32 flag)
{
    return Engine_GameFlagIsSet(flag);
}

static inline s32 GameFlag_Set(s32 flag)
{
    return Engine_GameFlagSet(flag);
}

static inline void GameFlag_Clear(s32 flag)
{
    Engine_GameFlagClear(flag);
}

static inline void Audio_PlayCue(s32 cue)
{
    Engine_AudioPlayCue(cue);
}

static inline void Shop_Open(s32 shop, s32 keeper)
{
    Engine_ShopOpen(shop, keeper);
}

static inline void Inn_Open(s32 inn, s32 keeper)
{
    Engine_InnOpen(inn, keeper);
}

static inline void Sanctum_Open(s32 priest)
{
    Engine_SanctumOpen(priest);
}

#endif
