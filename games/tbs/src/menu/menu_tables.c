/* The menu tables: cursor bobbing, sort orders, highlight widths and resource
   ids. None of these is const: the code that reads them declares them without
   it. */

#include "field/field.h"
#include "game/item.h"
#include "menu/menu.h"

/* The hand cursor's bobbing offsets in pixels, which mnCursorPos adds to its x
   at step (FrameCount >> 1) & 7; the ninth entry is never read. */
Uint8 MenuCursor_BobX[9] = {0, 1, 1, 2, 2, 1, 1, 0, 0};
/* The same for y: in the cursor's 8-bit OAM y, -1 and -2 wrap to one and two
   pixels up. */
Uint8 MenuCursor_BobY[9] = {0, -1, -1, -2, -2, -1, -1, 0, 0};

/* The item menu's sort orders (LIST_SORT, mnCopyListOrder): one item type per
   byte, with LIST_ORDER_EQUIPPED for only the equipped items of that type,
   ending with LIST_ORDER_END. Every order also lists the item type between
   ITEM_TYPE_PSYNERGY and ITEM_TYPE_RING, which no item has and no ITEM_TYPE
   enumerator names. */
/* clang-format off */
/* LIST_SORT_ITEMS_FIRST: the items that are not equipment first. */
Uint8 Menu_ListOrderItemsFirst[11] = {
	ITEM_TYPE_NONE, ITEM_TYPE_WEAPON, ITEM_TYPE_HELM, ITEM_TYPE_SHIELD,
	ITEM_TYPE_ARMOR, ITEM_TYPE_BOOTS, ITEM_TYPE_PSYNERGY, ITEM_TYPE_UNUSED, ITEM_TYPE_RING,
	ITEM_TYPE_SHIRT, LIST_ORDER_END,
};
/* LIST_SORT_EQUIPMENT_FIRST: the equipment first, the rest last. */
Uint8 Menu_ListOrderEquipmentFirst[11] = {
	ITEM_TYPE_WEAPON, ITEM_TYPE_HELM, ITEM_TYPE_SHIELD, ITEM_TYPE_ARMOR,
	ITEM_TYPE_BOOTS, ITEM_TYPE_PSYNERGY, ITEM_TYPE_UNUSED, ITEM_TYPE_RING, ITEM_TYPE_SHIRT,
	ITEM_TYPE_NONE, LIST_ORDER_END,
};
/* LIST_SORT_EQUIPPED_THEN_EQUIPMENT: the equipped items, the other equipment, */
/* then the rest. */
Uint8 Menu_ListOrderEquippedThenEquipment[20] = {
	LIST_ORDER_EQUIPPED | ITEM_TYPE_WEAPON, LIST_ORDER_EQUIPPED | ITEM_TYPE_HELM,
	LIST_ORDER_EQUIPPED | ITEM_TYPE_SHIELD, LIST_ORDER_EQUIPPED | ITEM_TYPE_ARMOR,
	LIST_ORDER_EQUIPPED | ITEM_TYPE_BOOTS, LIST_ORDER_EQUIPPED | ITEM_TYPE_PSYNERGY,
	LIST_ORDER_EQUIPPED | ITEM_TYPE_UNUSED, LIST_ORDER_EQUIPPED | ITEM_TYPE_RING,
	LIST_ORDER_EQUIPPED | ITEM_TYPE_SHIRT,
	ITEM_TYPE_WEAPON, ITEM_TYPE_HELM, ITEM_TYPE_SHIELD, ITEM_TYPE_ARMOR,
	ITEM_TYPE_BOOTS, ITEM_TYPE_PSYNERGY, ITEM_TYPE_UNUSED, ITEM_TYPE_RING, ITEM_TYPE_SHIRT,
	ITEM_TYPE_NONE, LIST_ORDER_END,
};
/* LIST_SORT_EQUIPPED_THEN_ITEMS: the equipped items, the items that are not */
/* equipment, then the other equipment. */
Uint8 Menu_ListOrderEquippedThenItems[20] = {
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

#if LANGUAGE != LANGUAGE_JAPANESE
/* The x in pixels of the item command cursor, three columns on each of two */
/* rows (mnItemMenuCommandCursorX; the Japanese edition has neither). */
Sint32 ItemMenu_CommandColumnXTable[6] = {
	86, 117, 166,
	86, 117, 166,
};
/* clang-format on */
#endif

/* The width in tiles of the Status screen's highlight on each status row (Down,
   Poison, Venom, Curse and Haunt). */
#if LANGUAGE == LANGUAGE_JAPANESE
Uint8 StatusScreen_CursorWidths[5] = {9, 4, 6, 5, 7};
#elif LANGUAGE == LANGUAGE_GERMAN
Uint8 StatusScreen_CursorWidths[5] = {7, 5, 7, 6, 5};
#else
Uint8 StatusScreen_CursorWidths[5] = {6, 7, 7, 6, 6};
#endif

/* The sprites of the four Djinn DjinnFigures shows: a Venus, a Mercury, a Mars
   and a Jupiter Djinni. */
Sint32 DjinnFigures_SpriteIds[4] = {SPRITE_VENUS_DJINNI, SPRITE_MERCURY_DJINNI,
	SPRITE_MARS_DJINNI, SPRITE_JUPITER_DJINNI};
