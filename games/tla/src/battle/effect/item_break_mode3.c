#include "types.h"
#include "system.h"
#include "object_runtime.h"
#include "field_sprite.h"

struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);

/// Spawns object 278 and grows it from a quarter to full size, an eighth of a
/// turn a frame, then faces it to angle.
struct ObjectRuntime *BattleFx_SpawnItemBreakMode3(s32 x, s32 y, s32 z,
                                                   s32 angle) {
  struct ObjectRuntime *object;
  s32 scale;
  /* FAKEMATCH: the limit's first byte is pinned to the r2 the native builds it
   * in; as a free temporary it takes r3 and the scale r2 (differ 30). */
  register s32 limit asm("r2");

  Audio_PlayCue(138);
  object = Object_Spawn(278, x, y, z);
  if (object != NULL) {
    object->scaleX = object->scaleY = 0x4000;
    object->acceleration = object->speedLimit = 0x30000;
    /* FAKEMATCH: ⚓️ stores both speeds before it loads the sprite; without the
     * barrier the load is scheduled between them. */
    asm volatile("");
    ((struct FieldSprite *)object->animation)->priority = 0;
    Object_SetMode(object, 3);
    scale = object->scaleX;
    /* FAKEMATCH: ⚓️ loads the scale before it builds the limit; the plain test
     * starts the constant first (one pair of instructions swapped). */
    asm volatile("");
    if (scale < 0x10000) {
      do {
        scale += 0x800;
        object->scaleY = scale;
        object->scaleX = scale;
        object->angle += 0x2000;
        WaitFrames(1);
        limit = 255;
        scale = object->scaleX;
        /* FAKEMATCH: the native starts the 0xffff limit, loads the scale, then
         * finishes the limit; as a constant the load comes after its shift
         * (differ 2). */
        asm volatile("" : "+l"(limit));
      } while (scale <= (limit << 8) + 255);
    }
    object->angle = angle;
  }
  return object;
}
