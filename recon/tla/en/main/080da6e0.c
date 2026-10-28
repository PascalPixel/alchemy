// First draft of Func_080da6e0, differ 221 of 259 (2026-10-18): stretches a
// WORK_A0 slot's chain of nodes from (x1, y1, z1) towards (x2, y2, z2), one
// node per 6 units of distance, unrolling it as a spiral over as many frames.
// Same calls and branches as the listing; left: the native keeps the slot in
// r10 and the tail's zero in r9 from the top (a 44-byte stack frame), where this
// spills the slot (48), and every stack slot after that moves.
#include "types.h"
#include "worka0.h"
#include "iwram_call.h"
#include "fixed_math.h"

void *MemAlloc(s32 size);
void MemFree(void *block);
void Object_SetMode(void *object, s32 mode);
void WaitFrames(s32 frames);
s32 ArcTan2(s32 y, s32 x);

s32 Func_080da6e0(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2,
                  s32 wave) {
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Node *node;
  struct WorkA0Node **nodes;
  s32 dx, dy, dz;
  s32 dist;
  s32 count;
  s32 frame;
  s32 i;
  s32 angle;
  s32 radius;
  s32 x, y, z;

  dx = (x2 - x1) >> 8;
  dy = (y2 - y1) >> 8;
  dz = (z2 - z1) >> 8;
  dist = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                    Iwram_MulQ16(dz, dz));
  count = dist / 6 + 1;
  node = work->slots[slot].head;
  nodes = MemAlloc(0x200);
  work->slots[slot].tail = NULL;
  for (i = 0; i <= count; i++) {
    nodes[i] = node;
    node = node->next;
  }
  Object_SetMode(((void **)work)[0], 3);
  Object_SetMode(((void **)work)[1], 3);
  WaitFrames(4);
  for (frame = 0; frame <= count; frame++) {
    angle = frame << 12;
    radius = 0x40000;
    for (i = 0; i <= count; i++) {
      node = nodes[i];
      node->unk11[0] = 0;
      if (i <= frame) {
        x = x1 + Iwram_MulQ16(Trig_Cos(angle), radius);
        node->x = x;
        y = y1;
        node->y = y;
        z = z1 + Iwram_MulQ16(Trig_Sin(angle), radius);
        node->z = z;
        node->used = 2;
        {
          u16 step = ArcTan2(0x60000, radius);

          angle += step;
          radius += step * 4;
        }
      } else {
        dx = (x - node->x) >> 8;
        dy = (y - node->y) >> 8;
        dz = (z - node->z) >> 8;
        dist = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                          Iwram_MulQ16(dz, dz));
        if (dist <= 6)
          break;
        x = node->x = x + (node->x - x) * 6 / dist;
        y = node->y = y + (node->y - y) * 6 / dist;
        z = node->z = z + (node->z - z) * 6 / dist;
        node->used = 2;
      }
    }
    WaitFrames(1);
  }
  MemFree(nodes);
  return 0;
}
