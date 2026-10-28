#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "graphics/vram_tab.h"
#include "system/dma.h"

struct Actor *Actor_GetById(u32 id);
void wait(s32 frames);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02005d5c(struct Actor *object, s32 mode);
void Func_020006cc(struct Actor *object);

/// One step of two paired objects trading places: the effect work's halfword
/// at 26 names the first, and its partner is the other id of its even and odd
/// pair. On step 0 the partner rises 28 pixels, its sprite tiles are cleared
/// and the first's copied over them; on steps 1 to 5 it sinks and 0x30 words
/// more are copied; through step 7 the first flattens. In mode 1 both settle,
/// the first's flag is set and the partner's cleared, their animations step
/// and their cells are written again (Func_020006cc).
void Func_020007b4(s32 mode, s32 step) {
  s16 *pair = (s16 *)((u8 *)Ram_HeapSlots->effectWork + 26);
  s32 even = *pair - ((*(u16 *)pair & 1) << 1);
  struct Actor *first = Actor_GetById(*pair);
  struct Actor *second = Actor_GetById(even + 1);
  struct FieldSprite *firstSprite, *secondSprite;
  struct VramBlockCacheEntry *entry;

  if (step == 0) {
    firstSprite = first->animation;
    secondSprite = second->animation;
    second->motionFlags = 0;
    second->y += -0x1c0000;
    wait(1);
    entry = &gResourceTableEntries[secondSprite->vramBlock];
    second->watchedObject = (void *)(entry->offset + 0x6010000);
    entry = &gResourceTableEntries[firstSprite->vramBlock];
    first->watchedObject = (void *)(entry->offset + 0x6010000);
    Dma_Fill32(step, second->watchedObject, entry->size);
    Dma_Set(first->watchedObject, second->watchedObject, 0x840000c0,
            (volatile u32 *)0x040000d4);
    first->watchedObject = (void *)((u8 *)first->watchedObject + 0x300);
    second->watchedObject = (void *)((u8 *)second->watchedObject + 0x300);
  } else if (step <= 5) {
    second->y += 0x53333;
    Dma_Set(first->watchedObject, second->watchedObject, 0x84000030,
            (volatile u32 *)0x040000d4);
    first->watchedObject = (void *)((u8 *)first->watchedObject + 0x100);
    second->watchedObject = (void *)((u8 *)second->watchedObject + 0x100);
  }
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
