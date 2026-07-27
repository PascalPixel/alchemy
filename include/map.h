#ifndef ALCHEMY_MAP_H
#define ALCHEMY_MAP_H

#include "types.h"

struct MapCell {
    u16 metatile_and_flags;
    u8 collision_code;
    u8 attribute_b;
};

struct MapLayer {
    struct MapCell *cells;
    u8 unknown04[0x2c];
};

struct MapState {
    u8 unknown000[0x130];
    struct MapLayer layers[4];
};

struct WorldPosition {
    s32 x;
    u8 unknown_04[4];
    s32 y;
};

#endif
