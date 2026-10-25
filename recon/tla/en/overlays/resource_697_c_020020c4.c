// Draft, differ 6 of 71: the Trial Road's per-map step check, while no event
// runs (event work byte 344 clear): in room 1 entrances up to 7 set flag 324,
// set the leader's bit 6 of priority flags and run the stage pattern's handler
// (gData_02005b64 by pattern & 7), entrance 55 runs Func_02003900; rooms 2 to 4
// run their own handlers (room 2 after setting flag 324). Returns 0.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"

extern char SceneId_Ov697Room1;
extern char SceneId_Ov697Room2;
extern char SceneId_Ov697Room3;
extern char SceneId_Ov697Room4;
extern void (*const gData_02005b64[])(void);

void Func_02003900(void);
void Func_02001dd0(void);
void Func_02001eb0(void);
void Func_0200200c(void);

s32 Func_020020c4(void) {
  s32 scene = gPartyState.scene;
  s32 entrance = gPartyState.entrance;
  s32 pattern = gTrialRoadState->pattern;

  if (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 344) != 0)
    return 0;
  if (scene == (s32)&SceneId_Ov697Room1) {
    if (entrance <= 7) {
      GameFlag_SetBit(324);
      Object_GetById(gPartyState.currentOwner)->priorityFlags |= 64;
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
