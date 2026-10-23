#ifndef ALCHEMY_MAP_VAULT_H
#define ALCHEMY_MAP_VAULT_H

/// @file
/// The metatile behaviors of Vault's layout, which MapId_Vault (vault.c) and
/// MapId_VaultRevisit (vault_revisit.c) both draw and answer.

/// The metatile behaviors Vault's coord and bg events answer; the warps have
/// behaviors of their own.
enum VaultMetatileBehavior {
  /// Either end of the rope bridge in the town's south-east, where stepping
  /// on raises the path that runs under the bridge to the bridge's deck
  /// (Vault_RaisePathToBridge).
  /// ★ Named from what the player sees: the rope bridge the leader walks
  /// across (render probe vault-bridge)
  MB_VAULT_BRIDGE_END = 1,
  /// The three cells west of the bridge's west end, where stepping off
  /// lowers the path back under the bridge (Vault_LowerPathUnderBridge).
  /// ★ Named from what the player sees: the rope bridge the leader walks
  /// across (render probe vault-bridge)
  MB_VAULT_WEST_OF_BRIDGE = 2,
  /// The door of the house where a man keeps a sparkling rock the volcano
  /// threw.
  MB_VAULT_GEM_OWNERS_HOUSE_DOOR = 5,
  MB_VAULT_CHILDS_HOUSE_DOOR = 6,
  MB_VAULT_INN_DOOR = 7,
  MB_VAULT_ITEM_SHOP_DOOR = 8,
  MB_VAULT_WEAPON_AND_ARMOR_SHOP_DOOR = 9,
  MB_VAULT_MAYORS_HOUSE_DOOR = 10,
  MB_VAULT_SANCTUM_DOORWAY = 11,
  MB_VAULT_DOOR_TO_JAIL = 12,
  /// The doorless way into the room with the crate.
  MB_VAULT_WEST_ENTRANCE = 13,
  /// Before the south gate's warp (VAULT_TO_WORLD_MAP_SOUTH).
  MB_VAULT_SOUTH_GATE = 20,
  /// Before the north gate's warp (VAULT_TO_WORLD_MAP_NORTH).
  MB_VAULT_NORTH_GATE = 21,
  MB_VAULT_BELL = 30,
  /// The hidden passage down into Vault Cave, which Reveal shows
  /// (VAULT_REVISIT_TO_VAULT_CAVE_PASSAGE).
  MB_VAULT_HIDDEN_PASSAGE = 40,
  /// The cave mouth in the town's northeast cliff
  /// (VAULT_REVISIT_TO_VAULT_CAVE_MOUTH).
  MB_VAULT_CLIFF_CAVE_MOUTH = 41,
  /// ★ Named from the game: "peered into the well" (MsgFieldPeeredWell)
  MB_VAULT_WELL = 90,
  MB_VAULT_WOODEN_BOX = 101
};

#endif
