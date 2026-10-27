#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "system.h"
#include "mem.h"
#include "types.h"
#include "ram_buffer.h"
#include "scene.h"
#include "select.h"
#include "text/msg_ids.h"

struct UiWindow *UiWindow_Create(s32, s32, s32, s32, s32);

struct SelectionNode *NodeChain_GetNodeAtCount(struct SelectionScreen *, u32);
void UiText_DrawCharacterAtOffset(s32, struct UiWindow *, s32, s32);
void RenderOutput_PrepareForRedraw(struct UiWindow *);

void Menu_OpenSelectionWindow(s32 mode, u32 count) {
  struct SelectionScreen *screen;
  struct SelectionNode *node;
  struct UiWindow **slot;
  struct UiWindow *window;

  screen = Ram_HeapSlots->menuWork;
  node = NodeChain_GetNodeAtCount(screen, count);
  slot = &screen->window;
  window = *slot;
  if (window == 0) {
    if (mode == 6) {
      if (screen->locked != 0) {
        *slot = UiWindow_Create(17, 17, 5, 3, mode);
      } else {
        *slot = UiWindow_Create(17, 0, 5, 3, mode);
      }
      screen->verticalScroll = 0;
      screen->locked = 999;
    } else {
      *slot = UiWindow_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
    }
    RenderOutput_PrepareForRedraw(screen->window);
  } else {
    if (count != 0 && window->width != count + 2) {
      UiWork_Finalize(window, 2);
      *slot = UiWindow_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
    }
    RenderOutput_PrepareForRedraw(screen->window);
  }
  if (screen->count != 0) {
    UiText_DrawCharacterAtOffset(node->message, screen->window, 0, 0);
  } else {
    switch (mode) {
    case 4:
      UiText_DrawCharacterAtOffset((s32)MsgAbilityNotKnown, screen->window, 0,
                                   0);
      break;
    case 2:
      UiText_DrawCharacterAtOffset((s32)MsgItemNotHeld, screen->window, 0, 0);
      break;
    }
  }
}

void Resource_ResetOwnerEntries(void) {
  struct SelectionScreen *state = Ram_HeapSlots->menuWork;
  struct SelectionNode *node;

  Resource_ScheduleOwnerReset();
  UiWork_Finalize(state->window, 2);
  WaitFrames(1);
  node = state->head;
  while (node != 0) {
    if (node->kind != 0) {
      Resource_ResetEntry(node->slot);
      node->kind = 0;
    }
    node = node->next;
  }
  node = state->path;
  while (node != 0) {
    if (node->kind != 0) {
      Resource_ResetEntry(node->slot);
      node->kind = 0;
    }
    node = node->next;
  }
  Resource_ResetPendingTransfer();
  if (state->records[0].y != 0) {
    Resource_ResetEntry(state->records[0].slot);
    if (state->records[0].y != 0) {
      Resource_ResetEntry(state->records[1].slot);
    }
  }
  Resource_ResetEntry(state->records[14].slot);
  Runtime_ReleaseHeapBlock(72);
}
