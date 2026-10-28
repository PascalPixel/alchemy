// Draft of overlay 6ad's Func_02001e20, the twin of overlay 6aa's
// Func_02001ae0: its draft (resource_6aa_c_02001ae0.c) with this overlay's
// symbols: differ 95 of 168. Left: what that draft's header records.
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
  u16 value68;
  u16 direction;
};

extern u8 gData_020023c4[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 Func_02001d5c(struct ObjectRuntime *a, struct ObjectRuntime *b);
s32 Func_02001dc0(struct ObjectRuntime *a, struct ObjectRuntime *b);

void Func_02001e20(struct ObjectRuntime *object) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  u8 *event = Ram_HeapSlots->eventWork;
  s32 dx = object->bounce;
  s32 dz;
  s32 i;
  s32 direction;
  struct RideSlot *slot;

  object->x += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->z += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct SpriteParts *)object->animation)->unk12 += object->action;
  if (leader->y == leader->terrainHeight && Func_02001d5c(object, leader) &&
      gAdventure.renderMode == 0) {
    if (object->unk4c != 0) {
      work->value68 = 0;
      {
        u16 *cell = &work->direction;

        if (object->unk4c > 0)
          direction = 1;
        else
          direction = 0xffff;
        *cell = direction;
      }
    }
    object->priorityFlags = 2;
    {
      s32 speed = object->unk4c;

      object->unk4c = 0;
      object->z -= speed;
      object->gravity += speed >> 2;
    }
    work->landed = 1;
    *(u16 *)(event + 340) = work->value0a;
  }
  slot = work->slots;
  for (i = 0; i < 4 && slot->id != 0; i++, slot++) {
    struct ObjectRuntime *other = Object_GetById(slot->id);

    if (other->y == other->terrainHeight && Func_02001dc0(object, other)) {
      if (slot->blocked == 0) {
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
