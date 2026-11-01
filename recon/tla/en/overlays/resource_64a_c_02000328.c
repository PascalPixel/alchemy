// Draft, differ 116 of 756, skeleton gap 2 (tla-en, 2026-11-01): the title's
// file flow: the ending's save question, the file actions, transferring data
// from Golden Sun (password or link) around the cell buffer swap, the
// difficulty questions and starting the game. Every instruction is there but
// one pair: in the save path's last cell copy the native loads the buffer's
// address before the size's shift (one swap). The other 114 are scratch
// registers (r1, r2, r3) on the constants and addresses in the palette reads,
// the zero index of ldrsh and -1. They start at the first palette call, where
// the native holds the address in r3 and this holds it in r2; parts of the
// pattern look like reload's register rotation. Tried without effect there:
// a party local, hue and brightness locals, unk220[5], byte offsets and three
// prototypes for Func_02002310; Call3 for one copy, in place of Value3.
//
// Forms the native forced: every loop is built from labels (loop.c otherwise
// hoists its addresses); the buffers are constants of each call
// (Ram_MapCellBackup, and Ram_MapCellBuffer: as symbols their pool loads are
// shared and five registers become seven); cells and party are one local per
// site; the copies and flags use call.h's Value3 and flag_call.h's wrappers.
// i also holds the answers and recovery the first recovery message, for r5
// and r6. To adopt: Ram_MapCellBackup (Data_02012f94 + 0x6c, 0x02013000 in
// every edition) needs a linked name, the eight Msg names need their PO
// contexts (messages 5, 7, 8 and 36 to 41 are plain numbers there), and
// SceneId_Lobby needs a name for row 312 of scenes.s (0x6b5, the lobby).
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "frame.h"
#include "scene_ids.h"
#include "call.h"
#include "flag_call.h"
#include "field/event_work.h"

extern s16 gData_020024f6;
extern s16 gData_020024f8;
extern s16 gData_02003860;
extern u32 gData_02001000;
extern u8 gData_02000000[];
extern u32 gInput[];
extern char MsgSomeDataIsCorrupted[];
extern char MsgDataCouldNotBeRecovered[];
extern char MsgSaveYourAdventure[];
extern char MsgTransferDataFromGoldenSun[];
extern char MsgPlayOnEasyLevel[];
extern char MsgPlayOnHardLevel[];
extern char MsgPlayOnNormalLevel[];
extern char MsgYeah[];
extern char SceneId_Lobby;

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
s32 Task_Add(void (*func)(void), s32 priority);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
void FieldMessage_Show(s32 message, s32 position);
void Message_CloseFinishedWindows(void);
s32 Menu_RunConfirmSelection(s32 a, s32 b, s32 c, s32 d);
s32 Menu_SelectSaveSlotAction(void);
void Party_AddActiveOwner(s32 id);

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
#define PARTY_WORD(party, index) FIELD_AT(party, u32, 4 * (index))

#define Ram_MapCellBackup FIXED_ADDRESS((u8 *), 0x02013000, gMapCellBackup)

/// Swaps the map cell buffer out around a password or link screen.
#define COPY_CELLS(to, from)                                                   \
  Value3((s32 (*)())Iwram_CopyWords, (s32)(to), (s32)(from), 0x3000)
#define SAVE_CELLS()                                                           \
  u8 *cells = gData_02000000;                                                  \
                                                                               \
  COPY_CELLS(Ram_MapCellBuffer, cells)
#define RESTORE_CELLS()                                                        \
  {                                                                            \
    COPY_CELLS(Ram_MapCellBackup, cells);                                      \
    COPY_CELLS(cells, Ram_MapCellBuffer);                                      \
  }

/// Goes back to the saved scene and entrance.
#define RETURN_TO_SAVED(party)                                                 \
  {                                                                            \
    (party)->scene = (party)->savedScene;                                      \
    (party)->entrance = (party)->savedEntrance;                                \
    Func_020023e0((party)->scene, (party)->entrance);                          \
    Flag_Clear(0x109);                                                         \
  }

s32 Func_02000328(void) {
  s32 started = 0;
  u32 mask;
  s32 result, i, limit, status, message, failure, recovery;

  Func_0200005c();
  Task_Add(Func_020001f4, 0x480);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = started;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  if (gMainRoutineWork.entrance == 2) {
  ask_save:
    message = (s32)MsgSaveYourAdventure;
    FieldMessage_Show(message, 5);
    i = Func_020023d8(1);
    Message_CloseFinishedWindows();
    if (i == 0) {
      FieldMessage_Show(message + 1, 1);
      gMainRoutineWork.unk22d[2] = 1;
      Party_AddActiveOwner(5);
      Party_AddActiveOwner(6);
      Party_AddActiveOwner(7);
      Party_AddActiveOwner(1);
      Party_AddActiveOwner(2);
      Party_AddActiveOwner(3);
      i = Func_020022d0();
      if (i == -1)
        goto ask_save;
    }
    FieldEvent_Wait(60);
    Event_FinishScreenTransition();
    Func_02002408(78);
    FieldEvent_Wait(300);
    Func_020023b8((s32)&SceneId_Title, 1);
    return 0;
  }
  gSleepDisabled = started;
top:
  result = Func_020022f0();
  if (result < 0 && gDebugMode) {
    gMainRoutineWork.unk244[6] = 1;
    gAutoSleepEnabled = 1;
    Func_020022b8((s32)&SceneId_DailaInteriors1, 1, 8);
  }
  if (result == 0 && !started) {
    wait(30);
    Task_Add(Func_0200015c, 0x480);
    wait(1);
    started = 1;
  }
  if (result > 0)
    result = Menu_SelectSaveSlotAction();
  else
    result = 0;
  if (result == 0) {
    Func_02002350();
    Func_02002310(gMainRoutineWork.paletteGlow[0],
                  gMainRoutineWork.paletteGlow[1]);
    i = 4;
    limit = 5;
  open:
    wait(6);
    result = Func_020022f8(i);
    if (result == -1) {
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
    FieldMessage_Show((s32)MsgTransferDataFromGoldenSun, 13);
    result = Menu_RunConfirmSelection(1, 0, 0, 0);
    Message_CloseFinishedWindows();
    if (result == 0) {
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
        Func_02002370(Ram_MapCellBackup, 0, 0);
      } else if (result == 0) {
        SAVE_CELLS();
        Func_02002350();
        status = Func_02000e44();
        RESTORE_CELLS()
        if (status == -1)
          goto enter;
        Func_02002370(Ram_MapCellBackup, 1, 0);
      }
    } else {
      Func_02002398();
    }
    i = 0;
    limit = 1;
  close:
    wait(6);
    result = Func_020022f8(i);
    if (result == -1) {
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
      message = (s32)MsgPlayOnEasyLevel;
      FieldMessage_Show(message, 13);
      i = Menu_RunConfirmSelection(1, (s32)MsgYeah, 8, 0);
      Message_CloseFinishedWindows();
      if (i == 0) {
        if (gData_02003860 == 0) {
          FieldMessage_Show(message + 1, 1);
        } else {
          SAVE_CELLS();
          result = Func_020022d8(5);
          if (result != 0)
            goto save;
          {
            struct MainRoutineWork *party = &gMainRoutineWork;

            Func_02002310(party->paletteGlow[0], party->paletteGlow[1]);
            RESTORE_CELLS()
            Func_020023a0(Ram_MapCellBackup);
            party->unk284[3] = 1;
          }
          goto leave;
        }
      }
      FieldMessage_Show((s32)MsgPlayOnHardLevel, 13);
      i = Menu_RunConfirmSelection(1, (s32)MsgYeah, 8, 0);
      Message_CloseFinishedWindows();
      if (i == 0) {
        Flag_Set(46);
        gMainRoutineWork.unk284[3] = 2;
        goto leave;
      }
      FieldMessage_Show((s32)MsgPlayOnNormalLevel, 13);
      i = Menu_RunConfirmSelection(1, (s32)MsgYeah, 8, 0);
      Message_CloseFinishedWindows();
      if (i != 0)
        goto save;
    }
  leave:
    Func_020023b8((s32)&SceneId_Title, 8);
    goto finish;
  }
  if (result == 1) {
    result = Func_020022d8(1);
    if (result == -1)
      goto top;
    Flag_Set(0x109);
    {
      struct MainRoutineWork *party = &gMainRoutineWork;

      Func_02002310(party->paletteGlow[0], party->paletteGlow[1]);
      if (PARTY_WORD(party, 0) != Func_02002388()) {
        RETURN_TO_SAVED(party)
      } else if (mask = 0x208, (gInput[0] & mask) == mask) {
        if (Func_020002e0()) {
          failure = (s32)MsgDataCouldNotBeRecovered;
          goto refuse;
        }
        RETURN_TO_SAVED(party)
        Flag_Set(0x13e);
      } else if (party->playTime != gData_02001000) {
        recovery = (s32)MsgSomeDataIsCorrupted;
        FieldMessage_Show(recovery, 9);
        FieldMessage_Show(recovery + 1, 13);
        if (Menu_RunConfirmSelection(1, 0, 0, 0) != 0) {
          Message_CloseFinishedWindows();
          goto top;
        }
        Message_CloseFinishedWindows();
        if (Func_020002e0()) {
          failure = recovery + 2;
        refuse:
          FieldMessage_Show(failure, 9);
          goto top;
        }
        RETURN_TO_SAVED(party)
        Flag_Set(0x13f);
      }
    }
    Flag_Clear(0x106);
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
    result = Func_020022d8(4);
    if (result == -1)
      goto top;
    {
      struct MainRoutineWork *party = &gMainRoutineWork;

      party->currentOwner = 4;
      if (Flag_IsSet(0x9ff))
        Func_02002390();
      Func_02002310(party->paletteGlow[0], party->paletteGlow[1]);
      Flag_Clear(0x109);
      Flag_Clear(0x106);
      party->ownerOverride = 0;
      gSleepDisabled = 1;
      Func_020023b8((s32)&SceneId_Lobby, 1);
    }
    goto finish;
  }
  if (result != 5)
    goto top;
link:
  result = Func_020022d8(6);
  if (result == -1)
    goto top;
  Func_02002310(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
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
    Func_02002370(Ram_MapCellBackup, 0, 1);
  } else if (result == 0) {
    SAVE_CELLS();
    Func_02002350();
    status = Func_02000e44();
    RESTORE_CELLS()
    if (status == -1)
      goto again;
    Func_02002370(Ram_MapCellBackup, 1, 1);
  }
  if (gData_020024f6 != 0 || gData_020024f8 != 0) {
    i = 0;
    limit = 1;
  shut:
    wait(6);
    result = Func_020022f8(i);
    if (result == -1) {
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
  Flag_Set(0x109);
finish:
  ((struct EventWork *)Ram_HeapSlots->eventWork)->countdown = 999;
  FieldEvent_Wait(30);
  Func_02002408(78);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(60);
  return 0;
}
