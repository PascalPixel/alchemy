#include "types.h"
#include "field_obj.h"
#include "field_event.h"
#include "flags.h"

// Enable the Bone offer while the leader stands beside it.
static void Vault_UpdateBoneProximityFlag(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 fixedX = leader->x.fixed;
  s32 x;
  s32 z;

  struct FieldActor *leaderAtZRead = Object_GetById(ACTOR_PARTY_LEADER);
  x = fixedX >> 20;
  z = leaderAtZRead->z.fixed;
  x -= 34;
  z >>= 20;

  if ((u32)x <= 1 && z > 40 && z <= 42) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_50);
  } else {
    Engine_GameFlagClear(FLAG_SCENE_TEMP_50);
  }
}
