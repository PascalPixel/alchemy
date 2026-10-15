/* Near miss of Func_02002bd4 (overlay 67C, 104 bytes), differ 8 of 46, order
   only: the native stores the copied terrain height before it loads the script
   address (str r3, [r5, #20]; ldr r1), and clears the frame before it copies
   the parent sprite out of r8 (strb r5, [r7, #26]; mov r2, r8); this form has
   both pairs the other way round. */
#include "../../../../../games/tla/src/scene/aqua_rock_inside/local.h"

void Func_02002bd4(struct ObjectRuntime *object)
{
    struct ObjectRuntime *child = Engine_ObjectCreate(14, object->x, object->y + 0x40000, object->z + 0x40000);
    struct FieldSprite *parent = object->animation;

    if (child != 0) {
        struct FieldSprite *sprite = child->animation;

        child->terrain_height = object->terrain_height;
        Func_02003ccc(child, Data_0200411c);
        child->motion_flags = 0;
        if (sprite != 0) {
            Animation_ApplyChildArgument(sprite, 1);
            sprite->next_frame = 0;
            sprite->priority = parent->priority;
        }
    }
}
