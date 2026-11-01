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

struct SwingWork {
  u32 unk00;
  s32 arg;
  struct SwingPoint points[16];
  u16 values[4];
};

extern u8 gData_020023c4[];
s32 GameFlag_Test(s32 flag);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
s32 Task_Add(void (*func)(void), s32 priority);
void Func_020025d4(void);

void Func_02002614(u16 *points, u16 *values, s32 arg) {
  struct SwingWork *work = (struct SwingWork *)(gData_020023c4 + 0x188);
  struct SwingPoint *point = work->points;

  if (!GameFlag_Test(0x109)) {
    s32 x;
    s32 i;

    Iwram_ClearWords(work, 0x150);
    x = *points++;
    i = 0;
    while (x > 0) {
      s32 z = points[0];
      s32 w = points[1];

      point->unk00 = 0;
      point->x = x << 16;
      point->z = z << 16;
      point->y = Map_GetTerrainHeight(0, x << 16, (z << 16) + 0x100000);
      point->w = w << 16;
      point->index = i++;
      points += 2;
      point++;
      if (i > 15)
        break;
      x = *points++;
    }
    if (values != 0) {
      /* FAKEMATCH(barrier): the form kept; as no asm, 89 of 112 instructions
       * differ in tla-en. */
      asm(""
          :
          : "r"(values), "r"(values), "r"(values), "r"(values), "r"(values));
      x = *values++;
      i = 0;
      if (x > 0) {
      loop2:
        work->values[i++] = x;
        if (i > 3)
          goto done2;
        x = *values++;
        if (x > 0)
          goto loop2;
      }
    }
  done2:;
  }
  work->arg = arg;
  if (REG_BLDCNT == 0) {
    {
      s32 alpha = 0xc08;
      REG_BLDALPHA = alpha;
    }
    {
      s32 cnt = 0x3f10;
      REG_BLDCNT = cnt;
    }
  }
  Task_Add(Func_020025d4, 0x480);
}
