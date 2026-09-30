#include "TYPES.H"
#include "SCENE.H"
#include "RAM_BUFFER.H"
s32 BattlePres_SetupTransitionScene(s32, s32, s32, s32);

/* battle/presentation/trans/timer.c */
struct Display080c01bc {
  u8 padding_00[0x36];
  s16 field_36;
};

struct Position080c01bc {
  s16 field_00;
  s16 field_02;
};

void BattlePres_AdvanceTransitionTimer(void)
{
  s32 v;
  struct Display080c01bc *disp;
  u32 *timer;
  struct Position080c01bc *pos;
  u32 t;
  u32 next;
  timer = *((u32 **)Ram_Disp);
  t = *timer;
  disp = *((struct Display080c01bc **)Ram_CameraWork);
  v = 0x34 - t;
  if (v > 0x20)
  {
    if (1)
    {
      v = 0x20;
    }
  }
  pos = (struct Position080c01bc *)Ram_BgScroll;
  if (v < 0)
  {
    if (v || t)
    {
      v = 0;
    } else
    {
      v = 0;
    }
  }
  pos->field_02 = (s16)v;
  if (t <= 0x50U)
  {
    disp->field_36 = (s16)(((45 * t) * 8) + 0xAF80);
  }
  next = (*timer = (*timer) + 1);
  if (next <= 0x50U)
  {
    BattlePres_SetupTransitionScene(0, 0, 0, 0xB4 - next);
    return;
  }
  BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
}
