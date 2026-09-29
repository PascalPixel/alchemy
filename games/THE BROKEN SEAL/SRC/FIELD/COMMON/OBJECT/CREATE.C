/* The engine's object constructor, reached from far code through
   Object_CreateFar: takes a free object slot, attaches the sprite or the
   two-sprite list the descriptor id names (its top nibble is the kind), and
   resets position, scale, speed and script to their defaults. */
#include "DMA.H"

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

struct FieldObject *ObjectDispatch_FindFreeObject(void);
void *ResourceObject_Create(s32 id);
struct AnimationMetadata *Resource_GetMetadataRecordFar(s32 id);
void Object_SetPositionAndResetMotion(struct FieldObject *object, s32 x, s32 y, s32 z);

struct FieldObject *Func_0800c150(s32 id, s32 x, s32 y, s32 z)
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
    object = ObjectDispatch_FindFreeObject();
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
