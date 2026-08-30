#include "types.h"

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
