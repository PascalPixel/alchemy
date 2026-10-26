// Draft, differ 179 (183 before statements reordered 2026-10-26).
// Near miss, differ 185 of 157 (2026-10-18), first pass: The Broken Seal's
// Map_CenterOnFollowedPoint, with this game's list of camera rectangles
// (s16 pixels, ended by -1, the pointer at map work 0xfc) that push the
// followed point out to the nearest edge. Left: the native keeps the
// followed pointer in a stack slot (sub sp, #4, stored after each read) and
// hoists -1 into r10; this keeps the pointer in a register. Its distance
// tests subtract the other way round where a difference is negative
// (subs r2, r1, r5), as ABS of an expression folds; written as ABS(left - x)
// throughout, the allocation drifts further (differ 220).
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

void Func_0802af9c(s32 x, s32 y);
void Func_0802ad84(void);

void Func_0802aa74(void) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  s32 *followedPosition = work->origin;
  s32 x = 0;
  s32 height = 0;
  s32 z = 0;

  if (followedPosition != NULL) {
    s16 *rect;

    x = *followedPosition++;
    height = *followedPosition++;
    rect = *(s16 **)((u8 *)work + 0xfc);
    z = *followedPosition++;
    if (rect != NULL) {
      while (rect[0] != -1) {
        s32 left = (rect[0] << 16) - 0x780000;
        s32 top = (rect[1] << 16) + height - 0x400000;
        s32 right = (rect[2] << 16) + 0x780000;
        s32 bottom = (rect[3] << 16) + height + 0x600000;

        if (x >= left && z >= top && x <= right && z <= bottom) {
          s32 nearX = left;
          s32 dx = left - x;
          s32 nearZ;
          s32 dz;

          if (ABS(dx) > ABS(right - x)) {
            nearX = right;
            dx = right - x;
          }
          nearZ = top;
          dz = top - z;
          if (ABS(dz) > ABS(bottom - z)) {
            nearZ = bottom;
            dz = bottom - z;
          }
          if (ABS(dx) <= ABS(dz))
            x = nearX;
          else
            z = nearZ;
          break;
        }
        rect += 4;
      }
      followedPosition[-3] = x;
      followedPosition[-1] = z;
    }
  }
  Func_0802af9c(x, z - height);
  Func_0802ad84();
}
