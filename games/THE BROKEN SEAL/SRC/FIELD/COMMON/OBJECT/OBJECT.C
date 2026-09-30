#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "DMA.H"
#include "OBJECT_DISPATCH.H"
#include "OBJECT_COMMANDS.H"
#include "IWRAM_CALL.H"
#include "SCRIPT_INTERPRETER.H"

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

s32 Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
void ObjectSystem_Configure(s32 mode);
void Object_UpdateAllMotion(void);
void Object_UpdateAllThumb(void);
void ObjectSystem_UpdateCameraFixed(void);
void ObjectSystem_UpdateCamera(void);
extern s32 Data_03001d1c;
extern s32 Data_03001cc0;

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
s32 Scheduler_DisableCallbacks(u32 value);

struct CameraTile {
    u32 unk_00 : 12;
    u32 layer : 2;
    u32 priority : 2;
    u32 unk_10 : 16;
};

struct CameraSprite {
    u8 unknown_00[4];
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 flip_x : 1;
    u16 flip_y : 1;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unknown_0a[0x0a];
    u16 second_tile : 10;
    u16 second_priority : 2;
    u16 second_palette : 4;
    u8 unknown_16[2];
    s32 scale;
    u8 resource;
    u8 flags_1d;
    u8 unk_1e[7];
    u8 activated;
};

struct CameraObject {
    u32 active;
    u16 unk_04;
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    s32 height;
    s32 scale_x;
    s32 scale_y;
    u8 unk_20[2];
    u8 layer;
    u8 flags;
    u8 unk_24[0x2c];
    struct CameraSprite *sprite;
    u8 kind;
    u8 unk_55[7];
    u8 held;
    u8 unk_5d[0x13];
};

struct CameraLayer {
    struct CameraTile *tiles;
    u8 unk_04[44];
};

struct CameraState {
    u8 unk_000[0xe4];
    s32 x;
    s32 z;
    u8 unk_0ec[0x44];
    struct CameraLayer layers[1];
};

struct CameraSync {
    s16 count;
    s16 unk_02;
    s16 frozen;
};

extern u8 Render_DecodeFrame[];
extern u8 Render_DecodeFrameCodeSize[];
u8 *Runtime_AllocateHeapBlock(s32 slot, u32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 Resource_ActivateEntry(u32 resource_index);
void Render_ApplyProjectedPlacement(void *sprite, s32 *position, s32 *scale, u16 angle);
extern const s32 Camera_FixedViewMatrix[];

struct FixedObject {
    u32 active;
    u16 unk_04;
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    s32 height;
    s32 scale_x;
    s32 scale_y;
    u8 unk_20[2];
    u8 layer;
    u8 flags;
    u8 unk_24[0x2c];
    void *sprite;
    u8 kind;
    u8 unk_55[0x1b];
};

struct FixedPoint {
    s32 x;
    s32 y;
    s32 z;
};

struct FixedCamera {
    struct FixedPoint eye;
    struct FixedPoint target;
    struct FixedPoint *eye_override;
    struct FixedPoint *target_override;
};

struct FixedSync {
    s16 count;
    s16 unk_02;
    s16 frozen;
};

s32 ArcTan2(s32 x, s32 y);
void Render_ResetTransformState(void);
s32 GameFlag_TestFar(s32 flag);
void _call_via_r3(u32 arg, s32 unused1, s32 unused2, u32 routine);
void Graphics_PrepareTransferAndRun(struct FixedPoint *eye, struct FixedPoint *target);
void Graphics_PrepareTransferInIwramWork(struct FixedPoint *eye, struct FixedPoint *target);
void Render_PlaceProjectedSprite(void *sprite, s32 *position, s32 *scale, s32 angle, s32 layer);

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
        Scheduler_AddOrUpdateCallback(Object_UpdateAllMotion, 0xc8a);
    else
        Scheduler_AddOrUpdateCallback(Object_UpdateAllThumb, 0xc8a);
    if ((u32)(mode - 3) <= 1) {
        Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCameraFixed, 0xc80);
    } else {
        Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCamera, 0xc80);
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

/* The render decoder and its data run from a heap copy (DECODE.S). */
/* main:0800c62c ObjectSystem_UpdateCamera - exact (592 of 592 bytes,
   2026-09-30 helper hF).

   Each frame the field camera places every live object on screen: objects
   inside the view are projected through their tile's layer bits, and objects
   that leave it (or sit at the origin) fall back to their resource entry.

   The approved IWRAM header now emits the reference's long first-view
   branch; the old instruction-length residual no longer applies. Do not
   change that header for this draft. Remaining: count-zero scheduling,
   the 63/MulQ16 entry register order, tile-layer scratch r0 versus r1,
   and the missing tail kind copy (adds r3,r6,#0; ands r3,r2). The latter
   shortens the body and shifts its final pool. No code or byte credit added.
   2026-09-29 (Venus): the tile layer in its own local (bits_val) fixes the
   r0/r1 scratch, and testing flags_1d against 1 rather than kind fixes the
   tail's operand order (23 to 11 diff lines). Left: the count-zero and
   63/pool-load scheduling at the loop head, and the out-of-view path loads
   flags_1d itself (ldrb r2) before joining the shared test; a goto into
   the shared test reorders the whole body.
   2026-09-30 (hF): exact. The out-of-view path stores the constant 1
   (no kind = 1 before the test), so its flags load is not cross-jumped
   into the shared tail; the loop counts up from 0 to 64, so loop.c emits
   the reversed counter's 63 after the hoisted MulQ16 entry and reload
   gives them r4 and r3 as in the ROM; and one do-while around the size
   load and Dma_Set ends in a loop note, a scheduling barrier that keeps
   the count-zero constant ahead of the object-list load, with the count
   reset now written before that load. */
void ObjectSystem_UpdateCamera(void)
{
    s32 cnt;
    struct CameraState *state;
    s32 cam_x;
    s32 cam_z;
    struct CameraSync *sync;
    struct CameraObject *obj;
    struct CameraSprite *sprite;
    s32 *cam;
    u32 size;
    u32 kind;
    s32 dx;
    s32 dz;
    s32 top;
    struct CameraTile *tile;
    u32 bits;
    s32 unused[9]; /* FAKEMATCH: the ROM frame keeps 36 more bytes than it uses. */
    s32 scale[2];
    s32 pos[4];
    s32 y;
    s32 height;
    u32 bits_val;

    state = *(struct CameraState **)((u32)((u8 *)&gObjectSlots) + 12);
    cam = &state->x;
    cam_x = cam[0] & 0xffff0000;
    cam_z = cam[1] & 0xffff0000;
    sync = *(struct CameraSync **)((u32)((u8 *)&gObjectSlots) + 4);
    /* FAKEMATCH: the do-while keeps the copy between the runtime loads and the count reset. */
    do {
        size = (u32)Render_DecodeFrameCodeSize;
        Dma_Set(Render_DecodeFrame, Runtime_AllocateHeapBlock(52, size), 0x84000000 | (size >> 2),
            (volatile u32 *)0x040000d4);
    } while (0);
    sync->count = 0;
    obj = *(struct CameraObject **)(u32)((u8 *)&gObjectSlots);
    for (cnt = 0; cnt < 64; cnt++, obj++) {
        if (obj->active == 0)
            continue;
        if (obj->x != 0 || obj->z != 0) {
            kind = obj->kind & 15;
            if (kind == 0)
                continue;
            if (kind != 1)
                continue;
            if (sync->frozen != 0 && obj->held == 0) {
                struct CameraSprite *frozen = obj->sprite;

                Resource_ActivateEntry(frozen->resource);
                frozen->activated = kind;
                continue;
            }
            dx = obj->x - cam_x;
            dz = obj->z - cam_z;
            top = dz - obj->y;
            sprite = obj->sprite;
            if (dx > -0x200000 && dx < 0x1100000 && top > -0x200000 && top < 0xe00000) {
                tile = &state->layers[obj->layer].tiles[(obj->x >> 20) + ((obj->z >> 20) << 7)];
                if (obj->flags & 1) {
                    bits = tile->priority;
                    if (bits != 0) {
                        sprite->priority = bits;
                        sprite->second_priority = bits;
                    }
                }
                bits_val = tile->layer;
                if (bits_val != 0)
                    obj->layer = bits_val - 1;
                scale[0] = Iwram_MulQ16(obj->scale_x, sprite->scale);
                scale[1] = Iwram_MulQ16(obj->scale_y, sprite->scale);
                pos[0] = dx;
                pos[1] = obj->y;
                pos[2] = dz;
                pos[3] = obj->height;
                if (obj->flags & 2) {
                    pos[1] += 0xfec00000;
                    pos[2] += 0xfec00000;
                    pos[3] += 0xfec00000;
                }
                if (obj->flags & 4) {
                    pos[1] += 0x1400000;
                    pos[2] += 0x1400000;
                    pos[3] += 0x1400000;
                }
                Render_ApplyProjectedPlacement(sprite, pos, scale, obj->angle);
                continue;
            }
            if (obj->held == 0) {
                if (!(sprite->flags_1d & 1)) {
                    Resource_ActivateEntry(sprite->resource);
                    sprite->activated = 1;
                }
            }
        } else {
            kind = obj->kind & 15;
            if (kind == 1) {
                sprite = obj->sprite;
                if (obj->held == 0 && !(sprite->flags_1d & 1)) {
                    Resource_ActivateEntry(sprite->resource);
                    sprite->activated = kind;
                }
            }
        }
    }
    Runtime_ReleaseHeapBlock(52);
}

/* A routine that only reports success, before the fixed camera update;
   nothing in the image calls it by name. */
s32 ObjectCamera_ReturnTrue(void)
{
    return 1;
}

/* The fixed-camera variant of the field object pass: it aims the view from
   the camera eye toward its target, then walks the object table from the
   last record down and places every single or four-part sprite. */
void ObjectSystem_UpdateCameraFixed(void)
{
    s32 cnt;
    struct FixedCamera *camera;
    struct FixedSync *sync;
    struct FixedPoint *eye;
    struct FixedPoint *target;
    struct FixedObject *obj;
    s32 angle;
    s32 part;
    s32 kind;
    s32 *pos;
    s32 unit;
    u32 size;
    void **parts;
    void *sprite;
    s32 scale2[2];
    s32 scale[2];
    /* FAKEMATCH: unused words that give the ROM's 52-byte frame. */
    s32 unused[6];

    camera = *(struct FixedCamera **)((u32)((u8 *)&gObjectSlots) + 0x1c);
    sync = *(struct FixedSync **)((u32)((u8 *)&gObjectSlots) + 4);
    /* FAKEMATCH: the do-while keeps the size load after the runtime loads. */
    do { size = (u32)Render_DecodeFrameCodeSize; } while (0);
    Dma_Set(Render_DecodeFrame, Runtime_AllocateHeapBlock(52, size), 0x84000000 | (size >> 2),
        (volatile u32 *)0x040000d4);
    eye = &camera->eye;
    target = &camera->target;
    if (camera->eye_override != NULL)
        eye = camera->eye_override;
    if (camera->target_override != NULL)
        target = camera->target_override;
    angle = (s16)ArcTan2((eye->x - target->x) >> 16, (eye->z - target->z) >> 16);
    sync->count = 0;
    Render_ResetTransformState();
    if (GameFlag_TestFar(0x16b)) {
        angle += 0xffffe000;
        Iwram_TransformMatrix(Camera_FixedViewMatrix);
        Graphics_PrepareTransferAndRun(eye, target);
    } else {
        Graphics_PrepareTransferInIwramWork(eye, target);
    }
    obj = *(struct FixedObject **)(u32)((u8 *)&gObjectSlots);
    obj += 63;
    unit = 0x10000;
    for (cnt = 63; cnt >= 0; cnt--, obj--) {
        if (obj->active == 0)
            continue;
        kind = obj->kind & 15;
        switch (kind) {
        case 1:
            pos = &obj->x;
            scale[0] = obj->scale_x;
            scale[1] = obj->scale_y;
            sprite = obj->sprite;
            if (GameFlag_TestFar(0x16b)) {
                scale[0] = unit;
                scale[1] = unit;
            }
            Render_PlaceProjectedSprite(sprite, pos, scale, obj->angle + angle, obj->layer);
            break;
        case 2:
            scale2[0] = obj->scale_x;
            scale2[1] = obj->scale_y;
            if (GameFlag_TestFar(0x16b)) {
                scale2[0] = unit;
                scale2[1] = unit;
            }
            parts = obj->sprite;
            for (part = 3; part >= 0; part--) {
                sprite = *parts++;
                if (sprite != NULL)
                    Render_PlaceProjectedSprite(sprite, &obj->x, scale2, obj->angle + angle, obj->layer);
            }
            break;
        case 0:
            break;
        }
    }
    Runtime_ReleaseHeapBlock(52);
}

/* script/advance_past_cursor.c */
/* script/interpreter/cmd/advance.c */
s32 Script_AdvancePastCursor(struct ScriptInterpreter *interpreter)
{
    s32 zero;

    interpreter->script = interpreter->script + interpreter->cursor + 1;
    zero = 0;
    interpreter->cursor = zero;
    return 1;
}
