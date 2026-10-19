#ifndef ALCHEMY_UI_H
#define ALCHEMY_UI_H

/// @file
/// UiWork, the window system's work, and the status screen's cursors.

#include "types.h"
#include "tbs_edition.h"
#include "static_assert.h"
#include "farcall.h"

struct UiWork {
  u8 unk000[RENDER_DIRTY_OFFSET];
  unsigned dirty : 8;
  u8 unknownMode[RENDER_MENU_BUSY_OFFSET - RENDER_DIRTY_OFFSET - 1];
  unsigned menuBusy : 8;
};

struct OwnerStatusStatCursor {
  u8 entry;
  u8 column;
  u8 row;
  u8 highlightColumn;
  u8 highlightRow;
  u8 highlightWidth;
  u8 unk06[2];
};

struct OwnerStatusCursor {
  u8 entry;
  u8 column;
  u8 row;
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
