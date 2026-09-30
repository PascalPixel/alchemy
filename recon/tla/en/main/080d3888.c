#include "OBJECT_RUNTIME.H"
#include "FIELD_EVENT.H"

void ObjectDispatch_InitializeFar(struct ObjectRuntime *, const void *);
void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetMode(struct ObjectRuntime *, s32);
void Battle_WaitMode0(s32);
extern const u8 ObjectMotion_StepAngleScript[];

void ObjectMotion_SetActionVariant(s32 object_id, s32 priority)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL && (object->animation_kind & 0xF) == 1) {
        struct FieldSprite *sprite = object->animation;

        sprite->priority = priority;
        sprite->second_priority = priority;
        object->unknown_23 &= ~ACTOR_PRIORITY_AUTOMATIC;
    }
}
