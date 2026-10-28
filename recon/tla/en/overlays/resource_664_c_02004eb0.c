// Draft, differ 64 of 64: creates eight objects of type 672, each with its
// sprite at priority 1, a phase of 16 times its number in byte 0x63, a
// speed of 0x8000 plus 0x1000 times its number and Func_02000550 as its
// update; odd ones take action 212, even ones 116. Left: registers (the
// native keeps the count in r6 and a zero in r7) and the native takes both
// action field addresses and counts up before the parity test.
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

struct ObjectRuntime *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Animation_SetStateFlags(struct ObjectRuntime *object, s32 flags);
void Func_02000550(void);

void Func_02004eb0(void) {
  struct ObjectRuntime *object;
  s32 i = 0;
  struct FieldSprite *sprite;

loop:
  object = Import_ActorCreate(672, 0, 0, 0);
  sprite = object->animation;
  sprite->priority = 1;
  ((u8 *)object)[0x63] = i << 4;
  object->motionFlags = 0;
  object->priorityFlags = 0;
  object->speedLimit = object->acceleration = (i << 12) + 0x8000;
  object->update = Func_02000550;
  ObjectDispatch_SetSingleChildFlags(object, 0);
  Object_SetPartAttribute(object, 2);
  if (i & 1) {
    object->action = 212;
    object->unk66 = 0;
  } else {
    object->action = 116;
    object->unk66 = 0;
  }
  i++;
  Animation_SetStateFlags(object, 1);
  if (i != 8)
    goto loop;
}
