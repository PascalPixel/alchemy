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
s32 Func_020054d8(s32 terrain, s32 x, s32 z);
void Func_020054b8(struct Actor *actor, s32 x, s32 y, s32 z);
void Func_020054e8(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);
void Func_020054f0(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);
void Func_02005510(s32 a, s32 b, s32 c);
void Func_020056c0(s32 sound);
void Func_02002d08(struct Actor *actor);
void Func_02002e8c(s32 x, s32 y, s32 z, u32 angle);
extern u8 gData_020023c4[];

void Func_02002f80(struct Actor *actor) {
  struct Vector3 position;
  struct WorkEvent *work = Ram_HeapSlots->eventWork;
  s32 state = actor->action;

  if (state == 0) {
    s32 dz;
    s32 dx;
    s32 tileX;
    struct MapCell *cellsA;
    struct MapCell *cellsB;
    s32 widthA;
    s32 widthB;
    s32 offset;
    s32 zB;
    s32 tileZ;
    s32 heightA;
    s32 heightB;
    s32 x;
    s32 z;
    s32 xA;
    s32 xB;
    s32 zA;
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
    if (Func_020054d8(0, position.x, position.z) == 7) {
      actor->targetZ = 0x80000000;
      actor->targetY = 0x80000000;
      actor->targetX = 0x80000000;
      actor->acceleration = 0x4000;
      actor->action = 1;
      Func_020056c0(145);
      /* FAKEMATCH(wrapper): forwarded, so each argument builds its own 0x50000;
       * as a direct call, 6 of 362 instructions differ in tla-en. */
      Call3((void (*)())Func_02005510, 0x50000, 0x50000, 0x10000);
      /* FAKEMATCH(wrapper): forwarded, so each argument builds its own -1; as a
       * direct call, 17 of 362 instructions differ in tla-en. */
      Call3((void (*)())Func_02005510, -1, -1, SPLIT_CONSTANT(0xe600, 102));
      if (actor->watchedObject != NULL)
        ((void (*)(struct Actor *))actor->watchedObject)(actor);
    }

    map = Ram_HeapSlots->mapWork;
    tileX = actor->x >> 20;
    tileZ = actor->z >> 20;
    cellsA = &map->layers[1].cells[tileX + (tileZ << 7)];
    cellsB = &map->layers[2].cells[tileX + (tileZ << 7)];
    widthA = *(s32 *)&map->layers[1].unk08[0] >> 20;
    heightA = *(s32 *)&map->layers[1].unk08[4] >> 20;
    widthB = *(s32 *)&map->layers[2].unk08[0] >> 20;
    heightB = *(s32 *)&map->layers[2].unk08[4] >> 20;

    for (dz = -1, zB = tileZ + heightB - 2, z = tileZ - 1,
        zA = tileZ + heightA - 1;
         dz <= 1; zB++, z++, zA++, dz++) {
      for (dx = -1, xB = tileX + widthB - 1, x = tileX - 1,
          xA = tileX + widthA - 1;
           dx <= 1; xB++, x++, xA++, dx++) {
        offset = ((dz << 7) + dx) * 4;
        cell = (struct MapCell *)(offset + (s32)cellsA);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          Func_020054f0(64, 64, 1, 1, xA, zA);
          Func_020054e8(64, 64, 1, 1, x, z);
          Func_020056c0(SPLIT_CONSTANT(0x13e, 255));
          Func_02002e8c((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,
                        actor->angle);
        }
        cell = (struct MapCell *)(offset + (s32)cellsB);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          Func_020054f0(64, 64, 1, 2, xB, zB);
          Func_020054e8(64, 64, 1, 1, x, z);
          Func_020056c0(0x23c);
          Func_02002e8c((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,
                        actor->angle);
        }
      }
    }

    if (actor->scaleX < 0x10000) {
      actor->scaleX += 0x1000;
      actor->scaleY += 0x1000;
    }
    Func_020054b8(actor, position.x, position.y, position.z);

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
      Func_02002d08(actor);
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
