// Draft of overlay 6aa's Func_020024c0, written from its listing: it keeps the
// active members' HP, runs Func_02004d08 for the leader, then counts the
// members still standing and lists in the event work those it downed. Differ 90
// of 108. Left: register allocation (the native keeps the work in r6, the count
// in r7 and the leader in r8, and the owner byte's base and offset in sl and r8
// across the call, where this spills them to a 20-byte frame).
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "battle_unit.h"

struct MarsLighthouseEventWork {
  u8 unk000[0x16a];
  u16 unk16a;
  u16 downCount;
  u16 standingCount;
  u16 downOwners[8];
};

s32 Func_02004dc0(void);
void Func_02004da8(s32 a, s32 b);
void Func_02004d08(s32 id, s32 a);
void *Object_GetById(s32 id);
struct BattleUnit *Unit_Get(s32 owner);

void Func_020024c0(void) {
  struct MarsLighthouseEventWork *work = Ram_HeapSlots->eventWork;
  s32 leader;
  s32 count;
  s32 i;
  s16 hp[8];

  leader = Func_02004dc0();
  Object_GetById(leader);
  count = Party_CountActiveOwners();
  i = 0;
  if (i < count) {
  save:
    hp[i] = Unit_Get(gPartyState.activeOwners[i])->hp;
    i++;
    if (i < count)
      goto save;
  }
  Func_02004da8(-10, 0);
  work->downCount = 0;
  work->standingCount = 0;
  Func_02004d08(leader, 258);
  i = 0;
  if (i < count) {
  check:
    {
      struct BattleUnit *unit;
      u8 *owner = &gPartyState.activeOwners[i];
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
