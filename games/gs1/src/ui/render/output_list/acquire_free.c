#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

void *RenderOutput_AcquireFree(void)
{
  void **p;
  void *state;
  /* Detach and return the head of the free list. */
  state = *((void **) ADDR_03001E8C);
  p = *((void ***) (((u8 *) state) + RENDER_FREE_HEAD_OFS));
  if (p != ((void *) 0))
  {
    if ((*p) == ((void *) 0))
    {
      *((s32 *) (((u8 *) state) + RENDER_FREE_TAIL_OFS)) = (s32) (state + RENDER_FREE_HEAD_OFS);
    }
    *((void ***) (((u8 *) state) + RENDER_FREE_HEAD_OFS)) = *p;
    *p = (void *) 0;
  }
  return p;
}
