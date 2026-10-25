// Draft, differ 36 of 150: one step of two paired objects trading places (the
// pair the effect work's word 26 names, rounded down to even): on step 0
// the second rises 28 pixels, its sprite tiles are cleared and the first's
// copied over them; on steps 1 to 5 it sinks and 0x30 words more are copied;
// through step 7 the first flattens; in mode 1 both settle, the first's flag
// is set and the second's cleared, their scripts step and their cells are
// written again (Func_020006cc). Left: order and registers in step 0's
// table reads and transfers.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "graphics/vram_tab.h"
#include "system/dma.h"

struct ObjectRuntime *Object_GetById(u32 id);
void WaitFrames(s32 frames);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02005d5c(struct ObjectRuntime *object, s32 mode);
void Func_020006cc(struct ObjectRuntime *object);

void Func_020007b4(s32 mode, s32 step) {
  s16 *pair = (s16 *)((u8 *)Ram_HeapSlots->effectWork + 26);
  s32 id = *pair - ((*(u16 *)pair & 1) << 1);
  struct ObjectRuntime *first = Object_GetById(id);
  struct ObjectRuntime *second = Object_GetById(id + 1);
  struct FieldSprite *firstSprite, *secondSprite;
  struct VramBlockCacheEntry *entry;
  u32 *firstTiles, *secondTiles;
  s32 advance;

  if (step == 0) {
    firstSprite = first->animation;
    secondSprite = second->animation;
    second->motionFlags = 0;
    second->y += -0x1c0000;
    WaitFrames(1);
    entry = &gResourceTableEntries[secondSprite->vramBlock];
    second->linkedObject = (void *)(entry->offset + 0x6010000);
    entry = &gResourceTableEntries[firstSprite->vramBlock];
    first->linkedObject = (void *)(entry->offset + 0x6010000);
    Dma_Fill32(step, second->linkedObject, entry->size);
    Dma_Set(first->linkedObject, second->linkedObject, 0x840000c0,
            (volatile u32 *)0x040000d4);
    advance = 0x300;
  } else if (step <= 5) {
    second->y += 0x53333;
    Dma_Set(first->linkedObject, second->linkedObject, 0x84000030,
            (volatile u32 *)0x040000d4);
    advance = 0x100;
  } else {
    goto flatten;
  }
  first->linkedObject = (void *)((u8 *)first->linkedObject + advance);
  second->linkedObject = (void *)((u8 *)second->linkedObject + advance);
flatten:
  if (step <= 7)
    first->scaleY += -0x1f00;
  if (mode == 1) {
    first->scaleY = 0x10000;
    second->y = second->terrainHeight;
    GameFlag_SetBit(first->action);
    GameFlag_ClearBit(second->action);
    Func_02005d5c(first, ((struct FieldSprite *)first->animation)->frame + 1);
    Func_02005d5c(second, ((struct FieldSprite *)second->animation)->frame - 1);
    Func_020006cc(first);
    Func_020006cc(second);
  }
}
