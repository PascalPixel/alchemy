// Vault's interiors, before and after the thieves are caught: whether the
// leader stands beside the Bone on the floor, which the scenes' event tables
// test through FLAG_SCENE_TEMP_50.
#include "types.h"
#include "field_obj.h"
#include "field_event.h"
#include "flags.h"

// A scene callback, every frame: set FLAG_SCENE_TEMP_50 while the leader is on
// the cells around the Bone (columns 34 and 35, rows 41 and 42; the Bone lies
// at (568, 672)), and clear it elsewhere.
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
