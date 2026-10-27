// Draft, differ 172 of 181: an object vanishes in a burst of light: it spins
// for sixteen frames unless told not to, then hides while a sprite (resource
// 0x1e8, palette 15) plays its eight-frame flash over it for seventy-two
// frames, twice as large in mode 1; the object is then moved away and the
// sprite freed.
#include "types.h"
#include "object_runtime.h"
#include "system/dma.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern const u16 Field_Palette685[];

u8 *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
struct ObjectRuntime *Object_GetById(s32 id);
void WaitTicks(s32 ticks);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void *Resource_GetTableEntry(s32 resourceId);
void Func_020033dc(void *entry, u8 *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Func_020035fc(u8 *sprite, s32 width, s32 height, s32 attributes, s32 tile);
void Func_020035c4(s32 *position);
void Func_0200362c(s32 cue);
void Func_02003604(u8 *sprite);
void Func_020034d4(s32 id, s32 x, s32 z);
s32 Resource_ResetEntry(u32 entry);
void Runtime_ReleaseHeapBlock(s32 slot);

struct FlashSprite {
  u8 unk00[5];
  u8 unk05;
  u8 unk06[2];
  u16 tile : 10;
  u16 unk08_10 : 6;
};

void Func_020029f8(s32 id, s32 mode) {
  u8 *work = Runtime_AllocateHeapBlockFar(220, 0x428);
  struct ObjectRuntime *object = Object_GetById(id);
  s32 slot;
  s32 tile;
  u8 *sprite;
  s32 position[3];
  s32 i;

  if (mode == 0) {
    for (i = 15; i >= 0; i--) {
      object->angle += 0x2000;
      WaitTicks(1);
    }
  }
  WaitTicks(2);
  object->scaleX = 0;
  ObjectDispatch_SetSingleChildFlags(object, 0);
  Dma_Set(Field_Palette685, (void *)0x050003e0, 0x80000010,
          (volatile u32 *)0x040000d4);
  Func_020033dc(Resource_GetTableEntry(0x1e8), work);
  slot = Resource_FindFreeEntry();
  tile = VramCache_Load(slot, 0x400, work);
  sprite = work + 0x400;
  Func_020035fc(sprite, 8, 16, 0x40000000, tile);
  FIELD_AT(sprite, u16, 30) = 240;
  sprite[5] &= ~32;
  sprite[9] = (sprite[9] & ~12) | 4 | 240;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  Func_020035c4(position);
  FIELD_AT(sprite, s32, 12) = position[0];
  FIELD_AT(sprite, s32, 16) = position[2];
  if (mode == 1) {
    FIELD_AT(sprite, s32, 20) = 0x20000;
    FIELD_AT(sprite, s32, 24) = 0x20000;
    FIELD_AT(sprite, s32, 16) = position[2] + 0xa0000;
  }
  Func_0200362c(145);
  for (i = 0; i <= 71; i++) {
    sprite = work + 0x400;
    switch (i) {
    case 6:
      ((struct FlashSprite *)sprite)->tile = tile + 4;
      break;
    case 12:
      ((struct FlashSprite *)sprite)->tile = tile + 8;
      break;
    case 22:
      ((struct FlashSprite *)sprite)->tile = tile + 12;
      break;
    case 32:
      ((struct FlashSprite *)sprite)->tile = tile + 16;
      break;
    case 42:
      ((struct FlashSprite *)sprite)->tile = tile + 20;
      break;
    case 52:
      ((struct FlashSprite *)sprite)->tile = tile + 24;
      break;
    case 62:
      ((struct FlashSprite *)sprite)->tile = tile + 28;
      break;
    }
    Func_02003604(sprite);
    WaitTicks(1);
  }
  Func_020034d4(id, 0, 0);
  Resource_ResetEntry(slot);
  Runtime_ReleaseHeapBlock(220);
}
