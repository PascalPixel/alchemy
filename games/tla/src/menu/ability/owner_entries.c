#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "m7_interfaces.h"


void PsynergyMenu_PositionOwnerEntry(struct RenderOutput **slot, s32 index,
    s32 originX, s32 originY,
    s32 columns) {
    struct RenderOutput *object;

    if (index > 0x1F) {
        index = 0;
    }
    object = *slot;
    object->y = (index / columns) * 0x10 + originY;
    object->x = (index % columns) * 0x10 + originX;
    UiIcon_PrepareObject(object);
}
