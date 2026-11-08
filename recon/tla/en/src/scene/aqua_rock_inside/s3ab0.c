#include "types.h"
#include "iwram_call.h"
#include "system/io_reg.h"

struct WaterOutlet {
  u16 state;
  u16 index;
  s32 x;
  s32 y;
  s32 z;
  s32 level;
};

struct WaterWork {
  u32 unk00;
  void (*handler)(void);
  struct WaterOutlet outlets[16];
  u16 levels[4];
};

extern u8 gData_020023c4[];

typedef void (*TaskFunc)(void);

s32 GameFlag_Test(s32 flag);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Task_Add(TaskFunc func, s32 order);
void Func_02003a70(void);

void Func_02003ab0(const u16 *spots, const u16 *levels, void (*handler)(void)) {
  struct WaterWork *work = (struct WaterWork *)(gData_020023c4 + 0x188);
  struct WaterOutlet *outlet = work->outlets;

  if (!GameFlag_Test(0x109)) {
    s32 x;
    s32 i;

    Iwram_ClearWords(work, 0x150);
    x = *spots++;
    i = 0;
    while (x > 0) {
      s32 z = spots[0];
      s32 level = spots[1];

      outlet->state = 0;
      outlet->x = x << 16;
      outlet->z = z << 16;
      outlet->y = Map_GetTerrainHeight(0, x << 16, (z << 16) + 0x100000);
      outlet->level = level << 16;
      outlet->index = i++;
      spots += 2;
      outlet++;
      if (i > 15)
        break;
      x = *spots++;
    }
    if (levels != 0) {
      x = *levels++;
      i = 0;
      if (x > 0) {
      loop2:
        work->levels[i++] = x;
        if (i > 3)
          goto done2;
        x = *levels++;
        if (x > 0)
          goto loop2;
      }
    }
  done2:;
  }
  work->handler = handler;
  if (REG_BLDCNT == 0) {
    {
      s32 alpha = SPLIT_CONSTANT(0xc08);
      REG_BLDALPHA = alpha;
    }
    {
      s32 cnt = SPLIT_CONSTANT(0x3f10);
      REG_BLDCNT = cnt;
    }
  }
  Task_Add(Func_02003a70, 0x480);
}
