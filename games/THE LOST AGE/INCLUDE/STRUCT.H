#ifndef STRUCT_H
#define STRUCT_H

#include "TYPES.H"

struct FlashChipType {
    u32 handlers[5];
    u32 size;
    u16 sector_size;
    u16 unknown_1a;
    u8 sector_shift;
};

struct ParticleChild {
    u8 padding[9];
    u8 flags;
};

struct ParticleEffectObject {
    u8 padding[80];
    struct ParticleChild *child;
};

struct ParticlePosition {
    s32 x;
    s32 y;
    s32 z;
};

struct TableEntry {
    u16 unused;
    u16 value;
};

#endif
