#include "item_icon.h"

void Func_0810bdb0(void) {
  struct IconWork *work = Ram_HeapSlots->menuRuntime;

  /* FAKEMATCH: the native loads the menu work before it builds the field offset
   * (one reordered row). */
  __asm__ volatile("" : "+l"(work));
  if (work->active) {
    struct IconSprite *sprite = work->sprites;
    u16 *resource = work->resources;
    s32 i;

    for (i = ICON_COUNT - 1; i >= 0; i--) {
      if (*resource++ != ICON_NONE)
        Oam_AddSpriteAtDepth(sprite, 240);
      sprite++;
    }
  }
}
