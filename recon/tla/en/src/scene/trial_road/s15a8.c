#include "../../../../../../games/tla/src/scene/trial_road/trial_road.h"

void Func_02002dec(void);
void Func_0200322c(void);

void Func_020015a8(void) { GameFlag_SetBit(SPLIT_CONSTANT(SPLIT_CONSTANT(0x18a, 255))); }

void Func_020015b8(void) { Func_02005a50_(68, 6, 1, 21, 13, 64, 64); }

void Func_020015d8(void) {
  Func_02005a50_(68, 6, 1, 21, 13, 64, 64);
  if (!GameFlag_Test(SPLIT_CONSTANT(0x9b9))) {
    Func_02005a60(249, 72, 152);
    ((struct MainRoutineWork254 *)&gMainRoutineWork)->value = 0x19b9;
    gMainRoutineWork.battleRule24b = 2;
    Func_02005998(106, 4);
  }
}

s32 Func_02001638(void) { return 1; }

void Func_0200163c(void) {
  Func_02005810_(33, 30, 1, 1, 10, 11);
  Func_02005810(33, 30, 1, 1, 18, 7);
  Func_02005810(33, 30, 1, 1, 18, 15);
  Func_02005810(35, 30, 1, 1, 74, 11);
  Func_02005810(35, 30, 1, 1, 82, 7);
  Func_02005810(35, 30, 1, 1, 82, 15);
}

void Func_020016c8(void) {
  Func_02005810_(34, 30, 1, 1, 10, 11);
  Func_02005810(34, 30, 1, 1, 18, 7);
  Func_02005810(34, 30, 1, 1, 18, 15);
  Func_02005810(36, 30, 1, 1, 74, 11);
  Func_02005810(36, 30, 1, 1, 82, 7);
  Func_02005810(36, 30, 1, 1, 82, 15);
}

void Func_02001754(s32 sound) {
  if (GameFlag_Test(SPLIT_CONSTANT(SPLIT_CONSTANT(0x900, 255)))) {
    if (gTrialRoadState->mirrored == 0) {
      Map_Stamp(26, 79, 1, 2, 13, 79);
      Map_CopyCells(78, 18, 1, 1, 77, 18);
    } else {
      Func_02005820(26, 79, 1, 2, 18, 79);
      Func_02005818(81, 18, 1, 1, 82, 18);
    }
    if (sound)
      Func_02005a68(138);
  }
  GameFlag_SetBit(SPLIT_CONSTANT(SPLIT_CONSTANT(0x184, 255)));
}

void Func_020017e0(void) {
  struct MainRoutineWork *party;
  s32 count = GameFlag_Test(SPLIT_CONSTANT(SPLIT_CONSTANT(0x300, 129)));

  count += GameFlag_Test(0x382);
  count += GameFlag_Test(SPLIT_CONSTANT(SPLIT_CONSTANT(0x284, 255)));
  count += GameFlag_Test(0x384);
  if (GameFlag_Test(0x385))
    Func_02002dec();
  else
    Func_0200322c();
  party = &gMainRoutineWork;
  party->battleRule24b = 2;
  do {
  } while (0);
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov697Room2, 4);
  Party_SetFields1f2And1f4((s32)&SceneId_Ov697Room2, 5);
  Func_02005998(13, 4 - count);
}

/// When the object stands in column 8, sets flag 0x9fc, stops its motion and
/// lifts it one pixel.
void Func_0200185c(s32 a, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object->x >> 20 == 8) {
    GameFlag_SetBit(SPLIT_CONSTANT(SPLIT_CONSTANT(0x900, 252)));
    object->motionFlags = 0;
    object->y -= 0x10000;
  }
}
