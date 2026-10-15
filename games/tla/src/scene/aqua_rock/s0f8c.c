#include "local.h"
#include "map.h"

struct AquaRockEventWork {
  u8 unk000[0x16a];
  s16 field16a;
};

void Func_02001e00(void);

static inline u8 CellAttribute(struct MapCell *cells, s32 row, s32 col) {
  cells += row * 128 + col;
  return cells->attributeB;
}

void Func_02000f8c(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct AquaRockEventWork *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
  s32 x = object->x;
  s32 z = object->z;
  s32 y = object->y;
  s32 col;
  s32 row;

  if (gPartyState.renderMode == 2) {
    col = x >> 20;
    row = (z - y - 0xc0000) >> 20;
    if (CellAttribute(map->layers[1].cells, row, col) != 0)
      work->field16a = 44;
  } else {
    col = x >> 20;
    row = (z - y) >> 20;
    if (CellAttribute(map->layers[1].cells, row, col) != 0)
      Func_02001e00();
  }
}
