/// @file
/// Vault's rooms, before and after the thieves are caught: whether the
/// leader stands beside the Bone on the floor, which the scenes' trigger tables
/// test through FLAG_VAULT_LEADER_BESIDE_BONE.

/// The leader stands beside the Bone on the inn's barrel, so the girl at the
/// inn's talk row gives way and the A button searches the Bone.
#define FLAG_VAULT_LEADER_BESIDE_BONE FLAG_SCENE_TEMP_50

/// A task: sets FLAG_VAULT_LEADER_BESIDE_BONE while the leader stands in cells
/// (34, 41) to (35, 42), beside the barrel, and clears it anywhere else.
static void Vault_UpdateLeaderBesideBoneFlag(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  FIXED fixedX = leader->x.fixed;
  s32 column;
  s32 row;

  row = Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed;
  column = FIELD_CELL(fixedX);
  column -= 34;
  row = FIELD_CELL(row);

  if (column >= 0 && column <= 1 && row > 40 && row <= 42) {
    GameFlag_Set(FLAG_VAULT_LEADER_BESIDE_BONE);
  } else {
    GameFlag_Clear(FLAG_VAULT_LEADER_BESIDE_BONE);
  }
}
