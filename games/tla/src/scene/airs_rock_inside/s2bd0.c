#include "airs_rock_inside.h"

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

void Func_02002bd0(struct FieldActor *object)
{
    s32 radius = ((object->y - object->terrain_height) >> 1) + 0x100000;

    object->x = WORD_AT(object, 68) + Iwram_MulQ16(Math_Cosine((WORD_AT(object, 48) & 255) << 11), radius);
    object->z = WORD_AT(object, 76) + Iwram_MulQ16(Math_Sine((WORD_AT(object, 48) & 255) << 11), radius) / 3;
    object->y += WORD_AT(object, 72);
    WORD_AT(object, 48)++;
}
