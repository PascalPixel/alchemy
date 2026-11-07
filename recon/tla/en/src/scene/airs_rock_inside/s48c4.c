#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "graphics/vram_tab.h"
#include "system/dma.h"

extern u16 gData_0200621c[];
struct Actor *Actor_GetById(u32 id);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02005d5c(struct Actor *object, s32 mode);
void Func_020006cc(struct Actor *object);

void Func_020048c4(void) {
  struct Actor *first = 0, *second = 0, *leader;
  struct FieldSprite *firstSprite, *secondSprite;
  struct VramBlockCacheEntry *entry;
  u16 *pair;
  u32 id, i;
  s32 busy;

  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  pair = gData_0200621c;
  if (*pair == 0xffff)
    goto found;
search:
  id = *pair;
  first = Actor_GetById(id);
  if (first->x >> 20 == leader->x >> 20 && first->z >> 20 == leader->z >> 20) {
    second = Actor_GetById(id - ((id & 1) << 1) + 1);
    goto found;
  }
  pair += 3;
  if (*pair != 0xffff)
    goto search;
found:
  busy = ((u8 *)first)[0x63] | ((u8 *)second)[0x63];
  if (busy != 0)
    return;
  Func_02005e14();
  Func_02005f2c(0);
  firstSprite = first->animation;
  secondSprite = second->animation;
  second->motionFlags = busy;
  second->y += -0x1c0000;
  wait(1);
  entry = &gResourceTableEntries[secondSprite->vramBlock];
  second->watchedObject = (void *)(entry->offset + 0x6010000);
  entry = &gResourceTableEntries[firstSprite->vramBlock];
  first->watchedObject = (void *)(entry->offset + 0x6010000);
  Dma_Fill32(busy, second->watchedObject, entry->size);
  Dma_Set(first->watchedObject, second->watchedObject, 0x840000c0,
          (volatile u32 *)0x040000d4);
  first->watchedObject = (void *)((u8 *)first->watchedObject + 0x300);
  second->watchedObject = (void *)((u8 *)second->watchedObject + 0x300);
  leader->motionFlags = busy;
  i = 0;
step:
  if (i <= 4) {
    second->y += 0x53333;
    Dma_Set(first->watchedObject, second->watchedObject, 0x84000030,
            (volatile u32 *)0x040000d4);
    first->watchedObject = (void *)((u8 *)first->watchedObject + 0x100);
    second->watchedObject = (void *)((u8 *)second->watchedObject + 0x100);
  }
  leader->y += -0x38000;
  leader->terrainHeight = leader->y;
  first->scaleY += -0x1f00;
  wait(1);
  i++;
  if (i != 8)
    goto step;
  leader->motionFlags = 3;
  first->scaleY = 0x10000;
  second->y = second->terrainHeight;
  GameFlag_SetBit(first->action);
  GameFlag_ClearBit(second->action);
  Func_02005d5c(first, ((struct FieldSprite *)first->animation)->frame + 1);
  Func_02005d5c(second, ((struct FieldSprite *)second->animation)->frame - 1);
  Func_020006cc(first);
  Func_020006cc(second);
  FieldEvent_Wait(10);
  Func_02005e1c();
}
