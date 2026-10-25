#include "types.h"
#include "graphics.h"
#include "window.h"
#include "ram_buffer.h"

extern const s16 gPortrait_CharacterMap[];
extern const s16 gPortrait_SpriteMap[];

struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 flags);
s32 CreateSideObject(s32 id, s32 kind, s32 slot, struct UiWindow *window, s32 x,
                     s32 y);

void UiWork_ClearPortraitSlots(void) {
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  s32 index = 0;

  do {
    work->resourceIds[index] = 999;
    index++;
  } while (index != 2);
}
