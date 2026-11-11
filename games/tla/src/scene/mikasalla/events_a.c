#include "local.h"
#include "event_inline.h"

struct WarpEntry {
  s32 map;
  u16 x;
  u16 z;
};

extern struct WarpEntry gData_0200073c[];

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_0200009c(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  u32 id;
  s16 *choice;
  s32 index;

  Func_02000648();
  Event_ResolvePendingActions(0);
  for (id = 8; id <= 63; id++) {
    u8 *actor = (u8 *)Actor_GetById(id);

    if (actor != NULL) {
      u8 *flags = &actor[85];
      s32 zero = 0;

      *flags = zero;
    }
  }
  choice = (s16 *)(work + 340);
  index = *choice;
  Sound_Play(158);
  index--;
  MapGrid_AnimateCells(gData_0200073c[index].map, gData_0200073c[index].x,
                 gData_0200073c[index].z);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;

  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  if (index != 4) {
    Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
    Event_Wait(10);
  }
  Import_EventSetExitRequest(*choice);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02000650();
}

void Func_0200014c(void) { Func_080d7788(18, 2, 8); }

#include "scene_ids.h"

extern s32 gData_020012ac[];
extern s32 gData_020010d8[];

s32 *Func_0200015c(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65dRoom2)
    return gData_020012ac;
  return gData_020010d8;
}
