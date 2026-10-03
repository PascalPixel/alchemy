#ifndef ALCHEMY_EFFECT_SLOT_H
#define ALCHEMY_EFFECT_SLOT_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

#define EFFECT_NO_TARGET ((s32)0x80000000)

struct EffectSlot;
typedef void (*EffectCallback)(struct EffectSlot *);

struct EffectSlot {
    /* 0x00 */ void *object;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 z;
    /* 0x0c */ s32 target_x;
    /* 0x10 */ s32 target_z;
    /* 0x14 */ s32 origin_x;
    /* 0x18 */ s32 origin_z;
    /* 0x1c */ s32 speed;
    /* 0x20 */ s32 max_speed;
    /* 0x24 */ s32 acceleration;
    /* 0x28 */ s32 scale_x;
    /* 0x2c */ s32 scale_y;
    /* 0x30 */ u16 heading;
    /* 0x32 */ s16 max_turn_step;
    /* 0x34 */ EffectCallback callback;
    /* 0x38 */ u16 age;
    /* 0x3a */ s16 callback_delay;
    /* The orbit callback uses pixels and 1/32-turn angle units. */
    /* 0x3c */ s16 orbit_radius;
    /* 0x3e */ s16 orbit_angle;
    /* 0x40 */ s8 state;
    /* Set, the update places the slot on its target and drops the target
       once within eight units of it; clear, the slot keeps steering. */
    /* 0x41 */ s8 stop_at_target;
    /* 0x42 */ s8 flag42;
    /* 0x43 */ s8 update_motion;
    /* 0x44 */ s8 render;
    /* 0x45 */ s8 active;
    /* 0x46 */ u8 random_value;
    /* 0x47 */ u8 flags;
};

LAYOUT_SIZE_GUARD(EffectSlot_Size, struct EffectSlot, 0x48);
LAYOUT_OFFSET_GUARD(EffectSlot_OrbitRadius, struct EffectSlot, orbit_radius, 0x3c);
LAYOUT_OFFSET_GUARD(EffectSlot_State, struct EffectSlot, state, 0x40);

u32 BattleFx_HasReachedTarget(struct EffectSlot *);
void EffectSlot_SetPosition(struct EffectSlot *, s32, s32);
void EffectSlot_SetObjectMode(struct EffectSlot *, s32);
void EffectSlot_SetCallback(struct EffectSlot *, EffectCallback);
void EffectSlot_Update(struct EffectSlot *);

#endif
