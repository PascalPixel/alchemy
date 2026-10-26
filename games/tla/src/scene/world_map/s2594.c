#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

extern const u8 gData_02006bc8[];

u32 Random16Far(void);
struct ObjectRuntime *Func_02004b30(s32 kind, s32 x, s32 y, s32 z);
void Func_02004b18(struct ObjectRuntime *object, s32 mode);
void Func_02004b20(struct ObjectRuntime *object, const u8 *script);

/// Spawns object 285 at a random spot with a random scale and starts its
/// script.
void Func_02002594(void) {
  struct ObjectRuntime *obj;
  struct FieldSprite *sprite;
  s32 x;
  s32 z;
  s32 zero;
  s32 scale;

  x = ((Random16Far() * 40) >> 16) << 16;
  z = ((Random16Far() * 30) >> 16) << 16;
  obj = Func_02004b30(285, x + 0x7b80000, 0, z + 0x1c5c0000);
  if (obj != NULL) {
    /* FAKEMATCH(one-pass): the sprite setup kept in a once-only block; as
     * plain statements, 2 of 53 instructions differ in tla-en. */
    do {
      sprite = obj->animation;
      scale = ((Random16Far() << 15) >> 16) + 0x13333;
      /* FAKEMATCH(temporary): the zero both flag bytes take; as a constant 0
       * in the nextFrame store, 10 of 53 instructions differ in tla-en. */
      zero = 0;
      sprite->priority = 2;
      sprite->nextFrame = zero;
      obj->motionFlags = 0;
    } while (0);
    obj->scaleX = scale;
    obj->scaleY = scale;
    Func_02004b18(obj, 1);
    Func_02004b20(obj, gData_02006bc8);
  }
}
