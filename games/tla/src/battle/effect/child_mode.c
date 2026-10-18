#include "types.h"

struct ChildObject {
  u8 unk00[5];
  u8 mode;
  u8 unk06[10];
  s32 active;
};

struct ChildGroup {
  u8 unk00[25];
  u8 dirty;
  u8 unk1a;
  u8 count;
  u8 unk1c[12];
  struct ChildObject *children[1];
};

// Sets the draw parameter (byte 5) of each active child of group to mode
// and marks the group for redrawing.
void ObjectGroup_SetActiveChildParam(struct ChildGroup *group, s32 mode) {
  s32 i;

  for (i = 0; i < group->count; i++) {
    struct ChildObject *child = group->children[i];

    if (child != NULL && child->active != 0)
      *((u8 *)child + 5) = mode;
  }
  group->dirty = 1;
}

// Fills count bytes of table with a rising shade ramp (0 to 63, by a step
// scaled by scale), then repeats it seven more times.
void BattleFx_BuildShadeRamp(u8 *table, u32 count, s32 scale) {
  u32 step = count * 2;
  s32 total = 0;
  u32 i;
  s32 j;

  if (step > 63)
    step = 63;
  step = 63 / step;
  if (step < 6)
    step = 6;
  for (i = 0; i != count; i++) {
    s32 value;

    total += step;
    value = (scale * total) >> 16;
    if (value > 63)
      value = 63;
    table[i] = value;
  }
  for (j = 1; j != 8; j++) {
    for (i = 0; i != count; i++)
      table[j * count + i] = table[i];
  }
}

// Fills table with a shade ramp that rises over count bytes and falls back,
// repeated repeats times.
void BattleFx_BuildShadeRampWave(u8 *table, u32 count, u32 repeats, s32 scale) {
  s32 total = 0;
  u32 step = count * 2;
  u32 i;
  u32 j;

  if (step > 63)
    step = 63;
  step = 63 / step;
  if (step < 6)
    step = 6;
  for (i = 0; i != count; i++) {
    s32 value;

    total += step;
    value = (scale * total) >> 16;
    if (value > 63)
      value = 63;
    table[i] = value;
  }
  for (i = 0; i != count; i++)
    table[count + i] = table[count - 1 - i];
  for (j = 1; j != repeats; j++) {
    for (i = 0; i != count * 2; i++)
      table[j * count * 2 + i] = table[i];
  }
}

// As BattleFx_BuildShadeRampWave, at twice the step.
void BattleFx_BuildSteepShadeRampWave(u8 *table, u32 count, u32 repeats, s32 scale) {
  s32 total = 0;
  u32 step = count * 2;
  u32 i;
  u32 j;

  if (step > 63)
    step = 63;
  step = 63 / step;
  if (step < 6)
    step = 6;
  step *= 2;
  for (i = 0; i != count; i++) {
    s32 value;

    total += step;
    value = (scale * total) >> 16;
    if (value > 63)
      value = 63;
    table[i] = value;
  }
  for (i = 0; i != count; i++)
    table[count + i] = table[count - 1 - i];
  for (j = 1; j != repeats; j++) {
    for (i = 0; i != count * 2; i++)
      table[j * count * 2 + i] = table[i];
  }
}
