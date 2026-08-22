#ifndef ALCHEMY_EFFECT_0809B11C_H
#define ALCHEMY_EFFECT_0809B11C_H

#include "types.h"

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
    /* 0x3c */ u8 unknown3c[4];
    /* 0x40 */ s8 state;
    /* 0x41 */ s8 flag41;
    /* 0x42 */ s8 flag42;
    /* 0x43 */ s8 update_motion;
    /* 0x44 */ s8 render;
    /* 0x45 */ s8 active;
    /* 0x46 */ u8 random_value;
    /* 0x47 */ u8 flags;
};

u32 Func_0809ba34(struct EffectSlot *);
void Func_0809ba5c(struct EffectSlot *, s32, s32);
void Func_0809ba70(struct EffectSlot *, s32);
void Func_0809ba7c(struct EffectSlot *, EffectCallback);
void Func_0809b804(struct EffectSlot *);
void Func_0809b86c(struct EffectSlot *);
void Func_0809b8f4(struct EffectSlot *);
void Func_0809bb34(struct EffectSlot *);

#endif
