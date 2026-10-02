#include "EDITION.H"
#include "OBJDISP.H"
#include "RESOURCE.H"
#include "OBJECT_RUNTIME.H"
#include "OBJECT_DISPATCH.H"
#include "IO_REG.H"
#include "DMA.H"

/* Object dispatch and its linker-neighbor bank. The TBS terminal camera bank
   remains raw; TLA emits only the adjacent lookup/release prefix. */
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#include "RAM_BUFFER.H"
#else
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"

extern u8 ResourceTableEntries[];
extern u8 gCam[];

/* map/shared/Map_RenderAnimatedTileFrame.c */
struct MapBase {
    u16 unused;
    u16 offset;
};

extern u8 Map_TileDissolveOrder[];

/* map/shared/render_animated_tile_frames_for_object.c */
void Map_RenderAnimatedTileFrame(u8 *object, u32 position);

/* map/shared/get_screen_relative_position.c */
struct Thing {
    u8 filler0[8];
    s32 field8;
    u8 filler12[4];
    s32 field16;
};

/* The object system state block (92 bytes). */
struct ObjectSystem {
    u8 unk_00[6];
    u8 priority;
    u8 flag;
    u8 unk_08[84];
};

void ObjectSystem_Configure(s32 mode);
void Object_UpdateAllMotion(void);
void Object_UpdateAllThumb(void);
void ObjectSystem_UpdateCameraFixed(void);
void ObjectSystem_UpdateCamera(void);
extern s32 Data_03001d1c;
extern s32 Data_03001cc0;

/* object/dispatch/find_free_object.c */
extern u8 *gObjectSlots;

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
struct AnimationMetadata *Resource_GetMetadataRecordFar(s32 id);
void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
s32 AnimationObjects_SelectAnimation(void *, s32);
void AnimationObjects_SetField15OnActive(void *, s32);
struct State_0800b7c0;
s32 Animation_InitializeObjects(struct State_0800b7c0 *);
struct MetadataSlotState;
s32 ResourceMetadata_Register(struct MetadataSlotState *state, s32 id);
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

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
s32 BattleFx_ApplyColorToTargetBufferFar(s32, s32);
s32 BattleFx_StartBufferInterpolationFar(s32);

void Map_RenderAnimatedTileFrame(u8 *object, u32 position)
{
    u16 *destination;
    s32 count;
    u32 row;
    u8 *table;
    u32 high_mask;
    u32 index_mask;
    u8 offset_mask;

    destination = (u16 *)(0x06010000
        + ((struct MapBase *)((u32)&ResourceTableEntries))[object[0x1C]].offset);
    count = (object[0x20] * object[0x21]) / 64;
    row = 0;

    if (row < (u32)count) {
        table = Map_TileDissolveOrder;
        high_mask = 0xFF00;
        index_mask = 0x3F;
        offset_mask = 0x3E;

        /* 1行ごとに32セル進める。 */
        for (; row < (u32)count; row++, destination += 32, position++) {
            if ((u32)(position - 0x40) <= 0x3F) {
                u8 entry;
                u16 *cell;
                u32 parity;

                entry = table[(position + row * 16) & index_mask];
                cell = (u16 *)((u8 *)destination + (entry & offset_mask));
                parity = 1;
                parity &= entry;
                if (parity != 0)
                    *cell = *(u8 *)cell;
                else
                    *cell &= high_mask;
            }
        }
    }
}

void Map_RenderAnimatedTileFramesForObject(u8 *object)
{
    u32 pos;

    pos = 0;
    do {
        Map_RenderAnimatedTileFrame(object, pos);
        Map_RenderAnimatedTileFrame(object, pos + 1);
        Map_RenderAnimatedTileFrame(object, pos + 2);
        Map_RenderAnimatedTileFrame(object, pos + 3);
        pos += 4;
        WaitFrames(1);
    } while (pos <= 0x7f);
}

void Map_RenderAllAnimatedTileFrames(u8 **tbl, s32 cnt)
{
    u8 **top;
    u8 **p;
    s32 n;
    u32 pos;
    u32 pos1;
    u32 pos2;
    u32 pos3;

    top = tbl;
    pos = 0;
    do {
        if (cnt > 0) {
            pos1 = pos + 1;
            pos2 = pos + 2;
            p = top;
            pos3 = pos + 3;
            n = cnt;
            do {
                Map_RenderAnimatedTileFrame(*p, pos);
                Map_RenderAnimatedTileFrame(*p, pos1);
                Map_RenderAnimatedTileFrame(*p, pos2);
                n -= 1;
                Map_RenderAnimatedTileFrame(*p++, pos3);
            } while (n != 0);
        }
        WaitFrames(1U);
        pos += 4;
    } while (pos <= 0x7FU);
}

s32 Map_GetScreenRelativePosition(struct Thing *obj, s32 *out)
{
    u8 *state = *(u8 **)((u32)&gCam);
    s32 *org = (s32 *)(state + 228);
    s32 a;
    s32 b;
    s32 x;
    s32 y;

    a = org[0] & 0xffff0000;
    b = org[1] & 0xffff0000;
    x = obj->field8 - a;
    y = obj->field16 - b;
    if ((u32)(x + 0x001fffff) <= 0x012ffffe && y > 0 && y < 0xe00000) {
        *out++ = x >> 16;
        *out = y >> 16;
        return 0;
    }
    *out++ = 0;
    *out = 0;
    return -1;
}

/* Allocates and clears the object system state and the object table,
   configures the system for the mode and schedules the object update (the
   motion-only one in mode 4) and the camera (fixed in modes 3 and 4). */
void ObjectSystem_Initialize(s32 mode)
{
    struct ObjectSystem *state;
    u8 *objects;
    volatile u32 fill;

    state = (struct ObjectSystem *)Runtime_AllocateBlock(6, 92);
    objects = Runtime_AllocateBlock(5, 0x1c00);
    ObjectSystem_Configure(mode);
    fill = 0;
    Dma_Set((const void *)&fill, objects, 0x85000700, (volatile u32 *)0x040000d4);
    fill = 0;
    Dma_Set((const void *)&fill, state, 0x85000017, (volatile u32 *)0x040000d4);
    if (mode == 4)
        Scheduler_AddOrUpdateCallback((s32)(Object_UpdateAllMotion), 0xc8a);
    else
        Scheduler_AddOrUpdateCallback((s32)(Object_UpdateAllThumb), 0xc8a);
    if ((u32)(mode - 3) <= 1) {
        Scheduler_AddOrUpdateCallback((s32)(ObjectSystem_UpdateCameraFixed), 0xc80);
    } else {
        Scheduler_AddOrUpdateCallback((s32)(ObjectSystem_UpdateCamera), 0xc80);
        Data_03001d1c = 0;
        Data_03001cc0 = 0;
    }
    state->priority = 15;
    state->flag = 0;
}

void ResourceTable_ReservedNoOpC0C4(void)
{
}

/* The second empty routine after the resource table code; nothing in the
   image calls it. */
void ResourceTable_ReservedNoOp2(void)
{
}


#endif

void *ObjectDispatch_FindFreeObject(void)
{
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    struct ObjectRuntime *entry = Ram_HeapSlots->script_objects;
#else
    struct ObjectRuntime *entry = (struct ObjectRuntime *)gObjectSlots;
#endif
    void *ret = 0;
    s32 index = 0;

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    /* FAKEMATCH: ordinary eight-form lookup 36/4 initializes its used zero result after the first script load; this initialized entry/result boundary retains the native register order without an added access. */
    asm("" : "+r"(entry), "+r"(ret));
#endif
    while (index <= 63) {
        if (entry->script == 0) {
            ret = entry;
            break;
        }
        index++;
        entry++;
    }
    return ret;
}

void ObjectDispatch_Release(struct DispatchObject *work)
{
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    u32 zero;
    u32 *src;
#else
    volatile u32 zero;
    volatile u32 *src;
#endif
    s32 count;
    struct ResourceObjectWork **child;

    if (work) {
        switch (work->kind & 15) {
        case 1:
            ResourceObject_Release((struct ResourceObjectWork *)work->target.child);
            break;
        case 2:
            child = (struct ResourceObjectWork **)work->target.children;
            count = 3;
            do {
                struct ResourceObjectWork *entry = *child++;
                if (entry)
                    ResourceObject_Release(entry);
            } while (--count >= 0);
            break;
        }
        src = &zero;
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
        /* FAKEMATCH: ordinary release 88/15 captures its real DMA-zero pointer after the store/setup; tie only initialized src before the actual zero store to retain native order, without reading the uninitialized word. */
        asm("" : "+r"(src));
#endif
        *src = 0;
        Dma_Set(src, work, 0x85000000 | (sizeof(struct ObjectRuntime) / 4), REG_DMA3);
    }
}

#if !(defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT))
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
        Object_SetPositionAndResetMotion((struct ObjectRuntime *)object, x, y, z);
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
            ResourceMetadata_Register((struct MetadataSlotState *)child, value);
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


#endif
