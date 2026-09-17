#ifndef GUARD_SPAWN_CONFIGURED_EFFECT_H
#define GUARD_SPAWN_CONFIGURED_EFFECT_H

#include "TYPES.H"

struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

#endif
