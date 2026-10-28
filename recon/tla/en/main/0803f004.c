// Draft, differ 18 of 223 (2026-10-26, 20 before): the forward branch sets the
// new node's dx right after its y. Earlier notes follow.
// Draft, differ 20 of 223 in tla-en under -mthumb-slow-loads (2026-10-26). A
// device search (each statement followed by an empty asm or a one-pass block,
// wrapped in one, or swapped with the next, kept when it helps) reaches 6 with
// an empty asm after `p->x = screen->baseX - 16;`; an empty asm after
// `p->scaleEnd = 256;`; an empty asm after `first->prev = NULL;`; an empty asm
// after `p->scaleStep = -32;`, and finds nothing closer.
// Near miss: The Broken Seal source unchanged; nine reordered rows, all of one
// kind. The native stores a node field as soon as its value is ready and
// finishes the next field offset afterwards (movs r4, #230; adds r3, #80; strh
// r3, [r5, #16]; lsls r4, r4, #2); the draft finishes the split constant first.
// The two halves of the constant straddle the store, so a barrier between the
// statements cannot order them.
#include "types.h"
#include "select.h"

#define SELECTION_ROWS 4

struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind);
void MenuSelection_SetupEntry(u32 kind, s32 base, struct SelectionNode *node,
                              s32 reuse);
void Resource_ResetEntry(s32 slot);
void wait(s32 frames);

void Menu_ScrollSelectionList(struct SelectionScreen *screen, s32 forward) {
  struct SelectionNode *p;
  struct SelectionNode *last;
  struct SelectionNode *first;
  s32 base;
  s32 kind;
  u32 index;
  s32 y;
  s32 z;

  if (forward != 0) {
    index = screen->top + SELECTION_ROWS;
    base = screen->bases[index];
    kind = screen->kinds[index];
    p = Resource_FindFreeTransferEntry(0);
    if (p == NULL)
      return;
    MenuSelection_SetupEntry(kind, base, p, 0);
    y = screen->baseX;
    p->x = y + 80;
    z = screen->baseY;
    p->targetX = y + 64;
    p->y = z;
    p->dx = -2;
    p->targetY = z;
    p->scaleStep = 32;
    p->scale = 32;
    p->scaleEnd = 256;
    last = p;
    p = screen->head;
    p->scaleStep = -32;
    p->scaleEnd = 0;
    for (;;) {
      p->targetX = p->x - 16;
      p->dx = -2;
      if (p->next == NULL)
        break;
      p = p->next;
    }
    p->next = last;
    last->next = NULL;
    last->prev = p;
    p = screen->head;
    do {
      wait(1);
    } while (p->scale != 0);
    screen->head = p->next;
    Resource_ResetEntry(p->slot);
    p->kind = 0;
    p = p->next;
    p->prev = NULL;
  } else {
    index = screen->top;
    base = screen->bases[index];
    kind = screen->kinds[index];
    p = Resource_FindFreeTransferEntry(0);
    if (p == NULL)
      return;
    MenuSelection_SetupEntry(kind, base, p, 0);
    p->x = screen->baseX - 16;
    p->y = screen->baseY;
    p->targetY = p->y;
    p->dx = 2;
    p->scale = 32;
    p->scaleStep = 32;
    p->targetX = p->x + 16;
    p->scaleEnd = 256;
    first = p;
    p = screen->head;
    p->prev = first;
    first->next = p;
    first->prev = NULL;
    screen->head = first;
    p = first;
    for (;;) {
      p->targetX = p->x + 16;
      p->dx = 2;
      if (p->next == NULL)
        break;
      p = p->next;
    }
    p->scaleEnd = 0;
    p->scaleStep = -32;
    p = screen->head;
    do {
      wait(1);
    } while (p->scale != 256);
    while (p->next != NULL)
      p = p->next;
    Resource_ResetEntry(p->slot);
    p->kind = 0;
    p->prev->next = NULL;
  }
}
