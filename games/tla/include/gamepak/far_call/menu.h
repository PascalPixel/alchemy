#ifndef ALCHEMY_FAR_CALL_MENU_H
#define ALCHEMY_FAR_CALL_MENU_H

/// @file
/// The far calls into the menu bank, as the other banks' code calls them.

#include "types.h"

struct WindowSprite;
void Icon_PrepareActorFar(struct WindowSprite *object);
void ItemMenu_DrawItemDetailsFar(void *window, s32 item);
void ItemMenu_DrawItemDetailsWithCountFar(void *window, s32 item, s32 count);
void Palette_LoadIconsIntoHighlightFar(void);
void Menu_PlaceOwnerEntryObjectsFar(void *window, s32 a, s32 b);
void Menu_CreateOwnerEntryObjectsFar(s32 first);
void Menu_ReleaseEntryObjectsFar(void);

#endif
