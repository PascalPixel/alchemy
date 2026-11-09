#include "types.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

struct JupiterLighthouseAerieObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

void Func_02000054(s32 step);
void Func_02001070(struct JupiterLighthouseAerieObject *object);
void Func_020017a8(s32 a, s32 b, s32 c);
void Func_02002a78(s32 x, s32 z, s32 a, s32 b);
void Func_02002a88(void);
void Func_02002a90(void);
void Func_02002af0(void);

void Func_020001b4(void) {
  Event_SetFlag(0xa0a);
  Func_02002a88();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Func_020017a8(11, 9, 0);
  FieldEvent_Wait(40);
  if (Event_GetFlag(0xa0b) == 0) {
    Func_02000054(0);
    FieldEvent_Wait(40);
  } else {
    Func_02000054(2);
  }
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldEvent_Wait(40);
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(9)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(10)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(11)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(12)));
  Func_02002af0();
  Func_02002a90();
}

void Func_02000264(void) {
  Event_SetFlag(0xa0b);
  Func_02002a88();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Func_020017a8(12, 10, 1);
  FieldEvent_Wait(40);
  if (Event_GetFlag(0xa0a) == 0) {
    Func_02000054(1);
    FieldEvent_Wait(40);
  } else {
    Func_02000054(3);
  }
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldEvent_Wait(40);
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(9)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(10)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(11)));
  Func_02001070(((struct JupiterLighthouseAerieObject *)Actor_Get(12)));
  Func_02002af0();
  Func_02002a90();
}

void Func_02000314(s32 unused, s32 actor) {
  struct JupiterLighthouseAerieObject *object =
      ((struct JupiterLighthouseAerieObject *)Actor_Get(actor));

  Func_02002a78(object->x, object->z, 0, 4);
}
