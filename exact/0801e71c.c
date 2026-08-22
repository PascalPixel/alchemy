#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

void Func_0801e71c(s32 arg0)
{
  s16 new_var2;
  void *new_var;
  new_var = *((void **) ADDR_03001E8C);
  new_var2 = (s16) (arg0 & 0xF);
 do { *((s16 *) (new_var + RENDER_PARAM_OFS)) = new_var2; } while (0);
}
