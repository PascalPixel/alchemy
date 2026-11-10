#include "types.h"
#include "scene_tables.h"
#include "gabomba_statue_chamber.h"
#include "event_inline.h"

void Func_02000f00(void);
void Func_02001010(void);
void Func_02001064(void);
void Func_020010d4(void);
void Func_02001710(void);
void Func_02001b14(void);
void Func_02001ba8(void);
void Func_02001c44(void);

s32 Func_02001aa4(void) {
  struct GabombaStatueChamberEventWork *work;
  s16 scene;

  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 521;
  ((struct GabombaStatueChamberObject *)Actor_GetById(Party_GetLeaderActor()))
      ->priorityFlags |= 0x20;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov672Room1)
    Func_02001b14();
  else if (scene == (s32)&SceneId_Ov672Room2)
    Func_02001ba8();
  else if (scene == (s32)&SceneId_Ov672Room3)
    Func_02001c44();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02001b14(void) {
  Func_02001e80(8);
  Func_02001e80(9);
  Func_02001e80(10);
  wait(1);
  if (GameFlag_Test(0x8fd)) {
    Func_02001010();
    Func_02001064();
    Actor_SetPosition(8, PIXELS(424), PIXELS(152));
    wait(1);
    Func_02000f00();
  } else {
    s32 entrance;

    entrance = gMainRoutineWork.entrance;
    if (entrance == 5) {
      Func_020010d4();
      return;
    }
    if (entrance == 6) {
      Func_02001710();
      return;
    }
  }
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->entrance == 3) {
      GameFlag_ClearBit(0xa27);
      GameFlag_SetBit(0xa28);
    }
  }
}
