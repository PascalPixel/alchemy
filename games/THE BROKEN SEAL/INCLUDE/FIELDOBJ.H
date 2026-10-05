#ifndef ALCHEMY_FIELDOBJ_H
#define ALCHEMY_FIELDOBJ_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* The engine's event work. */
struct ScenePlacement;
struct SceneEvent;

struct EventWork {
    /* The scene's registered placement streams, then its event stream. */
    struct ScenePlacement *tables[4];
    struct SceneEvent *events;
    /* Actors 0 through 7. */
    struct FieldActor *actors[8];
    /* Actors placed by the scene, with ids 8 through 65. */
    struct FieldActor *placed_actors[58];
    u8 unknown_11c[0x50];
    /* The map trigger the leader has run into, handled on the next frame. */
    s16 touched_trigger;
    u8 unknown_16e[2];
    /* The exit a script asks the scene to leave by. */
    u16 requested_exit;
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
    u8 unknown_184[0x18];
    /* Frames the leader has pressed against something it cannot pass. */
    s16 blocked_steps;
    u8 unknown_19e[0x22];
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
LAYOUT_OFFSET_GUARD(EventWork_RequestedExit, struct EventWork, requested_exit, 0x170);
LAYOUT_OFFSET_GUARD(EventWork_FacingTrigger, struct EventWork, facing_trigger, 0x17a);
LAYOUT_OFFSET_GUARD(EventWork_PsynergyRequest, struct EventWork, psynergy_request, 0x17e);
LAYOUT_OFFSET_GUARD(EventWork_RaisedTrigger, struct EventWork, raised_trigger, 0x182);
LAYOUT_OFFSET_GUARD(EventWork_BlockedSteps, struct EventWork, blocked_steps, 0x19c);
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
    /* The ground height under the actor, which a falling actor lands on. */
    s32 terrain_height;
    /* The sprite's scale on each axis; 0x10000 draws it at full size. */
    s32 scale_x;
    s32 scale_y;
    /* How near another actor must come to touch this one. */
    u16 radius;
    /* The map layer the actor stands on, for its heights and collision. */
    u8 terrain_id;
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
    /* What a landing keeps of the fall speed, and what pulls the actor down
       each frame while it falls toward the ground. */
    s32 bounce;
    s32 gravity;
    /* The height a rise starts from; rise_counter lifts the actor above it. */
    s32 rise_base_y;
    struct FieldSprite *sprite;
    /* Cleared while the actor is outside the area where actors are active. */
    u8 active;
    u8 motion_flags;
    u8 unknown_56[3];
    u8 collision_flags;
    /* Whether the actor turns to face the way it moves; a script clears
       ACTOR_FACES_MOTION to walk an actor without turning it. */
    u8 steering_flags;
    /* While set, the motion pass skips the actor's script and movement. */
    u8 paused;
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
LAYOUT_OFFSET_GUARD(FieldActor_TerrainHeight, struct FieldActor, terrain_height, 0x14);
LAYOUT_OFFSET_GUARD(FieldActor_ScaleY, struct FieldActor, scale_y, 0x1c);
LAYOUT_OFFSET_GUARD(FieldActor_PriorityFlags, struct FieldActor, priority_flags, 0x23);
LAYOUT_OFFSET_GUARD(FieldActor_MotionFlags, struct FieldActor, motion_flags, 0x55);
LAYOUT_OFFSET_GUARD(FieldActor_CollisionFlags, struct FieldActor, collision_flags, 0x59);
LAYOUT_OFFSET_GUARD(FieldActor_SteeringFlags, struct FieldActor, steering_flags, 0x5a);
LAYOUT_OFFSET_GUARD(FieldActor_Paused, struct FieldActor, paused, 0x5b);
LAYOUT_OFFSET_GUARD(FieldActor_RiseEnabled, struct FieldActor, rise_enabled, 0x63);
LAYOUT_OFFSET_GUARD(FieldActor_Bounce, struct FieldActor, bounce, 0x44);
LAYOUT_OFFSET_GUARD(FieldActor_Gravity, struct FieldActor, gravity, 0x48);
LAYOUT_OFFSET_GUARD(FieldActor_RiseBaseY, struct FieldActor, rise_base_y, 0x4c);
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

/* An actor's steering flags. */
enum ActorSteeringFlag {
    /* Each frame the motion pass turns the actor toward its velocity, by at
       most a sixteenth of a turn. */
    ACTOR_FACES_MOTION = 0x01
};

#endif
