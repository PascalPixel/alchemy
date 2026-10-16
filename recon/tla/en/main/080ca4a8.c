// Near miss, differ 2 of 44 (ported from The Broken Seal's
// BattleFx_SetSpecialFromTable, with ⚓️'s dragon hall check after the
// loop): the native loads the table (ldr r0) between the two halves of
// the scene offset (movs r2, #240 / lsls r2, r2, #1); this draft loads it
// after them. sched2 ranks the two at equal priority and the shift has more
// dependents. Declarations reordered, a while or for loop, the scene
// through a pointer, a block or empty asm before or after the table or the
// scene read all differ 2 to 19; permute (3 min) found nothing. It joins
// battle/effect/special.c, after Event_GetSpecialValue, when it matches.
#include "types.h"
#include "party_state.h"

s32 GameFlag_Test(s32 flag);

extern s32 gData_080ef984[];

extern char ResourceId_DragonHallABackdrop;
extern char ResourceId_DragonHallBBackdrop;

// Looks the scene up in gData_080ef984 and keeps the last marked backdrop
// before it; with flag 0xa30 set, the second dragon hall backdrop becomes
// the first.
void Func_080ca4a8(void) {
  s32 target = gPartyState.scene;
  s32 *table = gData_080ef984;
  s32 entry = *table++;
  s32 result; // not initialised: with no marked row before the scene it keeps
              // whatever it held

  if (entry != 0 && entry != target) {
    do {
      if (entry & 0x80000000) {
        result = entry & 0xFFFF;
      }
      entry = *table++;
    } while (entry != 0 && entry != target);
  }
  if (result == (s32)&ResourceId_DragonHallBBackdrop && GameFlag_Test(0xa30))
    result = (s32)&ResourceId_DragonHallABackdrop;
  gPartyState.special = result;
}
