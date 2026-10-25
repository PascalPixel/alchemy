// Draft, differ 208 of 161: adds a water spout on every map cell of the given
// layer whose spout kind lies in [first, first + 8), at the given heights,
// and marks those cells as spout cells (120).
#include "types.h"
#include "ram_buffer.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

struct WaterSpout {
  s32 x;
  s32 y;
  s32 z;
  s32 baseY;
  u16 frame;
  u16 active;
  s32 *anchor;
  u32 top[3];
  u32 column[3];
};

struct WaterSpouts {
  u16 resource;
  u16 count;
  struct WaterSpout spouts[32];
};

extern struct WaterSpouts gData_0200536c;
extern u8 Data_0202c001[];

void Func_020032d4(u32 first, s32 level, s32 baseLevel) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *cells = FIELD_AT(map + 264, u8 *, 48);
  s32 originX = FIELD_AT(map + 264, s32, 8) + FIELD_AT(map, s32, 236);
  s32 originZ = FIELD_AT(map + 264, s32, 12) + FIELD_AT(map, s32, 240);
  s32 width = (FIELD_AT(map, s32, 244) - FIELD_AT(map, s32, 236)) >> 20;
  s32 depth = (FIELD_AT(map, s32, 248) - FIELD_AT(map, s32, 240)) >> 20;
  s32 corner =
      ((FIELD_AT(map, s32, 240) >> 20) << 7) + (FIELD_AT(map, s32, 236) >> 20);
  struct WaterSpout *spout = &gData_0200536c.spouts[gData_0200536c.count];
  u16 row;

  *(s32 *)(Data_0202c001 + 0x1df) = level << 8;
  cells += corner << 2;
  for (row = 0; row < depth; row++) {
    u8 *cell = cells + (row << 9);
    u16 column;

    for (column = 0; column < width; column++, cell += 4) {
      u32 kind = cell[2];

      if (kind != 0 && kind >= first && kind < first + 8) {
        spout->x = (column << 20) + originX + 0x80000;
        spout->z = (row << 20) + originZ + 0x80000;
        spout->baseY = baseLevel << 19;
        spout->y = level << 19;
        spout->frame = kind - first;
        spout->anchor = 0;
        spout->active = 1;
        spout++;
        gData_0200536c.count++;
        FIELD_AT(map, u8 *, 316)[corner + column + (row << 7)] = 120;
      }
    }
  }
}
