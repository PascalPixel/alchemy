#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "object_runtime.h"
#include "script_object_runtime.h"

struct ObjectSprite {
    u8 unk00[5];
    u8 attr0High;
    u8 unk06;
    u8 attr1High;
};

void GameFlag_SetBit(s32 flag);

s32 Object_GetSpriteShape(u32 objectId)
{
    struct ObjectRuntime *object;
    struct ObjectSprite *sprite;
    s32 shape = 0;

    object = ObjectTable_Get(objectId);
    if (object != 0) {
        sprite = object->animation;
        if (sprite != 0)
            shape = sprite->attr0High >> 6;
    }
    return shape;
}

void Object_ReturnHome(s32 mode, u32 objectId, s32 flag)
{
    struct ScriptObjectRuntime *object;

    object = (struct ScriptObjectRuntime *)ObjectTable_Get(objectId);
    if (object == 0)
        return;
    if (mode == 1)
        object->z = 0;
    if (mode == 2) {
        GameFlag_SetBit(flag);
        if (object->x != 0)
            Object_SetSquareSprite(objectId);
        object->x = object->homeX << 16;
        object->z = object->homeZ << 16;
        Func_080e70f8(objectId);
    }
}
