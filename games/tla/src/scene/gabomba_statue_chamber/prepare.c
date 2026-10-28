#include "types.h"
#include "scene_tables.h"
#include "gabomba_statue_chamber.h"

s32 Func_02001f00(void);
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
  struct MainRoutineWork *party;
  s16 scene;

  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 521;
  Actor_GetById(Func_02001f00())->priorityFlags |= 0x20;
  party = &gMainRoutineWork;
  /* FAKEMATCH: ⚓️ loads gMainRoutineWork's address before it builds the scene's
   * offset; read as a plain field the offset's first half is scheduled ahead of
   * the load (differ 2). */
  asm volatile("" : "+l"(party));
  scene = party->scene;
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
    GabombaStatueChamber_ObjPosSet(8, 0x1a80000, 0x980000);
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
    /* FAKEMATCH(pin): a hard register is outside global CSE, so the party
     * state's address is loaded again here, as the native does; as a plain
     * read of gMainRoutineWork.entrance, 30 of 58 instructions differ in tla-en. */
    register struct MainRoutineWork *party asm("r3") = &gMainRoutineWork;

    if (party->entrance == 3) {
      GameFlag_ClearBit(0xa27);
      GameFlag_SetBit(0xa28);
    }
  }
}
