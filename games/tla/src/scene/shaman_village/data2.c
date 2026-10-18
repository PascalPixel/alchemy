#include "types.h"
#include "field_scene.h"
#include "edition.h"

extern u8 gData_02004aa4[];
extern u8 gData_02004ae0[];
extern u8 gData_02004b1c[];

// clang-format off
s32 gData_02004c08[] = {
    22, 26, 0, 23, 7, -0x8000,
    23, 9, -1024, 23, 10, -1024,
    0, 1, 13, 24, 0, 17,
};

s32 gData_02004c50[] = {
    (s32)gData_02004aa4,
    (s32)gData_02004ae0,
    (s32)gData_02004b1c,
};

u32 gData_02004c5c[] = {
    39, 2, 4, 0xb80000, 0, 0xd80000,
    1, 39, 1, 0, 10, 22,
    3, 0xc000, 22, 40, 1, 0,
    10, 17,
};

u32 gData_02004cac[] = {
    39, 2, 4, 0x1080000, 0, 0xd80000,
    1, 39, 1, 0, 10, 22,
    3, 0xc000, 22, 40, 1, 0,
    10, 17,
};

u32 gData_02004cfc[] = {
    39, 2, 4, 0x980000, 0, 0x1080000,
    1, 39, 1, 0, 10, 22,
    3, 0xb000, 0, 10, 17,
};

u32 gData_02004d40[] = {
    39, 2, 4, 0x880000, 0, 0x1080000,
    1, 39, 1, 0, 10, 22,
    3, 0xc000, 0, 10, 17,
};

u32 gData_02004d84[] = {
    39, 2, 4, 0x680000, 0, 0x1080000,
    1, 39, 1, 0, 10, 22,
    3, 0xc000, 0, 10, 17,
};

u32 gData_02004dc8[] = {
    39, 2, 4, 0x580000, 0, 0x1080000,
    1, 39, 1, 0, 10, 22,
    3, 0xc000, 0, 10, 17,
};

u32 gData_02004e0c[] = {
    39, 2, 4, 0x780000, 0, 0xfc0000,
    1, 4, 0xfc0000, 0, 0xfc0000, 1,
    4, 0xfc0000, 0, 0x780000, 1, 39,
    1, 17,
};

u32 gData_02004e5c[] = {
    39, 2, 4, 0x880000, 0, 0xfc0000,
    1, 4, 0xfc0000, 0, 0xfc0000, 1,
    4, 0xfc0000, 0, 0x780000, 1, 39,
    1, 17,
};

u32 gData_02004eac[] = {
    39, 2, 4, 0x680000, 0, 0xfc0000,
    1, 4, 0xfc0000, 0, 0xfc0000, 1,
    4, 0xfc0000, 0, 0x780000, 1, 39,
    1, 22, 40, 1, 17,
};

u32 gData_02004f08[] = {
    39, 2, 4, 0x780000, 0, 0x1080000,
    1, 39, 1, 0, 1, 3,
    17,
};

u32 gData_02004f3c[] = {
    39, 2, 4, 0x880000, 0, 0x2280000,
    1, 39, 1, 0, 10, 22,
    3, 0xd000, 0, 10, 17,
};

u32 gData_02004f80[] = {
    39, 2, 4, 0xb80000, 0, 0x2180000,
    1, 39, 1, 0, 10, 22,
    3, 0xa000, 0, 10, 17,
};

u32 gData_02004fc4[] = {
    39, 2, 4, 0x980000, 0, 0x2180000,
    1, 39, 1, 0, 10, 22,
    3, 0xe000, 0, 10, 17,
};

u32 gData_02005008[] = {
    39, 2, 4, 0xc80000, 0, 0x2280000,
    1, 39, 1, 0, 10, 22,
    3, 0xa000, 0, 10, 17,
};

u32 gData_0200504c[] = {
    39, 2, 4, 0xd80000, 0, 0x2340000,
    1, 4, 0xd80000, 0, 0x1d60000, 1,
    4, 0xa80000, 0, 0x1b20000, 1, 4,
    0xa80000, 0, 0x17c0000, 1, 39, 1,
    22, 40, 1, 17,
};

u32 gData_020050bc[] = {
    39, 2, 4, 0xb80000, 0, 0x2180000,
    1, 39, 1, 0, 1, 3,
    0, 1, 17,
};

s32 gData_020050f8[] = {
    23, 9, 0x2000, 23, 10, 0x4000,
    0, 1, 13, 4, 0, (s32)0x80010000,
    23, 9, -4096, 23, 10, -0x2000,
    0, 1, 13, 8, (s32)0xc0010000, 17,
};

s16 gData_02005158[] = {
    -1,
};

// Scale for each edition.
#if (LANGUAGE == LANGUAGE_FRENCH)
#define SHAMAN_GRAPHIC1_SCALE 384
#define SHAMAN_GRAPHIC4_START_SCALE 512
#define SHAMAN_GRAPHIC4_SMALL_SCALE 384
#define SHAMAN_GRAPHIC4_LARGE_SCALE 512
#elif (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define SHAMAN_GRAPHIC1_SCALE 256
#define SHAMAN_GRAPHIC4_START_SCALE 384
#define SHAMAN_GRAPHIC4_SMALL_SCALE 256
#define SHAMAN_GRAPHIC4_LARGE_SCALE 384
#else
#define SHAMAN_GRAPHIC1_SCALE 256
#define SHAMAN_GRAPHIC4_START_SCALE 512
#define SHAMAN_GRAPHIC4_SMALL_SCALE 256
#define SHAMAN_GRAPHIC4_LARGE_SCALE 384
#endif

s16 gData_0200515a[] = {
    0x4000, -188, 0x800, 0x1000, SHAMAN_GRAPHIC1_SCALE, 1, 0x2000, 16,
    1, 0x7fff, 14, 0x3000, 0, 20, 0x7fff, 60,
    0x3000, 0x800, 20, 0x7fff, 60, -1,
};

s16 gData_02005186[] = {
    0x1000, SHAMAN_GRAPHIC4_START_SCALE, 1, 0x2000, 0, 1,
    0x1000, SHAMAN_GRAPHIC4_SMALL_SCALE, 6, 0x2000, 16, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_LARGE_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_SMALL_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_LARGE_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_SMALL_SCALE, 6, 0x7fff, 6, 0x2000, 0, 30,
    0x1000, SHAMAN_GRAPHIC4_LARGE_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_SMALL_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_LARGE_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_SMALL_SCALE, 6, 0x7fff, 6,
    0x1000, SHAMAN_GRAPHIC4_LARGE_SCALE, 6, 0x7fff, 6, -1,
};
// clang-format on
