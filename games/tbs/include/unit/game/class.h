#ifndef ALCHEMY_UNIT_GAME_CLASS_H
#define ALCHEMY_UNIT_GAME_CLASS_H

#include "types.h"

struct BtlUnit;
struct ClassDefinition;
struct SummonDefinition;

s32 Trade_ListFlaggedEntries(u8 *output);
const struct SummonDefinition *SummonDefinition_GetNear(u32 summon_id);
s32 Owner_LookupFourColumnTable(s32 row, s32 column);
s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 output[4]);
s32 Owner_GetResistanceValue(s32 owner, s32 index);
s32 Owner_GetDefaultElement(struct BtlUnit *state);
u32 Owner_CalculateBaseElements(s32 owner, s16 destination[4][2]);
s32 Owner_DetermineClass(s32 character, const u8 *djinn);
struct ClassDefinition *Owner_GetRecordStride84(s32 arg0);
void Owner_RefreshDerivedData(s32 owner_no);

#endif
