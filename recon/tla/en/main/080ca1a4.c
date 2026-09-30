#include "TYPES.H"

extern u8 *gEventWork;

extern s32 Encounter_SelectEnemyGroup();

unsigned char BattleFx_GetPhaseResult(s32 phase_index)
{
  s32 entry_offset;
  int entry_address;
  int weighted_index_address;
  u16 weighted_row;
  u8 *entry;
  u8 *weighted_row_address;
  entry_offset = phase_index * 4;
  entry = ((u8 *)entry_offset) + (s32)Encounter_AreaEntryTable;
  weighted_index_address = entry_offset + (s32)Encounter_AreaEntryTable;
  weighted_row_address = entry;
  weighted_row = *((u16 *)weighted_row_address);
  entry_address = weighted_index_address;
  /* Exact GCC 2.96 output carries this call's result through r0. */
  BattleFx_GetWeightedResult(
      weighted_row, *((u16 *)(((u8 *)entry_address) + 2)));
}
