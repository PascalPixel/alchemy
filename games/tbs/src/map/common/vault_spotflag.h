/// @file
/// Vault's interiors, before and after the thieves are caught: whether the
/// leader stands beside the Bone on the floor, which the scenes' event tables
/// test through FLAG_SCENE_TEMP_50.
#include "types.h"
#include "object_event.h"
#include "script_layer.h"
#include "flags.h"

/// Keeps the girl at the inn from offering another Bone too soon.
/// She doesn't offer one while the leader stands beside the one on the floor.
/// Each scene adds this callback in the girl at the inn's house after the
/// thieves are jailed. FLAG_SCENE_TEMP_50 records proximity for the talk-event
/// table.
static void Vault_UpdateBoneProximityFlag(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 fixedX = leader->x.fixed;
  s32 column;
  s32 row;

  struct FieldActor *leaderAgain = Object_GetById(ACTOR_PARTY_LEADER);
  column = fixedX >> 20;
  row = leaderAgain->z.fixed;
  column -= 34;
  row >>= 20;

  if ((u32)column <= 1 && row > 40 && row <= 42) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_50);
  } else {
    Engine_GameFlagClear(FLAG_SCENE_TEMP_50);
  }
}
