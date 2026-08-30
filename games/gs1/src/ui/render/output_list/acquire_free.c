#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

void *RenderOutput_AcquireFree(void)
{
  void **free_output;
  void *render_state;
  /* Detach and return the head of the free list. */
  render_state = *((void **) ADDR_03001E8C);
  free_output = *((void ***) (((u8 *) render_state) + RENDER_FREE_HEAD_OFS));
  if (free_output != ((void *) 0))
  {
    if ((*free_output) == ((void *) 0))
    {
      *((s32 *) (((u8 *) render_state) + RENDER_FREE_TAIL_OFS)) = (s32) (render_state + RENDER_FREE_HEAD_OFS);
    }
    *((void ***) (((u8 *) render_state) + RENDER_FREE_HEAD_OFS)) = *free_output;
    *free_output = (void *) 0;
  }
  return free_output;
}
