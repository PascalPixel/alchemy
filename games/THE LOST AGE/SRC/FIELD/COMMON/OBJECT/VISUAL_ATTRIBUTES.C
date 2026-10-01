#include "OBJECT_RUNTIME.H"

/* The two priorities of the sprite an object draws. */
struct ObjectSprite {
    u8 unknown_00[9];
    u8 unknown_09 : 2;
    u8 priority : 2;
    u8 unknown_09_high : 4;
    u8 unknown_0a[0x1b];
    u8 unknown_25 : 2;
    u8 part_priority : 2;
    u8 unknown_25_high : 4;
};

struct ObjectRuntime *Object_GetById(u32);
extern const u8 ObjectMotion_StepAngleScript[];

/* ☀️'s, with ⚓️'s object layout. */
void Object_ResetTargetAndSetMode1(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->target_x = 0x80000000;
        object->target_y = 0x80000000;
        object->target_z = 0x80000000;
        Object_ResetMotion(object);
        Object_SetMode(object, 1);
    }
}

void ObjectMotion_ArmCallback(s32 object_id, s32 angle, s32 wait)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->action = angle;
        Object_SetCallback(object, ObjectMotion_StepAngleScript);
        Battle_WaitMode0(wait);
    }
}

/* ⚓️'s variant of the above, refreshing the object's selector in place of
   the wait. */
void ObjectMotion_ArmCallbackAndRefresh(u32 object_id, s32 angle)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->action = angle;
        Object_SetCallback(object, ObjectMotion_StepAngleScript);
        Object_RefreshSelectorById(object_id);
    }
}

void ObjectMotion_SetActionVariant(u32 object_id, s32 priority)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL && (object->animation_kind & 0xF) == 1) {
        struct ObjectSprite *sprite = object->animation;

        sprite->priority = priority;
        sprite->part_priority = priority;
        object->unknown_23 &= ~1;
    }
}

void ObjectVisual_CopyAttributes(u32 target_id, u32 source_id)
{
    void *p;
    u8 flags;
    u32 shape;
    u32 dst_attr;
    u32 merged;

    p = Object_GetById(source_id);
    p = *(void **)((u8 *)p + 0x50);
    flags = *(u8 *)((u8 *)p + 0x10);
    shape = *(u16 *)((u8 *)p + 0x8);

    p = Object_GetById(target_id);
    p = *(void **)((u8 *)p + 0x50);
    dst_attr = *(u16 *)((u8 *)p + 0x8);
    *(u8 *)((u8 *)p + 0x10) = flags;
    shape <<= 22;
    shape >>= 22;
    merged = 0xfffffc00;
    merged &= dst_attr;
    merged |= shape;
    *(u16 *)((u8 *)p + 0x8) = merged;
}
