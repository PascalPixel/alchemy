#include "airs_rock_inside.h"

extern s16 gData_02006050[];
extern s16 gData_02006054[];
extern s16 gData_0200605a[];
extern s16 gData_02006064[];
extern s16 gData_02006068[];
extern s16 gData_02006072[];
extern s16 gData_0200607e[];

void Func_02005df4(s32 x, s32 z, s32 layer, s32 value);
void Func_02005f74(const s16 *list);

static inline void ClearLayer2Shape(s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline builds the shared z constant again
   * for the second call, as the native does; called directly, the constant is
   * kept in r5 across the first call (differ 12). */
  Func_02005df4(x, z, 2, 0);
}

/// Runs each room's list on entering it; room 7 also clears two cells' shapes
/// and room 9 stops object 10's timer.
void Func_02003254(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov664Room1) {
    Func_02005f74(gData_02006050);
  } else if (scene == (s32)&SceneId_Ov664Room2) {
    Func_02005f74(gData_02006054);
  } else if (scene == (s32)&SceneId_Ov664Room3) {
    Func_02005f74(gData_0200605a);
  } else if (scene == (s32)&SceneId_Ov664Room4) {
    Func_02005f74(gData_02006064);
  } else if (scene == (s32)&SceneId_Ov664Room7) {
    ClearLayer2Shape(0x2600000, 0x1500000);
    ClearLayer2Shape(0x2700000, 0x1500000);
    Func_02005f74(gData_02006068);
  } else if (scene == (s32)&SceneId_Ov664Room8) {
    Func_02005f74(gData_02006072);
  } else if (scene == (s32)&SceneId_Ov664Room9) {
    Actor_GetById(10)->timer = 0;
    Func_02005f74(gData_0200607e);
  }
}
