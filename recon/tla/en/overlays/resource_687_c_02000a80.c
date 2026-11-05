// Draft, differ 14 of 188 (tla-en, 2026-11-05). Older notes below may predate this figure.
// What is left is GCSE's PRE: the listing keeps redundancies PRE removes here. An
// earlier analysis-only compile with -fno-gcse gave its reloads, work in r11 and
// index at sp+16. That is a compiler question, out of scope here, so it stays open.
// PRE from gcse.c and the .07.gcse dump: a store anywhere in a block ends availability
// of every MEM, pool loads included, so PRE reloads after a store and keeps the value.
// NOT SOURCE, none of these may be adopted: spots.spot[index & 0xffff] emits an ands
// and a split 0xffff the listing lacks. It stands where (u16)index belongs because it
// breaks PRE's (ashift index 16) between the table read and the spot read: index then
// spills to sp+16, work takes r11 and the whole tail from the table read on matches.
// With (u16)index, or index as u16 or s32, this body is differ 135, gap 28; & 0xffff on
// a u16 folds back to the same thing. The second site alone masked gives 94.
// These are aliasing or order spellings kept for their effect, which review rejects,
// and each only stops one merge: the union and plain delta reads (without them 175),
// count2 taken before the store (without it 97), tail (without it 114).
// Plain C with none of this, every read spelled gData_*, is differ 248, gap 41.
// One redundancy still open: the pool load of gData_020022bc from the first read to
// the store after the if. It keeps that address in r6, so stamp falls to ip and the
// listing's frame r6, stamp r7 is lost; that is most of the top half's difference.
// Nothing tried splits it. u16 last, phase from *frame (88, gap 14, skeleton worse),
// and storing nowRaw (142) gain nothing.
// With barriers (out, no new device) the earlier body reached differ 12, gap 1:
//   asm("" : : "m"(index));    just before frame = &gFrameCount
//   asm("" : : : "memory");    just after nowRaw =
// which shows the statements and their order are right. A union view of index, an
// 8-byte local, `<< 8 << 8` and set-once address locals all failed to split the shift
// or the pool load. Volatile globals are out: a volatile HImode read becomes ldrh and
// shifts, not ldrsh. Forcing index into memory (a one-element array) is the same
// device as the barrier and the listing's slot is below params, so it is a real spill.
#include "iwram_call.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

struct Spot {
  s32 x;
  s32 z;
};

struct Spots {
  struct Spot spot[6];
};

struct Params {
  s32 unk_00;
  u8 pad[20];
  u16 unk_18;
  u8 pad2[14];
};

extern u32 gFrameCount;
extern u32 gData_020022c4;
extern u16 gData_020022be;
extern s16 gData_020022c0;
extern u16 gData_020022bc;
extern struct Spots gData_02002118;
extern u8 gData_02002110[];
s32 Func_0200034c(s32, s32, s32);
u32 Random16Far(void);
void Func_02002054();
void Func_0200015c(s32, s32, s32, s32, s32, s32, s32, struct Params *);

void Func_02000a80(void) {
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_GetById(8);
  s16 index;
  s32 now;
  s32 last;
  s32 nowRaw;
  u32 phase;
  u16 *count2;
  u32 *frame;
  u32 *stamp;

  index = (Func_0200034c(0, actor->x >> 20, actor->z >> 20) >> 8) - 1;
  frame = &gFrameCount;
  stamp = &gData_020022c4;

  if (*frame - *stamp > 50) {
    gData_020022be = 0;
    gData_020022c0 = 0;
  }
  last = gData_020022bc;
  now = *(s16 *)(work + 340);
  nowRaw = *(u16 *)(work + 340);
  if (last != now) {
    // Each read below takes its own expression; see the header.
    union Delta { s16 s; u16 u; } *both = (union Delta *)&gData_020022c0;
    s16 *delta = (s16 *)both;
    s32 raw;
    s32 diff;
    u16 *count;
    s32 step;

    if (both->s == 0) {
      *delta = last - nowRaw;
    }
    if (*delta == 7) {
      *delta = -1;
    }
    raw = *(u16 *)delta;
    if ((s16)raw == -7) {
      *delta = 1;
    }
    diff = last - now;
    raw = both->u;
    step = (s16)raw;
    count = &gData_020022be;
    if (diff == step || now - last == step * 7) {
      (*count)++;
      *stamp = *frame;
    } else {
      *count = 0;
      *delta = 0;
    }
  }
  // Set before the store, so PRE cannot reuse this pool load below.
  gData_020022bc = *(u16 *)(work + 340);
  count2 = &gData_020022be;
  if (*count2 > 3) {
    phase = gFrameCount & 3;
    if (phase == 0) {
      struct Spots spots = gData_02002118;
      struct Params params;
      u32 z;
      u32 x;

      /* FAKEMATCH(barrier): an empty asm holds the order around it; as no asm,
       * 19 of 188 instructions differ in tla-en. */
      do {
        params.unk_18 = 0x2a1;
        params.unk_00 = 2;
      } while (0);
      x = Random16Far() & 31;
      z = Random16Far() & 31;
      Call1(Func_02002054, 246);
      x <<= 16;
      z <<= 16;
      // Not source; see the header.
      Func_0200015c(spots.spot[index & 0xffff].x + x, 0,
                    spots.spot[index & 0xffff].z + z, 0, 0x13333, phase,
                    0x120001, &params);
    }
  }
  if (gData_020022be == gData_02002110[(u16)index]) {
    s32 zero = 0;
    struct Tail { u8 pad[362]; u16 value; } *tail = (struct Tail *)work;

    tail->value = 200;
    gData_020022be = zero;
    gData_020022c0 = zero;
  }
}
