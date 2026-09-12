#include "battle_summon.h"
#include "gs1_edition.h"

struct SummonChargeState {
    u8 unknown_00[0x10];
    u16 class_ids[6];
    u32 used_masks[6];
    s8 channels[6];
    u8 unknown_3a[6];
    u8 count;
};

struct BattleActorDefinition {
    u8 name[14];
    u8 unknown_0e[282];
    u8 class_id;
    u8 unavailable;
};

extern struct SummonChargeState *Data_03001e74;

struct BattleActorDefinition *Func_08077008(s32 actor_id);

s32 Summon_ReleaseCharge(s32 actor_id)
{
    struct SummonChargeState *state = Data_03001e74;
    struct BattleActorDefinition *actor;
    s32 count = state->count;
    s32 index;
    s32 name_length;
    s32 bit;
    s32 class_id;

    actor = Func_08077008(actor_id);
    if (actor->unavailable != 0)
        return;

    class_id = actor->class_id;
    for (index = 0; index < count; index++) {
        if (state->class_ids[index] == class_id)
            break;
    }
    if (index == count || state->used_masks[index] == 0)
        return;

    for (name_length = 0; name_length <= 13; name_length++) {
        if (actor->name[name_length] == 0)
            break;
    }

    bit = 32;
    if (name_length > 0)
        bit = actor->name[name_length - 1] - LIST_MARKER_CHAR;
    state->used_masks[index] &= ~(1 << bit);
}
