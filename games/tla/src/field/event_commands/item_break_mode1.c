#include "types.h"
#include "system.h"
#include "actor.h"
#include "fixed_math.h"

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);

/// Spawns object 278 and grows it from a quarter to full size, an eighth of a
/// turn a frame, then faces it to angle.
struct Actor *BattleEffect_SpawnItemBreakMode1(s32 x, s32 y, s32 z,
                                                   s32 angle) {
  struct Actor *object;
  s32 scale;

  Sound_PlayFar(138);
  object = Actor_Spawn(278, x, y, z);
  if (object != NULL) {
    object->scaleY = 0x4000;
    object->scaleX = 0x4000;
    object->speedLimit = 0x30000;
    object->acceleration = 0x30000;
    object->actionFlags = 0;
    Actor_SetMode(object, 1);
    scale = object->scaleX;
    if (scale < Q16_ONE) {
      do {
        scale += 0x800;
        object->scaleY = scale;
        object->scaleX = scale;
        object->angle += 0x2000;
        wait(1);
        scale = object->scaleX;
      } while (scale <= 0xffff);
    }
    object->angle = angle;
  }
  return object;
}
