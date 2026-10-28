// Draft of overlay 6ab's Func_02001fcc, the twin of overlay 6aa's
// Func_020024c0: its draft (resource_6aa_c_020024c0.c) with this overlay's
// symbols: differ 90 of 108. Left: what that draft's header records.
#include "types.h"
#include "ram_buffer.h"
#include "sequence_parent_work.h"
#include "battle_unit.h"

struct MarsLighthouseEventWork {
  u8 unk000[0x16a];
  u16 unk16a;
  u16 downCount;
  u16 standingCount;
  u16 downOwners[8];
};

s32 Func_02004464(void);
void Func_02004454(s32 a, s32 b);
void Func_020043bc(s32 id, s32 a);
void *Actor_GetById(s32 id);
struct BattleUnit *Unit_Get(s32 owner);

void Func_02001fcc(void) {
  struct MarsLighthouseEventWork *work = Ram_HeapSlots->eventWork;
  s32 leader;
  s32 count;
  s32 i;
  s16 hp[8];

  leader = Func_02004464();
  Actor_GetById(leader);
  count = Party_CountActiveOwners();
  i = 0;
  if (i < count) {
  save:
    hp[i] = Unit_Get(gSequenceParentWork.activeOwners[i])->hp;
    i++;
    if (i < count)
      goto save;
  }
  Func_02004454(-10, 0);
  work->downCount = 0;
  work->standingCount = 0;
  Func_020043bc(leader, 258);
  i = 0;
  if (i < count) {
  check:
    {
      struct BattleUnit *unit;
      u8 *owner = &gSequenceParentWork.activeOwners[i];
      unit = Unit_Get(*owner);

      if (unit->hp > 0) {
        work->standingCount++;
      } else if (hp[i] != 0) {
        work->downOwners[(s16)work->downCount++] = *owner;
        work->unk16a = 0xffff;
        ((u8 *)unit)[0x131] = 0;
      }
    }
    i++;
    if (i < count)
      goto check;
  }
}
