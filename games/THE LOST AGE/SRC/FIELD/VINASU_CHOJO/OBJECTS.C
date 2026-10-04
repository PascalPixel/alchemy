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

struct OverlayObject *Func_02001b3c(s32 kind, s32 x, s32 y, s32 z);
void Object_SetSpritePriority(struct OverlayObject *object, s32 priority);
void Animation_SetStateFlags(struct OverlayObject *object, s32 flags);

/* Creates one of the lighthouse top's objects at x, y, z with part
   attribute 14 and its animation flag set. */
struct OverlayObject *OverlayObject_SpawnWithMode14(s32 x, s32 y, s32 z, s32 kind)
{
    struct OverlayObject *object = Func_02001b3c(kind, x, y, z);

    if (object != 0) {
        Object_SetSpritePriority(object, 0);
        ConfigureOverlayObject(object, 14);
        Animation_SetStateFlags(object, 1);
        return object;
    }
    return 0;
}

/* Creates an object in front, with part attribute 15 and its second
   display flag set. */
struct OverlayObject *OverlayObject_PrepareObjectWithCommand15(s32 x, s32 y, s32 z, s32 kind)
{
    struct OverlayObject *object = Func_02001b3c(kind, x, y, z);

    if (object != 0) {
        Object_SetSpritePriority(object, 1);
        ConfigureOverlayObject(object, 15);
        {
            s32 bits = 2;
            u8 *flags = (u8 *)object + 35;
            bits |= *flags;
            *flags = bits;
        }
        return object;
    }
    return 0;
}
