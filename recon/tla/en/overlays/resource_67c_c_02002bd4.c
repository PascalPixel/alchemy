/* Draft, differ 8 of 46: the listing loads sprite (child->animation) right after object->terrain_height, before the store and the Data_0200411c pool load; the draft loads the pool first and the sprite after the store, and copies the priority bits one load later. Tried the scene-layer inlines and a one-pass block. */
#include "../../../../games/tla/src/field/overlay_67c/local.h"

void Func_02002bd4(struct ObjectRuntime *object)
{
    struct ObjectRuntime *child = Engine_ObjectCreate(14, object->x, object->y + 0x40000, object->z + 0x40000);
    struct FieldSprite *source = object->animation;

    if (child != 0) {
        struct FieldSprite *sprite = child->animation;

        child->terrain_height = object->terrain_height;
        Func_02003ccc(child, Data_0200411c);
        child->motion_flags = 0;
        if (sprite != 0) {
            Animation_ApplyChildArgument(sprite, 1);
            sprite->next_frame = 0;
            sprite->priority = source->priority;
        }
    }
}
