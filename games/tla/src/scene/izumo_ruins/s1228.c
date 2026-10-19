#include "izumo_ruins.h"

/// Outside the first room, sets the high byte of map cell (50, 13) on layer 2 to
/// 0xff; then starts the room's effect.
void Func_020011d4(void) {
  if (gPartyState.scene == (s32)&SceneId_Ov67FRoom1) {
    Func_02001518(gData_02001654);
  } else {
    /* FAKEMATCH: the cell read through an s16 return type: ⚓️ masks the old
     * cell in r3 and builds 0xff00 in r2; read as s32 the two registers swap
     * (differ 6). */
    Func_02000ee8(
        2, 50, 13,
        0xff00 | (((s16 (*)(u32, s32, s32))Func_02000eac)(2, 50, 13) & 255));
    Func_02001518(gData_02001658);
  }
}

void Func_02001228(void) {
  Func_02001488();
  Func_020014e8(0);
  Func_02001520();
  if (gPartyState.scene == (s32)&SceneId_Ov67FRoom2)
    Func_02000ee8(2, 50, 13, Func_02000eac(2, 50, 13) & 255);
  Func_02001490();
}

void Func_02001270(void) {
  Func_02001488();
  Call6((void (*)())Func_02001460, 7, 2, 8, 7, 7, 12);
  Call6((void (*)())Func_02001450, 7, 66, 7, 76, 8, 9);
  GameFlag_SetBit(0x941);
  Func_02001490();
}

u8 *Func_020012b0(void) {
  s16 scene = gPartyState.scene;

  if (scene == (s32)&SceneId_Ov67FRoom1)
    return gData_020018ec;
  return gData_020019ac;
}
