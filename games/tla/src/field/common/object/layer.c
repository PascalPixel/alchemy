#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "edition.h"

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
// The Spanish and Italian far-call tables reach this function under the
// name of another entry.
#define Func_080d4ab4 Func_080da908
#endif

struct ObjectLayerAnimation {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
  u8 unk0a[27];
  u8 unused25 : 2;
  u8 shadowLayer : 2;
};

/// Gives the object the party leader's display layer.
s32 Func_080d4ab4(struct ObjectRuntime *object) {
  struct ObjectRuntime *leader = ObjectTable_Get(gPartyState.currentOwner);
  s32 layer = ((struct ObjectLayerAnimation *)leader->animation)->layer;

  if ((object->animationKind & 0xf) == 1) {
    struct ObjectLayerAnimation *animation = object->animation;

    animation->layer = layer;
    animation->shadowLayer = layer;
    object->priorityFlags &= 0xfe;
  }
  return 0;
}
