// Near miss: differ 6 of 40: the native reloads x for the saved copy before
// it doubles the sine, and stores it before the saved z; the draft reloads
// and stores it after. The twin of ☀️'s SceneEffect_UpdateOrbitAroundActor.
#include "../../../../games/tla/src/scene/mars_aerie/mars_aerie.h"

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

void Func_02003210(struct ObjectRuntime *object) {
  struct ObjectRuntime *center = Object_GetById(23);
  u16 *pangle = (u16 *)&object->action;
  s32 angle = *pangle;
  s32 across;

  object->x = center->x + Math_Cosine(angle) * (object->speedLimit + 3);
  across = center->z + (Math_Sine(angle) << 1);
  object->z = across;
  object->targetX = object->x;
  object->targetZ = across;
  *pangle -= 0x800;
}
