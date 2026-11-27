#ifndef ALCHEMY_MENU_STUBS_H
#define ALCHEMY_MENU_STUBS_H

/* @file */
/* The menu module's stub calls: the Item, Psynergy, Djinn and Status screens */
/* and the party member chooser. */

s32 mnItemMenuRunStub(void);
s32 mnPsynergyMenuRunStub(void);
s32 mnStatScrRunStub(void);
s32 mnPartyMemberChooserOpenStub(void);
void mnCreatePartyFiguresStub(void *source, s32 originX, s32 originY,
							  s32 spacing, s32 unused);
void mnReleasePartyFiguresStub(void);
void mnIconShowStub(struct WINDOW_SPR *object);
s32 mnDjinnMenuRunStub(void);
void mnItemMenuDrawItemDetailsStub(s32 window, s32 inventoryEntry);

#endif
