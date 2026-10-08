

#include "object_runtime.h"
#include "metadata_lookup.h"
#include "object_dispatch.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "dma.h"

struct ObjectRuntimeDraft {
    s32 *script;
    s16 step;
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    s32 terrain_height;
    s32 scale_x;
    s32 scale_y;
    u16 radius;
    u8 terrain_id;
    u8 unknown_23;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed_limit;
    s32 acceleration;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    s32 value_44;
    s32 value_48;
    s32 value_4c;
    void *animation;
    u8 animation_kind;
    u8 flags;
    u8 motion_flags;
    u8 unknown_57[2];
    u8 value_59;
    u8 action_flags;
    u8 movement_state;
    u8 unknown_5c[0x04];
    void *callback;
    union {
        struct { s16 action; s16 unknown; } motion;
        struct { s16 x; s16 z; } tile;
    } value_64;
    struct ObjectRuntimeDraft *linked_object;
    u8 unknown_6c[0x04];
    s32 movement_dx;
    s32 movement_dy;
    s32 movement_dz;
    struct ObjectRuntimeDraft *support_object;
};

struct ObjectDraftWork {
    u8 unknown_00[24];
    s32 child_list_cursor;
    u8 unknown_1c[64];
};

struct DispatchChild *ResourceObject_Create(s32 resource);
void *ObjectDispatch_FindFreeObject(void);

typedef char RuntimeDraft_size[sizeof(struct ObjectRuntimeDraft)==128?1:-1];

struct ObjectRuntimeDraft *FieldObject_Create(s32 resource, s32 x, s32 y, s32 z)
{
    struct ObjectRuntimeDraft *object;
    struct DispatchChild *child;
    struct DispatchChild **entry;
    struct ObjectDraftWork *work;
    s32 kind;
    u32 zero;
    u32 *src;

    kind = resource /4096;
    resource &=0xfff;
    object = ObjectDispatch_FindFreeObject();
    if (object !=0) {
        object->radius =16;
        switch (kind) {
        case 0:
            child = ResourceObject_Create(resource);
            if (child !=0) {
                object->animation_kind =1;
                object->animation = child;
                object->radius = Resource_GetMetadataRecordFar(resource)->box_y >>1;
            } else {
                object->animation_kind =0;
            }
            break;
        case 2:
            work = Ram_HeapSlots->unknown_18[0];
            entry = (struct DispatchChild **)((u8 *)work +8 +work->child_list_cursor++ *4);
            object->animation_kind =kind;
            src =&zero;
            /* FAKEMATCH:ordinary constructor392/78 includes16 differing bytes in the real DMA-zero pointer/store/setup sequence. As with the measured release, tie only the initialized used src pointer before its existing store; no zero-word read. */
            asm("" : "+r"(src));
            object->animation = entry;
            *src =0;
            Dma_Set(src, entry, 0x85000004, REG_DMA3);
            child = ResourceObject_Create(resource);
            if (child !=0) {
                object->radius = Resource_GetMetadataRecordFar(resource)->box_y >>1;
                *entry++ = child;
            }
            child = ResourceObject_Create(resource +1);
            if (child !=0)
                *entry = child;
            break;
        }
    }
    if (object !=0) {
        Object_SetPositionAndResetMotion((struct ObjectRuntime *)object, x, y, z);
        object->script = (s32 *)ObjectDispatch_Table6;
        object->speed_limit =0x20000;
        object->step =0;
        object->scale_x =0x10000;
        object->scale_y =0x10000;
        object->acceleration =0x10000;
        object->flags =3;
        object->value_48 =0x10000;
        object->value_44 =0x4000;
        object->value_59 =0;
        object->action_flags =1;
        object->value_4c =0;
        object->angle =0x4000;
        object->value_64.tile.x = x /65536;
        object->value_64.tile.z = z /65536;
        object->support_object =0;
        object->movement_dx =0;
        object->movement_dy =0;
        object->movement_dz =0;
    }
    return object;
}
