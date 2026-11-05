#include "types.h"
#include "actor.h"
#include "field_sprite.h"

struct Actor *Func_020021cc(s32 kind, s32 x, s32 y, s32 z);
u32 Random16Far(void);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);
void Func_02000720(void);

void Func_020007d0(void) {
  s32 i;
  s32 phase;
  struct Actor *object;

  for (i = 0, phase = 0; i != 22; i++, phase += 5) {
    object = Func_020021cc(672, 0, 0, 0);
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
