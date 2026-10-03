#include "ENTRY_SETUP.H"

void ConfigureOverlayObject(struct OverlayObject *object, s32 parameter)
{
    object->unknown_55 = 0;
    object->unknown_59 = 8;
    Engine_ActorSetSpriteFlags(object, 0);
    ObjectGroup_SetChildValue(object, parameter);
}

void *OverlayObject_SpawnWithMode14(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    void *obj;
    u8 *p;
    s32 mask;
    u8 flag;

    obj = Engine_ObjectCreate(arg3, arg0, arg1, arg2);
    if (obj != 0) {
        p = *(u8 **)((u8 *)obj + 0x50);
        mask = 13;
        flag = p[9];
        mask = -mask;
        mask &= flag;
        p[9] = mask;
        ConfigureOverlayObject(obj, 0xE);
        Engine_ObjectSetBlendMode(obj, 1);
        return obj;
    }
    return 0;
}

void *OverlayObject_PrepareObjectWithCommand15(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    void *object;
    u8 *rec;
    s32 flags;
    s32 result;

    object = Engine_ObjectCreate(arg3, arg0, arg1, arg2);
    if (object != 0) {
        rec = *(u8 **)((u8 *)object + 0x50);
        flags = rec[9];
        flags = (flags & -13) | 4;
        rec[9] = flags;
        ConfigureOverlayObject(object, 0xF);
        flags = *((u8 *)object + 0x23);
        result = 2;
        result |= flags;
        *((u8 *)object + 0x23) = result;
        return object;
    }
    return 0;
}
