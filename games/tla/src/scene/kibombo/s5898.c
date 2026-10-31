#include "../../../../games/tla/src/scene/kibombo/kibombo.h"
#include "../../../../games/tla/include/scene_tables.h"
#include "edition.h"
#include "fixed_math.h"

void Func_02003288(void);
void Func_02004cf4(void);
void Func_02005c74(s32 actor, s32 angle);

void Func_02005898(void) {
  struct KibomboObject *object;

  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Func_02003288();
  if (gMainRoutineWork.entrance == 9) {
    if (GameFlag_Test(0x8ef)) {
      Func_02005b0c(10, 0, 0);
      if (GameFlag_Test(0x8ee)) {
        /* FAKEMATCH(temporary): the angle in a local of each branch, built
         * before the position; as the constant passed to
         * Kibombo_ObjPlaceFacing, 26 of 105 instructions differ in tla-en. */
        s32 angle = FACING_NORTHWEST + FACING_STEP;

        Kibombo_ObjPlaceFacing(8, PIXELS(434), PIXELS(634), angle);
      } else {
        s32 angle = FACING_NORTHWEST + FACING_STEP;

        Kibombo_ObjPlaceFacing(8, PIXELS(364), PIXELS(576), angle);
      }
    } else {
      Func_02004cf4();
    }
  }
#if (LANGUAGE != LANGUAGE_JAPANESE)
  if (GameFlag_Test(0x109)) {
    object = Actor_GetById(16);
    object->x -= PIXELS(14);
    object->z -= PIXELS(8);
  }
#endif
  Func_02005c74(16, 0);
  object = Actor_GetById(16);
  object->sprite->rotation = 0;
  object->scaleX = Q16_ONE;
  object->scaleY = Q16_ONE;
  object->x += PIXELS(14);
  object->z += PIXELS(8);
  Actor_SetModeById(16, 6);
  if (gMainRoutineWork.entrance == 14)
    GameFlag_SetBit(0x8ee);
}
