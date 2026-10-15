#include "shaman_village.h"

void Func_020042c8(struct ShamanVillageObject *object);

void Func_0200430c(s32 x, s32 y, s32 z)
{
    struct ShamanVillageObject *object = Engine_ObjectCreate(285, x, y, z);

    if (object != NULL) {
        object->field_5c = 2;
        object->motion_flags = 0;
        object->field_64 = 0;
        object->priority_flags &= ~1;
        Object_SetPartAttribute(object, 7);
        object->sprite->priority = 1;
        object->priority_flags |= 2;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        object->scale_x = 0x8000;
        object->scale_y = 0x8000;
        object->field_68 = 60;
        object->update = Func_020042c8;
        Engine_ObjectSetMode(object, 5);
        Animation_SetStateFlags(object, 1);
    }
}
