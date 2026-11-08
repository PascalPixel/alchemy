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
  Call2((void (*)())Func_020043bc, leader, 258);
  for (i = 0; i < count; i++) {
    struct MainRoutineWork *main = &gMainRoutineWork;
    struct BattleUnit *unit = Unit_Get(main->activeOwners[i]);

    if (unit->hp > 0) {
      work->standingCount++;
    } else if (hp[i] != 0) {
      work->downOwners[(s16)work->downCount++] = main->activeOwners[i];
      work->unk16a = 0xffff;
      ((u8 *)unit)[0x131] = 0;
    }
  }
}
