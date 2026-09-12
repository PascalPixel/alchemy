#include "scene.h"
#include "object_dispatch.h"
#include "types.h"

/* object/dispatch/dispatch_register_child_metadata.c */
/* object/dispatch/register_child_metadata.c */
void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *object, s32 value)
{
    if (object != 0 && (object->kind & 0xf) == 1) {
        void *child = object->target.child;
        if (value >= 0)
            ResourceMetadata_Register((s32)child);
    }
}

/* object/dispatch/init_from_table5_with_argument.c */
void ObjectDispatch_InitFromTable5WithArgument(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, (u32)RomBytes_08013608);
    object->argument = argument;
}

/* object/dispatch/init_from_table0.c */
void ObjectDispatch_InitFromTable0(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)RomBytes_08013590);
}

/* object/dispatch/init_from_table1.c */
void ObjectDispatch_InitFromTable1(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)RomBytes_080135a8);
}

/* object/dispatch/init_from_table2.c */
void ObjectDispatch_InitFromTable2(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)RomBytes_080135c0);
}

/* object/dispatch/init_from_table3.c */
void ObjectDispatch_InitFromTable3(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)RomBytes_080135d8);
}

/* object/dispatch/init_from_table6.c */
void ObjectDispatch_InitFromTable6(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)RomBytes_08013620);
}

/* object/dispatch/init_from_table4_with_argument.c */
void ObjectDispatch_InitFromTable4WithArgument(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, (u32)RomBytes_080135f0);
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

/* animation/set_state_field5_bits2_to3.c */
/* animation/set_state_field5_bits2_to3.c */
/* animation/set_state_field5_bits2_3.c */
struct Value_0800c548 {
    u8 padding[5];
    u8 unk_0 : 2;
    u8 field_2 : 2;
    u8 unk_4 : 4;
};

void Animation_SetStateField5Bits2To3(u8 *obj, u32 v)
{
    if (obj != 0 && obj[84] == 1) {
        struct Value_0800c548 *state = *(struct Value_0800c548 **)(obj + 80);
        state->field_2 = v;
    }
}

/* animation/set_state_field1d_bit1.c */
struct Value_0800c570 {
    u8 padding[29];
    u8 unk_0 : 1;
    u8 field_1 : 1;
    u8 unk_2 : 6;
};

void Animation_SetStateField1dBit1(u8 *obj, u32 v)
{
    if (obj != 0 && obj[84] == 1) {
        struct Value_0800c570 *state = *(struct Value_0800c570 **)(obj + 80);
        state->field_1 = v;
    }
}

/* animation/apply_child_values.c */
void Animation_ApplyChildValues(void *obj)
{
    if ((obj != NULL) && (FIELD_AT_OFFSET(obj, u8 *, 0x54) == 1)) {
        Sys_Check(FIELD_AT_OFFSET(obj, s32 *, 0x50));
    }
}

/* graphics/registers/enable_obj_layer_and_callbacks.c */

void Graphics_EnableObjLayerAndCallbacks(void)
{
    Sys_CheckLayerAndCallbacks((u32)Sys_Run);
    Sys_CheckLayerAndCallbacks((u32)Sys_unk2_4);
    Sys_Apply(0x10000, 1);
    BattleFx_StartBufferInterpolation(1);
    WaitFrames(1);
    *(u16 *)0x04000000 = (0xF1FF & *(u16 *)0x04000000) | 0x1000;
}

/* object/dispatch/stop_callbacks_and_hide_layers.c */
void ObjectDispatch_StopCallbacksAndHideLayers(void)
{
    Obj_Check((u32)Obj_Run);
    Obj_Check((u32)Obj_unk2_2);
    *(u16 *)0x04000000 &= 0xE1FF;
}
