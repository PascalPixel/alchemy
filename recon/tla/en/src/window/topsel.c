#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "heap_release.h"
#include "glyph.h"
#include "select.h"
#include "types.h"
#include "resource.h"
#include "system.h"
#include "ram_buffer.h"
#include "node_chain.h"

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(void *, void *))Resource_DecodeByteLzInRam)

extern char ResourceId_CommandIcons;

void Menu_LoadSelectedResource(void) {
  struct SelectionScreen *state = Ram_HeapSlots->menuWork;
  struct SelectionNode *selection = NodeChain_GetNodeAtCount(state);
  struct SelectionNode *transfer;
  GlyphTransfer *buffer;
  u8 *table;
  void *resource;
  s32 no;

  if (selection->kind != 1 && selection->kind != 6)
    return;

  buffer =
      (GlyphTransfer *)HeapSlot_Alloc(68, sizeof(GlyphTransfer));
  transfer = &state->records[15];
  no = selection->base;
  table = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
  {
    u8 **destination = &buffer->encoded;
    resource = table + *(u16 *)(table + selection->base * 2);
    *destination = resource;
  }
  Resource_DecodeByteLzInRam(resource, buffer);

  if (transfer->kind == 0)
    transfer->slot = Resource_FindFreeEntry();
  transfer->tile = VramCache_Load(transfer->slot, 0x400, buffer);
  transfer->kind = 1;
  transfer->base = no;
  transfer->scale = 40;
  transfer->scaleStep = 40;
  transfer->scaleEnd = 240;
  HeapSlot_Release(68);
}

void Resource_ResetPendingTransfer(void) {
  struct SelectionNode *transfer =
      &((struct SelectionScreen *)Ram_HeapSlots->menuWork)->records[15];

  if (transfer->kind != 0) {
    Resource_ResetEntry(transfer->slot);
    transfer->kind = 0;
  }
}
