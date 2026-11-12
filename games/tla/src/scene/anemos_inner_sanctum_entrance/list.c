#include "types.h"
#include "actor.h"
#include "event_inline.h"

extern s16 gData_02001cde;
extern struct Actor *gData_02001ce0[];
extern u16 gData_02001de2;
extern u16 gData_02001de4;
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

void Func_02000ba4(s32 id, s32 value) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    object->timer = value;
    gData_02001ce0[gData_02001cde] = object;
    gData_02001cde++;
  }
}

void Func_02000bd4(u16 value) { gData_02001de2 = value; }

void Func_02000be0(u16 value) { gData_02001de4 = value; }

#include "scene_tables.h"

// clang-format off
u32 gData_02000d6c[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02000da8[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02000de4[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s32 gData_02000e20[] = {
    23, 9, 0x400, 23, 10, 0x400,
    23, 6, -0x4000, 0, 1, 13,
    40, 0, 17,
};

s32 gData_02000e5c[] = {
    23, 9, -2048, 23, 10, -2048,
    23, 6, 0x4000, 0, 1, 13,
    24, 0, 3, 0, 1, 17,
};

s32 gData_02000ea4[] = {
    23, 9, -1024, 23, 10, -1024,
    0, 1, 13, 40, 0, 3,
    0, 1, 17,
};
// clang-format on

u32 gData_02000ee0[] = {
    0x1c0b1b0b,
    0x1c0c1b0c,
    0x1c0d1b0d,
};

u16 gData_02000eec[] = {
    0xe0c,
    0xb09,
    0xa0d,
};
