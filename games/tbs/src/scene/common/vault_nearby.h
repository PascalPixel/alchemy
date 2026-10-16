#include "types.h"
#include "field_obj.h"

// UB: update hooks return void. In tbs-en vault_revisit, plain
// void differs in 13 bytes of the 152-byte function. Keep the unused
// native result in matching builds.
#ifdef UBFIX
static void SceneActor_UpdateProximityToLeader(union FieldObject *fieldObject) {
#else
static s32 SceneActor_UpdateProximityToLeader(union FieldObject *fieldObject) {
#endif
  struct FieldActor *self = (struct FieldActor *)fieldObject;
  u8 **globals = gWindowWork;
  u8 *scene = globals[0];
  u8 *workspace = globals[12];
  s16 *flags = &self->timer;
  s32 force = 0;
  s32 range = 18;
  struct FieldActor *partner;
  struct FieldActor *player;

  if ((*flags & 1) != 0) {
    partner = Object_GetById(15);
  } else {
    partner = Object_GetById(14);
  }
  if (SceneActor_CheckFacingAndRange(self, partner, 32, 0) != 0) {
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  }

  player = Object_GetById(ACTOR_PARTY_LEADER);

  if (*(s16 *)(workspace + 376) != 0 || scene[RENDER_MODE_OFFSET] != 0) {
    range = 26;
    if ((*flags & 2) != 0) {
      force = 1;
    }
  }

  SceneActor_CheckFacingAndRange(self, player, range, force);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}
