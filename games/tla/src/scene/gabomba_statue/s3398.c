#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "event_inline.h"

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

void Func_02001c10(void);
void Func_020020f0(s32 value);
void Func_020034ec(void);
void Func_020034f4(void);
void Func_0200359c(void);
void Func_020035f0(void);
void Func_02003634(void);
void Func_02003868(void);

/// Prepares the current room.
s32 Func_02003398(void) {
  s16 scene;

  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect = 521;
  if (gMainRoutineWork.scene != (s32)&SceneId_Ov671Room1) {
    if (Event_GetFlag(515))
      Func_020020f0(0);
    else if (Event_GetFlag(0x8cd))
      Func_020020f0(-1);
    else
      Func_020020f0(1);
    Func_02001c10();
  }
  Actor_Get(FieldParty_GetLeaderActor())->priorityFlags |= 0x20;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov671Room1) {
    Func_020034ec();
  } else if (scene == (s32)&SceneId_Ov671Room2) {
    MapGrid_CopyCellAttributes(15, 68, 8, 3, 15, 4);
    Func_020034f4();
  } else if (scene == (s32)&SceneId_Ov671Room3) {
    Func_0200359c();
  } else if (scene == (s32)&SceneId_Ov671Room5) {
    Func_020035f0();
  } else if (scene == (s32)&SceneId_Ov671Room6) {
    Func_02003634();
  } else if (scene == (s32)&SceneId_Ov671Room7) {
    Func_02003868();
  }
  return 0;
}
