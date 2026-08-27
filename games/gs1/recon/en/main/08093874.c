#include "types.h"

#define ObjectTable_Get Func_0808ba1c
/* Object table: 192 pointers at Data_03001ebc + 0x14 (object/table/get.c). */
void *ObjectTable_Get(u32 object);

struct BattleEffectVisual_08093874 {
    u8 unknown_00[9];
    u8 flags;
    u8 unknown_0a[28];
    u8 value_26;
};

struct BattleEffectLinkedObject_08093874 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[60];
    struct BattleEffectVisual_08093874 *visual;
    u8 value_54;
    u8 value_55;
    u8 unknown_56[14];
    u16 counter;
    u16 resource_id;
    struct BattleEffectLinkedObject_08093874 *resource;
    void (*callback)(void);
};

struct BattleEffectLinkedObject_08093874 *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
void Func_08009098(struct BattleEffectLinkedObject_08093874 *object, const void *configuration);
void Func_08009080(struct BattleEffectLinkedObject_08093874 *object, s32 mode);
void Func_080090d0(struct BattleEffectLinkedObject_08093874 *object);
void Func_0809376c(void);
extern const u8 Data_0809fc2c[];

void Func_08093874(s32 id, s32 flags)
{
    struct BattleEffectLinkedObject_08093874 *object = ObjectTable_Get(id);
    struct BattleEffectLinkedObject_08093874 *child;
    s32 mode;

    child = 0;

    if (object == 0)
        return;

    mode = flags & 3;
    if (mode != 0) {
        if (mode == 2 || object->resource == 0) {
            child = Func_080090c8(209, object->x, object->y, object->z);
        } else {
            child = object->resource;
        }
    } else {
        child = object->resource;
        if (child == 0)
            return;
        Func_080090d0(child);
        object->resource = 0;
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
        Func_08009098(child, Data_0809fc2c);
        child->counter = 1;
        break;
    }

    child->resource_id = id;
    child->value_55 = 0;
    child->callback = Func_0809376c;
    child->visual->value_26 = 0;
    child->resource = object;

    if (flags & 0x100) {
        s32 mask = 13;
        u8 visual_flags = child->visual->flags;

        mask = -mask;
        mask &= visual_flags;
        mask |= 4;
        child->visual->flags = mask;
    } else {
        s32 copied_flags = 12;
        u8 source_flags = object->visual->flags;
        u8 destination_flags;
        s32 clear_mask = 13;

        copied_flags &= source_flags;
        destination_flags = child->visual->flags;
        clear_mask = -clear_mask;
        clear_mask &= destination_flags;
        clear_mask |= copied_flags;
        child->visual->flags = clear_mask;
    }
}
