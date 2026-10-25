// Draft, differ 65 of 78: loads resource 0x1d9's picture into the menu work,
// moves its colour-15 pixels to palette 6, uploads it to a free video slot
// and sets up its sprite.
#include "types.h"
#include "ram_buffer.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void *Resource_GetTableEntry(s32 resourceId);
void Func_020033dc(void *entry, u8 *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Func_020035fc(u8 *sprite, s32 width, s32 height, s32 attributes, s32 tile);

struct PictureSprite {
  u8 unk00[5];
  u8 unk05;
  u8 unk06[3];
  u8 unused09 : 2;
  u8 layer : 2;
};

void Func_02002c88(void) {
  u8 *work = Ram_HeapSlots->menuRuntime;
  u8 *pixel;
  s32 i;
  s32 slot;
  s32 tile;
  u8 *sprite;

  Func_020033dc(Resource_GetTableEntry(0x1d9), work);
  pixel = work;
  for (i = 0; i < 0x1000; i++, pixel++) {
    if ((*pixel & 0xf0) == 0xf0)
      *pixel = (*pixel & 15) + 96;
  }
  slot = Resource_FindFreeEntry();
  tile = VramCache_Load(slot, 0x1000, work);
  FIELD_AT(work, u16, 4188) = slot;
  FIELD_AT(work, u16, 4190) = tile;
  sprite = work + 4108;
  Func_020035fc(sprite, 31, 31, 0xc0000000, tile);
  sprite[5] |= 32;
  ((struct PictureSprite *)sprite)->layer = 2;
  FIELD_AT(sprite, u16, 30) = 1;
  FIELD_AT(work, s32, 4196) = 0;
  FIELD_AT(work, s32, 4200) = 0;
}
