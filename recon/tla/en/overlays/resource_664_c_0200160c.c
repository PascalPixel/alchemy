/* Near miss of Func_0200160c (overlay 664, 132 bytes), differ 4 of 53, two
   loads one step late: the native loads the pooled zero of the two flag bytes
   before it finishes the timer address (adds r2, r5, #0; ldr r1; adds r2, #100)
   and the pooled 0xffff8001 before the mask is complete (lsls r3, #7; ldr r2;
   adds r3, #255); this form loads each after. Three minutes of permuter found
   nothing better. */
#include "../../../../games/tla/src/field/overlay_664/ov664.h"

s32 Random16Far(void);
void Func_02001458(void);

void Func_0200160c(s32 x, s32 y, s32 z)
{
    struct FieldActor *object = Engine_ObjectCreate(0x11d, x, y, z);

    Func_02005d5c(object, 1);
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Object_SetPartAttribute(object, 6);
    Animation_SetStateFlags(object, 1);
    object->timer = 27;
    object->motion_flags = 0;
    BYTE_AT(object, 90) = 0;
    WORD_AT(object, 24) = 0x8000;
    object->speed = 0x40000;
    object->acceleration = ((Random16Far() & 0x7fff) << 1) + 0xffff8001;
    WORD_AT(object, 76) = 0x3800;
    WORD_AT(object, 108) = (s32)Func_02001458;
}
