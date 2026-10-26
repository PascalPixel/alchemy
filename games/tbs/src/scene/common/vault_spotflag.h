/// @file
/// Vault's interiors, before and after the thieves are caught: whether the
/// leader stands beside the Bone on the floor, which the maps' event tables
/// test through FLAG_VAULT_LEADER_BESIDE_BONE.
#include "types.h"
#include "actor.h"
#include "script_layer.h"
#include "flags.h"

/// The leader stands beside the Bone on the inn's floor, so the girl at the
/// inn's talk row gives way and the A button searches the Bone.
/// ★ Named from the game: "Bone" (MsgItemName)
#define FLAG_VAULT_LEADER_BESIDE_BONE FLAG_SCENE_TEMP_50

/// Keeps the girl at the inn from offering another Bone too soon. She doesn't
/// offer one while the leader stands beside the one on the floor. Each map adds
/// this callback in the girl at the inn's house after the thieves are jailed.
/// FLAG_VAULT_LEADER_BESIDE_BONE records proximity for the actor talk
/// table.
static void Vault_UpdateBoneProximityFlag(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  s32 fixedX = leader->x.fixed;
  s32 column;
  s32 row;

  struct Actor *leaderAgain = Actor_GetById(ACTOR_PARTY_LEADER);
  column = fixedX >> 20;
  row = leaderAgain->z.fixed;
  column -= 34;
  row >>= 20;

  if ((u32)column <= 1 && row > 40 && row <= 42) {
    GameFlag_Set(FLAG_VAULT_LEADER_BESIDE_BONE);
  } else {
    GameFlag_Clear(FLAG_VAULT_LEADER_BESIDE_BONE);
  }
}
