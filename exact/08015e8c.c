#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

void *Func_08015e8c(void)
{
  void **temp_r0;
  void **temp_r2;
  void *temp_r3;
  /* 空きリストの先頭を1個取り外して返す。 */
  temp_r3 = *((void **) ADDR_03001E8C);
  temp_r0 = *((void ***) (((u8 *) temp_r3) + RENDER_FREE_HEAD_OFS));
  if (temp_r0 != ((void *) 0))
  {
    ;
    if ((*temp_r0) == ((void *) 0))
    {
      *((s32 *) (((u8 *) temp_r3) + RENDER_FREE_TAIL_OFS)) = (s32) (temp_r3 + RENDER_FREE_HEAD_OFS);
    }
    *((void ***) (((u8 *) temp_r3) + RENDER_FREE_HEAD_OFS)) = *temp_r0;
    *temp_r0 = (void *) 0;
  }
  return temp_r0;
}
