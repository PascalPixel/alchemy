#include "types.h"
#include "gs1_edition.h"

#define BattlePresentation_AdvanceTransitionTimer Func_080c01bc

#if defined(GS1_EDITION_DE)
#define TIMER_CELL_ADDR 0x03001F08
#define POSITION_ADDR   0x03001AE0
#else
#define TIMER_CELL_ADDR 0x03001EF8
#define POSITION_ADDR   0x03001AD0
#endif

s32 Func_080c0cec(s32, s32, s32, s32);

struct Display080c01bc {
  u8 padding_00[0x36];
  s16 field_36;
};

struct Position080c01bc {
  s16 field_00;
  s16 field_02;
};

void BattlePresentation_AdvanceTransitionTimer(void)
{
  s32 v;
  struct Display080c01bc *disp;
  u32 *timer;
  struct Position080c01bc *pos;
  u32 t;
  u32 next;
  timer = *((u32 **)TIMER_CELL_ADDR);
  t = *timer;
  disp = *((struct Display080c01bc **)(TIMER_CELL_ADDR - 0x78));
  v = 0x34 - t;
  if (v > 0x20)
  {
    if (1)
    {
      v = 0x20;
    }
  }
  pos = (struct Position080c01bc *)POSITION_ADDR;
  if (v < 0)
  {
    if (v || t)
    {
      v = 0;
    }
    else
    {
      v = 0;
    }
  }
  pos->field_02 = (s16) v;
  if (t <= 0x50U)
  {
    disp->field_36 = (s16) (((45 * t) * 8) + 0xAF80);
  }
  next = (*timer = (*timer) + 1);
  if (next <= 0x50U)
  {
    Func_080c0cec(0, 0, 0, 0xB4 - next);
    return;
  }
  Func_080c0cec(0, 0, 0, 0x64);
}
