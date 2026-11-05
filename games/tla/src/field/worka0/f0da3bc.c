#include "worka0.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "heap_release.h"

s32 ArcTan2(s32 y, s32 x);
void wait(s32 frames);

s32 Func_080da3bc(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct WorkA0 *work = WORK_A0;
  s32 dx = toX - x;
  s32 ex = dx >> 8;
  s32 dy = toY - y;
  s32 ey = dy >> 8;
  s32 dz = toZ - z;
  s32 ez = dz >> 8;
  s32 segments = Iwram_SqrtEntry(Iwram_MulQ16(ex, ex) + Iwram_MulQ16(ey, ey) +
                                 Iwram_MulQ16(ez, ez)) /
                     6 +
                 1;
  s32 frame;
  struct WorkA0Node *node = work->slots[index].head;
  s32 sine;
  s32 height;
  s32 cosine;
  s32 lastX, lastY, lastZ;
  struct WorkA0Node **nodes;
  struct WorkA0Node **cursor;
  struct WorkA0Slot *slot;
  s32 i;
  u16 angle;

  cursor = nodes = Heap_Alloc(0x200);
  if (segments >= 0) {
    i = segments + 1;
    do {
      i += -1;
      /* FAKEMATCH(barrier): an empty asm reading the counter, which copies it
       * for the compare ahead of the store; as the plain counted loop, 2 of 319
       * instructions differ in tla-en. */
      asm("" : : "l"(i));
      *cursor++ = node;
      node = node->next;
    } while (i != 0);
  }
  angle = ArcTan2(ex, ey);
  sine = Trig_Sin(angle);
  cosine = Trig_Cos(angle);
  for (frame = 1; frame <= segments * 2; frame++) {
    for (i = segments; i >= 0; i--) {
      node = nodes[i];
      // XXX: byte 17 of a node is the slot it belongs to, plus one; likely
      // what the per-frame callbacks read to draw it.
      node->unk11[0] = index + 1;
      if (i == segments) {
        s32 offsetX;
        s32 offsetY;

        height = Iwram_MulQ16(Trig_Sin((frame << 14) / i), 0x80000);
        offsetX = Iwram_MulQ16(height, cosine);
        lastX = node->x = x + dx * frame / 2 / i + offsetX;
        offsetY = Iwram_MulQ16(height, sine);
        lastY = node->y = y + dy * frame / 2 / i + offsetY;
        lastZ = node->z = z + dz * frame / 2 / i;
      } else {
        ex = (lastX - node->x) >> 8;
        ey = (lastY - node->y) >> 8;
        ez = (lastZ - node->z) >> 8;
        height = Iwram_SqrtEntry(Iwram_MulQ16(ex, ex) + Iwram_MulQ16(ey, ey) +
                                 Iwram_MulQ16(ez, ez));

        if (height <= 6)
          break;
        lastX = node->x = lastX + (node->x - lastX) * 6 / height;
        lastY = node->y = lastY + (node->y - lastY) * 6 / height;
        lastZ = node->z = lastZ + (node->z - lastZ) * 6 / height;
        node->used = 1;
      }
    }
    wait(1);
  }
  slot = &work->slots[index];
  // XXX: the field worka0.h calls tail is tested as a flag by Func_080d9d40;
  // a pointer here only because the adopted header types it so.
  slot->tail = (struct WorkA0Node *)1;
  slot->amplitude = 0x40000;
  slot->angle = 0x4000;
  Heap_Release(nodes);
  return 0;
}
