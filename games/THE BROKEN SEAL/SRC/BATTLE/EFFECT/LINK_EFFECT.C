#include "TYPES.H"

/* Object table: 192 pointers at Data_03001ebc + 0x14 (object/table/get.c). */
void *ObjectTable_Get(u32 object);

struct LinkedEffectVisual {
    u8 unknown_00[9];
    u8 flags;
    u8 unknown_0a[28];
    u8 value_26;
};

struct LinkedEffectObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[60];
    struct LinkedEffectVisual *visual;
    u8 value_54;
    u8 value_55;
    u8 unknown_56[14];
    u16 counter;
    u16 resource_id;
    struct LinkedEffectObject *resource;
    void (*callback)(void);
};

struct LinkedEffectObject *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
void Func_08009098(struct LinkedEffectObject *object, const void *configuration);
void Func_08009080(struct LinkedEffectObject *object, s32 mode);
void Func_080090d0(struct LinkedEffectObject *object);
void Func_0809376c(void);
extern const u8 Data_0809fd38[];

void BattleFx_ConfigureLinkedObject(s32 id, s32 flags)
{
    struct LinkedEffectObject *object = ObjectTable_Get(id);
    struct LinkedEffectObject *child;
    struct LinkedEffectVisual *visual;
    s32 mode;

    child = 0;
    visual = 0;

    if (object == 0)
        return;

    if ((flags & 3) != 0) {
        if ((flags & 3) == 2 || object->resource == 0) {
            child = Func_080090c8(209, object->x, object->y, object->z);
        }
    } else {
        child = object->resource;
        if (child == 0)
            return;
        Func_080090d0(child);
        /* FAKEMATCH: reuse the null visual value for the child link. */
        object->resource = (struct LinkedEffectObject *)visual;
        return;
    }

    if (child == 0)
        return;

    mode = flags & 3;
    switch (mode) {
    case 1:
        Func_08009080(child, 1);
        object->resource = child;
        child->counter = 1;
        break;
    case 2:
        Func_08009080(child, 2);
        Func_08009098(child, Data_0809fd38);
        child->counter = 1;
        break;
    }

    child->resource_id = id;
    child->value_55 = 0;
    child->callback = Func_0809376c;
    visual = child->visual;
    visual->value_26 = 0;
    child->resource = object;

    if (flags & 0x100) {
        s32 mask = 13;
        u8 visual_flags = visual->flags;

        mask = -mask;
        mask &= visual_flags;
        mask |= 4;
        visual->flags = mask;
    } else {
        s32 copied_flags = 12;
        u8 source_flags = object->visual->flags;
        u8 destination_flags;
        s32 clear_mask = 13;

        copied_flags &= source_flags;
        destination_flags = visual->flags;
        clear_mask = -clear_mask;
        clear_mask &= destination_flags;
        clear_mask |= copied_flags;
        visual->flags = clear_mask;
    }
}
