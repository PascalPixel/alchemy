// Draft, differ 221 of 362, skeleton gap 41/46 (tla-en, 2026-11-01)
// Types, loads, calls and control flow agree with the listing; 367 instructions
// against 362. A landing watcher: tests the 3 by 3 cells around the actor.
// Left, all from one cause: the listing's 3 by 3 cell loops keep everything in
// the body (no giv reduction, tileX + width and dz << 7 rebuilt each pass),
// which is the fact 3 family of the twins: alchemy-gcc's PRE hoists them.
// - A real for reduces the offset and (x << 20) givs and hoists; dropped.
// - Top-entered goto loops: PRE (.07.gcse "PRE/HOIST: edge (6,7)") takes
//   tileX + width and dz << 7 out, costing a spill.
// - This form, entered at the bottom test, stops PRE, but the entry `goto` is
//   never threaded, so two real entry tests remain, and try_merge_blocks moves
//   the row block after rowTest (`goto row` plus no fall-through): that is the
//   whole gap at the end of the skeleton. cse1 does not follow an unconditional
//   jump, and cse2 runs with follow_jumps off.
// - Tried, all out again: a conditional entry on the loop variable (the first
//   thread_jumps threads it before gcse, PRE returns); on a one-constant local
//   (cprop folds it to the same goto); `do { dz = -1; } while (0)` plus a copy
//   to survive to the second thread_jumps (cse2 folds it first); {dx, dz} as a
//   DImode pair (kills dz << 7's hoist but takes r6:r7 and PRE keeps tileX + w);
//   {tileX, dx, dz} in memory (loses the reloads; gap 52/61).
// - A real for over dx and dz with every argument written tileX + widthA + dx
//   gives the listing's x, xA, xB, z, zA, zB as loop.c's own givs, and PRE then
//   hoists only dz << 7, but that makes the offset a giv too and reduces
//   (x << 20) + 0x80000 and the 0x80000 (differ 294). A one-constant bound
//   (dz <= radius) leaves the entry tests in and PRE still hoists onto edge
//   (7,8), past the test (differ 308). Top-entered both loops: differ 339;
//   column only top-entered: differ 231, gap 40/45; row only: differ 368.
// Allocation, the rest of the differ, cannot be reached in plain C here: the
// listing spills dx and the offset yet keeps position in r7 and zA in r11.
// .18.greg priorities are offset 0.73, dx 0.49, zA 0.25, position 0.17, so
// the offset (r7) and dx (r10) go first. Those two sit under position only if
// their refs are not weighted by loop depth (offset 3 refs, dx 5). A named
// offset local changes nothing. A compiler question, out of scope here (K3),
// as is whether PRE ran on the native at all.
// Call3 at the -1, -1 call is the old draft's device: without it the two -1
// share a register (differ 320). Call3 at the 0x50000 call too gives r0 before
// r1 as the listing has (differ 215), but that is a new device, so not kept.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "vector3.h"
#include "map.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))

struct Work020023c4 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[4];
  u16 unk0a;
  u8 unk0c[0x6c - 0x0c];
  struct Actor *unk6c;
};

struct WorkEvent {
  u8 unk000[340];
  u16 unk154;
  u8 unk156[358 - 342];
  s16 unk166;
};

s32 GameFlag_Test(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Vector_AddPolarOffsetFar(s32 distance, u32 angle, struct Vector3 *v);
s32 Func_0200422c(s32 terrain, s32 x, s32 z);
void Func_02004204(struct Actor *actor, s32 x, s32 y, s32 z);
void Func_0200423c(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);
void Func_02004244(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);
void Func_02004264(s32 a, s32 b, s32 c);
void Func_0200446c(s32 sound);
void Func_020024d4(struct Actor *actor);
void Func_02002658(s32 x, s32 y, s32 z, u32 angle);
extern u8 gData_020023c4[];

void Func_0200274c(struct Actor *actor) {
  struct Vector3 position;
  struct WorkEvent *work = Ram_HeapSlots->eventWork;
  s32 state = actor->action;

  if (state == 0) {
    s32 dz, dx, tileX;
    struct MapCell *cellsA, *cellsB;
    s32 widthA, widthB;
    s32 zB;
    s32 tileZ, heightA, heightB;
    s32 x, z, xA, xB, zA;
    s32 flag;

    struct MapWork *map;
    struct MapCell *cell;
    struct WorkEvent *work2;
    struct Work020023c4 *other;
    struct Actor *target;

    flag = GameFlag_Test(0x106);
    if (work->unk166 != 0)
      flag |= 1;
    actor->movementState = flag;

    position.x = actor->x;
    position.y = actor->y;
    position.z = actor->z;
    Vector_AddPolarOffsetFar(0x80000, actor->angle, &position);
    if (Func_0200422c(0, position.x, position.z) == 7) {
      actor->targetZ = 0x80000000;
      actor->targetY = 0x80000000;
      actor->targetX = 0x80000000;
      actor->acceleration = 0x4000;
      actor->action = 1;
      Func_0200446c(145);
      Call3((void (*)())Func_02004264, 0x50000, 0x50000, 0x10000);
      Call3((void (*)())Func_02004264, -1, -1, 0xe666);
      if (actor->watchedObject != NULL)
        ((void (*)(struct Actor *))actor->watchedObject)(actor);
    }

    map = Ram_HeapSlots->mapWork;
    tileX = actor->x >> 20;
    tileZ = actor->z >> 20;
    cellsA = &map->layers[1].cells[(tileZ << 7) + tileX];
    cellsB = &map->layers[2].cells[(tileZ << 7) + tileX];
    widthA = *(s32 *)&map->layers[1].unk08[0] >> 20;
    heightA = *(s32 *)&map->layers[1].unk08[4] >> 20;
    widthB = *(s32 *)&map->layers[2].unk08[0] >> 20;
    heightB = *(s32 *)&map->layers[2].unk08[4] >> 20;

    dz = -1;
    zB = tileZ + heightB - 2;
    z = tileZ - 1;
    zA = tileZ + heightA - 1;
    goto rowTest;
  row:
    dx = -1;
    xB = tileX + widthB - 1;
    x = tileX - 1;
    xA = tileX + widthA - 1;
    goto columnTest;
  column:
    cell = &cellsA[(dz << 7) + dx];
    if (cell->collisionCode == 77) {
      cell->collisionCode = 0;
      Func_02004244(64, 64, 1, 1, xA, zA);
      Func_0200423c(64, 64, 1, 1, x, z);
      Func_0200446c(0x23d);
      Func_02002658((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,
                    actor->angle);
    }
    cell = &cellsB[(dz << 7) + dx];
    if (cell->collisionCode == 77) {
      cell->collisionCode = 0;
      Func_02004244(64, 64, 1, 2, xB, zB);
      Func_0200423c(64, 64, 1, 1, x, z);
      Func_0200446c(0x23c);
      Func_02002658((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,
                    actor->angle);
    }
    dx++;
    xB++;
    x++;
    xA++;
  columnTest:
    if (dx <= 1)
      goto column;
    zB++;
    dz++;
    z++;
    zA++;
  rowTest:
    if (dz <= 1)
      goto row;

    if (actor->scaleX < 0x10000) {
      actor->scaleX += 0x1000;
      actor->scaleY += 0x1000;
    }
    Func_02004204(actor, position.x, position.y, position.z);

    work2 = Ram_HeapSlots->eventWork;
    other = (struct Work020023c4 *)(gData_020023c4 + 0x188);
    target = Actor_GetById(gMainRoutineWork.currentOwner);
    if (ABS(actor->x - target->x) < 0x140000 &&
        (u32)(actor->y - target->y + 0x7ffff) <= 0x1ffffe &&
        ABS(actor->z - target->z) < 0x140000) {
      other->unk04 = 2;
      other->unk6c = actor;
      work2->unk154 = other->unk0a + 2;
    }
  } else if (state == 1) {
    actor->movementState = 0;
    if (actor->scaleX > 0) {
      actor->scaleX -= 0x1000;
      actor->scaleY -= 0x1000;
      Func_020024d4(actor);
    } else {
      actor->z = 0;
      actor->y = 0;
      actor->x = 0;
      actor->velocityZ = 0;
      actor->velocityY = 0;
      actor->velocityX = 0;
      actor->update = NULL;
    }
  }
}
