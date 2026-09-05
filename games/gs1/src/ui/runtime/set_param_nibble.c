#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

void UiWork_SetParamNibble(s32 param)
{
  s16 val;
  void *work;
  work = *((void **) ADDR_03001E8C);
  val = (s16) (param & 0xF);
 do { *((s16 *) (work + RENDER_PARAM_OFS)) = val; } while (0);
}
