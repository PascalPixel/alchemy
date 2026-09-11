#include "metadata_lookup.h"
#include "types.h"
#include "scene.h"
#include "battle_effect_runtime.h"

/* battle/effects/objects/copy_linked_object_position.c */
s16 *Battle_Run(s16);

s32 BattleFx_CopyLinkedObjectPosition(void *obj)
{
    void *link;

    link = FIELD_AT_OFFSET(obj, void **, 0x68);
    if (link != NULL) {
        FIELD_AT_OFFSET(obj, s8 *, 0x55) = 0;
        FIELD_AT_OFFSET(obj, s32 *, 8) = (s32)FIELD_AT_OFFSET(link, s32 *, 8);
        FIELD_AT_OFFSET(obj, s32 *, 0xC) = (s32)(FIELD_AT_OFFSET(link, s32 *, 0xC) + (FIELD_AT_OFFSET(Battle_unk2_4(*Battle_Run(FIELD_AT_OFFSET(obj, s16 *, 0x66))), s8 *, 8) << 0x10) + 0x80000);
        FIELD_AT_OFFSET(obj, s32 *, 0x14) = (s32)FIELD_AT_OFFSET(link, s32 *, 0x14);
        FIELD_AT_OFFSET(obj, s32 *, 0x10) = (s32)FIELD_AT_OFFSET(link, s32 *, 0x10);
    }
    return 0;
}

/* battle/effects/objects/spawn_linked_resource_object.c */
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

struct BattleEffectLinkedObject *Battle_Run(
    s32 kind,
    s32 x,
    s32 y,
    s32 z);

extern const u8 gRom[];

void BattleFx_SpawnLinked(
    s32 resource_id,
    s32 flags,
    s32 state)
{
    struct BattleEffectResource *resource;

    if ((flags & 0xff) == 6) {
        Battle_Do(110);
    }

    resource = ObjectTable_Get(resource_id);
    if (resource != 0) {
        struct BattleEffectLinkedObject *object =
            Battle_Run(21, resource->x, resource->y, resource->z);

        if (object != 0) {
            Battle_Apply(object, gRom);
            Battle_Apply2(object, flags & 15);
            object->value_55 = 0;
            object->counter = 0;
            object->resource_id = resource_id;
            object->callback = Battle_unk2_4;
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
                u8 flags;
                s32 clear_mask = 13;

                copied_flags &= source_flags;
                flags = object->visual->flags;
                clear_mask = -clear_mask;
                clear_mask &= flags;
                clear_mask |= copied_flags;
                object->visual->flags = clear_mask;
            }
        }
        Battle_unk2_2(state);
    }
}
