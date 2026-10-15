#include "local.h"
#include "event_work.h"

void Func_020009c0(s32 id);
void Func_02000a94(s32 id);

/* FAKEMATCH: the callers set up the arguments as for a call whose value is used
 * (flag before index); the setter is declared void for the modules that match
 * with that. */
#define Vent_SetActive(index, active)                                          \
  ((s32 (*)(s32, s32))Func_0200271c)(index, active)

void Func_02001304(void) {
  s32 timer = Func_02002730();

  switch (timer) {
  case 0:
    Vent_SetActive(0, 1);
    Vent_SetActive(1, 1);
    Vent_SetActive(4, 1);
    Vent_SetActive(5, 1);
    Func_02002978(170);
    break;
  case 180:
    Vent_SetActive(0, 0);
    Vent_SetActive(1, 0);
    Vent_SetActive(4, 0);
    Vent_SetActive(5, 0);
    Func_02002978(-1);
    break;
  case 240:
    Vent_SetActive(2, 1);
    Vent_SetActive(3, 1);
    Vent_SetActive(6, 1);
    Vent_SetActive(7, 1);
    Vent_SetActive(8, 1);
    Vent_SetActive(9, 1);
    Func_02002978(170);
    break;
  case 420:
    Vent_SetActive(2, 0);
    Vent_SetActive(3, 0);
    Vent_SetActive(6, 0);
    Vent_SetActive(7, 0);
    Vent_SetActive(8, 0);
    Vent_SetActive(9, 0);
    Func_02002978(-1);
    break;
  }
  if ((u32)(timer - 251) <= 178 && GameFlag_Test(0xa6e) && !GameFlag_Test(59) &&
      !GameFlag_Test(0x310))
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a = 100;
  Func_0200273c(480);
}

void Func_02001424(void) {
  s32 timer = Func_02002730();

  switch (timer) {
  case 0:
    Vent_SetActive(0, 1);
    Vent_SetActive(1, 1);
    Func_02002978(170);
    break;
  case 120:
    Vent_SetActive(0, 0);
    Vent_SetActive(1, 0);
    Func_02002978(-1);
    break;
  }
  if ((u32)(timer - 11) <= 118) {
    if (GameFlag_Test(0x221))
      Func_02000810(8);
    else if (GameFlag_Test(0x220))
      Func_020008d8(8);
  }
  Func_0200273c(480);
}

void Func_020014a0(void) {
  s32 timer = Func_02002730();

  switch (timer) {
  case 0:
    Vent_SetActive(0, 1);
    Vent_SetActive(1, 1);
    Func_02002978(170);
    break;
  case 120:
    Vent_SetActive(0, 0);
    Vent_SetActive(1, 0);
    Func_02002978(-1);
    break;
  }
  if ((u32)(timer - 11) <= 118) {
    if (GameFlag_Test(0x221))
      Func_020009c0(10);
    else if (GameFlag_Test(0x220))
      Func_02000a94(10);
  }
  Func_0200273c(480);
}

void Func_0200151c(void) {
  switch (Func_02002730()) {
  case 0:
    Vent_SetActive(0, 1);
    Vent_SetActive(1, 1);
    Func_02002978(170);
    break;
  case 180:
    Vent_SetActive(0, 0);
    Vent_SetActive(1, 0);
    Func_02002978(-1);
    break;
  }
  Func_0200273c(240);
}
