#include "types.h"
#include "adventure.h"
#include "scene_call.h"
#include "text/msg_ids.h"

void Func_02001138(s32 a0);
void Func_02001148(s32 a0, s32 a1);
s32 Func_020010d0(s32 item);
s32 PartyInventory_FindOwner(s32 item);
s32 Func_0200097c(void);
s32 Func_020009a4(void);
s32 Func_020009f8(void);
s32 Func_02000a7c(void);
s32 Func_02000c04(void);

void Func_02000760(void) {
  u8 *message = MsgBeyondTheWeapon;

  Func_02001138((s32)message);
  Func_02001148(10, 0);
  switch (Func_0200097c()) {
  case 0:
    Func_02001138((s32)(message + 1));
    Func_02001148(10, 0);
    break;
  case 1:
    Func_02001138((s32)(message + 2));
    Func_02001148(10, 0);
    break;
  case 2:
    Func_02001138((s32)(message + 3));
    Func_02001148(10, 0);
    break;
  }
}

void Func_020007b4(void) {
  u8 *message = MsgIFeelA;

  Func_02001138((s32)message);
  Func_02001148(10, 0);
  switch (Func_020009a4()) {
  case 3:
    Func_02001138((s32)MsgYouAreLost);
    Func_02001148(10, 0);
    break;
  case 4:
    Func_02001138((s32)MsgADistantIsle);
    Func_02001148(10, 0);
    break;
  case 5:
    Func_02001138((s32)MsgISeeA);
    Func_02001148(10, 0);
    break;
  case 6:
    Func_02001138((s32)MsgATinyFigure);
    Func_02001148(10, 0);
    break;
  case 7:
    Func_02001138((s32)MsgISeeYou);
    Func_02001148(10, 0);
    break;
  default:
    Func_02001138((s32)(message + 6));
    Func_02001148(10, 0);
    break;
  }
}

void Func_02000838(void) {
  s32 message = Func_02000c04();

  if (message == (s32)MsgInTheItem) {
    Func_02001138(message);
    Func_02001148(10, 0);
  } else {
    Func_02001138((s32)MsgInTheItem - 42);
    Func_02001148(10, 0);
    Func_02001138(message);
    Func_02001148(10, 0);
  }
}

void Func_02000878(void) {
  Func_02001138((s32)MsgBeyondTheItem);
  Func_02001148(10, 0);
  switch (Func_020009f8()) {
  case 0:
    Func_02001138((s32)MsgISeeYouInside);
    Func_02001148(10, 0);
    break;
  case 9:
    Func_02001138((s32)MsgYouMustForge);
    Func_02001148(10, 0);
    break;
  case 10:
    Func_02001138((s32)MsgYouWillRaise);
    Func_02001148(10, 0);
    break;
  case 11:
    Func_02001138((s32)MsgYouWillGain);
    Func_02001148(10, 0);
    break;
  case 12:
    Func_02001138((s32)MsgYouHaveThe);
    Func_02001148(10, 0);
    break;
  case 13:
    Func_02001138((s32)MsgTheObjectThat);
    Func_02001148(10, 0);
    break;
  }
}

void Func_02000920(void) {
  u8 *message = MsgTheObjectYou;

  Func_02001138((s32)message);
  Func_02001148(10, 0);
  switch (Func_02000a7c()) {
  case 14:
    Func_02001138((s32)(message + 1));
    Func_02001148(10, 0);
    break;
  case 15:
    Func_02001138((s32)(message + 2));
    Func_02001148(10, 0);
    break;
  case 16:
    Func_02001138((s32)(message + 3));
    Func_02001148(10, 0);
    break;
  case 17:
    Func_02001138((s32)(message + 4));
    Func_02001148(10, 0);
    break;
  }
}

s32 Func_0200097c(void) {
  if (GameFlag_Test(0x8ff) == 0)
    return 0;
  return (GameFlag_Test(0x940) != 0) + 1;
}

s32 Func_020009a4(void) {
  if (Func_020010d0(144) == -1)
    return 3;
  if (Func_020010d0(139) == -1)
    return 4;
  if (Func_020010d0(138) == -1)
    return 5;
  if (PartyInventory_FindOwner(213) == -1)
    return 6;
  if (PartyInventory_FindOwner(214) == -1)
    return 7;
  return 8;
}

s32 Func_020009f8(void) {
  if (GameFlag_Test(0x8ff) == 0)
    return 0;
  if (PartyInventory_FindOwner(326) == -1) {
    if (GameFlag_Test(0x940) == 0) {
      if (PartyInventory_FindOwner(439) == -1 ||
          PartyInventory_FindOwner(440) == -1 ||
          PartyInventory_FindOwner(441) == -1)
        return 9;
      return (GameFlag_Test(0x905) != 0) + 10;
    }
  } else {
    if (GameFlag_Test(0x940) == 0)
      return 12;
  }
  return 13;
}

s32 Func_02000a7c(void) {
  if (PartyInventory_FindOwner(439) == -1 &&
      (PartyInventory_FindOwner(441) != -1 ||
       PartyInventory_FindOwner(440) != -1))
    return 14;
  if (PartyInventory_FindOwner(439) != -1 &&
      PartyInventory_FindOwner(440) == -1 &&
      PartyInventory_FindOwner(441) == -1)
    return 15;
  if (PartyInventory_FindOwner(439) != -1 &&
      PartyInventory_FindOwner(440) == -1 &&
      PartyInventory_FindOwner(441) != -1)
    return 15;
  if (PartyInventory_FindOwner(439) != -1 &&
      PartyInventory_FindOwner(440) != -1 &&
      PartyInventory_FindOwner(441) == -1)
    return 16;
  if (PartyInventory_FindOwner(439) != -1 &&
      PartyInventory_FindOwner(440) != -1 &&
      PartyInventory_FindOwner(441) != -1)
    return 17;
  // No remaining combinations: the native falls off the end without a value.
}

s32 Func_02000b60(s32 group) {
  s32 i = 17;

  /* FAKEMATCH: taking group's address keeps GCSE from hoisting group * 20 out
   * of the loop, which the native rebuilds on every pass (differ 10 without
   * it). */
  (void)&group;
loop:
  {
    s32 flag = group * 20 + i + 48;

    if (group == 0) {
      if (i == 7)
        goto next;
    }
    if (group == 1) {
      if (i == 9)
        goto next;
      if (i == 10)
        goto next;
    }
    if (GameFlag_Test(flag))
      return i + 1;
  }
next:
  if (--i > 6)
    goto loop;
  return -1;
}
