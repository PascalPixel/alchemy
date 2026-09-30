#include "OBJECT_DISPATCH.H"
#include "TYPES.H"
#include "SCENE.H"

s32 AnimationObjects_SelectAnimation(void *, s32);
void AnimationObjects_SetField15OnActive(void *, s32);

void ObjectDispatch_Initialize(struct DispatchObject *object, u32 value)
{
    if (object != 0) {
        object->value_04 = 0;
        object->value_00 = value;
        object->value_5b = 0;
        object->value_5d = 0;
        object->value_57 = 0;
    }
}

void ObjectDispatch_ApplyArgumentToChildren(void *raw_object, s32 argument)
{
    struct DispatchObject *object;
    void **items;
    s32 count;
    void *item;

    object = raw_object;
    if (object != 0) {
        switch (object->kind & 0xf) {
        case 1:
            AnimationObjects_SelectAnimation(object->target.child, argument);
            break;
        case 2:
            items = object->target.children;
            count = 3;
            do {
                item = *items++;
                if (item != 0) {
                    AnimationObjects_SelectAnimation(item, argument);
                }
                count--;
            } while (count >= 0);
            break;
        }
    }
}

void ObjectDispatch_ApplyValueToChildren(struct DispatchObject *object, s32 value)
{
    s32 count;
    void *child;
    void **children;

    if (object != 0) {
        switch (object->kind & 0xf) {
        case 1:
            AnimationObjects_SetField15OnActive(object->target.child, value);
            return;
        case 2:
            children = object->target.children;
            count = 3;
            do {
                child = *children++;
                if (child != 0)
                    AnimationObjects_SetField15OnActive(child, value);
                count--;
            } while (count >= 0);
            break;
        }
    }
}

void ObjectDispatch_ApplyPairToChildren(void *arg0, s32 arg1, s32 arg2)
{
    void **items;
    void *item;
    s32 count;

    if (arg0 != 0) {
        switch (*((u8 *)arg0 + 84) & 15) {
        case 1:
            AnimationObjects_SelectAnimation(*(void **)((u8 *)arg0 + 80), arg1);
            AnimationObjects_SetField15OnActive(*(void **)((u8 *)arg0 + 80), arg2);
            break;
        case 2:
            items = *(void ***)((u8 *)arg0 + 80);
            for (count = 3; count >= 0; count--) {
                item = *items++;
                if (item != 0) {
                    AnimationObjects_SelectAnimation(item, arg1);
                    AnimationObjects_SetField15OnActive(item, arg2);
                }
            }
            break;
        }
    }
}

void ObjectDispatch_SetChildField1e(struct DispatchObject *object, u32 value)
{
    if (object != 0 && (object->kind & 0xf) == 1)
        *(s16 *)((u8 *)object->target.child + 0x1e) = value;
}

struct State_0800b7c0;

s32 Animation_InitializeObjects(struct State_0800b7c0 *);

void Animation_SetIndexAndInitObjects(void *obj, s32 no)
{
    if ((obj != NULL) && ((0xF & FIELD_AT_OFFSET(obj, u8 *, 0x54)) == 1)) {
        obj = FIELD_AT_OFFSET(obj, void **, 0x50);
        if (no >= 0) {
            *FIELD_AT_OFFSET(obj, s16 **, 0x28) = (s16)no;
            Animation_InitializeObjects((struct State_0800b7c0 *)obj);
        }
    }
}

s32 ResourceMetadata_Register(s32 child);

void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *object, s32 value)
{
    if (object != 0 && (object->kind & 0xf) == 1) {
        void *child = object->target.child;
        if (value >= 0)
            ResourceMetadata_Register((s32)child);
    }
}

void ObjectDispatch_InitFromTable5WithArgument(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, (u32)ObjectDispatch_Table5Script);
    object->argument = argument;
}

void ObjectDispatch_InitFromTable0(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)ObjectDispatch_Table0Script);
}

void ObjectDispatch_InitFromTable1(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)ObjectDispatch_Table1Script);
}

void ObjectDispatch_InitFromTable2(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)ObjectDispatch_Table2Script);
}

void ObjectDispatch_InitFromTable3(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)ObjectDispatch_Table3Script);
}

void ObjectDispatch_InitFromTable6(struct DispatchObject *object)
{
    ObjectDispatch_Initialize(object, (u32)ObjectDispatch_Table6Script);
}

void ObjectDispatch_InitFromTable4WithArgument(struct DispatchObject *object, s32 argument)
{
    ObjectDispatch_Initialize(object, (u32)ObjectDispatch_Table4Script);
    if (argument != 0) {
        object->value_34 = 0x8000;
        object->value_30 = 0x40000;
        object->argument = argument;
        object->value_64 = 0;
    }
}

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 WaitFrames(s32);

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

void ObjectDispatch_SetSingleChildField26(u8 *arg0, u32 arg1)
{
    if (arg0 != NULL) {
        if ((arg0[0x54] & 0xf) == 1)
            (*(u8 **)(arg0 + 0x50))[0x26] = arg1;
    }
}

struct ChildStateFlags {
    u8 padding[5];
    u8 unk_0 : 2;
    u8 field_2 : 2;
    u8 unk_4 : 4;
};

void Animation_SetStateField5Bits2To3(u8 *obj, u32 v)
{
    if (obj != 0 && obj[84] == 1) {
        struct ChildStateFlags *state = *(struct ChildStateFlags **)(obj + 80);
        state->field_2 = v;
    }
}

struct ChildDisplayFlags {
    u8 padding[29];
    u8 unk_0 : 1;
    u8 field_1 : 1;
    u8 unk_2 : 6;
};

void Animation_SetStateField1dBit1(u8 *obj, u32 v)
{
    if (obj != 0 && obj[84] == 1) {
        struct ChildDisplayFlags *state = *(struct ChildDisplayFlags **)(obj + 80);
        state->field_1 = v;
    }
}

s32 ObjectGroup_SetChildValueUnlessFifteen(s32);

void Animation_ApplyChildValues(void *obj)
{
    if ((obj != NULL) && (FIELD_AT_OFFSET(obj, u8 *, 0x54) == 1)) {
        ObjectGroup_SetChildValueUnlessFifteen(FIELD_AT_OFFSET(obj, s32 *, 0x50));
    }
}

s32 WaitFrames(s32);
s32 Scheduler_EnableCallbacks(u32 value);
s32 BattleFx_ApplyColorToTargetBufferFar(s32, s32);
s32 BattleFx_StartBufferInterpolationFar(s32);
void ObjectSystem_UpdateCamera(void);
void ObjectSystem_UpdateCameraFixed(void);

void Graphics_EnableObjLayerAndCallbacks(void)
{
    Scheduler_EnableCallbacks((u32)ObjectSystem_UpdateCamera);
    Scheduler_EnableCallbacks((u32)ObjectSystem_UpdateCameraFixed);
    BattleFx_ApplyColorToTargetBufferFar(0x10000, 1);
    BattleFx_StartBufferInterpolationFar(1);
    WaitFrames(1);
    *(u16 *)0x04000000 = (0xF1FF & *(u16 *)0x04000000) | 0x1000;
}

s32 Scheduler_DisableCallbacks(u32 value);
void ObjectSystem_UpdateCamera(void);
void ObjectSystem_UpdateCameraFixed(void);

void ObjectDispatch_StopCallbacksAndHideLayers(void)
{
    Scheduler_DisableCallbacks((u32)ObjectSystem_UpdateCamera);
    Scheduler_DisableCallbacks((u32)ObjectSystem_UpdateCameraFixed);
    *(u16 *)0x04000000 &= 0xE1FF;
}
