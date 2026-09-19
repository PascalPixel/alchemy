#include "OBJECT_DISPATCH.H"

void ObjectDispatch_InitFromTable6(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)ObjectDispatch_Table6);
}

void ObjectDispatch_InitFromTable4WithArgument(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, (u32)ObjectDispatch_Table4);
    if (argument != 0) {
        object->value_34 = 0x8000;
        object->value_30 = 0x40000;
        object->argument = argument;
        object->value_64 = 0;
    }
}

void ObjectDispatch_WaitForCommandEnd(struct DispatchObject *object)
{
    s32 cnt = 0;

    if (object->value_00 != 0 &&
        ((const s32 *)object->value_00)[object->value_04] != 17) {
        do {
            WaitFrames(1);
            cnt++;
            if (cnt > 599)
                break;
        } while (object->value_00 != 0 &&
                 ((const s32 *)object->value_00)[object->value_04] != 17);
    }
}

void ObjectDispatch_SetSingleChildField1a(struct DispatchObject *object, u32 value)
{
    if (object != NULL && (object->kind & 0xf) == 1)
        object->target.child->value_1a = value;
}

void Animation_SetStateFlags(struct DispatchObject *object, u32 value)
{
    if (object != NULL && object->kind == 1)
        object->target.child->state_flags = value;
}

void Animation_SetDisplayFlag(struct DispatchObject *object, u32 value)
{
    if (object != NULL && object->kind == 1)
        object->target.child->display_flag = value;
}

void Animation_ApplyChildValues(struct DispatchObject *object)
{
    if (object != NULL && object->kind == 1)
        Animation_ApplyChildValuesToRecord(object->target.child);
}
