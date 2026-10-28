#include "render_input.h"
#include "output.h"
#include "resource.h"
#include "ram_buffer.h"
#include "edition.h"

#if (LANGUAGE != LANGUAGE_JAPANESE)
#define UI_PALETTE_OWNER_OFFSET 0x1360
#else
#define UI_PALETTE_OWNER_OFFSET 0x1160
#endif

void WindowSprite_UpdateListTail(struct WindowSpriteList *list) {
  struct WindowSprite *node = list->head;
  struct WindowSprite **tail = &list->head;

  while (node != NULL) {
    tail = &node->next;
    node = node->next;
  }
  list->tailLink = tail;
}

void WindowSprite_AppendToList(struct WindowSpriteList *list,
                               struct WindowSprite *node) {
  /* FAKEMATCH: the first-word tail pointer access preserves the two-store
   * order. */
  if (list != NULL) {
    *list->tailLink = node;
    *(struct WindowSprite **)&list->tailLink = node;
  }
}

void WindowSprite_Release(struct WindowSprite *entry) {
  WindowSprite_ReleaseFree(entry);
  if ((u8)entry->kind != 0) {
    Resource_ResetEntry((u8)entry->index);
    if ((u8)entry->kind == 2) {
      u8 *dst = (u8 *)Ram_HeapSlots->windowTiles;
      s32 idx = entry->oam.palette * 2 + UI_PALETTE_OWNER_OFFSET;
      *(u16 *)(dst + idx) = 0x3E7;
    }
  }
  entry->active = 0;
}
