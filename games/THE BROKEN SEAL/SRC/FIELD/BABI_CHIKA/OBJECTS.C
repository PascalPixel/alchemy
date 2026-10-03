#include "FIELDOBJ.H"
/* Overlay object setup. */
#include "BABI.H"

void OverlayObject_SetOwnerMode(u8 *object, s32 mode)
{
    struct FieldActor *actor = (struct FieldActor *)object;

    actor->sprite->priority = mode;
}

/* Create an object with sprite priority zero and child value 14. */
u8 *OverlayObject_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind)
{
    struct FieldActor *ret;
    struct FieldActor *obj = Engine_ObjectCreate(kind, x, y, z);

    if (obj != 0) {
        struct FieldSprite *owner = obj->sprite;
        owner->priority = 0;
        obj->motion_flags = 0;
        obj->collision_flags = 8;
        Engine_ActorSetSpriteFlags(obj, 0);
        ObjectGroup_SetChildValue(obj, 14);
        Engine_ObjectSetBlendMode(obj, 1);
        ret = obj;
    } else {
        ret = 0;
    }
    return (u8 *)ret;
}

/* Create an underfoot object with sprite priority one and child value 15. */
u8 *OverlayObject_PrepareSpawnedObjectMode4(s32 x, s32 y, s32 z, s32 kind)
{
    struct FieldActor *ret;
    struct FieldActor *obj = Engine_ObjectCreate(kind, x, y, z);

    if (obj != 0) {
        struct FieldSprite *rec = obj->sprite;
        rec->priority = 1;
        obj->motion_flags = 0;
        obj->collision_flags = 8;
        Engine_ActorSetSpriteFlags(obj, 0);
        ObjectGroup_SetChildValue(obj, 15);
        obj->priority_flags = (obj->priority_flags & 0xfe) | 2;
        ret = obj;
    } else {
        ret = 0;
    }
    return (u8 *)ret;
}
