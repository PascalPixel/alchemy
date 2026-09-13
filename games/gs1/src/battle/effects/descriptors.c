#include "types.h"

u32 BattleFx_CheckDescriptorKind3Result(s32 descriptor, s32 value)
{
  u32 tmp;
  s32 kind;
  u32 result;
  if ((0xF & descriptor) != 3)
  {
    return 0U;
  }
  kind = 0x1FF & descriptor;
  result = 0;
  if (kind != 3)
  {
    tmp = ((u32)value & 0xFFF00000) ^ 0x500000;
    result = tmp | (0U - tmp);
    result = result >> 0x1F;
  }
  return result;
}


struct EffectDescriptor {
    s32 flags;
    u8 value;
    u8 unused_05;
    s16 condition;
    s32 result;
};

struct EffectDescriptorRuntime {
    u8 unused_00[16];
    struct EffectDescriptor *descriptors;
    u8 unused_14[0x15c];
    s16 queued_result;
    u8 unused_172[0x2a];
    s16 limit;
};

struct EffectObject {
    u8 unused_00[6];
    u16 reference;
};

extern u32 Func_0808d458(s32 descriptor, s32 value);
extern s32 Func_0808d428(s32 condition);
extern struct EffectObject *Func_0808ba1c(s32 object);
extern u8 Data_02000240;
extern void *Data_03001ebc;

struct EffectDescriptor *BattleFx_FindDescriptor(s32 kind, s32 value)
{
    struct EffectDescriptorRuntime *runtime =
        (struct EffectDescriptorRuntime *)Data_03001ebc;
    struct EffectDescriptor *descriptor = runtime->descriptors;
    s32 state_index = 250;
    s32 flags;
    u32 reference =
        Func_0808ba1c(*(u32 *)((s16 *)&Data_02000240 + state_index))->reference;

    flags = descriptor->flags;
    while (flags != -1) {
        if ((flags & 0xf) == kind && descriptor->value == value &&
            (Func_0808d458(flags, descriptor->result) != 0 ||
             (Func_0808d428(descriptor->condition) != 0 &&
              (flags = descriptor->flags, 1)))) {
            s32 accepted = 0;
            s32 threshold = 12;
            s32 range;

            if ((flags & 0x800) != 0)
                threshold = 2;
            range = flags & 0x600;

            switch (range) {
            case 0:
                accepted = 1;
                break;
            case 0x200:
                if (runtime->limit > threshold) {
                    accepted = 1;
                }
                break;
            case 0x400:
                flags &= 0xf000;
                accepted = (u16)((flags - reference) + 0x1fff) <= 0x3ffe;
                break;
            case 0x600:
                if (runtime->limit > threshold) {
                    flags &= 0xf000;
                    accepted =
                        (u16)((flags - reference) + 0x1fff) <= 0x3ffe;
                }
                break;
            }
            if (accepted) {
                return descriptor;
            }
        }
        descriptor++;
        if (descriptor->flags == -1)
            break;
        flags = descriptor->flags;
    }
    return 0;
}


typedef s32 (*WorkEntryFn)(void);

void Runtime_CallWorkBlockEntry(void)
{
    s32 base = 0x02008000;
    ((WorkEntryFn)*(s32 *)(base + 4))();
}


struct EffectSelectionWork {
    u8 unk_00[0x24a];
    s16 value;
};

extern u8 Data_02000240;
struct EffectDescriptor *Func_0808d48c(s32, s32);

s32 BattleFx_FindDescriptorWithOverride(s32 arg0)
{
    struct EffectDescriptor *result = Func_0808d48c(0, arg0);
    s32 value = ((struct EffectSelectionWork *)&Data_02000240)->value;

    if (value == arg0) {
        struct EffectDescriptor *next = Func_0808d48c(7, value);

        if (next != 0) {
            return (s32)next;
        }
    }
    return (s32)result;
}


struct BattleEffectAction {
    s16 id;
    u8 unused_02[20];
    s8 mode;
    u8 unused_17;
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

s32 BattleFx_RunDescriptorAction(s32 id)
{
    struct BattleEffectAction *action =
        (struct BattleEffectAction *)Func_0808d394(id);
    s32 result = -1;
    struct BattleActionObject *object = Func_08092054(id);
    struct EffectDescriptor *descriptor;
    s32 special = 0;
    u32 saved_value;
    u32 cleared_selection;
    u32 shifted_mode;
    s32 used_fallback = 0;
    s32 selected_offset = 0x24a;

    if (*(s16 *)((u8 *)&Data_02000240 + selected_offset) == id) {
        special = 1;
        descriptor = (struct EffectDescriptor *)Func_0808d48c(7, id);
        if (descriptor == 0) {
            descriptor = (struct EffectDescriptor *)Func_0808d48c(0, id);
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
        descriptor = (struct EffectDescriptor *)Func_0808d48c(0, id);
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



s32 BattleFx_RunKind6DescriptorAction(s32 arg0)
{
    s32 val;
    s32 ret;
    void *p;

    p = Func_0808d48c(6, arg0);
    ret = -1;
    if (p != NULL) {
        val = *(s32 **)((u8 *)p + 8);
        if (val != 0) {
            if (val < 0x10000) {
                Func_080916b0();
                Func_08092b94(*(s32 **)((u8 *)p + 8));
                Func_08092f84(-1, 0);
                ret = 0;
                Func_08091750();
            } else {
                ((void (*)(s32))val)(arg0);
                ret = 0;
            }
        }
    }
    return ret;
}


typedef struct {
    s32 flags;
    u8 unk_04[4];
    s32 result;
    u8 unk_0c[0x170 - 0xC];
    s16 queued_result;
    u8 unk_172[0x19C - 0x172];
    s16 limit;
} EffectDescriptorWorkView;

void Audio_PlayCue(s32);
void Func_08094354(void);
void Func_08094368(void);

s32 BattleAction_RunDescriptor(s32 arg0)
{
    EffectDescriptorWorkView *desc;
    EffectDescriptorWorkView *desc2;
    s32 kind;
    s32 ret;
    EffectDescriptorWorkView *work;

    desc = (EffectDescriptorWorkView *)Func_0808d48c(2, arg0);
    ret = -1;
    work = *(EffectDescriptorWorkView **)0x03001ebc;
    if ((desc != 0) && (desc->result != 0)) {
        if (desc->flags & 0x200) {
            work->limit = 0;
        }
        if (desc->result < 0x10000) {
            Func_080916b0();
            Func_08092b94(desc->result);
            Func_08092f84(-1, 0);
            ret = 0;
            Func_08091750();
        } else {
            ((void (*)(s32))desc->result)(arg0);
            goto block_17;
        }
    } else {
        desc2 = (EffectDescriptorWorkView *)Func_0808d48c(1, arg0);
        if (desc2 != 0) {
            kind = desc2->flags & 0x30;
            switch (kind) {
            case 0: Audio_PlayCue(0x7B); break;
            case 32: Audio_PlayCue(0x80); Func_08094354(); break;
            case 48: Audio_PlayCue(0x81); Func_08094368(); break;
            }
            work->queued_result = (s16)desc2->result;
block_17:
            ret = 0;
        }
    }
    return ret;
}
