#include "caverns.h"
#include "field_sprite.h"

void Func_020007ec(struct ObjectRuntime *object)
{
    object->x += object->bounce;
    object->y += object->gravity;
    object->z += object->unknown_4c;
    object->bounce -= object->bounce / 12;
    object->unknown_4c -= object->unknown_4c / 10;
    object->scale_x += object->speed_limit;
    object->scale_y += object->acceleration;
    /* FAKEMATCH: a one-pass block keeps the sprite load after the scale_y store (differ 6 without it). */
    do {
    } while (0);
    ((struct FieldSprite *)object->animation)->rotation += object->action;
}
