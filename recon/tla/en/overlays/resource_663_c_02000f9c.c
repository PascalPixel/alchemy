/* Near miss, differ 2 of 51 (one reordered pair): in the first branch the
 * native starts the scale step constant (movs r2, #192) before it loads
 * scale_x; the draft loads scale_x first. Without the one-pass block after
 * the speed store the constant is started before the store instead
 * (differ 4). A step temporary was tried. */
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);

void Func_02000f9c(struct ObjectRuntime *object)
{
    s32 scale;

    object->y += object->gravity;
    object->x += object->bounce;
    object->z += object->unknown_4c;
    if ((object->x >> 19) <= 39) {
        object->bounce += 0x18000;
        /* FAKEMATCH: the loop note ends the scheduling block after the speed store; without it the step constant is started before the store (one reordered pair twice). */
        do {
        } while (0);
        scale = object->scale_x + 0xccc;
    } else {
        object->bounce -= 0x18000;
        scale = object->scale_x - 0x51e;
    }
    object->scale_x = scale;
    object->scale_y = scale;
    if (object->bounce > 0)
        Object_SetSpritePriority(object, 1);
    else
        Object_SetSpritePriority(object, 2);
    ((struct FieldSprite *)object->animation)->rotation += 0x1800;
}
