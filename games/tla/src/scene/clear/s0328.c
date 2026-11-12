/// @file
/// Scene code of overlay 0x64a, in ROM order, from s0328's offset up to the next object the overlay links.

#include "edition.h"
#include "version.h"
#include "types.h"
#include "text/msg_ids.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "frame.h"
#include "scene_ids.h"
#include "call.h"
#include "field/event_work.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "io_reg.h"
#include "window.h"

extern s16 gData_020024f6;
extern s16 gData_020024f8;
extern s16 gData_02003860;
extern u32 gData_02001000;
extern u8 gData_02000000[];
extern u32 gInput[];
extern char SceneId_Ov6b5;

void Func_0200005c(void);
void Func_0200015c(void);
void Func_020001f4(void);
s32 Func_020002e0(void);
s32 Func_02000b24(void);
s32 Func_02000e44(void);
s32 Func_020022b0(s32 mode);
s32 Func_020022d0(void);
s32 Func_020022d8(s32 mode);
void Func_020022e0(void);
void Func_020022e8(void);
s32 Func_020022f0(void);
s32 Func_020022f8(s32 step);
void Func_02002350(void);
void Func_02002370(void *data, s32 a, s32 b);
u32 Func_02002388(void);
void Func_02002390(void);
void Func_02002398(void);
void Func_020023a0(void *data);
void Func_020023b8(s32 a, s32 b);
void Func_020023e0(s32 scene, s32 entrance);
void Func_020023e8(void);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
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

#define MAP_CELL_BACKUP (Ram_MapCellBuffer + 0x3000)

#if (LANGUAGE == LANGUAGE_JAPANESE)
#define YEAH_WIDTH 6
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define YEAH_WIDTH 10
#else
#define YEAH_WIDTH 8
#endif

/// Swaps the map cell buffer out around a password or link screen.
#define COPY_CELLS(to, from)                                                   \
  Value3((s32 (*)())Iwram_CopyWords, (s32)(to), (s32)(from), 0x3000)
#define SAVE_CELLS()                                                           \
  u8 *cells = gData_02000000;                                                  \
                                                                               \
  COPY_CELLS(Ram_MapCellBuffer, cells)
#define RESTORE_CELLS()                                                        \
  {                                                                            \
    COPY_CELLS(MAP_CELL_BACKUP, cells);                                        \
    COPY_CELLS(cells, Ram_MapCellBuffer);                                      \
  }

/// Goes back to the saved scene and entrance.
#define RETURN_TO_SAVED(party)                                                 \
  {                                                                            \
    (party)->scene = (party)->savedScene;                                      \
    (party)->entrance = (party)->savedEntrance;                                \
    Func_020023e0((party)->scene, (party)->entrance);                          \
    Event_ClearFlag(0x109);                                                         \
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
    i = Func_080d295c(1);
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
    Event_Wait(60);
    Event_FinishScreenTransition();
    Sound_Play(78);
    Event_Wait(300);
    Func_020023b8((s32)&SceneId_Title, 1);
    return 0;
  }
  gSleepDisabled = started;
top:
  result = Func_020022f0();
  if (result < 0 && gDebugMode) {
    gMainRoutineWork.unk244[6] = 1;
    gAutoSleepEnabled = 1;
    Func_0803ce64((s32)&SceneId_DailaInteriors1, 1, 8);
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
    Func_0803f9c0(gMainRoutineWork.paletteGlow[0],
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
        Func_02002370(MAP_CELL_BACKUP, 0, 0);
      } else if (result == 0) {
        SAVE_CELLS();
        Func_02002350();
#if !TLA_PATCH_2
        status = Func_02000e44();
        RESTORE_CELLS()
        if (status == -1)
          goto enter;
#else
        Func_02000e44();
        RESTORE_CELLS()
#endif
        Func_02002370(MAP_CELL_BACKUP, 1, 0);
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
      i = Menu_RunConfirmSelection(1, (s32)MsgYeah, YEAH_WIDTH, 0);
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

            Func_0803f9c0(party->paletteGlow[0], party->paletteGlow[1]);
            RESTORE_CELLS()
            Func_020023a0(MAP_CELL_BACKUP);
            party->unk284[3] = 1;
          }
          goto leave;
        }
      }
      FieldMessage_Show((s32)MsgPlayOnHardLevel, 13);
      i = Menu_RunConfirmSelection(1, (s32)MsgYeah, YEAH_WIDTH, 0);
      Message_CloseFinishedWindows();
      if (i == 0) {
        Event_SetFlag(46);
        gMainRoutineWork.unk284[3] = 2;
        goto leave;
      }
      FieldMessage_Show((s32)MsgPlayOnNormalLevel, 13);
      i = Menu_RunConfirmSelection(1, (s32)MsgYeah, YEAH_WIDTH, 0);
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
    Event_SetFlag(0x109);
    {
      struct MainRoutineWork *party = &gMainRoutineWork;

      Func_0803f9c0(party->paletteGlow[0], party->paletteGlow[1]);
      if (PARTY_WORD(party, 0) != Func_02002388()) {
        RETURN_TO_SAVED(party)
      } else if (mask = 0x208, (gInput[0] & mask) == mask) {
        if (Func_020002e0()) {
          failure = (s32)MsgDataCouldNotBeRecovered;
          goto refuse;
        }
        RETURN_TO_SAVED(party)
        Event_SetFlag(0x13e);
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
        Event_SetFlag(0x13f);
      }
    }
    Event_ClearFlag(0x106);
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
      if (Event_GetFlag(0x9ff))
        Func_02002390();
      Func_0803f9c0(party->paletteGlow[0], party->paletteGlow[1]);
      Event_ClearFlag(0x109);
      Event_ClearFlag(0x106);
      party->ownerOverride = 0;
      gSleepDisabled = 1;
      Func_020023b8((s32)&SceneId_Ov6b5, 1);
    }
    goto finish;
  }
  if (result != 5)
    goto top;
link:
  result = Func_020022d8(6);
  if (result == -1)
    goto top;
  Func_0803f9c0(gMainRoutineWork.paletteGlow[0],
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
    Func_02002370(MAP_CELL_BACKUP, 0, 1);
  } else if (result == 0) {
    SAVE_CELLS();
    Func_02002350();
    status = Func_02000e44();
    RESTORE_CELLS()
    if (status == -1)
      goto again;
    Func_02002370(MAP_CELL_BACKUP, 1, 1);
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
#if TLA_PATCH_1
  Event_SetFlag(0x109);
#endif
finish:
  ((struct EventWork *)Ram_HeapSlots->eventWork)->countdown = 999;
  Event_Wait(30);
  Sound_Play(78);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(60);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#if (LANGUAGE == LANGUAGE_JAPANESE)
/// The character a cell of the password grid stands for, in the Japanese
/// font's codes: kana, the voiced ones with the dakuten mark in the high byte.
void Func_02000a70(s32 cell, u16 *text) {
  text[1] = 0;
  text[2] = 0;
  if (cell <= 14)
    text[0] = cell + 145;
  else if (cell <= 43)
    text[0] = cell + 209;
  else if (cell == 45)
    text[0] = 0x86;
  else if (cell == 44)
    text[0] = 0xfd;
  else if (cell <= 55)
    text[0] = (cell + 104) | 0x4000;
  else if (cell == 56)
    text[0] = 0xe0 | 0x4000;
  else if (cell == 57)
    text[0] = 0xe3 | 0x4000;
  else if (cell == 58)
    text[0] = 0xe4 | 0x4000;
  else
    text[0] = (cell + 175) | 0x4000;
}
#else
/// The character a cell of the password grid stands for, in the font's
/// codes: capitals without I and O, the digits 2 to 9, small letters without
/// l and o, then symbols.
void Func_02000a70(s32 cell, u16 *text) {
  text[1] = 0;
  text[2] = 0;
  if (cell <= 7)
    text[0] = cell + 'A';
  else if (cell <= 12)
    text[0] = cell + 'A' + 1;
  else if (cell <= 23)
    text[0] = cell + 'A' + 2;
  else if (cell <= 31)
    text[0] = cell + 26;
  else if (cell <= 42)
    text[0] = cell + 'A';
  else if (cell <= 44)
    text[0] = cell + 'A' + 1;
  else if (cell <= 55)
    text[0] = cell + 'A' + 2;
  else if (cell == 56)
    text[0] = '!';
  else if (cell == 57)
    text[0] = '?';
  else if (cell == 58)
    text[0] = '#';
  else if (cell == 59)
    text[0] = '&';
  else if (cell == 60)
    text[0] = '$';
  else if (cell == 61)
    text[0] = '%';
  else if (cell == 62)
    text[0] = '+';
  else
    text[0] = '=';
}
#endif

// The copies go through an inline wrapper, as the Broken Seal's iwram_call.h
// writes it.
#undef Iwram_CopyWords
static inline s32 Iwram_CopyWords(void *destination, const void *source,
                                  s32 size) {
  return Iwram_CopyWordsEntry(destination, source, size);
}

extern volatile u16 gLinkStatus;
extern u16 gLinkPeerSignatures[][12];
extern u16 gSerialReceivedSize;
extern s32 gSerialReceiveDest;
extern u16 gData_02003a74[4];
extern u8 gGameFlagBytes[];
#define MAP_CELL_BACKUP (Ram_MapCellBuffer + 0x3000)
#define RECEIVED_FLAGS (Ram_MapCellBuffer + 0x3040)
#define RECEIVED_UNITS (Ram_MapCellBuffer + 0x3240)

void *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 mode);
void Text_DrawMessage(s32 message, void *window, s32 x, s32 y);

void WindowSprite_PrepareForRedraw(void *window);
s32 Serial_StartReceive(void *destination);
void Unit_Calculate(s32 id);
void Func_02002220(void);
void Func_02002228(void);

#if (LANGUAGE == LANGUAGE_JAPANESE)
#define REQUEST_EDITION 'C'
#elif (LANGUAGE == LANGUAGE_GERMAN)
#define REQUEST_EDITION 'E'
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define REQUEST_EDITION 'S'
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define REQUEST_EDITION 'F'
#elif (LANGUAGE == LANGUAGE_ITALIAN)
#define REQUEST_EDITION 'I'
#else
#define REQUEST_EDITION 'U'
#endif

#define MSGNO(name) ((s32)(name))

// The windows are as wide as each language's text needs.
#if (LANGUAGE == LANGUAGE_FRENCH)
#define RECEIVE_WINDOW_X 2
#define RECEIVE_WINDOW_WIDTH 26
#define STATUS_WINDOW_X 2
#define STATUS_WINDOW_WIDTH 26
#elif (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH)
#define RECEIVE_WINDOW_X 5
#define RECEIVE_WINDOW_WIDTH 20
#define STATUS_WINDOW_X 5
#define STATUS_WINDOW_WIDTH 20
#else
#define RECEIVE_WINDOW_X 6
#define RECEIVE_WINDOW_WIDTH 18
#define STATUS_WINDOW_X 5
#define STATUS_WINDOW_WIDTH 20
#endif

/// Receives clear data over the link cable: waits for a peer with a matching
/// signature, then for the transfer, and copies the received flags and units
/// in.
s32 Func_02000b24(void) {
  void *window;
  s32 i, idle, ok, shown;
  s32 mask, linkMask;
  s32 message;
  u8 *flags;
  u16 *request;

  window = Window_Create(RECEIVE_WINDOW_X, 5, RECEIVE_WINDOW_WIDTH, 8, 2);
  message = MSGNO(MsgFieldPreparingToReceive);
  Text_DrawMessage(message, window, 0, 4);
  Text_DrawMessage(message + 1, window, 0, 16);
  Text_DrawMessage(message + 3, window, 0, 36);
  Func_02002220();
  System_Wait(10);
restart:
  mask = 3;
  goto poll;
scan:
  if ((gLinkStatus & mask) == mask) {
    u16 *signature = gLinkPeerSignatures[((REG_SIOCNT << 26) >> 30) ^ 1];

    if (signature[0] == 48 && signature[1] == 48 && signature[2] == 48 &&
        signature[3] == 48) {
      request = gData_02003a74;
      request[0] = REQUEST_EDITION;
      request[1] = 'V';
      request[2] = 'T';
      request[3] = 'S';
      goto receive;
    }
  }
  System_Wait(1);
poll:
  if (!(gInput[1] & 2))
    goto scan;
  Event_PlaySound(113);
  goto close;
receive:
  System_Wait(10);
  Serial_StartReceive(MAP_CELL_BACKUP);
  System_Wait(10);
  linkMask = 3;
  idle = 0;
  ok = 1;
  shown = 0;
  i = 0;
transfer:
  if (!shown && gSerialReceivedSize != 0) {
    shown = 1;
    Window_Close(window, 2);
    window = Window_Create(STATUS_WINDOW_X, 10, STATUS_WINDOW_WIDTH, 4, 2);
    Text_DrawMessage(MSGNO(MsgFieldTransferringData), window, 0, 4);
  }
  if (i == 9 && gSerialReceivedSize == 0) {
    request = gData_02003a74;
    request[0] = 0;
    request[1] = 0;
    request[2] = 0;
    request[3] = 0;
    Func_02002228();
    System_Wait(10);
    Func_02002220();
    System_Wait(10);
    goto restart;
  }
  idle++;
  if ((gLinkStatus & linkMask) == linkMask)
    idle = 0;
  if (idle == 10) {
    ok = 0;
    goto done;
  }
  if (gSerialReceiveDest != 0) {
    System_Wait(1);
    i++;
    if (i < 600000)
      goto transfer;
  }
done:
  if (!ok) {
    WindowSprite_PrepareForRedraw(window);
    Text_DrawMessage(MSGNO(MsgFieldDataTransferWasInterrupted), window, 0, 4);
  interrupted:
    System_Wait(1);
    if (!(gInput[1] & 1))
      goto interrupted;
  close:
    Window_Close(window, 2);
    return -1;
  }
  System_Wait(10);
  Func_02002228();
  System_Wait(10);
  Window_Close(window, 2);
  flags = gGameFlagBytes;
  Iwram_CopyWords(flags, RECEIVED_FLAGS, 0x214);
  Iwram_CopyWords(flags + 0x220, RECEIVED_UNITS, 0xd20);
  Unit_Calculate(0);
  Unit_Calculate(1);
  Unit_Calculate(2);
  Unit_Calculate(3);
  Iwram_CopyWords(RECEIVED_FLAGS, flags, 0xf40);
  WindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  return 1;
}

// This overlay's copy of Window_SetTilemapFlagRect (window/fixed_block.c): sets
// the field in the tilemap flags of a rectangle of a window, clipped to the
// screen.
void Func_02000db4(const struct Window *object, s32 x, s32 y, s32 width,
                   s32 height, u32 field) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;

  x += (s16)object->x + 1;
  y += (s16)object->y + 1;
  field <<= 12;
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29) {
    width = 30 - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29) {
    height = 20 - y;
  }
  if (width > 0 && height > 0) {
    do {
      u16 *pixel = &work->tilemap[y][x];
      s32 remaining = width;

      while (remaining != 0) {
        u32 value = *pixel;

        value &= 0xFFFFEFFF;
        value |= field;
        remaining--;
        *pixel = value;
        pixel++;
      }
      height--;
      y++;
    } while (height != 0);
    work->dirty = 1;
  }
}
