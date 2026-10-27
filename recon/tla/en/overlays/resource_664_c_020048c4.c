// Draft, differ 34 of 219 (was 52): finds the paired object the leader stands
// on (the pair list gData_0200621c, three halfwords an entry, ended by 0xffff)
// and, when neither of the pair is busy, swaps them inside an event over eight
// frames as Func_020007b4 does a step at a time, the leader sinking with the
// first; then sets the first's flag, clears the second's and writes both cells
// again. Measured: the search is a goto loop, as the native rebuilds 0xffff at
// each test (a do-while hoists it into r9; 52 to 42); the busy bytes are read
// first's then second's (38); the step counter moves after the wait (34).
// Left: register choice only, the pair pointer and id swapped (r5 and r6; the
// pointer ranks above the id in global.c, 0.71 against 0.67) and the leader
// and busy swapped (r9 and r10). Tried: the declarations reordered, id as s32
// or u16, the first test on the table itself (51), 25 minutes of alchemy
// permute.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_sprite.h"
#include "graphics/vram_tab.h"
#include "system/dma.h"

extern u16 gData_0200621c[];
struct ObjectRuntime *Object_GetById(u32 id);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02005d5c(struct ObjectRuntime *object, s32 mode);
void Func_020006cc(struct ObjectRuntime *object);

void Func_020048c4(void) {
  struct ObjectRuntime *first = 0, *second = 0, *leader;
  struct FieldSprite *firstSprite, *secondSprite;
  struct VramBlockCacheEntry *entry;
  u16 *pair;
  u32 id, i;
  s32 busy;

  leader = Object_GetById(gPartyState.currentOwner);
  pair = gData_0200621c;
  if (*pair == 0xffff)
    goto found;
search:
  id = *pair;
  first = Object_GetById(id);
  if (first->x >> 20 == leader->x >> 20 && first->z >> 20 == leader->z >> 20) {
    second = Object_GetById(id - ((id & 1) << 1) + 1);
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
  WaitFrames(1);
  entry = &gResourceTableEntries[secondSprite->vramBlock];
  second->linkedObject = (void *)(entry->offset + 0x6010000);
  entry = &gResourceTableEntries[firstSprite->vramBlock];
  first->linkedObject = (void *)(entry->offset + 0x6010000);
  Dma_Fill32(busy, second->linkedObject, entry->size);
  Dma_Set(first->linkedObject, second->linkedObject, 0x840000c0,
          (volatile u32 *)0x040000d4);
  first->linkedObject = (void *)((u8 *)first->linkedObject + 0x300);
  second->linkedObject = (void *)((u8 *)second->linkedObject + 0x300);
  leader->motionFlags = busy;
  i = 0;
step:
  if (i <= 4) {
    second->y += 0x53333;
    Dma_Set(first->linkedObject, second->linkedObject, 0x84000030,
            (volatile u32 *)0x040000d4);
    first->linkedObject = (void *)((u8 *)first->linkedObject + 0x100);
    second->linkedObject = (void *)((u8 *)second->linkedObject + 0x100);
  }
  leader->y += -0x38000;
  leader->terrainHeight = leader->y;
  first->scaleY += -0x1f00;
  WaitFrames(1);
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
  FieldEvent_WaitFrames(10);
  Func_02005e1c();
}
