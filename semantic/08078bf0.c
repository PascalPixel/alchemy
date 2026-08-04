#include "item.h"
#include "layout_guard.h"
#include "types.h"

struct EffectSlot_08078bf0 {
    u16 effect;
    u16 metadata;
};

struct OwnerEffects_08078bf0 {
    u8 padding000[0x0f];
    u8 level;
    u8 padding010[0x48];
    struct EffectSlot_08078bf0 effects[32];
    u16 inventory[15];
    u8 padding0f6[0x33];
    u8 effect_table_id;
};

struct EffectCandidate_08078bf0 {
    u8 effect;
    u8 minimum_level;
    u8 padding02[2];
};

struct EffectTable_08078bf0 {
    u8 padding00[0x10];
    struct EffectCandidate_08078bf0 candidates[16];
};

LAYOUT_SIZE_GUARD(EffectSlot08078bf0_Size, struct EffectSlot_08078bf0, 4);
LAYOUT_OFFSET_GUARD(
    OwnerEffects08078bf0_Level,
    struct OwnerEffects_08078bf0,
    level,
    0x0f);
LAYOUT_OFFSET_GUARD(
    OwnerEffects08078bf0_Effects,
    struct OwnerEffects_08078bf0,
    effects,
    0x58);
LAYOUT_OFFSET_GUARD(
    OwnerEffects08078bf0_Inventory,
    struct OwnerEffects_08078bf0,
    inventory,
    0xd8);
LAYOUT_OFFSET_GUARD(
    OwnerEffects08078bf0_EffectTableId,
    struct OwnerEffects_08078bf0,
    effect_table_id,
    0x129);
LAYOUT_SIZE_GUARD(
    EffectCandidate08078bf0_Size,
    struct EffectCandidate_08078bf0,
    4);
LAYOUT_OFFSET_GUARD(
    EffectTable08078bf0_Candidates,
    struct EffectTable_08078bf0,
    candidates,
    0x10);

struct OwnerEffects_08078bf0 *Func_08077394(s32 owner);
const struct EffectTable_08078bf0 *Func_08079ad8(s32 table_id);

static void ClearTransientEffects_08078bf0(
    struct EffectSlot_08078bf0 effects[32]
) {
    s32 index;

    for (index = 0; index < 32; index++) {
        if (effects[index].effect & 0x8000) {
            effects[index].effect = 0;
        }
    }
    for (index = 0; index < 32; index++) {
        if (effects[index].effect & 0x4000) {
            effects[index].effect = 0;
        }
    }
}

static void CompactEffectsRight_08078bf0(
    struct EffectSlot_08078bf0 effects[32]
) {
    s32 read_index;
    s32 write_index = 31;

    for (read_index = 31; read_index >= 0; read_index--) {
        if (effects[read_index].effect != 0) {
            effects[write_index].effect = effects[read_index].effect;
            write_index--;
        }
    }
    while (write_index >= 0) {
        effects[write_index].effect = 0;
        write_index--;
    }
}

static void CompactEffectsLeft_08078bf0(
    struct EffectSlot_08078bf0 effects[32]
) {
    s32 read_index;
    s32 write_index = 0;

    for (read_index = 0; read_index < 32; read_index++) {
        if (effects[read_index].effect != 0) {
            effects[write_index].effect = effects[read_index].effect;
            write_index++;
        }
    }
    while (write_index < 32) {
        effects[write_index].effect = 0;
        write_index++;
    }
}

static s32 HasExactEffect_08078bf0(
    const struct EffectSlot_08078bf0 effects[32],
    u16 effect
) {
    s32 index;

    for (index = 0; index < 32; index++) {
        if (effects[index].effect == effect) {
            return 1;
        }
    }
    return 0;
}

static s32 HasBaseEffect_08078bf0(
    const struct EffectSlot_08078bf0 effects[32],
    u16 effect
) {
    s32 index;

    for (index = 0; index < 32; index++) {
        if ((effects[index].effect & 0x3fff) == effect) {
            return 1;
        }
    }
    return 0;
}

static s32 InsertEffect_08078bf0(
    struct EffectSlot_08078bf0 effects[32],
    u16 effect
) {
    s32 index;

    for (index = 0; index < 32; index++) {
        if (effects[index].effect == 0) {
            effects[index].effect = effect;
            return 1;
        }
    }
    return 0;
}

static u8 ItemEffectKind_08078bf0(const struct ItemDefinition *definition) {
    return ((const u8 *)definition)[0x0c];
}

static u16 ItemEffect_08078bf0(const struct ItemDefinition *definition) {
    return *(const u16 *)((const u8 *)definition + 0x28);
}

/*
 * Refresh the owner's 32 sparse effect slots. Stale generated entries are
 * removed, survivors are packed aside, eligible class-table and inventory
 * effects are inserted, and the finished list is compacted to the front.
 */
s32 Func_08078bf0(s32 owner) {
    struct OwnerEffects_08078bf0 *state = Func_08077394(owner);
    const struct EffectTable_08078bf0 *table =
        Func_08079ad8(state->effect_table_id);
    s32 index;

    if (state->effect_table_id == 0) {
        return 0;
    }

    ClearTransientEffects_08078bf0(state->effects);
    CompactEffectsRight_08078bf0(state->effects);

    for (index = 0; index < 16; index++) {
        const struct EffectCandidate_08078bf0 *candidate =
            &table->candidates[index];

        if (candidate->effect == 0 || state->level < candidate->minimum_level) {
            continue;
        }
        if (HasExactEffect_08078bf0(state->effects, candidate->effect)) {
            continue;
        }
        if (!InsertEffect_08078bf0(
                state->effects,
                (u16)(candidate->effect | 0x8000))) {
            break;
        }
    }

    for (index = 0; index < 15; index++) {
        u16 item = state->inventory[index];
        struct ItemDefinition *definition;
        u16 effect;

        if (item == 0 || !(item & 0x0200)) {
            continue;
        }

        definition = Func_08078414(item);
        if (ItemEffectKind_08078bf0(definition) != 3) {
            continue;
        }

        effect = ItemEffect_08078bf0(definition);
        if (HasBaseEffect_08078bf0(state->effects, effect)) {
            continue;
        }
        if (!InsertEffect_08078bf0(state->effects, (u16)(effect | 0x4000))) {
            break;
        }
    }

    CompactEffectsLeft_08078bf0(state->effects);
    return 0;
}
