#include "garoh.h"

struct MapLayer {
  u8 unk00[0x10];
  s32 unk10;
  s32 unk14;
  u8 unk18[0x10];
  u16 unk28;
  u16 unk2a;
  u8 unk2c[0x0c];
};

struct MapWork {
  u8 unk000[0x108];
  struct MapLayer layers[3];
};

void Func_02000280(void) {
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapWork;
  struct MapLayer *to = &map->layers[1];
  struct MapLayer *from = &map->layers[0];

  to->unk10 = from->unk10;
  to->unk14 = from->unk14;
  to->unk28 = from->unk28;
  to->unk2a = from->unk2a;
}
