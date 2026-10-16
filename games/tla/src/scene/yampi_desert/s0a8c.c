#include "scene.h"

void Func_02000a8c(void) {
  s16 scene;

  Func_02004040();
  Func_020041b8(0);
  scene = gPartyState.scene;
  if (scene == (s32)&SceneId_Ov661Room4) {
    Camera_FollowActor(16, 1);
    Object_LinkObjectAndSetCallback(gPartyState.currentOwner, 16);
  } else if (scene == (s32)&SceneId_Ov661Room5) {
    Camera_FollowActor(21, 1);
    Object_LinkObjectAndSetCallback(gPartyState.currentOwner, 21);
  }
  /* FAKEMATCH: entering the loop at its test keeps the test single and the
   * address load inside the loop; as a while loop the test is copied above it
   * and the address kept in r5 */
  goto test;
  do {
    WaitFrames(1);
  test:;
  } while (gData_02004808 == 210);
  Func_02004048();
}
