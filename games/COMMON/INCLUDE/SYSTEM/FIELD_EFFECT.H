#ifndef ALCHEMY_FIELD_EFFECT_H
#define ALCHEMY_FIELD_EFFECT_H

#include "FIELD_EVENT.H"

/*
 * Short-lived effect sprites a scene spawns: objects that drift by a
 * velocity, grow by a scale rate and spin, driven by a motion script.
 */

/*
 * An effect object. It shares the actor's position, scale, sprite and update
 * fields and keeps its own rates where a walking actor keeps its speed.
 */
struct FieldEffect {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[3];
    u8 priority_flags;
    u8 unknown_24[0x0c];
    s32 scale_rate_x;
    s32 scale_rate_y;
    u8 unknown_38[0x0c];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct FieldSprite *sprite;
    u8 unknown_54;
    u8 motion_flags;
    u8 unknown_56[3];
    u8 collision_flags;
    u8 unknown_5a[0x0a];
    u16 spin;
    u8 unknown_66[2];
    s32 countdown;
    void (*update)(union FieldObject *object);
};

LAYOUT_OFFSET_GUARD(FieldEffect_ScaleRateX, struct FieldEffect, scale_rate_x, 0x30);
LAYOUT_OFFSET_GUARD(FieldEffect_VelocityX, struct FieldEffect, velocity_x, 0x44);
LAYOUT_OFFSET_GUARD(FieldEffect_Sprite, struct FieldEffect, sprite, 0x50);
LAYOUT_OFFSET_GUARD(FieldEffect_Spin, struct FieldEffect, spin, 0x64);
LAYOUT_OFFSET_GUARD(FieldEffect_Countdown, struct FieldEffect, countdown, 0x68);
LAYOUT_OFFSET_GUARD(FieldEffect_Update, struct FieldEffect, update, 0x6c);

/* One object record, seen as the actor or the effect it holds. */
union FieldObject {
    struct FieldActor actor;
    struct FieldEffect effect;
};

/*
 * An object's motion script is a list of words: each command word is
 * followed by its operands. A repeat jumps back to its label, the index
 * after the matching word, or to the start for label 0, until it has run
 * the given number of times.
 */
#define SCRIPT_WAIT(frames) 0, (frames)
#define SCRIPT_SET_POSITION(x, y, z) 2, (x), (y), (z)
#define SCRIPT_REPEAT(count, label) 12, (count), (label)
#define SCRIPT_DELETE 27

/* What a spawned effect takes from its options, beside its motion script. */
enum EffectSpawnFlags {
    /* The low four bits choose the motion script and its animation. */
    EFFECT_SCRIPT_MASK = 0x0f,
    EFFECT_USE_PALETTE = 0x10000,
    EFFECT_USE_PRIORITY = 0x20000,
    EFFECT_SCALE_TO_TARGET = 0x40000,
    EFFECT_USE_START_SCALE = 0x80000,
    EFFECT_USE_TYPE = 0x100000,
    EFFECT_USE_SCRIPT = 0x200000,
    EFFECT_USE_ROTATION = 0x400000,
    EFFECT_USE_SPIN = 0x800000,
    EFFECT_USE_UPDATE = 0x1000000
};

struct EffectOptions {
    s32 priority;
    s32 palette;
    s32 start_scale_x;
    s32 start_scale_y;
    s32 target_scale_x;
    s32 target_scale_y;
    s16 type;
    const s32 *script;
    u16 rotation;
    u16 spin;
    void (*update)(union FieldObject *object);
};

LAYOUT_OFFSET_GUARD(EffectOptions_Type, struct EffectOptions, type, 0x18);
LAYOUT_OFFSET_GUARD(EffectOptions_Update, struct EffectOptions, update, 0x24);

void Effect_SetPriority(struct FieldEffect *effect, s32 priority);
struct FieldEffect *Effect_CreateTranslucent(s32 x, s32 y, s32 z, s32 type);
struct FieldEffect *Effect_Create(s32 x, s32 y, s32 z, s32 type);
void Effect_Move(union FieldObject *object);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z, u32 flags,
                  const struct EffectOptions *options);

#endif
