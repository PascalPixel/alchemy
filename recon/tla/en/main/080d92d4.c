// Draft, differ 2 of 204, skeleton gap 1 (tla-en, 2026-11-01): stretches a
// slot's chain of nodes (heap slot 0x9c work) between two objects. When the
// distance now needs more segments than the chain has, the chain is drawn out
// to the length it covers, then grown a frame later by Func_080d9790; otherwise
// it is drawn whole, the shared wobble angle advances and both objects return
// to mode 2. A target whose byte 99 is 99 adjusts its x and z through its hook
// at offset 104 first.
//
// Left: one sched2 order at entry, a compiler question (K2). The listing stores
// index before the 192 chain, here after. The sched2 dump gives the store rank
// 71 (the heap load's 69 plus the 2 of -mthumb-slow-loads' store rule) against
// the chain's 72; the store's only other dependent ranks far lower, so no form
// of this source moves it. Func_080d9498's draft has the same miss. Tried and
// taken back out: an empty asm or a one-pass block at the top moves the store
// but also fixes toId's entry copy (adds r6, r2) above the chain, so differ
// stays 2; a local copy of toId is folded by cse2, even with a dead store of it
// after the hook; only a device that keeps that copy in the body would close
// this, and it would take two or three tags for one instruction.
//
// Facts that fixed the rest (from differ 203): x, y and z are one variable each
// for the shifted distances and the end point (r8, r9, r11 in both); y's 0
// default ahead of the hook's label puts its first set before CSE's block, so
// y = toY survives instead of folding to toY, which keeps r9 busy and leaves the
// sine address reloaded; the kept pointer mul takes r6, which pushes xx and yy
// to the stack and, through reload, length and the partial sum; xx and yy are
// named so length's pseudo falls between them and the partial sum, which orders
// sp 20 and 16; the difference comes first in each product.
#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "actor.h"
#include "ram_buffer.h"

struct ChainSlot {
  u8 unk00[4];
  s16 segments;
  u8 unk06[18];
  s32 unk18;
  void *head;
};

struct ChainWork {
  u8 unk00[12];
  struct ChainSlot slots[8];
};

typedef void (*TargetHook)(struct Actor *, s32 *, s32 *);

extern s32 gData_080f393c;

void wait(s32 frames);
void Func_080d9498(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                   s32 wobble);
void Func_080d9790(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                   s32 wobble);

void Func_080d92d4(s32 index, s32 fromId, s32 toId) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  struct Actor *from = ObjectTable_Get(fromId);
  struct Actor *to = ObjectTable_Get(toId);

  if (from != NULL && to != NULL) {
    s32 toX = to->x;
    s32 toY = to->y;
    s32 toZ = to->z;
    struct ChainSlot *slot = &work->slots[index];
    s32 x, y = 0, z; // Default: y.
    s32 xx, yy, length, segments, count;
    s32 (*mul)(s32, s32);

    if (((u8 *)to)[99] == 99)
      (*(TargetHook *)((u8 *)to + 104))(to, &toX, &toZ);
    x = (toX - from->x) >> 8;
    y = (toY - from->y) >> 8;
    z = (toZ - from->z) >> 8;
    mul = Iwram_MulQ16;
    xx = mul(x, x);
    yy = mul(y, y);
    length = xx + yy + mul(z, z);
    segments = Iwram_SqrtEntry(length) / 6 + 1;
    count = slot->segments;
    if (segments > count) {
      count++;
      x = from->x + (toX - from->x) * count / segments;
      y = toY;
      z = from->z + (toZ - from->z) * count / segments;

      Func_080d9498(index, from->x, from->y + 0x80000, from->z, x, y + 0x80000,
                    z, Trig_Sin(gData_080f393c) << 2);
      slot->unk18 = 0;
      wait(1);
      Func_080d9790(index, from->x, from->y + 0x80000, from->z, x, y + 0x80000,
                    z, Trig_Sin(gData_080f393c) << 2);
    } else {
      Func_080d9498(index, from->x, from->y + 0x80000, from->z, to->x,
                    to->y + 0x80000, to->z, Trig_Sin(gData_080f393c) << 2);
      gData_080f393c += 0x1000;
      wait(6);
      Actor_SetMode(from, 2);
      Actor_SetMode(to, 2);
    }
  }
}
