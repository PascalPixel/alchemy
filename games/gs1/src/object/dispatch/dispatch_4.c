#include "scene.h"
#include "object_dispatch.h"
#include "types.h"

/* object/dispatch/register_child_metadata.c */
void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *object, s32 value)
{
    if (object != 0 && (object->kind & 0xf) == 1) {
        void *child = object->target.child;
        if (value >= 0)
            Obj_Check((s32)child);
    }
}

/* object/dispatch/init_from_table5_with_argument.c */
void ObjectDispatch_InitFromTable5WithArgument(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, (u32)gRom);
    object->argument = argument;
}

/* object/dispatch/init_from_table0.c */
void ObjectDispatch_InitFromTable0(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)gRom);
}

/* object/dispatch/init_from_table1.c */
void ObjectDispatch_InitFromTable1(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)gRom);
}

/* object/dispatch/init_from_table2.c */
void ObjectDispatch_InitFromTable2(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)gRom);
}

/* object/dispatch/init_from_table3.c */
void ObjectDispatch_InitFromTable3(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)gRom);
}

/* object/dispatch/init_from_table6.c */
void ObjectDispatch_InitFromTable6(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)gRom);
}

/* object/dispatch/init_from_table4_with_argument.c */
void ObjectDispatch_InitFromTable4WithArgument(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, (u32)gRom);
    if (argument != 0) {
        object->value_34 = 0x8000;
        object->value_30 = 0x40000;
        object->argument = argument;
        object->value_64 = 0;
    }
}

/* object/dispatch/wait_for_value_16.c */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void WaitFrames(s32);

void ObjectDispatch_WaitForValue16(void *obj)
{
    s32 cnt;

    cnt = 0;
    if (*(s32 *)((FIELD(obj, s16 *, 4) * 4) + FIELD(obj, s32 *, 0)) != 0x10) {
        do {
            WaitFrames(1);
            cnt++;
            if (cnt > 0x12B) {
                break;
            }
        } while (*(s32 *)((FIELD(obj, s16 *, 4) * 4) + FIELD(obj, s32 *, 0)) != 0x10);
    }
}

/* object/dispatch/set_single_child_field26.c */
void ObjectDispatch_SetSingleChildField26(u8 *arg0, u32 arg1)
{
    if (arg0 != NULL) {
        if ((arg0[0x54] & 0xf) == 1)
            (*(u8 **)(arg0 + 0x50))[0x26] = arg1;
    }
}
