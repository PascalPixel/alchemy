#include "types.h"

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

extern struct ActionDescriptor *Func_0808d394(s32);
extern struct EffectDescriptor *Func_0808d48c(s32, s32);
extern struct BattleActionObject *Func_08092054(s32);
extern s32 Func_080915dc(s32);
extern u32 Func_08004458(void);
extern void Func_080916b0(void);
extern void Func_08092b94(s32);
extern void Func_08092f84(s32, s32);
extern void Func_08091750(void);
extern void Func_08009088(struct BattleActionObject *, s32);
extern void Func_08092848(s32, s32, s32);
extern void Func_08015058(s32);
extern void Func_08091660(void);
extern void Func_08093a6c(struct BattleActionObject *, void *);
extern void Func_08009098(struct BattleActionObject *, void *);
extern void Func_0809ade8(s32);
extern u8 Data_02000240;

s32 BattleEffect_RunDescriptorAction(s32 id)
{
    struct ActionDescriptor_0808d5dc *action =
        (struct ActionDescriptor_0808d5dc *)Func_0808d394(id);
    s32 result = -1;
    struct BattleActionObject *object = Func_08092054(id);
    struct EffectDescriptor_0808d5dc *descriptor;
    s32 special = 0;
    u32 saved_value;
    u32 cleared_selection;
    u32 shifted_mode;
    s32 used_fallback = 0;
    s32 selected_offset = 0x24a;

    if (*(s16 *)((u8 *)&Data_02000240 + selected_offset) == id) {
        special = 1;
        descriptor = (struct EffectDescriptor_0808d5dc *)Func_0808d48c(7, id);
        if (descriptor == 0) {
            descriptor = (struct EffectDescriptor_0808d5dc *)Func_0808d48c(0, id);
            used_fallback = 1;
            if (descriptor == 0) {
                return -1;
            }
            if (descriptor->result >= 0x10000) {
                s32 index = Func_080915dc(id);
                u32 random = Func_08004458();
                s32 message =
                    0x0e0b + index * 2 + (random * 2 >> 16);
                Func_080916b0();
                Func_08092b94(message);
                Func_08092f84(id, 0);
                Func_08091750();
                goto finish;
            }
        }
    } else {
        descriptor = (struct EffectDescriptor_0808d5dc *)Func_0808d48c(0, id);
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
        Func_08009088(object, 0);
        saved_value = object->value;
        shifted_mode = (u8)action->mode << 24;
        if (shifted_mode <= (1 << 24) || shifted_mode == (3 << 24)) {
            s32 object_index = 250;
            s32 *object_slot =
                (s32 *)((s16 *)&Data_02000240 + object_index);
            struct BattleActionObject *linked = Func_08092054(*object_slot);
            *(void **)((u8 *)linked + 56) = *(void **)((u8 *)linked + 8);
            *(void **)((u8 *)linked + 60) = *(void **)((u8 *)linked + 12);
            *(void **)((u8 *)linked + 64) = *(void **)((u8 *)linked + 16);
            *(s32 *)((u8 *)linked + 36) = 0;
            *(s32 *)((u8 *)linked + 40) = 0;
            *(s32 *)((u8 *)linked + 44) = 0;
            Func_08092848(id, *object_slot, 0);
        }
    }
    if (descriptor->result < 0x10000) {
        Func_08015058(used_fallback);
        Func_080916b0();
        Func_08092b94(descriptor->result);
        Func_08092f84(id, 0);
        Func_08091750();
    } else {
        typedef void (*EffectRunner)(s32);
        Func_08091660();
        ((EffectRunner)descriptor->result)(id);
    }
    if (!special) {
        if (object->states[object->state_index] == (void *)16) {
            if (action->mode == 3) {
                s32 object_index = 250;
                s32 object_id =
                    *(s32 *)((s16 *)&Data_02000240 + object_index);
                object->linked_object = Func_08092054(object_id);
                object->flags_5a |= 1;
                Func_08093a6c(object, (void *)0x0809ff40);
            } else if (action->mode == 1) {
                object->saved_value = saved_value;
                Func_08009098(object, (void *)0x0809fc1c);
            }
        }
        object->busy_5b = 0;
        Func_08009088(object, 16);
    }
    result = 0;
finish:
    if (special) {
        s32 finish_selected_offset = 0x24a;
        s16 *selected =
            (s16 *)((u8 *)&Data_02000240 + finish_selected_offset);
        Func_0809ade8(*selected);
        cleared_selection = 0xffff;
        *selected = cleared_selection;
    }
    return result;
}
