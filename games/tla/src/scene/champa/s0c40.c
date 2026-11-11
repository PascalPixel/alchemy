#include "types.h"
#include "field_event.h"
#include "event_inline.h"

extern const u8 gData_02003620[];

void Func_020029dc(struct FieldActor *object, s32 mode);
void Func_020029e4(struct FieldActor *object, const void *script);

void Func_02000c40(s32 x, s32 y, s32 z, s32 angle, s32 frames) {
  s16 value = angle;
  struct FieldActor *object;
  struct FieldSprite *sprite;

  object = FieldActor_Create(0x1e8, x, y, z);
  if (object != 0) {
    sprite = object->sprite;
    Func_020029dc(object, 1);
    Func_020029e4(object, gData_02003620);
    object->motionFlags = 0;
    ((struct {
      u8 unk00[94];
      u16 frames;
    } *)object)
        ->frames = frames;

    sprite->nextFrame = 0;
    sprite->rotation = value;
  }
}
