#ifndef ALCHEMY_UI_H
#define ALCHEMY_UI_H

/// @file
/// The Status screen's cursors and the party status window's flags.

#include "types.h"
#include "tbs_edition.h"
#include "static_assert.h"
#include "farcall.h"

/// A cursor stop among the Status screen's stats: its help entry, where the
/// cursor points and the run of the row it highlights.
struct BattleSummaryStatCursor {
  u8 entry;           ///< the help line shown while the cursor is here
  u8 column;          ///< tiles
  u8 row;             ///< tiles
  u8 highlightColumn; ///< tiles
  u8 highlightRow;    ///< tiles
  u8 highlightWidth;  ///< tiles
  u8 reserved[2];     ///< 0 in every row
};

/// A cursor stop on the Status screen's equipment or statuses: its help
/// entry and where the cursor points.
struct BattleSummaryCursor {
  u8 entry;    ///< the help line shown while the cursor is here
  u8 column;   ///< tiles
  u8 row;      ///< tiles
  u8 reserved; ///< 0 in every row
};

STATIC_ASSERT(sizeof(struct BattleSummaryStatCursor) == 8);
STATIC_ASSERT(sizeof(struct BattleSummaryCursor) == 4);

/// The stat cursor stops: rows 0 to 8 for a character with Djinn, rows 9 to
/// 15 for one without (window/windata.s).
extern const struct BattleSummaryStatCursor gBattleStatusScreen_StatCursors[];
/// The cursor stops of the screen's second group
/// (STATUS_CURSOR_COMBAT_STATS): column 19, rows 1 to 4.
extern const struct BattleSummaryCursor gBattleStatusScreen_EquipmentCursors[];
/// The eight status cursor stops, two tiles apart on row 0.
extern const struct BattleSummaryCursor gBattleStatusScreen_ConditionCursors[];
/// The level label: `Lv`, or `Niv` in French. The Spanish and Italian
/// editions choose between it, gBattleStatusScreen_NivString and
/// gBattleStatusScreen_NvString by System_GetLanguage.
extern const u8 gBattleStatusScreen_LevelString[];
extern const u8 gBattleStatusScreen_HpString[];
extern const u8 gBattleStatusScreen_SlashString[];
extern const u8 gBattleStatusScreen_PpString[];
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
extern const u8 gBattleStatusScreen_NivString[];
extern const u8 gBattleStatusScreen_NvString[];
extern const u8 gBattleStatusScreen_KpString[];
extern const u8 gBattleStatusScreen_PvString[];
#endif

/// The party status window's flags (PartyStatus_Draw,
/// PartyStatus_CreateWindow): one column of six tiles per party member
/// (name, HP and, when shown, PP with a bar under each), and in battle an
/// optional five-tile column of standby Djinn counts on its left.
enum PartyStatusFlag {
  PARTY_STATUS_HP_ONLY = 0,     ///< names and HP
  PARTY_STATUS_SHOW_PP = 1,     ///< add the PP row
  PARTY_STATUS_SHOW_DJINN = 2,  ///< add the Djinn column (needs the PP row)
  PARTY_STATUS_CLOSED = 9,      ///< erase the window
  PARTY_STATUS_KEEP_FLAGS = -1, ///< redraw with the flags it has
  /// BattleWork.partyStatus while the party cannot summon.
  PARTY_STATUS_NO_SUMMONS = PARTY_STATUS_SHOW_PP,
  /// BattleWork.partyStatus while it can, drawn only while a Djinni is on
  /// standby.
  PARTY_STATUS_SUMMONS = PARTY_STATUS_SHOW_PP | PARTY_STATUS_SHOW_DJINN
};

#endif
