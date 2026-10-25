#include "types.h"
#include "window.h"
#include "ram_buffer.h"

#define UI_WORK ((struct UiRenderWork *)Ram_HeapSlots->windowTiles)

s32 UiText_BuildRenderEntries(s32 key, s32 mode);
void UiWindow_FitOnScreen(s32 entry, s32 *x, s32 *y, u32 *width, u32 *height,
                          u16 *bounds, s32 mode);
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 attrs);
s32 UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y,
                              u16 *bounds, s32 mode);

struct UiWindow *UiText_OpenMessageWindow(s32 message, s32 x, s32 y,
                                          u32 packed) {
  s32 entry;
  u32 height;
  u32 width;
  u16 bounds[4];
  s32 flags;
  u16 *out;
  struct UiWindow *window;
  struct UiRenderWork *work;

  work = UI_WORK;
  work->speakerVoiceSound = (packed << 4) >> 20;
  window = NULL;
  work->speakerVoiceCountdown = 0;
  packed &= 0xffff;
  flags = 0;
  entry = UiText_BuildRenderEntries(message, 1);
  if (work->entries[entry] == 0)
    return NULL;
  UiWindow_FitOnScreen(entry, &x, &y, &width, &height, (out = bounds), 0);
  if (width == 0 && height == 0)
    return NULL;
  if (!(packed & 1))
    flags |= 2;
  if (packed & 8)
    flags |= 8;
  if (packed & 16)
    flags |= 128;
  if (packed & 32)
    flags |= 256;
  window = UiWindow_Create(x, y, width, height, flags);
  if (window == NULL)
    return NULL;
  if (UiText_QueueRenderEntries(window, entry, 0, 0, out, 0) == 0) {
    UiWork_Finalize(window, 1);
    return NULL;
  }
  work->unk138a[0] = 0;
  work->unk138a[1] = 0;
  return window;
}
