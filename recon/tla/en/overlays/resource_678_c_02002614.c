// Draft, differ 121 of 112: sets up the swinging platforms' table (gData_020023c4
// + 0x188): unless flag 265 is set, clears it and records up to 16 points
// from a list of halfword triplets ended by 0 (x, z on the ground's height,
// and a third value) and up to 4 halfwords from a second list ended by 0;
// stores the third argument, sets a 12/8 blend if none is set and schedules
// Func_020025d4. The same listing as overlay 67c's Func_02003ab0. Left: the
// native keeps the point pointer in r8 and spills the count around the
// height call; this draft spills the pointer, which moves most rows.
#include "types.h"
#include "iwram_call.h"
#include "system/io_reg.h"

struct SwingPoint {
  u16 unk00;
  u16 index;
  s32 x;
  s32 y;
  s32 z;
  s32 w;
};

extern u8 gData_020023c4[];
s32 GameFlag_Test(s32 flag);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_020025d4(void);

void Func_02002614(u16 *points, u16 *values, s32 arg) {
  u8 *table = gData_020023c4 + 0x188;
  struct SwingPoint *point = (struct SwingPoint *)(table + 8);
  s32 x, z, w, i;

  if (!GameFlag_Test(265)) {
    Iwram_ClearWords(table, 0x150);
    x = *points++;
    for (i = 0; x > 0;) {
      z = points[0];
      w = points[1];
      point->unk00 = 0;
      point->z = z << 16;
      point->x = x << 16;
      point->y = Map_GetTerrainHeight(0, x << 16, (z << 16) + 0x100000);
      point->w = w << 16;
      point->index = i;
      i++;
      points += 2;
      point++;
      if (i > 15)
        break;
      x = *points++;
    }
    if (values != 0) {
      x = *values++;
      for (i = 0; x > 0;) {
        ((u16 *)(table + 0x148))[i] = x;
        i++;
        if (i > 3)
          break;
        x = *values++;
      }
    }
  }
  *(s32 *)(table + 4) = arg;
  if (REG_BLDCNT == 0) {
    REG_BLDALPHA = 0xc08;
    REG_BLDCNT = 0x3f10;
  }
  Scheduler_AddOrUpdateCallback(Func_020025d4, 0x480);
}
