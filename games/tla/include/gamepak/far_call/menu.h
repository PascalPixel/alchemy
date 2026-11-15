#ifndef ALCHEMY_FAR_CALL_MENU_H
#define ALCHEMY_FAR_CALL_MENU_H

/// @file
/// The far calls into the menu bank, as the other banks' code calls them.

#include "types.h"

struct WindowSprite;
/// Turns an icon's window sprite on and copies its screen position into its OAM
/// attributes, with no affine matrix.
void Icon_PrepareActorFar(struct WindowSprite *object);
void ItemMenu_DrawItemDetailsFar(void *window, s32 item);
/// Describes an item (an inventory entry) in window, one 8-pixel row a line.
void ItemMenu_DrawItemDetailsWithCountFar(void *window, s32 item, s32 count);
void Palette_LoadIconsIntoHighlightFar(void);
void Menu_PlaceOwnerEntryObjectsFar(void *window, s32 a, s32 b);
void Menu_CreateOwnerEntryObjectsFar(s32 first);
void Menu_ReleaseEntryObjectsFar(void);

#endif
