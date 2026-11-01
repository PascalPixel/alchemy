// Matches: differ 0 (tla-en, 2026-11-01)
// Sets up a room's water outlets (up to sixteen spots from a zero-ended list,
// each on the ground of layer 0) and up to four levels, stores the handler,
// sets the blend for the water when none is set and starts the frame callback.
// The listing is instruction for instruction overlay 678's Func_02002614, whose
// draft matched first and calls the same table swinging platforms; neither name
// is settled by evidence yet. This takes that draft's shape: one counter across
// both lists, a plain while for the first and labels for the second, x stored
// before z, the two blend values as one-constant locals. The one device raises
// the second list's pointer in global.c's priority so it reaches r7 before the
// counter takes r4; 678's notes record what was tried without it.
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
      /* FAKEMATCH(barrier): the form kept; as no asm, 89 of 112 instructions differ in tla-en. */
      asm("" : : "r"(levels), "r"(levels), "r"(levels), "r"(levels), "r"(levels));
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
      s32 alpha = 0xc08;
      REG_BLDALPHA = alpha;
    }
    {
      s32 cnt = 0x3f10;
      REG_BLDCNT = cnt;
    }
  }
  Task_Add(Func_02003a70, 0x480);
}
