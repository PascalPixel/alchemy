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
  struct SequenceParentWork *party;

  ((struct MapBits *)Ram_HeapSlots->mapWork)->mode = 1;
  /* FAKEMATCH: as below. */
  asm("");
  effect = &((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect;
  party = &gSequenceParentWork;
  *effect = 0x202;
  /* FAKEMATCH: the empty asm takes gSequenceParentWork, so its address loads before
   * the 0x202 store and the scene's offset starts after it, as in the native
   * (plain: differ 2; the store before the party pointer: differ 2). */
  asm("" : : "r"(party));
  scene = party->scene;
  if (scene == (s32)&SceneId_Ov679Room2) {
    Func_02001054();
  } else if (scene == (s32)&SceneId_Ov679Room1) {
    Task_Add(Func_02000540, 0x480);
    if (GameFlag_Test(0x95e))
      Func_02000974();
  } else if (scene == (s32)&SceneId_Ov679Room3) {
    *effect = 0x100;
    /* FAKEMATCH: each empty asm keeps the native's statement order: the store
     * ahead of the next statement's constant (sched2 starts the constant first
     * otherwise; without this one differ 4, without the first differ 2). */
    asm("");
    if (party->entrance == 6)
      Func_020010b4();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
