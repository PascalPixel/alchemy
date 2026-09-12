#include "types.h"
#include "scene.h"

/* battle/effects/data/lookup_result.c */
typedef struct Entry {
    s16 first;
    s16 second;
    s16 third;
    s16 result;
} Entry;

extern Entry gRom[];

s32 GameFlag_IsSet(s32 flag);
void BattleFx_SelectResultPointer(s32 arg0);

s32 BattleFx_LookupResult(void *arg0)
{
    s32 value;
    Entry *entry = gRom;
    s32 key = SceneData_Apply(arg0, &value);
    s32 result = 0;

    while (entry->first != -1) {
        if (entry->first == value &&
            (entry->second == -1 || entry->second == key) &&
            (entry->third == -1 || GameFlag_IsSet(entry->third) == 0)) {
            result = entry->result;
            break;
        }
        entry++;
    }
    BattleFx_SelectResultPointer(key);
    return result;
}

/* battle/effects/data/lookup_routines.c */
extern u8 *gWork;

s32 EffectRuntime_LookupByTableEntry(u32 index)
{
    u8 *table = gWork + 0x1a0;

    /* 第2引数は呼出元のr1を引き継ぐ特殊な呼出規約。 */
    return SceneData_ApplyLookupRoutines(table[index]);
}

s32 BattleFx_LookupResult(void *);

s32 BattleFx_ApplyLookupResult(s32 arg0, s32 arg1)
{
    return SceneData_ApplyLookupRoutines(BattleFx_LookupResult((void *)arg0), arg1);
}

extern u16 gRomLookupRoutines[];

u16 BattleFx_GetWeightedResult(s32 arg0, s32 arg1)
{
    return gRomLookupRoutines[(arg0 * 14) + arg1 + 2];
}

u16 BattleFx_GetWeightedResult(s32 arg0, s32 arg1);
extern u8 gRom2[];
unsigned char BattleFx_GetPhaseResult(s32 phase_index)
{
  s32 entry_offset;
  int entry_address;
  int weighted_index_address;
  u16 weighted_row;
  u8 *entry;
  u8 *weighted_row_address;
  entry_offset = phase_index * 4;
  entry = ((u8 *)entry_offset) + (s32)gRom2;
  weighted_index_address = entry_offset + (s32)gRom2;
  weighted_row_address = entry;
  weighted_row = *((u16 *)weighted_row_address);
  entry_address = weighted_index_address;
  /* Exact GCC 2.96 output carries this call's result through r0. */
  BattleFx_GetWeightedResult(
      weighted_row, *((u16 *)(((u8 *)entry_address) + 2)));
}

extern s16 gCell[];

struct SceneInteractionEntry {
    s16 id;
    s16 value : 15;
    u16 alt_source : 1;
    s16 condition;
    s16 result;
};

extern const struct SceneInteractionEntry gRom3[];


void Scene_ResolveInteractionResult(void)
{
    s16 result = 18;
    s16 progress = gCell[224];
    s16 sub = gCell[225];
    s16 alt = gCell[230];
    const struct SceneInteractionEntry *entry = gRom3;

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

    gCell[248] = result;
}
