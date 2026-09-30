#include "TYPES.H"
#include "SCENE.H"
#include "OBJECT_DISPATCH.H"
#include "OBJECT_COMMANDS.H"
#include "DMA.H"

void *ObjectDispatch_FindFreeObject(void);

/* object/dispatch/find_free_object.c */
extern u8 *gObjectSlots;
void ResourceObject_Release(void *);

struct FieldObject {
    u32 script;
    u16 unknown_04;
    u16 unknown_06;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u16 radius;
    u8 unknown_22[14];
    s32 speed_limit;
    s32 acceleration;
    u8 unknown_38[12];
    s32 unknown_44;
    s32 unknown_48;
    s32 unknown_4c;
    void *animation;
    u8 animation_kind;
    u8 unknown_55;
    u8 unknown_56[3];
    u8 unknown_59;
    u8 unknown_5a;
    u8 unknown_5b[9];
    s16 tile_x;
    s16 tile_z;
};

struct AnimationMetadata {
    u8 unknown_00[9];
    u8 radius;
};

struct ObjectSpriteList {
    u8 unknown_00[24];
    s32 count;
};

extern struct ObjectSpriteList *gMenuCtrlWork;
extern const u32 ObjectDispatch_DefaultScript[];
void *ResourceObject_Create(s32 id);
struct AnimationMetadata *Resource_GetMetadataRecordFar(s32 id);
void Object_SetPositionAndResetMotion(struct FieldObject *object, s32 x, s32 y, s32 z);

s32 AnimationObjects_SelectAnimation(void *, s32);
void AnimationObjects_SetField15OnActive(void *, s32);
struct State_0800b7c0;
s32 Animation_InitializeObjects(struct State_0800b7c0 *);
s32 ResourceMetadata_Register(s32 child);
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))
s32 WaitFrames(s32);

struct ChildStateFlags {
    u8 padding[5];
    u8 unk_0 : 2;
    u8 field_2 : 2;
    u8 unk_4 : 4;
};

struct ChildDisplayFlags {
    u8 padding[29];
    u8 unk_0 : 1;
    u8 field_1 : 1;
    u8 unk_2 : 6;
};

s32 ObjectGroup_SetChildValueUnlessFifteen(s32);
s32 Scheduler_EnableCallbacks(u32 value);
s32 BattleFx_ApplyColorToTargetBufferFar(s32, s32);
s32 BattleFx_StartBufferInterpolationFar(s32);
void ObjectSystem_UpdateCamera(void);
void ObjectSystem_UpdateCameraFixed(void);
s32 Scheduler_DisableCallbacks(u32 value);

void *ObjectDispatch_FindFreeObject(void)
{
    u8 *entry = gObjectSlots;
    void *ret = 0;
    s32 index = 0;

    while (index <= 63) {
        if (*(u32 *)entry == 0) {
            ret = entry;
            break;
        }
        index++;
        entry += 112;
    }
    return ret;
}

void ObjectDispatch_Release(struct DispatchObject *work)
{
    volatile u32 zero;
    s32 count;
    void **child;
    if (work) {
        switch (work->kind & 15) {
        case 1: ResourceObject_Release(work->target.child); break;
        case 2:
            child = work->target.children;
            count = 3;
            do { void *entry = *child++; if (entry) ResourceObject_Release(entry); } while (--count >= 0);
            break;
        }
        zero = 0;
        Dma_Set(&zero, work, 0x8500001c, (volatile u32 *)0x040000d4);
    }
}

/* The engine's object constructor, reached from far code through
   Object_CreateFar: takes a free object slot, attaches the sprite or the
   two-sprite list the descriptor id names (its top nibble is the kind), and
   resets position, scale, speed and script to their defaults. */
struct FieldObject *FieldObject_Create(s32 id, s32 x, s32 y, s32 z)
{
    struct FieldObject *object;
    void *sprite;
    s32 kind;
    u32 *list;
    u32 *entry;
    volatile u32 zero;

    ObjectDispatch_FindFreeObject();
    kind = id / 4096;
    id &= 0xfff;
    object = (struct FieldObject *)ObjectDispatch_FindFreeObject();
    if (object != NULL) {
        object->radius = 16;
        switch (kind) {
        case 0:
            sprite = ResourceObject_Create(id);
            if (sprite != NULL) {
                object->animation_kind = 1;
                object->animation = sprite;
                object->radius = Resource_GetMetadataRecordFar(id)->radius >> 1;
            } else {
                object->animation_kind = 0;
            }
            break;
        case 2:
            entry = (list = (u32 *)gMenuCtrlWork + gMenuCtrlWork->count++) + 2;
            object->animation_kind = kind;
            zero = 0;
            object->animation = entry;
            Dma_Set(&zero, entry, 0x85000004, (volatile u32 *)0x040000d4);
            sprite = ResourceObject_Create(id);
            if (sprite != NULL) {
                /* FAKEMATCH: stored as a plain halfword, outside the object
                   record's alias set, so the entry copy schedules above it
                   as in the reference. */
                *(u16 *)&object->radius = Resource_GetMetadataRecordFar(id)->radius >> 1;
                *entry++ = (u32)sprite;
            }
            sprite = ResourceObject_Create(id + 1);
            if (sprite != NULL)
                *entry = (u32)sprite;
            break;
        }
    }
    if (object != NULL) {
        Object_SetPositionAndResetMotion(object, x, y, z);
        object->script = (u32)ObjectDispatch_DefaultScript;
        object->speed_limit = 0x20000;
        object->unknown_04 = 0;
        object->scale_x = 0x10000;
        object->scale_y = 0x10000;
        object->acceleration = 0x10000;
        object->unknown_55 = 3;
        object->unknown_48 = 0x10000;
        object->unknown_44 = 0x4000;
        object->unknown_59 = 0;
        object->unknown_5a = 1;
        object->unknown_4c = 0;
        object->unknown_06 = 0x4000;
        object->tile_x = x / 65536;
        object->tile_z = z / 65536;
    }
    return object;
}

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

void Animation_SetStateField5Bits2To3(u8 *obj, u32 v)
{
    if (obj != 0 && obj[84] == 1) {
        struct ChildStateFlags *state = *(struct ChildStateFlags **)(obj + 80);
        state->field_2 = v;
    }
}

void Animation_SetStateField1dBit1(u8 *obj, u32 v)
{
    if (obj != 0 && obj[84] == 1) {
        struct ChildDisplayFlags *state = *(struct ChildDisplayFlags **)(obj + 80);
        state->field_1 = v;
    }
}

void Animation_ApplyChildValues(void *obj)
{
    if ((obj != NULL) && (FIELD_AT_OFFSET(obj, u8 *, 0x54) == 1)) {
        ObjectGroup_SetChildValueUnlessFifteen(FIELD_AT_OFFSET(obj, s32 *, 0x50));
    }
}

void Graphics_EnableObjLayerAndCallbacks(void)
{
    Scheduler_EnableCallbacks((u32)ObjectSystem_UpdateCamera);
    Scheduler_EnableCallbacks((u32)ObjectSystem_UpdateCameraFixed);
    BattleFx_ApplyColorToTargetBufferFar(0x10000, 1);
    BattleFx_StartBufferInterpolationFar(1);
    WaitFrames(1);
    *(u16 *)0x04000000 = (0xF1FF & *(u16 *)0x04000000) | 0x1000;
}

void ObjectDispatch_StopCallbacksAndHideLayers(void)
{
    Scheduler_DisableCallbacks((u32)ObjectSystem_UpdateCamera);
    Scheduler_DisableCallbacks((u32)ObjectSystem_UpdateCameraFixed);
    *(u16 *)0x04000000 &= 0xE1FF;
}

/* A routine that only reports success, after the object dispatcher;
   nothing in the image calls it by name. */
s32 ObjectDispatch_ReturnTrue(void)
{
    return 1;
}
