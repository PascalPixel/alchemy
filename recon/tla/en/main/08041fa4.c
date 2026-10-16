// Draft, differ 10 of 49, three pairs in the other order: the native loads
// the window work before it starts the count's offset, loads the entry mask
// from the pool before it rereads the count, and advances to the entries
// before the mask is applied. Measured: an empty asm barrier after the
// window work swaps r5 and r6 (differ 22). Twin of The Broken Seal's
// UiText_DrawResource.
#include "text_render_runtime.h"
#include "window.h"
#include "ram_buffer.h"

s32 UiText_BuildRenderEntries(s32 message, s32 mode);
void UiText_RenderWideStringAtOffset(u16 *text, struct UiWindow *window, s32 x,
                                     s32 y);

void UiText_DrawResource(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
  struct UiRenderWork *canvas =
      (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  u16 *counter = &canvas->count;
  s32 zero = 0;

  *counter = zero;
  UiText_BuildRenderEntries(arg0, 1);
  canvas->entries[*counter] = zero;
  *counter = (*counter + 1) & (UI_TEXT_ENTRY_COUNT - 1);
  UiText_RenderWideStringAtOffset(canvas->entries, (struct UiWindow *)arg1,
                                  arg2, arg3);
}
