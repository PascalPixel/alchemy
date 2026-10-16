// Draft, differ 13 of 32: the native reloads the party state address after the
// calls and saves no register (push {lr}); here it stays in r5 across them. The
// two pool words are the scene ids SceneId_Ov6ACRoom3 - SceneId_Ov6a8Room2, as
// in 6aa and 6ad. Tried: a party pointer in a block or reassigned, a flag
// variable, inline wrappers on either call, a one-pass block; the permuter
// finds nothing clean.
#include "../../../../games/tla/src/scene/mars_aerie/mars_aerie.h"

extern char SceneId_Ov6a8Room2;
extern char SceneId_Ov6ACRoom3;

s32 GameFlag_Test(s32 flag);
void Func_020057d4(s32 a);
void Func_020055dc(s32 scene, s32 a1);

s32 Func_02000450(void) {
  if (gPartyState.entrance == 1 && GameFlag_Test(0xa3d) == 0)
    Func_020057d4(1);
  Func_020055dc(gPartyState.map +
                    ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                0x5000000);
  return 0;
}
