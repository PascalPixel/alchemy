#ifndef ALCHEMY_FIELDOBJ_H
#define ALCHEMY_FIELDOBJ_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

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
    u8 unknown_174[6];
    /* The trigger tile just ahead of the leader. */
    s16 facing_trigger;
    u8 unknown_17c[2];
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

LAYOUT_OFFSET_GUARD(EventWork_PlacedActors, struct EventWork, placed_actors, 0x34);
LAYOUT_OFFSET_GUARD(EventWork_TouchedTrigger, struct EventWork, touched_trigger, 0x16c);
LAYOUT_OFFSET_GUARD(EventWork_FacingTrigger, struct EventWork, facing_trigger, 0x17a);
LAYOUT_OFFSET_GUARD(EventWork_PsynergyRequest, struct EventWork, psynergy_request, 0x17e);
LAYOUT_OFFSET_GUARD(EventWork_RaisedTrigger, struct EventWork, raised_trigger, 0x182);
LAYOUT_OFFSET_GUARD(EventWork_StartTransition, struct EventWork, start_transition, 0x1c0);
LAYOUT_OFFSET_GUARD(EventWork_TransitionFrames, struct EventWork, transition_frames, 0x1c8);
LAYOUT_OFFSET_GUARD(EventWork_Message, struct EventWork, message, 0x1d8);
LAYOUT_OFFSET_GUARD(EventWork_ViewCenter, struct EventWork, view_center, 0x1e0);

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

#include "FIELD_SPRITE.H"

/*
 * Actors and effects share the engine's object records; FIELD_EFFECT.H
 * joins the two views.
 */
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

LAYOUT_OFFSET_GUARD(FieldActor_Facing, struct FieldActor, facing, 0x06);
LAYOUT_OFFSET_GUARD(FieldActor_X, struct FieldActor, x, 0x08);
LAYOUT_OFFSET_GUARD(FieldActor_Z, struct FieldActor, z, 0x10);
LAYOUT_OFFSET_GUARD(FieldActor_ScaleY, struct FieldActor, scale_y, 0x1c);
LAYOUT_OFFSET_GUARD(FieldActor_PriorityFlags, struct FieldActor, priority_flags, 0x23);
LAYOUT_OFFSET_GUARD(FieldActor_MotionFlags, struct FieldActor, motion_flags, 0x55);
LAYOUT_OFFSET_GUARD(FieldActor_CollisionFlags, struct FieldActor, collision_flags, 0x59);
LAYOUT_OFFSET_GUARD(FieldActor_RiseEnabled, struct FieldActor, rise_enabled, 0x63);
LAYOUT_OFFSET_GUARD(FieldActor_VelocityY, struct FieldActor, velocity_y, 0x28);
LAYOUT_OFFSET_GUARD(FieldActor_Speed, struct FieldActor, speed, 0x30);
LAYOUT_OFFSET_GUARD(FieldActor_TargetX, struct FieldActor, target_x, 0x38);
LAYOUT_OFFSET_GUARD(FieldActor_Sprite, struct FieldActor, sprite, 0x50);
LAYOUT_OFFSET_GUARD(FieldActor_Update, struct FieldActor, update, 0x6c);

/* A target coordinate that names no point. */
#define ACTOR_NO_TARGET ((s32)0x80000000)

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

/* An actor's motion flags. */
enum ActorMotionFlag {
    /* Keeps the actor standing on the terrain height. */
    ACTOR_FOLLOWS_TERRAIN = 0x01,
    /* Pulls a raised actor back to the ground. */
    ACTOR_FALLS = 0x02
};

#endif
