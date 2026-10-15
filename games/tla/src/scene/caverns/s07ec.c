#include "caverns.h"
#include "field_sprite.h"

void Func_020007ec(struct ObjectRuntime *object)
{
    object->x += object->bounce;
    object->y += object->gravity;
    object->z += object->unk4c;
    object->bounce -= object->bounce / 12;
    object->unk4c -= object->unk4c / 10;
    object->scaleX += object->speedLimit;
    object->scaleY += object->acceleration;
    /* FAKEMATCH: a one-pass block keeps the sprite load after the scaleY store (differ 6 without it). */
    do
    {
    } while (0);
    ((struct FieldSprite *)object->animation)->rotation += object->action;
}
