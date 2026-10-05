#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "SCRIPT_OBJECT_RUNTIME.H"

struct ObjectSprite {
    u8 unknown_00[5];
    u8 attr0_high;
    u8 unknown_06;
    u8 attr1_high;
};

void GameFlag_SetBit(s32 flag);
void Func_080cee80(u32 object_id);
void Func_080e70f8(u32 object_id);

/* The shape bits of the object's sprite, 0 when it has none. */
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

/* Mode 1 moves the object to z 0. Mode 2 raises the flag, gives an object
   away from the origin its square sprite back, returns it to its home
   tile and settles it there. */
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
            Func_080cee80(object_id);
        object->x = object->home_x << 16;
        object->z = object->home_z << 16;
        Func_080e70f8(object_id);
    }
}
