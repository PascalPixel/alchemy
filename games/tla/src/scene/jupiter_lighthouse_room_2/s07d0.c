#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "event_inline.h"

u32 Random16Far(void);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02000720(void);

void Func_020007d0(void) {
  s32 i;
  s32 phase;
  struct Actor *object;

  for (i = 0, phase = 0; i != 22; i++, phase += 5) {
    object = ((struct Actor *)Import_ActorCreate(672, 0, 0, 0));
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->speedLimit = (Random16Far() << 2) + 0x60000;
    object->acceleration = ((Random16Far() << 11) >> 16) + 0x800;
    object->update = Func_02000720;
    object->action = Random16Far();
    object->unk66 = phase;
    Actor_SetSingleChildFlags(object, 0);
    Actor_SetPartAttribute(object, 2);
    ((struct FieldSprite *)object->animation)->priority = 1;
  }
}
