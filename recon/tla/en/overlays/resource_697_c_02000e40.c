// Draft: matched under -O2, back on its scaffold when the overlays moved to -Os (2026-11-02).
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
  Func_020059e8(0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  FieldEvent_Wait(6);
  object->speedLimit = 0x8000;
  object->acceleration = 0x3333;
  Func_02005a68(239);
  if (dx < 0 || dz < 0)
    mode = 3;
  Func_020057c8(object, mode);
  Func_020057f8(object, (toX << 20) + 0x80000, 0, z = (toZ << 20) + 0x80000);
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
      s32 value = 2;

      object->priorityFlags = value;
    }
    Func_02005a68(298);
    Func_02005a68(240);
    GameFlag_SetBit(id + 579);
  }
  FieldEvent_Wait(15);
  Func_02005888();
  Func_02000d70();
}
