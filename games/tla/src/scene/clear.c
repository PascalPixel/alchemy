/// @file
/// The scene code of overlay 0x64a, in ROM order.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "bg_scroll.h"
#include "dma.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "main_routine_work.h"
#include "actor.h"
#include "ram_buffer.h"
#include "resource.h"
#include "event_inline.h"
#include "input.h"
#include "scene_ids.h"
#include "event_work.h"
#include "frame.h"
#include "heap_slots.h"
#include "module/game/game.h"
#include "module/scene/clear.h"
#include "module/scene/scene.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "module/window/screens.h"
#include "module/window/window.h"
#include "serial.h"

extern u8 gData_0200265e[];
extern u8 Data_02012f94[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

static __inline__ void Decode_(void *source, void *destination) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps the buffer address
   * a constant of each call, as ☀️'s system call layer does; as a direct call
   * the address is kept in r5; as Decompress_TaggedLz called directly, 21 of
   * 103 instructions differ in tla-en (Func_0200005c). */
  Decompress_TaggedLz(source, destination);
}

/// The logo background, as the title's Title_LoadBackground, then the party
/// leader's motion flags cleared.
// As ☀️'s Clear_LoadBackground.
void Func_0200005c(void) {
  u8 *res;
  s32 id;
  u16 *map;
  u32 x;
  u32 y;
  u16 tile;
  struct BgScroll *scroll;
  s32 blank;

  id = (s32)&ResourceId_LogoOnBlack;
  ScreenFade_ToBlack(0);
  REG_BG2CNT = 0x681;
  gBgScroll[2].y = 0;
  blank = 0x1ff;
  res = Resource_GetTableEntry(id);
  Dma_Set(res, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  res += 0x1c0;
  Decode_(res, Ram_MapCellBuffer);
  Dma_Set((void *)Ram_MapCellBuffer, (void *)0x06006800,
          DMA_COPY_WORDS(0x9600 / 4), REG_DMA3);

  map = (u16 *)0x06003000;
  tile = 0x1a0;
  // Written with goto: as for loops, the pinned column counter keeps its exit
  // test ahead of the loop, and the scroll loop's zero leaves it.
  y = 0;
row:
  x = 0;
column:
  *map++ = tile++;
  x++;
  if (x <= 29)
    goto column;
  *map++ = blank;
  *map++ = blank;
  y++;
  if (y <= 19)
    goto row;
  scroll = gBgScroll;
  y = 0;
scroll_pair:
  scroll->y = 0;
  scroll->x = 0;
  scroll++;
  y++;
  if (y <= 3)
    goto scroll_pair;
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  ((struct ClearWork *)Ram_HeapSlots->mapWork)->mode = 0x1400;
  {
    struct Actor *leader;
    /* FAKEMATCH(pin): the flag's zero pinned to r5 as a halfword, so that it
     * loads from the pool after the call, as the native has; as a plain store
     * it reuses the scroll loop's zero kept in r5; as leader->motionFlags = 0,
     * 4 of 103 instructions differ in tla-en. */
    register u16 zero asm("r5");

    leader = Actor_Get(gMainRoutineWork.currentOwner);
    zero = 0;
    leader->motionFlags = zero;
  }
}

/// Queues an alpha blend of BG0 and the sprites over the rest whose first
/// weight rises by one every second frame (the second weight 16 less), and
/// removes the callback once the first weight passes 15.
// As ☀️'s Clear_UpdateBlend.
void Func_0200015c(void) {
  s32 frame;
  u32 level;

  frame = gData_020024f4[0] + 1;
  gData_020024f4[0] = frame;
  level = (u16)frame >> 1;
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT1_BG0 | BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND |
                          BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 |
                          BLDCNT_TGT2_BD,
                      IO_WRITE_16);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - (u16)level) << 8) | (u16)level,
                      IO_WRITE_16);
  if (level > 15)
    FieldTask_Remove(Func_0200015c);
}

/// Follows the two secret button codes: each held key that matches the next
/// key of a code advances it, a wrong key starts it again, and finishing a
/// code unlocks it with a sound.
void Func_020001f4(void) {
  if (gData_020024f6 == 0) {
    /* FAKEMATCH(pin): the held keys pointer pinned to r4, with the key pinned
     * to r3 below; as an unpinned local, 12 of 92 instructions differ in
     * tla-en. */
    register volatile u32 *held asm("r4") = &gInput.held;

    if (gData_020024fe != 0) {
      /* FAKEMATCH(pin): the key pinned to r3, with the pointer above; as an
       * unpinned local, 6 of 92 instructions differ in tla-en. */
      register u32 keys asm("r3") = *held;

      if (keys == 0)
        gData_020024fe = keys;
    } else if (*held != 0) {
      if (*held == gData_02002410[gData_020024fa]) {
        gData_020024fa++;
        gData_020024fe = 1;
        if (gData_02002410[gData_020024fa] == 0) {
          gData_020024f6 = 1;
          Event_PlaySound(110);
        }
      } else {
        gData_020024fa = 0;
      }
    }
  }
  if (gData_020024f8 == 0) {
    /* FAKEMATCH(pin): the held keys pointer pinned to r4, as for the first
     * code; as an unpinned local, 12 of 92 instructions differ in tla-en. */
    register volatile u32 *held asm("r4") = &gInput.held;

    if (gData_02002500 != 0) {
      /* FAKEMATCH(pin): the key pinned to r3, as for the first code; as an
       * unpinned local, 6 of 92 instructions differ in tla-en. */
      register u32 keys asm("r3") = *held;

      if (keys == 0)
        gData_02002500 = keys;
    } else if (*held != 0) {
      if (*held == gData_02002418[gData_020024fc]) {
        gData_020024fc++;
        gData_02002500 = 1;
        if (gData_02002418[gData_020024fc] == 0) {
          gData_020024f8 = 1;
          Event_PlaySound(110);
        }
      } else {
        gData_020024fc = 0;
      }
    }
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_020002e0(void) {
  struct MainRoutineWork *party;
  s32 mode;

  if (Event_GetFlag(324) == 0)
    return 0;
  party = &gMainRoutineWork;

  if (FIELD_AT(party, s16, 0x25e) == 2)
    return 0;
  mode = party->scene ^ (s32)&SceneId_WorldMap;
  return -((u32)(-mode | mode) >> 31);
}

#include "edition.h"
#include "version.h"
#include "text/msg_ids.h"
#include "iwram_call.h"
#include "frame.h"
#include "call.h"
#include "field/event_work.h"
#include "window.h"

/* VIEW(clear): variable gData_02003860 as s16 gData_02003860; module/tla.h has
 * u16 gData_02003860; tried as the 2 declarations' extern s16 gData_02003860,
 * it does not compile in tla-ja: games/tla/src/scene/clear.c:243: conflicting
 * types for `gData_02003860'. */
extern s16 gData_02003860__clear __asm__("gData_02003860");
/* VIEW(clear): variable gData_02001000 as u32 gData_02001000; module/tla.h has
 * s8 gData_02001000[]; tried as the rest of the object declares it,
 * Scene_Initialize moves: 2 of 738 instructions in tla-ja. */
extern u32 gData_02001000__clear __asm__("gData_02001000");

s32 Import_SaveToChosenFile(void);
s32 Save_ChooseAndLoadFile(s32 mode);
void Save_CopyFile(void);
u32 BuildStamp_GetTime(void);
void InventorySnapshot_Restore(void);
void StatusScreen_Run(void);

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
  Value3((s32 (*)())Iwram_CopyWordsEntry, (s32)(to), (s32)(from), 0x3000)
#define SAVE_CELLS()                                                           \
  u8 *cells = gSaveBuffer;                                                     \
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
    Event_ClearFlag(0x109);                                                    \
  }

s32 Scene_Initialize(void) {
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
    i = FieldYesNo_Ask(1);
    Message_CloseFinishedWindows();
    if (i == 0) {
      FieldMessage_Show(message + 1, 1);
      gMainRoutineWork.unk22d[2] = 1;
      Party_AddMember(5);
      Party_AddMember(6);
      Party_AddMember(7);
      Party_AddMember(1);
      Party_AddMember(2);
      Party_AddMember(3);
      i = Import_SaveToChosenFile();
      if (i == -1)
        goto ask_save;
    }
    Event_Wait(60);
    Event_FinishScreenTransition();
    Sound_Play(78);
    Event_Wait(300);
    Event_SetDestination((s32)&SceneId_Title, 1);
    return 0;
  }
  gSleepDisabled = started;
top:
  result = Import_SaveScanFileSummaries();
  if (result < 0 && gDebugMode) {
    gMainRoutineWork.unk244[6] = 1;
    gAutoSleepEnabled = 1;
    FieldMessage_ShowCentered((s32)&SceneId_DailaInteriors1, 1, 8);
  }
  if (result == 0 && !started) {
    wait(30);
    Task_Add(Func_0200015c, 0x480);
    wait(1);
    started = 1;
  }
  if (result > 0)
    result = FileList_SelectAction();
  else
    result = 0;
  if (result == 0) {
    Import_MainRoutineWorkInitializeDefaults();
    WindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                      gMainRoutineWork.paletteGlow[1]);
    i = 4;
    limit = 5;
  open:
    wait(6);
    result = NameEntry_EditMemberName(i);
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
    result = YesNoMenu_Run(1, 0, 0, 0);
    Message_CloseFinishedWindows();
    if (result == 0) {
    enter:
      result = IconMenu_ChooseSendOption(0);
      if (result == -1)
        goto top;
      if (result == 1) {
        SAVE_CELLS();
        status = Func_02000b24();
        if (status != -1)
          StatusScreen_Run();
        RESTORE_CELLS()
        if (status == -1)
          goto enter;
        Func_02002370(MAP_CELL_BACKUP, 0, 0);
      } else if (result == 0) {
        SAVE_CELLS();
        Import_MainRoutineWorkInitializeDefaults();
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
    result = NameEntry_EditMemberName(i);
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
    if (gData_02003860__clear != 0) {
    save:
      message = (s32)MsgPlayOnEasyLevel;
      FieldMessage_Show(message, 13);
      i = YesNoMenu_Run(1, (s32)MsgYeah, YEAH_WIDTH, 0);
      Message_CloseFinishedWindows();
      if (i == 0) {
        if (gData_02003860__clear == 0) {
          FieldMessage_Show(message + 1, 1);
        } else {
          SAVE_CELLS();
          result = Save_ChooseAndLoadFile(5);
          if (result != 0)
            goto save;
          {
            struct MainRoutineWork *party = &gMainRoutineWork;

            WindowColor_Apply(party->paletteGlow[0], party->paletteGlow[1]);
            RESTORE_CELLS()
            Func_020023a0(MAP_CELL_BACKUP);
            party->unk284[3] = 1;
          }
          goto leave;
        }
      }
      FieldMessage_Show((s32)MsgPlayOnHardLevel, 13);
      i = YesNoMenu_Run(1, (s32)MsgYeah, YEAH_WIDTH, 0);
      Message_CloseFinishedWindows();
      if (i == 0) {
        Event_SetFlag(46);
        gMainRoutineWork.unk284[3] = 2;
        goto leave;
      }
      FieldMessage_Show((s32)MsgPlayOnNormalLevel, 13);
      i = YesNoMenu_Run(1, (s32)MsgYeah, YEAH_WIDTH, 0);
      Message_CloseFinishedWindows();
      if (i != 0)
        goto save;
    }
  leave:
    Event_SetDestination((s32)&SceneId_Title, 8);
    goto finish;
  }
  if (result == 1) {
    result = Save_ChooseAndLoadFile(1);
    if (result == -1)
      goto top;
    Event_SetFlag(0x109);
    {
      struct MainRoutineWork *party = &gMainRoutineWork;

      WindowColor_Apply(party->paletteGlow[0], party->paletteGlow[1]);
      if (PARTY_WORD(party, 0) != BuildStamp_GetTime()) {
        RETURN_TO_SAVED(party)
      } else if (mask = 0x208, (gInput.held & mask) == mask) {
        if (Func_020002e0()) {
          failure = (s32)MsgDataCouldNotBeRecovered;
          goto refuse;
        }
        RETURN_TO_SAVED(party)
        Event_SetFlag(0x13e);
      } else if (party->playTime != gData_02001000__clear) {
        recovery = (s32)MsgSomeDataIsCorrupted;
        FieldMessage_Show(recovery, 9);
        FieldMessage_Show(recovery + 1, 13);
        if (YesNoMenu_Run(1, 0, 0, 0) != 0) {
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
    Save_CopyFile();
    goto top;
  }
  if (result == 3) {
    Save_EraseSelectedFile();
    goto top;
  }
  if (result == 4) {
    result = Save_ChooseAndLoadFile(4);
    if (result == -1)
      goto top;
    {
      struct MainRoutineWork *party = &gMainRoutineWork;

      party->currentOwner = 4;
      if (Event_GetFlag(0x9ff))
        InventorySnapshot_Restore();
      WindowColor_Apply(party->paletteGlow[0], party->paletteGlow[1]);
      Event_ClearFlag(0x109);
      Event_ClearFlag(0x106);
      party->ownerOverride = 0;
      gSleepDisabled = 1;
      Event_SetDestination((s32)&SceneId_Ov6b5, 1);
    }
    goto finish;
  }
  if (result != 5)
    goto top;
link:
  result = Save_ChooseAndLoadFile(6);
  if (result == -1)
    goto top;
  WindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                    gMainRoutineWork.paletteGlow[1]);
again:
  result = IconMenu_ChooseSendOption(2);
  if (result == -1)
    goto link;
  if (result == 1) {
    SAVE_CELLS();
    status = Func_02000b24();
    if (status != -1)
      StatusScreen_Run();
    RESTORE_CELLS()
    if (status == -1)
      goto again;
    Func_02002370(MAP_CELL_BACKUP, 0, 1);
  } else if (result == 0) {
    SAVE_CELLS();
    Import_MainRoutineWorkInitializeDefaults();
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
    result = NameEntry_EditMemberName(i);
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

#define RECEIVED_FLAGS (Ram_MapCellBuffer + 0x3040)
#define RECEIVED_UNITS (Ram_MapCellBuffer + 0x3240)

void WindowSprite_PrepareForRedraw(struct Window *work);
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
  Serial_Initialize();
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
  if (!(gInput.pressed & 2))
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
    Serial_RemoveIrqHandlers();
    System_Wait(10);
    Serial_Initialize();
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
    if (!(gInput.pressed & 1))
      goto interrupted;
  close:
    Window_Close(window, 2);
    return -1;
  }
  System_Wait(10);
  Serial_RemoveIrqHandlers();
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
  FieldWindowColor_Apply(gMainRoutineWork.paletteGlow[0],
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

#include "uiwindow.h"
#include "oam.h"
#include "fixed_math.h"
#include "heap_release.h"
#include "battle/battle_unit.h"
#include "owner_state.h"

extern u8 gData_0200247c[];

extern u8 MsgFieldPasswordKana[];

void Text_DrawMessageAsTiles(s32 character, struct RenderInput *position,
                             u32 offsetX, u32 offsetY);
s32 Party_RemoveMember(s32 value);
u16 Func_0200217c(u32 length, u8 *p);
void Resource_CopyFixedBlockB(void);
void Resource_CopyFixedBlockA(void);
void Graphics_ExpandVramTilesByColorTable(void *vram);
void MenuCursor_Advance(struct ShopCursor *cursor);
void MenuCursor_Set(struct ShopCursor *cursor, s32 x, s32 y, s32 kind);
void MenuCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 x, s32 y);

/// Gives a cursor its sprite.
static inline void Cursor_Attach(struct ShopCursor *cursor,
                                 struct WindowSprite *sprite) {
  cursor->anchor = sprite;
}

/// Sets a cursor where it starts.
static inline void Cursor_Place(struct ShopCursor *cursor, s32 x, s32 y) {
  MenuCursor_SetPositionImmediate(cursor, x, y);
}

/// Moves a cursor and shows it.
static inline void Cursor_Show(struct ShopCursor *cursor, s32 x, s32 y) {
  MenuCursor_Set(cursor, x, y, 1);
  MenuCursor_Advance(cursor);
}

#if (LANGUAGE == LANGUAGE_JAPANESE)
// The Japanese grid has 20 columns of kana and its side column is 20.
#define SIDE_COLUMN 20
#define SIDE_HIGHLIGHT_WIDTH 3
#define CHECK_WINDOW_X 8
#define CHECK_WINDOW_WIDTH 14
// Japanese decodes into the bytes after its 260-byte entered password.
#define DECODED_PASSWORD Data_0200284e
#define CURSOR_ON_SIDE_COLUMN(column) ((column) == SIDE_COLUMN)
#else
#define SIDE_COLUMN 16
#define SIDE_HIGHLIGHT_WIDTH 6
#define CHECK_WINDOW_X 5
#define CHECK_WINDOW_WIDTH 20
#define DECODED_PASSWORD Data_0200288a
#define CURSOR_ON_SIDE_COLUMN(column) ((column) > SIDE_COLUMN - 1)
#endif

#if (LANGUAGE == LANGUAGE_JAPANESE)
/// Draws the kana rows and the key help into the main window.
#define DRAW_TITLE(window)                                                     \
  {                                                                            \
    message = MSGNO(MsgFieldPasswordKana);                                     \
    Text_DrawMessageAsTiles(message++, window, 0, 0);                          \
    Text_DrawMessageAsTiles(message++, window, 0, 8);                          \
    Text_DrawMessageAsTiles(message++, window, 0, 16);                         \
    Text_DrawMessageAsTiles(message++, window, 0, 24);                         \
    Text_DrawMessageAsTiles(message++, window, 48, 0);                         \
    Text_DrawMessageAsTiles(message++, window, 48, 8);                         \
    Text_DrawMessageAsTiles(message++, window, 48, 16);                        \
    Text_DrawMessageAsTiles(message++, window, 48, 24);                        \
    Text_DrawMessageAsTiles(message++, window, 96, 0);                         \
    Text_DrawMessageAsTiles(message++, window, 96, 8);                         \
    Text_DrawMessageAsTiles(message++, window, 96, 16);                        \
    Text_DrawMessageAsTiles(message++, window, 96, 24);                        \
    Text_DrawMessageAsTiles(message++, window, 144, 0);                        \
    Text_DrawMessageAsTiles(message++, window, 144, 8);                        \
    Window_DrawDividerLine(window, 24, 0, 24, 5);                              \
    Text_DrawMessageAsTiles(message++, window, 192, 0);                        \
    Text_DrawMessageAsTiles(message++, window, 192, 8);                        \
    Text_DrawMessageAsTiles(message, window, 192, 24);                         \
  }
#else
/// Draws the password screen's title and key help into the main window.
#define DRAW_TITLE(window)                                                     \
  {                                                                            \
    Text_DrawStringInWindow(gData_02002440, window, 0, 0);                     \
    Text_DrawStringInWindow(gData_02002454, window, 0, 8);                     \
    Text_DrawStringInWindow(gData_02002468, window, 0, 16);                    \
    Text_DrawStringInWindow(gData_0200247c, window, 0, 24);                    \
    message = MSGNO(MsgFieldSpace);                                            \
    Window_DrawDividerLine(window, 20, 0, 20, 5);                              \
    Text_DrawMessageAsTiles(message++, window, 160, 0);                        \
    Text_DrawMessageAsTiles(message++, window, 160, 8);                        \
    Text_DrawMessageAsTiles(message, window, 160, 24);                         \
  }
#endif

s32 Func_02000e44(void) {
  struct ShopCursor cursorStore;
  struct ShopCursor gridStore;
  u16 glyph[12];
  s32 mode;
  s32 cursor;
  struct Window *grid;
  s32 column;
  s32 row;
  s32 length;
  s32 pages;
  struct PasswordWork *work;
  s32 drawColumn;
  s32 drawRow;
  s32 drawMode;
  s32 cursorX;
  s32 cursorY;
  s32 state;
  s32 advance;
  struct Window *window;
  s32 action;
  s32 page;
  s32 i;
  s32 message;
  s32 frame;
  s32 size2;
  s32 entry;
  struct WindowSprite *sprite;
  u16 *palette;
  s32 height;

  Window_LoadGraphics();
  WindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                    gMainRoutineWork.paletteGlow[1]);
  palette = (u16 *)0x050001c0;
  Dma_Set((const void *)0x050001e0, palette, (DMA_ENABLE | DMA_32BIT) << 16 | 8,
          (volatile u32 *)0x040000d4);
  palette[4] = 0x6318;
  mode = IconMenu_ChooseSendOption(1);
  if (mode == -1)
    return -1;
  Sound_Play(71);
  cursor = 0;
  column = 0;
  row = 0;
  work = Heap_AllocEwram(476);
  advance = 0;
  Graphics_ExpandVramTilesByColorTable((void *)0x06006000);
  if (mode == 0) {
    length = 260;
  } else {
    length = 61;
    if (mode != 1)
      length = 16;
  }
  pages = length / 50 + 1;
  window = Window_Create(1, 13, 29, 6, 2);
  DRAW_TITLE(window)
  grid = Window_Create(5, 1, 20, 12, 2);
  for (i = 0; i != 26; i++)
    work->unk1c2[i] = 0;
  for (i = 0; i != 260; i++)
    Data_0200274a[i] = 99;
  for (i = 0; i != 50; i++) {
    s32 col = i % 10;
    s32 y;
    s32 x;

    x = col * 12 + 8;
    y = i / 10 * 16 + 2;
    if (col > 4)
      x = col * 12 + 18;
    work->cells[i] = Func_02002330(grid, work->glyphs[i], x, y);
    work->shown[i] = 99;
    Iwram_ClearWordsEntry(work->cells[i], 128);
  }
  entry = VramCache_FindFreeEntry();
  if (entry <= 95) {
    Resource_CopyFixedBlockB();
    sprite = WindowSprite_Create(entry, OUTPUT_SIZE_16X16,
                                 (struct RenderInput *)window, 0, 0);
    Cursor_Attach(&cursorStore, sprite);
    Cursor_Place(&cursorStore, (s16)window->x * 8 - 2, (s16)window->y * 8 + 12);
  }
  entry = VramCache_FindFreeEntry();
  if (entry <= 95) {
    Resource_CopyFixedBlockA();
    sprite = WindowSprite_Create(entry, OUTPUT_SIZE_16X16,
                                 (struct RenderInput *)window, 0, 0);
    Cursor_Attach(&gridStore, sprite);
    sprite->sentinel = 255;
    sprite->sprite.oam.priority = 0;
    Cursor_Place(&gridStore, (s16)grid->x * 8 - 4, (s16)grid->y * 8 + 12);
  }
  height = 1;
  drawColumn = 0;
  drawRow = 0;
  drawMode = 1;
  cursorX = (s16)window->x * 8 - 8;
  cursorY = (s16)window->y * 8 + 11;
  state = 0;
  for (;;) {
    action = -1;
    if (state == 2)
      state = 1;
    Func_02000db4(window, drawColumn, drawRow, drawMode, height, 15);
    if (state == 0) {
      if (gInput.repeat & 16) {
        Sound_Play(111);
        {
          s32 next = column + 1;

          if (row == 2)
            column = next % SIDE_COLUMN;
          else
            column = next % (SIDE_COLUMN + 1);
        }
      }
      if (gInput.repeat & 32) {
        Sound_Play(111);
        if (row == 2)
          column = (column + SIDE_COLUMN - 1) % SIDE_COLUMN;
        else
          column = (column + SIDE_COLUMN) % (SIDE_COLUMN + 1);
      }
      if (gInput.repeat & 128) {
        Sound_Play(111);
        row = (row + 1) % 4;
        if (column == SIDE_COLUMN && row == 2)
          row = 3;
      }
      if (gInput.repeat & 64) {
        Sound_Play(111);
        row = (row + 3) % 4;
        if (column != SIDE_COLUMN)
          goto not_side;
        if (row == 2)
          row = 1;
      }
    }
    drawMode = SIDE_HIGHLIGHT_WIDTH;
    if (column != SIDE_COLUMN) {
    not_side:
      drawMode = 1;
    }
    drawColumn = column;
    drawRow = row;
    if (column > 4)
      drawColumn++;
    if (column > 9)
      drawColumn++;
    if (column > 14)
      drawColumn++;
    if (column > SIDE_COLUMN - 1)
      drawColumn++;
    Func_02000db4(window, drawColumn, row, drawMode, height, 14);
    {
      s32 x = ((s16)window->x + column) * 8 - 8;
      s32 y = ((s16)window->y + row) * 8 + 11;

      if (column > 4)
        x = ((s16)window->x + column) * 8;
      if (column > 9)
        x += 8;
      if (column > 14)
        x += 8;
      if (CURSOR_ON_SIDE_COLUMN(column))
        x += 8;
      cursorX += (x - cursorX) / 2;
      cursorY += (y - cursorY) / 2;
    }
    Cursor_Show(&cursorStore, cursorX, cursorY);
    {
      s32 cell = cursor % 50;
      s32 y = (cell / 10 * 2 + (s16)grid->y) * 8 - 2;
      s32 col;
      s32 x;

      frame = (gVBlankCount >> 1) & 7;
      col = cell % 10;
      x = col * 12 + (s16)grid->x * 8 + 16;

      if (col > 4)
        x += 10;
      x += gData_02002438[frame];
      y += gData_02002438[(frame + 5) & 7];
      Cursor_Show(&gridStore, x, y);
    }
    if (advance > 0 && ++advance == 8) {
      advance = 0;
      cursor++;
    }
    if (advance == 0) {
      s32 mask;
      u16 raw;
      s32 keys;
      struct InputState *input = &gInput;

      if (column == SIDE_COLUMN && row == 0)
        raw = input->repeat;
      else if (column == SIDE_COLUMN && row == 1)
        raw = input->repeat;
      else
        raw = input->pressed;
      keys = (s16)raw;
      keys <<= 16;
      mask = 0x10000;
      mask &= keys;
      if (mask && state == 0) {
        action = 0;
        if (column == SIDE_COLUMN) {
          if (row == 0)
            action = 1;
          else if (row == 1)
            action = 2;
          else if (row == 3)
            action = 3;
        } else if (cursor < length) {
          s32 code = gData_0200265e[column + row * SIDE_COLUMN];

          if (code <= 63) {
            Sound_Play(112);
            Data_0200274a[cursor] = code;
            // The native calls the remainder and reads nothing back.
            __modsi3(cursor, 10);
            if (cursor < length - 1) {
              if (cursor % 50 == 49)
                advance = 1;
              else
                cursor++;
            } else {
              state = 2;
            }
          }
        }
      }
      if (action != 0 || state == 1) {
        if (gInput.pressed & 8 || action == 3 || state == 1) {
          u16 check;
          struct Window *box;
          struct MainRoutineWork *party;
          s16 crc;

          action = Func_02002068(Data_0200274a, length, DECODED_PASSWORD);
          size2 = action - 2;
          check = Func_0200217c(size2, DECODED_PASSWORD);

          state = 0;
          box = Window_Create(CHECK_WINDOW_X, 12, CHECK_WINDOW_WIDTH, 3, 2);
          crc = check;
          if (DECODED_PASSWORD[size2] == (u16)crc >> 8 &&
              DECODED_PASSWORD[action - 1] == (u16)crc % 256) {
            Text_DrawMessageAsTiles(MSGNO(MsgFieldPasswordAccepted), box, 0, 0);
            Sound_Play(112);
          wait_accepted:
            System_Wait(1);
            if (!(gInput.pressed & 1))
              goto wait_accepted;
            Sound_Play(112);
            Import_MainRoutineWorkInitializeDefaults();
            party = &gMainRoutineWork;
            party->currentOwner = 0;
            Party_RemoveMember(4);
            Party_RemoveMember(5);
            Party_RemoveMember(6);
            Party_RemoveMember(7);
            Party_AddMember(0);
            Party_AddMember(1);
            Party_AddMember(2);
            Party_AddMember(3);
            Func_020018e0(mode, DECODED_PASSWORD);
            Window_Close(box, 2);
            Window_Close(grid, 2);
            Window_Close(window, 2);
            WindowColor_Apply(party->paletteGlow[0], party->paletteGlow[1]);
            Sound_Play(74);
            System_Wait(6);
            NameEntry_EditMemberName(0);
            StatusScreen_Run();
            Iwram_CopyWords(Data_02012f94 + 0xac, gGameFlagBytes, 0xf40);
            return 0;
          }
          Sound_Play(113);
          Text_DrawMessageAsTiles(MSGNO(MsgFieldInvalidPassword), box, 0, 0);
        wait_rejected:
          System_Wait(1);
          if (!(gInput.pressed & 1) && !(gInput.pressed & 2))
            goto wait_rejected;
          Window_Close(box, 2);
          WindowSprite_RedrawSavedRect(window);
          DRAW_TITLE(window)
        } else if (gInput.repeat & 512 || action == 2) {
          if (cursor > 0) {
            Sound_Play(111);
            cursor--;
          }
        } else if (gInput.repeat & 256 || action == 1) {
          if (cursor < length - 1) {
            Sound_Play(111);
            cursor++;
          }
        } else if (gInput.pressed & 2) {
          if (cursor > 0) {
            Sound_Play(111);
            cursor--;
          } else {
            Sound_Play(113);
            Window_Close(grid, 2);
            Window_Close(window, 2);
            Sound_Play(74);
            return -1;
          }
        }
      }
    }
    page = cursor / 50;
    WindowSprite_RedrawSavedRect(grid);
    {
      s32 y;

      for (i = 0, y = 2; i != 4; i++, y += 2)
        Window_DrawDividerLine(grid, 0, y, 18, y);
    }
    {
      for (i = 0; i != pages; i++) {
        s32 tile = i + 0xf301;

        if (i == page)
          tile = i + 0xf30b;
        Window_SetTilemapEntry(grid, tile, i - pages + 18, 10, 0);
      }
    }
    action = length;
    /* FAKEMATCH(barrier): the count is not known to be the length, whose
     * remainder the native reloads from the stack; as no asm, 2 of 1077
     * instructions differ in tla-en. */
    asm volatile("" : "+r"(action));
    if (length > 49) {
      if (pages - 1 == page) {
        /* FAKEMATCH(barrier): r2 no longer holds the length; as no asm, 2 of
         * 1077 instructions differ in tla-en. */
        asm volatile("" : : : "r2");
        action = length % 50;
      } else
        action = 50;
    }
    for (i = 0; i != action; i++) {
      u8 *entered = Data_0200274a;
      s32 index = i + page * 50;
      s32 raw = entered[index];
      u8 code = raw;

      if (code != work->shown[i]) {
        work->shown[i] = raw;
        if (code == 99) {
          Iwram_ClearWordsEntry(work->cells[i], 128);
        } else {
          Func_02000a70(entered[index], glyph);
          Func_02002328(glyph[0], work->cells[i]);
        }
      }
    }
    for (; i != 50; i++) {
      if (work->shown[i] != 99) {
        work->shown[i] = 99;
        Iwram_ClearWordsEntry(work->cells[i], 128);
      }
    }
    System_Wait(1);
  }
}

/// Unpacks source into a scratch block, then writes a 16 by 8 block of tiles
/// at the panel's cell into the BG map at 0x06002000 and into the window
/// tilemap's shadow: palette 15, the tile of each cell the row times the
/// panel's stride plus the column. The loops are the listing's: no loop pass
/// touches them, so the tile is rebuilt and the mask reloaded on every pass.
void Func_02001804(struct Panel_02001804 *panel, const void *source) {
  /* FAKEMATCH(pin): the shadow pointer in r5; as a plain local, 7 of 53
   * instructions differ in tla-en. */
  register u16 *shadow asm("r5") = (u16 *)Ram_HeapSlots->windowTiles;
  void *work = Heap_AllocEwram(0x300);
  u16 *tiles;
  s32 offset;
  s32 row;
  s32 column;

  Decompress_TaggedLz(source, work);
  offset = panel->y * 32 + panel->x;

  offset *= 2;
  tiles = (u16 *)(offset + 0x06002000);
  shadow = (u16 *)((u8 *)shadow + offset + 8);
  row = 0;
next_row:
  column = 0;
next_column:
  // One cell, the same tile in the map and in the shadow.
  {
    s16 tile = (panel->stride * row + column) | 0xf000;

    *tiles++ = tile;
    *shadow++ = tile;
  }
  column++;
  if (column <= 15)
    goto next_column;
  tiles += 16;
  shadow += 16;
  row++;
  if (row <= 7)
    goto next_row;
  Heap_Release(work);
}

void Func_0200187c(struct Panel_02001804 *panel) {
  /* FAKEMATCH(pin): the shadow pointer in r5; as a plain local, 23 of 43
   * instructions differ in tla-en. */
  register u16 *shadow asm("r5") = (u16 *)Ram_HeapSlots->windowTiles;
  void *work = Heap_AllocEwram(0x300);
  u16 *tiles;
  s32 offset;
  s32 row;
  s32 column;

  offset = (panel->y * 32 + panel->x) * 2;
  tiles = (u16 *)(offset + 0x06002000);
  shadow = (u16 *)((u8 *)shadow + offset + 8);
  row = 0;
next_row:
  column = 0;
next_column:
  // One cell, the same tile in the map and in the shadow.
  {
    s16 tile = (row * 16 + column + 32) | 0xf000;

    *tiles++ = tile;
    *shadow++ = tile;
  }
  column++;
  if (column <= 15)
    goto next_column;
  tiles += 16;
  shadow += 16;
  row++;
  if (row <= 7)
    goto next_row;
  Heap_Release(work);
}

extern s32 gData_020026a0[];
extern u16 gData_020026b0[];
extern u16 gData_020026c0[];

s32 Unit_GetExperienceForLevel(s32 id, s32 level);

void Func_020018e0(s32 mode, u8 *in) {
  u32 levelBits;
  u32 djinnBits;
  u8 flagBits;
  u8 keyItemBits;
  u32 packedStats[8];
  s32 i;
  s32 j;
  s32 n;
  s32 m;
  s32 position;
  s32 bit;

  levelBits = in[0] | in[1] << 8 | in[2] << 16 | (in[3] & 0xf0) << 20;
  djinnBits = (in[3] & 0x0f) | in[4] << 4 | in[5] << 12 | in[6] << 20;
  flagBits = in[7];
  keyItemBits = in[8];

  if (mode != 2) {
    s32 statsOffset = 8 + (mode != 0);

    for (i = 0; i != 2; i++) {
      packedStats[i * 4] = in[statsOffset] << 24 | in[statsOffset + 1] << 16 |
                           in[statsOffset + 2] << 8 | in[statsOffset + 3];
      packedStats[i * 4 + 1] = in[statsOffset + 4] << 24 |
                               in[statsOffset + 5] << 16 |
                               in[statsOffset + 6] << 8 | in[statsOffset + 7];
      packedStats[i * 4 + 2] =
          in[statsOffset + 7] << 28 | in[statsOffset + 8] << 20 |
          in[statsOffset + 9] << 12 | in[statsOffset + 10] << 4 |
          in[statsOffset + 11] >> 4;
      packedStats[i * 4 + 3] =
          in[statsOffset + 11] << 28 | in[statsOffset + 12] << 20 |
          in[statsOffset + 13] << 12 | in[statsOffset + 14] << 4;
      statsOffset += 15;
    }
    for (i = 0; i != 4; i++) {
      s16 *stats = &((struct BattleUnit *)Unit_Get(gData_020026a0[i]))->baseHp;
      s32 at = i * 8;

#define WORD (*(u32 *)((u8 *)packedStats + at))
      stats[0] = WORD >> 21;
      stats[1] = WORD << 11 >> 21;
      stats[4] = WORD & 0x1ff;
      at += 4;
      stats[5] = WORD >> 22 & 0x1ff;
      stats[6] = WORD >> 12 & 0x1ff;
      ((u8 *)stats)[14] = WORD >> 4 & 0x7f;
#undef WORD
    }
  }

  for (i = 0; i != 6; i++) {
    if ((flagBits >> i) & 1)
      GameFlag_SetBit(gData_02002490[i]);
    else
      GameFlag_ClearBit(gData_02002490[i]);
  }

  for (i = 0; i != 4; i++) {
    struct BattleUnit *unit = Unit_Get(gData_020026a0[i]);

    *(u8 *)&unit->level = levelBits >> (i * 7) & 0x7f;
#if TLA_PATCH_1
    unit->experience = Unit_GetExperienceForLevel(i, unit->level);
#endif
  }

  Djinn_GetRecoveryTable(0)->count = 0;
  for (i = 0; i != 4; i++) {
    struct BattleUnit *unit = Unit_Get(gData_020026a0[i]);

    // The native clears the same 40 bytes, djinnAvailable through
    // djinnActiveCounts, four times.
    for (n = 0; n != 4; n++)
      Iwram_ClearWordsEntry(unit->djinnAvailable, 40);
  }

  for (i = 0; i != 28; i++) {
    s32 element = i / 7;
    s32 number = i % 7;

    GameFlag_ClearBit(element * 20 + number + 48);
  }

  for (i = 0; i != 28; i++) {
    if ((djinnBits >> i) & 1)
      Djinn_GiveToParty(i / 7, i % 7);
  }

  if (mode != 0) {
    Unit_Get(gData_020026a0[0]);
    for (i = 0; i != 8; i++) {
      if ((keyItemBits >> i) & 1)
        PartyInventory_Add(gData_020026b0[i]);
    }
  }

  if (mode == 0) {
    position = 39;
    bit = 0;
    for (i = 0; i != 4; i++) {
      struct BattleUnit *unit = Unit_Get(gData_020026a0[i]);

      for (n = 0; n != 15; n++) {
        unit->inventory[n] =
            (in[position] << (bit + 1) | in[position + 1] >> (7 - bit)) & 0x1ff;
        bit++;
        position++;
        if (bit == 7) {
          bit = 0;
          position++;
        }
      }
    }

    position = 107;
    bit = -1;
    for (i = 0; i != 4; i++) {
      struct BattleUnit *unit = Unit_Get(gData_020026a0[i]);

      for (m = 0; m != 23; m++) {
        j = 0;
        if (bit < 0) {
          j = in[position] << -bit & 0x1f;
          position++;
          bit += 8;
        }
        j += in[position] >> bit & 0x1f;
        bit -= 5;
        if (bit == -5) {
          position++;
          bit = 3;
        }
        j &= 0x1f;
        for (n = 0; n != 15; n++) {
          if ((unit->inventory[n] & 0x1ff) == gData_020026c0[m])
            unit->inventory[n] |= j << 11;
        }
      }
    }

    gMainRoutineWork.coins = in[165] << 16 | in[166] << 8 | in[167];
  }

  for (i = 0; i != 4; i++) {
    Unit_UpdateClass(gData_020026a0[i]);
    Unit_Calculate(gData_020026a0[i]);
  }
}

extern u16 gData_02002700[];
extern s32 gData_020026f0[];
extern u16 gData_0200270c[];

s32 Func_02001c7c(s32 unused, s32 mode, u8 *out) {
  s32 size;
  u32 levels;
  u32 djinn;
  u8 found;
  u8 flags;
  u32 stats[8];
  s32 i;
  s32 pos;
  s32 bit;
  s32 j;
  s32 k;
  s32 n;
  s32 m;

  size = 11;
  switch (mode) {
  case 0:
    size = 173;
    break;
  case 1:
    size = 39;
    break;
  case 2:
    size = 9;
    break;
  }
  for (i = 0; i != size; i++)
    out[i] = 0;
  levels = 0;
  djinn = 0;
  found = 0;
  flags = 0;
  for (i = 0; i != 8; i++)
    stats[i] = 0;
  for (i = 0; i != 6; i++) {
    if (GameFlag_Test(gData_02002700[i]))
      flags |= 1 << i;
  }
  i = 0;
  pos = 4;
  for (; i != 4; i++) {
    struct BattleUnit *unit = Unit_Get(gData_020026f0[i]);
    struct UnitBaseStats *base = (struct UnitBaseStats *)&unit->baseHp;
    s32 value;

    if (base->hp > 1999)
      base->hp = 1999;
    value = (u16)base->hp;
    if ((s16)value < 0)
      base->hp = 0;
    if (base->pp > 1999)
      base->pp = 1999;
    value = (u16)base->pp;
    if ((s16)value < 0)
      base->pp = 0;
    if (base->attack > 999)
      base->attack = 999;
    value = base->attack;
    if (value < 0)
      value = 0;
    base->attack = value;
    if (base->defense > 999)
      base->defense = 999;
    value = base->defense;
    if (value < 0)
      value = 0;
    base->defense = value;
    if (base->agility > 999)
      base->agility = 999;
    value = base->agility;
    if (value < 0)
      value = 0;
    base->agility = value;
    if (base->luck > 99)
      base->luck = 99;
    value = base->luck;
    if (value < 0)
      value = 0;
    base->luck = value;
    stats[i * 2] = (u32)base->hp << 21 | base->pp << 10 | base->attack;
    *(u32 *)((u8 *)stats + pos) =
        (u32)base->defense << 22 | base->agility << 12 | base->luck << 4;
    if (unit->level > 99)
      unit->level = 99;
    value = unit->level;
    if (value == 0)
      unit->level = 1;
    levels |= unit->level << (i * 7);
    for (j = 0; j != 4; j++)
      djinn += unit->djinnAvailable[j] << (j * 7);
    for (j = 0; j != 15; j++) {
      s32 item = unit->inventory[j] & 0x1ff;

      for (k = 0; k != 8; k++) {
        if (item == gData_0200270c[k])
          found |= 1 << k;
      }
    }
    pos += 8;
  }
  if (mode == 0) {
    pos = 39;
    bit = 0;
    for (i = 0; i != 4; i++) {
      struct BattleUnit *unit = Unit_Get(gData_020026f0[i]);

      for (n = 0; n != 15; n++) {
        u16 item = unit->inventory[n] & 0x1ff;

        out[pos] += item >> (bit + 1);
        out[pos + 1] += item << (7 - bit);
        pos++;
        bit++;
        if (bit == 7) {
          bit = 0;
          pos++;
        }
      }
    }
    pos = 107;
    bit = -1;
    for (i = 0; i != 4; i++) {
      struct BattleUnit *unit = Unit_Get(gData_020026f0[i]);

      for (m = 0; m != 23; m++) {
        s32 count = 0;
        u16 quantity;
        /* FAKEMATCH(dead): a halfword copy of the count nothing reads, which
         * keeps the zero-extension hoisted above the sign test; without it, 139
         * of 479 instructions differ in tla-en. */

        for (n = 0; n != 15; n++) {
          if ((unit->inventory[n] & 0x1ff) == gData_0200271c[m])
            count = unit->inventory[n] >> 11;
        }
        quantity = count;
        if (bit < 0) {
          out[pos] += (u16)count >> -bit;
          bit += 8;
          pos++;
        }
        out[pos] += (u16)count << bit;
        bit -= 5;
        if (bit == -5) {
          pos++;
          bit = 3;
        }
      }
    }
    out[165] = (u32)gMainRoutineWork.coins >> 16;
    out[166] = (u32)gMainRoutineWork.coins >> 8;
    out[167] = gMainRoutineWork.coins;
  }
  if (mode != 2) {
    s32 statsOffset = 8 + (mode != 0);

    for (i = 0; i != 2; i++) {
      out[statsOffset] = stats[i * 4] >> 24;
      out[statsOffset + 1] = stats[i * 4] >> 16;
      out[statsOffset + 2] = stats[i * 4] >> 8;
      out[statsOffset + 3] = stats[i * 4];
      out[statsOffset + 4] = stats[i * 4 + 1] >> 24;
      out[statsOffset + 5] = stats[i * 4 + 1] >> 16;
      out[statsOffset + 6] = stats[i * 4 + 1] >> 8;
      out[statsOffset + 7] = stats[i * 4 + 1];
      out[statsOffset + 7] |= stats[i * 4 + 2] >> 28;
      out[statsOffset + 8] = stats[i * 4 + 2] >> 20;
      out[statsOffset + 9] = stats[i * 4 + 2] >> 12;
      out[statsOffset + 10] = stats[i * 4 + 2] >> 4;
      out[statsOffset + 11] = stats[i * 4 + 2] << 4;
      out[statsOffset + 11] |= stats[i * 4 + 3] >> 28;
      out[statsOffset + 12] = stats[i * 4 + 3] >> 20;
      out[statsOffset + 13] = stats[i * 4 + 3] >> 12;
      out[statsOffset + 14] = stats[i * 4 + 3] >> 4;
      statsOffset += 15;
    }
  }
  out[0] = levels;
  out[1] = levels >> 8;
  out[2] = levels >> 16;
  out[3] = ((levels >> 20) & 0xf0) | (djinn & 15);
  out[4] = djinn >> 4;
  out[5] = djinn >> 12;
  out[6] = djinn >> 20;
  out[7] = flags;
  if (mode != 0)
    out[8] = found;
  return size;
}

s32 Func_02002068(u8 *src, s32 length, u8 *dst) {
  u8 saved[260];
  s32 i;
  s32 shift = 2;
  s32 k = 0;
  s32 n = 0;
  s32 size = 11;
  u8 last;
  u8 c;

  if (length != 16) {
    size = 41;
    if (length != 61)
      size = 175;
  }
  for (i = 0; i != size; i++)
    dst[i] = 0;
  for (i = 0; i != length; i++) {
    saved[i] = src[i];
    src[i] = (saved[i] - i) & 63;
  }

  for (i = 0; i != length; i++, k++) {
    if (i % 10 > 8)
      continue;
    c = src[k];
    if (shift < 0) {
      dst[n] += c >> -shift;
      n++;
      if (n == size)
        break;
      shift += 8;
    }
    dst[n] += c << shift;
    shift -= 6;
    if (shift == -6) {
      shift = 2;
      n++;
      if (n == size)
        break;
    }
  }
  last = dst[n - 1];
  for (i = 0; i != n - 1; i++)
    dst[i] ^= last;
  for (i = 0; i != length; i++)
    src[i] = saved[i];
  return n;
}

/// The CRC-16 of length bytes at p (polynomial 0x1021, seed 0xffff, the
/// result inverted): the clear screen checks each decoded block with it,
/// over all but the block's last two bytes. The loops are the listing's,
/// counted up and tested at the bottom.
u16 Func_0200217c(u32 length, u8 *p) {
  u32 crc;
  u32 i;
  u32 j;

  crc = 0xffff;
  i = 0;
  if (length != 0) {
  next_byte:
    crc ^= *p << 8;
    j = 0;
  next_bit:
    // One bit: shift, adding the polynomial when the top bit was set.
    {
      u32 high = crc & 0x8000;
      u32 next = crc << 1;

      j++;
      crc = next + 0xffffefdf;
      if (high == 0)
        crc = next;
    }
    if (j != 8)
      goto next_bit;
    i++;
    p++;
    if (i != length)
      goto next_byte;
  }
  return ~crc;
}

u16 gData_02002410[] = {
    4,
    4,
    4,
    0,
};

// clang-format off
u16 gData_02002418[] = {
    64, 128, 64, 128, 32, 16, 32, 16,
#if (LANGUAGE == LANGUAGE_JAPANESE)
    64, 16, 128, 32, 64, 4, 0,
#else
    64, 16, 128, 32, 64, 4, 0, 0,
#endif
};

s8 gData_02002438[] = {
    0, 1, 2, 3, 3, 2, 1, 0,
};

// Alphabet table, absent in Japanese ("ABCDE FGHJK LMNPQ R" ...).
#if (LANGUAGE != LANGUAGE_JAPANESE)
u8 gData_02002440[] = {
    65, 66, 67, 68, 69, 32, 70, 71,
    72, 74, 75, 32, 76, 77, 78, 80,
    81, 32, 82, 0,
};

u8 gData_02002454[] = {
    83, 84, 85, 86, 87, 32, 88, 89,
    90, 50, 51, 32, 52, 53, 54, 55,
    56, 32, 57, 0,
};

u8 gData_02002468[] = {
    97, 98, 99, 100, 101, 32, 102, 103,
    104, 105, 106, 32, 107, 109, 110, 112,
    113, 32, 114, 0,
};

u8 gData_0200247c[] = {
    115, 116, 117, 118, 119, 32, 120, 121,
    122, 33, 63, 32, 35, 38, 36, 37,
    43, 32, 61, 0,
};
#endif

u16 gData_02002490[] = {
    0x941, 0x951, 0x8b3, 0x8d1, 0x81e, 0x868,
};
// clang-format on

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGERS_END},
};

u16 gData_020024f4[] = {
    0,
};

s16 gData_020024f6 = 0;

s16 gData_020024f8 = 0;

s16 gData_020024fa = 0;

s16 gData_020024fc = 0;

s16 gData_020024fe = 0;

s16 gData_02002500 = 0;
