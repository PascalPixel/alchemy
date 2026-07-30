#include "layout_guard.h"
#include "types.h"

typedef struct BattleState_080b9324 {
    u8 padding_00[0x45];
    u8 mode_45;
} BattleState_080b9324;

typedef struct OwnerMetadata_080b9324 {
    u8 padding_000[0x40];
    u16 weight_040;
    u8 padding_042;
    u8 entry_count_043;
    u8 padding_044[0xf7];
    u8 direct_flag_13b;
    u8 direct_flag_13c;
} OwnerMetadata_080b9324;

typedef struct BattleDecision_080b9324 {
    s16 owner_00;
    u8 padding_02[2];
    u16 weight_04;
    s16 state_06;
    u16 action_08;
    s16 target_0a;
    u8 padding_0c[4];
} BattleDecision_080b9324;

LAYOUT_OFFSET_GUARD(
    BattleState080b9324_Mode,
    BattleState_080b9324,
    mode_45,
    0x45);
LAYOUT_OFFSET_GUARD(
    OwnerMetadata080b9324_Weight,
    OwnerMetadata_080b9324,
    weight_040,
    0x40);
LAYOUT_OFFSET_GUARD(
    OwnerMetadata080b9324_EntryCount,
    OwnerMetadata_080b9324,
    entry_count_043,
    0x43);
LAYOUT_OFFSET_GUARD(
    OwnerMetadata080b9324_DirectFlag13b,
    OwnerMetadata_080b9324,
    direct_flag_13b,
    0x13b);
LAYOUT_OFFSET_GUARD(
    OwnerMetadata080b9324_DirectFlag13c,
    OwnerMetadata_080b9324,
    direct_flag_13c,
    0x13c);
LAYOUT_SIZE_GUARD(BattleDecision080b9324_Size, BattleDecision_080b9324, 0x10);
LAYOUT_OFFSET_GUARD(
    BattleDecision080b9324_Weight,
    BattleDecision_080b9324,
    weight_04,
    4);
LAYOUT_OFFSET_GUARD(
    BattleDecision080b9324_State,
    BattleDecision_080b9324,
    state_06,
    6);
LAYOUT_OFFSET_GUARD(
    BattleDecision080b9324_Action,
    BattleDecision_080b9324,
    action_08,
    8);
LAYOUT_OFFSET_GUARD(
    BattleDecision080b9324_Target,
    BattleDecision_080b9324,
    target_0a,
    0x0a);

extern BattleState_080b9324 *Data_03001e74;

u32 Func_08004458(void);
OwnerMetadata_080b9324 *Func_08077008(s32);
s32 Func_080b6b40(s32, u16 *);
void Func_080bd424(void *, s32);

/*
 * Build enemy battle decisions in randomized owner order.  Mode 2 limits both
 * the number of selected owners and each selected owner to one decision.
 */
s32 Func_080b9324(BattleDecision_080b9324 *output)
{
    volatile BattleState_080b9324 *battle = Data_03001e74;
    u16 owners[14];
    s32 selected_count;
    s32 produced = 0;
    s32 shuffle;
    s32 owner_number;

    if (battle->mode_45 == 1)
        return 0;

    selected_count = Func_080b6b40(2, owners);
    if (selected_count == 0)
        return 0;

    for (shuffle = 31; shuffle >= 0; shuffle--) {
        u32 first =
            (u32)(selected_count * Func_08004458()) >> 16;
        u32 second =
            (u32)(selected_count * Func_08004458()) >> 16;
        u16 temporary = owners[first];

        owners[first] = owners[second];
        owners[second] = temporary;
    }

    if (battle->mode_45 == 2) {
        s32 limit = ((u32)(Func_08004458() * 5) >> 16) + 1;

        if (limit <= 1)
            limit = 2;
        if (limit < selected_count)
            selected_count = limit;
    }

    for (owner_number = 0;
         owner_number < selected_count;
         owner_number++) {
        u16 owner = owners[owner_number];
        OwnerMetadata_080b9324 *metadata = Func_08077008(owner);
        s32 entry_number;

        for (entry_number = 0;
             entry_number < metadata->entry_count_043;
             entry_number++) {
            BattleDecision_080b9324 *decision = &output[produced];

            decision->owner_00 = owner;
            decision->weight_04 = metadata->weight_040;
            if (entry_number != 0) {
                decision->weight_04 =
                    (s16)metadata->weight_040 / 2;
            }

            if (metadata->direct_flag_13c != 0 ||
                metadata->direct_flag_13b != 0) {
                decision->state_06 = 8;
                decision->action_08 = 0;
                decision->target_0a = 0x100;
            } else {
                Func_080bd424(decision, 0);
            }

            produced++;
            if (battle->mode_45 == 2)
                break;
        }
    }

    return produced;
}
