// Draft, differ 286 (363 before statements reordered 2026-10-26) of 319 (2026-10-25): the heap slot 0xa0 work's twin of
// Func_080d9498, which Func_080da334 calls: stretches slot `index`'s chain
// between (x, y, z) and the target point for twice its segment count of frames,
// one segment per 6 units of the distance plus one. The last node rides the
// line between the ends, raised by a sine wave across the line's heading; each
// node before it is pulled to within 6 units of the next, and the frame ends at
// the first node already that close. At the end it starts the slot's wobble.
// Left: register and frame allocation (a 92-byte frame for the native's 88; the
// native keeps the shifted deltas in r9, r11 and r10 and counts the copy
// loop down from segments + 1).
#include "worka0.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "mem.h"

s32 ArcTan2(s32 y, s32 x);
void WaitTicks(s32 ticks);

s32 Func_080da3bc(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct WorkA0 *work = WORK_A0;
  s32 dz = toZ - z;
  s32 shiftedZ = dz >> 8;
  s32 dx = toX - x;
  s32 dy = toY - y;
  s32 shiftedX = dx >> 8;
  s32 shiftedY = dy >> 8;
  s32 frame;
  s32 segments = Iwram_SqrtEntry(Iwram_MulQ16(shiftedX, shiftedX) +
                                 Iwram_MulQ16(shiftedY, shiftedY) +
                                 Iwram_MulQ16(shiftedZ, shiftedZ)) /
                     6 +
                 1;
  s32 sine;
  s32 cosine;
  struct WorkA0Node *node = work->slots[index].head;
  struct WorkA0Node **nodes;
  struct WorkA0Node **cursor;
  struct WorkA0Slot *slot;
  s32 i;
  u16 angle;
  s32 lastX, lastY, lastZ;

  cursor = nodes = MemAlloc(0x200);
  for (i = 0; i <= segments; i++) {
    *cursor++ = node;
    node = node->next;
  }
  angle = ArcTan2(shiftedX, shiftedY);
  sine = Trig_Sin(angle);
  cosine = Trig_Cos(angle);
  for (frame = 1; frame <= segments * 2; frame++) {
    for (i = segments; i >= 0; i--) {
      node = nodes[i];
      // XXX: byte 17 of a node is the slot it belongs to, plus one; likely
      // what the per-frame callbacks read to draw it.
      node->unk11[6] = index + 1;
      if (i == segments) {
        s32 wave = Iwram_MulQ16(Trig_Sin((frame << 14) / i), 0x80000);
        s32 offset = Iwram_MulQ16(wave, cosine);

        node->x = lastX = x + dx * frame / 2 / i + offset;
        offset = Iwram_MulQ16(wave, sine);
        node->y = lastY = y + dy * frame / 2 / i + offset;
        node->z = lastZ = z + dz * frame / 2 / i;
      } else {
        s32 ey = (lastY - node->y) >> 8;
        s32 ez = (lastZ - node->z) >> 8;
        s32 ex = (lastX - node->x) >> 8;
        s32 distance = Iwram_SqrtEntry(
            Iwram_MulQ16(ex, ex) + Iwram_MulQ16(ey, ey) + Iwram_MulQ16(ez, ez));

        if (distance <= 6)
          break;
        node->x = lastX = lastX + (node->x - lastX) * 6 / distance;
        node->y = lastY = lastY + (node->y - lastY) * 6 / distance;
        node->z = lastZ = lastZ + (node->z - lastZ) * 6 / distance;
        node->used = 1;
      }
    }
    WaitTicks(1);
  }
  slot = &work->slots[index];
  // XXX: the field worka0.h calls tail is tested as a flag by Func_080d9d40;
  // a pointer here only because the adopted header types it so.
  slot->tail = (struct WorkA0Node *)1;
  slot->amplitude = 0x40000;
  slot->angle = 0x4000;
  MemFree(nodes);
  return 0;
}
