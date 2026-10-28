#ifndef ALCHEMY_M7_INTERFACES_H
#define ALCHEMY_M7_INTERFACES_H

#include "edition.h"
#include "render_input.h"

void PsynergyMenu_PositionOwnerEntry(struct WindowSprite **icons, s32 index,
                                     s32 x, s32 y, s32 columns);
void ItemMenu_PlaceIcon(struct WindowSprite **icons, s32 index, s32 x, s32 y,
                        s32 columns);
void Window_SetBounds(struct RenderInput *window, s32 x, s32 y, s32 width,
                      s32 height);

#endif
