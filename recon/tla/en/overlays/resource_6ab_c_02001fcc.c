// Draft, differ 70 (tla-en, 2026-11-05). Func_02001fcc of overlay 6ab.
// Plain loops under -Os. Left: allocation rank by one insn: the second loop's
// owner offset ranks 3 refs over 19 insns against the count's 5 over 64, so
// it takes r7 and the count r8; natively the count has r7, the leader then
// the offset r8 and the base sl.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
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
  s32 leader = Func_02004464();
  s32 count;
  s32 i;
  s16 hp[8];

  Actor_GetById(leader);
  count = Party_CountActiveOwners();
  for (i = 0; i < count; i++)
    hp[i] = Unit_Get(gMainRoutineWork.activeOwners[i])->hp;
  Func_02004454(-10, 0);
  work->downCount = 0;
  work->standingCount = 0;
  Func_020043bc(leader, 258);
  for (i = 0; i < count; i++) {
    struct BattleUnit *unit = Unit_Get(*(gMainRoutineWork.activeOwners + i));

    if (unit->hp > 0) {
      work->standingCount++;
    } else if (hp[i] != 0) {
      work->downOwners[(s16)work->downCount++] = gMainRoutineWork.activeOwners[i];
      work->unk16a = 0xffff;
      ((u8 *)unit)[0x131] = 0;
    }
  }
}
