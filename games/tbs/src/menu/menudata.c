#include "menu.h"
#include "field.h"
#include "edition.h"
#include "types.h"

// The menu tables: cursor bobbing, sort orders, highlight widths and
// resource ids.
//
// None of these is const: the code that reads them declares them without it.

/// The hand cursor's bobbing offsets in pixels, which UiMenu_PositionCursor
/// adds to its x at step (gFrameCount >> 1) & 7; the ninth entry is never
/// read.
u8 gUiMenu_CursorBobX[9] = {0, 1, 1, 2, 2, 1, 1, 0, 0};
/// The same for y: in the cursor's 8-bit OAM y, 0xff and 0xfe wrap to one
/// and two pixels up.
u8 gUiMenu_CursorBobY[9] = {0, 0xff, 0xff, 0xfe, 0xfe, 0xff, 0xff, 0, 0};

/// The item menu's sort orders (Menu_CopyListOrder), one item type per byte,
/// 0x80 (LIST_ORDER_EQUIPPED) for only the equipped items of that type,
/// ending with 0xff (LIST_ORDER_END). The item types (enum ItemType): 0 all
/// that is not equipment, 1 weapons, 2 armor, 3 shields, gloves and armlets,
/// 4 helms, 5 boots, 6 Psynergy items, 8 rings and 9 shirts; every order
/// also lists 7, which no ItemType names. L + A in the item menu sorts by
/// modes 0, 1 and 2, then this default (mode 3), in turn.
/// The default puts the items that are not equipment first.
u8 gMenu_ListOrderDefault[11] = {0x00, 0x01, 0x04, 0x03, 0x02, 0x05,
                                 0x06, 0x07, 0x08, 0x09, 0xff};
/// Mode 2: the equipment first, the rest last.
u8 gMenu_ListOrderMode2[11] = {0x01, 0x04, 0x03, 0x02, 0x05, 0x06,
                               0x07, 0x08, 0x09, 0x00, 0xff};
// clang-format off
/// Mode 1: the equipped items, the other equipment, then the rest.
u8 gMenu_ListOrderMode1[20] = {
    0x81, 0x84, 0x83, 0x82, 0x85, 0x86, 0x87, 0x88, 0x89, 0x01,
    0x04, 0x03, 0x02, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0xff,
};
/// Mode 0: the equipped items, the items that are not equipment, then the
/// other equipment. ItemMenu_SelectItem's list also sorts by it.
u8 gMenu_ListOrderMode0[20] = {
    0x81, 0x84, 0x83, 0x82, 0x85, 0x86, 0x87, 0x88, 0x89, 0x00,
    0x01, 0x04, 0x03, 0x02, 0x05, 0x06, 0x07, 0x08, 0x09, 0xff,
};

#if (LANGUAGE != LANGUAGE_JAPANESE)
/// The x in pixels of the item command cursor, three columns on each of two
/// rows (ItemMenu_CommandCursorX; the Japanese edition has neither).
s32 gItemMenu_CommandColumnXTable[6] = {
    86, 117, 166,
    86, 117, 166,
};
// clang-format on
#endif

// The width in tiles of the status screen's highlight on each ailment row
// (Down, Poison, Venom, Curse and Haunt).
#if (LANGUAGE == LANGUAGE_JAPANESE)
u8 gCharacterMenu_CursorWidths[5] = {9, 4, 6, 5, 7};
#elif (LANGUAGE == LANGUAGE_GERMAN)
u8 gCharacterMenu_CursorWidths[5] = {7, 5, 7, 6, 5};
#else
u8 gCharacterMenu_CursorWidths[5] = {6, 7, 7, 6, 6};
#endif

/// The sprites of the four Djinn FourObjectMotion shows: a Venus, a Mercury,
/// a Mars and a Jupiter Djinni (sprites 90 to 93).
s32 gFourObjectMotion_ResourceIds[4] = {0x5a, 0x5b, 0x5c, 0x5d};
