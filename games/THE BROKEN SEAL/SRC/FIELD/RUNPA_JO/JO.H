#ifndef RUNPA_JO_JO_H
#define RUNPA_JO_JO_H

/*
 * What the Lunpa fortress scene sees of the engine. The fortress links beside
 * the staged-actor code (FIELD/COMMON/OBJECT/STAGED_ACTOR.C), and several of
 * the overlay's import veneers carry that code's names, among them the sound
 * cue, which FIELD_EVENT.H spells as an inline service; so the fortress takes
 * the scene state and services it uses from this view instead, each service
 * reaching its veneer by the one name the overlay gives it.
 */

#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "FIELD_SCENE.H"
#include "SYSTEM.H"

#include "GAME_STATE.H"

/* Counts the frames the game has drawn. */
extern u32 gFrameCount;

/* The engine's event work. */
struct EventWork {
    u8 unknown_000[0x34];
    /* Actors placed by the scene, with ids 8 through 65. */
    struct FieldActor *placed_actors[58];
    u8 unknown_11c[0x50];
    /* The map trigger the leader has run into, handled on the next frame. */
    s16 touched_trigger;
    u8 unknown_16e[4];
    u16 unknown_172;
    u8 unknown_174[0xa];
    /* Psynergy the player has asked to use, handled on the next frame. */
    s16 psynergy_request;
    u8 unknown_180[2];
    /* A trigger a script raises for the events that answer it. */
    s16 raised_trigger;
    u8 unknown_184[0x3c];
    /* How the screen opens when the scene starts. */
    s32 start_transition;
    u8 unknown_1c4[4];
    /* The frames the screen takes to open or close. */
    s32 transition_frames;
    u8 unknown_1cc[0x0c];
    /* The message the next dialogue line shows. */
    u16 message;
    u8 unknown_1da[6];
    /* The actor at the centre of the area where actors are active. */
    struct FieldActor *view_center;
};

extern struct EventWork *gEventWork;

/* A scene transition is a style in the high byte and its variant in the low byte. */
enum SceneTransitionStyle {
    TRANSITION_FADE = 0,
    /* Fades in from the colour behind the background layers. */
    TRANSITION_BACKDROP_FADE = 1,
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

struct FieldSprite;
union FieldObject;

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
    /* How near another actor must come to touch this one. */
    u16 radius;
    u8 unknown_22;
    u8 priority_flags;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed;
    s32 acceleration;
    /* The fixed-point point the actor moves toward, or ACTOR_NO_TARGET. */
    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unknown_44[0x0c];
    struct FieldSprite *sprite;
    /* Cleared while the actor is outside the area where actors are active. */
    u8 active;
    u8 motion_flags;
    u8 unknown_56[3];
    u8 collision_flags;
    u8 unknown_5a;
    u8 unknown_5b;
    u8 unknown_5c;
    u8 unknown_5d[5];
    /* Counts the frames of a rise while rise_enabled is set. */
    u8 rise_counter;
    u8 rise_enabled;
    u16 unknown_64;
    u16 unknown_66;
    u8 unknown_68[4];
    /* Called each frame while set. */
    void (*update)(union FieldObject *object);
};

/* An actor's priority flags. */
enum ActorPriorityFlag {
    /* Cleared when a script sets the sprite priority. */
    ACTOR_PRIORITY_AUTOMATIC = 0x01,
    /*
     * Set on objects the party passes over or stands on: effects, a glint
     * on the ground, and a pillar while the leader stands on top of it.
     */
    ACTOR_PRIORITY_UNDERFOOT = 0x02
};

/* The map work cell; the event work pointer follows it at +0x4c. */
extern u8 gCam[];

/* Import veneers named by the staged-actor code. */
void Battle_WaitMode0(s32 frames);
void *Object_GetById(s32 actor);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Object_SetModeById(s32 actor, s32 animation);
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Audio_PlayCue(s32 cue);

/* The other import veneers the fortress calls. */
s32 PartyInventory_FindOwner(s32 item_id);
s32 BattleFx_PlayCueAndStartEmitterOnTarget(s32 value, s32 actor, s32 item);
void BattleFx_SetWeightedResult(s32 slot, s32 value);
void BattleFx_RunPageEffectForSlot(s32 slot, s32 first, s32 second);
void Party_SetFields1ceAnd1d0(s32 first, s32 second);
void Object_RefreshSelectorById(s32 actor);
void Map_SetWorkFlagBits9To11();
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
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    Engine_ActorFaceDirection(actor, facing, frames);
}

static inline void Actor_SetAttachedEffect(s32 actor, s32 effect)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    Engine_ActorSetAttachedEffect(actor, effect);
}

static inline void Actor_SetDestination(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    Engine_ActorSetDestination(actor, x, z);
}

static inline void Actor_SetDestinationOffset(s32 actor, s32 dx, s32 dz)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    ObjectMotion_OffsetPositionAndResetMotion(actor, dx, dz);
}

static inline void Actor_SetPosition(s32 actor, s32 fixed_x, s32 fixed_z)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    Engine_ActorSetPosition(actor, fixed_x, fixed_z);
}

static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static inline void Actor_ShowEmote(s32 actor, s32 emote, s32 frames)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    Engine_ActorShowEmote(actor, emote, frames);
}

static inline void Actor_WalkTo(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    Engine_ActorWalkTo(actor, x, z);
}

static inline void Actor_WalkToAndWait(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    Engine_ActorWalkToAndWait(actor, x, z);
}

static inline void Camera_MoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    Engine_CameraMoveTo(fixed_x, fixed_y, fixed_z, pan);
}

static inline void Camera_SetSpeed(s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    Engine_CameraSetSpeed(speed, acceleration);
}

static inline s32 GameFlag_IsSet(s32 flag)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    return Engine_GameFlagIsSet(flag);
}

static inline s32 GameFlag_Set(s32 flag)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    return Engine_GameFlagSet(flag);
}

static inline void Map_CopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x,
                                 s32 dest_y)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    Engine_MapCopyCells(src_x, src_y, width, height, dest_x, dest_y);
}

static inline void Work_SetValuesIfNonNegative(s32 first, s32 second, s32 third)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers. */
    Engine_WorkSetValuesIfNonNegative(first, second, third);
}

#endif
