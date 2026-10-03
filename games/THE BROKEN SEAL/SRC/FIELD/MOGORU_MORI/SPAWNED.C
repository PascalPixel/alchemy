#include "FIELDOBJ.H"
#include "MORI.H"

void OverlayObject_SetOwnerMode(u8 *object, s32 mode)
{
    struct FieldActor *actor = (struct FieldActor *)object;

    actor->sprite->priority = mode;
}

/* Spawn a scene object with sprite priority zero and child value 14. */
u8 *OverlayObject_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 kind)
{
    struct FieldActor *ret;
    struct FieldActor *obj = Object_Create(kind, x, y, z);

    if (obj != 0) {
        struct FieldSprite *rec = obj->sprite;
        rec->priority = 0;
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

u8 *OverlayObject_SpawnConfiguredWithMode15(s32 x, s32 y, s32 z, s32 kind)
{
    struct FieldActor *ret;
    struct FieldActor *obj = Object_Create(kind, x, y, z);

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
