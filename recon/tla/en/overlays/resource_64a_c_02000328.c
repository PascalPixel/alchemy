// Draft, differ 696 of 756 (tla-en; the other editions differ in their
// messages and layout): the game clear menu's flow: the ending choice, the
// save slot actions, the password and link screens around the cell buffer
// swap, and starting the next game. Left: register choice and block order;
// the message numbers are written as numbers.
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "iwram_call.h"
#include "frame.h"

extern s16 gData_020024f6;
extern s16 gData_020024f8;
extern s16 gData_02003860;
extern u32 gData_02001000;
extern u8 gData_02000000[];
extern u8 gMapCellBuffer[];
extern u8 Data_02012f94[];
extern u32 gInput[];

void Func_0200005c(void);
void Func_0200015c(void);
void Func_020001f4(void);
s32 Func_020002e0(void);
s32 Func_02000b24(void);
s32 Func_02000e44(void);
s32 Func_020022b0(s32 mode);
void Func_020022b8(s32 a, s32 b, s32 c);
s32 Func_020022d0(void);
s32 Func_020022d8(s32 mode);
void Func_020022e0(void);
void Func_020022e8(void);
s32 Func_020022f0(void);
s32 Func_020022f8(s32 step);
void Func_02002310(s32 hue, s32 brightness);
void Func_02002350(void);
void Func_02002370(void *data, s32 a, s32 b);
u32 Func_02002388(void);
void Func_02002390(void);
void Func_02002398(void);
void Func_020023a0(void *data);
void Func_020023b8(s32 a, s32 b);
s32 Func_020023d8(s32 mode);
void Func_020023e0(s32 scene, s32 entrance);
void Func_020023e8(void);
void Func_02002408(s32 cue);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void FieldEvent_WaitTicks(s32 ticks);
void WaitTicks(s32 ticks);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void UiWork_FinalizePendingCore(void);
s32 Menu_RunConfirmSelection(s32 a, s32 b, s32 c, s32 d);
s32 Menu_SelectSaveSlotAction(void);
void Party_AddActiveOwner(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);

#define PARTY_BYTE(offset) (*((u8 *)&gPartyState + (offset)))
#define PARTY_WORD(offset) (*(u32 *)((u8 *)&gPartyState + (offset)))

/// Swaps the map cell buffer out around a password or link screen.
#define SAVE_CELLS() Iwram_CopyWords(gMapCellBuffer, gData_02000000, 0x3000)
#define RESTORE_CELLS()                                                        \
  {                                                                            \
    Iwram_CopyWords(Data_02012f94 + 0x6c, gData_02000000, 0x3000);             \
    Iwram_CopyWords(gData_02000000, gMapCellBuffer, 0x3000);                   \
  }

/// Goes back to the saved scene and entrance.
#define RETURN_TO_SAVED()                                                      \
  {                                                                            \
    gPartyState.scene = gPartyState.savedScene;                                \
    gPartyState.entrance = gPartyState.savedEntrance;                          \
    Func_020023e0(gPartyState.scene, gPartyState.entrance);                    \
    GameFlag_ClearBit(0x109);                                                  \
  }

s32 Func_02000328(void) {
  s32 started = 0;
  s32 result, choice, i, limit, status;

  Func_0200005c();
  Scheduler_AddOrUpdateCallback(Func_020001f4, 0x480);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = started;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  if (gPartyState.entrance == 2) {
    do {
      UiText_ShowPositionedMessageAndWait(8, 5);
      choice = Func_020023d8(1);
      UiWork_FinalizePendingCore();
      if (choice != 0)
        break;
      UiText_ShowPositionedMessageAndWait(9, 1);
      PARTY_BYTE(0x22f) = 1;
      Party_AddActiveOwner(5);
      Party_AddActiveOwner(6);
      Party_AddActiveOwner(7);
      Party_AddActiveOwner(1);
      Party_AddActiveOwner(2);
      Party_AddActiveOwner(3);
    } while (Func_020022d0() == -1);
    FieldEvent_WaitTicks(60);
    Event_FinishScreenTransition();
    Func_02002408(78);
    FieldEvent_WaitTicks(300);
    Func_020023b8(0, 1);
    return 0;
  }
  gSleepDisabled = started;
top:
  result = Func_020022f0();
  if (result < 0 && gDebugMode) {
    PARTY_BYTE(0x24a) = 1;
    gAutoSleepEnabled = 1;
    Func_020022b8(11, 1, 8);
  }
  if (result == 0 && !started) {
    WaitTicks(30);
    Scheduler_AddOrUpdateCallback(Func_0200015c, 0x480);
    WaitTicks(1);
    started = 1;
  }
  if (result > 0)
    result = Menu_SelectSaveSlotAction();
  else
    result = 0;
  if (result == 0) {
    Func_02002350();
    Func_02002310(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
    i = 4;
    limit = 5;
  open:
    WaitTicks(6);
    if (Func_020022f8(i) == -1) {
      if (i == 4)
        goto top;
      i--;
      goto open;
    }
    if (gData_020024f6 != 0 || gData_020024f8 != 0)
      limit = 8;
    if (++i < limit)
      goto open;
  ask:
    UiText_ShowPositionedMessageAndWait(0x24, 13);
    choice = Menu_RunConfirmSelection(1, 0, 0, 0);
    UiWork_FinalizePendingCore();
    if (choice == 0) {
    enter:
      result = Func_020022b0(0);
      if (result == -1)
        goto top;
      if (result == 1) {
        SAVE_CELLS();
        status = Func_02000b24();
        if (status != -1)
          Func_020023e8();
        RESTORE_CELLS()
        if (status == -1)
          goto enter;
        Func_02002370(Data_02012f94 + 0x6c, 0, 0);
      } else if (result == 0) {
        SAVE_CELLS();
        Func_02002350();
        status = Func_02000e44();
        RESTORE_CELLS()
        if (status == -1)
          goto enter;
        Func_02002370(Data_02012f94 + 0x6c, 1, 0);
      }
    } else {
      Func_02002398();
    }
    i = 0;
    limit = 1;
  close:
    WaitTicks(6);
    if (Func_020022f8(i) == -1) {
      if (i == 0)
        goto ask;
      i--;
      goto close;
    }
    if (gData_020024f8 != 0)
      limit = 4;
    if (++i < limit)
      goto close;
    if (gData_02003860 != 0) {
    save:
      UiText_ShowPositionedMessageAndWait(0x25, 13);
      choice = Menu_RunConfirmSelection(1, 0x29, 8, 0);
      UiWork_FinalizePendingCore();
      if (choice == 0) {
        if (gData_02003860 == 0) {
          UiText_ShowPositionedMessageAndWait(0x26, 1);
        } else {
          SAVE_CELLS();
          if (Func_020022d8(5) != 0)
            goto save;
          Func_02002310(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
          RESTORE_CELLS()
          Func_020023a0(Data_02012f94 + 0x6c);
          PARTY_BYTE(0x287) = 1;
          goto leave;
        }
      }
      UiText_ShowPositionedMessageAndWait(0x27, 13);
      choice = Menu_RunConfirmSelection(1, 0x29, 8, 0);
      UiWork_FinalizePendingCore();
      if (choice == 0) {
        GameFlag_SetBit(46);
        PARTY_BYTE(0x287) = 2;
        goto leave;
      }
      UiText_ShowPositionedMessageAndWait(0x28, 13);
      choice = Menu_RunConfirmSelection(1, 0x29, 8, 0);
      UiWork_FinalizePendingCore();
      if (choice != 0)
        goto save;
    }
  leave:
    Func_020023b8(0, 8);
    goto finish;
  }
  if (result == 1) {
    s32 message;

    if (Func_020022d8(1) == -1)
      goto top;
    GameFlag_SetBit(0x109);
    Func_02002310(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
    if (PARTY_WORD(0) != Func_02002388()) {
      RETURN_TO_SAVED()
    } else if ((gInput[0] & 0x208) == 0x208) {
      if (Func_020002e0()) {
        message = 7;
        goto refuse;
      }
      RETURN_TO_SAVED()
      GameFlag_SetBit(0x13e);
    } else if (PARTY_WORD(4) != gData_02001000) {
      message = 5;
      UiText_ShowPositionedMessageAndWait(message, 9);
      UiText_ShowPositionedMessageAndWait(message + 1, 13);
      if (Menu_RunConfirmSelection(1, 0, 0, 0) != 0) {
        UiWork_FinalizePendingCore();
        goto top;
      }
      UiWork_FinalizePendingCore();
      if (Func_020002e0()) {
        message += 2;
      refuse:
        UiText_ShowPositionedMessageAndWait(message, 9);
        goto top;
      }
      RETURN_TO_SAVED()
      GameFlag_SetBit(0x13f);
    }
    GameFlag_ClearBit(0x106);
    goto finish;
  }
  if (result == 2) {
    Func_020022e0();
    goto top;
  }
  if (result == 3) {
    Func_020022e8();
    goto top;
  }
  if (result == 4) {
    if (Func_020022d8(4) == -1)
      goto top;
    gPartyState.currentOwner = 4;
    if (GameFlag_Test(0x9ff))
      Func_02002390();
    Func_02002310(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
    GameFlag_ClearBit(0x109);
    GameFlag_ClearBit(0x106);
    gPartyState.ownerOverride = 0;
    gSleepDisabled = 1;
    Func_020023b8(0x138, 1);
    goto finish;
  }
  if (result != 5)
    goto top;
link:
  if (Func_020022d8(6) == -1)
    goto top;
  Func_02002310(gPartyState.paletteGlow[0], gPartyState.paletteGlow[1]);
again:
  result = Func_020022b0(2);
  if (result == -1)
    goto link;
  if (result == 1) {
    SAVE_CELLS();
    status = Func_02000b24();
    if (status != -1)
      Func_020023e8();
    RESTORE_CELLS()
    if (status == -1)
      goto again;
    Func_02002370(Data_02012f94 + 0x6c, 0, 1);
  } else if (result == 0) {
    SAVE_CELLS();
    Func_02002350();
    status = Func_02000e44();
    RESTORE_CELLS()
    if (status == -1)
      goto again;
    Func_02002370(Data_02012f94 + 0x6c, 1, 1);
  }
  if (gData_020024f6 != 0 || gData_020024f8 != 0) {
    i = 0;
    limit = 1;
  shut:
    WaitTicks(6);
    if (Func_020022f8(i) == -1) {
      if (i == 0)
        goto link;
      i--;
      goto shut;
    }
    if (gData_020024f8 != 0)
      limit = 4;
    if (++i < limit)
      goto shut;
  }
  GameFlag_SetBit(0x109);
finish:
  *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x158) = 999;
  FieldEvent_WaitTicks(30);
  Func_02002408(78);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_WaitTicks(60);
  return 0;
}
