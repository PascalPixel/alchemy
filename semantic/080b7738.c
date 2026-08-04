#include "types.h"

#define OFFSET_OF(type, member) ((u32)&(((type *)0)->member))

struct Component_080b7738 {
    u8 unknown_00[9];
    u8 flags_09;
};

struct ActorObject_080b7738 {
    u8 unknown_00[0x0c];
    s32 effect_state_enabled_0c;
    u8 unknown_10[0x40];
    void *components_50;
    u8 component_type_54;
};

struct Effect_080b7738 {
    u8 unknown_00[6];
    u8 state_06;
};

struct EffectContext_080b7738 {
    u8 unknown_00[0x25];
    u8 dirty_25;
};

struct BattleObjectSlot_080b7738 {
    struct ActorObject_080b7738 *object_00;
    u8 unknown_04[0x20];
    struct Effect_080b7738 *effect_24;
};

typedef char Assert_080b7738_component_flags[
    OFFSET_OF(struct Component_080b7738, flags_09) == 0x09 ? 1 : -1];
typedef char Assert_080b7738_actor_effect_state[
    OFFSET_OF(struct ActorObject_080b7738, effect_state_enabled_0c) == 0x0c
        ? 1 : -1];
typedef char Assert_080b7738_actor_components[
    OFFSET_OF(struct ActorObject_080b7738, components_50) == 0x50 ? 1 : -1];
typedef char Assert_080b7738_actor_component_type[
    OFFSET_OF(struct ActorObject_080b7738, component_type_54) == 0x54 ? 1 : -1];
typedef char Assert_080b7738_effect_state[
    OFFSET_OF(struct Effect_080b7738, state_06) == 0x06 ? 1 : -1];
typedef char Assert_080b7738_context_dirty[
    OFFSET_OF(struct EffectContext_080b7738, dirty_25) == 0x25 ? 1 : -1];
typedef char Assert_080b7738_slot_effect[
    OFFSET_OF(struct BattleObjectSlot_080b7738, effect_24) == 0x24 ? 1 : -1];

s32 Func_080b6c08(s32 groups, u16 *ids);
s32 Func_080b7994(struct BattleObjectSlot_080b7738 *slot);
struct BattleObjectSlot_080b7738 *Func_080b7dd0(s32 id);
struct EffectContext_080b7738 *Func_080b7f70(
    struct ActorObject_080b7738 *object, s32 index);

/*
 * Refresh actor effect state and assign the two battlefield sides opposing
 * display priorities.
 */
void Func_080b7738(void)
{
    u16 ids[14];
    s32 count;
    s32 index;
    s32 first_priority;
    s32 second_priority;

    Func_080b6c08(3, ids);
    for (index = 0; index < 14 && ids[index] != 0x00ff; index++) {
        struct BattleObjectSlot_080b7738 *slot = Func_080b7dd0(ids[index]);

        if (slot != 0) {
            struct ActorObject_080b7738 *object = slot->object_00;

            Func_080b7994(slot);
            if (slot->effect_24 != 0) {
                struct EffectContext_080b7738 *context =
                    Func_080b7f70(object, 0);

                if (context != 0) {
                    s32 desired_state =
                        object->effect_state_enabled_0c != 0 ? 9 : 0;

                    if (slot->effect_24->state_06 != desired_state) {
                        slot->effect_24->state_06 = desired_state;
                        context->dirty_25 = 1;
                    }
                }
            }
        }
    }

    if (*(volatile s16 *)(*(u8 **)0x03001e80 + 0x36) >= 0) {
        first_priority = 1;
        second_priority = 2;
    } else {
        first_priority = 2;
        second_priority = 1;
    }

    count = Func_080b6c08(1, ids);
    for (index = 0; index < count; index++) {
        struct BattleObjectSlot_080b7738 *slot = Func_080b7dd0(ids[index]);

        if (slot != 0) {
            struct ActorObject_080b7738 *object = slot->object_00;
            s32 type = object->component_type_54 & 0x0f;

            if (type == 1) {
                struct Component_080b7738 *component =
                    object->components_50;

                component->flags_09 =
                    (component->flags_09 & ~0x0c) |
                    ((first_priority & 3) << 2);
            } else if (type == 2) {
                struct Component_080b7738 **components =
                    object->components_50;
                s32 component_index;

                for (component_index = 0; component_index < 4;
                     component_index++) {
                    struct Component_080b7738 *component =
                        components[component_index];

                    if (component != 0) {
                        component->flags_09 =
                            (component->flags_09 & ~0x0c) |
                            ((first_priority & 3) << 2);
                    }
                }
            }
        }
    }

    count = Func_080b6c08(2, ids);
    for (index = 0; index < count; index++) {
        struct BattleObjectSlot_080b7738 *slot = Func_080b7dd0(ids[index]);

        if (slot != 0) {
            struct ActorObject_080b7738 *object = slot->object_00;
            s32 type = object->component_type_54 & 0x0f;

            if (type == 1) {
                struct Component_080b7738 *component =
                    object->components_50;

                component->flags_09 =
                    (component->flags_09 & ~0x0c) |
                    ((second_priority & 3) << 2);
            } else if (type == 2) {
                struct Component_080b7738 **components =
                    object->components_50;
                s32 component_index;

                for (component_index = 0; component_index < 4;
                     component_index++) {
                    struct Component_080b7738 *component =
                        components[component_index];

                    if (component != 0) {
                        component->flags_09 =
                            (component->flags_09 & ~0x0c) |
                            ((second_priority & 3) << 2);
                    }
                }
            }
        }
    }
}
