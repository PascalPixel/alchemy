#include "ram_buffer.h"
#include "types.h"
#include "map.h"
#include "io_reg.h"

s32 GetWorldMapCollision(struct WorldPosition *position) {
  s32 x_step = position->x >> 17;
  s32 y = position->y;
  s32 y_step = y >> 17;
  u32 cell;
  u32 pixel_offset;
  u8 tile;
  u8 packed_pixels;
  u32 result;

  cell = (((y_step / 8) & 63) << 6) + ((x_step / 8) & 63);
  pixel_offset = (((y_step / 2) & 3) << 1) + ((x_step / 4) & 1);

  tile = BG_SCREEN_BLOCK(10)[cell];
  packed_pixels = *(Ram_MapCollision + 0x800 + (tile << 3) + pixel_offset);
  if (packed_pixels != 0) {
    if (x_step & 2)
      result = packed_pixels >> 4;
    else
      result = packed_pixels & 15;
    if (result != 0)
      return result;
  }

  tile = BG_SCREEN_BLOCK(8)[cell];
  packed_pixels = *(Ram_MapCollision + (tile << 3) + pixel_offset);
  if (packed_pixels != 0) {
    if (x_step & 2)
      result = packed_pixels >> 4;
    else
      result = packed_pixels & 15;
    if (result != 0)
      return result;
  }

  return 7;
}

s32 Func_0802da88(struct WorldPosition *position);

s32 Func_0802db64(void *object, struct WorldPosition *position) {
  if (Func_0802da88(position) != 255) {
    if ((u32)(GetWorldMapCollision(position) - 5) <= 7)
      return 0;
  }
  return -1;
}

s32 Func_0802db88(void *object, struct WorldPosition *position) {
  if (Func_0802da88(position) != 255) {
    s32 collision = GetWorldMapCollision(position);
    u8 *work = Ram_HeapSlots->eventWork;
    s32 mode;

    /* FAKEMATCH: the native loads the event work before it builds the mode
     * offset; the plain read builds the offset first (one reordered pair,
     * differ 2). */
    asm volatile("");
    mode = *(s8 *)(work + 0xd3a);
    if (mode == 0) {
      if ((u32)(collision - 1) <= 3 || collision == 6)
        return 0;
    } else if (mode == 1) {
      if ((u32)(collision - 1) <= 3 || collision == 6)
        return 0;
    } else if (mode == 2) {
      if ((u32)(collision - 1) <= 3 || collision == 6)
        return 0;
    }
  }
  return -1;
}

struct WorldMover {
  u8 unk00[12];
  s32 height;
};

s32 Func_0802dbd0(struct WorldMover *mover, struct WorldPosition *position) {
  if (Func_0802da88(position) != 255) {
    s32 collision = GetWorldMapCollision(position);
    u8 *work = Ram_HeapSlots->eventWork;

    s32 height = mover->height;

    /* FAKEMATCH: the native loads the height before it builds the limit; the
     * plain test builds the limit first (one reordered pair, differ 2). */
    asm volatile("");
    if (height < 0x40000) {
      if ((u32)(collision - 1) > 3 && collision != 6)
        return -1;
      return 0;
    } else {
      s32 mode = *(s8 *)(work + 0xd3a);

      if (mode == 0) {
        if ((u32)(collision - 1) <= 5)
          return 0;
      } else if (mode == 1) {
        if ((u32)(collision - 1) <= 7)
          return 0;
      } else {
        if (mode == 2) {
          if ((u32)(collision - 1) <= 7)
            return 0;
        } else {
          if ((u32)(collision - 4) <= 4)
            return 0;
        }
        if (collision == 10)
          return 0;
      }
    }
  }
  return -1;
}
