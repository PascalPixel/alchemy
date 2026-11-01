// Draft, differ 223 of 362, skeleton gap 47 (tla-en, 2026-11-01)
// Left, two causes. (1) gcse PRE hoists dz << 7, col + lowerX, col + upperX and
// col - 1; the listing rebuilds them in the loops. The two gotos into the loop
// tests stop that but leave two extra branches and move the row setup behind
// the row test. Dropping both gotos and adding `zero = 0` before the loops,
// `dz += zero` at the top of the inner body and `col += zero` at the top of the
// outer body (cprop folds them, cse2 deletes them, PRE sees the operands killed)
// gives no hoisting, the 56-byte frame and differ 185, skeleton gap 27. That is
// a dead device short of a match, so it came back out; reapply it to test (2).
// (2) With (1) applied every register but one matches: the listing keeps &ahead
// in r7 and spills offset to sp+12, the draft the reverse. Priorities are
// x 0.96, offset 0.73, z 0.72, &ahead 0.17, and z in r8 means r7 was taken
// before z, so &ahead would have to outrank z; refs and live length cannot give
// that. Unsolved; the whole rest of the diff follows from it (reload rotation,
// the r4 constant in Func_02002e8c's arguments).
// Measured dead ends: real for loops strength-reduce x << 20 and offset, which
// the listing leaves alone, even with x, z and the columns set at the top of
// each body. A memory struct for col, dx, dz stops PRE but addresses through a
// base register, not sp. An 8-byte pair { col, dx } stops PRE on col, but its
// doubled priority takes r5:r6. Slots give the declaration order dz, dx, col,
// upper, lower, upperX, lowerX, offset, lowerRow. x must stay ahead of upperCol:
// x last loses actor's r5 (differ 311). gData_020023c4 + 0x188 is this overlay's
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
    struct MapWork *map;
    s32 flag, row, upperZ;
    s32 dz, dx, col;
    u8 *upper, *lower;
    s32 upperX, lowerX;
    s32 offset, lowerRow, upperRow, x, z, upperCol, lowerCol;
    struct Actor *leader;
    struct Local *local;

    flag = GameFlag_Test(0x106);
    if (*(s16 *)(work + 0x166) != 0)
      flag |= 1;
    actor->movementState = flag;
    ahead.x = actor->x;
    ahead.y = actor->y;
    ahead.z = actor->z;
    Vector_AddPolarOffsetFar(0x80000, actor->angle, &ahead);
    if (Func_020054d8(0, ahead.x, ahead.z) == 7) {
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
    upperZ = *(s32 *)&map->layers[1].unk08[4];
    lowerX = *(s32 *)&map->layers[2].unk08[0] >> 20;
    lowerRow = row + (*(s32 *)&map->layers[2].unk08[4] >> 20) - 2;
    dz = -1;
    z = row - 1;
    upperRow = row + (upperZ >> 20) - 1;
    goto rowTest;
  nextRow:
    lowerCol = col + lowerX - 1;
    dx = -1;
    x = col - 1;
    upperCol = col + upperX - 1;
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
    Func_020054b8(actor, ahead.x, ahead.y, ahead.z);
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
