// Draft of Func_02002594 (overlay 64B, 53 instructions), differ 20: spawns
// object 285 at a random spot with a random scale. Written from the listing;
// the cells x and z held without their bases until the call let the second
// Random16Far call move ahead of x's shifts as in the native. Left: the z
// product's subs and shifts sit one row off, and the sprite byte 9 load,
// nextFrame and motionFlags stores are ordered differently (the native stores
// nextFrame and motionFlags together from one zero in r1, before the scales;
// here nextFrame sinks below them). Measured: nextFrame as a byte through a u8
// pointer, both as one chained assignment, both through u8 pointers (differ 20
// to 23).
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

extern const u8 gData_02006bc8[];

u32 Random16Far(void);
struct ObjectRuntime *Func_02004b30(s32 kind, s32 x, s32 y, s32 z);
void Func_02004b18(struct ObjectRuntime *object, s32 mode);
void Func_02004b20(struct ObjectRuntime *object, const u8 *script);

void Func_02002594(void) {
  struct ObjectRuntime *obj;
  struct FieldSprite *sprite;
  s32 x;
  s32 z;
  s32 scale;

  x = ((Random16Far() * 40) >> 16) << 16;
  z = ((Random16Far() * 30) >> 16) << 16;
  obj = Func_02004b30(285, x + 0x7b80000, 0, z + 0x1c5c0000);
  if (obj != NULL) {
    sprite = obj->animation;
    scale = ((Random16Far() << 15) >> 16) + 0x13333;
    sprite->priority = 2;
    sprite->nextFrame = 0;
    obj->motionFlags = 0;
    obj->scaleX = scale;
    obj->scaleY = scale;
    Func_02004b18(obj, 1);
    Func_02004b20(obj, gData_02006bc8);
  }
}
