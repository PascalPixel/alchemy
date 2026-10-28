#include "trial_road.h"

extern char SceneId_Ov697Room1;
extern char SceneId_Ov697Room2;
extern char SceneId_Ov697Room3;
extern char SceneId_Ov697Room4;
extern s32 (*const gData_02005b64[])(void);

void Func_02003900(void);
void Func_02001dd0(void);
void Func_02001eb0(void);
void Func_0200200c(void);

s32 Func_020020c4(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  s32 pattern = gTrialRoadState->pattern;

  if (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 344) != 0)
    return 0;
  if (scene == (s32)&SceneId_Ov697Room1) {
    if (entrance <= 7) {
      GameFlag_SetBit(324);
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
      gData_02005b64[pattern & 7]();
    } else if (entrance == 55) {
      Func_02003900();
    }
  }
  if (scene == (s32)&SceneId_Ov697Room3)
    Func_02001dd0();
  if (scene == (s32)&SceneId_Ov697Room4)
    Func_02001eb0();
  if (scene == (s32)&SceneId_Ov697Room2) {
    GameFlag_SetBit(324);
    Func_0200200c();
  }
  return 0;
}
