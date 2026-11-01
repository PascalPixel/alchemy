// Draft, differ 4 of 168, skeleton gap 2 (tla-en, 2026-11-01)
// Overlay 6ad's rolling ride object's step, the twin of 6aa's Func_02001ae0
// (6ab and 6ac hold copies). Every instruction is present and the registers
// agree; what is left is one pair on landing, in sched2: the native has the 0
// for unk4c before the two loads and its store straight after them, here both
// come later. Both loads have priority 13 and the 0 priority 2, so the 0 can
// only go first if the loads wait a cycle after the priorityFlags store, and
// -mthumb-slow-loads gives no wait to a load whose address a later store in
// the block writes (thumb_load_stored_back_p), which is true of unk4c and z.
// The loop's copy shows nothing, as its 0 is already in r1. That is a compiler
// question, not a source one; no C form reaches it.
// Settled forms, each measured: one actor variable for the leader and the
// slots' actors gives caller-save r4 around __divsi3 and with it r8, r9, r10;
// work->slots[i] lets strength reduction build r5 and loads the first id as
// ldrh [r8, r3]; each branch stores its own direction through a set of an s32;
// event->exit through struct EventWork, not a cast of a byte or halfword
// pointer (differ 28), fixes the pool order at the top and the reload registers
// at work->landed and work->blocked.
// Tried without gain: the landing store as a union member (gap 8) or bit-field,
// value0a likewise, a zero local, every order of the landing statements, a
// struct for the exit store with no char member (no dependence, same order).
#include "../../../../games/tla/src/scene/mars_lighthouse_entrance/room.h"
#include "field/event_work.h"

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

struct Actor *Actor_GetById(s32 id);
s32 Func_02001d5c(struct Actor *a, struct Actor *b);
s32 Func_02001dc0(struct Actor *a, struct Actor *b);

void Func_02001e20(struct Actor *object) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 dx = object->bounce;
  s32 dz;
  s32 i;
  s32 direction;
  s32 id;

  object->x += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->z += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct SpriteParts *)object->animation)->unk12 += object->action;
  if (actor->y == actor->terrainHeight && Func_02001d5c(object, actor) &&
      gMainRoutineWork.renderMode == 0) {
    if (object->unk4c != 0) {
      work->value68 = 0;
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
      s32 speed = object->unk4c;

      object->unk4c = 0;
      object->z -= speed;
      object->gravity += speed >> 2;
    }
    work->landed = 1;
    event->exit = work->value0a;
  }
  for (i = 0; i < 4 && (id = work->slots[i].id) != 0; i++) {
    actor = Actor_GetById(id);

    if (actor->y == actor->terrainHeight && Func_02001dc0(object, actor)) {
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
