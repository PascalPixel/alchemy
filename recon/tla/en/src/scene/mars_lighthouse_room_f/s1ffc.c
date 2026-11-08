#include "../../../../games/tla/src/scene/mars_lighthouse_entrance/room.h"
#include "call.h"
#include "field/event_work.h"

struct RideSlot {
  u16 id;
  u16 blocked;
};

struct RideDirection {
  u16 unk00;
  u16 unk02;
};

struct RideWork {
  u8 unk00[4];
  u16 landed;
  u16 blocked;
  u8 unk08[2];
  u16 value0a;
  u8 unk0c[0x48];
  struct RideSlot slots[4];
  u8 unk64[4];
  struct RideDirection direction;
};

extern u8 gData_020023c4[];

struct Actor *Actor_GetById(s32 id);
s32 Func_02001d5c(struct Actor *a, struct Actor *b);
s32 Func_02001dc0(struct Actor *a, struct Actor *b);

void Func_02001ffc(struct Actor *object) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct Actor *other = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 dx = object->bounce;
  s32 dz;
  s32 i;
  struct RideDirection *direction;

  object->z += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->x += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct SpriteParts *)object->animation)->unk12 += object->action;
  if (other->y == other->terrainHeight && Func_02001d5c(object, other) &&
      gMainRoutineWork.renderMode == 0) {
    if (object->unk4c != 0) {
      work->direction.unk02 = 0;
      direction = &work->direction;
      if (object->unk4c > 0)
        direction->unk00 = 1;
      else
        direction->unk00 = SPLIT_CONSTANT(0xffff);
    }
    object->priorityFlags = 2;
    object->z -= object->bounce;
    object->gravity += object->bounce >> 2;
    object->unk4c = 0;
    work->landed = 1;
    event->exit = work->value0a;
  }
  for (i = 0; i < 4; i++) {
    s32 id = work->slots[i].id;

    if (id == 0)
      break;
    other = Actor_GetById(id);
    if (other->y == other->terrainHeight && Func_02001dc0(object, other)) {
      if (work->slots[i].blocked == 0) {
        object->priorityFlags = 2;
        object->z -= object->unk4c;
        object->gravity += object->unk4c >> 2;
        object->unk4c = 0;
      } else {
        work->blocked = 1;
      }
    }
  }
}
