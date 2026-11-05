/// @file
/// Tracks actor 8 around the ring of cells: counts steps made in one direction,
/// spawning an effect every fourth frame once past 3, and rewards a full count.
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

struct EventWork687 {
  u8 unk000[0x16a];
  u16 unk16a;
};

void Func_02000a80(void) {
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_GetById(8);
  s16 index;
  s32 now;
  s32 last;
  s32 nowRaw;
  u32 phase;
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
    // Each read below takes its own expression: the native reloads for each.
    s16 *delta = (s16 *)&gData_020022c0;
    s32 raw;
    s32 diff;
    u16 *count;
    s32 step;

    if (*delta == 0) {
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
    raw = *(u16 *)delta;
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
  gData_020022bc = *(u16 *)(work + 340);
  if (gData_020022be > 3) {
    phase = gFrameCount & 3;
    if (phase == 0) {
      struct Spots spots = gData_02002118;
      struct Params params;
      u32 z;
      u32 x;

      struct Params *p = &params;

      p->unk_18 = 0x2a1;
      p->unk_00 = 2;
      x = Random16Far() & 31;
      z = Random16Far() & 31;
      Call1(Func_02002054, 246);
      x <<= 16;
      z <<= 16;
      Func_0200015c(spots.spot[(u16)index].x + x, 0,
                    spots.spot[(u16)index].z + z, 0, 0x13333, phase,
                    0x120001, p);
    }
  }
  if (gData_020022be == gData_02002110[(u16)index]) {
    s32 zero = 0;

    ((struct EventWork687 *)work)->unk16a = 200;
    gData_020022be = zero;
    gData_020022c0 = zero;
  }
}
