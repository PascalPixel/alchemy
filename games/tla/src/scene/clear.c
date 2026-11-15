/// @file
/// The clear-data screen: Golden Sun's data brought over by password or link
/// cable (Clear_EnterPassword, Clear_ReceiveByLink) and applied to Isaac's
/// party (Clear_ApplyImportedData), with two secret button codes on its title
/// (Clear_CheckButtonCodes).
#include "system/task.h"
#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "system/bg_scroll.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/io_write_queue.h"
#include "system/main_routine_work.h"
#include "field/actor.h"
#include "system/ram_buffer.h"
#include "graphics/resource.h"
#include "field/event_inline.h"
#include "system/input.h"
#include "scene_ids.h"
#include "field/field_sequence_event.h"
#include "system/frame.h"
#include "system/heap_slots.h"
#include "scene/scene.h"
#include "field/map_grid.h"
#include "window/screens.h"
#include "window/window.h"
#include "system/serial.h"
#include "edition.h"
#include "version.h"
#include "system/iwram_call.h"
#include "field/call.h"
#include "field/field_sequence_event.h"
#include "window/window.h"
#include "graphics/uiwindow.h"
#include "graphics/oam.h"
#include "fixed_math.h"
#include "system/heap_release.h"
#include "battle/battle_unit.h"
#include "game/owner_state.h"
#include "macros.h"
#include "system/system_call.h"
#include "window/cursor.h"
#include "graphics/vram_tab.h"
#include "item_ids.h"
#include "field/field_sequence.h"

extern u16 gData_02003a74[];

/// A question's window, left open for the Yes/No menu below it.
#define QUESTION_MESSAGE                                                       \
  (FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN | FIELD_MESSAGE_LOWER)

/// The two secret button codes: unlocked, held and progress of each.
extern s16 gClear_CodeUnlocked;

extern u16 gClear_PasswordStackItems[];

extern u16 gClear_BlendFrame[];
extern s16 gClear_ExtraCodeUnlocked;
extern s16 gClear_CodeProgress;
extern s16 gClear_ExtraCodeProgress;
extern s16 gClear_CodeHeld;
extern s16 gClear_ExtraCodeHeld;
extern u16 gClear_ButtonCode[];
extern u16 gClear_ExtraButtonCode[];

s32 Clear_ReceiveByLink(void);

s32 Clear_EnterPassword(void);

extern s8 gClear_CursorWobble[];
void Clear_ApplyImportedData(s32 mode, u8 *data);
/// Decodes length symbols of an entered password (src) into dst, undoing The
/// Broken Seal's FileScreen_EncodePassword: each symbol less its place, the
/// checksum symbols skipped, six bits a symbol, then every byte but the last
/// XORed with the last. Returns the bytes written.
s32 Clear_DecodePassword(u8 *src, s32 length, u8 *dst);
extern u16 gClear_ImportedFlags[];

struct Window;

void DataTransfer_Apply(void *data, s32 a, s32 b);
void DataTransfer_GiveDefaultDjinn(void);
void ClearData_Apply(void *data);
extern u8 gClear_FirstPasswordRow[];
extern u8 gClear_SecondPasswordRow[];
extern u8 gClear_ThirdPasswordRow[];
extern u8 gClear_PasswordText[];
extern u8 gClear_PasswordWork[];
extern u8 gClear_PasswordCodes[];
void *Glyph_AddBlankSprite(struct Window *window, void *glyph, s32 x, s32 y);

extern u8 gClear_PasswordGrid[];
extern u8 Data_02012f94[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// The logo background, as the title's Title_LoadBackground, then the party
/// leader's motion flags cleared.
// As ☀️'s Clear_LoadBackground.
void Clear_LoadBackground(void) {
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
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6);
  gBgScroll[2].y = 0;
  blank = 0x1ff;
  res = Resource_GetTableEntry(id);
  Dma_Set(res, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  res += 0x1c0;
  FieldDecompress_TaggedLz(res, Ram_MapCellBuffer);
  Dma_Set((void *)Ram_MapCellBuffer, BG_SCREEN_BLOCK(13),
          DMA_COPY_WORDS(0x9600 / 4), REG_DMA3);

  map = (u16 *)BG_SCREEN_BLOCK(6);
  tile = 0x1a0;
  for (y = 0; y < 20; y++) {
    for (x = 0; x < 30; x++)
      *map++ = tile++;
    *map++ = blank;
    *map++ = blank;
  }
  scroll = gBgScroll;
  for (y = 0; y <= 3; y++) {
    scroll->y = 0;
    scroll->x = 0;
    scroll++;
  }
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  Ram_HeapSlots->mapGrid->dispcnt = DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  Actor_Get(gMainRoutineWork.leaderActorId)->motionFlags = 0;
}

/// Queues an alpha blend of BG0 and the sprites over the rest whose first
/// weight rises by one every second frame (the second weight 16 less), and
/// removes the callback once the first weight passes 15.
// As ☀️'s Clear_UpdateBlend.
void Clear_FadeInBlend(void) {
  s32 frame;
  u32 level;

  frame = gClear_BlendFrame[0] + 1;
  gClear_BlendFrame[0] = frame;
  level = (u16)frame >> 1;
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT1_BG0 | BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND |
                          BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 |
                          BLDCNT_TGT2_BD,
                      IO_WRITE_HALFWORD);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - (u16)level) << 8) | (u16)level,
                      IO_WRITE_HALFWORD);
  if (level > 15)
    FieldTask_Remove(Clear_FadeInBlend);
}

/// Follows the two secret button codes: each held key that matches the next
/// key of a code advances it, a wrong key starts it again, and finishing a
/// code unlocks it with a sound.
void Clear_CheckButtonCodes(void) {
  if (gClear_CodeUnlocked == 0) {
    if (gClear_CodeHeld != 0) {
      if (gInput.held == 0)
        gClear_CodeHeld = 0;
    } else if (gInput.held != 0) {
      if (gInput.held == gClear_ButtonCode[gClear_CodeProgress]) {
        gClear_CodeProgress++;
        gClear_CodeHeld = 1;
        if (gClear_ButtonCode[gClear_CodeProgress] == 0) {
          gClear_CodeUnlocked = 1;
          Event_PlaySound(SE_CHIME);
        }
      } else {
        gClear_CodeProgress = 0;
      }
    }
  }
  if (gClear_ExtraCodeUnlocked == 0) {
    if (gClear_ExtraCodeHeld != 0) {
      if (gInput.held == 0)
        gClear_ExtraCodeHeld = 0;
    } else if (gInput.held != 0) {
      if (gInput.held == gClear_ExtraButtonCode[gClear_ExtraCodeProgress]) {
        gClear_ExtraCodeProgress++;
        gClear_ExtraCodeHeld = 1;
        if (gClear_ExtraButtonCode[gClear_ExtraCodeProgress] == 0) {
          gClear_ExtraCodeUnlocked = 1;
          Event_PlaySound(SE_CHIME);
        }
      } else {
        gClear_ExtraCodeProgress = 0;
      }
    }
  }
}

s32 Clear_CanRetreat(void) {
  struct MainRoutineWork *party;
  s32 mode;

  if (Event_GetFlag(FLAG_RETREAT_BLOCKED) == 0)
    return 0;
  party = &gMainRoutineWork;

  if (party->sceneType == 2)
    return 0;
  mode = party->next ^ SCENE_WORLD_MAP;
  return -NONZERO(mode);
}

#include "text/msg_ids.h"

// The play time the save kept in the scene's own state, which a sound save
// matches.
#define gClearSavedPlayTime (*(u32 *)gSceneLocalState)

s32 Save_ChooseAndLoadFile(s32 mode);
void Save_CopyFile(void);
u32 BuildStamp_GetTime(void);
void InventorySnapshot_Restore(void);
void StatusScreen_Run(void);

#define MAP_CELL_BACKUP (Ram_MapCellBuffer + 0x3000)

#if (LANGUAGE == LANGUAGE_JAPANESE)
#define YEAH_WIDTH 6
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define YEAH_WIDTH 10
#else
#define YEAH_WIDTH 8
#endif

/// Swaps the map cell buffer out around a password or link screen.
#define COPY_CELLS(to, from) Iwram_CopyWords((s32)(to), (s32)(from), 0x3000)
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
    (party)->next = (party)->restartScene;                                     \
    (party)->entrance = (party)->restartEntrance;                              \
    WorldMap_SetReturnLeavingScene((party)->next, (party)->entrance);          \
    Event_ClearFlag(FLAG_KEEP_PARTY_POSITION);                                 \
  }

s32 Scene_Initialize(void) {
  s32 started = 0;
  u32 mask;
  s32 result, i, limit, status, message, failure, recovery;

  Clear_LoadBackground();
  Task_Add(Clear_CheckButtonCodes, TASK_PASS_VBLANK);
  gFieldWork->transition = started;
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
      gMainRoutineWork.sendFlag = 1;
      Party_AddMember(5);
      Party_AddMember(6);
      Party_AddMember(7);
      Party_AddMember(1);
      Party_AddMember(2);
      Party_AddMember(3);
      i = Save_ToChosenFile();
      if (i == -1)
        goto ask_save;
    }
    Event_Wait(60);
    Event_FinishScreenTransition();
    Sound_Play(SOUND_FADE_OUT_BGM);
    Event_Wait(300);
    Event_SetDestination(SCENE_TITLE, 1);
    return 0;
  }
  gSleepDisabled = started;
top:
  result = Save_ScanFileSummaries();
  if (result < 0 && gDebugMode) {
    gMainRoutineWork.idleSleepEnabled = 1;
    gIdleSleepEnabled = 1;
    FieldMessage_ShowCentered(SCENE_DAILA_INTERIORS_HOUSES, 1, 8);
  }
  if (result == 0 && !started) {
    wait(30);
    Task_Add(Clear_FadeInBlend, TASK_PASS_VBLANK);
    wait(1);
    started = 1;
  }
  if (result > 0)
    result = FileList_SelectAction();
  else
    result = 0;
  if (result == 0) {
    MainRoutineWork_InitializeDefaults();
    WindowColor_Apply(gMainRoutineWork.windowHue,
                      gMainRoutineWork.windowBrightness);
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
    if (gClear_CodeUnlocked != 0 || gClear_ExtraCodeUnlocked != 0)
      limit = 8;
    if (++i < limit)
      goto open;
  ask:
    // "Transfer data from Golden Sun?"
    FieldMessage_Show((s32)MsgTransferDataFromGoldenSun, QUESTION_MESSAGE);
    result = YesNoMenu_Run(1, 0, 0, 0);
    Message_CloseFinishedWindows();
    if (result == 0) {
    enter:
      result = IconMenu_ChooseSendOption(0);
      if (result == -1)
        goto top;
      if (result == 1) {
        SAVE_CELLS();
        status = Clear_ReceiveByLink();
        if (status != -1)
          StatusScreen_Run();
        RESTORE_CELLS()
        if (status == -1)
          goto enter;
        DataTransfer_Apply(MAP_CELL_BACKUP, 0, 0);
      } else if (result == 0) {
        SAVE_CELLS();
        MainRoutineWork_InitializeDefaults();
#if !TLA_PATCH_2
        status = Clear_EnterPassword();
        RESTORE_CELLS()
        if (status == -1)
          goto enter;
#else
        Clear_EnterPassword();
        RESTORE_CELLS()
#endif
        DataTransfer_Apply(MAP_CELL_BACKUP, 1, 0);
      }
    } else {
      DataTransfer_GiveDefaultDjinn();
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
    if (gClear_ExtraCodeUnlocked != 0)
      limit = 4;
    if (++i < limit)
      goto close;
    if (gAnyFileCleared != 0) {
    save:
      message = (s32)MsgPlayOnEasyLevel;
      FieldMessage_Show(message, QUESTION_MESSAGE);
      i = YesNoMenu_Run(1, (s32)MsgYeah, YEAH_WIDTH, 0);
      Message_CloseFinishedWindows();
      if (i == 0) {
        if (gAnyFileCleared == 0) {
          FieldMessage_Show(message + 1, 1);
        } else {
          SAVE_CELLS();
          result = Save_ChooseAndLoadFile(5);
          if (result != 0)
            goto save;
          {
            struct MainRoutineWork *party = &gMainRoutineWork;

            WindowColor_Apply(party->windowHue, party->windowBrightness);
            RESTORE_CELLS()
            ClearData_Apply(MAP_CELL_BACKUP);
            party->difficulty = 1;
          }
          goto leave;
        }
      }
      // "Play on HARD level? (Monsters are stronger)"
      FieldMessage_Show((s32)MsgPlayOnHardLevel, QUESTION_MESSAGE);
      i = YesNoMenu_Run(1, (s32)MsgYeah, YEAH_WIDTH, 0);
      Message_CloseFinishedWindows();
      if (i == 0) {
        Event_SetFlag(FLAG_HARD_LEVEL);
        gMainRoutineWork.difficulty = 2;
        goto leave;
      }
      // "Play on Normal level? (Default difficulty)"
      FieldMessage_Show((s32)MsgPlayOnNormalLevel, QUESTION_MESSAGE);
      i = YesNoMenu_Run(1, (s32)MsgYeah, YEAH_WIDTH, 0);
      Message_CloseFinishedWindows();
      if (i != 0)
        goto save;
    }
  leave:
    Event_SetDestination(SCENE_TITLE, 8);
    goto finish;
  }
  if (result == 1) {
    result = Save_ChooseAndLoadFile(1);
    if (result == -1)
      goto top;
    Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
    {
      struct MainRoutineWork *party = &gMainRoutineWork;

      WindowColor_Apply(party->windowHue, party->windowBrightness);
      if (party->buildStamp != BuildStamp_GetTime()) {
        RETURN_TO_SAVED(party)
      } else if (mask = 0x208, (gInput.held & mask) == mask) {
        if (Clear_CanRetreat()) {
          failure = (s32)MsgDataCouldNotBeRecovered;
          goto refuse;
        }
        RETURN_TO_SAVED(party)
        Event_SetFlag(FLAG_SAVE_RESTARTED_AT_SANCTUM);
      } else if (party->playTime != gClearSavedPlayTime) {
        recovery = (s32)MsgSomeDataIsCorrupted;
        FieldMessage_Show(recovery, FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
        FieldMessage_Show(recovery + 1, QUESTION_MESSAGE);
        if (YesNoMenu_Run(1, 0, 0, 0) != 0) {
          Message_CloseFinishedWindows();
          goto top;
        }
        Message_CloseFinishedWindows();
        if (Clear_CanRetreat()) {
          failure = recovery + 2;
        refuse:
          FieldMessage_Show(failure, FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
          goto top;
        }
        RETURN_TO_SAVED(party)
        Event_SetFlag(FLAG_SAVE_RECOVERED_AT_SANCTUM);
      }
    }
    Event_ClearFlag(FLAG_MENU_OPEN);
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

      party->leaderActorId = 4;
      if (Event_GetFlag(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT))
        InventorySnapshot_Restore();
      WindowColor_Apply(party->windowHue, party->windowBrightness);
      Event_ClearFlag(FLAG_KEEP_PARTY_POSITION);
      Event_ClearFlag(FLAG_MENU_OPEN);
      party->ownerOverride = 0;
      gSleepDisabled = 1;
      Event_SetDestination(SCENE_LOBBY, 1);
    }
    goto finish;
  }
  if (result != 5)
    goto top;
link:
  result = Save_ChooseAndLoadFile(6);
  if (result == -1)
    goto top;
  WindowColor_Apply(gMainRoutineWork.windowHue,
                    gMainRoutineWork.windowBrightness);
again:
  result = IconMenu_ChooseSendOption(2);
  if (result == -1)
    goto link;
  if (result == 1) {
    SAVE_CELLS();
    status = Clear_ReceiveByLink();
    if (status != -1)
      StatusScreen_Run();
    RESTORE_CELLS()
    if (status == -1)
      goto again;
    DataTransfer_Apply(MAP_CELL_BACKUP, 0, 1);
  } else if (result == 0) {
    SAVE_CELLS();
    MainRoutineWork_InitializeDefaults();
    status = Clear_EnterPassword();
    RESTORE_CELLS()
    if (status == -1)
      goto again;
    DataTransfer_Apply(MAP_CELL_BACKUP, 1, 1);
  }
  if (gClear_CodeUnlocked != 0 || gClear_ExtraCodeUnlocked != 0) {
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
    if (gClear_ExtraCodeUnlocked != 0)
      limit = 4;
    if (++i < limit)
      goto shut;
  }
#if TLA_PATCH_1
  Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
#endif
finish:
  gFieldWork->exit = EXIT_KEEP_DESTINATION;
  Event_Wait(30);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(60);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The places of the password level menu (RESOURCE_LAYOUT_PASSWORD_LEVEL), as
/// The Broken Seal's file screen numbers them.
enum PasswordLevel {
  PASSWORD_LEVEL_GOLD = 0,
  PASSWORD_LEVEL_SILVER = 1,
  PASSWORD_LEVEL_BRONZE = 2
};

/// The symbols of each level's password: The Broken Seal packs 173, 39 or 9
/// bytes (FileScreen_PackPasswordData), then two of its checksum, six bits a
/// symbol and a tenth symbol after every nine.
#define PASSWORD_GOLD_LENGTH 260
#define PASSWORD_SILVER_LENGTH 61
#define PASSWORD_BRONZE_LENGTH 16
/// The bytes each level decodes to, its checksum's two included.
#define PASSWORD_GOLD_BYTES 175
#define PASSWORD_SILVER_BYTES 41
#define PASSWORD_BRONZE_BYTES 11
#define PASSWORD_SYMBOLS_PER_PAGE 50 ///< five rows of ten
#define PASSWORD_SYMBOLS_PER_ROW 10
#define PASSWORD_SYMBOL_MASK 0x3f ///< six bits: 64 symbols
/// A place of the entered password that holds no symbol yet.
#define PASSWORD_SYMBOL_EMPTY 99

/// The password grid's glyph sprites, one for each place of a page.
struct PasswordWork {
  void *glyphs[PASSWORD_SYMBOLS_PER_PAGE];
  void *cells[PASSWORD_SYMBOLS_PER_PAGE];
  u8 shown[PASSWORD_SYMBOLS_PER_PAGE]; ///< the symbol each place shows
  u8 unused[26];                       ///< cleared on entry, read by nothing
};

#if (LANGUAGE == LANGUAGE_JAPANESE)
/// The character a cell of the password grid stands for, in the Japanese
/// font's codes: kana, the voiced ones with the dakuten mark in the high byte.
void Clear_GetPasswordCharacter(s32 cell, u16 *text) {
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
void Clear_GetPasswordCharacter(s32 cell, u16 *text) {
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
s32 Clear_ReceiveByLink(void) {
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
  if (!(gInput.pressed & KEY_B))
    goto scan;
  Event_PlaySound(SE_MENU_CANCEL);
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
  if (gSerialReceiveDestination != 0) {
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
    if (!(gInput.pressed & KEY_A))
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
  FieldWindowColor_Apply(gMainRoutineWork.windowHue,
                         gMainRoutineWork.windowBrightness);
  return 1;
}

/// This overlay's copy of Window_SetTilemapFlagRect (window/fixed_block.c):
/// sets the field in the tilemap flags of a rectangle of a window, clipped to
/// the screen.
void Clear_DrawGridCell(const struct Window *object, s32 x, s32 y, s32 width,
                        s32 height, u32 field) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;

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

extern u8 gClear_FourthPasswordRow[];

extern u8 MsgFieldPasswordKana[];

void Text_DrawMessageAsTiles(s32 character, struct RenderInput *position,
                             u32 offsetX, u32 offsetY);
u16 Clear_CalculatePasswordCrc16(u32 length, u8 *p);
void Resource_CopyFixedBlockB(void);
void Resource_CopyFixedBlockA(void);
void Graphics_ExpandVramTilesByColorTable(void *vram);
void MenuCursor_Advance(struct MenuCursor *cursor);
void MenuCursor_Set(struct MenuCursor *cursor, s32 x, s32 y, s32 kind);
void MenuCursor_SetPositionImmediate(struct MenuCursor *cursor, s32 x, s32 y);

/// Gives a cursor its sprite.
static inline void Cursor_Attach(struct MenuCursor *cursor,
                                 struct WindowSprite *sprite) {
  cursor->anchor = sprite;
}

/// Sets a cursor where it starts.
static inline void Cursor_Place(struct MenuCursor *cursor, s32 x, s32 y) {
  MenuCursor_SetPositionImmediate(cursor, x, y);
}

/// Moves a cursor and shows it.
static inline void Cursor_Show(struct MenuCursor *cursor, s32 x, s32 y) {
  MenuCursor_Set(cursor, x, y, 1);
  MenuCursor_Advance(cursor);
}

#if (LANGUAGE == LANGUAGE_JAPANESE)
/// The Japanese grid has 20 columns of kana and its side column is 20.
#define SIDE_COLUMN 20
#define SIDE_HIGHLIGHT_WIDTH 3
#define CHECK_WINDOW_X 8
#define CHECK_WINDOW_WIDTH 14
/// Japanese decodes into the bytes after its 260-byte entered password.
#define DECODED_PASSWORD gClear_PasswordCodes
#define CURSOR_ON_SIDE_COLUMN(column) ((column) == SIDE_COLUMN)
#else
#define SIDE_COLUMN 16
#define SIDE_HIGHLIGHT_WIDTH 6
#define CHECK_WINDOW_X 5
#define CHECK_WINDOW_WIDTH 20
#define DECODED_PASSWORD gClear_PasswordWork
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
    Text_DrawStringInWindow(gClear_FirstPasswordRow, window, 0, 0);            \
    Text_DrawStringInWindow(gClear_SecondPasswordRow, window, 0, 8);           \
    Text_DrawStringInWindow(gClear_ThirdPasswordRow, window, 0, 16);           \
    Text_DrawStringInWindow(gClear_FourthPasswordRow, window, 0, 24);          \
    message = MSGNO(MsgFieldSpace);                                            \
    Window_DrawDividerLine(window, 20, 0, 20, 5);                              \
    Text_DrawMessageAsTiles(message++, window, 160, 0);                        \
    Text_DrawMessageAsTiles(message++, window, 160, 8);                        \
    Text_DrawMessageAsTiles(message, window, 160, 24);                         \
  }
#endif

s32 Clear_EnterPassword(void) {
  struct MenuCursor cursorStore;
  struct MenuCursor gridStore;
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
  s32 crcLength;
  s32 entry;
  struct WindowSprite *sprite;
  u16 *palette;
  s32 height;

  Window_LoadGraphics();
  WindowColor_Apply(gMainRoutineWork.windowHue,
                    gMainRoutineWork.windowBrightness);
  palette = (u16 *)&BG_PLTT_COLOR(14, 0);
  Dma_Set((const void *)&BG_PLTT_COLOR(15, 0), palette, DMA_COPY_WORDS(8),
          REG_DMA3);
  palette[4] = 0x6318;
  mode = IconMenu_ChooseSendOption(RESOURCE_LAYOUT_PASSWORD_LEVEL);
  if (mode == -1)
    return -1;
  Sound_Play(MUS_PASSWORD);
  cursor = 0;
  column = 0;
  row = 0;
  work = Heap_AllocEwram(sizeof(struct PasswordWork));
  advance = 0;
  Graphics_ExpandVramTilesByColorTable((void *)0x06006000);
  if (mode == PASSWORD_LEVEL_GOLD) {
    length = PASSWORD_GOLD_LENGTH;
  } else {
    length = PASSWORD_SILVER_LENGTH;
    if (mode != PASSWORD_LEVEL_SILVER)
      length = PASSWORD_BRONZE_LENGTH;
  }
  pages = length / PASSWORD_SYMBOLS_PER_PAGE + 1;
  window = Window_Create(1, 13, 29, 6, 2);
  DRAW_TITLE(window)
  grid = Window_Create(5, 1, 20, 12, 2);
  for (i = 0; i != ARRAY_COUNT(work->unused); i++)
    work->unused[i] = 0;
  for (i = 0; i != PASSWORD_GOLD_LENGTH; i++)
    gClear_PasswordText[i] = PASSWORD_SYMBOL_EMPTY;
  for (i = 0; i != PASSWORD_SYMBOLS_PER_PAGE; i++) {
    s32 col = i % PASSWORD_SYMBOLS_PER_ROW;
    s32 y;
    s32 x;

    x = col * 12 + 8;
    y = i / PASSWORD_SYMBOLS_PER_ROW * 16 + 2;
    if (col > 4)
      x = col * 12 + 18;
    work->cells[i] = Glyph_AddBlankSprite(grid, work->glyphs[i], x, y);
    work->shown[i] = PASSWORD_SYMBOL_EMPTY;
    Iwram_ClearWordsEntry(work->cells[i], 128);
  }
  entry = VramCache_FindFreeEntry();
  if (entry < VRAM_CACHE_ENTRY_COUNT) {
    Resource_CopyFixedBlockB();
    sprite = WindowSprite_Create(entry, OUTPUT_SIZE_16X16,
                                 (struct RenderInput *)window, 0, 0);
    Cursor_Attach(&cursorStore, sprite);
    Cursor_Place(&cursorStore, (s16)window->x * 8 - 2, (s16)window->y * 8 + 12);
  }
  entry = VramCache_FindFreeEntry();
  if (entry < VRAM_CACHE_ENTRY_COUNT) {
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
    Clear_DrawGridCell(window, drawColumn, drawRow, drawMode, height, 15);
    if (state == 0) {
      if (gInput.repeat & KEY_RIGHT) {
        Sound_Play(SE_MENU_CURSOR_MOVE);
        {
          s32 next = column + 1;

          if (row == 2)
            column = next % SIDE_COLUMN;
          else
            column = next % (SIDE_COLUMN + 1);
        }
      }
      if (gInput.repeat & KEY_LEFT) {
        Sound_Play(SE_MENU_CURSOR_MOVE);
        if (row == 2)
          column = (column + SIDE_COLUMN - 1) % SIDE_COLUMN;
        else
          column = (column + SIDE_COLUMN) % (SIDE_COLUMN + 1);
      }
      if (gInput.repeat & KEY_DOWN) {
        Sound_Play(SE_MENU_CURSOR_MOVE);
        row = (row + 1) % 4;
        if (column == SIDE_COLUMN && row == 2)
          row = 3;
      }
      if (gInput.repeat & KEY_UP) {
        Sound_Play(SE_MENU_CURSOR_MOVE);
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
    Clear_DrawGridCell(window, drawColumn, row, drawMode, height, 14);
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
      s32 cell = cursor % PASSWORD_SYMBOLS_PER_PAGE;
      s32 y = (cell / PASSWORD_SYMBOLS_PER_ROW * 2 + (s16)grid->y) * 8 - 2;
      s32 col;
      s32 x;

      frame = (gVBlankCount >> 1) & 7;
      col = cell % PASSWORD_SYMBOLS_PER_ROW;
      x = col * 12 + (s16)grid->x * 8 + 16;

      if (col > 4)
        x += 10;
      x += gClear_CursorWobble[frame];
      y += gClear_CursorWobble[(frame + 5) & 7];
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
          s32 code = gClear_PasswordGrid[column + row * SIDE_COLUMN];

          if (code <= PASSWORD_SYMBOL_MASK) {
            Sound_Play(SE_MENU_CONFIRM);
            gClear_PasswordText[cursor] = code;
            __modsi3(cursor, PASSWORD_SYMBOLS_PER_ROW);
            if (cursor < length - 1) {
              if (cursor % PASSWORD_SYMBOLS_PER_PAGE ==
                  PASSWORD_SYMBOLS_PER_PAGE - 1)
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
        if (gInput.pressed & KEY_START || action == 3 || state == 1) {
          u16 check;
          struct Window *box;
          struct MainRoutineWork *party;
          s16 crc;

          action = Clear_DecodePassword(gClear_PasswordText, length,
                                        DECODED_PASSWORD);
          crcLength = action - 2;
          check = Clear_CalculatePasswordCrc16(crcLength, DECODED_PASSWORD);

          state = 0;
          box = Window_Create(CHECK_WINDOW_X, 12, CHECK_WINDOW_WIDTH, 3, 2);
          crc = check;
          if (DECODED_PASSWORD[crcLength] == (u16)crc >> 8 &&
              DECODED_PASSWORD[action - 1] == (u16)crc % 256) {
            Text_DrawMessageAsTiles(MSGNO(MsgFieldPasswordAccepted), box, 0, 0);
            Sound_Play(SE_MENU_CONFIRM);
          wait_accepted:
            System_Wait(1);
            if (!(gInput.pressed & KEY_A))
              goto wait_accepted;
            Sound_Play(SE_MENU_CONFIRM);
            MainRoutineWork_InitializeDefaults();
            party = &gMainRoutineWork;
            party->leaderActorId = 0;
            Party_RemoveMember(4);
            Party_RemoveMember(5);
            Party_RemoveMember(6);
            Party_RemoveMember(7);
            Party_AddMember(0);
            Party_AddMember(1);
            Party_AddMember(2);
            Party_AddMember(3);
            Clear_ApplyImportedData(mode, DECODED_PASSWORD);
            Window_Close(box, 2);
            Window_Close(grid, 2);
            Window_Close(window, 2);
            WindowColor_Apply(party->windowHue, party->windowBrightness);
            Sound_Play(MUS_CLEAR);
            System_Wait(6);
            NameEntry_EditMemberName(0);
            StatusScreen_Run();
            Iwram_CopyWords(Data_02012f94 + 0xac, gGameFlagBytes, 0xf40);
            return 0;
          }
          Sound_Play(SE_MENU_CANCEL);
          Text_DrawMessageAsTiles(MSGNO(MsgFieldInvalidPassword), box, 0, 0);
        wait_rejected:
          System_Wait(1);
          if (!(gInput.pressed & KEY_A) && !(gInput.pressed & KEY_B))
            goto wait_rejected;
          Window_Close(box, 2);
          WindowSprite_RedrawSavedRect(window);
          DRAW_TITLE(window)
        } else if (gInput.repeat & KEY_L || action == 2) {
          if (cursor > 0) {
            Sound_Play(SE_MENU_CURSOR_MOVE);
            cursor--;
          }
        } else if (gInput.repeat & KEY_R || action == 1) {
          if (cursor < length - 1) {
            Sound_Play(SE_MENU_CURSOR_MOVE);
            cursor++;
          }
        } else if (gInput.pressed & KEY_B) {
          if (cursor > 0) {
            Sound_Play(SE_MENU_CURSOR_MOVE);
            cursor--;
          } else {
            Sound_Play(SE_MENU_CANCEL);
            Window_Close(grid, 2);
            Window_Close(window, 2);
            Sound_Play(MUS_CLEAR);
            return -1;
          }
        }
      }
    }
    page = cursor / PASSWORD_SYMBOLS_PER_PAGE;
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
    /* FAKEMATCH(barrier): the count is not known to be the length. */
    asm volatile("" : "+r"(action));
    if (length > PASSWORD_SYMBOLS_PER_PAGE - 1) {
      if (pages - 1 == page) {
        /* FAKEMATCH(barrier): r2 no longer holds the length. */
        asm volatile("" : : : "r2");
        action = length % PASSWORD_SYMBOLS_PER_PAGE;
      } else
        action = PASSWORD_SYMBOLS_PER_PAGE;
    }
    for (i = 0; i != action; i++) {
      u8 *entered = gClear_PasswordText;
      s32 index = i + page * PASSWORD_SYMBOLS_PER_PAGE;
      s32 raw = entered[index];
      u8 code = raw;

      if (code != work->shown[i]) {
        work->shown[i] = raw;
        if (code == PASSWORD_SYMBOL_EMPTY) {
          Iwram_ClearWordsEntry(work->cells[i], 128);
        } else {
          Clear_GetPasswordCharacter(entered[index], glyph);
          Glyph_DrawToTiles(glyph[0], work->cells[i]);
        }
      }
    }
    for (; i != PASSWORD_SYMBOLS_PER_PAGE; i++) {
      if (work->shown[i] != PASSWORD_SYMBOL_EMPTY) {
        work->shown[i] = PASSWORD_SYMBOL_EMPTY;
        Iwram_ClearWordsEntry(work->cells[i], 128);
      }
    }
    System_Wait(1);
  }
}

/// Unpacks the tiles into a scratch block, then fills a 16 by 8 block
/// of the window tilemap, in VRAM and in its shadow, at the window's cell: each
/// cell's tile the row times the window's width plus the column.
UNUSED void Clear_DrawPanel(struct Window *window,
                            const void *compressedTiles) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  u16 *shadow;
  u16 *vram;
  void *decodedTiles;
  s32 row;
  s32 col;
  s32 cell;

  decodedTiles = Heap_AllocEwram(768);
  Decompress_TaggedLz(compressedTiles, decodedTiles);
  cell = (s16)window->y * 32 + (s16)window->x;
  vram = (u16 *)WINDOW_TILEMAP_VRAM + cell;
  shadow = &work->tilemap[0][cell];
  for (row = 0; row <= 7; row++) {
    for (col = 0; col <= 15; col++) {
      s16 tile = WINDOW_TILE(window->width * row + col);

      *vram++ = tile;
      *shadow++ = tile;
    }
    vram += 16;
    shadow += 16;
  }
  Heap_Release(decodedTiles);
}

/// Fills a 16 by 8 block of the window tilemap with the tiles from 32
/// on.
UNUSED void Clear_DrawLowerPanel(struct Window *window) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  u16 *shadow;
  u16 *vram;
  void *buffer;
  s32 row;
  s32 col;
  s32 cell;

  buffer = Heap_AllocEwram(768);
  cell = (s16)window->y * 32 + (s16)window->x;
  vram = (u16 *)WINDOW_TILEMAP_VRAM + cell;
  shadow = &work->tilemap[0][cell];
  for (row = 0; row <= 7; row++) {
    for (col = 0; col <= 15; col++) {
      s16 tile = WINDOW_TILE(row * 16 + col + 32);

      *vram++ = tile;
      *shadow++ = tile;
    }
    vram += 16;
    shadow += 16;
  }
  Heap_Release(buffer);
}

extern s32 gClear_ImportedMembers[];
extern u16 gClear_ImportedKeyItems[];
extern u16 gClear_ImportedStackItems[];

s32 Unit_GetExperienceForLevel(s32 id, s32 level);

/// Applies a decoded password of level mode (enum PasswordLevel) to Isaac's
/// party: the unpacking of The Broken Seal's FileScreen_PackPasswordData.
void Clear_ApplyImportedData(s32 mode, u8 *in) {
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

  if (mode != PASSWORD_LEVEL_BRONZE) {
    s32 statsOffset = 8 + (mode != PASSWORD_LEVEL_GOLD);

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
      s16 *stats = &Unit_Get(gClear_ImportedMembers[i])->baseHp;
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
      GameFlag_Set(gClear_ImportedFlags[i]);
    else
      GameFlag_Clear(gClear_ImportedFlags[i]);
  }

  for (i = 0; i != 4; i++) {
    struct Unit *unit = Unit_Get(gClear_ImportedMembers[i]);

    *(u8 *)&unit->level = levelBits >> (i * 7) & 0x7f;
#if TLA_PATCH_1
    unit->experience = Unit_GetExperienceForLevel(i, unit->level);
#endif
  }

  Djinn_GetRecoveryTable(0)->list.count = 0;
  for (i = 0; i != 4; i++) {
    struct Unit *unit = Unit_Get(gClear_ImportedMembers[i]);

    for (n = 0; n != 4; n++)
      Iwram_ClearWordsEntry(unit->djinnHeld, 40);
  }

  for (i = 0; i != 28; i++) {
    s32 element = i / 7;
    s32 number = i % 7;

    GameFlag_Clear(element * 20 + number + 48);
  }

  for (i = 0; i != 28; i++) {
    if ((djinnBits >> i) & 1)
      Djinn_GiveToParty(i / 7, i % 7);
  }

  if (mode != PASSWORD_LEVEL_GOLD) {
    Unit_Get(gClear_ImportedMembers[0]);
    for (i = 0; i != 8; i++) {
      if ((keyItemBits >> i) & 1)
        PartyInventory_Add(gClear_ImportedKeyItems[i]);
    }
  }

  if (mode == PASSWORD_LEVEL_GOLD) {
    position = 39;
    bit = 0;
    for (i = 0; i != 4; i++) {
      struct Unit *unit = Unit_Get(gClear_ImportedMembers[i]);

      for (n = 0; n != 15; n++) {
        unit->inventory[n] =
            (in[position] << (bit + 1) | in[position + 1] >> (7 - bit)) &
            INVENTORY_ITEM_ID_MASK;
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
      struct Unit *unit = Unit_Get(gClear_ImportedMembers[i]);

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
          if ((unit->inventory[n] & INVENTORY_ITEM_ID_MASK) ==
              gClear_ImportedStackItems[m])
            unit->inventory[n] |= j << INVENTORY_QUANTITY_SHIFT;
        }
      }
    }

    gMainRoutineWork.coins = in[165] << 16 | in[166] << 8 | in[167];
  }

  for (i = 0; i != 4; i++) {
    Unit_UpdateClass(gClear_ImportedMembers[i]);
    Unit_Calculate(gClear_ImportedMembers[i]);
  }
}

extern u16 gClear_PasswordFlags[];
extern s32 gClear_PasswordMembers[];
extern u16 gClear_PasswordKeyItems[];

/// Unused: packs the four heroes' data for a password of level mode (enum
/// PasswordLevel) as The Broken Seal's file screen does
/// (FileScreen_PackPasswordData), and returns the byte count. Nothing in this
/// game calls it.
UNUSED s32 Clear_PackPasswordData(s32 unused, s32 mode, u8 *out) {
  s32 length;
  u32 levelBits;
  u32 djinnBits;
  u8 keyItemBits;
  u8 flagBits;
  u32 packedStats[8];
  s32 i;
  s32 outputIndex;
  s32 bitOffset;
  s32 j;
  s32 k;
  s32 n;
  s32 m;

  length = 11;
  switch (mode) {
  case PASSWORD_LEVEL_GOLD:
    length = 173;
    break;
  case PASSWORD_LEVEL_SILVER:
    length = 39;
    break;
  case PASSWORD_LEVEL_BRONZE:
    length = 9;
    break;
  }
  for (i = 0; i != length; i++)
    out[i] = 0;
  levelBits = 0;
  djinnBits = 0;
  keyItemBits = 0;
  flagBits = 0;
  for (i = 0; i != 8; i++)
    packedStats[i] = 0;
  for (i = 0; i != 6; i++) {
    if (GameFlag_Get(gClear_PasswordFlags[i]))
      flagBits |= 1 << i;
  }
  i = 0;
  outputIndex = 4;
  for (; i != 4; i++) {
    struct Unit *unit = Unit_Get(gClear_PasswordMembers[i]);
    struct UnitBaseStats *base = (struct UnitBaseStats *)&unit->baseHp;
    s32 value;

    if (base->hp > UNIT_CHARACTER_HP_MAX)
      base->hp = UNIT_CHARACTER_HP_MAX;
    value = (u16)base->hp;
    if ((s16)value < 0)
      base->hp = 0;
    if (base->pp > UNIT_CHARACTER_HP_MAX)
      base->pp = UNIT_CHARACTER_HP_MAX;
    value = (u16)base->pp;
    if ((s16)value < 0)
      base->pp = 0;
    if (base->attack > UNIT_STAT_MAX)
      base->attack = UNIT_STAT_MAX;
    value = base->attack;
    if (value < 0)
      value = 0;
    base->attack = value;
    if (base->defense > UNIT_STAT_MAX)
      base->defense = UNIT_STAT_MAX;
    value = base->defense;
    if (value < 0)
      value = 0;
    base->defense = value;
    if (base->agility > UNIT_STAT_MAX)
      base->agility = UNIT_STAT_MAX;
    value = base->agility;
    if (value < 0)
      value = 0;
    base->agility = value;
    if (base->luck > UNIT_LUCK_MAX)
      base->luck = UNIT_LUCK_MAX;
    value = base->luck;
    if (value < 0)
      value = 0;
    base->luck = value;
    packedStats[i * 2] = (u32)base->hp << 21 | base->pp << 10 | base->attack;
    *(u32 *)((u8 *)packedStats + outputIndex) =
        (u32)base->defense << 22 | base->agility << 12 | base->luck << 4;
    if (unit->level > UNIT_LEVEL_MAX)
      unit->level = UNIT_LEVEL_MAX;
    value = unit->level;
    if (value == 0)
      unit->level = 1;
    levelBits |= unit->level << (i * 7);
    for (j = 0; j != 4; j++)
      djinnBits += unit->djinnHeld[j] << (j * 7);
    for (j = 0; j != 15; j++) {
      s32 item = unit->inventory[j] & INVENTORY_ITEM_ID_MASK;

      for (k = 0; k != 8; k++) {
        if (item == gClear_PasswordKeyItems[k])
          keyItemBits |= 1 << k;
      }
    }
    outputIndex += 8;
  }
  if (mode == PASSWORD_LEVEL_GOLD) {
    outputIndex = 39;
    bitOffset = 0;
    for (i = 0; i != 4; i++) {
      struct Unit *unit = Unit_Get(gClear_PasswordMembers[i]);

      for (n = 0; n != 15; n++) {
        u16 item = unit->inventory[n] & INVENTORY_ITEM_ID_MASK;

        out[outputIndex] += item >> (bitOffset + 1);
        out[outputIndex + 1] += item << (7 - bitOffset);
        outputIndex++;
        bitOffset++;
        if (bitOffset == 7) {
          bitOffset = 0;
          outputIndex++;
        }
      }
    }
    outputIndex = 107;
    bitOffset = -1;
    for (i = 0; i != 4; i++) {
      struct Unit *unit = Unit_Get(gClear_PasswordMembers[i]);

      for (m = 0; m != 23; m++) {
        s32 count = 0;
        u16 quantity;
        /* FAKEMATCH(dead): the form kept; as no quantity copy, 133 of 479
         * instructions differ in tla-en. */

        for (n = 0; n != 15; n++) {
          if ((unit->inventory[n] & INVENTORY_ITEM_ID_MASK) ==
              gClear_PasswordStackItems[m])
            count = unit->inventory[n] >> INVENTORY_QUANTITY_SHIFT;
        }
        quantity = count;
        if (bitOffset < 0) {
          out[outputIndex] += (u16)count >> -bitOffset;
          bitOffset += 8;
          outputIndex++;
        }
        out[outputIndex] += (u16)count << bitOffset;
        bitOffset -= 5;
        if (bitOffset == -5) {
          outputIndex++;
          bitOffset = 3;
        }
      }
    }
    out[165] = (u32)gMainRoutineWork.coins >> 16;
    out[166] = (u32)gMainRoutineWork.coins >> 8;
    out[167] = gMainRoutineWork.coins;
  }
  if (mode != PASSWORD_LEVEL_BRONZE) {
    s32 statsOffset = 8 + (mode != PASSWORD_LEVEL_GOLD);

    for (i = 0; i != 2; i++) {
      out[statsOffset] = packedStats[i * 4] >> 24;
      out[statsOffset + 1] = packedStats[i * 4] >> 16;
      out[statsOffset + 2] = packedStats[i * 4] >> 8;
      out[statsOffset + 3] = packedStats[i * 4];
      out[statsOffset + 4] = packedStats[i * 4 + 1] >> 24;
      out[statsOffset + 5] = packedStats[i * 4 + 1] >> 16;
      out[statsOffset + 6] = packedStats[i * 4 + 1] >> 8;
      out[statsOffset + 7] = packedStats[i * 4 + 1];
      out[statsOffset + 7] |= packedStats[i * 4 + 2] >> 28;
      out[statsOffset + 8] = packedStats[i * 4 + 2] >> 20;
      out[statsOffset + 9] = packedStats[i * 4 + 2] >> 12;
      out[statsOffset + 10] = packedStats[i * 4 + 2] >> 4;
      out[statsOffset + 11] = packedStats[i * 4 + 2] << 4;
      out[statsOffset + 11] |= packedStats[i * 4 + 3] >> 28;
      out[statsOffset + 12] = packedStats[i * 4 + 3] >> 20;
      out[statsOffset + 13] = packedStats[i * 4 + 3] >> 12;
      out[statsOffset + 14] = packedStats[i * 4 + 3] >> 4;
      statsOffset += 15;
    }
  }
  out[0] = levelBits;
  out[1] = levelBits >> 8;
  out[2] = levelBits >> 16;
  out[3] = (levelBits >> 20 & 0xf0) | (djinnBits & 15);
  out[4] = djinnBits >> 4;
  out[5] = djinnBits >> 12;
  out[6] = djinnBits >> 20;
  out[7] = flagBits;
  if (mode != PASSWORD_LEVEL_GOLD)
    out[8] = keyItemBits;
  return length;
}

s32 Clear_DecodePassword(u8 *src, s32 length, u8 *dst) {
  u8 saved[PASSWORD_GOLD_LENGTH];
  s32 i;
  s32 shift = 2;
  s32 k = 0;
  s32 n = 0;
  s32 size = PASSWORD_BRONZE_BYTES;
  u8 last;
  u8 c;

  if (length != PASSWORD_BRONZE_LENGTH) {
    size = PASSWORD_SILVER_BYTES;
    if (length != PASSWORD_SILVER_LENGTH)
      size = PASSWORD_GOLD_BYTES;
  }
  for (i = 0; i != size; i++)
    dst[i] = 0;
  for (i = 0; i != length; i++) {
    saved[i] = src[i];
    src[i] = (saved[i] - i) & PASSWORD_SYMBOL_MASK;
  }

  for (i = 0; i != length; i++, k++) {
    // Every tenth symbol is the checksum of the nine before it.
    if (i % PASSWORD_SYMBOLS_PER_ROW > 8)
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
/// over all but the block's last two bytes.
u16 Clear_CalculatePasswordCrc16(u32 length, u8 *p) {
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

u16 gClear_ButtonCode[] = {
    4,
    4,
    4,
    0,
};

// clang-format off
u16 gClear_ExtraButtonCode[] = {
    64, 128, 64, 128, 32, 16, 32, 16,
#if (LANGUAGE == LANGUAGE_JAPANESE)
    64, 16, 128, 32, 64, 4, 0,
#else
    64, 16, 128, 32, 64, 4, 0, 0,
#endif
};

s8 gClear_CursorWobble[] = {
    0, 1, 2, 3, 3, 2, 1, 0,
};

// Alphabet table, absent in Japanese ("ABCDE FGHJK LMNPQ R" ...).
#if (LANGUAGE != LANGUAGE_JAPANESE)
u8 gClear_FirstPasswordRow[] = {
    65, 66, 67, 68, 69, 32, 70, 71,
    72, 74, 75, 32, 76, 77, 78, 80,
    81, 32, 82, 0,
};

u8 gClear_SecondPasswordRow[] = {
    83, 84, 85, 86, 87, 32, 88, 89,
    90, 50, 51, 32, 52, 53, 54, 55,
    56, 32, 57, 0,
};

u8 gClear_ThirdPasswordRow[] = {
    97, 98, 99, 100, 101, 32, 102, 103,
    104, 105, 106, 32, 107, 109, 110, 112,
    113, 32, 114, 0,
};

u8 gClear_FourthPasswordRow[] = {
    115, 116, 117, 118, 119, 32, 120, 121,
    122, 33, 63, 32, 35, 38, 36, 37,
    43, 32, 61, 0,
};
#endif

u16 gClear_ImportedFlags[] = {
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
    {TRIGGER_TABLE_END},
};

u16 gClear_BlendFrame[] = {
    0,
};

s16 gClear_CodeUnlocked = 0;

s16 gClear_ExtraCodeUnlocked = 0;

s16 gClear_CodeProgress = 0;

s16 gClear_ExtraCodeProgress = 0;

s16 gClear_CodeHeld = 0;

s16 gClear_ExtraCodeHeld = 0;

#undef MAP_CELL_BACKUP
#undef YEAH_WIDTH
#undef COPY_CELLS
#undef SAVE_CELLS
#undef RESTORE_CELLS
#undef RETURN_TO_SAVED
#undef RECEIVED_FLAGS
#undef RECEIVED_UNITS
#undef REQUEST_EDITION
#undef MSGNO
#undef QUESTION_MESSAGE
#undef RECEIVE_WINDOW_X
#undef RECEIVE_WINDOW_WIDTH
#undef STATUS_WINDOW_X
#undef STATUS_WINDOW_WIDTH
#undef SIDE_COLUMN
#undef SIDE_HIGHLIGHT_WIDTH
#undef CHECK_WINDOW_X
#undef CHECK_WINDOW_WIDTH
#undef DECODED_PASSWORD
#undef CURSOR_ON_SIDE_COLUMN
#undef DRAW_TITLE
#undef WORD

// clang-format off
LATE_DATA s32 gClear_ImportedMembers[] = {
    ISAAC, GARET, IVAN, MIA,
};

LATE_DATA u16 gClear_ImportedKeyItems[] = {
    ITEM_ORB_OF_FORCE, ITEM_DOUSE_DROP, ITEM_FROST_JEWEL, ITEM_LIFTING_GEM,
    ITEM_HALT_GEM, ITEM_CLOAK_BALL, ITEM_CARRY_STONE, ITEM_CATCH_BEADS,
};

LATE_DATA u16 gClear_ImportedStackItems[] = {
    ITEM_HERB, ITEM_NUT, ITEM_VIAL, ITEM_POTION, ITEM_PSY_CRYSTAL,
    ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_WATER_OF_LIFE, ITEM_POWER_BREAD,
    ITEM_COOKIE, ITEM_APPLE, ITEM_HARD_NUT, ITEM_MINT, ITEM_LUCKY_PEPPER,
    ITEM_SMOKE_BOMB, ITEM_SLEEP_BOMB, ITEM_GAME_TICKET, ITEM_LUCKY_MEDAL,
    ITEM_SACRED_FEATHER, ITEM_OIL_DROP, ITEM_WEASELS_CLAW, ITEM_BRAMBLE_SEED,
    ITEM_CRYSTAL_POWDER, ITEM_NONE,
};

LATE_DATA s32 gClear_PasswordMembers[] = {
    ISAAC, GARET, IVAN, MIA,
};

LATE_DATA u16 gClear_PasswordFlags[] = {
    0x941, 0x951, 0x8b3, 0x8d1, 0x81e, 0x868,
};

LATE_DATA u16 gClear_PasswordKeyItems[] = {
    ITEM_ORB_OF_FORCE, ITEM_DOUSE_DROP, ITEM_FROST_JEWEL, ITEM_LIFTING_GEM,
    ITEM_HALT_GEM, ITEM_CLOAK_BALL, ITEM_CARRY_STONE, ITEM_CATCH_BEADS,
};

LATE_DATA u16 gClear_PasswordStackItems[] = {
    ITEM_HERB, ITEM_NUT, ITEM_VIAL, ITEM_POTION, ITEM_PSY_CRYSTAL,
    ITEM_ANTIDOTE, ITEM_ELIXIR, ITEM_WATER_OF_LIFE, ITEM_POWER_BREAD,
    ITEM_COOKIE, ITEM_APPLE, ITEM_HARD_NUT, ITEM_MINT, ITEM_LUCKY_PEPPER,
    ITEM_SMOKE_BOMB, ITEM_SLEEP_BOMB, ITEM_GAME_TICKET, ITEM_LUCKY_MEDAL,
    ITEM_SACRED_FEATHER, ITEM_OIL_DROP, ITEM_WEASELS_CLAW, ITEM_BRAMBLE_SEED,
    ITEM_CRYSTAL_POWDER,
};
// clang-format on
