#ifndef ALCHEMY_MAP_VAULT_H
#define ALCHEMY_MAP_VAULT_H

/// @file
/// The cell behaviors of Vault's layout, which SceneId_Vault (vault.c) and
/// SceneId_VaultRevisit (vault_revisit.c) both draw and answer.

/// The cell behaviors Vault's coord and examine triggers answer; the exits
/// have behaviors of their own.
enum VaultCellBehavior {
  /// Either end of the rope bridge in the town's south-east, where stepping
  /// on raises the path that runs under the bridge to the bridge's deck
  /// (Vault_RaisePathToBridge).
  /// ★ Named from what the player sees: the rope bridge the leader walks
  /// across (render probe vault-bridge)
  CELL_BEHAVIOR_VAULT_BRIDGE_END = 1,
  /// The three cells west of the bridge's west end, where stepping off
  /// lowers the path back under the bridge (Vault_LowerPathUnderBridge).
  /// ★ Named from what the player sees: the rope bridge the leader walks
  /// across (render probe vault-bridge)
  CELL_BEHAVIOR_VAULT_WEST_OF_BRIDGE = 2,
  /// The door of the house where a man keeps a sparkling rock the volcano
  /// threw.
  CELL_BEHAVIOR_VAULT_GEM_OWNERS_HOUSE_DOOR = 5,
  CELL_BEHAVIOR_VAULT_CHILDS_HOUSE_DOOR = 6,
  CELL_BEHAVIOR_VAULT_INN_DOOR = 7,
  CELL_BEHAVIOR_VAULT_ITEM_SHOP_DOOR = 8,
  CELL_BEHAVIOR_VAULT_WEAPON_AND_ARMOR_SHOP_DOOR = 9,
  CELL_BEHAVIOR_VAULT_MAYORS_HOUSE_DOOR = 10,
  CELL_BEHAVIOR_VAULT_SANCTUM_DOORWAY = 11,
  CELL_BEHAVIOR_VAULT_DOOR_TO_JAIL = 12,
  /// The doorless way into the room with the crate.
  CELL_BEHAVIOR_VAULT_WEST_ENTRANCE = 13,
  /// Before the south gate's exit (VAULT_TO_WORLD_MAP_SOUTH).
  CELL_BEHAVIOR_VAULT_SOUTH_GATE = 20,
  /// Before the north gate's exit (VAULT_TO_WORLD_MAP_NORTH).
  CELL_BEHAVIOR_VAULT_NORTH_GATE = 21,
  CELL_BEHAVIOR_VAULT_BELL = 30,
  /// The hidden passage down into Vault Cave, which Reveal shows
  /// (VAULT_REVISIT_TO_VAULT_CAVE_PASSAGE).
  CELL_BEHAVIOR_VAULT_HIDDEN_PASSAGE = 40,
  /// The cave mouth in the town's northeast cliff
  /// (VAULT_REVISIT_TO_VAULT_CAVE_MOUTH).
  CELL_BEHAVIOR_VAULT_CLIFF_CAVE_MOUTH = 41,
  /// ★ Named from the game: "peered into the well" (MsgFieldPeeredWell)
  CELL_BEHAVIOR_VAULT_WELL = 90,
  CELL_BEHAVIOR_VAULT_WOODEN_BOX = 101
};

#endif
