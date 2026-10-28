#include "kibombo.h"
#include "scene_tables.h"

void Func_02005a4c(void);
void Func_02005be4(s32 frames);

void Func_02003c84(void) {
  Func_02005c9c(212);
  do {
    /* FAKEMATCH: the one-pass block ends CSE's block, so each fade builds
     * 0x7fff afresh; without the three blocks, 68 instructions differ in
     * tla-en. */
    Kibombo_FadeTo(0x7fff, 0);
    Func_02005be4(1);
  } while (0);
  wait(1);
  Kibombo_FadeTo(0x10000, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Kibombo_TaskAdd(Func_02002d64, 0x480);
  wait(60);
  Task_RemoveFar(Func_02002d64);
  wait(1);

  Func_02005c9c(212);
  do {
    /* FAKEMATCH: as above. */
    Kibombo_FadeTo(0x7fff, 0);
    Func_02005be4(1);
  } while (0);
  wait(1);
  Kibombo_CamMove(146 << 18, -1, PIXELS(248), 0);
  Func_02005a4c();
  wait(1);
  Kibombo_FadeTo(0x10000, 0);
  Func_02005be4(1);
  wait(1);
  Kibombo_TaskAdd(Func_02002d64, 0x480);
  wait(60);
  Task_RemoveFar(Func_02002d64);
  wait(1);

  Func_02005c9c(212);
  do {
    /* FAKEMATCH: as above. */
    Kibombo_FadeTo(0x7fff, 0);
    Func_02005be4(1);
  } while (0);
  wait(1);
  Kibombo_CamMove(PIXELS(248), -1, 134 << 18, 0);
  Func_02005a4c();
  wait(1);
  Kibombo_FadeTo(0x10000, 0);
  Func_02005be4(1);
  wait(1);
  Kibombo_TaskAdd(Func_02002d64, 0x480);
  wait(180);
  Task_RemoveFar(Func_02002d64);
  wait(1);

  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 1;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Kibombo_Wait(10);
  Func_02005bc4(10);
}
