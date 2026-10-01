#include "TYPES.H"

/* The two bytes the lighthouse top's objects are set up with. */
struct OverlayObject {
    u8 unknown_00[0x55];
    u8 unknown_55;
    u8 unknown_56[3];
    u8 unknown_59;
};

void ObjectDispatch_SetSingleChildField26(struct OverlayObject *object, s32 value);
void Object_SetPartAttribute(struct OverlayObject *object, s32 value);

/* ☀️'s ConfigureOverlayObject, under ⚓️'s names for what it calls. */
void ConfigureOverlayObject(struct OverlayObject *object, s32 parameter)
{
    object->unknown_55 = 0;
    object->unknown_59 = 8;
    ObjectDispatch_SetSingleChildField26(object, 0);
    Object_SetPartAttribute(object, parameter);
}
