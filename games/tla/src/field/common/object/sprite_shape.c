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
    u8 unknown_00[5];
    u8 attr0_high;
    u8 unknown_06;
    u8 attr1_high;
};

void GameFlag_SetBit(s32 flag);

s32 Object_GetSpriteShape(u32 object_id)
{
    struct ObjectRuntime *object;
    struct ObjectSprite *sprite;
    s32 shape = 0;

    object = ObjectTable_Get(object_id);
    if (object != 0) {
        sprite = object->animation;
        if (sprite != 0)
            shape = sprite->attr0_high >> 6;
    }
    return shape;
}

void Object_ReturnHome(s32 mode, u32 object_id, s32 flag)
{
    struct ScriptObjectRuntime *object;

    object = (struct ScriptObjectRuntime *)ObjectTable_Get(object_id);
    if (object == 0)
        return;
    if (mode == 1)
        object->z = 0;
    if (mode == 2) {
        GameFlag_SetBit(flag);
        if (object->x != 0)
            Object_SetSquareSprite(object_id);
        object->x = object->home_x << 16;
        object->z = object->home_z << 16;
        Func_080e70f8(object_id);
    }
}
