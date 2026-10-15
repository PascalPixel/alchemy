#include "types.h"
#include "field_obj.h"

static s32 SceneActor_UpdateScalePulse(struct FieldActor *state) {
  switch (state->timer) {
  case 6:
    state->scaleX -= 0x4000;
    state->scaleY += 0x2000;
    break;
  case 4:
    state->scaleX += 0x2000;
    state->scaleY += -0x1000;
    break;
  case 2:
    state->scaleX += 0x1000;
    state->scaleY -= 0x800;
    break;
  case 0:
    state->scaleX = 0x10000;
    state->scaleY = 0x10000;
    state->timer = (Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60);
    break;
  }
  state->timer--;
  return 1;
}
