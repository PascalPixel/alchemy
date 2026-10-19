/// @file
/// Display: free list of sprite outputs (RenderOutput).

#include "types.h"
#include "edition.h"
#include "window.h"
#include "ram_buffer.h"

#define UI_WORK ((struct UiRenderWork *)Ram_HeapSlots->windowTiles)

/// Remove the first entry from the free list, or return NULL if empty.
struct RenderOutput *RenderOutput_AcquireFree(void) {
  struct UiRenderWork *work = UI_WORK;
  struct RenderOutput *out = work->freeOutputs.head;

  if (out != NULL) {
    if (out->next == NULL)
      work->freeOutputs.tailLink = &work->freeOutputs.head; // Now empty.
    work->freeOutputs.head = out->next;
    out->next = NULL;
  }
  return out;
}

/// Return an entry to the free-list tail; reject entries outside outputs[].
void RenderOutput_ReleaseFree(struct RenderOutput *out) {
  struct UiRenderWork *work = UI_WORK;

  if ((u32)out >= (u32)work->outputs &&
      (u32)out < (u32)&work->freeOutputs.head) {
    struct RenderOutput **tail = work->freeOutputs.tailLink;

    work->freeOutputs.tailLink = &out->next;
    *tail = out;
    out->next = NULL;
  }
}

/// Link all 64 outputs[] entries into the free list.
void UiWork_InitFreeList(void) {
  s32 n;
  struct UiRenderWork *work;
  struct RenderOutput *out;
  struct RenderOutput *next;

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
