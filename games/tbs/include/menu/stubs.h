#ifndef ALCHEMY_MENU_STUBS_H
#define ALCHEMY_MENU_STUBS_H

// The menu module's stub calls: the Item, Psynergy, Djinn and Status screens and the party member
// chooser.

Sint32 mnItemMenuRunStub(void);
Sint32 mnPsynergyMenuRunStub(void);
Sint32 mnStatusScreenRunStub(void);
Sint32 mnPartyMemberChooserOpenStub(void);
void   mnCreatePartyFiguresStub(
      void *src, Sint32 originX, Sint32 originY, Sint32 spacing, Sint32 unused);
void   mnReleasePartyFiguresStub(void);
void   mnIconShowStub(struct WINDOW_SPR *object);
Sint32 mnDjinnMenuRunStub(void);
void   mnItemMenuDrawItemDetailsStub(Sint32 window, Sint32 inventoryEntry);

#endif
