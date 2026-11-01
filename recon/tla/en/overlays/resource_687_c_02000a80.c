// Draft, differ 135 of 188, skeleton gap 28 (tla-en, 2026-11-01)
// What is left is GCSE's PRE: the listing behaves as if it never ran. An earlier
// analysis-only compile with -fno-gcse gave its reloads, work in r11 and index at
// sp+16. That reading is a compiler question, out of scope here, so nothing below
// changes it; this note records how far C alone gets.
// PRE worked out from gcse.c and the .07.gcse dump: a store anywhere in a block ends
// availability of every MEM, pool loads included. So a value loaded before a store in
// its block cannot be reused past the block. Three forms below come from that, each
// a spelling kept for its effect: the four delta reads each take a different
// expression (union or plain, HImode or zero-extended, since expr_equiv_p compares
// alias sets), so none is partly redundant; count2 takes its address before the store
// to gData_020022bc; diff comes before the last read, which gives the listing's
// scratch r5 there. tail gives 200 a temporary half the size, so zero gets r3.
// z <<= 16 after the call raises z's priority, so x gets r6 and z r5.
// Without a device, two redundancies remain, and they cost the 135: the pool load of
// gData_020022bc (bb 2 to the store after the if) and (ashift index 16) (the call
// block to the table read). The second keeps index in a register and pushes work to
// the sp+16 slot, and everything after shifts.
// With barriers this body reaches differ 12, skeleton gap 1; they are out because
// that is not differ 0. Index must be s32 with the (s16) cast on its assignment:
//   asm("" : : "m"(index));    just before frame = &gFrameCount (index in memory,
//                              and its store comes first)
//   asm("" : : : "memory");    just after nowRaw = (ends the pool load's availability)
// The 12 left at that point: count2's address is in r1 where the listing has r3 (the
// listing loads it after the store, and a memory asm there moves the pool 8 bytes
// early, differ 32, because an empty asm counts 4), and the two index reads load into
// r3 where the listing has r0 and r4, because a memory read ties to the shift's result
// and only a true spill gives the listing's form. A union view of index (HImode read
// at one site), an 8-byte local (a set through a subreg is not hashed), `<< 8 << 8`
// with or without a one-pass block, and set-once address locals all failed to split
// that shift or the pool load: cse folds or merges them, or they get registers.
// Volatile globals are out: a volatile HImode read becomes ldrh and shifts, not ldrsh.
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
  count2 = &gData_020022be;
  gData_020022bc = *(u16 *)(work + 340);
  if (*count2 > 3) {
    phase = gFrameCount & 3;
    if (phase == 0) {
      struct Spots spots = gData_02002118;
      struct Params params;
      u32 z;
      u32 x;

      params.unk_18 = 0x2a1;
      params.unk_00 = 2;
      x = Random16Far() & 31;
      z = Random16Far() & 31;
      Call1(Func_02002054, 246);
      x <<= 16;
      z <<= 16;
      Func_0200015c(spots.spot[(u16)index].x + x, 0,
                    spots.spot[(u16)index].z + z, 0, 0x13333, phase,
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
