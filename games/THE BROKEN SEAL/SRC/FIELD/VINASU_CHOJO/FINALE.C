/* The closing scene step. */
#include "CHOJO.H"

void FieldScene_RunScene3c9_02005b90(union FieldObject *object)
{
    struct FieldEffect *anchor;
    s32 spin;

    anchor = *(struct FieldEffect **)((u8 *)object + 104);
    spin = object->effect.spin;
    object->effect.x = anchor->x + Math_Cos(spin) * (*(s32 *)((u8 *)object + 48) + 28);
    object->effect.z = (Math_Sin(spin) << 4) + 0xa40000;
    *(s32 *)((u8 *)object + 56) = object->effect.x;
    *(s32 *)((u8 *)object + 64) = object->effect.z;
    object->effect.spin -= 0x200;
}
