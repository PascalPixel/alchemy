// Near miss: differ 2 of 48 (one reordered row): the native copies the anchor
// to r1 (the reload for anchor->x) before it adds 6 to the radius; here sched2
// gives the add 81 and the multiply 80 against the copy's 79, so the copy
// follows the multiply. The twin of ☀️'s SceneEffect_UpdateCounterDrivenOrbit;
// its form verbatim (byte offsets, the angle through a block local) also
// differs by 2. Tried: the sum and product in either order (9), the radius as
// a block local, the anchor through a one-pass block (4), an asm input operand
// on the anchor (45), three minutes of alchemy permute (17,543 candidates).
#include "../../../../games/tla/src/scene/mars_lighthouse_aerie/mars_lighthouse_aerie.h"

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

void Func_020000e4(struct ObjectRuntime *object) {
  struct ObjectRuntime *anchor = Object_GetById(8);
  u16 *pangle = (u16 *)&object->action;
  s32 angle = *pangle;
  s32 cosine;
  s32 sine;
  s32 along;
  s32 across;

  cosine = Math_Cosine(angle);
  along = anchor->x + cosine * (object->speedLimit + object->unk5c[6] + 6);
  object->x = along;
  sine = Math_Sine(angle);
  across = anchor->z + sine * (object->unk5c[6] + 4);
  object->z = across;
  object->targetX = object->x;
  object->targetZ = across;
  *pangle -= 0x800;
}
