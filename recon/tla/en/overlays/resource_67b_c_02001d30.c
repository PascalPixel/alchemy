// differ 8: the listing loads the child's sprite between the load and the
// store of terrain_height and the pool address after them, and the parent
// sprite's attribute byte before the child's. Assigning the sprite after the
// terrain copy moves the parent sprite out of r8 (differ 28).
#include "../../../../games/tla/src/field/overlay_67b/local.h"

void Func_02001d30(struct ObjectRuntime *object)
{
    struct ObjectRuntime *child = Engine_ObjectCreate(14, object->x, object->y + 0x40000, object->z + 0x40000);
    struct FieldSprite *parent_sprite = object->animation;

    if (child != 0) {
        struct FieldSprite *sprite = child->animation;

        child->terrain_height = object->terrain_height;
        Func_02002918(child, Data_02002c44);
        child->motion_flags = 0;
        if (sprite != 0) {
            Animation_ApplyChildArgument(sprite, 1);
            sprite->next_frame = 0;
            sprite->priority = parent_sprite->priority;
        }
    }
}
