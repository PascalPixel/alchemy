#include "types.h"
#include "actor.h"
#include "event_inline.h"
#include "field.h"

extern const u8 gData_02004744[];

void Func_020041cc(struct Actor *object, s32 mode);
void Func_020041dc(struct Actor *object, const void *script);
void Func_020015ec(void);

void Func_02001750(struct Actor *object) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSingleChildFlags(object, 0);
  Func_020041dc(object, gData_02004744);
  object->action = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

void Func_02001780(s32 *position, s32 speed, s32 flag) {
  struct Actor *object = FieldActor_Create(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    ((void (*)())Func_02001750)(object, 2);
    object->update = Func_020015ec;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_020041cc(object, 2);
  }
}

#include "../../../../games/tla/src/scene/mars_lighthouse_entrance/room.h"
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

s32 Func_02001528(struct Actor *a, struct Actor *b);
s32 Func_0200158c(struct Actor *a, struct Actor *b);

void Func_020017c8(struct Actor *object) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct Actor *other = Actor_Get(gMainRoutineWork.currentOwner);
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
  if (other->y == other->terrainHeight && Func_02001528(object, other) &&
      gMainRoutineWork.renderMode == 0) {
    if (object->unk4c != 0) {
      work->direction.unk02 = 0;
      direction = &work->direction;
      if (object->unk4c > 0)
        direction->unk00 = 1;
      else
        direction->unk00 = 0xffff;
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
    other = Actor_Get(id);
    if (other->y == other->terrainHeight && Func_0200158c(object, other)) {
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

void Func_02001928(struct Actor *object, s32 priority);

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_02004744; Func_02001e1c of the Mars Lighthouse entrance is the same
/// function.
void Func_02001928(struct Actor *object, s32 part) {
  /* FAKEMATCH(temporary): one zero for every cleared field; as six stores of
   * 0, 29 of 30 instructions differ in tla-en. */
  s32 zero = 0;

  object->motionFlags = zero;
  object->collisionFlags = zero;

  ((struct SpriteParts *)object->animation)->part = part;
  Actor_SetSingleChildFlags(object, 0);
  Func_020041dc(object, gData_02004744);
  object->action = zero;
  object->speedLimit = zero;
  object->acceleration = zero;
}

void Func_0200196c(s32 *position, s32 speed, s32 flag) {
  struct Actor *object = FieldActor_Create(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -speed;
    Func_02001928(object, 3);
    object->update = Func_020015ec;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_020041cc(object, 2);
  }
}
