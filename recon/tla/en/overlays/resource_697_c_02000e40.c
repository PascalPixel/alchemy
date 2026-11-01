// Matches: differ 0 (tla-en, 2026-11-01)
// Pushes a Trial Road object by (dx, dz) cells when the leader stands right
// behind it (else the event work's byte 388 is set to 13): it slides up to six
// cells until a cell of its width (across axis) is blocked (Func_02000df8), the
// leader pushing (mode 8, sound 239) and following; stopping short of column 18
// it settles (sounds 298, 213), past it it drops off the edge to x 312, takes
// its flag (id + 579) and sounds 298 and 240.
// Two barriers carry the match; both emit nothing. Plain source ranks toZ above
// object and n above dx and dz, against the listing's object > toZ > toX > dx >
// dz > n. All the refs the listing shows for dx, dz and object are already
// there, so the source's extra refs are still unknown. Tried without success:
// a goto-built loop, a second n = 0 (n's live length doubles again, but its
// eighth ref outweighs that), and no copy of fromZ to toZ beside its set (toZ
// falls to 8 refs but still outranks object). The frame's top 12 bytes are the
// unused local.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"
#include "vector3.h"

struct EventWork184 {
  u8 unk000[388];
  s16 value;
};

void Func_02005880(void);
void Func_02005888(void);
void Func_020059e8(s32 value);
void Func_020057f8(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02005800(struct Actor *object);
s32 Func_02000df8(s32 x, s32 z);
void FieldEvent_Wait(s32 frames);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Func_02000d70(void);

void Func_02000e40(s32 id, s32 dx, s32 dz, s32 axis) {
  struct Actor *object = Actor_GetById(id);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 fromX = object->x >> 20;
  s32 fromZ = object->z >> 20;
  s32 toX = fromX;
  s32 toZ = fromZ;
  s32 n;
  s32 mode = 2;
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;
  // Unused: gives the stack frame its 12 bytes.
  struct Vector3 unused;


  if ((dx != 0 && toX != x + dx) || (dz != 0 && toZ != z + dz)) {
    ((struct EventWork184 *)Ram_HeapSlots->eventWork)->value = 13;
    return;
  }
  for (n = 0; n <= 5;) {
    x = toX + dx;
    z = toZ + dz;
    if (axis == 0) {
      if (Func_02000df8(x, z - 1))
        break;
      if (Func_02000df8(x, z))
        break;
      if (Func_02000df8(x, z + 1))
        break;
    } else {
      if (Func_02000df8(x - 1, z))
        break;
      if (Func_02000df8(x, z))
        break;
      if (Func_02000df8(x + 1, z))
        break;
    }
    toX = x;
    n++;
    toZ = z;
  }
  if (fromX == toX && fromZ == toZ)
    return;
  Func_02005880();
  /* FAKEMATCH(barrier): the empty asm reading dx, dz and object, which ranks them above n and toZ; as no asm, 162 of 240 instructions differ in tla-en. */
  asm("" : : "g"(dx), "g"(dz), "g"(object));
  Func_020059e8(0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  FieldEvent_Wait(6);
  object->speedLimit = 0x8000;
  object->acceleration = 0x3333;
  Func_02005a68(239);
  if (dx < 0 || dz < 0)
    mode = 3;
  Func_020057c8(object, mode);
  Func_020057f8(object, (toX << 20) + 0x80000, 0,
                z = (toZ << 20) + 0x80000);
  FieldEvent_Wait(6);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x4ccc, 0x3333);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, dx << 3,
                                            dz << 3);
  FieldEvent_Wait(24);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Func_02005800(object);
  if (toX <= 17) {
    Func_020057c8(object, 1);
    Func_02005a68(298);
    Func_02005a68(213);
  } else {
    Func_020057c8(object, 3);
    Func_020057f8(object, 312 << 16, 0, z);
    FieldEvent_Wait(30);
    Func_020057c8(object, 8);
    Func_02005800(object);
    {
      u8 *flags = &object->priorityFlags;
      s32 value;

      /* FAKEMATCH(barrier): the empty asm passing flags through, clobbering r0 and r2, which puts flags in r3 and the store after the sound id; as no asm, 8 of 240 instructions differ in tla-en. */
      asm("" : "=r"(flags) : "0"(flags) : "r0", "r2");
      value = 2;
      *flags = value;
    }
    Func_02005a68(298);
    Func_02005a68(240);
    GameFlag_SetBit(id + 579);
  }
  FieldEvent_Wait(15);
  Func_02005888();
  Func_02000d70();
}
