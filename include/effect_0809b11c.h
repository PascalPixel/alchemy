#ifndef ALCHEMY_EFFECT_0809B11C_H
#define ALCHEMY_EFFECT_0809B11C_H

#include "types.h"

struct Effect_0809b11c {
    u8 padding00[4];
    s32 initial_x;
    s32 initial_z;
    s32 result_x;
    s32 result_z;
    s32 saved_x;
    s32 saved_z;
    s32 unknown1c;
    s32 speed_x;
    s32 speed_z;
    u8 padding28[0x0a];
    u16 unknown32;
    u8 padding34[0x0c];
    s8 state;
    s8 unknown41;
    s8 initial_state;
};

#endif
