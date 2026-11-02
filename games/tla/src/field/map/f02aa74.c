#include "types.h"
#include "ram_buffer.h"
#include "map.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

void Func_0802af9c(s32 x, s32 y);
void Func_0802ad84(void);

void Func_0802aa74(void) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  s32 x = 0;
  s32 height = 0;
  s32 z = 0;
  s32 *followedPosition = work->origin;

  if (followedPosition != NULL) {
    s16 *rectList;
    s16 *rect;

    x = *followedPosition++;
    height = *followedPosition++;
    z = *followedPosition++;
    rectList = *(s16 **)((u8 *)work + 0xfc);
    if (rectList != NULL) {
      for (rect = rectList; rect[0] != -1; rect += 4) {
        s32 left = (rect[0] << 16) - 0x780000;
        s32 top = (rect[1] << 16) + height - 0x400000;
        s32 right = (rect[2] << 16) + 0x780000;
        s32 bottom = (rect[3] << 16) + height + 0x600000;

        if (x >= left && z >= top && x <= right && z <= bottom) {
          s32 nearX = left;
          s32 nearZ;

          if (ABS(nearX - x) > ABS(right - x))
            nearX = right;
          nearZ = top;
          if (ABS(nearZ - z) > ABS(bottom - z))
            nearZ = bottom;
          if (ABS(nearX - x) <= ABS(nearZ - z))
            x = nearX;
          else
            z = nearZ;
          break;
        }
      }
      followedPosition[-3] = x;
      followedPosition[-1] = z;
    }
  }
  Func_0802af9c(x, z - height);
  Func_0802ad84();
}
