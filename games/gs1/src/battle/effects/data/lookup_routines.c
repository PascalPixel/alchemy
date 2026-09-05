#include "types.h"

extern u8 *Data_03001ebc;

extern s32 Func_0808ae74();

s32 EffectRuntime_LookupByTableEntry(u32 index) {
    u8 *table = Data_03001ebc + 0x1a0;

    /* 第2引数は呼出元のr1を引き継ぐ特殊な呼出規約。 */
    return Func_0808ae74(table[index]);
}


s32 BattleEffect_LookupResult(void *);
s32 Func_0808ae74(s32, s32);

s32 BattleEffect_ApplyLookupResult(s32 arg0, s32 arg1) {
    return Func_0808ae74(BattleEffect_LookupResult((void *)arg0), arg1);
}

extern u16 Data_0809c610[];

u16 BattleEffect_GetWeightedResult(s32 arg0, s32 arg1) {
    return Data_0809c610[(arg0 * 14) + arg1 + 2];
}

u16 BattleEffect_GetWeightedResult(s32 arg0, s32 arg1);
extern u8 Data_0809d8b0[];
unsigned char BattleEffect_GetPhaseResult(s32 phase_index)
{
  s32 entry_offset;
  int entry_address;
  int weighted_index_address;
  u16 weighted_row;
  u8 *entry;
  u8 *weighted_row_address;
  entry_offset = phase_index * 4;
  entry = ((u8 *) entry_offset) + (s32)Data_0809d8b0;
  weighted_index_address = entry_offset + (s32)Data_0809d8b0;
  weighted_row_address = entry;
  weighted_row = *((u16 *) weighted_row_address);
  entry_address = weighted_index_address;
  /* Exact GCC 2.96 output carries this call's result through r0. */
  BattleEffect_GetWeightedResult(
      weighted_row, *((u16 *) (((u8 *) entry_address) + 2)));
}


extern s16 Data_02000240[];

struct SceneInteractionEntry {
    s16 id;
    s16 value : 15;
    u16 alt_source : 1;
    s16 condition;
    s16 result;
};

extern const struct SceneInteractionEntry Data_0809d9f0[];

s32 GameFlag_IsSet(s32 flag);

void FieldScene_ResolveInteractionResult(void)
{
    s16 result = 18;
    s16 progress = Data_02000240[224];
    s16 sub = Data_02000240[225];
    s16 alt = Data_02000240[230];
    const struct SceneInteractionEntry *entry = Data_0809d9f0;

    for (; entry->id != -1; entry++) {
        if (entry->alt_source) {
            if (entry->id != progress) {
                continue;
            }
            if (entry->value != -1 && entry->value != sub) {
                continue;
            }
            if (entry->condition != -1 && GameFlag_IsSet(entry->condition) == 0) {
                continue;
            }
            result = entry->result;
            break;
        } else {
            if (entry->id != alt) {
                continue;
            }
            if (entry->value != -1 && entry->value != sub) {
                continue;
            }
            if (entry->condition != -1 && GameFlag_IsSet(entry->condition) == 0) {
                continue;
            }
            result = entry->result;
            break;
        }
    }

    Data_02000240[248] = result;
}
