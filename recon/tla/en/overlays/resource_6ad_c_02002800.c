// Draft, differ 2 (tla-en, 2026-11-05). Func_02002800 of overlay 6ad.
// Overlay 6ab's Func_02001fcc with this overlay's symbols; see that draft.
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

s32 Func_02005690(void);
void Func_02005650(s32 a, s32 b);
void Func_020055c0(s32 id, s32 a);
void *Actor_GetById(s32 id);
struct BattleUnit *Unit_Get(s32 owner);

void Func_02002800(void) {
  struct MarsLighthouseEventWork *work = Ram_HeapSlots->eventWork;
  s32 leader = Func_02005690();
  s32 count;
  s32 i;
  s16 hp[8];

  Actor_GetById(leader);
  count = Party_CountActiveOwners();
  for (i = 0; i < count; i++)
    hp[i] = Unit_Get(gMainRoutineWork.activeOwners[i])->hp;
  Func_02005650(-10, 0);
  work->downCount = 0;
  work->standingCount = 0;
  Call2((void (*)())Func_020055c0, leader, 258);
  for (i = 0; i < count; i++) {
    u8 *main = (u8 *)&gMainRoutineWork;
    s32 at = i + 536;
    struct BattleUnit *unit = Unit_Get(main[at]);

    if (unit->hp > 0) {
      work->standingCount++;
    } else if (hp[i] != 0) {
      work->downOwners[(s16)work->downCount++] = main[at];
      work->unk16a = 0xffff;
      ((u8 *)unit)[0x131] = 0;
    }
  }
}
