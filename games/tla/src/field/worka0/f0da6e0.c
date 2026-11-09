#include "types.h"
#include "worka0.h"
#include "iwram_call.h"
#include "fixed_math.h"

void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
void Actor_SetMode(void *object, s32 mode);
void wait(s32 frames);
s32 ArcTan2(s32 y, s32 x);

s32 Func_080da6e0(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2,
                  s32 wave) {
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Node *tail = NULL;
  struct WorkA0Node *node;
  s32 dx, dy, dz;
  s32 dist;
  s32 count;
  s32 frame;
  s32 i;
  s32 angle;
  s32 x, y, z;
  struct WorkA0Node **nodes;
  struct WorkA0Node **entry;

  dx = (x2 - x1) >> 8;
  dy = (y2 - y1) >> 8;
  dz = (z2 - z1) >> 8;
  dist = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);
  count = Iwram_Sqrt(dist) / 6 + 1;
  node = work->slots[slot].head;
  entry = nodes = Heap_Alloc(0x200);
  work->slots[slot].tail = tail;
  if (count >= 0) {
    i = count + 1;
    do {
      i += -1;
      /* FAKEMATCH(barrier): an empty asm reading the counter in a low register,
       * which copies it for the compare ahead of the store; without it, 2 of
       * 259 instructions differ in tla-en. */
      asm("" : : "l"(i));
      *entry++ = node;
      node = node->next;
    } while (i != 0);
  }
  Actor_SetMode(((void **)work)[0], 3);
  Actor_SetMode(((void **)work)[1], 3);
  wait(4);
  for (frame = 0; frame <= count; frame++) {
    angle = frame << 12;
    dist = 0x40000;
    for (i = 0; i <= count; i++) {
      node = nodes[i];
      node->unk11[0] = 0;
      if (i <= frame) {
        x = node->x = x1 + Iwram_MulQ16(Trig_Cos(angle), dist);
        y = node->y = y1;
        z = node->z = z1 + Iwram_MulQ16(Trig_Sin(angle), dist);
        node->used = 2;
        {
          u16 step = ArcTan2(0x60000, dist);

          angle += step;
          dist += step * 4;
        }
      } else {
        dx = (x - node->x) >> 8;
        dy = (y - node->y) >> 8;
        dz = (z - node->z) >> 8;
        dist = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
               Iwram_MulQ16(dz, dz);
        dist = Iwram_Sqrt(dist);
        if (dist <= 6)
          break;
        x = node->x = x + (node->x - x) * 6 / dist;
        y = node->y = y + (node->y - y) * 6 / dist;
        node->z = z + (node->z - z) * 6 / dist;
        node->used = 2;
        z = node->z;
      }
    }
    wait(1);
  }
  Heap_Release(nodes);
  return 0;
}
