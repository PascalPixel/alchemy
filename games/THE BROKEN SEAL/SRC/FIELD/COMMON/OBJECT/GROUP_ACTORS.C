#include "OBJECT_RUNTIME.H"
#include "GLOBAL_CELLS.H"

struct ObjectRuntime *Object_CreateFar(s32, s32, s32, s32);
void Object_Destroy(struct ObjectRuntime *);
void ObjectDispatch_SetSingleChildField26Far(struct ObjectRuntime *, s32);
void Object_SetMode(struct ObjectRuntime *, s32);
void ObjectDispatch_ApplyValueToChildrenFar(struct ObjectRuntime *, s32);
extern u8 gPlayerObjectId[];

struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z)
{
    u8 *base = *(u8 **)ADDR_03001F30;
    struct ObjectRuntime *object;
    u8 *child;
    u8 flag;

    object = Object_CreateFar(kind, x, y, z);
    if (object != NULL) {
        if (object->animation_kind == 0) {
            Object_Destroy(object);
            return NULL;
        }
        object->terrain_height = *(s32 *)(*(u8 **)(base + 16) + 20);
        flag = 4;
        object->flags = flag;
        object->unknown_23 = flag;
        child = object->animation;
        child[9] &= ~(flag + 8);
        ObjectDispatch_SetSingleChildField26Far(object, 0);
        Object_SetMode(object, 1);
    }
    return object;
}

void ObjectGroup_SetActionForOthers(struct ObjectRuntime *excluded_object,
                                  s32 group_mode, s32 action)
{
    s16 *active_object_id;
    struct ObjectRuntime *object;
    s32 object_id;

    object_id = 0;
    active_object_id = (s16 *)gPlayerObjectId;
    do {
        object = ObjectTable_Get(object_id);
        if (object_id != *active_object_id && object != NULL && object != excluded_object) {
            object->movement_state = group_mode;
            ObjectDispatch_ApplyValueToChildrenFar(object, action);
        }
        object_id++;
    } while (object_id <= 0x42);
}
