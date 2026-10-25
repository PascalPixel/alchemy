#include "types.h"
#include "party_state.h"

s16 Event_GetSpecialValue(void) { return gPartyState.special; }

s32 GameFlag_Test(s32 flag);

extern s32 gData_080ef984[];

extern char ResourceId_DragonHallABackdrop;
extern char ResourceId_DragonHallBBackdrop;

/// The Broken Seal's BattleEffect_SetSpecialFromTable, with this game's
/// dragon hall check after the loop: looks the scene up in gData_080ef984 and
/// keeps the last marked backdrop before it; with flag 0xa30 set, the second
/// dragon hall backdrop becomes the first.
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
