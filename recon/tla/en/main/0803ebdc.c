// Draft, differ 6 of 212, skeleton gap 3 (tla-en, 2026-11-01)
// Twin of SelectionMenu_StepRight in The Broken Seal: same statements, locals,
// loops and row logic, already ported, so the transfer gained nothing. Tried
// on 2026-11-01, none below 6: a count local (149), a count pointer (19),
// `++` in `end`'s assignment (196), a u16 cursor temporary (18), a cursor
// local at the end (10, 20), a head local at the end (10), the twin's plain
// records[1].base store (8), `top += 1` and `cursor = cursor + 1` (6, no gain).
// Near miss: scheduler order, three
// single instructions, each where the native waits longer after a store than
// -mthumb-slow-loads charges. (1) After the cursor's increment the native
// stores it before shifting the count's 0x394 offset: the sched2 dump ranks
// the store 5 and the shift 6, because the count load is a zero-extended
// ldrh, which the store-then-load rule skips; at cost 2 the two would tie and
// RTL order would give the native's. (2) Before scrolling, the native copies
// the screen into r0 between the arrow store and the top load: the top load
// opens top++'s read-modify-write, which the rule excuses, so it issues at
// once. (3) At the end the native builds the first instruction of the
// cursor's 0x39e offset (movs r0, #192) before the status store, as in its
// sibling 0803eb2c, where the dump needs a three-cycle wait. No C form makes
// either load plain or moves the costs; the two one-pass blocks below are the
// mildest devices that hold 6 (an empty asm after the status store does no
// better). The tree's prototypes and the Broken Seal twin's indexed entry loop
// compile the same. Permuter (4 minutes, 16944 candidates): nothing below 6.
// Earlier (2026-10-15): every instruction is the native's and only the order
// differs; Menu_StepRight on the shared SelectionScreen and SelectionNode of
// menu/select.h reads the row targets baseX and baseY as the native does.
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
      do {
        /* FAKEMATCH(one-pass): the form kept; as a plain records[1].base store, 8 of 212 instructions differ in tla-en. */
        state->records[1].base = 8;
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
  do {
    /* FAKEMATCH(one-pass): the form kept; as a plain status store, 10 of 212 instructions differ in tla-en. */
    state->status = 1;
  } while (0);
  Menu_LoadSelectionNodeResource(state, state->cursorIndex);
  wait(1);
  Menu_OpenSelectionWindow(state->head->kind, 0);
  wait(1);
}
