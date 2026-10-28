#include "kibombo.h"

struct KibomboEffectWork {
  u8 unk000[0x7c4];
  s16 field7c4;
};

void Func_020037e4(void) {
  struct KibomboEffectWork *work = Ram_HeapSlots->effectWork;

  /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the work
   * pointer before it builds the field's offset (2 differ without it). */
  do {
  } while (0);
  if (work->field7c4 != 0) {
    if (GameFlag_Test(0x220))
      Func_02000624();
    else if (GameFlag_Test(0x221))
      Func_02000668();
    else if (GameFlag_Test(0x222))
      Func_020006ac();
    else if (GameFlag_Test(0x223))
      Func_020006f0();
  }
}

void Func_0200384c(void) {
  Func_02005c14(136, 1);
  Task_Add(Func_020037e4, 0x480);
  Func_02005c1c(gAdventure.currentOwner, -1);
  Func_02005c34();
  Field_DispatchTypeHandler(1);
  Func_02005c24();
  Task_RemoveFar(Func_020037e4);
  Func_02005c2c();
}

void Func_02003898(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Kibombo4)
    Task_RemoveFar(Func_02002c34);
  else if (scene == (s32)&SceneId_Kibombo5)
    Task_RemoveFar(Func_02002ccc);
  else if (scene == (s32)&SceneId_Kibombo6)
    Task_RemoveFar(Func_02002d64);
}

void Func_020038ec(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;

  Func_02005a9c();
  Func_02005c3c(0);
  if (GameFlag_Test(0x202))
    Func_02003898();
  Func_02005c9c(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005bc4(work->exit);
}

void Func_02003930(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x209;
  Func_020038ec();
}

void Func_02003948(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;
  struct KibomboObject *leader;
  s32 x;
  s32 px;

  /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the party
   * state's address before it builds the owner's offset (2 differ without it).
   */
  do {
  } while (0);
  leader = Actor_GetById(gAdventure.currentOwner);
  Func_02005a9c();
  Func_02005c3c(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  Actor_SetModeById(gAdventure.currentOwner, 2);
  Kibombo_ObjSpeed(gAdventure.currentOwner, 0x8000, 0x4000);
  x = leader->x;
  px = x >> 16;
  Func_02005a54(leader, ((8 - px % 16) << 16) + x, leader->y, leader->z);
  Func_02005a5c(leader);
  leader->facing = 0xc000;
  if (GameFlag_Test(0x202))
    Func_02003898();
  Actor_SetMovementTypeById(gAdventure.currentOwner,
                                          gData_02005cd0);
  wait(12);
  Func_02005c9c(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005bc4(work->exit);
}
