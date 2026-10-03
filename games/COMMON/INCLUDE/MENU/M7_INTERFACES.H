#ifndef ALCHEMY_M7_INTERFACES_H
#define ALCHEMY_M7_INTERFACES_H

#include "RENDER_INPUT.H"

void PsynergyMenu_PositionOwnerEntry(
    struct RenderOutput **icons, s32 index, s32 x, s32 y, s32 columns);
void ItemMenu_PosOwner(
    struct RenderOutput **icons, s32 index, s32 x, s32 y, s32 columns);
void UiWindow_SetBounds(
    struct RenderInput *window, s32 x, s32 y, s32 width, s32 height);

#endif
