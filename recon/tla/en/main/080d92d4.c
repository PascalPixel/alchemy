// Draft, differ 203 (226 before statements reordered 2026-10-26) of 204: stretches a slot's chain of nodes (heap slot
// 0x9c work) between two objects. When the distance now needs more segments
// than the chain has, the chain is drawn out to the length it covers, then
// grown a frame later by Func_080d9790; otherwise it is drawn whole, the
// shared wobble angle advances and both objects return to mode 2. A target
// whose byte 99 is 99 adjusts its x and z through its hook at offset 104
// first. Left: other. The native stack frame is 52 bytes against 40: it spills
// the two first squares (sp 28, 24) and stores the partial and whole sums to
// sp 16 and 20, never read again, so the source probably kept them in
// memory in a way this draft does not (an array or address-taken locals);
// the rest follows from that, mostly register choice (dx, dy, dz in r8, r9,
// r11, the raised end y kept in r6). count * (to - from) written either way
// gives 226 and 228.
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

void WaitFrames(s32 frames);
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
    s32 dx, dy, dz;
    s32 segments;

    if (((u8 *)to)[99] == 99)
      (*(TargetHook *)((u8 *)to + 104))(to, &toX, &toZ);
    dx = (toX - from->x) >> 8;
    dz = (toZ - from->z) >> 8;
    dy = (toY - from->y) >> 8;
    segments = Iwram_SqrtEntry(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                               Iwram_MulQ16(dz, dz)) /
                   6 +
               1;
    if (segments > slot->segments) {
      s32 count = slot->segments + 1;
      s32 endX = from->x + (toX - from->x) * count / segments;
      s32 endZ = from->z + (toZ - from->z) * count / segments;

      Func_080d9498(index, from->x, from->y + 0x80000, from->z, endX,
                    toY + 0x80000, endZ, Trig_Sin(gData_080f393c) * 4);
      slot->unk18 = 0;
      WaitFrames(1);
      Func_080d9790(index, from->x, from->y + 0x80000, from->z, endX,
                    toY + 0x80000, endZ, Trig_Sin(gData_080f393c) * 4);
    } else {
      Func_080d9498(index, from->x, from->y + 0x80000, from->z, to->x,
                    to->y + 0x80000, to->z, Trig_Sin(gData_080f393c) * 4);
      gData_080f393c += 0x1000;
      WaitFrames(6);
      Actor_SetMode(from, 2);
      Actor_SetMode(to, 2);
    }
  }
}
