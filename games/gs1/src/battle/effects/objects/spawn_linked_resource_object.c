#include "battle_effect_runtime.h"

struct BattleEffectVisual {
    u8 unknown_00[9];
    u8 flags;
    u8 unknown_0a[28];
    u8 value_26;
};

struct BattleEffectResource {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[60];
    struct BattleEffectVisual *visual;
};

struct BattleEffectLinkedObject {
    u8 unknown_00[80];
    struct BattleEffectVisual *visual;
    u8 value_54;
    u8 value_55;
    u8 unknown_56[14];
    u16 counter;
    u16 resource_id;
    struct BattleEffectResource *resource;
    void (*callback)(void);
};

struct BattleEffectLinkedObject *Func_080090c8(
    s32 kind,
    s32 x,
    s32 y,
    s32 z);
void Func_08009098(
    struct BattleEffectLinkedObject *object,
    const void *configuration);
void Func_08009080(struct BattleEffectLinkedObject *object, s32 mode);
void Func_080f9010(s32 cue);
void Func_0809163c(s32 state);
void Func_0809376c(void);
extern const u8 Data_0809fc2c[];

void BattleEffect_SpawnLinkedResourceObject(
    s32 resource_id,
    s32 flags,
    s32 state)
{
    struct BattleEffectResource *resource;

    if ((flags & 0xff) == 6) {
        Func_080f9010(110);
    }

    resource = ObjectTable_Get(resource_id);
    if (resource != 0) {
        struct BattleEffectLinkedObject *object =
            Func_080090c8(21, resource->x, resource->y, resource->z);

        if (object != 0) {
            Func_08009098(object, Data_0809fc2c);
            Func_08009080(object, flags & 15);
            object->value_55 = 0;
            object->counter = 0;
            object->resource_id = resource_id;
            object->callback = Func_0809376c;
            object->visual->value_26 = 0;
            object->resource = resource;

            if ((flags & 0x100) != 0) {
                s32 mask = 13;
                u8 visual_flags = object->visual->flags;

                mask = -mask;
                mask &= visual_flags;
                mask |= 4;
                object->visual->flags = mask;
            } else {
                s32 copied_flags = 12;
                u8 source_flags = resource->visual->flags;
                u8 destination_flags;
                s32 clear_mask = 13;

                copied_flags &= source_flags;
                destination_flags = object->visual->flags;
                clear_mask = -clear_mask;
                clear_mask &= destination_flags;
                clear_mask |= copied_flags;
                object->visual->flags = clear_mask;
            }
        }
        Func_0809163c(state);
    }
}
