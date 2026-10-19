#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiflow.h"

struct UiMessageSlot {
  struct UiWindow *window;
  s32 result;
  s32 cursor;
};

s32 UiText_BuildRenderEntries(s32 message, s32 mode);
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 style);
void Func_0803a2b0(s32 x, s32 y, s32 width, s32 height, s32 mode);
s32 UiText_StartBattleMessage(struct UiWindow *window, s32 entry, s32 cursor);

void UiMessage_Show(s32 message) {
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  struct UiMessageSlot *slot = Ram_HeapSlots->unk84[4];
  struct UiWindow *window;
  s32 entry;
  s32 result;

  work->menuState = 2;
  entry = UiText_BuildRenderEntries(message, 1);
  work->menuState = 1;
  /* FAKEMATCH: ⚓️ stores the menu state before it addresses the entry */
  __asm__ volatile("");
  result = 0;
  if (work->entries[entry] != 0) {
    if (slot->window == NULL) {
      window = UiWindow_Create(0, 15, 30, 6, 10);
      slot->window = window;
      Func_0803a2b0(0, 15, 30, 6, 1);
      slot->cursor = result;
    } else {
      window = slot->window;
    }
    if (window != NULL) {
      result = UiText_StartBattleMessage(window, entry, slot->cursor);
      slot->result = result;
      slot->cursor = 0;
      if (result == 0)
        UiWork_Finalize(window, 1);
    }
  }
}
