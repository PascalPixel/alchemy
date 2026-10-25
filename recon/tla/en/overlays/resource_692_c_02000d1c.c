// Draft, differ 2 of 60: an object's update that, every fourth frame, spawns a
// small effect (type 403, its own script) at a random spot along the object's
// z, moving at a random z velocity; every sixteenth frame it also
// plays sound 136 at the object.
#include "types.h"
#include "object_runtime.h"
#include "frame.h"
#include "field_effect.h"

extern const s32 gData_0200502c[];

void Func_02004f28(struct ObjectRuntime *object, s32 sound);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_02000d1c(struct ObjectRuntime *object) {
  struct EffectOptions options;
  u32 frame = gFrameCount;
  s32 dz;
  s32 vz;

  if ((frame & 3) != 0)
    return;
  options.type = 403;
  options.priority = 2;
  options.script = gData_0200502c;
  if ((frame & 15) == 0)
    Func_02004f28(object, 136);
  dz = Random16Far() & 15;
  vz = ((Random16Far() & 31) - 16) << 12;
  Func_0200015c(object->x, object->y, object->z + ((dz - 8) << 15), 0, 0, vz,
                0x320001, &options);
}
