// Near miss, differ 2 of 288 (tla-en, 2026-10-30): in the rising loop the
// native loads the scale step (ldr r2, -0x2000) before object->scaleY, so that load
// waits a cycle after the store to gData_02007950->y; alchemy-gcc's
// -mthumb-slow-loads exempts it (the load opens a read-modify-write), and
// sched2 then ranks the data-dependent load first. A scheduler cost-model
// difference; no source form tried moves it.
// Reading the fields through struct KibomboObject rather than byte offsets
// gives its loads and stores one alias set, which settled every other
// difference and cleared the five empty asms (was differ 8); the scene's
// inline wrappers keep the call constants unshared. alchemy permute (4 minutes,
// 8,024 candidates, 30 October) found nothing better.
// 26 October 2026: an empty asm or one-pass block after any one statement
// and alchemy permute (5 to 6 minutes) did not move this load.
#include "../../../../games/tla/src/scene/kibombo/kibombo.h"

void Func_02004a28(void) {
  struct KibomboObject *object;
  u32 i;

  object = Actor_GetById(14);
  Func_02004524();
  Actor_SetSpritePriority(17, 1);
  Kibombo_SePlay(0x240);
  Actor_SetPartAttribute(Actor_GetById(12), 7);
  Actor_SetPartAttribute(Actor_GetById(13), 7);
  wait(8);
  Actor_SetPartAttribute(Actor_GetById(12), 0);
  Actor_SetPartAttribute(Actor_GetById(13), 0);
  wait(8);
  Func_02005b94(17, 0x100, 20);
  Func_02005b4c((s32)MsgLookTheGreatGabomba);
  Func_02005b5c(17, 0, 20);
  Kibombo_SePlay(0x240);
  Func_020036e0(1, 0);
  FieldEvent_Wait(20);
  Func_02005c9c(233);
  object->scaleY = 0x1999;
  Actor_SetPositionAndResetMotion(Actor_GetById(14), 0x1080000, 0x180000,
                                  0x1340000);
  wait(1);
  Actor_SetModeById(14, 3);
  for (i = 0; i < 64; i++) {
    object->scaleY += 0x400;
    object->y += 0x2000;
    wait(1);
  }
  Func_02005c9c(0x12a);
  if (gData_02007950 != 0) {
    gData_02007950->motionFlags = 0;
    // As a while loop, which moves the global's address out of the loop, 24
    // of 288 instructions differ in tla-en.
    if (gData_02007950->y > 0x240000) {
    lower:
      gData_02007950->y -= 0x8000;
      wait(1);
      if (gData_02007950->y > 0x240000)
        goto lower;
    }
  }
  Func_02005c9c(232);
  if (gData_02007950 != 0) {
    gData_02007950->motionFlags = 0;
    // As a for loop, which moves the global's address out of the loop, 25 of
    // 288 instructions differ in tla-en.
    i = 0;
  rise:
    gData_02007950->y += 0x60000;
    object->scaleY -= 0x2000;
    object->y -= 0x8000;
    wait(1);
    i++;
    if (i < 4)
      goto rise;
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
    for (i = 0; i < 4; i++) {
      object->scaleY -= 0x2000;
      object->y -= 0x8000;
      wait(1);
    }
  }
  Func_02005b0c(14, 0, 0);
  Func_020036e0(0, 0);
  Func_02003640(1, 1);
  FieldEvent_Wait(10);
  Func_02003640(0, 1);
  FieldEvent_Wait(40);
  Kibombo_SePlay(0x240);
  Func_02003640(1, 1);
  Func_020036e0(1, 0);
  Func_02005c9c(132);
  Actor_SetPositionAndResetMotion(Actor_GetById(14), 0x1080000, 0x180000,
                                  0x1340000);
  wait(1);
  for (i = 0; i < 8; i++) {
    object->scaleY += 0x2000;
    object->y += 0x8000;
    wait(1);
  }
  Func_02005b9c(17, 0x102);
  FieldEvent_Wait(40);
  Kibombo_ObjTurn(17, 0x3000);
  Kibombo_MessagePut(0x4011, 0);
  Kibombo_ObjEmote(17, 0x108, 20);
  Kibombo_MessagePut(0x4011, 0);
  Motion_SetModeAndWaitAnimation(17, 3);
  Kibombo_MessagePut(0x4011, 0);
  Kibombo_ObjSpeed(17, 0xcccc, 0x6666);
  Kibombo_ObjWalk(17, 0x108, 0x140);
  Func_02005b0c(17, 0, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x203;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_SetBit(0x8fb);
  Func_02005bc4(13);
}
