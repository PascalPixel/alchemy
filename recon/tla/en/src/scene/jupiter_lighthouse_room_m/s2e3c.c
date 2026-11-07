#include "types.h"
#include "field_event.h"
#include "call.h"
#include "scene_tables.h"

void Scene_SetArrivalFlags();
void Func_02002fc4();
void Func_02003a38();
void Func_02003ac0();
struct FieldActor *Actor_GetById();
s32 GameFlag_Test();
void Func_02004844();
void Func_0200474c();
void Func_020049ac();
void Func_02004774(s32 x, s32 z, s32 a, s32 b);

/// The scene's map preparation.
s32 Func_02002e3c(void) {
  struct FieldActor *object;

  Scene_SetArrivalFlags(0);
  Func_02002fc4(1);
  Func_02003a38();
  Func_02003ac0(20, 0);
  object = Actor_GetById(20);
  if (GameFlag_Test(0xa0c)) {
    Call3(Func_02004844, 20, PIXELS(1016), PIXELS(248));
    Call6(Func_0200474c, 104, 42, 5, 11, 61, 42);
    Call6(Func_0200474c, 106, 52, 1, 1, 63, 84);
    Func_020049ac(0x4100000, 0xa00000, 2, 236);
  }
  object->motionFlags = 0;
  {
    s32 zero = 0;

    /* FAKEMATCH(barrier): the last call's zero is built before the position
     * loads; as a plain 0 argument, 2 of 62 instructions differ in tla-en. */
    __asm__("" : : "r"(zero));
    Func_02004774(object->x, object->z, zero, -6);
  }
  return 0;
}
