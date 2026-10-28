#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "glyph.h"
#include "resource.h"
#include "dma.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "select.h"

extern u8 Tile_Decompress4bppCode[];
extern u8 Tile_ExpandMaskedCode[];
extern u8 Tile_ExpandOpaqueCode[];
extern u8 Tile_Decompress4bppCodeSize[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern u8 Tile_ExpandOpaqueCodeSize[];

void Resource_ClearOwnerListAndCounters(void) {
  struct SelectionScreen *screen;

  screen = Ram_HeapSlots->menuWork;
  screen->head = NULL;
  screen->unk39a = 0;
  if (screen->cursorIndex & 0x80) {
    screen->top = 0;
    screen->cursorIndex = 0;
  }
  screen->verticalScroll = 0;
  screen->count = 0;
}

void Resource_PushPendingPair(u32 first, u32 second) {
  struct SelectionScreen *state = Ram_HeapSlots->menuWork;
  u16 cnt = state->count;

  if (cnt != 16) {
    state->kinds[cnt] = first;
    state->bases[cnt] = second;
    state->count++;
  }
}

struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind);
void MenuSelection_SetupEntry(u32 kind, s32 base, struct SelectionNode *node,
                              s32 unused);
void Menu_LoadSelectedResource(void);

/// Creates icons for the first five listed entries in a row along the bottom of
/// the screen, then loads the selected entry's preview. Reached through the
/// far-call table and from Menu_RunSelectionForValue.
// As ☀️'s SelectionScreen_BuildEntries.
void SelectionScreen_BuildEntries(u32 unused) {
  struct SelectionScreen *screen = Ram_HeapSlots->menuWork;
  u32 count = screen->count;
  u32 index = screen->top;
  struct SelectionNode *previous = 0;
  struct SelectionNode *node;
  s32 rowPosition = 0;

  while (index < count) {
    s32 base = screen->bases[index];
    u32 kind = screen->kinds[index];

    node = Resource_FindFreeTransferEntry(0);
    if (node == 0)
      break;
    MenuSelection_SetupEntry(kind, base, node, 0);
    if (screen->head == 0) {
      screen->head = node;
      node->prev = 0;
    } else {
      previous->next = node;
      node->prev = previous;
    }
    node->next = 0;
    rowPosition++;
    previous = node;
    if (rowPosition == 5)
      break;
    index++;
  }

  // Icons are 16 pixels apart: the row is centred on x 100, at y 140.
  {
    s32 offset = rowPosition * 8;
    /* FAKEMATCH(pin): the 100 a halfword pinned to r3 behind a barrier, so it
     * loads from the pool before the offset of baseX is built; as
     * screen->baseX = 100 - rowPosition * 8, 2 of 130 instructions differ in
     * tla-en. */
    register u16 left asm("r3") = 100;

    asm volatile("" : "+r"(left));
    screen->baseX = left - offset;
  }
  screen->baseY = 140;
  // The count is reused as a pixel offset while the row is placed.
  for (previous = screen->head, rowPosition = 0; previous != 0;
       previous = previous->next) {
    s32 x = screen->baseX + rowPosition;
    s32 y;

    previous->x = x;
    y = screen->baseY;
    previous->y = y;
    previous->targetX = x;
    previous->targetY = y;
    // While locked is 0 a yes or no row sits at the top of the screen.
    if (previous->kind == 6 && screen->locked == 0) {
      previous->y = 6;
      previous->targetY = 6;
    }
    previous->dx = 0;
    previous->dy = 0;
    rowPosition += 16;
  }
  Menu_LoadSelectedResource();
}
