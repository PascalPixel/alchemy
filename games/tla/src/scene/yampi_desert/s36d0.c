#include "scene.h"

void Func_020036d0(void) {
  if (Actor_GetById(64) != 0)
    Func_02004090(64, 0, 0);
  if (gSequenceParentWork.entrance == 2) {
    GameFlag_ClearBit(0xa29);
    GameFlag_ClearBit(0xa2a);
    GameFlag_SetBit(0xa2b);
  }
}

void Func_02001bec(void);
void Func_02002ebc(void);
void Func_02003da0(s32 unused, s32 value);
void Func_02003e3c(s32 a, s32 b, s32 c, s32 d, s32 e);

/// Room 10's setup.
void Func_0200371c(void) {
  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  GameFlag_SetBit(0x144);
  if (gSequenceParentWork.entrance == 1 && !GameFlag_Test(0x88c)) {
    Func_02002ebc();
  } else {
    /* FAKEMATCH: the memory clobber keeps GCSE from carrying gSequenceParentWork's
     * address from the first test into this one, which reloads it; plainly,
     * 13 instructions differ in tla-en. */
    asm("" ::: "memory");
    if (gSequenceParentWork.entrance == 2 && !GameFlag_Test(0x88c))
      Func_02001bec();
  }
  if (GameFlag_Test(0x8ed)) {
    Func_02003ff0(35, 39, 13, 11, 3, 3);
    Scene_Call4090(9, 232 << 16, 200 << 16);
  }
  Func_02003da0(0, 240);
  Func_02003e3c(0, 9, 0, 0, 64);
  wait(10);
}
