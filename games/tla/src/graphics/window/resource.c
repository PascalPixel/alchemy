#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "resource.h"
#include "render_input.h"
#include "mem.h"

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(void *, void *))Resource_DecodeByteLzInRam)

extern char ResourceId_CommandIcons;

void RenderResource_LoadCommandIcon(s32 index, s32 value, s32 flag) {
  s32 size = 1024;
  void *buffer = Runtime_AllocateBlock(56, size);
  u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

  if (value <= 95) {
    Resource_DecodeByteLzInRam((void *)((u32)base + base[index]), buffer);
    if (flag != 0)
      Runtime_RemapBytesByTableFar(buffer, 768);
    VramCache_Load(value, size, buffer);
    Runtime_ReleaseHeapBlock(56);
  }
}

struct WindowSprite *RenderResource_CreateCommandIcon(s32 arg0, s32 arg1,
                                                struct RenderInput *arg2,
                                                s32 arg3, s32 arg4) {
  s32 index;
  struct WindowSprite *entity;

  index = Resource_FindFreeEntry();
  entity = NULL;
  if (index != 0x60) {
    RenderResource_LoadCommandIcon(arg0, index, arg1);
    entity = WindowSprite_Create(index, 0x80000000, arg2, arg3, arg4);
    entity->oam.palette256 = 1;
    entity->sentinel = 0xfb;
  }
  return entity;
}
