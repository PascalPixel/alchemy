#ifndef ALCHEMY_EFFECT_SLOT_H
#define ALCHEMY_EFFECT_SLOT_H

#include "types.h"
#include "layout_guard.h"

#define EFFECT_NO_TARGET ((s32)0x80000000)

struct EffectSlot;
typedef void (*EffectCallback)(struct EffectSlot *);

struct EffectSlot {
     void *object;
     s32 x;
     s32 z;
     s32 target_x;
     s32 target_z;
     s32 origin_x;
     s32 origin_z;
     s32 speed;
     s32 max_speed;
     s32 acceleration;
     s32 scale_x;
     s32 scale_y;
     u16 heading;
     s16 max_turn_step;
     EffectCallback callback;
     u16 age;
     s16 callback_delay;
     s16 orbit_radius;
     s16 orbit_angle;
     s8 state;
     s8 stop_at_target;
     s8 flag42;
     s8 update_motion;
     s8 render;
     s8 active;
     u8 random_value;
     u8 flags;
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
