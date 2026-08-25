#include "types.h"

#define BattleEffect_FindDescriptor Func_0808d48c

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
    u8 unused_14[0x188];
    s16 limit;
};

struct SelectedEffectState {
    u8 unused_00[0x1f4];
    s32 object;
};

struct EffectObject {
    u8 unused_00[6];
    u16 reference;
};

extern u32 Func_0808d458(s32 descriptor, s32 value);
extern s32 Func_0808d428(s32 condition);
extern struct EffectObject *Func_0808ba1c(s32 object);

struct EffectDescriptor *BattleEffect_FindDescriptor(s32 kind, s32 value)
{
    struct EffectDescriptorRuntime *runtime =
        *(struct EffectDescriptorRuntime **)0x03001ebc;
    struct SelectedEffectState *state = (struct SelectedEffectState *)0x02000240;
    struct EffectDescriptor *descriptor = runtime->descriptors;
    u16 reference = Func_0808ba1c(state->object)->reference;

    while (descriptor->flags != -1) {
        s32 flags = descriptor->flags;
        if ((flags & 0xf) == kind && descriptor->value == value &&
            (Func_0808d458(flags, descriptor->result) != 0 ||
             Func_0808d428(descriptor->condition) != 0)) {
            s32 threshold = (flags & 0x800) != 0 ? 2 : 12;
            s32 range = flags & 0x600;
            s32 accepted = 0;

            if (range == 0) {
                accepted = 1;
            } else if (range == 0x200) {
                if (runtime->limit > threshold) {
                    accepted = 1;
                }
            } else if (range == 0x400) {
                accepted = (u16)(((flags & 0xf000) - reference) + 0x1fff) <=
                           0x3ffe;
            } else if (range == 0x600 && runtime->limit > threshold) {
                accepted = (u16)(((flags & 0xf000) - reference) + 0x1fff) <=
                           0x3ffe;
            }
            if (accepted) {
                return descriptor;
            }
        }
        descriptor++;
    }
    return 0;
}
