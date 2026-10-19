#ifndef ALCHEMY_UI_H
#define ALCHEMY_UI_H

/// @file
/// UiWork, the window system's work, and the status screen's cursors.

#include "types.h"
#include "tbs_edition.h"
#include "static_assert.h"
#include "farcall.h"

/// A view of the window system's work that names only its dirty bits and its
/// menu busy flag.
struct UiWork {
  u8 unk000[RENDER_DIRTY_OFFSET];
  unsigned dirty : 8;
  u8 unknownMode[RENDER_MENU_BUSY_OFFSET - RENDER_DIRTY_OFFSET - 1];
  unsigned menuBusy : 8;
};

/// A cursor stop among the status screen's stats: its help entry, where the
/// cursor points and the run of the row it highlights.
struct OwnerStatusStatCursor {
  u8 entry;
  u8 column;          ///< tiles
  u8 row;             ///< tiles
  u8 highlightColumn; ///< tiles
  u8 highlightRow;    ///< tiles
  u8 highlightWidth;  ///< tiles
  u8 unk06[2];
};

/// A cursor stop on the status screen's equipment or conditions: its help
/// entry and where the cursor points.
struct OwnerStatusCursor {
  u8 entry;
  u8 column; ///< tiles
  u8 row;    ///< tiles
  u8 unk03;
};

STATIC_ASSERT(sizeof(struct OwnerStatusStatCursor) == 8);
STATIC_ASSERT(sizeof(struct OwnerStatusCursor) == 4);

extern const struct OwnerStatusStatCursor gOwnerStatus_StatCursors[];
extern const struct OwnerStatusCursor gOwnerStatus_EquipmentCursors[];
extern const struct OwnerStatusCursor gOwnerStatus_ConditionCursors[];
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
