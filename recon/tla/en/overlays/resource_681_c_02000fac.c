// Draft, differ 16 of 54: restores person 10 as Func_02000f00 saved it, moving
// it back when it was pushed, and sets person 9 walking again.
#include "types.h"
#include "object_runtime.h"
#include "iwram_call.h"

extern struct ObjectRuntime gData_0200646c;
extern s32 gData_020064ec;
extern s32 gData_020064f0;
extern s32 Data_020064f4;
extern s32 gData_020064f8;
extern s32 gData_020055c0[];
extern s32 gData_0200559c[];

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_ResetAndSetPositionInMode2(u32 objectId, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(u32 objectId);
void ObjectDispatch_SetAnimationSpeed(struct ObjectRuntime *object, s32 value);
void Object_SetModeById(u32 id, s32 mode);

void Func_02000fac(void) {
  struct ObjectRuntime *walker = Object_GetById(9);
  struct ObjectRuntime *object = Object_GetById(10);

  if (gData_020064ec != object->x || gData_020064f0 != object->z) {
    ObjectMotion_ResetAndSetPositionInMode2(10, gData_020064ec >> 16,
                                            ((s16 *)&gData_020064f0)[1]);
    ObjectMotion_CommitCurrentPositionAndActivate(10);
  }
  if (walker->script == gData_020055c0 || walker->script == gData_0200559c) {
    walker->movementState = 0;
    ObjectDispatch_SetAnimationSpeed(walker, 16);
  }
  Iwram_CopyWords(object, &gData_0200646c, 128);
  Object_SetModeById(10, Data_020064f4);
  ObjectDispatch_SetAnimationSpeed(object, gData_020064f8);
}
