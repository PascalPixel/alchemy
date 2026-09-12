#include "object_dispatch.h"

void ObjectDispatch_InitFromTable5WithArgument(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, (u32)Data_08013608);
    object->argument = argument;
}

void ObjectDispatch_InitFromTable0(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)Data_08013590);
}

void ObjectDispatch_InitFromTable1(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)Data_080135a8);
}

void ObjectDispatch_InitFromTable2(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)Data_080135c0);
}

void ObjectDispatch_InitFromTable3(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)Data_080135d8);
}

void ObjectDispatch_InitFromTable6(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)Data_08013620);
}

void ObjectDispatch_InitFromTable4WithArgument(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, (u32)Data_080135f0);
    if (argument != 0) {
        object->value_34 = 0x8000;
        object->value_30 = 0x40000;
        object->argument = argument;
        object->value_64 = 0;
    }
}
