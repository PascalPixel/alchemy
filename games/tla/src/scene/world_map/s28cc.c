#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "fixed_math.h"
#include "scene_tables.h"
#include "event_inline.h"

extern u32 gFrameCount;
extern const u8 gData_02004f10[];

u32 Random16Far(void);
void Func_02004b18(struct Actor *object, s32 mode);
void Func_02004b20(struct Actor *object, const u8 *script);

static __inline__ struct Actor *Func_02004b30_(s32 kind, s32 x, s32 y, s32 z) {
  /* FAKEMATCH(wrapper): the create call forwarded through an inline, so its
   * arguments build in r0 after the position loads; as a direct call, 32 of
   * 91 instructions differ in tla-en. */
  return ((struct Actor *)Import_ActorCreate(kind, x, y, z));
}

void Func_020028cc(void) {
  struct Actor *leader;
  struct Actor *obj;
  struct FieldSprite *spr;
  u32 value;

  if ((gFrameCount & 15) != 0)
    return;
  leader = Actor_GetById(8);
  obj = Func_02004b30_(285, leader->x - PIXELS(32), leader->y,
                       leader->z - PIXELS(16));
  if (obj == NULL)
    return;
  obj->scaleX = Q16_ONE / 2;
  obj->scaleY = Q16_ONE / 2;
  spr = obj->animation;
  if ((Random16Far() * 2) >> 16) {
    s32 back = ((Random16Far() * 48) >> 16) << 16;

    obj->x -= back >> 1;
    obj->z -= back;
  } else {
    value = ((Random16Far() << 5) >> 16) << 16;
    obj->x += value;
    value = (s32)value >> 1;
    obj->z += value;
  }
  spr->nextFrame = 0;
  spr->priority = ((struct FieldSprite *)leader->animation)->priority;
  obj->priorityFlags |= 2;
  obj->motionFlags = leader->motionFlags;
  Actor_SetPartAttribute(obj, 9);
  Func_02004b18(obj, 2);
  Func_02004b20(obj, gData_02004f10);
}
