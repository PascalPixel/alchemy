#include "OBJECT_RUNTIME.H"
extern u8 Data_03001f30[];

struct ObjectRuntime *Object_CreateFar(s32, s32, s32, s32);
void Object_Destroy(struct ObjectRuntime *);
void ObjectDispatch_SetSingleChildField26Far(struct ObjectRuntime *, s32);
void Object_SetMode(struct ObjectRuntime *, s32);
void ObjectDispatch_ApplyValueToChildrenFar(struct ObjectRuntime *, s32);
extern u8 gPlayerObjectId[];

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
