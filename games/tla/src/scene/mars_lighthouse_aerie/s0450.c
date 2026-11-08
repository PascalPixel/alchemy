#include "../../../../games/tla/src/scene/mars_lighthouse_aerie/mars_lighthouse_aerie.h"

extern char SceneId_Ov6a8Room2;
extern char SceneId_Ov6ACRoom3;

s32 GameFlag_Test(s32 flag);
void Func_020057d4(s32 a);
void Func_020055dc(s32 scene, s32 a1);

s32 Func_02000450(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 32 instructions
   * differ in tla-en. */
  if (gMainRoutineWork.entrance == 1 &&
      GameFlag_Test(SPLIT_CONSTANT(0xa3d)) == 0)
    Func_020057d4(1);
  Func_020055dc(gMainRoutineWork.map +
                    ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                0x5000000);
  return 0;
}
