#include "types.h"
#include "scene.h"
#include "motobj.h"
#include "object_runtime.h"

void BattleFx_AdvanceAngleWithRamp(struct ObjectRuntime *object) {
  u32 step;

  step = object->action * 0x50;
  object->angle += step + 0x1000;
  if (step < 0x1000)
    object->action++;
}
