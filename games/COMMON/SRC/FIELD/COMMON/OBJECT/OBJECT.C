#include "RUNTIME_MEM.H"
#include "EDITION.H"
#include "OBJDISP.H"
#include "RESOURCE.H"
#include "METADATA_LOOKUP.H"
#include "OBJECT_RUNTIME.H"
#include "OBJECT_DISPATCH.H"
#include "IO_REG.H"
#include "DMA.H"
#include "BATTLE_RUNTIME.H"

/* Object dispatch and its linker-neighbor bank. The TBS terminal camera bank
   follows the dispatcher; TLA emits only the adjacent lookup/release prefix. */
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#include "RAM_BUFFER.H"
#else
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "MAP_SCROLL.H"
#include "SCRIPT.H"
#include "ANIMSPR.H"
#include "FIELD_SPRITE.H"
#include "FIELDOBJ.H"
#include "SCRIPT_OBJECT_RUNTIME.H"
#include "SCRIPT_MOTION.H"
#include "VRAM_BLOCK.H"
#include "IWRAM_CALL.H"

extern u8 Map_TileDissolveOrder[];

/* map/shared/render_animated_tile_frames_for_object.c */
void Map_RenderAnimatedTileFrame(u8 *object, u32 position);

void ObjectSystem_Configure(s32 mode);
void Object_UpdateAllMotion(void);
void Object_UpdateAllThumb(void);
void ObjectSystem_UpdateCameraFixed(void);
void ObjectSystem_UpdateCamera(void);
extern s32 Data_03001d1c;
extern s32 Data_03001cc0;

/* object/dispatch/find_free_object.c */
extern struct ObjectRuntime *gObjectSlots;

extern struct ObjectSystemWork *gMenuCtrlWork;
extern const u32 ObjectDispatch_DefaultScript[];
void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
s32 AnimationObjects_SelectAnimation(struct AnimationObject *, s32);
void AnimationObjects_SetField15OnActive(struct AnimationObject *, s32);
s32 Animation_InitializeObjects(struct AnimationObject *);
s32 ResourceMetadata_Register(struct AnimationObject *state, s32 id);

/* The existing packed lane of the animation child, used only by its bit setter. */
struct ChildDisplayFlags {
    u8 padding[29];
    u8 unk_0 : 1;
    u8 field_1 : 1;
    u8 unk_2 : 6;
};

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

    struct AnimationObject *sprite = (struct AnimationObject *)object;

    destination = (u16 *)(0x06010000
        + gVramBlockCache[sprite->slot].offset);
    count = (sprite->width * sprite->height) / 64;
    row = 0;

    if (row < count) {
        table = Map_TileDissolveOrder;
        high_mask = 0xFF00;
        index_mask = 0x3F;
        offset_mask = 0x3E;

        /* 1行ごとに32セル進める。 */
        for (; row < count; row++, destination += 32, position++) {
            if ((position - 0x40) <= 0x3F) {
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

s32 Map_GetScreenRelativePosition(struct ObjectRuntime *obj, s32 *out)
{
    struct MapScrollWork *state = gMapWork[0];
    s32 *org = &state->view_x;
    s32 a;
    s32 b;
    s32 x;
    s32 y;

    a = org[0] & 0xffff0000;
    b = org[1] & 0xffff0000;
    x = obj->x - a;
    y = obj->z - b;
    if ((x >= -0x1fffff && x <= 0x10fffff) && y > 0 && y < 0xe00000) {
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
    struct ObjectSystemWork *state;
    struct ObjectRuntime *objects;
    volatile u32 fill;

    state = Runtime_AllocateBlock(6, sizeof(*state));
    objects = Runtime_AllocateBlock(5, 64 * sizeof(*objects));
    ObjectSystem_Configure(mode);
    fill = 0;
    Dma_Set((const void *)&fill, objects, 0x85000700, (volatile u32 *)0x040000d4);
    fill = 0;
    Dma_Set((const void *)&fill, state, 0x85000017, (volatile u32 *)0x040000d4);
    if (mode == 4)
        Scheduler_AddOrUpdateCallback(Object_UpdateAllMotion, 0xc8a);
    else
        Scheduler_AddOrUpdateCallback(Object_UpdateAllThumb, 0xc8a);
    if (mode >= 3 && mode <= 4) {
        Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCameraFixed, 0xc80);
    } else {
        Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCamera, 0xc80);
        Data_03001d1c = 0;
        Data_03001cc0 = 0;
    }
    state->edge = 15;
    state->fill = 0;
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
    struct ObjectRuntime *entry = gObjectSlots;
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
struct ObjectRuntime *FieldObject_Create(s32 id, s32 x, s32 y, s32 z)
{
    struct ObjectRuntime *object;
    struct FieldActor *actor;
    struct ScriptMotionObject *motion;
    struct ScriptObjectRuntime *script;
    void *sprite;
    s32 kind;
    void **entry;
    volatile u32 zero;

    ObjectDispatch_FindFreeObject();
    kind = id / 4096;
    id &= 0xfff;
    object = ObjectDispatch_FindFreeObject();
    if (object != NULL) {
        actor = (struct FieldActor *)object;
        actor->radius = 16;
        switch (kind) {
        case 0:
            sprite = ResourceObject_Create(id);
            if (sprite != NULL) {
                object->animation_kind = 1;
                object->animation = sprite;
                actor->radius = Resource_GetMetadataRecordFar(id)->box_y >> 1;
            } else {
                object->animation_kind = 0;
            }
            break;
        case 2:
            entry = (void **)((u32 *)gMenuCtrlWork + gMenuCtrlWork->count++ + 2);
            object->animation_kind = kind;
            zero = 0;
            object->animation = entry;
            Dma_Set(&zero, entry, 0x85000004, (volatile u32 *)0x040000d4);
            sprite = ResourceObject_Create(id);
            if (sprite != NULL) {
                actor->radius = Resource_GetMetadataRecordFar(id)->box_y >> 1;
                *entry++ = sprite;
            }
            sprite = ResourceObject_Create(id + 1);
            if (sprite != NULL)
                *entry = sprite;
            break;
        }
    }
    if (object != NULL) {
        motion = (struct ScriptMotionObject *)object;
        script = (struct ScriptObjectRuntime *)object;
        Object_SetPositionAndResetMotion(object, x, y, z);
        object->script = (s32 *)ObjectDispatch_DefaultScript;
        object->speed_limit = 0x20000;
        object->step = 0;
        actor->scale_x = 0x10000;
        actor->scale_y = 0x10000;
        object->acceleration = 0x10000;
        object->flags = 3;
        motion->gravity = 0x10000;
        motion->vertical.bounce = 0x4000;
        object->unknown_59 = 0;
        object->action_flags = 1;
        *(s32 *)((u8 *)motion + 0x4c) = 0;
        object->angle = 0x4000;
        script->home_x = x / 65536;
        script->home_z = z / 65536;
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

void ObjectDispatch_ApplyPairToChildren(struct DispatchObject *object, s32 arg1, s32 arg2)
{
    void **items;
    void *item;
    s32 count;

    if (object != 0) {
        switch (object->kind & 15) {
        case 1:
            AnimationObjects_SelectAnimation(object->target.child, arg1);
            AnimationObjects_SetField15OnActive(object->target.child, arg2);
            break;
        case 2:
            items = object->target.children;
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
        ((struct FieldSprite *)object->target.child)->rotation = value;
}

void Animation_SetIndexAndInitObjects(void *raw_object, s32 no)
{
    struct DispatchObject *object = raw_object;
    struct AnimationObject *child;

    if (object != NULL && (object->kind & 0xf) == 1) {
        child = object->target.child;
        if (no >= 0) {
            child->entries[0]->anim_id = no;
            Animation_InitializeObjects(child);
        }
    }
}

void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *object, s32 value)
{
    if (object != 0 && (object->kind & 0xf) == 1) {
        struct AnimationObject *child = object->target.child;
        if (value >= 0)
            ResourceMetadata_Register(child, value);
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

void ObjectDispatch_WaitForValue16(void *raw_object)
{
    struct ScriptInterpreter *object = raw_object;
    s32 cnt;

    cnt = 0;
    if (object->script[object->cursor] != 0x10) {
        do {
            WaitFrames(1);
            cnt++;
            if (cnt > 0x12b) {
                break;
            }
        } while (object->script[object->cursor] != 0x10);
    }
}

void ObjectDispatch_SetSingleChildField26(struct DispatchObject *object, u32 value)
{
    if (object != NULL) {
        if ((object->kind & 0xf) == 1)
            ((struct AnimationObject *)object->target.child)->flags = value;
    }
}

void Animation_SetStateField5Bits2To3(struct DispatchObject *obj, u32 v)
{
    if (obj != 0 && obj->kind == 1) {
        struct FieldSprite *sprite = obj->target.child;
        sprite->blend_mode = v;
    }
}

void Animation_SetStateField1dBit1(struct DispatchObject *obj, u32 v)
{
    if (obj != 0 && obj->kind == 1) {
        /* FAKEMATCH: the ordinary canonical byte mask keeps 40 bytes but
           changes the mask/register order; retain the existing packed bit lane. */
        struct ChildDisplayFlags *state = obj->target.child;
        state->field_1 = v;
    }
}

void Animation_ApplyChildValues(struct DispatchObject *obj, u32 value)
{
    if (obj != NULL && obj->kind == 1) {
        ObjectGroup_SetChildValueUnlessFifteen(obj->target.child, value);
    }
}

void Graphics_EnableObjLayerAndCallbacks(void)
{
    Scheduler_EnableCallbacks(ObjectSystem_UpdateCamera);
    Scheduler_EnableCallbacks(ObjectSystem_UpdateCameraFixed);
    BattleFx_ApplyColorToTargetBufferFar(0x10000, 1);
    BattleFx_StartBufferInterpolationFar(1);
    WaitFrames(1);
    REG_DISPCNT = (REG_DISPCNT & 0xF1FF) | DISPCNT_OBJ_ON;
}

void ObjectDispatch_StopCallbacksAndHideLayers(void)
{
    Scheduler_DisableCallbacks(ObjectSystem_UpdateCamera);
    Scheduler_DisableCallbacks(ObjectSystem_UpdateCameraFixed);
    REG_DISPCNT &= 0xE1FF;
}

/* A routine that only reports success, after the object dispatcher;
   nothing in the image calls it by name. */
s32 ObjectDispatch_ReturnTrue(void)
{
    return 1;
}

/* The field camera: each frame one of the two scheduler callbacks above
   places every live object on screen, through the tile map's layer bits or
   through a fixed view. */

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
    void *sprite;
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

struct CameraPoint {
    s32 x;
    s32 y;
    s32 z;
};

struct FixedCamera {
    struct CameraPoint eye;
    struct CameraPoint target;
    struct CameraPoint *eye_override;
    struct CameraPoint *target_override;
};

extern u8 Render_DecodeFrame[];
extern u8 Render_DecodeFrameCodeSize[];
extern const s32 Camera_FixedViewMatrix[];
void Render_ApplyProjectedPlacement(void *sprite, s32 *position, s32 *scale, u16 angle);
s32 ArcTan2(s32 x, s32 y);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferAndRun(struct CameraPoint *eye, struct CameraPoint *target);
void Graphics_PrepareTransferInIwramWork(struct CameraPoint *eye, struct CameraPoint *target);
void Render_PlaceProjectedSprite(void *sprite, s32 *position, s32 *scale, s32 angle, s32 layer);

/* Objects inside the view are projected through their tile's layer bits;
   objects that leave it (or sit at the origin) fall back to their resource
   entry. */
void ObjectSystem_UpdateCamera(void)
{
    /* FAKEMATCH: unused; the native frame keeps room for three points above the position. */
    struct CameraPoint spare[3];
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
    s32 scale[2];
    s32 pos[4];
    s32 y;
    s32 height;
    u32 bits_val;

    state = *(struct CameraState **)(((u8 *)&gObjectSlots) + 12);
    cam = &state->x;
    cam_x = cam[0] & 0xffff0000;
    cam_z = cam[1] & 0xffff0000;
    sync = *(struct CameraSync **)(((u8 *)&gObjectSlots) + 4);
    /* FAKEMATCH: the do-while keeps the copy between the runtime loads and the count reset. */
    do {
        size = (u32)Render_DecodeFrameCodeSize;
        Dma_Set(Render_DecodeFrame, Runtime_AllocateHeapBlock(52, size), 0x84000000 | (size >> 2),
            (volatile u32 *)0x040000d4);
    } while (0);
    sync->count = 0;
    obj = *(struct CameraObject **)((u8 *)&gObjectSlots);
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

/* A routine that only reports success, between the two camera updates;
   nothing in the image calls it by name. */
s32 ObjectCamera_ReturnTrue(void)
{
    return 1;
}

/* The fixed camera looks from an eye at a target and places every live
   object in back-to-front order, unscaled while flag 0x16b is set. */
void ObjectSystem_UpdateCameraFixed(void)
{
    s32 cnt;
    struct FixedCamera *camera;
    struct CameraSync *sync;
    struct CameraPoint *eye;
    struct CameraPoint *target;
    struct CameraObject *obj;
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
    /* FAKEMATCH: unused; the native frame keeps room for two points above the scales. */
    struct CameraPoint spare[2];

    camera = *(struct FixedCamera **)(((u8 *)&gObjectSlots) + 0x1c);
    sync = *(struct CameraSync **)(((u8 *)&gObjectSlots) + 4);
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
    obj = *(struct CameraObject **)((u8 *)&gObjectSlots);
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

#endif
