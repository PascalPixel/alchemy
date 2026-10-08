#ifndef ALCHEMY_FIELDOBJ_H
#define ALCHEMY_FIELDOBJ_H

#include "types.h"
#include "layout_guard.h"

#include "eventwrk.h"

extern struct EventWork *gEventWork;

enum SceneTransitionStyle {
    TRANSITION_FADE = 0,
    TRANSITION_BACKDROP_FADE = 1,
    TRANSITION_WINDOW = 2
};

#define SCENE_TRANSITION(style, variant) ((style) << 8 | (variant))

#include "fieldcoo.h"

#include "field_sprite.h"

union FieldObject;

struct FieldActor {
    u8 unknown_00[6];
    u16 facing;
    union FieldCoordinate x;
    union FieldCoordinate y;
    union FieldCoordinate z;
    s32 terrain_height;
    s32 scale_x;
    s32 scale_y;
    u16 radius;
    u8 terrain_id;
    u8 priority_flags;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed;
    s32 acceleration;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    s32 bounce;
    s32 gravity;
    s32 rise_base_y;
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
    u8 unknown_68[4];
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

#define ACTOR_NO_TARGET ((s32)0x80000000)

enum ActorPriorityFlag {
    ACTOR_PRIORITY_AUTOMATIC = 0x01,
    ACTOR_PRIORITY_UNDERFOOT = 0x02
};

enum ActorMotionFlag {
    ACTOR_FOLLOWS_TERRAIN = 0x01,
    ACTOR_FALLS = 0x02
};

enum ActorSteeringFlag {
    ACTOR_FACES_MOTION = 0x01
};

#endif
