// Draft, differ 12 of 65: saves person 10 (a copy of the object, its mode, its
// animation and its place) and stops it where it stands; person 9 halts too
// when it runs one of its two walking scripts. Func_02000fac restores it.
// The position is read once into x and z before the copies, and the targets
// are set x, y, z (from 24). Left: the store-then-load rows: after each store
// the native issues a pool load or a store before the next load from the
// objects, where this draft loads at once.
#include "types.h"
#include "object_runtime.h"
#include "iwram_call.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern struct ObjectRuntime gData_0200646c;
extern s32 gData_020064ec;
extern s32 gData_020064f0;
extern s32 Data_020064f4;
extern s32 gData_020064f8;
extern s32 gData_020055c0[];
extern s32 gData_0200559c[];

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_ResetTargetsAndVelocity(u32 objectId);
void ObjectDispatch_SetAnimationSpeed(struct ObjectRuntime *object, s32 value);

void Func_02000f00(void) {
  struct ObjectRuntime *walker = Object_GetById(9);
  struct ObjectRuntime *object = Object_GetById(10);
  u8 *sprite = object->animation;
  u8 *animation = FIELD_AT(sprite, u8 *, 40);

  Iwram_CopyWords(&gData_0200646c, object, 128);
  gData_020064f8 = animation[21];
  Data_020064f4 = sprite[24];
  {
    s32 x = object->x;
    s32 z = object->z;

    gData_020064ec = x;
    object->update = 0;
    gData_020064f0 = z;
    object->velocityZ = 0;
    object->velocityY = 0;
    object->velocityX = 0;
    object->targetX = x;
    object->targetY = object->y;
    object->targetZ = z;
  }
  ObjectMotion_ResetTargetsAndVelocity(10);
  ObjectDispatch_SetAnimationSpeed(object, 10);
  object->speedLimit = 0x10000;
  if (walker->script == gData_020055c0 || walker->script == gData_0200559c) {
    walker->movementState = 1;
    ObjectDispatch_SetAnimationSpeed(walker, 0);
  }
}
