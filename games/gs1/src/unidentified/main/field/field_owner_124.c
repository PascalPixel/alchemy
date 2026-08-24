#include "types.h"
#include "gs1_edition.h"

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

void Func_080c01bc(void)
{
  s32 var_r2;
  struct Display080c01bc *new_var2;
  u32 *temp_r0;
  struct Position080c01bc *new_var;
  u32 temp_r1;
  u32 temp_r2;
  temp_r0 = *((u32 **)TIMER_CELL_ADDR);
  temp_r1 = *temp_r0;
  new_var2 = *((struct Display080c01bc **)(TIMER_CELL_ADDR - 0x78));
  var_r2 = 0x34 - temp_r1;
  if (var_r2 > 0x20)
  {
    if (1)
    {
      var_r2 = 0x20;
    }
  }
  new_var = (struct Position080c01bc *)POSITION_ADDR;
  if (var_r2 < 0)
  {
    if (var_r2 || temp_r1)
    {
      var_r2 = 0;
    }
    else
    {
      var_r2 = 0;
    }
  }
  new_var->field_02 = (s16) var_r2;
  if (temp_r1 <= 0x50U)
  {
    new_var2->field_36 = (s16) (((45 * temp_r1) * 8) + 0xAF80);
  }
  temp_r2 = (*temp_r0 = (*temp_r0) + 1);
  if (temp_r2 <= 0x50U)
  {
    Func_080c0cec(0, 0, 0, 0xB4 - temp_r2);
    return;
  }
  Func_080c0cec(0, 0, 0, 0x64);
}
