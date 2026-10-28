#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "edition.h"

extern u8 gMapCellBuffer[];
extern u8 gMapShapeGrid[];

s32 Func_0802d45c(s32 layer, s32 x, s32 z);
s32 Func_080c8730(s32 layer, s32 unused, s32 value);
s32 Func_080c8720(s32 layer, s32 unused, s32 value);

s32 Func_0802d4d8(s32 layer, s32 x, s32 z) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  u8 *grid;

  x >>= 16;
  z >>= 16;
  if (work != NULL)
    grid = work->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  return Ram_MapCollision[grid[x / 16 + ((z / 16) << 7)] * 4] & 15;
}

s32 Func_0802d530(s32 layer, s32 x, s32 z) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  u8 *grid;
  u8 *code;
  s32 kind;

  x >>= 16;
  z >>= 16;
  if (work != NULL)
    grid = work->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  code = &Ram_MapCollision[grid[x / 16 + ((z / 16) << 7)] * 4];
  kind = code[0];
  if (kind == 7 && code[1] != code[2])
    kind = 99;
  return kind;
}

s32 Func_0802d594(s32 layer, s32 x, s32 z, s32 reach) {
  s32 best;
  s32 height;

  best = Func_0802d45c(layer, x, z);
  height = Func_0802d45c(layer, x - reach, z);
  if (best < height)
    best = height;
  height = Func_0802d45c(layer, x + reach, z);
  if (best < height)
    best = height;
  height = Func_0802d45c(layer, x, z - reach);
  if (best < height)
    best = height;
  height = Func_0802d45c(layer, x, z + reach);
  if (best < height)
    best = height;
  return best;
}

void Func_0802d600(s32 x, s32 z, s32 layer, s32 value) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  u8 *shape;

  shape = gMapShapeGrid +
          (work->layers[layer & 3].cells - (struct MapCell *)Ram_MapCellBuffer);
  shape += (x >> 20) + ((z >> 20) << 7);
  *shape = Func_080c8730(layer, 0, value);
}

s32 Func_0802d644(s32 a, s32 b, s32 c) { return Func_0802d45c(c, a, b) >> 19; }

s32 Func_0802d658(s32 layer, s32 x, s32 z) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  u8 *grid;

  x >>= 16;
  z >>= 16;
  if (work != NULL)
    grid = work->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  return Ram_MapCollision[grid[x / 16 + ((z / 16) << 7)] * 4] & 15;
}

u8 GetMapCellCollision(s32 layer, s32 x, s32 y) {
  struct MapWork *state;
  struct MapCell *cells;

  state = Ram_HeapSlots->mapWork;
  x >>= 20;
  y >>= 20;
  cells = (struct MapCell *)gMapCellBuffer;
  if (state != NULL) {
    cells = state->layers[layer & 3].cells;
  }
  cells += x + (y << 7);
  return cells->collisionCode;
}

void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collisionCode) {
  struct MapWork *state = Ram_HeapSlots->mapWork;

  x >>= 20;
  y >>= 20;
  if (state != NULL) {
    struct MapCell *cells = state->layers[layer & 3].cells;

    cells += x + (y << 7);
    cells->collisionCode = collisionCode;
  }
}
