// Draft, differ 82 of 108, skeleton gap 26 (tla-en, 2026-11-01)
// Overlay 6ad's Func_02002800, the twin of overlay 6aa's Func_020024c0: it keeps
// the active members' HP, runs Func_020055c0 for the leader, then counts the
// members still standing and lists in the event work those it downed.
// Left: the second loop's owner read and allocation. The native keeps the work
// in r6, the count in r7, the leader then the owner offset in r8, and the owner
// byte's base in sl; hp's address gets no register (mov r1, sp at the store).
// Here gcse hoists hp's address, which outranks the count for a low register
// (3 refs over 12 insns against 7 over 64), so the count falls to r8. With the
// second read as activeOwners[i] the first's base and offset are reused, as in
// the native, but they take r4 and r6 and the frame grows to 20 for a caller
// save; the native's need refs of 4 or fewer to stay out of r4, or a form not
// found yet. The *(p + i) spelling recomputes the address instead: fewer
// differences, wrong shape. Stopping CSE from following the else branch freed
// the low registers (work reached r6) but added code.
// Tried: for loops (loop.c hoists and strength-reduces, so the gotos stay), a
// pointer to hp in both loops (it takes sl and loses r6), an owner pointer
// (draft's old form, gap 30), temporaries for owner, HP and the slot, gotos for
// the else, separate counters, declaration orders.
#include "types.h"
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
  s32 leader;
  s32 count;
  s32 i;
  s16 hp[8];

  leader = Func_02005690();
  Actor_GetById(leader);
  count = Party_CountActiveOwners();
  i = 0;
  if (i < count) {
  save:
    hp[i] = Unit_Get(gMainRoutineWork.activeOwners[i])->hp;
    i++;
    if (i < count)
      goto save;
  }
  Func_02005650(-10, 0);
  work->downCount = 0;
  work->standingCount = 0;
  Func_020055c0(leader, 258);
  i = 0;
  if (i < count) {
  check:
    {
      struct BattleUnit *unit = Unit_Get(gMainRoutineWork.activeOwners[i]);

      if (unit->hp > 0) {
        work->standingCount++;
      } else if (hp[i] != 0) {
        work->downOwners[(s16)work->downCount++] = *(gMainRoutineWork.activeOwners + i);
        work->unk16a = 0xffff;
        ((u8 *)unit)[0x131] = 0;
      }
    }
    i++;
    if (i < count)
      goto check;
  }
}
