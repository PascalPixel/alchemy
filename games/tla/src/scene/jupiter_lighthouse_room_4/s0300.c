#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "event_inline.h"

u32 Random16Far(void);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02000244(struct Actor *object);

void Func_02000300(void) {
  struct Actor *object;
  s32 i;
  s32 phase;

  i = 0;
  for (phase = 0; i != 7; i++) {
    object = (FieldActor_Create(672, 0, 0, 0));
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->speedLimit = (Random16Far() << 2) + 0x60000;
    object->acceleration = ((Random16Far() << 11) >> 16) + 0x800;
    object->update = (void (*)(void))Func_02000244;
    object->action = Random16Far();
    object->unk66 = phase;
    Actor_SetSingleChildFlags(object, 0);
    Actor_SetSpriteColor(object, 2);
    ((struct FieldSprite *)object->animation)->priority = 1;
    phase += 5;
  }
}
