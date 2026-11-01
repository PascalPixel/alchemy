// Draft, differ 321 of 362, skeleton gap 93 (tla-en, 2026-11-01)
// The whole body is translated through struct Actor, struct MapWork and struct
// MapCell. Left: compiled with -fno-gcse as a diagnostic, this C gives the
// listing's 56-byte frame and nearly its skeleton from plain bottom-test goto
// loops, so the original's loops reach gcse with something that stops PRE
// hoisting dz << 7, col + lowerX, col + upperX and col - 1. Entering each loop
// through its test (the two gotos below) stops the hoisting and gives the
// frame, but leaves two extra branches and moves the row setup behind the row
// test. for, while and do forms bring loop.c's strength reduction and reversal,
// which the listing does not have; bounds held in locals are folded by cse.
// Also left: the listing keeps &ahead in r7 and spills offset, the draft the
// reverse, and that sets the reload forms at the head (the r8 for
// &actor->action) and around the Vec. gData_020023c4 + 0x188 is this overlay's
// address.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "event_work.h"
#include "main_routine_work.h"
#include "call.h"

struct Vec {
  s32 x, y, z;
};

struct Local {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[4];
  u16 unk0a;
  u8 unk0c[0x60];
  struct Actor *unk6c;
};

void Func_02002d08(struct Actor *actor);
void Func_02002e8c(s32 x, s32 y, s32 z, s32 angle);
void Func_020054b8(struct Actor *actor, s32 x, s32 y, s32 z);
s32 Func_020054d8(s32 mode, s32 x, s32 z);
void Func_020054e8(s32 a, s32 b, s32 w, s32 h, s32 x, s32 z);
void Func_020054f0(s32 a, s32 b, s32 w, s32 h, s32 x, s32 z);
void Func_02005510(s32 x, s32 y, s32 z);
void Func_020056c0(s32 sound);
s32 GameFlag_Test(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Vector_AddPolarOffsetFar(s32 distance, u32 angle, struct Vec *vec);
extern u8 gData_020023c4[];

void Func_02002f80(struct Actor *actor) {
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;

  if (actor->action == 0) {
    struct Vec ahead;
    struct Vec *p;
    struct MapWork *map;
    s32 flag, col, row;
    u8 *upper, *lower;
    s32 upperX, lowerX, upperRow, lowerRow;
    s32 dz, dx, offset, x, z, upperCol, lowerCol;
    struct Actor *leader;
    struct Local *local;

    flag = GameFlag_Test(0x106);
    if (*(s16 *)(work + 0x166) != 0)
      flag |= 1;
    actor->movementState = flag;
    p = &ahead;
    p->x = actor->x;
    p->y = actor->y;
    p->z = actor->z;
    Vector_AddPolarOffsetFar(0x80000, actor->angle, p);
    if (Func_020054d8(0, p->x, p->z) == 7) {
      actor->targetX = actor->targetY = actor->targetZ = 0x80000000;
      actor->acceleration = 0x4000;
      actor->action = 1;
      Func_020056c0(145);
      Call3((void (*)())Func_02005510, 0x50000, 0x50000, 0x10000);
      Call3((void (*)())Func_02005510, -1, -1, 0xe666);
      if (actor->watchedObject != 0)
        ((void (*)(struct Actor *))actor->watchedObject)(actor);
    }

    map = Ram_HeapSlots->mapWork;
    col = actor->x >> 20;
    row = actor->z >> 20;
    upper = (u8 *)(map->layers[1].cells + (col + (row << 7)));
    lower = (u8 *)(map->layers[2].cells + (col + (row << 7)));
    upperX = *(s32 *)&map->layers[1].unk08[0] >> 20;
    upperRow = *(s32 *)&map->layers[1].unk08[4];
    lowerX = *(s32 *)&map->layers[2].unk08[0] >> 20;
    lowerRow = row + (*(s32 *)&map->layers[2].unk08[4] >> 20) - 2;
    dz = -1;
    z = row - 1;
    upperRow = row + (upperRow >> 20) - 1;
    goto rowTest;
  nextRow:
    lowerCol = col + lowerX - 1;
    dx = -1;
    upperCol = col + upperX - 1;
    x = col - 1;
    goto colTest;
  nextCol:
    offset = ((dz << 7) + dx) << 2;
    if (((struct MapCell *)(upper + offset))->collisionCode == 77) {
      ((struct MapCell *)(upper + offset))->collisionCode = 0;
      Func_020054f0(64, 64, 1, 1, upperCol, upperRow);
      Func_020054e8(64, 64, 1, 1, x, z);
      Func_020056c0(573);
      Func_02002e8c((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,
                    actor->angle);
    }
    if (((struct MapCell *)(lower + offset))->collisionCode == 77) {
      ((struct MapCell *)(lower + offset))->collisionCode = 0;
      Func_020054f0(64, 64, 1, 2, lowerCol, lowerRow);
      Func_020054e8(64, 64, 1, 1, x, z);
      Func_020056c0(572);
      Func_02002e8c((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,
                    actor->angle);
    }
    dx++;
    lowerCol++;
    x++;
    upperCol++;
  colTest:
    if (dx <= 1)
      goto nextCol;
    lowerRow++;
    dz++;
    z++;
    upperRow++;
  rowTest:
    if (dz <= 1)
      goto nextRow;
    if (actor->scaleX < 0x10000) {
      actor->scaleX += 0x1000;
      actor->scaleY += 0x1000;
    }
    Func_020054b8(actor, p->x, p->y, p->z);
    work = (u8 *)Ram_HeapSlots->eventWork;
    local = (struct Local *)(gData_020023c4 + 0x188);
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if ((actor->x - leader->x >= 0 ? actor->x - leader->x
                                   : leader->x - actor->x) < 0x140000 &&
        (u32)(actor->y - leader->y + 0x7ffff) <= 0x1ffffe &&
        (actor->z - leader->z >= 0 ? actor->z - leader->z
                                   : leader->z - actor->z) < 0x140000) {
      local->unk04 = 2;
      local->unk6c = actor;
      *(u16 *)(work + 0x154) = local->unk0a + 2;
    }
  } else if (actor->action == 1) {
    actor->movementState = 0;
    if (actor->scaleX > 0) {
      actor->scaleX -= 0x1000;
      actor->scaleY -= 0x1000;
      Func_02002d08(actor);
    } else {
      actor->x = actor->y = actor->z = 0;
      actor->velocityX = actor->velocityY = actor->velocityZ = 0;
      actor->update = 0;
    }
  }
}
