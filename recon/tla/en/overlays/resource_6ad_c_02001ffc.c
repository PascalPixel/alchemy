// Draft, differ 28 of 167, skeleton gap 4 (tla-en, 2026-11-01)
// The rolling ride object's step with its axes swapped from Func_02001e20:
// bounce drives z and unk4c drives x. Every instruction is present; what is
// left is order and the reload registers. Settled forms, each measured:
// one actor variable for the leader and the slots' actors is what makes
// caller-save take r4 (saved around __divsi3), and with it the native's r8, r9
// and r10; the loop indexes work->slots[i], so strength reduction builds the
// pointer in r5 and the first id loads as ldrh [r8, r3]; each branch stores
// its own direction through a set of an s32, which stops if-conversion
// hoisting the 1 and lets cross-jumping share the strh.
// Left: pool loads at the top, where the native loads gMainRoutineWork into r3
// before the ride work (a local for its address, or for the owner, gets that
// order but moves other things, gap 5); sched2 keeps the 0 and its store to
// unk4c late on landing, where the native has them before the z update (the 0
// is the mask of the priorityFlags store, BYTE_AT moves it but no better; a
// union access to unk4c moves the store, so it is alias-sensitive, but that
// form is not source); and reload takes r1 for r8 at work->landed and
// work->blocked where the native takes r2, which also swaps the ldrh of
// value0a with the movs before it. Tried without gain: every order of the
// landing statements, a z local, the landed store through U16_AT, s16 event
// store, u16 id (gap 8/20), value0a in a local.
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
s32 Func_02001d5c(struct Actor *a, struct Actor *b);
s32 Func_02001dc0(struct Actor *a, struct Actor *b);

void Func_02001ffc(struct Actor *object) {
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
  if (actor->y == actor->terrainHeight && Func_02001d5c(object, actor) &&
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
