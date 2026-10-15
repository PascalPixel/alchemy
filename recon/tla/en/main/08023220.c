

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
    s32 terrainHeight;
    s32 scaleX;
    s32 scaleY;
    u16 radius;
    u8 terrainId;
    u8 unk23;
    s32 velocityX;
    s32 velocityY;
    s32 velocityZ;
    s32 speedLimit;
    s32 acceleration;
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    s32 value44;
    s32 value48;
    s32 value4c;
    void *animation;
    u8 animationKind;
    u8 flags;
    u8 motionFlags;
    u8 unk57[2];
    u8 value59;
    u8 actionFlags;
    u8 movementState;
    u8 unk5c[0x04];
    void *callback;
    union {
        struct { s16 action; s16 unknown; } motion;
        struct { s16 x; s16 z; } tile;
    } value64;
    struct ObjectRuntimeDraft *linkedObject;
    u8 unk6c[0x04];
    s32 movementDx;
    s32 movementDy;
    s32 movementDz;
    struct ObjectRuntimeDraft *supportObject;
};

struct ObjectDraftWork {
    u8 unk00[24];
    s32 childListCursor;
    u8 unk1c[64];
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
                object->animationKind =1;
                object->animation = child;
                object->radius = Resource_GetMetadataRecordFar(resource)->boxY >>1;
            } else {
                object->animationKind =0;
            }
            break;
        case 2:
            work = Ram_HeapSlots->unk18[0];
            entry = (struct DispatchChild **)((u8 *)work +8 +work->childListCursor++ *4);
            object->animationKind =kind;
            src =&zero;
            /* FAKEMATCH:ordinary constructor392/78 includes16 differing bytes in the real DMA-zero pointer/store/setup sequence. As with the measured release, tie only the initialized used src pointer before its existing store; no zero-word read. */
            asm("" : "+r"(src));
            object->animation = entry;
            *src =0;
            Dma_Set(src, entry, 0x85000004, REG_DMA3);
            child = ResourceObject_Create(resource);
            if (child !=0) {
                object->radius = Resource_GetMetadataRecordFar(resource)->boxY >>1;
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
        object->script = (s32 *)gObjectDispatch_Table6;
        object->speedLimit =0x20000;
        object->step =0;
        object->scaleX =0x10000;
        object->scaleY =0x10000;
        object->acceleration =0x10000;
        object->flags =3;
        object->value48 =0x10000;
        object->value44 =0x4000;
        object->value59 =0;
        object->actionFlags =1;
        object->value4c =0;
        object->angle =0x4000;
        object->value64.tile.x = x /65536;
        object->value64.tile.z = z /65536;
        object->supportObject =0;
        object->movementDx =0;
        object->movementDy =0;
        object->movementDz =0;
    }
    return object;
}
