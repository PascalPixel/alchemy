#include "menu/menu.h"
#include "field/field.h"
#include "edition.h"
#include "types.h"
#include "game/item.h"

//--------------------------------------------------------------------------
// Menu tables: cursor bobbing, list orders, cursor widths and resource ids.
// The menus declare them without const, so they are not const here either.
//--------------------------------------------------------------------------

/// The cursor's bob, indexed by (gFrameCount >> 1) & 7
u8 gMenuCursor_BobX[9] = {0, 1, 1, 2, 2, 1, 1, 0, 0};
u8 gMenuCursor_BobY[9] = {0,    0xff, 0xff, 0xfe, 0xfe,
                          0xff, 0xff, 0,    0}; // 0, -1, -1, -2 ...

// The item menu's sort orders (enum ListSort, Menu_CopyListOrder): one item
// type per byte, with LIST_ORDER_EQUIPPED for only the equipped items of that
// type, ending with LIST_ORDER_END. The class-changing items stand between the
// boots and the Psynergy items, and the Elemental Stars after the shirts.
// clang-format off
/// LIST_SORT_ITEMS_FIRST: the items that are not equipment first.
u8 gMenu_ListOrderItemsFirst[13] = {
    ITEM_TYPE_NONE, ITEM_TYPE_WEAPON, ITEM_TYPE_HELM, ITEM_TYPE_SHIELD,
    ITEM_TYPE_ARMOR, ITEM_TYPE_BOOTS, ITEM_TYPE_CLASS_CHANGE, ITEM_TYPE_PSYNERGY,
    ITEM_TYPE_TRIDENT, ITEM_TYPE_RING, ITEM_TYPE_SHIRT, ITEM_TYPE_ELEMENTAL_STAR,
    LIST_ORDER_END,
};
/// LIST_SORT_EQUIPMENT_FIRST: the equipment first, the rest last.
u8 gMenu_ListOrderEquipmentFirst[13] = {
    ITEM_TYPE_WEAPON, ITEM_TYPE_HELM, ITEM_TYPE_SHIELD, ITEM_TYPE_ARMOR,
    ITEM_TYPE_BOOTS, ITEM_TYPE_CLASS_CHANGE, ITEM_TYPE_PSYNERGY, ITEM_TYPE_TRIDENT,
    ITEM_TYPE_RING, ITEM_TYPE_SHIRT, ITEM_TYPE_ELEMENTAL_STAR, ITEM_TYPE_NONE,
    LIST_ORDER_END,
};
/// LIST_SORT_EQUIPPED_THEN_EQUIPMENT: the equipped items, the other equipment,
/// then the rest.
u8 gMenu_ListOrderEquippedThenEquipment[24] = {
    LIST_ORDER_EQUIPPED | ITEM_TYPE_WEAPON, LIST_ORDER_EQUIPPED | ITEM_TYPE_HELM,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_SHIELD, LIST_ORDER_EQUIPPED | ITEM_TYPE_ARMOR,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_BOOTS, LIST_ORDER_EQUIPPED | ITEM_TYPE_CLASS_CHANGE,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_PSYNERGY, LIST_ORDER_EQUIPPED | ITEM_TYPE_TRIDENT,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_RING, LIST_ORDER_EQUIPPED | ITEM_TYPE_SHIRT,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_ELEMENTAL_STAR,
    ITEM_TYPE_WEAPON, ITEM_TYPE_HELM, ITEM_TYPE_SHIELD, ITEM_TYPE_ARMOR,
    ITEM_TYPE_BOOTS, ITEM_TYPE_CLASS_CHANGE, ITEM_TYPE_PSYNERGY, ITEM_TYPE_TRIDENT,
    ITEM_TYPE_RING, ITEM_TYPE_SHIRT, ITEM_TYPE_ELEMENTAL_STAR,
    ITEM_TYPE_NONE, LIST_ORDER_END,
};
/// LIST_SORT_EQUIPPED_THEN_ITEMS: the equipped items, the items that are not
/// equipment, then the other equipment.
u8 gMenu_ListOrderEquippedThenItems[24] = {
    LIST_ORDER_EQUIPPED | ITEM_TYPE_WEAPON, LIST_ORDER_EQUIPPED | ITEM_TYPE_HELM,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_SHIELD, LIST_ORDER_EQUIPPED | ITEM_TYPE_ARMOR,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_BOOTS, LIST_ORDER_EQUIPPED | ITEM_TYPE_CLASS_CHANGE,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_PSYNERGY, LIST_ORDER_EQUIPPED | ITEM_TYPE_TRIDENT,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_RING, LIST_ORDER_EQUIPPED | ITEM_TYPE_SHIRT,
    LIST_ORDER_EQUIPPED | ITEM_TYPE_ELEMENTAL_STAR,
    ITEM_TYPE_NONE,
    ITEM_TYPE_WEAPON, ITEM_TYPE_HELM, ITEM_TYPE_SHIELD, ITEM_TYPE_ARMOR,
    ITEM_TYPE_BOOTS, ITEM_TYPE_CLASS_CHANGE, ITEM_TYPE_PSYNERGY, ITEM_TYPE_TRIDENT,
    ITEM_TYPE_RING, ITEM_TYPE_SHIRT, ITEM_TYPE_ELEMENTAL_STAR,
    LIST_ORDER_END,
};

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
/// The item commands' cursor x, three columns a row
s32 gItemMenu_CommandColumnXTable[6] = {
    86, 117, 166,
    86, 117, 166,
};
// clang-format on
#endif

// The command names' widths
#if (LANGUAGE == LANGUAGE_FRENCH)
u8 gStatusScreen_CursorWidths[5] = {6, 7, 7, 6, 6};
#else
u8 gStatusScreen_CursorWidths[5] = {9, 4, 6, 5, 7};
#endif

s32 gDjinnFigures_SpriteIds[4] = {0xf3, 0xf4, 0xf5, 0xf6};

u16 gData_08105a50[5] = {16, 1, 2, 15, 7};
