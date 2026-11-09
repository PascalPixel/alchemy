#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "event_inline.h"

extern const u8 gData_02006bc8[];

u32 Random16Far(void);
void Func_02004b18(struct Actor *object, s32 mode);
void Func_02004b20(struct Actor *object, const u8 *script);

/// Spawns object 285 at a random spot with a random scale and starts its
/// script.
void Func_02002594(void) {
  struct Actor *obj;
  struct FieldSprite *sprite;
  u8 *motionFlags;
  s32 x;
  s32 z;
  s32 scale;

  x = ((Random16Far() * 40) >> 16) << 16;
  z = ((Random16Far() * 30) >> 16) << 16;
  obj = ((struct Actor *)Import_ActorCreate(285, x + 0x7b80000, 0,
                                            z + 0x1c5c0000));
  if (obj != NULL) {
    sprite = obj->animation;
    scale = ((Random16Far() << 15) >> 16) + 0x13333;
    sprite->nextFrame = 0;
    sprite->priority = 2;
    // The flags byte's address in a local: stored directly, 2 of 53 differ.
    motionFlags = &obj->motionFlags;
    *motionFlags = 0;
    obj->scaleX = scale;
    obj->scaleY = scale;
    Func_02004b18(obj, 1);
    Func_02004b20(obj, gData_02006bc8);
  }
}
