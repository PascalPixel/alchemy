#ifndef ALCHEMY_M7_INTERFACES_H
#define ALCHEMY_M7_INTERFACES_H

#include "types.h"

struct WindowBounds {
    u8 padding_00[8];
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
};

struct Object080a1c {
    u8 padding_00[6];
    s16 x;
    s16 y;
};

void PsynergyMenu_PositionOwnerEntry(
    struct Object080a1c **, s32, s32, s32, s32);
void InventoryMenu_PositionOwnerEntry(
    struct Object080a1c **, s32, s32, s32, s32);
void WindowBounds_Set(
    struct WindowBounds *, s32, s32, s32, s32);

#endif
