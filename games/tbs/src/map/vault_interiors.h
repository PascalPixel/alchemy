#ifndef ALCHEMY_MAP_VAULT_INTERIORS_H
#define ALCHEMY_MAP_VAULT_INTERIORS_H

/// @file
/// The metatile behaviors of the layout of Vault's houses, shops and inn, which
/// MapId_VaultInteriors (vault_interiors.c) and MapId_VaultInteriorsRevisit
/// (vault_interiors_revisit.c) both answer.

/// The cells the interiors' coord events and bg events answer; the warps have
/// metatile behaviors of their own.
enum VaultInteriorsCell {
  /// The stairs down from the inn's guests' room.
  VAULT_INTERIORS_CELL_GUESTS_ROOM_STAIRS = 7,
  /// Inside the guests' room, where the suspects first see Ivan.
  VAULT_INTERIORS_CELL_GUESTS_ROOM = 10,
  /// The first of the eight cells (to 18) that move the fleeing suspects on
  /// along their routes.
  VAULT_INTERIORS_CELL_FIRST_SUSPECT_ROUTE = 11,
  VAULT_INTERIORS_CELL_LAST_SUSPECT_ROUTE = 18,
  /// The guests' room's door.
  VAULT_INTERIORS_CELL_GUESTS_ROOM_DOOR = 20,
  /// In the guests' room, where the party sets out to corner the suspects.
  VAULT_INTERIORS_CELL_CORNERING_START = 21,
  /// The way up behind the crate to the inn's loft.
  VAULT_INTERIORS_CELL_LOFT_CLIMB = 26,
  /// The way back down from the loft.
  VAULT_INTERIORS_CELL_LOFT_DESCENT = 27,
  /// The gap in the roof, which the leader jumps.
  VAULT_INTERIORS_CELL_ROOF_GAP = 40,
  /// The barrel beside the girl at the inn, where the Bones are once the
  /// thieves are jailed.
  VAULT_INTERIORS_CELL_INN_BARREL = 41,
  VAULT_INTERIORS_CELL_JAR = 100,
  VAULT_INTERIORS_CELL_FIRST_BARREL = 101,
  VAULT_INTERIORS_CELL_SECOND_BARREL = 102,
  VAULT_INTERIORS_CELL_WOODEN_BOX = 103,
  /// ★ Named from the game: "History of Vault, Vol 1."
  /// (MsgVaultTheresABookEntitledHistoryOf)
  VAULT_INTERIORS_CELL_HISTORY_BOOKCASE = 200,
  /// ★ Named from the game: "Gems of Angara."
  /// (MsgVaultTheresABookEntitledGemsOf)
  VAULT_INTERIORS_CELL_GEMS_BOOKCASE = 201
};

#endif
