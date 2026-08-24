#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

s32 Func_080f9048();
s32 Func_080199ec(void *arg0)
{
  void *new_var3;
  s32 new_var4;
  unsigned char new_var;
  s32 var_r2;
  s32 var_r6;
  void *temp_r5;
  int new_var2;
  new_var3 = *((void **) ADDR_03001E8C);
  temp_r5 = new_var3;
  var_r6 = 0;
  if (((*((u8 *) (((u8 *) temp_r5) + RENDER_BUSY_OFS))) != 0) && (Func_080f9048() == 0))
  {
    var_r6 = 1;
  }
  var_r2 = (new_var4 = *((s32 *) ADDR_03001C94));
  new_var = 0;
  if ((*((u8 *) (temp_r5 + RENDER_MODE_OFS))) != new_var)
  {
    var_r2 = *((s32 *) ADDR_03001AF8);
  }
  if (0x303 & var_r2)
  {
    var_r6 = 1;
  }
  if (var_r6 != 0)
  {
    ;
    *((s16 *) (((u8 *) arg0) + 0x14)) = new_var;
    return 1;
  }
  return 0;
}
