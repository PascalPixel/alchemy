#include "jupiter_room_6.h"

void Func_020000b4(s32 mode) {
  if (mode == 1) {
    Func_02001bec(133);
    JupiterRoom6_MapPlay(84, 36, 2, 3, 78, 40);
    WaitFrames(6);
  }
  JupiterRoom6_MapPlay(84, 39, 2, 3, 78, 40);
  JupiterRoom6_BlendStart(20, 38, 2, 1, 14, 42);
}

void Func_02000108(void) {
  Func_02001bec(134);
  JupiterRoom6_MapPlay(84, 36, 2, 3, 78, 40);
  WaitFrames(6);
  JupiterRoom6_MapPlay(84, 42, 2, 3, 78, 40);
  JupiterRoom6_BlendStart(20, 44, 2, 1, 14, 42);
}

void Func_02000154(void) {
  struct JupiterRoom6Object *object = Object_GetById(12);

  if (!JupiterRoom6_FlagCheck(0xa19) && object->x >> 20 == 4 &&
      object->z >> 20 == 37) {
    JupiterRoom6_FlagSet(0xa19);
    Func_020000b4(1);
  }
}
