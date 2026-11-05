#include "types.h"
#include "call.h"
#include "actor.h"
#include "field_sprite.h"

struct Actor *Func_02001dc0(s32 kind, s32 x, s32 y, s32 z);
u32 Random16Far(void);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Func_02000244(struct Actor *object);

void Func_02000300(void) {
  struct Actor *object;
  s32 i;
  s32 phase;

  i = 0;
  for (phase = 0; i != 7; i++) {
    object = Func_02001dc0(672, 0, 0, 0);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->speedLimit = (Random16Far() << 2) + 0x60000;
    object->acceleration = ((Random16Far() << 11) >> 16) + 0x800;
    object->update = (void (*)(void))Func_02000244;
    object->action = Random16Far();
    object->unk66 = phase;
    Actor_SetSingleChildFlags(object, 0);
    Actor_SetPartAttribute(object, 2);
    ((struct FieldSprite *)object->animation)->priority = 1;
    phase += 5;
  }
}
