#ifndef ALCHEMY_UI_H
#define ALCHEMY_UI_H

/// @file
/// UiWork, the window system's work, and the status screen's cursors.

#include "types.h"
#include "tbs_edition.h"
#include "static_assert.h"
#include "farcall.h"

/// A view of the window system's work (struct UiRenderWork) that names only
/// its dirty bits and its menu busy flag.
struct UiWork {
  u8 unk000[RENDER_DIRTY_OFFSET]; ///< UiRenderWork up to its dirty bits
  unsigned dirty : 8;             ///< UiRenderWork.dirty
  u8 unknownMode[RENDER_MENU_BUSY_OFFSET - RENDER_DIRTY_OFFSET - 1];
  unsigned menuBusy : 8; ///< UiRenderWork.menuBusy
};

/// A cursor stop among the status screen's stats: its help entry, where the
/// cursor points and the run of the row it highlights.
struct OwnerStatusStatCursor {
  u8 entry;           ///< the help line shown while the cursor is here
  u8 column;          ///< tiles
  u8 row;             ///< tiles
  u8 highlightColumn; ///< tiles
  u8 highlightRow;    ///< tiles
  u8 highlightWidth;  ///< tiles
  u8 unk06[2];        ///< 0 in every row
};

/// A cursor stop on the status screen's equipment or conditions: its help
/// entry and where the cursor points.
struct OwnerStatusCursor {
  u8 entry;  ///< the help line shown while the cursor is here
  u8 column; ///< tiles
  u8 row;    ///< tiles
  u8 unk03;  ///< 0 in every row
};

STATIC_ASSERT(sizeof(struct OwnerStatusStatCursor) == 8);
STATIC_ASSERT(sizeof(struct OwnerStatusCursor) == 4);

/// The stat cursor stops: rows 0 to 8 for a character with Djinn, rows 9 to
/// 15 for one without (window/windata.s).
extern const struct OwnerStatusStatCursor gOwnerStatus_StatCursors[];
/// The cursor stops of the screen's second group
/// (STATUS_CURSOR_COMBAT_STATS): column 19, rows 1 to 4.
extern const struct OwnerStatusCursor gOwnerStatus_EquipmentCursors[];
/// The eight condition cursor stops, two tiles apart on row 0.
extern const struct OwnerStatusCursor gOwnerStatus_ConditionCursors[];
/// The level label: `Lv`, or `Niv` in French. The Spanish and Italian
/// editions choose between it, gOwnerStatus_NivString and
/// gOwnerStatus_NvString by System_GetLanguage.
extern const u8 gOwnerStatus_LevelString[];
extern const u8 gOwnerStatus_HpString[];
extern const u8 gOwnerStatus_SlashString[];
extern const u8 gOwnerStatus_PpString[];
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
extern const u8 gOwnerStatus_NivString[];
extern const u8 gOwnerStatus_NvString[];
extern const u8 gOwnerStatus_KpString[];
extern const u8 gOwnerStatus_PvString[];
#endif

#endif
