#ifndef ALCHEMY_SCENE_VAULT_H
#define ALCHEMY_SCENE_VAULT_H

/// @file
/// The collision codes of Vault's map, which SceneId_Vault (vault.c) and
/// SceneId_VaultRevisit (vault_revisit.c) both draw and answer.

/// The cells Vault's touch and search events answer; the exits have codes of
/// their own.
enum VaultCell {
  /// Either end of the rope bridge in the town's south-east, where stepping
  /// on raises the path that runs under the bridge to the bridge's deck
  /// (Vault_RaisePathToBridge).
  /// ★ Named from what the player sees: the rope bridge the leader walks
  /// across (render probe vault-bridge)
  VAULT_CELL_BRIDGE_END = 1,
  /// The three cells west of the bridge's west end, where stepping off
  /// lowers the path back under the bridge (Vault_LowerPathUnderBridge).
  /// ★ Named from what the player sees: the rope bridge the leader walks
  /// across (render probe vault-bridge)
  VAULT_CELL_WEST_OF_BRIDGE = 2,
  /// The door of the house where a man keeps a sparkling rock the volcano
  /// threw.
  VAULT_CELL_GEM_OWNERS_HOUSE_DOOR = 5,
  VAULT_CELL_CHILDS_HOUSE_DOOR = 6,
  VAULT_CELL_INN_DOOR = 7,
  VAULT_CELL_ITEM_SHOP_DOOR = 8,
  VAULT_CELL_WEAPON_AND_ARMOR_SHOP_DOOR = 9,
  VAULT_CELL_MAYORS_HOUSE_DOOR = 10,
  VAULT_CELL_SANCTUM_DOORWAY = 11,
  VAULT_CELL_JAIL_DOOR = 12,
  /// The doorless way into the room with the crate.
  VAULT_CELL_WEST_ENTRANCE = 13,
  /// Before the south gate's exit (VAULT_TO_WORLD_MAP_SOUTH).
  VAULT_CELL_SOUTH_GATE = 20,
  /// Before the north gate's exit (VAULT_TO_WORLD_MAP_NORTH).
  VAULT_CELL_NORTH_GATE = 21,
  VAULT_CELL_BELL = 30,
  /// The hidden passage down into Vault Cave, which Reveal shows
  /// (VAULT_REVISIT_TO_VAULT_CAVE_PASSAGE).
  VAULT_CELL_CAVE_PASSAGE = 40,
  /// The cave mouth in the town's northeast cliff
  /// (VAULT_REVISIT_TO_VAULT_CAVE_MOUTH).
  VAULT_CELL_CAVE_MOUTH = 41,
  VAULT_CELL_WELL = 90,
  VAULT_CELL_WOODEN_BOX = 101
};

#endif
