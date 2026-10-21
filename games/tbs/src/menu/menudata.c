#include "menu.h"
#include "field.h"
#include "edition.h"
#include "types.h"
#include "item.h"
#include "unit/menu/menu.h"
#include "game/character_ids.h"

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

// The item menu's sort orders (enum ListSort, Menu_CopyListOrder): one item
// type per byte, with LIST_ORDER_EQUIPPED for only the equipped items of that
// type, ending with LIST_ORDER_END. Every order also lists the item type
// between ITEM_TYPE_PSYNERGY and ITEM_TYPE_RING, which no item has and no
// ItemType enumerator names.
// clang-format off
/// LIST_SORT_ITEMS_FIRST: the items that are not equipment first.
u8 gMenu_ListOrderDefault[11] = {
    ITEM_TYPE_NONE, ITEM_TYPE_WEAPON, ITEM_TYPE_HELM, ITEM_TYPE_SHIELD,
    ITEM_TYPE_ARMOR, ITEM_TYPE_BOOTS, ITEM_TYPE_PSYNERGY, ITEM_TYPE_UNUSED, ITEM_TYPE_RING,
    ITEM_TYPE_SHIRT, LIST_ORDER_END,
};
/// LIST_SORT_EQUIPMENT_FIRST: the equipment first, the rest last.
u8 gMenu_ListOrderMode2[11] = {
    ITEM_TYPE_WEAPON, ITEM_TYPE_HELM, ITEM_TYPE_SHIELD, ITEM_TYPE_ARMOR,
    ITEM_TYPE_BOOTS, ITEM_TYPE_PSYNERGY, ITEM_TYPE_UNUSED, ITEM_TYPE_RING, ITEM_TYPE_SHIRT,
    ITEM_TYPE_NONE, LIST_ORDER_END,
};
/// LIST_SORT_EQUIPPED_THEN_EQUIPMENT: the equipped items, the other equipment,
/// then the rest.
u8 gMenu_ListOrderMode1[20] = {
    LIST_ORDER_EQUIPPED | ITEM_TYPE_WEAPON, LIST_ORDER_EQUIPPED | ITEM_TYPE_HELM,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_SHIELD, LIST_ORDER_EQUIPPED | ITEM_TYPE_ARMOR,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_BOOTS, LIST_ORDER_EQUIPPED | ITEM_TYPE_PSYNERGY,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_UNUSED, LIST_ORDER_EQUIPPED | ITEM_TYPE_RING,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_SHIRT,
    ITEM_TYPE_WEAPON, ITEM_TYPE_HELM, ITEM_TYPE_SHIELD, ITEM_TYPE_ARMOR,
    ITEM_TYPE_BOOTS, ITEM_TYPE_PSYNERGY, ITEM_TYPE_UNUSED, ITEM_TYPE_RING, ITEM_TYPE_SHIRT,
    ITEM_TYPE_NONE, LIST_ORDER_END,
};
/// LIST_SORT_EQUIPPED_THEN_ITEMS: the equipped items, the items that are not
/// equipment, then the other equipment. ItemMenu_SelectItem's list also sorts
/// by it.
u8 gMenu_ListOrderMode0[20] = {
    LIST_ORDER_EQUIPPED | ITEM_TYPE_WEAPON, LIST_ORDER_EQUIPPED | ITEM_TYPE_HELM,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_SHIELD, LIST_ORDER_EQUIPPED | ITEM_TYPE_ARMOR,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_BOOTS, LIST_ORDER_EQUIPPED | ITEM_TYPE_PSYNERGY,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_UNUSED, LIST_ORDER_EQUIPPED | ITEM_TYPE_RING,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_SHIRT,
    ITEM_TYPE_NONE,
    ITEM_TYPE_WEAPON, ITEM_TYPE_HELM, ITEM_TYPE_SHIELD, ITEM_TYPE_ARMOR,
    ITEM_TYPE_BOOTS, ITEM_TYPE_PSYNERGY, ITEM_TYPE_UNUSED, ITEM_TYPE_RING, ITEM_TYPE_SHIRT,
    LIST_ORDER_END,
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
/// a Mars and a Jupiter Djinni.
s32 gFourObjectMotion_ResourceIds[4] = {VENUS_DJINNI, MERCURY_DJINNI,
                                        MARS_DJINNI, JUPITER_DJINNI};
