// Draft, differ 24 of 276 (2026-10-26, 26 before): the tile copied after the
// home position. Earlier notes follow.
// Draft, differ 26 of 276 in tla-en under -mthumb-slow-loads (2026-10-26). A
// device search (each statement followed by an empty asm or a one-pass block,
// wrapped in one, or swapped with the next, kept when it helps) reaches 14
// with an empty asm after `other->scaleEnd = 0x100;`; an empty asm after
// `screen->head = NULL;`, and finds nothing closer.
// Draft, differ 34 of 276, 2026-10-15: The Broken Seal's
// Menu_ConfirmSelection, ported fresh; every instruction is the native's
// and only the order differs. Remaining: in five blocks the native
// schedules a constant, copy or pool load ahead of a load the draft
// issues first (the 0x80 mark's pool load before the cursor's ldrh, the
// sprite bits' constants before their byte loads, the path's offset after
// the node's stores). The empty asm after the node's target store fixes
// one of them (differ 38 without it); a u16 temporary for the 0x80 mark
// drops two instructions (differ 58). The function ends with the empty
// Func_0803f60c (bx lr) its listing holds, which joins it on adoption.
#include "types.h"
#include "select.h"

void Menu_SendNodeCountList(struct SelectionScreen *screen);
void Menu_ReloadNodeResource(struct SelectionScreen *screen, u32 index);
void WaitTicks(s32 ticks);
void Resource_ResetPendingTransfer(void);
s32 Resource_ResetEntry(u32 entry);
struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind);

u32 Menu_ConfirmSelection(struct SelectionScreen *screen) {
  struct SelectionNode *node;
  struct SelectionNode *other = NULL;
  u32 cnt = 0;
  u32 index;
  struct SelectionSprite *sprite;

  index = screen->top + screen->cursorIndex;
  Menu_SendNodeCountList(screen);
  Menu_ReloadNodeResource(screen, screen->cursorIndex);
  screen->status = 33;
  WaitTicks(1);
  screen->records[0].kind = 0;
  screen->records[1].kind = 0;
  screen->records[14].kind = 0;
  screen->records[14].scale = 0;
  Resource_ResetPendingTransfer();
  node = screen->head;
  while (node != NULL && screen->cursorIndex != cnt) {
    node = node->next;
    cnt++;
  }
  node->homeX = node->x;
  node->homeY = node->y;
  for (other = screen->head; other != NULL; other = other->next) {
    if (other != node) {
      other->targetX = node->x;
      other->dx = (node->x - other->x) >> 1;
    }
  }
  WaitTicks(2);
  for (other = screen->head; other != NULL; other = other->next) {
    if (other != node) {
      Resource_ResetEntry(other->slot);
      other->kind = 0;
    }
  }
  screen->head = node;
  node->prev = NULL;
  node->next = NULL;
  node->targetX = 4;
  asm("");
  asm("");
  for (other = screen->path, cnt = 0; other != NULL; other = other->next) {
    node->targetX += 16;
    cnt++;
  }
  screen->pathTop[cnt] = screen->top;
  screen->pathCursor[cnt] = screen->cursorIndex;
  node->dx = (node->targetX - node->x) >> 1;
  screen->unk39a = 0;
  screen->cursorIndex |= 0x80;
  WaitTicks(2);
  other = Resource_FindFreeTransferEntry(1);
  other->kind = node->kind;
  other->message = node->message;
  other->base = node->base;
  other->slot = node->slot;
  other->x = node->x;
  other->y = node->y;
  other->targetX = other->x;
  other->targetY = other->y;
  other->tile = node->tile;
  other->homeX = node->homeX;
  other->homeY = node->homeY;
  other->dx = 0;
  other->dy = 0;
  other->scale = 0x100;
  other->scaleEnd = 0x100;
  sprite = &other->sprite;
  sprite->mode = 0;
  sprite->colors = 0;
  sprite->mosaic = 0;
  sprite->size = 1;
  sprite->shape = 0;
  sprite->palette = 0;
  sprite->tile = other->tile;
  node->kind = 0;
  screen->head = NULL;
  if (screen->path != NULL) {
    node = screen->path;
    while (node->next != NULL)
      node = node->next;
    node->next = other;
    other->prev = node;
    other->next = NULL;
  } else {
    screen->path = other;
    other->prev = NULL;
    other->next = NULL;
  }
  return index;
}
