#include "OBJECT_RUNTIME.H"
#include "FIELD_EVENT.H"

void ObjectDispatch_InitializeFar(struct ObjectRuntime *, const void *);
void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetMode(struct ObjectRuntime *, s32);
void Battle_WaitMode0(s32);
extern const u8 ObjectMotion_StepAngleScript[];

void ObjectMotion_ArmCallback(s32 object_id, s32 angle, s32 wait)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->action = angle;
        ObjectDispatch_InitializeFar(object, ObjectMotion_StepAngleScript);
        Battle_WaitMode0(wait);
    }
}
