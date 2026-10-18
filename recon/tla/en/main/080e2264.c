// Near miss, differ 254 of 274: the native keeps the event work in r8 and
// its frame is 8 bytes smaller (44); it builds 0xffff and -1 for the four
// marks with split constants where this loads them from the pool.
// Looks at 21 map cells around the effect owner, at the tile offsets
// of Data_080f0f64 (signed x, z byte pairs), on the owner's layer. Each cell
// of kind 1, 2 or 3 that Func_080cda84 accepts for the effect's id is listed
// as a tile in the event work's point list at 0xcc0, which ends with -1, -1;
// a kind-1 cell also stores its kind in the heap slot 0x5c work's byte
// 0x1b34, and the last kind-2 and kind-3 cells are kept in pixels, centred
// on their tile, at 0xd3c and 0xd40.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"

extern const s8 Data_080f0f64[];

s32 Func_080dbda8(s32 x, s32 z, s32 layer);
s32 Func_080cda84(s32 id, struct ObjectRuntime *object, s32 *position);

void Func_080e2264(void) {
  u8 *effect = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  s16 *points = (s16 *)(event + 0xcc0);
  struct ObjectRuntime *owner = *(struct ObjectRuntime **)(effect + 16);
  s32 count = 0;
  s16 *point;
  s32 position[3];
  s32 x, z;
  s32 kind;
  u32 i;

  work[0x1b34] = count;
  *(u16 *)(event + 0xd3c) = 0xffff;
  *(s16 *)(event + 0xd3e) = -1;
  *(s16 *)(event + 0xd40) = -1;
  *(s16 *)(event + 0xd42) = -1;
  point = points;
  for (i = 0; i <= 20; i++) {
    x = owner->x + (Data_080f0f64[i * 2] << 20);
    z = owner->z + (Data_080f0f64[i * 2 + 1] << 20);
    kind = Func_080dbda8(x, z, owner->terrainId);
    position[0] = x;
    position[2] = z;
    if (kind == 1 &&
        Func_080cda84(*(s16 *)(effect + 30), owner, position) != -1) {
      point[0] = x / 0x100000;
      point[1] = z / 0x100000;
      count++;
      work[0x1b34] = kind;
      point += 2;
    }
    if (kind == 2 &&
        Func_080cda84(*(s16 *)(effect + 30), owner, position) != -1) {
      *(s16 *)(event + 0xd3c) = x >> 16;
      *(s16 *)(event + 0xd3e) = z >> 16;
      *(s16 *)(event + 0xd3c) = (*(s16 *)(event + 0xd3c) & 0xfff0) + 8;
      *(s16 *)(event + 0xd3e) = (*(s16 *)(event + 0xd3e) & 0xfff0) + 8;
      point[0] = x / 0x100000;
      point[1] = z / 0x100000;
      point += 2;
      count++;
    }
    if (kind == 3 &&
        Func_080cda84(*(s16 *)(effect + 30), owner, position) != -1) {
      *(s16 *)(event + 0xd40) = x >> 16;
      *(s16 *)(event + 0xd42) = z >> 16;
      *(s16 *)(event + 0xd40) = (*(s16 *)(event + 0xd40) & 0xfff0) + 8;
      *(s16 *)(event + 0xd42) = (*(s16 *)(event + 0xd42) & 0xfff0) + 8;
      point[0] = x / 0x100000;
      point[1] = z / 0x100000;
      point += 2;
      count++;
    }
  }
  points[count * 2] = -1;
  points[count * 2 + 1] = -1;
}
