#include "types.h"
#include "select.h"

void Menu_ReloadNodeResource(struct SelectionScreen *screen, u32 index);
void Menu_LoadSelectionNodeResource(struct SelectionScreen *screen, u32 index);
void wait(s32 frames);
void MenuSelection_SetupEntry(u32 kind, s32 base, struct SelectionNode *node,
                              s32 unused);
void Menu_ScrollSelectionList(struct SelectionScreen *screen, u32 down);
void Menu_OpenSelectionWindow(s32 mode, u32 count);

void Menu_StepRight(struct SelectionScreen *state) {
  struct SelectionNode *node;
  u32 entryIndex;
  s32 y;
  u32 end;

  Menu_ReloadNodeResource(state, state->cursorIndex);
  state->status = 33;
  wait(1);
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
        wait(1);
      node = state->head;
      if (node != NULL) {
        entryIndex = 0;
        do {
          MenuSelection_SetupEntry(state->kinds[entryIndex],
                                   state->bases[entryIndex], node, 1);
          node = node->next;
          entryIndex++;
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
      do {
      } while (0);
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
  do {
  } while (0);
  Menu_LoadSelectionNodeResource(state, state->cursorIndex);
  wait(1);
  Menu_OpenSelectionWindow(state->head->kind, 0);
  wait(1);
}
