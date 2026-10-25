// Draft, differ 254 of 184: draws Aqua Rock's thirty-two water spouts: for each
// active one a top sprite and, when it stands above its base, a column sprite
// as tall as its height, placed from the camera and queued by depth.
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
  u16 active;  ///< 0 free, 2 risen
  s32 *anchor; ///< a position it follows, or null
  u32 top[3];
  u32 column[3];
};

struct WaterSpouts {
  u16 resource;
  u16 count;
  struct WaterSpout spouts[32];
};

extern struct WaterSpouts gData_0200536c;
extern u8 gResourceTableEntries[];

void Func_02003c94(u32 *sprite, s32 depth);

void Func_020030f4(void) {
  s32 viewX = FIELD_AT(Ram_HeapSlots->mapWork, s32, 228) & 0xffff0000;
  s32 viewZ = FIELD_AT(Ram_HeapSlots->mapWork, s32, 232) & 0xffff0000;
  s32 originY = FIELD_AT(FIELD_AT(Ram_HeapSlots->mapWork, s32 *, 0), s32, 4);
  struct WaterSpout *spout = gData_0200536c.spouts;
  u32 tile = FIELD_AT(gResourceTableEntries + (gData_0200536c.resource << 2),
                      u16, 2) >>
             5;
  s32 i = 31;

loop:
  if (spout->active != 0) {
    s32 baseY = spout->baseY;
    s32 y = spout->y;
    s32 screenX;
    s32 screenZ;
    s32 top;
    s32 rise;
    s32 attributes;
    s32 depth;

    if (spout->anchor != 0) {
      screenX = spout->anchor[0] - viewX;
      screenZ = spout->anchor[2] - viewZ - originY;
      top = spout->anchor[1] + y - originY;
    } else {
      screenX = spout->x - viewX;
      screenZ = spout->z - viewZ - originY;
      top = y - originY;
    }
    rise = screenZ - top;
    if (spout->y != baseY || spout->active == 2)
      attributes = 0x800;
    else
      attributes = 0xc00;
    depth = ((top + screenZ) >> 16) + 50;
    screenX = (screenX >> 16) - 8;
    rise = (rise >> 16) - 8;
    if ((u32)(screenX + 7) <= 270) {
      s32 column;

      if ((u32)(rise + 39) <= 238) {
        screenX &= 511;
        spout->top[0] = 0;
        spout->top[1] = (rise & 255) | (screenX << 16) | 0x40002000;
        spout->top[2] = (tile + (spout->frame << 3)) | attributes;
        Func_02003c94(spout->top, depth);
      }
      column = ((screenZ - top) >> 16) + 8;
      if ((u32)(column + 55) <= 238) {
        s32 height = (spout->y - spout->baseY) >> 16;

        if (height != 0) {
          height--;
          if ((u32)(height - 1) > 7)
            height = 7;
          spout->column[0] = 0;
          spout->column[1] = (column & 255) | (screenX << 16) | 0x6000;
          spout->column[2] = attributes | (tile + (height << 2) + 64);
          Func_02003c94(spout->column, depth);
        }
      }
    }
  }
  spout++;
  if (--i >= 0)
    goto loop;
}
