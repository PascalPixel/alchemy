/* Draft: complete 240-byte owner, 110 differing halfwords (44 aligned
 * edits). Corrects the stale child reuse and configuration at 0809fd38;
 * case 1 stores its retained mode, and late zeros use the actual pool.
 * Remaining: initial zero/mode sharing, pool order and visual allocation.
 * Separate initial flag tests did not close the gap.
 */
#include "TYPES.H"

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

struct BattleEffectLinkedObject_08093874 *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_InitializeFar(struct BattleEffectLinkedObject_08093874 *object, const void *configuration);
void Object_SetMode(struct BattleEffectLinkedObject_08093874 *object, s32 mode);
void Func_080090d0(struct BattleEffectLinkedObject_08093874 *object);
void BattleFx_CopyLinkedObjectPosition(void);
extern const u8 Data_0809fd38[];
extern const u8 Value_00000000;

void Func_08093874(s32 id, s32 flags)
{
    struct BattleEffectLinkedObject_08093874 *object = ObjectTable_Get(id);
    struct BattleEffectLinkedObject_08093874 *child;
    s32 mode;

    child = 0;

    if (object == 0)
        return;

    if ((flags & 3) != 0) {
        if ((flags & 3) == 2 || object->resource == 0) {
            child = Object_CreateFar(209, object->x, object->y, object->z);
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
        Object_SetMode(child, 1);
        object->resource = child;
        child->counter = mode;
        break;
    case 2:
        Object_SetMode(child, 2);
        ObjectDispatch_InitializeFar(child, Data_0809fd38);
        child->counter = 1;
        break;
    }

    child->resource_id = id;
    child->value_55 = (s32)&Value_00000000;
    child->callback = BattleFx_CopyLinkedObjectPosition;
    child->visual->value_26 = (s32)&Value_00000000;
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
