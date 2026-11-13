#ifndef ALCHEMY_FAR_CALL_MENU_H
#define ALCHEMY_FAR_CALL_MENU_H

/// @file
/// What games/tla/src/shop/ declares for the files that use it, written once.

#include "types.h"

struct WindowSprite;

void Icon_PrepareActorFar(struct WindowSprite *object);
void ItemMenu_DrawItemDetailsFar(void *window, s32 item);

#endif
