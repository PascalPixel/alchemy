#include "global_cells.h"
#ifndef RUNPA_JO_JO_H
#define RUNPA_JO_JO_H

#include "mapcopy.h"

#include "types.h"
#include "layout_guard.h"
#include "field_scene.h"
#include "system.h"

#include "game_state.h"

#ifndef MODERN // modern GCC keeps the headers' volatile declaration
extern u32 gFrameCount;
#endif

struct EventWork {
    u8 unknown_000[0x34];
    struct FieldActor *placed_actors[58];
    u8 unknown_11c[0x50];
    s16 touched_trigger;
    u8 unknown_16e[4];
    u16 unknown_172;
    u8 unknown_174[0xa];
    s16 psynergy_request;
    u8 unknown_180[2];
    s16 raised_trigger;
    u8 unknown_184[0x3c];
    s32 start_transition;
    u8 unknown_1c4[4];
    s32 transition_frames;
    u8 unknown_1cc[0x0c];
    u16 message;
    u8 unknown_1da[6];
    struct FieldActor *view_center;
};

extern struct EventWork *gEventWork;

enum SceneTransitionStyle {
    TRANSITION_FADE = 0,
    TRANSITION_BACKDROP_FADE = 1,
    TRANSITION_WINDOW = 2
};

#define SCENE_TRANSITION(style, variant) ((style) << 8 | (variant))

union FieldCoordinate {
    s32 fixed;
    struct {
        u16 fraction;
        s16 pixel;
    } part;
};

struct FieldSprite;
union FieldObject;

struct FieldActor {
    u8 unknown_00[6];
    u16 facing;
    union FieldCoordinate x;
    union FieldCoordinate y;
    union FieldCoordinate z;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u16 radius;
    u8 unknown_22;
    u8 priority_flags;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed;
    s32 acceleration;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unknown_44[0x0c];
    struct FieldSprite *sprite;
    u8 active;
    u8 motion_flags;
    u8 unknown_56[3];
    u8 collision_flags;
    u8 steering_flags;
    u8 paused;
    u8 unknown_5c;
    u8 unknown_5d[5];
    u8 rise_counter;
    u8 rise_enabled;
    u16 timer;
    u16 work;
    struct FieldActor *linked_object;
    void (*update)(union FieldObject *object);
};

enum ActorPriorityFlag {
    ACTOR_PRIORITY_AUTOMATIC = 0x01,
    ACTOR_PRIORITY_UNDERFOOT = 0x02
};

void Engine_EventWait(s32 frames);
void *Object_GetById(s32 actor);
void Engine_ActorSetSpeed(s32 actor, s32 speed, s32 acceleration);
void Engine_ActorSetDestinationOffset(s32 actor, s32 dx, s32 dz);
void Engine_ActorWaitForMove(s32 actor);
void Engine_MapCopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Engine_ActorSetAnimation(s32 actor, s32 animation);
static inline void Event_PlayCue(s32 cue)
{
    Engine_AudioPlayCue(cue);
}

s32 Engine_PartyInventoryFindOwner(s32 item_id);
s32 Engine_BattleFxPlayCueAndStartEmitterOnTarget(s32 value, s32 actor, s32 item);
void Engine_BattleFxSetWeightedResult(s32 slot, s32 value);
void Engine_BattleFxRunPageEffectForSlot(s32 slot, s32 first, s32 second);
void Engine_PartySetFields1ceAnd1d0(s32 first, s32 second);
void Object_RefreshSelectorById(s32 actor);
void Engine_MapSetWorkFlagBits9To11();
void Engine_ActorFaceActor(s32 actor, s32 target, s32 frames);
void Engine_ActorFaceDirection(s32 actor, s32 facing, s32 frames);
void Engine_ActorFaceEachOther(s32 actor, s32 other, s32 frames);
void Engine_ActorRunRepeatedMotion(s32 actor, s32 repeats);
void Engine_ActorSetPosition(s32 actor, s32 fixed_x, s32 fixed_z);
void Engine_ActorSetSpriteFlags(struct FieldActor *actor, s32 flags);
void Engine_ActorShowEmote(s32 actor, s32 emote, s32 frames);
void Engine_ActorWalkTo(s32 actor, s32 x, s32 z);
void Engine_CameraSetSpeed(s32 speed, s32 acceleration);
struct FieldActor *Engine_EventGetViewCenter();
void Engine_EventSetMessage(s32 message);
void Engine_EventShowMessage(s32 speaker, s32 flags);
s32 Engine_GameFlagIsSet(s32 flag);
void Engine_ObjectDispatchRelease(struct FieldActor *object);
s32 Engine_TaskAddCallback(void (*callback)(void), s32 priority);
s32 Engine_TaskRemoveCallback(void (*callback)(void));
void Engine_ActorEnableActionCallback(s32 actor, const u8 *table);
void Engine_ActorJump(s32 actor, s32 height, s32 frames);
void Engine_ActorSetAnimationAndWait(s32 actor, s32 animation);
void Engine_ActorSetAttachedEffect(s32 actor, s32 effect);
void Engine_ActorSetChildValue(s32 actor, s32 value);
void Engine_ActorSetDestination(s32 actor, s32 x, s32 z);
void Engine_ActorSetSpritePriority(s32 actor, s32 priority);
void Engine_ActorStartRepeatedMotion(s32 actor, s32 repeats);
void Engine_ActorStop(s32 actor);
void Engine_ActorWalkToAndWait(s32 actor, s32 x, s32 z);
void Engine_CameraFollowActor(s32 actor, s32 keep_position);
void Engine_CameraMoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan);
void Engine_CameraMoveToActor(s32 actor, s32 pan);
void Engine_CameraWaitForMove(void);
s32 Engine_EventAskYesNo(s32 speaker, s32 flags);
void Engine_EventBegin(void);
s32 Engine_EventChooseYesNo(s32 actor, s32 flags);
void Engine_EventCloseScreen(void);
void Engine_EventEnd(void);
s32 Engine_EventOpenMessage(s32 speaker, s32 flags);
void Engine_EventOpenScreen(void);
void Engine_EventRequestExit(s32 exit);
void Engine_GameFlagClear(s32 flag);
s32 Engine_GameFlagSet(s32 flag);
void Engine_ItemShowFound(s32 item, s32 height);
void Engine_MapCopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Engine_MapRedraw(void);
void Engine_MessageShowCentered(s32 message, s32 flags);
s32 Engine_PartyGiveItem(s32 item, s32 flags);
void Engine_PsynergyCancel(void);
s32 Engine_RandomNext(void);
void Engine_WorkSetValuesIfNonNegative(s32 first, s32 second, s32 third);

static inline void Actor_FaceDirection(s32 actor, s32 facing, s32 frames)
{
    Engine_ActorFaceDirection(actor, facing, frames);
}

static inline void Actor_SetAttachedEffect(s32 actor, s32 effect)
{
    Engine_ActorSetAttachedEffect(actor, effect);
}

static inline void Actor_SetDestination(s32 actor, s32 x, s32 z)
{
    Engine_ActorSetDestination(actor, x, z);
}

static inline void Actor_SetDestinationOffset(s32 actor, s32 dx, s32 dz)
{
    Engine_ActorSetDestinationOffset(actor, dx, dz);
}

static inline void Actor_SetPosition(s32 actor, s32 fixed_x, s32 fixed_z)
{
    Engine_ActorSetPosition(actor, fixed_x, fixed_z);
}

static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
    Engine_ActorSetSpeed(actor, speed, acceleration);
}

static inline void Actor_ShowEmote(s32 actor, s32 emote, s32 frames)
{
    Engine_ActorShowEmote(actor, emote, frames);
}

static inline void Actor_WalkTo(s32 actor, s32 x, s32 z)
{
    Engine_ActorWalkTo(actor, x, z);
}

static inline void Actor_WalkToAndWait(s32 actor, s32 x, s32 z)
{
    Engine_ActorWalkToAndWait(actor, x, z);
}

static inline void Camera_MoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan)
{
    Engine_CameraMoveTo(fixed_x, fixed_y, fixed_z, pan);
}

static inline void Camera_SetSpeed(s32 speed, s32 acceleration)
{
    Engine_CameraSetSpeed(speed, acceleration);
}

#ifdef MODERN
// field.h declares the engine's GameFlag_IsSet; modern GCC refuses this
// static one under the same name
#undef GameFlag_IsSet
#define GameFlag_IsSet RunpaJo_GameFlagIsSet
#endif
static inline s32 GameFlag_IsSet(s32 flag)
{
    return Engine_GameFlagIsSet(flag);
}

static inline s32 GameFlag_Set(s32 flag)
{
    return Engine_GameFlagSet(flag);
}

static inline void Map_CopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x,
                                 s32 dest_y)
{
    Engine_MapCopyCells(src_x, src_y, width, height, dest_x, dest_y);
}

static inline void Work_SetValuesIfNonNegative(s32 first, s32 second, s32 third)
{
    Engine_WorkSetValuesIfNonNegative(first, second, third);
}

static inline void *Actor_Get(s32 actor)
{
    return Object_GetById(actor);
}

static inline void Map_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y)
{
    Engine_MapCopyCellAttributes(src_x, src_y, width, height, dest_x, dest_y);
}

void Engine_MapSetWorkFlagBits9To11(s32 first);
static inline s32 Task_AddCallback(void (*callback)(void), s32 priority)
{
    return Engine_TaskAddCallback(callback, priority);
}

#endif
