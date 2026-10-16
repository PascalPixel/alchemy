// Near miss, differ 14 of 212, 2026-10-15: Menu_StepRight on the shared
// SelectionScreen and SelectionNode of menu/select.h, which reads the row
// targets baseX and baseY as the native does without the earlier draft's
// FAKEMATCH halfword macros (the same 14 either way). Every instruction is
// the native's and only the order differs, in four places: the native
// stores the incremented cursor before it builds the count's 0x394 offset,
// this after; the native builds the 0x398 offset after the last node's
// first target store, this before; before scrolling down the native copies
// the screen into r0 before loading top and sets r1 before the increment,
// this after both; and at the end the native finishes the cursor's 0x39e
// offset after the status store, this before it.
#include "types.h"
#include "select.h"

void Menu_ReloadNodeResource(struct SelectionScreen *screen, u32 index);
void Menu_LoadSelectionNodeResource(struct SelectionScreen *screen, u32 index);
void WaitFrames(s32 frames);
void MenuSelection_SetupEntry(u32 kind, u32 base, struct SelectionNode *node,
                              s32 mode);
void Menu_ScrollSelectionList(struct SelectionScreen *screen, s32 direction);
void Menu_OpenSelectionWindow(s32 mode, u32 count);

void Menu_StepRight(struct SelectionScreen *state) {
  struct SelectionNode *node;
  u16 *ids;
  s32 y;
  u32 end;

  Menu_ReloadNodeResource(state, state->cursorIndex);
  state->status = 33;
  WaitFrames(1);
  state->cursorIndex++;
  if (state->count > 5) {
    end = state->top + state->cursorIndex;
    if (end == state->count) {
      node = state->head;
      state->records[0].kind = 0;
      while (node->next != NULL) {
        node->targetX = state->baseX;
        node->targetY = state->baseY;
        node->dx = -12;
        node = node->next;
      }
      node->targetX = state->baseX;
      node->targetY = state->baseY;
      node->dx = -12;
      while (node->targetX != node->x)
        WaitFrames(1);
      node = state->head;
      if (node != NULL) {
        ids = state->kinds;
        do {
          MenuSelection_SetupEntry(ids[0], ids[16], node, 1);
          node = node->next;
          ids++;
        } while (node != NULL);
      }
      state->cursorIndex = 0;
      state->top = 0;
      node = state->head;
      y = node->x + 16;
      node = node->next;
      while (node != NULL) {
        node->targetX = y;
        node->dx = 12;
        node = node->next;
        y += 16;
      }
      state->records[1].kind = 1;
    } else if (state->cursorIndex == 4 && end + 1 < state->count) {
      state->cursorIndex--;
      state->records[1].base = 8;
      state->top++;
      Menu_ScrollSelectionList(state, 1);
      if (state->top + state->cursorIndex + 2 == state->count)
        state->records[1].kind = 0;
      state->records[0].kind = 1;
    }
  } else if (state->cursorIndex == state->count) {
    state->cursorIndex = 0;
  }
  state->status = 1;
  Menu_LoadSelectionNodeResource(state, state->cursorIndex);
  WaitFrames(1);
  Menu_OpenSelectionWindow(state->head->kind, 0);
  WaitFrames(1);
}
