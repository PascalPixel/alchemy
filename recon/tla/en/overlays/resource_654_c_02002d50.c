// Draft, differ 6 of 106 (tla-en, 2026-11-05): the two-compare arms read through a local again; the older notes below still hold.
// Near miss, differ 6 of 106 (tla-en, 2026-10-31): both rows come from one
// insn, the state pointer's add. In the native it is in the jump-table block
// before sched2: its bb2 uses no call result, so the call keeps a low priority
// and the work load goes first. Formed before the switch, the add reads r0 in
// bb2, the call's 32-cycle latency lifts its priority, and sched2 sets the
// Actor_GetById argument first. Read as actor->state in each case, bb2 comes
// out in native order but each case forms its own pointer (differ 80). No
// pass here moves it into that block: Thumb runs no sched1, PRE finds no
// redundancy (0 insns created), and LCM never inserts on the edge into it.
// 26 October 2026, under -mthumb-slow-loads: differ 6 of 106: the native issues
// a load one slot later after a store, where alchemy-gcc issues it at once
// (one-pass after the work pointer leaves 4); alchemy permute (5 to 6 minutes)
// and an empty asm or one-pass block after any one statement do not move it.
// Near miss, differ 6 of 106: the touch dispatch of the wall room (☀️'s
// EV13_TOUCH shape): by the exit touched, 13 to 20, it runs one of the
// room's events when object 14's state allows. Left: the native loads the
// event work before it sets the Actor_GetById argument (one row), and it
// forms the state pointer (adds r0, #98) in the jump-table block after the
// range check; formed before the switch it comes ahead of the check, and
// formed in each case it stays there (differ 58).
#include "../../../../games/tla/src/scene/shrine_of_the_sea_god/shrine_of_the_sea_god.h"

void Func_020027f8(void);
void Func_02002830(void);
void Func_0200291c(void);
void Func_0200296c(void);
void Func_020029a4(void);
void Func_020029dc(void);
void Func_02002a2c(void);
void Func_02002b18(void);
void Func_02002b50(void);
void Func_02002c18(void);
void Func_02002c50(void);
void Func_02002c88(void);

void Func_02002d50(void) {
  struct ShrineOfTheSeaGodEventWork *work;
  u8 *state;

  if (!GameFlag_Test(0x87c))
    return;
  if (GameFlag_Test(115))
    return;
  work = Ram_HeapSlots->eventWork;
  state = &Actor_GetById(14)->state;
  switch (work->exit) {
  case 13:
    if (*state == 0)
      Func_020027f8();
    GameFlag_ClearBit(0x206);
    break;
  case 14:
    if (*state == 0)
      Func_020029a4();
    break;
  case 15:
    {
      u8 current = *state;

      if (current == 1)
        Func_02002830();
      else if (current == 4 && !GameFlag_Test(0x206))
        Func_02002b50();
    }
    break;
  case 16:
    {
      u8 current = *state;

      if (current == 1)
        Func_02002b18();
      else if (current == 5)
        Func_02002c50();
    }
    break;
  case 17:
    {
      u8 current = *state;

      if (current == 2)
        Func_0200291c();
      else if (current == 4)
        Func_02002c18();
    }
    break;
  case 18:
    {
      u8 current = *state;

      if (current == 2)
        Func_02002a2c();
      else if (current == 5 && !GameFlag_Test(0x206))
        Func_02002c88();
    }
    break;
  case 19:
    if (*state == 3)
      Func_0200296c();
    break;
  case 20:
    if (*state == 3)
      Func_020029dc();
    GameFlag_ClearBit(0x206);
    break;
  }
}
