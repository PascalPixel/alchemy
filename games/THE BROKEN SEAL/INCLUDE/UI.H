#ifndef ALCHEMY_UI_H
#define ALCHEMY_UI_H

#include "TYPES.H"
#include "TBS_EDITION.H"
#include "LAYOUT_GUARD.H"

/* The flags the menus set in the window work. */
struct UiWork {
    u8 unknown_000[RENDER_DIRTY_OFS];
    unsigned dirty : 8;
    u8 unknown_mode[RENDER_MENU_BUSY_OFS - RENDER_DIRTY_OFS - 1];
    unsigned menu_busy : 8;
};

/* Cursor rows owned by the status screen's ROM tables. */
struct OwnerStatusStatCursor {
    u8 entry;
    u8 column;
    u8 row;
    u8 highlight_column;
    u8 highlight_row;
    u8 highlight_width;
    u8 unknown_06[2];
};

struct OwnerStatusCursor {
    u8 entry;
    u8 column;
    u8 row;
    u8 unknown_03;
};

LAYOUT_SIZE_GUARD(OwnerStatusStatCursor_Size, struct OwnerStatusStatCursor, 8);
LAYOUT_SIZE_GUARD(OwnerStatusCursor_Size, struct OwnerStatusCursor, 4);

extern const struct OwnerStatusStatCursor OwnerStatus_StatCursors[];
extern const struct OwnerStatusCursor OwnerStatus_EquipmentCursors[];
extern const struct OwnerStatusCursor OwnerStatus_ConditionCursors[];
extern const u8 OwnerStatus_LevelString[];
extern const u8 OwnerStatus_HpString[];
extern const u8 OwnerStatus_SlashString[];
extern const u8 OwnerStatus_PpString[];
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
extern const u8 OwnerStatus_NivString[];
extern const u8 OwnerStatus_NvString[];
extern const u8 OwnerStatus_KpString[];
extern const u8 OwnerStatus_PvString[];
#endif

/* Window and text services the menus reach through the far-call table at
   0x08015000. */

s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
s32 UiWork_IsCompleteFar(void);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

#endif
