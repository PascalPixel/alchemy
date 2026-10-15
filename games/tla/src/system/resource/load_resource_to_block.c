#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "resource.h"
#include "types.h"

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(void *, void *))Resource_DecodeByteLzInRam)

void VramBlock_LoadResource(s32 entryId, s32 size, s32 resourceId) {
  void *source;
  void *buffer;

  if (entryId > 95)
    return;
  source = Resource_GetTableEntry(resourceId);
  buffer = MemAlloc(size);
  Resource_DecodeByteLzInRam(source, buffer);
  VramBlock_LoadCached(entryId, size, buffer);
  MemFree(buffer);
}
