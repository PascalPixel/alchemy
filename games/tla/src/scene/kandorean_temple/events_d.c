#include "local.h"

void Func_020006b0(struct ObjectRuntime *object)
{
    struct ObjectRuntime *steam = Func_02005140_(0x10d, object->x, object->y, object->z);
    struct FieldSprite *sprite;

    if (steam) {
        sprite = steam->animation;
        Func_02005138(steam, gData_020054e4);
        steam->motionFlags = 0;
        steam->terrainId = 1;
        steam->priorityFlags = 2;
        if (sprite) {
            Animation_ApplyChildArgument(sprite, 2);
            sprite->blendMode = 1;
            sprite->nextFrame = 0;
            sprite->priority = 3;
        }
    }
}

void Func_02000710(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
    struct ObjectRuntime *steam;

    if ((gFrameCount & 7) == 0) {
        steam = Func_02005140(14, leader->x, leader->y + 0x60000, leader->z);
        if (steam) {
            ObjectDispatch_SetSingleChildFlags(steam, 0);
            Func_02005130(steam, 1);
            Func_02005138(steam, gData_020054f0);
        }
    }
    Func_02000778(leader, 0xd999);
}
