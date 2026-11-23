#ifndef ALCHEMY_FAR_CALL_MENU_H
#define ALCHEMY_FAR_CALL_MENU_H

/// @file
/// The far calls into the menu bank, as the other banks' code calls them.

#include "types.h"

struct WindowSprite;
/// Turns an icon's window sprite on and copies its screen position into its OAM
/// attributes, with no affine matrix.
void Icon_PrepareActorFar(struct WindowSprite *object);
/// Describes item in window with the entry's own count.
void ItemMenu_DrawItemDetailsFar(void *window, s32 item);
/// Describes an item (an inventory entry) in window, one 8-pixel row a line,
/// with count as its count.
void ItemMenu_DrawItemDetailsWithCountFar(void *window, s32 item, s32 count);
/// Lends HIGHLIGHT_PALETTE to the element icons.
void Palette_LoadIconsIntoHighlightFar(void);
/// Creates the first four party members' figures in a row from tile (originX,
/// originY) inside window, three tiles apart.
void Menu_PlaceOwnerEntryObjectsFar(void *window, s32 originX, s32 originY);
/// Replaces the menu's party figures with up to four party members' figures,
/// from party place first on.
void Menu_CreateOwnerEntryObjectsFar(s32 first);
/// Destroys the menu's party figures and stops placing them.
void Menu_ReleaseEntryObjectsFar(void);

#endif
