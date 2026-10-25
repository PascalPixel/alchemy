#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);

struct ActorSprite {
  u8 unk00[18];
  u16 rotation;
};

void Func_02002f88(s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);

  ObjectDispatch_SetSingleChildFlags(Object_GetById(id), 0);
  object->angle = 0x4000;
  object->motionFlags = 0;
  {
    s32 x = object->x;

    asm("" : "+l"(x));
    object->x = x + 0xc0000;
  }
  object->y = 0x100000;
  object->collisionFlags = 0;
  {
    register struct ActorSprite *sprite asm("r2") =
        (struct ActorSprite *)object->animation;

    asm("" : "+l"(sprite));
    sprite->rotation = 0xc000;
  }
}
