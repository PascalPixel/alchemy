#include "types.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

s32 Func_020010d0(s32 item);
s32 Func_0200097c(void);
s32 Func_020009a4(void);
s32 Func_020009f8(void);
s32 Func_02000a7c(void);
s32 Func_02000c04(void);

void Func_02000760(void) {
  u8 *message = MsgBeyondTheWeapon;

  FieldEvent_SetMessageId((s32)message);
  Event_ShowMessage(10, 0);
  switch (Func_0200097c()) {
  case 0:
    FieldEvent_SetMessageId((s32)(message + 1));
    Event_ShowMessage(10, 0);
    break;
  case 1:
    FieldEvent_SetMessageId((s32)(message + 2));
    Event_ShowMessage(10, 0);
    break;
  case 2:
    FieldEvent_SetMessageId((s32)(message + 3));
    Event_ShowMessage(10, 0);
    break;
  }
}

void Func_020007b4(void) {
  u8 *message = MsgIFeelA;

  FieldEvent_SetMessageId((s32)message);
  Event_ShowMessage(10, 0);
  switch (Func_020009a4()) {
  case 3:
    FieldEvent_SetMessageId((s32)MsgYouAreLost);
    Event_ShowMessage(10, 0);
    break;
  case 4:
    FieldEvent_SetMessageId((s32)MsgADistantIsle);
    Event_ShowMessage(10, 0);
    break;
  case 5:
    FieldEvent_SetMessageId((s32)MsgISeeA);
    Event_ShowMessage(10, 0);
    break;
  case 6:
    FieldEvent_SetMessageId((s32)MsgATinyFigure);
    Event_ShowMessage(10, 0);
    break;
  case 7:
    FieldEvent_SetMessageId((s32)MsgISeeYou);
    Event_ShowMessage(10, 0);
    break;
  default:
    FieldEvent_SetMessageId((s32)(message + 6));
    Event_ShowMessage(10, 0);
    break;
  }
}

void Func_02000838(void) {
  s32 message = Func_02000c04();

  if (message == (s32)MsgInTheItem) {
    FieldEvent_SetMessageId(message);
    Event_ShowMessage(10, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgInTheItem - 42);
    Event_ShowMessage(10, 0);
    FieldEvent_SetMessageId(message);
    Event_ShowMessage(10, 0);
  }
}

void Func_02000878(void) {
  FieldEvent_SetMessageId((s32)MsgBeyondTheItem);
  Event_ShowMessage(10, 0);
  switch (Func_020009f8()) {
  case 0:
    FieldEvent_SetMessageId((s32)MsgISeeYouInside);
    Event_ShowMessage(10, 0);
    break;
  case 9:
    FieldEvent_SetMessageId((s32)MsgYouMustForge);
    Event_ShowMessage(10, 0);
    break;
  case 10:
    FieldEvent_SetMessageId((s32)MsgYouWillRaise);
    Event_ShowMessage(10, 0);
    break;
  case 11:
    FieldEvent_SetMessageId((s32)MsgYouWillGain);
    Event_ShowMessage(10, 0);
    break;
  case 12:
    FieldEvent_SetMessageId((s32)MsgYouHaveThe);
    Event_ShowMessage(10, 0);
    break;
  case 13:
    FieldEvent_SetMessageId((s32)MsgTheObjectThat);
    Event_ShowMessage(10, 0);
    break;
  }
}

void Func_02000920(void) {
  u8 *message = MsgTheObjectYou;

  FieldEvent_SetMessageId((s32)message);
  Event_ShowMessage(10, 0);
  switch (Func_02000a7c()) {
  case 14:
    FieldEvent_SetMessageId((s32)(message + 1));
    Event_ShowMessage(10, 0);
    break;
  case 15:
    FieldEvent_SetMessageId((s32)(message + 2));
    Event_ShowMessage(10, 0);
    break;
  case 16:
    FieldEvent_SetMessageId((s32)(message + 3));
    Event_ShowMessage(10, 0);
    break;
  case 17:
    FieldEvent_SetMessageId((s32)(message + 4));
    Event_ShowMessage(10, 0);
    break;
  }
}

s32 Func_0200097c(void) {
  if (Event_GetFlag(0x8ff) == 0)
    return 0;
  return (Event_GetFlag(0x940) != 0) + 1;
}

s32 Func_020009a4(void) {
  if (Func_020010d0(144) == -1)
    return 3;
  if (Func_020010d0(139) == -1)
    return 4;
  if (Func_020010d0(138) == -1)
    return 5;
  if (FieldPartyInventory_FindOwner(213) == -1)
    return 6;
  if (FieldPartyInventory_FindOwner(214) == -1)
    return 7;
  return 8;
}

s32 Func_020009f8(void) {
  if (Event_GetFlag(0x8ff) == 0)
    return 0;
  if (FieldPartyInventory_FindOwner(326) == -1) {
    if (Event_GetFlag(0x940) == 0) {
      if (FieldPartyInventory_FindOwner(439) == -1 ||
          FieldPartyInventory_FindOwner(440) == -1 ||
          FieldPartyInventory_FindOwner(441) == -1)
        return 9;
      return (Event_GetFlag(0x905) != 0) + 10;
    }
  } else {
    if (Event_GetFlag(0x940) == 0)
      return 12;
  }
  return 13;
}

s32 Func_02000a7c(void) {
  if (FieldPartyInventory_FindOwner(439) == -1 &&
      (FieldPartyInventory_FindOwner(441) != -1 ||
       FieldPartyInventory_FindOwner(440) != -1))
    return 14;
  if (FieldPartyInventory_FindOwner(439) != -1 &&
      FieldPartyInventory_FindOwner(440) == -1 &&
      FieldPartyInventory_FindOwner(441) == -1)
    return 15;
  if (FieldPartyInventory_FindOwner(439) != -1 &&
      FieldPartyInventory_FindOwner(440) == -1 &&
      FieldPartyInventory_FindOwner(441) != -1)
    return 15;
  if (FieldPartyInventory_FindOwner(439) != -1 &&
      FieldPartyInventory_FindOwner(440) != -1 &&
      FieldPartyInventory_FindOwner(441) == -1)
    return 16;
  if (FieldPartyInventory_FindOwner(439) != -1 &&
      FieldPartyInventory_FindOwner(440) != -1 &&
      FieldPartyInventory_FindOwner(441) != -1)
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
    if (Event_GetFlag(flag))
      return i + 1;
  }
next:
  if (--i > 6)
    goto loop;
  return -1;
}
