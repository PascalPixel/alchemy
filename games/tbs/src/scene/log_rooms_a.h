#ifndef ALCHEMY_MAP_LOG_ROOMS_A_H
#define ALCHEMY_MAP_LOG_ROOMS_A_H

/// @file
/// The metatile behaviors of the layout of Vault's houses, shops and inn, which
/// SceneId_LogRoomsA (log_rooms_a.c) and SceneId_LogRoomsB
/// (log_rooms_b.c) both answer.

/// The cells the interiors' step triggers and examine triggers answer; the
/// exits have metatile behaviors of their own.
enum LogRoomsAMetatileBehavior {
  /// The stairs down from the inn's guests' room.
  MB_LOG_ROOMS_A_GUESTS_ROOM_STAIRS = 7,
  /// Inside the guests' room, where the suspects first see Ivan.
  MB_LOG_ROOMS_A_GUESTS_ROOM = 10,
  /// The first of the eight cells (to 18) that move the fleeing suspects on
  /// along their routes.
  MB_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE = 11,
  MB_LOG_ROOMS_A_LAST_SUSPECT_ROUTE = 18,
  /// The guests' room's door.
  MB_LOG_ROOMS_A_GUESTS_ROOM_DOOR = 20,
  /// In the guests' room, where the party sets out to corner the suspects.
  MB_LOG_ROOMS_A_CORNERING_START = 21,
  /// The way up behind the crate to the inn's loft.
  MB_LOG_ROOMS_A_LOFT_CLIMB = 26,
  /// The way back down from the loft.
  MB_LOG_ROOMS_A_LOFT_DESCENT = 27,
  /// The gap in the roof, which the leader jumps.
  MB_LOG_ROOMS_A_ROOF_GAP = 40,
  /// The barrel beside the girl at the inn, where the Bones are once the
  /// thieves are jailed.
  MB_LOG_ROOMS_A_INN_BARREL = 41,
  MB_LOG_ROOMS_A_JAR = 100,
  /// ★ Named from the game: "checked the barrel" (MsgVaultIsaacCheckedBarrel)
  MB_LOG_ROOMS_A_FIRST_BARREL = 101,
  /// ★ Named from the game: "checked the barrel" (MsgVaultIsaacCheckedBarrel)
  MB_LOG_ROOMS_A_SECOND_BARREL = 102,
  MB_LOG_ROOMS_A_WOODEN_BOX = 103,
  /// ★ Named from the game: "History of Vault, Vol 1."
  /// (MsgVaultTheresABookEntitledHistoryOf)
  MB_LOG_ROOMS_A_HISTORY_BOOKCASE = 200,
  /// ★ Named from the game: "Gems of Angara."
  /// (MsgVaultTheresABookEntitledGemsOf)
  MB_LOG_ROOMS_A_GEMS_BOOKCASE = 201
};

#endif
