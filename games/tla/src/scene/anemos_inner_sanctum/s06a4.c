#include "types.h"
#include "actor.h"
#include "event_inline.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern const u8 gData_02003674[];

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Func_020032e0(struct Actor *object, s32 mode);
void Func_020032e8(struct Actor *object, const u8 *script);

void Func_020006a4(struct Actor *source) {
  struct Actor *object =
      (FieldActor_Create(672, source->x, source->y, source->z));

  object->motionFlags = 0;
  Actor_SetSpriteColor(object, BYTE_AT(source, 98));
  Scene_SetActorSpritePriority(object, 2);
  Func_020032e0(object, 1);
  Actor_SetSingleChildFlags(object, 0);
  Func_020032e8(object, gData_02003674);
  Animation_SetStateFlags(object, 1);
}
