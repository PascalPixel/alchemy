#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "gs1_edition.h"
#include "runtime_interfaces.h"

/* ui/render/output_list/acquire_free.c */
void *RenderOutput_AcquireFree(void)
{
  void **p;
  void *state;
  /* Detach and return the head of the free list. */
  state = *((void **)ADDR_03001E8C);
  p = *((void ***)(((u8 *)state) + RENDER_FREE_HEAD_OFS));
  if (p != ((void *) 0))
  {
    if ((*p) == ((void *) 0))
    {
      *((s32 *)(((u8 *)state) + RENDER_FREE_TAIL_OFS)) = (s32)(state + RENDER_FREE_HEAD_OFS);
    }
    *((void ***)(((u8 *)state) + RENDER_FREE_HEAD_OFS)) = *p;
    *p = (void *) 0;
  }
  return p;
}

/* ui/render/output_list/release_free.c */
extern struct State_080173ac *gIw;

void RenderOutput_ReleaseFree(u32 arg0)
{
    u8 *base = (u8 *)gIw;
    /* 管理領域内の要素だけを空きリストへ戻す。 */
    if (arg0 >= (u32)(base + 0x698) && arg0 < (u32)(base + 0xd98)) {
        u32 old = *(u32 *)(base + 0xd9c);
        *(u32 *)(base + 0xd9c) = arg0;
        *(u32 *)old = arg0;
        *(u32 *)arg0 = 0;
    }
}

/* ui/runtime/init/init_free_list.c */

void UiWork_InitFreeList(void)
{
    s32 count;
    u8 *base;
    u8 *item;
    u8 *next;

    base = (u8 *)gIw;
    /* 0x1cバイト単位の空きリストを初期化する。 */
    item = base + 0x698;
    *(u8 **)(base + 0xd98) = item;
    count = 0x3e;
    do {
        next = item + 0x1c;
        count--;
        *(u8 **)item = next;
        item = next;
    } while (count >= 0);
    *(s32 *)next = 0;
    *(u8 **)(base + 0xd9c) = next;
}
