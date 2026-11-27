#ifndef ALCHEMY_MENU_STUBS_H
#define ALCHEMY_MENU_STUBS_H

/* @file */
/* The stub calls into the menu module, as the other modules' code calls them.
 */

#include "types.h"

struct WINDOW_SPR;
/* Turns an icon's window sprite on and copies its screen position into its OAM
 */
/* attributes, with no affine matrix. */
void mnIconShowStub(struct WINDOW_SPR *object);
/* Describes item in window with the entry's own count. */
void mnItemMenuDrawItemDetailsStub(void *window, s32 item);
/* Describes an item (an inventory entry) in window, one 8-pixel row a line, */
/* with count as its count. */
void mnItemMenuDrawItemDetailsWithCountStub(void *window, s32 item, s32 count);
/* Lends HIGHLIGHT_PALETTE to the element icons. */
void btPaletteLoadIconsIntoHighlightStub(void);
/* Creates the first four party members' figures in a row from tile (originX, */
/* originY) inside window, three tiles apart. */
void mnShowPartyFiguresStub(void *window, s32 originX, s32 originY);
/* Replaces the menu's party figures with up to four party members' figures, */
/* from party place first on. */
void mnCreatePartyFiguresStub(s32 first);
/* Destroys the menu's party figures and stops placing them. */
void mnReleasePartyFiguresStub(void);

#endif
