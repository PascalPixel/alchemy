// Draft, differ 241 of 188, skeleton gap 67 (tla-en, 2026-11-01)
// Control flow, types, the frame (108 bytes) and every access follow the listing.
// Left is all GCSE: PRE puts a load of *delta after each store to it, cse2 turns
// that into copies of the stored value, and it merges the reloaded addresses and
// work + 340 into long-lived pseudos, where the listing rereads *delta at every
// join and reloads gFrameCount, gData_020022be and gData_020022bc from the pool.
// That pushes work to the sp+16 spill the listing gives index, and index takes
// r11's place in a low register. An analysis-only compile with -fno-gcse gives the
// listing's reloads, its work in r11 and index at sp+16, its pool hops, and the
// tail; only the struct copy's register lists and the two Random16Far results
// (r5/r6 swapped) differ there. Tried without gain: work as a struct, index as s32,
// a pointer for work + 340, later declarations, dropping frame or stamp.
// Rereading *delta through volatile would be a new device (no other function uses
// these globals), so it is left out. wait and zero are locals because a plain
// constant stored to a halfword comes from the pool, and the listing's tail uses
// movs; the other zero, 1 and -1 are the listing's pool constants.
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
  s16 index = (Func_0200034c(0, actor->x >> 20, actor->z >> 20) >> 8) - 1;
  s32 now;
  s32 last;
  s32 nowRaw;
  u32 phase;
  u32 *frame = &gFrameCount;
  u32 *stamp = &gData_020022c4;

  if (*frame - *stamp > 50) {
    gData_020022be = 0;
    gData_020022c0 = 0;
  }
  last = gData_020022bc;
  now = *(s16 *)(work + 340);
  nowRaw = *(u16 *)(work + 340);
  if (last != now) {
    s16 *delta = &gData_020022c0;
    u16 *count;
    s32 step;

    if (*delta == 0) {
      *delta = last - nowRaw;
    }
    if (*delta == 7) {
      *delta = -1;
    }
    if (*delta == -7) {
      *delta = 1;
    }
    step = *delta;
    count = &gData_020022be;
    if (last - now == step || now - last == step * 7) {
      (*count)++;
      *stamp = *frame;
    } else {
      *count = 0;
      *delta = 0;
    }
  }
  gData_020022bc = *(u16 *)(work + 340);
  if (gData_020022be > 3) {
    phase = gFrameCount & 3;
    if (phase == 0) {
      struct Spots spots = gData_02002118;
      struct Params params;
      u32 x, z;

      params.unk_18 = 0x2a1;
      params.unk_00 = 2;
      x = Random16Far() & 31;
      z = Random16Far() & 31;
      Call1(Func_02002054, 246);
      Func_0200015c(spots.spot[(u16)index].x + (x << 16), 0,
                    spots.spot[(u16)index].z + (z << 16), 0, 0x13333, phase,
                    0x120001, &params);
    }
  }
  if (gData_020022be == gData_02002110[(u16)index]) {
    s32 wait = 200;
    s32 zero = 0;

    *(u16 *)(work + 362) = wait;
    gData_020022be = zero;
    gData_020022c0 = zero;
  }
}
