#include "kibombo.h"
#include "io_reg.h"

void Func_0200538c(void);
void Func_020040cc(void);
void Func_02004294(void);
void Func_02004424(void);
void Func_020047e4(void);
void Func_0200489c(void);
void Func_02004978(void);
void Func_02004a28(void);
void Func_02003dec(void);

void Func_02005594(void) {
  u32 i;

  Func_02005a9c();
  Func_02005c3c(0);
  GameFlag_SetBit(0x869);
  GameFlag_SetBit(0x202);
  gData_02007948[0] = BG_PLTT[9];
  gData_02007948[1] = BG_PLTT[7];
  gData_02007948[2] = BG_PLTT[6];
  gData_02007948[3] = BG_PLTT[76];
  Func_0200538c();
  if (GameFlag_Test(0x8fb)) {
    Func_02003640(1, 1);
    Func_020036e0(1, 4);
  }
  WaitFrames(1);
  switch (gAdventure.entrance) {
  case 10:
    Func_020040cc();
    return;
  case 11:
    Func_02004294();
    return;
  case 12:
    Func_02004424();
    return;
  case 13:
    Func_020047e4();
    return;
  case 14:
    Func_0200489c();
    return;
  case 15:
    Func_02004978();
    return;
  case 16:
    Func_02004a28();
    return;
  }
  Actor_SetSpritePriority(17, 1);
  Actor_SetSpritePriority(7, 2);
  Actor_SetSpritePriority(26, 2);
  Actor_SetSpritePriority(5, 2);
  Actor_SetSpritePriority(6, 2);
  if (GameFlag_Test(0x92e))
    Func_02005b0c(7, 0, 0);
  else if (GameFlag_Test(0x92f)) {
    /* FAKEMATCH: the temporary builds the angle before the position, as the
     * native does (differ 4 without it). */
    s32 angle = 0x2000;

    Kibombo_ObjPlaceFacing(7, 152 << 17, 176 << 15, angle);
  }
  if (GameFlag_Test(0x8fd)) {
    Func_020036e0(1, 6);
    Func_02005b0c(17, 0, 0);
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Task_Add(Func_02002ccc, 0x480);
  for (i = 0; i <= 7; i++) {
    u8 *field = &Actor_GetById(i + 18)->field5c;
    /* FAKEMATCH: the pin keeps the 2 in the loop, built after the field's
     * address as the native does; as a constant loop.c hoists it into r6
     * (differ 10). */
    register s32 value asm("r3") = 2;

    *field = value;
  }
  if (GameFlag_Test(0x8f1)) {
    if (!GameFlag_Test(0x8fd))
      Func_020032b0();
  } else if (gAdventure.entrance == 5) {
    Func_02003dec();
  }
  Func_02005aa4();
  if (gAdventure.entrance == 99) {
    Func_02003640(0, 0);
    Func_020036e0(0, 0);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (gAdventure.entrance == 98) {
    Func_02003640(1, 1);
    Func_020036e0(1, 0);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (gAdventure.entrance == 97) {
    Func_02003640(1, 1);
    Func_020036e0(1, 4);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (gAdventure.entrance == 96) {
    Func_02003640(1, 1);
    Func_020036e0(1, 4);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
    Actor_SetModeById(14, 5);
  }
}
