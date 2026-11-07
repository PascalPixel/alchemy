#include "types.h"
#include "main_routine_work.h"
#include "scene_tables.h"

struct JupiterLighthouseAerieObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct JupiterLighthouseAerieObject *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void FieldEvent_Wait(s32 frames);
void Actor_SetModeById(s32 actor, s32 mode);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Camera_FollowActor(s32 actor, s32 target);
void Func_02000054(s32 step);
void Func_02001070(struct JupiterLighthouseAerieObject *object);
void Func_020017a8(s32 a, s32 b, s32 c);
void Func_02002a78(s32 x, s32 z, s32 a, s32 b);
void Func_02002a88(void);
void Func_02002a90(void);
void Func_02002af0(void);
void Func_02002b18(s32 skip);

void Func_020001b4(void) {
  GameFlag_SetBit(0xa0a);
  Func_02002a88();
  Func_02002b18(0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Func_020017a8(11, 9, 0);
  FieldEvent_Wait(40);
  if (GameFlag_Test(0xa0b) == 0) {
    Func_02000054(0);
    FieldEvent_Wait(40);
  } else {
    Func_02000054(2);
  }
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldEvent_Wait(40);
  Func_02001070(Actor_GetById(9));
  Func_02001070(Actor_GetById(10));
  Func_02001070(Actor_GetById(11));
  Func_02001070(Actor_GetById(12));
  Func_02002af0();
  Func_02002a90();
}

void Func_02000264(void) {
  GameFlag_SetBit(0xa0b);
  Func_02002a88();
  Func_02002b18(0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Func_020017a8(12, 10, 1);
  FieldEvent_Wait(40);
  if (GameFlag_Test(0xa0a) == 0) {
    Func_02000054(1);
    FieldEvent_Wait(40);
  } else {
    Func_02000054(3);
  }
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldEvent_Wait(40);
  Func_02001070(Actor_GetById(9));
  Func_02001070(Actor_GetById(10));
  Func_02001070(Actor_GetById(11));
  Func_02001070(Actor_GetById(12));
  Func_02002af0();
  Func_02002a90();
}

void Func_02000314(s32 unused, s32 actor) {
  struct JupiterLighthouseAerieObject *object = Actor_GetById(actor);

  Func_02002a78(object->x, object->z, 0, 4);
}
