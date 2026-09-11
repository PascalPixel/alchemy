#include "types.h"
#include "scene.h"
#include "abi/battle/effects/descriptors/run_action.h"

struct ActionDescriptor_0808d5dc {
    s16 id;
    u8 unused_02[20];
    s8 mode;
    u8 unused_17;
};

struct EffectDescriptor_0808d5dc {
    s32 flags;
    u8 unused_04[4];
    s32 result;
};

struct BattleActionObject {
    void **states;
    s16 state_index;
    u16 value;
    u8 unused_08[82];
    u8 flags_5a;
    u8 busy_5b;
    u8 unused_5c[8];
    u16 saved_value;
    u8 unused_66[2];
    void *linked_object;
};

extern struct ActionDescriptor *Battle_Run(s32);
extern struct EffectDescriptor *Battle_Run2(s32, s32);
extern struct BattleActionObject *Battle_Run3(s32);

extern u8 gCell;

s32 BattleFx_RunDescriptorAction(s32 id)
{
    struct ActionDescriptor_0808d5dc *action =
        (struct ActionDescriptor_0808d5dc *)Battle_Run(id);
    s32 result = -1;
    struct BattleActionObject *object = Battle_Run3(id);
    struct EffectDescriptor_0808d5dc *descriptor;
    s32 special = 0;
    u32 saved_value;
    u32 cleared_selection;
    u32 shifted_mode;
    s32 used_fallback = 0;
    s32 selected_offset = 0x24a;

    if (*(s16 *)((u8 *)&gCell + selected_offset) == id) {
        special = 1;
        descriptor = (struct EffectDescriptor_0808d5dc *)Battle_Run2(7, id);
        if (descriptor == 0) {
            descriptor = (struct EffectDescriptor_0808d5dc *)Battle_Run2(0, id);
            used_fallback = 1;
            if (descriptor == 0) {
                return -1;
            }
            if (descriptor->result >= 0x10000) {
                s32 index = Battle_Check(id);
                u32 random = Battle_Run4();
                s32 message =
                    0x0e0b + index * 2 + (random * 2 >> 16);
                Battle_Run5();
                Battle_Do(message);
                Battle_Apply(id, 0);
                Battle_Run6();
                goto finish;
            }
        }
    } else {
        descriptor = (struct EffectDescriptor_0808d5dc *)Battle_Run2(0, id);
    }

    if (descriptor == 0)
        goto finish;
    if (descriptor->result != 0)
        goto run_descriptor;
    goto finish;
run_descriptor:
    if (!special) {
        u8 *busy = &object->busy_5b;
        *busy = 1;
        Battle_Apply2(object, 0);
        saved_value = object->value;
        shifted_mode = (u8)action->mode << 24;
        if (shifted_mode <= (1 << 24) || shifted_mode == (3 << 24)) {
            s32 object_index = 250;
            s32 *object_slot =
                (s32 *)((s16 *)&gCell + object_index);
            struct BattleActionObject *linked = Battle_Run3(*object_slot);
            *(void **)((u8 *)linked + 56) = *(void **)((u8 *)linked + 8);
            *(void **)((u8 *)linked + 60) = *(void **)((u8 *)linked + 12);
            *(void **)((u8 *)linked + 64) = *(void **)((u8 *)linked + 16);
            *(s32 *)((u8 *)linked + 36) = 0;
            *(s32 *)((u8 *)linked + 40) = 0;
            *(s32 *)((u8 *)linked + 44) = 0;
            Battle_Place(id, *object_slot, 0);
        }
    }
    if (descriptor->result < 0x10000) {
        Battle_Do2(used_fallback);
        Battle_Run5();
        Battle_Do(descriptor->result);
        Battle_Apply(id, 0);
        Battle_Run6();
    } else {
        typedef void (*EffectRunner)(s32);
        Battle_Run7();
        ((EffectRunner)descriptor->result)(id);
    }
    if (!special) {
        if (object->states[object->state_index] == (void *)16) {
            if (action->mode == 3) {
                s32 object_index = 250;
                s32 object_id =
                    *(s32 *)((s16 *)&gCell + object_index);
                object->linked_object = Battle_Run3(object_id);
                object->flags_5a |= 1;
                Battle_Apply3(object, (void *)0x0809ff40);
            } else if (action->mode == 1) {
                object->saved_value = saved_value;
                Battle_Apply4(object, (void *)0x0809fc1c);
            }
        }
        object->busy_5b = 0;
        Battle_Apply2(object, 16);
    }
    result = 0;
finish:
    if (special) {
        s32 finish_selected_offset = 0x24a;
        s16 *selected =
            (s16 *)((u8 *)&gCell + finish_selected_offset);
        Battle_Do3(*selected);
        cleared_selection = 0xffff;
        *selected = cleared_selection;
    }
    return result;
}
