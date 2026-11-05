// Draft, differ 24: overlay 6aa's copy of the function drafted in
// resource_6ad_c_02001ffc.c, whose header says what it is and what still differs.
#include "../../../../games/tla/src/scene/mars_lighthouse_entrance/room.h"

struct RideSlot {
  u16 id;
  u16 blocked;
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
  u16 direction;
  u16 value6a;
};

extern u8 gData_020023c4[];

struct Actor *Actor_GetById(s32 id);
s32 Func_02001a1c(struct Actor *a, struct Actor *b);
s32 Func_02001a80(struct Actor *a, struct Actor *b);

void Func_02001cbc(struct Actor *object) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
  u8 *event = Ram_HeapSlots->eventWork;
  s32 dz = object->bounce;
  s32 dx;
  s32 i;
  s32 id;
  s32 direction;

  object->z += dz;
  object->y += object->gravity;
  dx = object->unk4c;
  object->x += dx;
  object->bounce = dz - dz / 18;
  object->unk4c = dx - dx / 16;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct SpriteParts *)object->animation)->unk12 += object->action;
  if (actor->y == actor->terrainHeight && Func_02001a1c(object, actor) &&
      gMainRoutineWork.renderMode == 0) {
    if (object->unk4c != 0) {
      work->value6a = 0;
      {
        u16 *cell = &work->direction;

        if (object->unk4c > 0) {
          direction = 1;
          *cell = direction;
        } else {
          direction = 0xffff;
          *cell = direction;
        }
      }
    }
    object->priorityFlags = 2;
    {
      s32 speed = object->bounce;

      object->unk4c = 0;
      object->z -= speed;
      /* FAKEMATCH(barrier): an empty asm holds the order around it; as no asm,
       * 26 of 167 instructions differ in tla-en. */
      asm("");
      object->gravity += speed >> 2;
    }
    work->landed = 1;
    *(u16 *)(event + 340) = work->value0a;
  }
  for (i = 0; i < 4 && (id = work->slots[i].id) != 0; i++) {
    actor = Actor_GetById(id);
    if (actor->y == actor->terrainHeight && Func_02001a80(object, actor)) {
      if (work->slots[i].blocked == 0) {
        s32 speed;

        object->priorityFlags = 2;
        speed = object->unk4c;
        object->unk4c = 0;
        object->z -= speed;
        object->gravity += speed >> 2;
      } else {
        work->blocked = 1;
      }
    }
  }
}
