// Draft, differ 6 of 297, skeleton gap 2 (tla-en, 2026-11-01): swings a slot's
// chain (heap slot 0x9c work) between (x, y, z) and the target point for twice
// its segment count of frames: the last node rides the line between the ends,
// raised by a sine wave across the line's heading (ArcTan2 of the distance),
// and each node before it is pulled to within 6 units of the next. It marks
// the slot active (1), and at the end sets its fields 0x0c, 0x14 and 0x18 and
// the slot's flag. Frame, registers and every other instruction match; two
// sched2 orders are left, both in the entry block and neither a source form:
// the listing stores z after index, x and y and before the 192 load, here
// first; the listing stores index * 32 before the add that uses it, here after.
// The sched2 dump explains both: a store ahead of a load that misses costs 2,
// and each listing order needs 3. Stores 4, 6 and 8 rank 23 (the heap load's 21
// plus 2) against the constant chain's 24, and index * 32's store ranks 6 (the
// ldrsh's 4 plus 2) against the add's 7, where a tie would go the listing's way
// on dependents. Both are -mthumb-slow-loads' store rule, a compiler question
// (K2). An empty asm or one-pass block at the top does not help: it also fixes
// the stack arguments' loads (toX, toY, toZ) above the 192 load, and moving the
// initializers to statements for it swaps the slots of index * 32 and nodes.
// Facts that fixed the rest: ex, ey hold the shifted distances at both uses;
// ex also holds the wave and the root; dx then ex and dy then ey, in that order,
// are declared with their initializers; lastX, lastY and lastZ are read back
// from the node after its store, and visible is set before lastZ, which makes
// the shared tail; a void ** cursor gives its store a different alias set from
// node->next, which puts the counter's copy ahead of stmia. Work9c's header has
// no name for these slot fields, so the structs stay here.
#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"

s32 ArcTan2(s32 y, s32 x);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);

struct ChainNode {
  struct ChainNode *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  s8 visible;
  u8 unk13;
};

struct ChainSlot {
  u8 unk00[4];
  s16 segments;
  u8 unk06[2];
  s16 flag;
  s16 active;
  s32 unk0c;
  u8 unk10[4];
  s32 unk14;
  s32 unk18;
  struct ChainNode *head;
};

struct ChainWork {
  u8 unk00[12];
  struct ChainSlot slots[8];
};

s32 Func_080d9498(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  s32 dx = toX - x;
  s32 ex = dx >> 8;
  s32 dy = toY - y;
  s32 ey = dy >> 8;
  s32 dz = toZ - z;
  s32 segments;
  s32 frame;
  s32 sine;
  s32 cosine;
  s32 lastX;
  s32 lastY;
  s32 lastZ;
  struct ChainSlot *slot = &work->slots[index];
  struct ChainNode *node = slot->head;
  void **cursor;
  s32 i;
  u16 angle;
  struct ChainNode **nodes;

  if (slot->active == 1)
    return;
  slot->active = 1;
  segments = slot->segments;
  cursor = nodes = Heap_Alloc(0x200);
  for (i = 0; i <= segments; i++) {
    *cursor++ = node;
    node = node->next;
  }
  angle = ArcTan2(ex, ey);
  sine = Trig_Sin(angle);
  cosine = Trig_Cos(angle);
  for (frame = 1; frame <= segments * 2; frame++) {
    for (i = segments; i >= 0; i--) {
      node = nodes[i];
      node->unk13 = index + 1;
      if (i == segments) {
        ex = Iwram_MulQ16(Trig_Sin((frame << 14) / i), 0x80000);

        node->x = x + dx * frame / 2 / i + Iwram_MulQ16(ex, cosine);
        lastX = node->x;
        node->y = y + dy * frame / 2 / i + Iwram_MulQ16(ex, sine);
        lastY = node->y;
        node->z = z + dz * frame / 2 / i;
        lastZ = node->z;
      } else {
        s32 ez;

        ex = (lastX - node->x) >> 8;
        ey = (lastY - node->y) >> 8;
        ez = (lastZ - node->z) >> 8;
        ex = Iwram_SqrtEntry(Iwram_MulQ16(ex, ex) + Iwram_MulQ16(ey, ey) +
                             Iwram_MulQ16(ez, ez));

        if (ex > 6) {
          node->x = lastX + (node->x - lastX) * 6 / ex;
          lastX = node->x;
          node->y = lastY + (node->y - lastY) * 6 / ex;
          lastY = node->y;
          node->z = lastZ + (node->z - lastZ) * 6 / ex;
          node->visible = 1;
          lastZ = node->z;
        }
      }
    }
    wait(1);
  }
  slot = &work->slots[index];
  slot->unk18 = 1;
  slot->unk0c = 0x40000;
  slot->unk14 = 0x4000;
  if (slot->flag != 0)
    GameFlag_SetBit(slot->flag);
  Heap_Release(nodes);
  return 0;
}
