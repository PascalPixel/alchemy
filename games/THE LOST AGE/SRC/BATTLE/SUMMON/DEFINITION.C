#include "TYPES.H"

struct Entry {
    u16 value;
    u8 flags0;
    u8 flatbs;
    u8 rest[4];
};

extern struct Entry Summon_EntryTable[];

u32 Summon_GetEntryByte3Kind(s32 arg0)
{
  u32 kind;
  u8 *p;
  p = (u8 *)((arg0 * 8) + (s32)Summon_EntryTable);
  kind = ((u8)(*((u8 *)(p + 3)))) >> 5;
  if (((s32)kind) > 4)
  {
    kind = -1U;
  }
  return kind;
}
