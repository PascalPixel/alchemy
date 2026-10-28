// Draft, differ 106 of 213: updates the swinging actors recorded by
// Func_02001068 (16 records of gData_020023c4 + 0x188): at each end of its
// period an actor plays cue 206 or 395 and starts rising or sinking, its
// height stops at the record's limits, the period counts on unless flag
// 0x104 is set, the cell under it is marked in the map's shape grid,
// collision height and cell flags, and its shadow follows it with a bob
// while it is raised. The same listing as overlay 661's Func_020039b8 and
// 697's Func_02003da0. Left: the native keeps the table in r9, the cell flags base in r11, the collision cursor in r8 and the count in r10, takes the shape grid from the stack, and stores the cell flags through a shared tail.
#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "iwram_call.h"

struct SwingActor {
  s16 id;
  s16 phase;
  s16 height;
  s16 period;
  s16 riseAt;
  s16 sinkAt;
  s16 top;
  s16 bottom;
  s16 unk10;
  s16 speed;
  s16 unk14;
  s16 bob;
  struct Actor *shadow;
};

extern u8 gData_020023c4[];
extern s32 gMapCollision[];
struct Actor *Actor_GetById(u32 id);
void Func_0200131c(s32 cue);
s32 GameFlag_Test(s32 flag);
s32 Math_Sine(s32 angle);

void Func_02000be4(void) {
  u8 *table = gData_020023c4 + 0x188;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *shape = *(u8 **)(map + 0x13c);
  u8 *cells = *(u8 **)(map + 0x138);
  s32 *collision = gMapCollision + *(s16 *)(table + 2);
  struct SwingActor *swing = (struct SwingActor *)(table + 4);
  struct Actor *actor, *shadow;
  s32 n, cell, flag;
  u8 *flags;

  for (n = 0; n <= 15; n++, swing++, collision++) {
    if (swing->id == 0)
      continue;
    actor = Actor_GetById(swing->id);
    if (actor->x == 0 && actor->z == 0)
      continue;
    if (swing->phase == swing->riseAt) {
      Func_0200131c(206);
      swing->speed = 4;
    }
    if (swing->phase == swing->sinkAt) {
      Func_0200131c(395);
      swing->speed = -8;
    }
    if (swing->speed != 0) {
      swing->height += swing->speed;
      if (swing->height >= swing->top) {
        swing->height = swing->top;
        swing->speed = 0;
        swing->bob = 0;
      }
      if (swing->height <= swing->bottom) {
        swing->height = swing->bottom;
        swing->speed = 0;
        swing->bob = 0;
      }
    }
    flag = GameFlag_Test(0x104);
    if (!flag && ++swing->phase >= swing->period)
      swing->phase = flag;
    cell = ((actor->z >> 20) << 7) + (actor->x >> 20);
    shape[cell] = table[2] + n;
    *collision = ((((s16 *)&actor->y)[1] + swing->height) / 8) << 8;
    flags = cells + cell * 4;
    flags[3] = swing->speed != 0 ? 0 : flags[3] | 128;
    if (swing->height != 0)
      flags[3] |= 16;
    shadow = swing->shadow;
    if (shadow == 0)
      continue;
    if (swing->height == 0) {
      shadow->y = actor->y - 0x100000;
    } else {
      swing->bob += 0x1000;
      shadow->y = actor->y + (swing->height << 16) +
                  Iwram_MulQ16(0x20000, Math_Sine(swing->bob)) - 0xe0000;
    }
    shadow->x = actor->x;
    shadow->z = actor->z;
  }
}
