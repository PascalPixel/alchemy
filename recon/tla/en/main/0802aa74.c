// Draft, differ 220 of 157: keeps the camera focus out of the map's
// bounding rectangles (s16 cells, -1 ends the list, each widened by 120
// pixels and shifted by the focus height): inside one, x or z moves to the
// nearer edge, then the camera is placed from x and z - y. No ☀️ twin
// found. The same shape, distances as b - a when a - b is negative as the
// native computes them (with an ABS of the difference, negs: differ 202,
// fewer only because the shape is further off). Left is register choice:
// the native keeps x, y, z in lr, r8 and ip and the focus pointer in a
// stack slot it rewrites after each load, which cascades through the loop.
#include "types.h"
#include "ram_buffer.h"

#define DISTANCE(a, b) ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))

struct CameraBound {
  s16 left;
  s16 top;
  s16 right;
  s16 bottom;
};

struct CameraMapWork {
  s32 *focus;
  u8 unk04[248];
  struct CameraBound *bounds;
};

void Func_0802af9c(s32 x, s32 y);
void Func_0802ad84(void);

void Func_0802aa74(void) {
  struct CameraMapWork *map = (struct CameraMapWork *)Ram_HeapSlots->mapWork;
  s32 *focus = map->focus;
  struct CameraBound *bound;
  s32 x = 0;
  s32 y = 0;
  s32 z = 0;

  if (focus != 0) {
    x = *focus++;
    y = *focus++;
    z = *focus++;
    bound = map->bounds;
    if (bound != 0) {
      for (; bound->left != -1; bound++) {
        s32 left = (bound->left << 16) - 0x780000;
        s32 top = (bound->top << 16) + y - 0x400000;
        s32 right = (bound->right << 16) + 0x780000;
        s32 bottom = (bound->bottom << 16) + y + 0x600000;

        if (x >= left && z >= top && x <= right && z <= bottom) {
          s32 edgeX = left;
          s32 edgeZ = top;

          if (DISTANCE(left, x) > DISTANCE(right, x))
            edgeX = right;
          if (DISTANCE(top, z) > DISTANCE(bottom, z))
            edgeZ = bottom;
          if (DISTANCE(edgeX, x) <= DISTANCE(edgeZ, z))
            x = edgeX;
          else
            z = edgeZ;
          break;
        }
      }
      focus[-3] = x;
      focus[-1] = z;
    }
  }
  Func_0802af9c(x, z - y);
  Func_0802ad84();
}
