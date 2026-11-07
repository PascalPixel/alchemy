/// @file
/// Display: free list of sprite outputs (WindowSprite).

#include "types.h"
#include "edition.h"
#include "window.h"
#include "ram_buffer.h"

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowTiles)

/// Removes and returns the first entry of the free list, or NULL if it is
/// empty.
struct WindowSprite *WindowSprite_AcquireFree(void) {
  struct WindowWork *work = UI_WORK;
  struct WindowSprite *out = work->freeOutputs.head;

  if (out != NULL) {
    if (out->next == NULL)
      work->freeOutputs.tailLink = &work->freeOutputs.head; // Now empty.
    work->freeOutputs.head = out->next;
    out->next = NULL;
  }
  return out;
}

/// Puts an entry back at the free list's tail; entries outside outputs[] are
/// ignored.
void WindowSprite_ReleaseFree(struct WindowSprite *out) {
  struct WindowWork *work = UI_WORK;

  if ((u32)out >= (u32)work->outputs &&
      (u32)out < (u32)&work->freeOutputs.head) {
    struct WindowSprite **tail = work->freeOutputs.tailLink;

    work->freeOutputs.tailLink = &out->next;
    *tail = out;
    out->next = NULL;
  }
}

/// Link all 64 outputs[] entries into the free list.
void WindowSprite_InitializeFreeList(void) {
  s32 n;
  struct WindowWork *work;
  struct WindowSprite *out;
  struct WindowSprite *next;

  work = UI_WORK;
  out = work->outputs;
  work->freeOutputs.head = out;
  n = UI_OUTPUT_COUNT - 2;
  do {
    next = out + 1;
    n--;
    out->next = next;
    out = next;
  } while (n >= 0);
  next->next = NULL;
  work->freeOutputs.tailLink = &next->next;
}
