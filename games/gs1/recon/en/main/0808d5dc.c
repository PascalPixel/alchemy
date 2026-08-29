#include "types.h"

#define BattleEffect_RunDescriptorAction Func_0808d5dc

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

struct BattleEffectState {
    u8 unused_00[0x1f4];
    s32 object;
    u8 unused_1f8[0x24a - 0x1f8];
    s16 selected_id;
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
extern void Func_080072f0();
extern void Func_08093a6c(struct BattleActionObject *, void *);
extern void Func_08009098(struct BattleActionObject *, void *);
extern void Func_0809ade8(s32);

s32 BattleEffect_RunDescriptorAction(s32 id)
{
    struct BattleEffectState *state = (struct BattleEffectState *)0x02000240;
    struct ActionDescriptor_0808d5dc *action =
        (struct ActionDescriptor_0808d5dc *)Func_0808d394(id);
    struct BattleActionObject *object = Func_08092054(id);
    struct EffectDescriptor_0808d5dc *descriptor;
    s32 special = 0;
    s32 used_fallback = 0;
    s32 result = -1;
    u16 saved_value;

    if (state->selected_id == id) {
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
                index = index * 2 + (Func_08004458() * 2 >> 16);
                Func_080916b0();
                Func_08092b94(*(s16 *)(0x0809ff40 + index));
                Func_08092f84(id, 0);
                Func_08091750();
                goto finish;
            }
        }
    } else {
        descriptor = (struct EffectDescriptor_0808d5dc *)Func_0808d48c(0, id);
        if (descriptor == 0) {
            goto finish;
        }
    }

    if (descriptor->result == 0) {
        goto finish;
    }
    if (!special) {
        object->busy_5b = 1;
        Func_08009088(object, 0);
        saved_value = object->value;
        if ((u8)action->mode <= 1 || (u8)action->mode == 3) {
            struct BattleActionObject *linked = Func_08092054(state->object);
            linked->linked_object = *(void **)((u8 *)linked + 8);
            *(void **)((u8 *)linked + 60) = *(void **)((u8 *)linked + 12);
            *(void **)((u8 *)linked + 64) = *(void **)((u8 *)linked + 16);
            *(s32 *)((u8 *)linked + 36) = 0;
            *(s32 *)((u8 *)linked + 40) = 0;
            *(s32 *)((u8 *)linked + 44) = 0;
            Func_08092848(id, state->object, 0);
        }
    }
    if (descriptor->result < 0x10000) {
        Func_08015058(used_fallback);
        Func_080916b0();
        Func_08092b94(descriptor->result);
        Func_08092f84(id, 0);
        Func_08091750();
    } else {
        Func_08091660();
        Func_080072f0(id, descriptor->result);
    }
    if (!special) {
        if (object->states[object->state_index] == (void *)16) {
            if (action->mode == 3) {
                object->linked_object = Func_08092054(state->object);
                object->flags_5a |= 1;
                Func_08093a6c(object, (void *)0x0809fc1c);
            } else if (action->mode == 1) {
                object->saved_value = saved_value;
                Func_08009098(object, (void *)0x0809ff40);
            }
        }
        object->busy_5b = 0;
        Func_08009088(object, 16);
    }
    result = 0;
finish:
    if (special) {
        volatile s16 *selected = (volatile s16 *)((u8 *)state + 0x24a);
        Func_0809ade8(*selected);
        *selected = -1;
    }
    return result;
}
