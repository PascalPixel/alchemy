/* Draft, not exact (2026-09-24): 376 of 392 bytes, same instructions.
   Residual: literal pool placement. The reference dumps a pool after the
   case-0 branch and forces a second one, with a branch around it, before
   the tile-coordinate stores; this candidate keeps one pool at the end. */

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

extern struct ObjectSpriteList *Data_03001e68;
extern u8 Data_00000000[];

struct FieldObject *ObjectDispatch_FindFreeObject(void);
void *Func_0800bc70(s32 id);
struct AnimationMetadata *Func_08185000(s32 id);
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
        sprite = Func_0800bc70(id);
        if (sprite != NULL) {
            object->animation_kind = 1;
            object->animation = sprite;
            object->radius = Func_08185000(id)->radius >> 1;
        } else {
            object->animation_kind = (u8)(u32)Data_00000000;
        }
        break;
    case 2:
        list = (u32 *)Data_03001e68 + Data_03001e68->count++;
        entry = list + 2;
        object->animation_kind = kind;
        zero = 0;
        object->animation = entry;
        Dma_Set(&zero, entry, 0x85000004, (volatile u32 *)0x040000d4);
        sprite = Func_0800bc70(id);
        if (sprite != NULL) {
            object->radius = Func_08185000(id)->radius >> 1;
            *entry = (u32)sprite;
            entry = list + 3;
        }
        sprite = Func_0800bc70(id + 1);
        if (sprite != NULL)
            *entry = (u32)sprite;
        break;
    }
    }
    if (object != NULL) {
        Object_SetPositionAndResetMotion(object, x, y, z);
        object->script = 0x0801358c;
        object->speed_limit = 0x20000;
        object->unknown_04 = 0;
        object->scale_x = 0x10000;
        object->scale_y = 0x10000;
        object->acceleration = 0x10000;
        object->unknown_55 = 3;
        object->unknown_48 = 0x10000;
        object->unknown_44 = 0x4000;
        object->unknown_59 = (u8)(u32)Data_00000000;
        object->unknown_5a = 1;
        object->unknown_4c = 0;
        object->unknown_06 = 0x4000;
        object->tile_x = x / 65536;
        object->tile_z = z / 65536;
    }
    return object;
}
