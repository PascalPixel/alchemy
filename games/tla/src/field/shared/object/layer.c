#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"

struct ObjectLayerAnimation {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
  u8 unk0a[27];
  u8 unused25 : 2;
  u8 shadowLayer : 2;
};

/// Gives the object the party leader's display layer.
s32 Func_080d4ab4(struct Actor *object) {
  struct Actor *leader = ObjectTable_Get(gSequenceParentWork.currentOwner);
  s32 layer = ((struct ObjectLayerAnimation *)leader->animation)->layer;

  if ((object->animationKind & 0xf) == 1) {
    struct ObjectLayerAnimation *animation = object->animation;

    animation->layer = layer;
    animation->shadowLayer = layer;
    object->priorityFlags &= 0xfe;
  }
  return 0;
}
