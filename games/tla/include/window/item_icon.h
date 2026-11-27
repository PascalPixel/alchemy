#ifndef ALCHEMY_ITEM_ICON_H
#define ALCHEMY_ITEM_ICON_H

/* Item and ability icons as window sprites, and the shops' grid of item
   icons. */

#include "types.h"
#include "system.h"
#include "graphics/vram_tab.h"

/* An icon slot that holds no VRAM cache entry. */
#define ICON_NONE VRAM_CACHE_ENTRY_COUNT

/* An icon sprite's OAM attributes, as OAM lays them out. */
typedef struct ICON_ATTR {
	Uint8  y;
	Uint8  attr0High;
	Uint16 x : 9;
	Uint16 attr1High : 7;
	Uint16 tile : 10;
	Uint16 attr2High : 6;
	Uint16 affineParam; /* OAM's fourth halfword, the affine parameter slot */
} ICON_ATTR;

/* An icon's sprite: its link in the sprite list and its OAM attributes. */
typedef struct ICON_SPR {
	Uint32	  link;
	ICON_ATTR attribute;
} ICON_SPR;

void mnItemIconLoadLayersIntoSlotStub(
	Sint32 item, Sint32 mode, Sint32 resource);

void mnItemIconGridDraw(void);
void mnItemIconGridSetPos(Sint32 index, Sint32 x, Sint32 y);
void mnItemIconGridSetVisible(Sint32 active);
void mnItemIconGridLoadItems(Uint16 *items, Sint32 count, Sint32 mode);

/* The compressed item icons, indexed by an item's icon field. */
extern Uint8 *ItemIcons[];
/* The end of ItemIcons, so mnIconCountItemIcons can count them. */
extern Uint8 *ItemIconsEnd[];
/* The other compressed icons, among them the Status screen's status icons. */
extern Uint8 *IconMiscellaneousIconPointers[];
/* The compressed Psynergy icons, drawn over the shared icon base. */
extern Uint8 *PsynergyIcons[];
/* The end of PsynergyIcons, for mnIconCountPsynergyIcons. */
extern Uint8 *PsynergyIconsEnd[];
/* Draws one of the other icons into the glyph buffer. */
void mnIconDrawMiscellaneous(Uint32 index);
/* Builds a command icon, overlayIcon over baseIcon, and loads it into a sprite
   tile slot: *slot, or a free one when reuse is 0. */
void mnIconBuildCommandIconTiles(Sint32 overlayIcon, Sint32 baseIcon,
	Sint32 *slot, Sint32 *tile, Sint32 reuse);

#endif
