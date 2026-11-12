/// @file
/// Scene code of overlay 0x681, in ROM order, from s495c's offset up to the next object the overlay links.

#include "../../../../games/tla/include/types.h"
#include "../../../../games/tla/include/system/ram_buffer.h"
#include "../../../../games/tla/include/map.h"
#include "system/ram_buffer.h"
#include "../../../../games/tla/include/actor.h"
#include "system/main_routine_work.h"
#include "event_inline.h"
#include "types.h"
#include "map.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"

void Func_0200495c(s32 layer, s32 keep) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayer *mapLayer = &map->layers[layer];
  u32 *cells = (u32 *)mapLayer->cells;
  s32 width = (map->maxX - map->minX) >> 20;
  s32 height = (map->maxZ - map->minZ) >> 20;
  u32 fill;
  u16 z;
  u16 x;

  cells += ((map->minZ >> 20) << 7) + (map->minX >> 20);
  fill = *cells;
  for (z = 0; z < height; z++) {
    u32 *cell = &cells[z * 128];

    for (x = 0; x < width; x++) {
      if (((u8 *)cell)[2] != 0 && ((u8 *)cell)[2] != keep)
        *cell = fill;
      cell++;
    }
  }
}

void Func_020049e4(void) {
  if (gMainRoutineWork.renderMode == 2) {
    u8 *event = Ram_HeapSlots->eventWork;
    struct MapWork *map = Ram_HeapSlots->mapWork;
    struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
    s32 x = object->x / 0x100000;
    s32 z = (object->z - object->y + 0x7fff) / 0x100000;
    struct MapCell *cells = map->layers[2].cells;
    struct MapCell *cell = &cells[x + (z << 7)];
    s32 code = cell->collisionCode;

    if (code >= 1 && code <= 230)
      *(u16 *)(event + 340) = code;
  }
}

void Actor_SetSingleChildFlags(struct Actor *object, u32 value);

void Func_02004a5c(s32 id, s32 x, s32 y, s32 z) {
  struct Actor *object = Actor_Get(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[2].cells;
  s32 column = x / 0x100000;
  s32 row = (z - y + 0x7fff) / 0x100000;

  cell += column + (row << 7);
  cell->collisionCode = 255;
  object->x = (x & 0xfff00000) + 0x80000;
  object->z = (z & 0xfff00000) + 0x80000;
  object->y = object->z - (row << 20);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  Actor_SetSingleChildFlags(object, 0);
}

extern const u8 gData_020062d4[];
extern const u8 gData_02006290[];

void Func_02004e5c(void);
void Func_02004e64(void);
void Func_02004dac(struct Actor *object, const u8 *script);
void Func_02004d9c(struct Actor *object, s32 mode);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void Func_02004f54(void);

void Func_02004af0(s32 id, s32 flag) {
  struct MainRoutineWork *entryWork = &gMainRoutineWork;
  struct Actor *leader;
  s32 gravity;
  s32 value;
  struct Actor *object;
  s32 i;
  struct MainRoutineWork *work;

  /* FAKEMATCH(barrier): the form kept; as plain gMainRoutineWork.currentOwner
   * with no asm, 2 of 218 instructions differ in tla-en. */
  __asm__("" : "=r"(entryWork) : "0"(entryWork));
  /* FAKEMATCH(barrier): the form kept; as plain code with no asm here, 2 of 218
   * instructions differ in tla-en. */
  __asm__("");
  /* FAKEMATCH(temporary): the form kept; as id used directly from entry, 4 of
   * 218 instructions differ in tla-en. */
  value = id;
  leader = Actor_Get(entryWork->currentOwner);
  gravity = leader->gravity;
  Func_02004e5c();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  object = Actor_Get(value);
  Func_02004a5c(value, leader->x, leader->y, leader->z);
  Func_02004dac(object, gData_020062d4);
  i = 15;
burst:
  object = (FieldActor_Create(
      255, leader->x, leader->y + 0x100000, leader->z));
  if (object != NULL) {
    u32 angle;
    u32 size;
    s32 scale;

    Func_02004dac(object, gData_02006290);
    angle = Random16Far();
    value = Random16Far();
    size = Random16Far();
    scale = (size >> 1) + 0x2000;
    object->velocityY = Iwram_MulQ16(value << 3, Math_Cosine(angle));
    object->velocityX = Iwram_MulQ16(value << 3, Math_Sin(angle));
    object->acceleration = 0;
    object->scaleX = scale;
    object->scaleY = scale;
    object->gravity = 0x547a;
    object->bounce = 0x8000;
  }
  i--;
  if (i >= 0)
    goto burst;
  Event_PlaySound(590);
  Event_PlaySound(217);
  MapGrid_SetShake(0x20000, 0x20000, 0xe666);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 257);
  Func_02004d9c(leader, 40);
  FieldEvent_Wait(40);
  Event_PlaySound(204);
  leader->motionFlags = 3;
  leader->gravity = 0x547a;
  for (value = 0; value < 180; value++) {
    System_Wait(1);
    if (leader->y <= leader->terrainHeight)
      break;
  }
  Event_PlaySound(188);
  work = &gMainRoutineWork;
  leader->gravity = gravity;
  Actor_SetAttachedEffect(work->currentOwner, 0);
  Func_02004d9c(leader, 38);
  MapGrid_SetShake(0x10000, 0x40000, 0x10000);
  System_Wait(10);
  MapGrid_SetShake(-1, -1, 0xe666);
  System_Wait(20);
  FieldCamera_MoveTo(leader->x, leader->y, leader->z, 1);
  Func_02004f54();
  FieldEvent_Wait(20);
  Actor_SetSingleChildFlags(leader, 1);
  Func_02004d9c(leader, 1);
  leader->velocityY = 0x40000;
  System_Wait(20);
  work->renderMode = 0;
  Func_02004e64();
  Event_SetFlag(flag);
}
