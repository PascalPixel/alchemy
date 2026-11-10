#include "mars_lighthouse_star_room.h"
#include "scene_tables.h"


// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_0200432c_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static __inline__ void Func_0200423c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

void Func_02000db8(s32 id) {
  struct Actor *object = Actor_Get(id);

  Actor_SetSingleChildFlags(Actor_Get(id), 0);
  FieldActor_SetSpritePriority(id, 3);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  object->collisionFlags = 0;
  if (id == 13) {
    Func_0200432c_(13, 175 << 18, PIXELS(224));
    Actor_SetAnimation(13, 10);
    MapGrid_CopyCellAttributes(39, 22, 2, 1, 43, id);
  } else {
    Func_0200432c_(14, 165 << 18, PIXELS(216));
    Actor_SetAnimation(14, 9);
    Func_0200423c_(31, 21, 2, 2, 40, 12);
  }
}
