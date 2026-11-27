#ifndef ALCHEMY_ITEM_ICON_H
#define ALCHEMY_ITEM_ICON_H

/* @file */
/* Item and ability icons as window sprites, and the shops' grid of item */
/* icons. */

#include "types.h"
#include "system.h"
#include "graphics/vram_tab.h"

/* An icon slot that holds no VRAM cache entry. */
#define ICON_NONE VRAM_CACHE_ENTRY_COUNT

/* An icon sprite's OAM attributes, as OAM lays them out. */
typedef struct ICON_ATTR {
	u8 y;
	u8 attr0High;
	u16 x : 9;
	u16 attr1High : 7;
	u16 tile : 10;
	u16 attr2High : 6;
	u16 affineParam; /* OAM's fourth halfword, the affine parameter slot */
} ICON_ATTR;

/* An icon's sprite: its link in the sprite list and its OAM attributes. */
typedef struct ICON_SPR {
	u32 link;
	ICON_ATTR attr;
} ICON_SPR;

void mnItemIconLoadLayersIntoSlotStub(s32 item, s32 mode, s32 resource);

void mnItemIconGridDraw(void);
void mnItemIconGridSetPos(s32 index, s32 x, s32 y);
void mnItemIconGridSetVisible(s32 active);
void mnItemIconGridLoadItems(u16 *items, s32 count, s32 mode);

/* The compressed item icons, indexed by an item's icon field. */
extern u8 *ItemIcons[];
/* The end of ItemIcons, so mnIconCountItemIcons can count them. */
extern u8 *ItemIconsEnd[];
/* The other compressed icons, among them the Status screen's status icons. */
extern u8 *Icon_MiscIconPointers[];
/* The compressed Psynergy icons, drawn over the shared icon base. */
extern u8 *PsynergyIcons[];
/* The end of PsynergyIcons, for mnIconCountPsynergyIcons. */
extern u8 *PsynergyIconsEnd[];
/* Draws one of the other icons into the glyph buffer. */
void mnIconDrawMisc(u32 index);
/* Builds a command icon, overlayIcon over baseIcon, and loads it into a sprite
 */
/* tile slot: *slot, or a free one when reuse is 0. */
void mnIconBuildCommandIconTiles(s32 overlayIcon, s32 baseIcon, s32 *slot,
								 s32 *tile, s32 reuse);

#endif
