#include "types.h"

struct Fields_080b7f70 {
    u8 filler[0x50];
    void *field50;
    u8 field54;
};

struct EffectContext {
    u8 padding_000[32];
    u8 type;
    u8 padding_021[4];
    u8 dirty;
};

struct Effect {
    u8 padding_000[6];
    u8 state;
};

struct EffectRecord {
    struct Fields_080b7f70 *owner;
    u8 padding_004[4];
    u16 effectId;
    u8 padding_00a[18];
    u16 availableMask;
    u8 selectedBit;
    s8 cooldown;
    struct Effect *effect;
    void *secondaryEffect;
};

void *Func_080b7f70(struct Fields_080b7f70 *owner, s32 index);
struct Effect *Func_08009048(struct EffectContext *context, s32 effectId);
void Func_08009050(struct EffectContext *context, struct Effect *effect);
void Func_08009070(struct Effect *effect, s32 index);

/*
 * The reference preserves r0 in its epilogue (pop {r1}; bx r1), matching GCC's
 * scalar-return convention. No path establishes a meaningful result, and the
 * sole caller discards it; C99 6.9.1p12 only makes this fallthrough undefined
 * when the caller uses the value.
 */
s32 Func_080b7994(struct EffectRecord *record)
{
    struct Effect *oldEffect;
    struct Effect *effect;
    struct EffectContext *context;
    struct Fields_080b7f70 *owner;
    s32 effectId;
    s32 oldBit;
    s32 changed = 0;

    if (record->cooldown >= 0)
        record->cooldown--;

    oldEffect = record->effect;
    if (oldEffect == 0) {
        if ((s16)record->availableMask == 0)
            goto cooldown_expired;
        goto update;
    } else {
        if ((((s16)record->availableMask >> record->selectedBit) & 1) == 0)
            goto update;
    }
cooldown_expired:
    if (record->cooldown != 0)
        goto done;

update:
    effectId = -1;
    owner = record->owner;
    if ((s16)record->availableMask != 0) {
        oldBit = record->selectedBit;
        for (effectId = oldBit + 1;; effectId++) {
            if (effectId > 13)
                effectId = 0;
            if ((((s16)record->availableMask >> effectId) & 1) != 0)
                break;
        }

        if (oldBit != effectId || oldEffect == 0) {
            record->selectedBit = effectId;
            changed = 1;
        }
        record->cooldown = 80;
    } else {
        changed = 1;
    }

    context = Func_080b7f70(owner, 0);
    if (context == 0)
        goto done;

    if (effectId >= 0) {
        if (context->type == 32)
            effectId += 340;
        else
            effectId += 355;
    }

    if (record->effect != 0 && changed != 0) {
        Func_08009050(context, record->effect);
        record->effect = 0;
    }

    if (effectId >= 0 && changed != 0) {
        effect = Func_08009048(context, effectId);
        record->effect = effect;
        if (effect == (struct Effect *)-1)
            record->effect = 0;
        effect = record->effect;
        if (effect != 0) {
            effect->state = 3;
            Func_08009070(effect, 0);
        }
    }

    context->dirty = 1;
    if (effectId >= 0)
        record->effectId = effectId;
    else
        record->effectId = 0;

done:
    ;
}
