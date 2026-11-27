// The menus' tables: the hand cursor's bob, the item list's sort orders, the cursor widths over the
// item commands and the Status screen's rows, and the sprites and icons the Djinn and Status
// screens load. The menus declare them without const, so they are not const here either.

#include "menu/menu.h"
#include "game/item.h"
#include "window/screens.h"
#include "sprite_ids.h"

// The hand cursor's bob in pixels, a step every two ticks: right and up by up to two and back
// (mnCursorPos). The ninth step is never reached.
Uint8 MenuCursorBobX[9] = {0, 1, 1, 2, 2, 1, 1, 0, 0};
// clang-format off
Uint8 MenuCursorBobY[9] = {0, (Uint8)-1, (Uint8)-1, (Uint8)-2, (Uint8)-2, (Uint8)-1, (Uint8)-1, 0, 0};

// The item menu's sort orders (LIST_SORT, mnCopyListOrder): one item type per byte, with LIST_ORDER_EQUIPPED for only the equipped items of that type, ending with LIST_ORDER_END. The class-changing items stand between the boots and the Psynergy items, and the Elemental Stars after the shirts. LIST_SORT_ITEMS_FIRST: the items that are not equipment first.
Uint8 MenuListOrderItemsFirst[13] = {
    ITEM_TYPE_NONE, ITEM_TYPE_WEAPON, ITEM_TYPE_HELM, ITEM_TYPE_SHIELD,
    ITEM_TYPE_ARMOR, ITEM_TYPE_BOOTS, ITEM_TYPE_CLASS_CHANGE, ITEM_TYPE_PSYNERGY,
    ITEM_TYPE_TRIDENT, ITEM_TYPE_RING, ITEM_TYPE_SHIRT, ITEM_TYPE_ELEMENTAL_STAR,
    LIST_ORDER_END,
};
// LIST_SORT_EQUIPMENT_FIRST: the equipment first, the rest last.
Uint8 MenuListOrderEquipmentFirst[13] = {
    ITEM_TYPE_WEAPON, ITEM_TYPE_HELM, ITEM_TYPE_SHIELD, ITEM_TYPE_ARMOR,
    ITEM_TYPE_BOOTS, ITEM_TYPE_CLASS_CHANGE, ITEM_TYPE_PSYNERGY, ITEM_TYPE_TRIDENT,
    ITEM_TYPE_RING, ITEM_TYPE_SHIRT, ITEM_TYPE_ELEMENTAL_STAR, ITEM_TYPE_NONE,
    LIST_ORDER_END,
};
// LIST_SORT_EQUIPPED_THEN_EQUIPMENT: the equipped items, the other equipment, then the rest.
Uint8 MenuListOrderEquippedThenEquipment[24] = {
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
// LIST_SORT_EQUIPPED_THEN_ITEMS: the equipped items, the items that are not equipment, then the other equipment.
Uint8 MenuListOrderEquippedThenItems[24] = {
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
// The hand's x at each of the six item commands, in their two rows of three (mnItemMenuGetCommandCursorX): the middle column stands 9 pixels left of the even 40-pixel spacing the other editions use.
Sint32 ItemMenuCommandColumnXTable[6] = {
    86, 117, 166,
    86, 117, 166,
};
#endif
// clang-format on

// The highlight's width in tiles over each label of the Status screen's rows
// (mnStatusScreenDrawSelectionCursor), fitted to each language's words.
#if (LANGUAGE == LANGUAGE_FRENCH)
Uint8 StatusScreenCursorWidths[5] = {6, 7, 7, 6, 6};
#else
Uint8 StatusScreenCursorWidths[5] = {9, 4, 6, 5, 7};
#endif

// The four Djinn figures' sprites, one for each element: Venus, Mercury, Mars and Jupiter
// (mnDjinnFiguresReplaceSlot).
Sint32 DjinnFiguresSpriteIds[4] = {
    SPRITE_VENUS_DJINNI, SPRITE_MERCURY_DJINNI, SPRITE_MARS_DJINNI, SPRITE_JUPITER_DJINNI};

// The status icons (mnIconLoadSlots).
Uint16 StatusScreenStatusIcons[5] = {OWNER_CONDITION_DOWNED,
                                     OWNER_CONDITION_POISON,
                                     OWNER_CONDITION_DEADLY_POISON,
                                     OWNER_CONDITION_CURSED_ITEM,
                                     OWNER_CONDITION_HAUNT};
