#include "types.h"
#include "field_obj.h"

static struct FieldActor *SceneActor_FindActorAtCell(s32 x, s32 z) {
  struct FieldActor **actors = gEventWork->placedActors - 8;
  u32 i;

  for (i = 8; i <= 65; i++) {
    struct FieldActor *actor = actors[i];

    if (x == actor->x.fixed >> 20 && z == actor->z.fixed >> 20) {
      return actor;
    }
  }
  return 0;
}
