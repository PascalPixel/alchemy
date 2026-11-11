#include "local.h"
#include "event_work.h"
#include "scene_ids.h"

struct MapBits {
  u8 unk00[23];
  u8 low : 2;
  u8 mode : 2;
  u8 high : 4;
};

/// The overlay's entry: sets the map mode and screen effect 0x202, then starts
/// each room's own setup.
s32 Func_02000fb8(void) {
  s32 *effect;
  s16 scene;
  struct MainRoutineWork *party;

  ((struct MapBits *)Ram_HeapSlots->mapWork)->mode = 1;
  effect = &((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect;
  *effect = 0x202;
  party = &gMainRoutineWork;
  scene = party->scene;
  if (scene == (s32)&SceneId_Ov679Room2) {
    Func_02001054();
  } else if (scene == (s32)&SceneId_Ov679Room1) {
    FieldTask_AddFrame(Func_02000540, 0x480);
    if (Event_GetFlag(0x95e))
      Func_02000974();
  } else if (scene == (s32)&SceneId_Ov679Room3) {
    *effect = 0x100;
    if (party->entrance == 6)
      Func_020010b4();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
