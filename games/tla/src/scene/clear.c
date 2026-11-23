/// @file
/// The File Screen overlay (SceneId_Clear), the first thing the player sees
/// after the title: the title logo, the file menu (new game, continue, copy,
/// erase, the Battle Arena and Update) and what a new game asks. A new game
/// may bring The Broken Seal's data over, by password or link cable
/// (Clear_EnterPassword, Clear_ReceiveByLink), applied to Isaac's party
/// (Clear_ApplyImportedData), and two secret button codes on the screen
/// (Clear_CheckButtonCodes) name more of the party.
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
#include "scene/title.h"
#include "field/map_grid.h"
#include "window/screens.h"
#include "window/window.h"
#include "system/serial.h"
#include "edition.h"
#include "version.h"
#include "system/iwram_call.h"
#include "field/call.h"
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
#include "djinn_ids.h"
#include "field/field_sequence.h"
#include "graphics/text_font.h"
#include "game/character_ids.h"
#include "system/save.h"

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
s32 NameEntry_EditMemberName(s32 member);
s32 IconMenu_ChooseSendOption(s32 mode);

s32 Clear_EnterPassword(void);

extern s8 gClear_CursorBob[];
void Clear_ApplyImportedData(s32 mode, u8 *data);
/// Decodes length symbols of an entered password (src) into dst, undoing The
/// Broken Seal's FileScreen_EncodePassword: each symbol less its place, the
/// checksum symbols skipped, six bits a symbol, then every byte but the last
/// XORed with the last. Returns the bytes written.
s32 Clear_DecodePassword(u8 *src, s32 length, u8 *dst);
extern u16 gClear_ImportedFlags[];

struct Window;

// The scene passes it a third argument it does not take, 0 for a new game and
// 1 for Update.
void DataTransfer_Apply(u8 *save, s32 byPassword);
void DataTransfer_GiveDefaultDjinn(void);
void ClearData_Apply(void *data);
extern u8 gClear_FirstPasswordRow[];
extern u8 gClear_SecondPasswordRow[];
extern u8 gClear_ThirdPasswordRow[];
void *Glyph_AddBlankSprite(struct Window *window, void *glyph, s32 x, s32 y);

extern u8 gClear_PasswordGrid[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Loads the title logo onto BG2 as Title_LoadBackground (title.c) does,
/// and stops the leader's motion.
void Clear_LoadBackground(void) {
  u8 *resource;
  s32 resourceId;
  u16 *tilemap;
  u32 x;
  u32 y;
  u16 tile;
  struct BgScroll *scroll;
  s32 blankTile;

  resourceId = RESOURCE_LOGO_ON_BLACK;
  ScreenFade_ToBlack(0);
  REG_BG2CNT = LOGO_BGCNT;
  gBgScroll[2].y = 0;
  blankTile = LOGO_BLANK_TILE;
  resource = Resource_GetTableEntry(resourceId);
  Dma_Set(resource, (void *)BG_PLTT, DMA_COPY_WORDS(LOGO_PALETTE_COLORS / 2),
          REG_DMA3);
  resource += LOGO_PALETTE_COLORS * 2;
  FieldDecompress_TaggedLz(resource, Ram_MapCellBuffer);
  // A screen's worth of 8-bit tiles.
  Dma_Set((void *)Ram_MapCellBuffer, LOGO_TILES,
          DMA_COPY_WORDS(DISPLAY_WIDTH * DISPLAY_HEIGHT / 4), REG_DMA3);

  tilemap = LOGO_MAP;
  tile = LOGO_FIRST_TILE;
  for (y = 0; y < DISPLAY_TILE_HEIGHT; y++) {
    for (x = 0; x < DISPLAY_TILE_WIDTH; x++)
      *tilemap++ = tile++;
    *tilemap++ = blankTile;
    *tilemap++ = blankTile;
  }
  scroll = gBgScroll;
  for (y = 0; y <= 3; y++) {
    scroll->y = 0;
    scroll->x = 0;
    scroll++;
  }
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  ((struct MapWork *)Ram_HeapSlots->mapGrid)->dispcnt =
      DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  Actor_Get(gMainRoutineWork.leaderActorId)->motionFlags = 0;
}

/// The task that fades BG0 and the sprites in over the logo, a step of the
/// blend every second frame.
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
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                      (BLEND_WEIGHT_MAX - (u16)level) << 8 | (u16)level,
                      IO_WRITE_HALFWORD);
  if (level >= BLEND_WEIGHT_MAX)
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

/// Nonzero (-1) when the save was made where Retreat is blocked, unless in a
/// town or on the world map: such a save cannot restart at the Sanctum, by L
/// and Start held as it loads or as a corrupted save is recovered.
s32 Clear_IsRetreatBlocked(void) {
  struct MainRoutineWork *party;
  s32 mode;

  if (Event_GetFlag(FLAG_RETREAT_BLOCKED) == 0)
    return 0;
  party = &gMainRoutineWork;

  if (party->sceneType == SCENE_TYPE_TOWN)
    return 0;
  mode = party->next ^ SCENE_WORLD_MAP;
  return -NONZERO(mode);
}

#include "text/msg_ids.h"

// The play time the save kept in the scene's own state, which a sound save
// matches.
#define SAVED_PLAY_TIME (*(u32 *)gSceneLocalState)

s32 Save_ChooseAndLoadFile(s32 mode);
void Save_CopyFile(void);
u32 BuildStamp_GetTime(void);
void InventorySnapshot_Restore(void);
void StatusScreen_Run(void);

/// The game's state as a file holds it, from gSaveBuffer (system/save.h's
/// save image): a save part's three sectors.
#define SAVE_IMAGE_BYTES (SAVE_SECTORS_PER_PART * SAVE_SECTOR_SIZE)
/// Where the data a password or link screen brings in waits for
/// DataTransfer_Apply, as a save image: after the state set aside at the
/// start of the map cell buffer, free on this screen.
#define TRANSFERRED_SAVE (Ram_MapCellBuffer + SAVE_IMAGE_BYTES)
/// A save image from the game flags to the units' end: the flags, the main
/// routine work and the party's units.
#define SAVE_IMAGE_STATE_BYTES                                                 \
  (SAVE_IMAGE_UNITS + sizeof(gCharacterUnits) - SAVE_IMAGE_GAME_FLAGS)

/// The width of the answers "Yeah!" and "Wait a sec" to "Play on EASY level?"
/// and the two questions after it, as each language's words need.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define YEAH_WIDTH 6
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define YEAH_WIDTH 10
#else
#define YEAH_WIDTH 8
#endif

/// The password and link screens read The Broken Seal's data into the game's
/// state: KEEP_SAVE_IMAGE sets the state aside in the map cell buffer first,
/// and RESTORE_SAVE_IMAGE copies what the screen left to TRANSFERRED_SAVE and
/// puts the state back.
#define COPY_SAVE_IMAGE(to, from) Iwram_CopyWords(to, from, SAVE_IMAGE_BYTES)
#define KEEP_SAVE_IMAGE()                                                      \
  u8 *image = gSaveBuffer;                                                     \
                                                                               \
  COPY_SAVE_IMAGE(Ram_MapCellBuffer, image)
#define RESTORE_SAVE_IMAGE()                                                   \
  {                                                                            \
    COPY_SAVE_IMAGE(TRANSFERRED_SAVE, image);                                  \
    COPY_SAVE_IMAGE(image, Ram_MapCellBuffer);                                 \
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
  s32 result, i, limit, status, message, recovery;

  Clear_LoadBackground();
  Task_Add(Clear_CheckButtonCodes, TASK_PASS_FRAME);
  gFieldWork->transition = started;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  // After the epilogue: the cleared game may be saved, then the title.
  if (gMainRoutineWork.entrance == CLEAR_FROM_EPILOGUE) {
    do {
      message = (s32)MsgSaveYourAdventure;
      // "Save your adventure?"
      FieldMessage_Show(message, FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN);
      i = FieldYesNo_Ask(1);
      Message_CloseFinishedWindows();
      if (i != YES) {
        break;
      }
      // "You cannot resume your adventure with this data."
      FieldMessage_Show(message + 1, FIELD_MESSAGE_ZOOM);
      // Marks the file cleared (SaveFileSummary's sendFlag).
      gMainRoutineWork.sendFlag = 1;
      Party_AddMember(JENNA);
      Party_AddMember(SHEBA);
      Party_AddMember(PIERS);
      Party_AddMember(GARET);
      Party_AddMember(IVAN);
      Party_AddMember(MIA);
      i = Save_ToChosenFile();
    } while (i == -1);
    Event_Wait(60);
    Event_FinishScreenTransition();
    Sound_Play(SOUND_FADE_OUT_BGM);
    Event_Wait(300);
    Event_SetDestination(SCENE_TITLE, TITLE_START);
    return 0;
  }
  gSleepDisabled = started;
top:
  result = Save_ScanFileSummaries();
  if (result < 0 && gDebugMode) {
    gMainRoutineWork.idleSleepEnabled = 1;
    gIdleSleepEnabled = 1;
    // "Unable to detect backup memory."
    FieldMessage_ShowCentered((s32)MsgNoBackupMemory, 1, 8);
  }
  if (result == 0 && !started) {
    wait(30);
    Task_Add(Clear_FadeInBlend, TASK_PASS_FRAME);
    wait(1);
    started = 1;
  }
  if (result > 0)
    result = FileList_SelectAction();
  else
    result = 0;
  if (result == SAVE_ACTION_NEW_GAME) {
    // Felix is named, and with either code Jenna, Sheba and Piers too.
    MainRoutineWork_InitializeDefaults();
    WindowColor_Apply(gMainRoutineWork.windowHue,
                      gMainRoutineWork.windowBrightness);
    i = FELIX;
    limit = FELIX + 1;
    for (;;) {
      wait(6);
      result = NameEntry_EditMemberName(i);
      if (result == -1) {
        if (i == FELIX) {
          goto top;
        }
        i--;
        continue;
      }
      if (gClear_CodeUnlocked != 0 || gClear_ExtraCodeUnlocked != 0) {
        limit = PIERS + 1;
      }
      if (++i >= limit) {
        break;
      }
    }
  ask:
    // "Transfer data from Golden Sun?"
    FieldMessage_Show((s32)MsgTransferDataFromGoldenSun, QUESTION_MESSAGE);
    result = YesNoMenu_Run(1, 0, 0, 0);
    Message_CloseFinishedWindows();
    if (result == YES) {
      for (;;) {
        result = IconMenu_ChooseSendOption(RESOURCE_LAYOUT_TRANSFER_METHOD);
        if (result == -1) {
          goto top;
        }
        if (result == TRANSFER_METHOD_CABLE) {
          KEEP_SAVE_IMAGE();
          status = Clear_ReceiveByLink();
          if (status != -1) {
            StatusScreen_Run();
          }
          RESTORE_SAVE_IMAGE()
          if (status == -1) {
            continue;
          }
          Call3((void (*)())DataTransfer_Apply, (s32)TRANSFERRED_SAVE, 0, 0);
        } else if (result == TRANSFER_METHOD_PASSWORD) {
          KEEP_SAVE_IMAGE();
          MainRoutineWork_InitializeDefaults();
#if !TLA_PATCH_2
          status = Clear_EnterPassword();
          RESTORE_SAVE_IMAGE()
          if (status == -1) {
            continue;
          }
#else
          Clear_EnterPassword();
          RESTORE_SAVE_IMAGE()
#endif
          Call3((void (*)())DataTransfer_Apply, (s32)TRANSFERRED_SAVE, 1, 0);
        }
        break;
      }
    } else {
      DataTransfer_GiveDefaultDjinn();
    }
    // Isaac is named, and with the second code Garet, Ivan and Mia too.
    i = ISAAC;
    limit = ISAAC + 1;
    for (;;) {
      wait(6);
      result = NameEntry_EditMemberName(i);
      if (result == -1) {
        if (i == ISAAC) {
          goto ask;
        }
        i--;
        continue;
      }
      if (gClear_ExtraCodeUnlocked != 0) {
        limit = MIA + 1;
      }
      if (++i >= limit) {
        break;
      }
    }
    if (gAnyFileCleared != 0) {
    save:
      message = (s32)MsgPlayOnEasyLevel;
      // "Play on EASY level? (Characters start at Lost Age Clear Data levels)"
      FieldMessage_Show(message, QUESTION_MESSAGE);
      i = YesNoMenu_Run(1, (s32)MsgYeah, YEAH_WIDTH, 0);
      Message_CloseFinishedWindows();
      if (i == YES) {
        if (gAnyFileCleared == 0) {
          // "...Sorry, you have no Clear Data."
          FieldMessage_Show(message + 1, FIELD_MESSAGE_ZOOM);
        } else {
          KEEP_SAVE_IMAGE();
          result = Save_ChooseAndLoadFile(FILE_LIST_CLEARED);
          if (result != 0)
            goto save;
          {
            struct MainRoutineWork *party = &gMainRoutineWork;

            WindowColor_Apply(party->windowHue, party->windowBrightness);
            RESTORE_SAVE_IMAGE()
            ClearData_Apply(TRANSFERRED_SAVE);
            party->difficulty = DIFFICULTY_EASY;
          }
          goto leave;
        }
      }
      // "Play on HARD level? (Monsters are stronger)"
      FieldMessage_Show((s32)MsgPlayOnHardLevel, QUESTION_MESSAGE);
      i = YesNoMenu_Run(1, (s32)MsgYeah, YEAH_WIDTH, 0);
      Message_CloseFinishedWindows();
      if (i == YES) {
        Event_SetFlag(FLAG_HARD_LEVEL);
        gMainRoutineWork.difficulty = DIFFICULTY_HARD;
        goto leave;
      }
      // "Play on Normal level? (Default difficulty)"
      FieldMessage_Show((s32)MsgPlayOnNormalLevel, QUESTION_MESSAGE);
      i = YesNoMenu_Run(1, (s32)MsgYeah, YEAH_WIDTH, 0);
      Message_CloseFinishedWindows();
      if (i != YES)
        goto save;
    }
  leave:
    Event_SetDestination(SCENE_TITLE, TITLE_STORY_CRAWL);
    goto finish;
  }
  if (result == SAVE_ACTION_CONTINUE) {
    result = Save_ChooseAndLoadFile(FILE_LIST_CONTINUE);
    if (result == -1)
      goto top;
    Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
    {
      struct MainRoutineWork *party = &gMainRoutineWork;

      WindowColor_Apply(party->windowHue, party->windowBrightness);
      if (party->buildStamp != BuildStamp_GetTime()) {
        RETURN_TO_SAVED(party)
      } else if (mask = KEY_L | KEY_START, (gInput.held & mask) == mask) {
        if (Clear_IsRetreatBlocked()) {
          // "Unfortunately, your data could not be recovered."
          FieldMessage_Show((s32)MsgDataCouldNotBeRecovered,
                            FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
          goto top;
        }
        RETURN_TO_SAVED(party)
        Event_SetFlag(FLAG_SAVE_RESTARTED_AT_SANCTUM);
      } else if (party->playTime != SAVED_PLAY_TIME) {
        recovery = (s32)MsgSomeDataIsCorrupted;
        // "Some data is corrupted and cannot be recovered."
        FieldMessage_Show(recovery, FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
        // "Do you want to try to recover from a Sanctum?"
        FieldMessage_Show(recovery + 1, QUESTION_MESSAGE);
        if (YesNoMenu_Run(1, 0, 0, 0) != YES) {
          Message_CloseFinishedWindows();
          goto top;
        }
        Message_CloseFinishedWindows();
        if (Clear_IsRetreatBlocked()) {
          // "Unfortunately, your data could not be recovered."
          FieldMessage_Show(recovery + 2,
                            FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
          goto top;
        }
        RETURN_TO_SAVED(party)
        Event_SetFlag(FLAG_SAVE_RECOVERED_AT_SANCTUM);
      }
    }
    Event_ClearFlag(FLAG_MENU_OPEN);
    goto finish;
  }
  if (result == SAVE_ACTION_COPY) {
    Save_CopyFile();
    goto top;
  }
  if (result == SAVE_ACTION_ERASE) {
    Save_EraseSelectedFile();
    goto top;
  }
  if (result == SAVE_ACTION_BATTLE) {
    result = Save_ChooseAndLoadFile(FILE_LIST_EXTRA);
    if (result == -1)
      goto top;
    {
      struct MainRoutineWork *party = &gMainRoutineWork;

      party->leaderActorId = ACTOR_FELIX;
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
  if (result != SAVE_ACTION_UPDATE)
    goto top;
link:
  result = Save_ChooseAndLoadFile(FILE_LIST_UPDATE);
  if (result == -1)
    goto top;
  WindowColor_Apply(gMainRoutineWork.windowHue,
                    gMainRoutineWork.windowBrightness);
  for (;;) {
    // The menu reads only its mode's lowest bit: the transfer method.
    result = IconMenu_ChooseSendOption(2);
    if (result == -1) {
      goto link;
    }
    if (result == TRANSFER_METHOD_CABLE) {
      KEEP_SAVE_IMAGE();
      status = Clear_ReceiveByLink();
      if (status != -1) {
        StatusScreen_Run();
      }
      RESTORE_SAVE_IMAGE()
      if (status == -1) {
        continue;
      }
      Call3((void (*)())DataTransfer_Apply, (s32)TRANSFERRED_SAVE, 0, 1);
    } else if (result == TRANSFER_METHOD_PASSWORD) {
      KEEP_SAVE_IMAGE();
      MainRoutineWork_InitializeDefaults();
      status = Clear_EnterPassword();
      RESTORE_SAVE_IMAGE()
      if (status == -1) {
        continue;
      }
      Call3((void (*)())DataTransfer_Apply, (s32)TRANSFERRED_SAVE, 1, 1);
    }
    break;
  }
  if (gClear_CodeUnlocked != 0 || gClear_ExtraCodeUnlocked != 0) {
    i = ISAAC;
    limit = ISAAC + 1;
    for (;;) {
      wait(6);
      result = NameEntry_EditMemberName(i);
      if (result == -1) {
        if (i == ISAAC) {
          goto link;
        }
        i--;
        continue;
      }
      if (gClear_ExtraCodeUnlocked != 0) {
        limit = MIA + 1;
      }
      if (++i >= limit) {
        break;
      }
    }
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
/// The checksum's two bytes at the end of each level's
/// (Clear_CalculatePasswordCrc16).
#define PASSWORD_CRC_BYTES 2
#define PASSWORD_CRC_SEED 0xffff
#define PASSWORD_CRC_POLYNOMIAL 0x1021
#define PASSWORD_CRC_TOP_BIT 0x8000 ///< of its 16 bits
/// Where a password's parts start among its bytes: the four levels and the
/// Djinn from the first, the story flags, then Silver's and Bronze's
/// Psynergy items, which Gold has not; the stats rows after those, two party
/// members to a row; and Gold's alone, the party's items, 15 a member in nine
/// bits each, their quantities, five bits for each of the 23 items that stack
/// (gClear_ImportedStackItems), and the coins, high byte first.
#define PASSWORD_FLAGS_BYTE 7
#define PASSWORD_KEY_ITEMS_BYTE 8
#define PASSWORD_STATS_ROW_BYTES 15
#define PASSWORD_ITEMS_BYTE 39
#define PASSWORD_QUANTITIES_BYTE 107
#define PASSWORD_COINS_BYTE 165
#define PASSWORD_STACK_ITEMS 23
/// A stacking item's quantity in the password: its five bits.
#define PASSWORD_QUANTITY_MASK                                                 \
  (INVENTORY_QUANTITY_MASK >> INVENTORY_QUANTITY_SHIFT)
/// A party member's base stats in the password, two words of fields: HP (11
/// bits), PP (11) and Attack (10); then Defense (10), Agility (10) and Luck (8)
/// over four bits of 0. This game reads Attack, Defense and Agility back in
/// nine bits and Luck in seven.
#define PASSWORD_HP_SHIFT 21
#define PASSWORD_PP_SHIFT 10
#define PASSWORD_DEFENSE_SHIFT 22
#define PASSWORD_AGILITY_SHIFT 12
#define PASSWORD_LUCK_SHIFT 4
#define PASSWORD_PP_MASK 0x7ff   ///< eleven bits
#define PASSWORD_STAT_MASK 0x1ff ///< nine bits: Attack, Defense and Agility
#define PASSWORD_LUCK_MASK 0x7f  ///< seven bits
/// A level in the password's first bytes: seven bits each.
#define PASSWORD_LEVEL_MASK 0x7f
/// A password glyph's sprite: four tiles, 16 pixels square.
#define GLYPH_SPRITE_BYTES (4 * TILE_SIZE_4BPP)
/// A unit's Djinn, djinnHeld to djinnSetCounts.
#define UNIT_DJINN_BYTES 40

/// The password as entered, a symbol a byte: a Gold password is 260 symbols,
/// which the Japanese buffer holds exactly, as in The Broken Seal.
// XXX (no evidence): why the other editions reserve 320, and the Japanese 60
// bytes after the decoded password; nothing in this game writes or reads past
// 260 or the decoded bytes, so the bytes show only the space.
#if TLA_PATCH_1
#define PASSWORD_BUFFER_SIZE 320
#else
#define PASSWORD_BUFFER_SIZE PASSWORD_GOLD_LENGTH
#endif
u8 gClear_PasswordText[PASSWORD_BUFFER_SIZE] BSS_DATA;
/// The entered password's bytes, decoded (Clear_DecodePassword).
u8 gClear_DecodedPassword[PASSWORD_GOLD_BYTES] BSS_DATA;
#if !TLA_PATCH_1
/// Unused: the Japanese edition's 60 bytes after the decoded password.
u8 gClear_PasswordSpare[60] BSS_DATA;
#endif

/// The password grid's glyph sprites, one for each place of a page.
struct PasswordWork {
  void *glyphs[PASSWORD_SYMBOLS_PER_PAGE];
  void *cells[PASSWORD_SYMBOLS_PER_PAGE];
  u8 shown[PASSWORD_SYMBOLS_PER_PAGE]; ///< the symbol each place shows
  u8 unused[26];                       ///< cleared on entry, read by nothing
};

/// The password's symbols in the font: in the Japanese edition kana in their
/// order (charmap_tla_ja.tsv's codes), あ to そ, た to ん and を, then with the
/// voiced mark が to ぞ, だ, で, ど and ば to ぼ; in the others capitals
/// without I and O, the digits 2 to 9, small letters without l and o, then ! ?
/// # & $ % + and =.
#if LANGUAGE == LANGUAGE_JAPANESE
#define SYMBOL_TA 15
#define SYMBOL_N 44
#define SYMBOL_WO 45
#define SYMBOL_GA 46
#define SYMBOL_DA 56
#define SYMBOL_BA 59
/// The character codes of kana that start runs in the font's order, as The
/// Broken Seal's window/text_font.h names them.
#define KANA_WO 134 ///< を
#define KANA_A 145  ///< あ, then い to そ
#define KANA_KA 150 ///< か, then き to そ
#define KANA_TA 224 ///< た, then ち to ん
#define KANA_TE 227 ///< て
#define KANA_TO 228 ///< と
#define KANA_HA 234 ///< は, then ひ to ほ
#define KANA_N 253  ///< ん
/// A kana's code with the voiced mark, gFont_MarkGlyphCodes's second.
#define VOICED(code) ((code) | 1 << TEXT_MARK_SHIFT)
#else
#define SYMBOL_DIGITS 24
#define SYMBOL_SMALL_LETTERS 32
#define SYMBOL_SIGNS 56
#endif

/// The character a password symbol stands for, in the font's codes, a kana's
/// voiced mark in its top bits.
void Clear_GetPasswordCharacter(s32 cell, u16 *text) {
  text[1] = 0;
  text[2] = 0;
#if LANGUAGE != LANGUAGE_JAPANESE
  if (cell <= 'H' - 'A')
    text[0] = cell + 'A';
  else if (cell <= 'N' - 'A' - 1)
    text[0] = cell + 'A' + 1;
  else if (cell <= 'Z' - 'A' - 2)
    text[0] = cell + 'A' + 2;
  else if (cell < SYMBOL_SMALL_LETTERS)
    text[0] = cell - SYMBOL_DIGITS + '2';
  else if (cell <= SYMBOL_SMALL_LETTERS + 'k' - 'a')
    text[0] = cell - SYMBOL_SMALL_LETTERS + 'a';
  else if (cell <= SYMBOL_SMALL_LETTERS + 'n' - 'a' - 1)
    text[0] = cell - SYMBOL_SMALL_LETTERS + 'a' + 1;
  else if (cell <= SYMBOL_SMALL_LETTERS + 'z' - 'a' - 2)
    text[0] = cell - SYMBOL_SMALL_LETTERS + 'a' + 2;
  else if (cell == SYMBOL_SIGNS)
    text[0] = '!';
  else if (cell == SYMBOL_SIGNS + 1)
    text[0] = '?';
  else if (cell == SYMBOL_SIGNS + 2)
    text[0] = '#';
  else if (cell == SYMBOL_SIGNS + 3)
    text[0] = '&';
  else if (cell == SYMBOL_SIGNS + 4)
    text[0] = '$';
  else if (cell == SYMBOL_SIGNS + 5)
    text[0] = '%';
  else if (cell == SYMBOL_SIGNS + 6)
    text[0] = '+';
  else
    text[0] = '=';
#else
  if (cell < SYMBOL_TA)
    text[0] = cell + KANA_A;
  else if (cell < SYMBOL_N)
    text[0] = cell - SYMBOL_TA + KANA_TA;
  else if (cell == SYMBOL_WO)
    text[0] = KANA_WO;
  else if (cell == SYMBOL_N)
    text[0] = KANA_N;
  else if (cell < SYMBOL_DA)
    text[0] = VOICED(cell - SYMBOL_GA + KANA_KA);
  else if (cell == SYMBOL_DA)
    text[0] = VOICED(KANA_TA);
  else if (cell == SYMBOL_DA + 1)
    text[0] = VOICED(KANA_TE);
  else if (cell == SYMBOL_DA + 2)
    text[0] = VOICED(KANA_TO);
  else
    text[0] = VOICED(cell - SYMBOL_BA + KANA_HA);
#endif
}

/// The transferred data's game flags, and its main routine work and what
/// follows (system/save.h's save image).
#define TRANSFERRED_FLAGS (TRANSFERRED_SAVE + SAVE_IMAGE_GAME_FLAGS)
#define TRANSFERRED_MAIN_ROUTINE_WORK                                          \
  (TRANSFERRED_SAVE + SAVE_IMAGE_MAIN_ROUTINE_WORK)

/// The first letter of the greeting this screen answers The Broken Seal with,
/// before V, T and S: the one its FILE_SCREEN_LINK_LANGUAGE_TAG waits for.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define LINK_LANGUAGE_TAG 'C'
#elif (LANGUAGE == LANGUAGE_GERMAN)
#define LINK_LANGUAGE_TAG 'E'
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define LINK_LANGUAGE_TAG 'S'
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define LINK_LANGUAGE_TAG 'F'
#elif (LANGUAGE == LANGUAGE_ITALIAN)
#define LINK_LANGUAGE_TAG 'I'
#else
#define LINK_LANGUAGE_TAG 'U'
#endif

/// Receiving by cable stops waiting after this long, almost three hours, and
/// ends with "Data transfer was interrupted" once The Broken Seal goes unheard
/// this long, as its LINK_SEND_FRAMES and LINK_SILENT_FRAMES.
#define LINK_RECEIVE_FRAMES 600000
#define LINK_SILENT_FRAMES 10

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

/// Receives The Broken Seal's data over the link cable: waits for that game
/// to greet with "0000", answers with the greeting it waits for, receives its
/// save image into TRANSFERRED_SAVE and reads the image's flags and units into
/// the game's state. Returns 1, or -1 when B gives up or the link is lost.
s32 Clear_ReceiveByLink(void) {
  void *window;
  s32 i, idle, ok, shown;
  s32 mask, linkMask;
  s32 message;
  u8 *flags;
  u16 *request;

  window = Window_Create(RECEIVE_WINDOW_X, 5, RECEIVE_WINDOW_WIDTH, 8,
                         WINDOW_FLAG_INSTANT);
  // "Preparing to receive", "saved Clear Data" and "B Button: Quit".
  message = (s32)MsgFieldPreparingToReceive;
  Text_DrawMessage(message, window, 0, 4);
  Text_DrawMessage(message + 1, window, 0, 16);
  Text_DrawMessage(message + 3, window, 0, 36);
  Serial_Initialize();
  System_Wait(10);
restart:
  mask = LINK_STATUS_HEARD_BOTH;
  goto poll;
scan:
  if ((gLinkStatus & mask) == mask) {
    u16 *signature = gLinkPeerSignatures[SIO_ID_OF(REG_SIOCNT) ^ 1];

    // The Broken Seal sets its greeting to "0000" as it waits to send.
    if (signature[0] == '0' && signature[1] == '0' && signature[2] == '0' &&
        signature[3] == '0') {
      request = (u16 *)gSerialTransfer.payload;
      request[0] = LINK_LANGUAGE_TAG;
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
  Serial_StartReceive(TRANSFERRED_SAVE);
  System_Wait(10);
  linkMask = LINK_STATUS_HEARD_BOTH;
  idle = 0;
  ok = 1;
  shown = 0;
  i = 0;
transfer:
  if (!shown && gSerialReceivedSize != 0) {
    shown = 1;
    Window_Close(window, 2);
    window = Window_Create(STATUS_WINDOW_X, 10, STATUS_WINDOW_WIDTH, 4,
                           WINDOW_FLAG_INSTANT);
    Text_DrawMessage((s32)MsgFieldTransferringData, window, 0, 4);
  }
  if (i == 9 && gSerialReceivedSize == 0) {
    request = (u16 *)gSerialTransfer.payload;
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
  if (idle == LINK_SILENT_FRAMES) {
    ok = 0;
    goto done;
  }
  if (gSerialReceiveDestination != 0) {
    System_Wait(1);
    i++;
    if (i < LINK_RECEIVE_FRAMES)
      goto transfer;
  }
done:
  if (!ok) {
    WindowSprite_PrepareForRedraw(window);
    Text_DrawMessage((s32)MsgFieldDataTransferWasInterrupted, window, 0, 4);
    do {
      System_Wait(1);
    } while (!(gInput.pressed & KEY_A));
  close:
    Window_Close(window, 2);
    return -1;
  }
  System_Wait(10);
  Serial_RemoveIrqHandlers();
  System_Wait(10);
  Window_Close(window, 2);
  // The flags and the fields before the Djinn recovery list come over in
  // place, as both games keep them; then The Broken Seal's main routine work
  // and units, moved on by as much as this game's list is longer, so that its
  // units land on this game's.
  flags = gGameFlagBytes;
  Iwram_CopyWords(flags, TRANSFERRED_FLAGS,
                  SAVE_IMAGE_MAIN_ROUTINE_WORK - SAVE_IMAGE_GAME_FLAGS +
                      offsetof(struct MainRoutineWork, djinnRecovery));
  Iwram_CopyWords(flags +
                      (SAVE_IMAGE_MAIN_ROUTINE_WORK - SAVE_IMAGE_GAME_FLAGS) +
                      (SAVE_IMAGE_UNITS - BROKEN_SEAL_SAVE_IMAGE_UNITS),
                  TRANSFERRED_MAIN_ROUTINE_WORK,
                  BROKEN_SEAL_SAVE_IMAGE_UNITS + sizeof(gCharacterUnits) -
                      SAVE_IMAGE_MAIN_ROUTINE_WORK);
  Unit_Calculate(0);
  Unit_Calculate(1);
  Unit_Calculate(2);
  Unit_Calculate(3);
  Iwram_CopyWords(TRANSFERRED_FLAGS, flags, SAVE_IMAGE_STATE_BYTES);
  FieldWindowColor_Apply(gMainRoutineWork.windowHue,
                         gMainRoutineWork.windowBrightness);
  return 1;
}

#include "../window/screens_set_text_tile_rect.c"

extern u8 gClear_FourthPasswordRow[];

extern u8 MsgFieldPasswordKana[];

void Text_DrawMessageAsTiles(s32 character, struct Window *position,
                             u32 offsetX, u32 offsetY);
u16 Clear_CalculatePasswordCrc16(u32 length, u8 *p);
void Resource_CopyFixedBlockB(void);
void Resource_CopyFixedBlockA(void);
void Graphics_ExpandVramTilesByColorTable(void *vram);
void MenuCursor_Advance(struct MenuCursor *cursor);
void MenuCursor_Set(struct MenuCursor *cursor, s32 x, s32 y, s32 frames);
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
#define CURSOR_ON_SIDE_COLUMN(column) ((column) == SIDE_COLUMN)
#else
#define SIDE_COLUMN 16
#define SIDE_HIGHLIGHT_WIDTH 6
#define CHECK_WINDOW_X 5
#define CHECK_WINDOW_WIDTH 20
#define CURSOR_ON_SIDE_COLUMN(column) ((column) > SIDE_COLUMN - 1)
#endif

#if (LANGUAGE == LANGUAGE_JAPANESE)
/// Draws the kana to choose from and the side column's Space, Return and
/// Finish into the lower window.
#define DRAW_KEYS(window)                                                      \
  {                                                                            \
    message = (s32)MsgFieldPasswordKana;                                       \
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
/// Draws the symbols to choose from and the side column's Space, Return and
/// Finish into the lower window.
#define DRAW_KEYS(window)                                                      \
  {                                                                            \
    Text_DrawStringInWindow(gClear_FirstPasswordRow, window, 0, 0);            \
    Text_DrawStringInWindow(gClear_SecondPasswordRow, window, 0, 8);           \
    Text_DrawStringInWindow(gClear_ThirdPasswordRow, window, 0, 16);           \
    Text_DrawStringInWindow(gClear_FourthPasswordRow, window, 0, 24);          \
    message = (s32)MsgFieldSpace;                                              \
    Window_DrawDividerLine(window, 20, 0, 20, 5);                              \
    Text_DrawMessageAsTiles(message++, window, 160, 0);                        \
    Text_DrawMessageAsTiles(message++, window, 160, 8);                        \
    Text_DrawMessageAsTiles(message, window, 160, 24);                         \
  }
#endif

/// The password screen: chooses the password's level, then lets the player
/// enter it on the grid of symbols, checks it and applies it; returns -1 when
/// cancelled.
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
  s32 count;
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
  // The highlight is the window palette with a grey background.
  palette = (u16 *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0);
  Dma_Set((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette,
          DMA_COPY_WORDS(8), REG_DMA3);
  palette[WINDOW_BACKGROUND_COLOR] = RGB(24, 24, 24);
  mode = IconMenu_ChooseSendOption(RESOURCE_LAYOUT_PASSWORD_LEVEL);
  if (mode == -1)
    return -1;
  Sound_Play(MUS_PASSWORD_ENTRY);
  cursor = 0;
  column = 0;
  row = 0;
  work = Heap_AllocEwram(sizeof(struct PasswordWork));
  advance = 0;
  Graphics_ExpandVramTilesByColorTable(BG_CHAR_BLOCK(0) +
                                       PAGE_DIGITS_TILE * TILE_SIZE_4BPP);
  if (mode == PASSWORD_LEVEL_GOLD) {
    length = PASSWORD_GOLD_LENGTH;
  } else {
    length = PASSWORD_SILVER_LENGTH;
    if (mode != PASSWORD_LEVEL_SILVER)
      length = PASSWORD_BRONZE_LENGTH;
  }
  pages = length / PASSWORD_SYMBOLS_PER_PAGE + 1;
  window = Window_Create(1, 13, 29, 6, WINDOW_FLAG_INSTANT);
  DRAW_KEYS(window)
  grid = Window_Create(5, 1, 20, 12, WINDOW_FLAG_INSTANT);
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
    Iwram_ClearWordsEntry(work->cells[i], GLYPH_SPRITE_BYTES);
  }
  entry = VramCache_FindFreeEntry();
  if (entry < VRAM_CACHE_ENTRY_COUNT) {
    Resource_CopyFixedBlockB();
    sprite = WindowSprite_Create(entry, OUTPUT_SIZE_16X16,
                                 (struct Window *)window, 0, 0);
    Cursor_Attach(&cursorStore, sprite);
    Cursor_Place(&cursorStore, (s16)window->x * 8 - 2, (s16)window->y * 8 + 12);
  }
  entry = VramCache_FindFreeEntry();
  if (entry < VRAM_CACHE_ENTRY_COUNT) {
    Resource_CopyFixedBlockA();
    sprite = WindowSprite_Create(entry, OUTPUT_SIZE_16X16,
                                 (struct Window *)window, 0, 0);
    Cursor_Attach(&gridStore, sprite);
    sprite->depth = OAM_DEPTH_FRONT;
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
    Window_SetTextTileRect(window, drawColumn, drawRow, drawMode, height,
                           WINDOW_PALETTE);
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
        if (column == SIDE_COLUMN && row == 2)
          row = 1;
      }
    }
    if (column == SIDE_COLUMN)
      drawMode = SIDE_HIGHLIGHT_WIDTH;
    else
      drawMode = 1;
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
    Window_SetTextTileRect(window, drawColumn, row, drawMode, height,
                           HIGHLIGHT_PALETTE);
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
      x += gClear_CursorBob[frame];
      y += gClear_CursorBob[(frame + 5) & 7];
      Cursor_Show(&gridStore, x, y);
    }
    if (advance > 0 && ++advance == 8) {
      advance = 0;
      cursor++;
    }
    if (advance == 0) {
      u16 raw;
      struct InputState *input = &gInput;

      if (column == SIDE_COLUMN && row == 0)
        raw = input->repeat;
      else if (column == SIDE_COLUMN && row == 1)
        raw = input->repeat;
      else
        raw = input->pressed;
      if ((raw & KEY_A) && state == 0) {
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
                                        gClear_DecodedPassword);
          crcLength = action - PASSWORD_CRC_BYTES;
          check =
              Clear_CalculatePasswordCrc16(crcLength, gClear_DecodedPassword);

          state = 0;
          box = Window_Create(CHECK_WINDOW_X, 12, CHECK_WINDOW_WIDTH, 3,
                              WINDOW_FLAG_INSTANT);
          crc = check;
          if (gClear_DecodedPassword[crcLength] == (u16)crc >> 8 &&
              gClear_DecodedPassword[action - 1] == (u16)crc % 256) {
            Text_DrawMessageAsTiles((s32)MsgFieldPasswordAccepted, box, 0, 0);
            Sound_Play(SE_MENU_CONFIRM);
            do {
              System_Wait(1);
            } while (!(gInput.pressed & KEY_A));
            Sound_Play(SE_MENU_CONFIRM);
            MainRoutineWork_InitializeDefaults();
            party = &gMainRoutineWork;
            party->leaderActorId = ACTOR_ISAAC;
            Party_RemoveMember(FELIX);
            Party_RemoveMember(JENNA);
            Party_RemoveMember(SHEBA);
            Party_RemoveMember(PIERS);
            Party_AddMember(ISAAC);
            Party_AddMember(GARET);
            Party_AddMember(IVAN);
            Party_AddMember(MIA);
            Clear_ApplyImportedData(mode, gClear_DecodedPassword);
            Window_Close(box, 2);
            Window_Close(grid, 2);
            Window_Close(window, 2);
            WindowColor_Apply(party->windowHue, party->windowBrightness);
            Sound_Play(MUS_THE_ADVENTURE_RESUMES);
            System_Wait(6);
            NameEntry_EditMemberName(0);
            StatusScreen_Run();
            // DEAD: the flags and party copied where RESTORE_SAVE_IMAGE
            // copies the whole state next; the ROM keeps the copy.
            Iwram_CopyWords(TRANSFERRED_FLAGS, gGameFlagBytes,
                            SAVE_IMAGE_STATE_BYTES);
            return 0;
          }
          Sound_Play(SE_MENU_CANCEL);
          Text_DrawMessageAsTiles((s32)MsgFieldInvalidPassword, box, 0, 0);
          do {
            System_Wait(1);
          } while (!(gInput.pressed & KEY_A) && !(gInput.pressed & KEY_B));
          Window_Close(box, 2);
          WindowSprite_RedrawSavedRect(window);
          DRAW_KEYS(window)
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
            Sound_Play(MUS_THE_ADVENTURE_RESUMES);
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
        s32 tile = i + PAGE_NUMBER_TILE;

        if (i == page)
          tile = i + PAGE_NUMBER_LIT_TILE;
        Window_SetTilemapEntry(grid, tile, i - pages + 18, 10, 0);
      }
    }
    if (length < PASSWORD_SYMBOLS_PER_PAGE)
      count = length;
    else if (pages - 1 == page)
      count = length % PASSWORD_SYMBOLS_PER_PAGE;
    else
      count = PASSWORD_SYMBOLS_PER_PAGE;
    for (i = 0; i != count; i++) {
      u8 *entered = gClear_PasswordText;
      s32 index = i + page * PASSWORD_SYMBOLS_PER_PAGE;
      s32 raw = entered[index];
      u8 code = raw;

      if (code != work->shown[i]) {
        work->shown[i] = raw;
        if (code == PASSWORD_SYMBOL_EMPTY) {
          Iwram_ClearWordsEntry(work->cells[i], GLYPH_SPRITE_BYTES);
        } else {
          Clear_GetPasswordCharacter(entered[index], glyph);
          Glyph_DrawToTiles(glyph[0], work->cells[i]);
        }
      }
    }
    for (; i != PASSWORD_SYMBOLS_PER_PAGE; i++) {
      if (work->shown[i] != PASSWORD_SYMBOL_EMPTY) {
        work->shown[i] = PASSWORD_SYMBOL_EMPTY;
        Iwram_ClearWordsEntry(work->cells[i], GLYPH_SPRITE_BYTES);
      }
    }
    System_Wait(1);
  }
}

/// The grid of the window tilemap the two unused routines below fill: 16 by
/// 8 entries; the second's tiles start at TILE_GRID_FIRST_TILE.
#define TILE_GRID_COLUMNS 16
#define TILE_GRID_ROWS 8
#define TILE_GRID_FIRST_TILE 32

/// Unused: unpacks the tiles into a scratch buffer, then fills a grid of the
/// window tilemap, in VRAM and in its shadow, at the window's cell: each
/// cell's tile the row times the window's width plus the column.
UNUSED void Clear_FillTileGrid(struct Window *window,
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
  cell = (s16)window->y * BG_SCREEN_WIDTH + (s16)window->x;
  vram = (u16 *)WINDOW_TILEMAP_VRAM + cell;
  shadow = &work->tilemap[0][cell];
  for (row = 0; row < TILE_GRID_ROWS; row++) {
    for (col = 0; col < TILE_GRID_COLUMNS; col++) {
      s16 tile = WINDOW_TILE(window->width * row + col);

      *vram++ = tile;
      *shadow++ = tile;
    }
    vram += BG_SCREEN_WIDTH - TILE_GRID_COLUMNS;
    shadow += BG_SCREEN_WIDTH - TILE_GRID_COLUMNS;
  }
  Heap_Release(decodedTiles);
}

/// Unused: fills a grid of the window tilemap with the tiles from
/// TILE_GRID_FIRST_TILE on.
UNUSED void Clear_FillTileGridInOrder(struct Window *window) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  u16 *shadow;
  u16 *vram;
  void *buffer;
  s32 row;
  s32 col;
  s32 cell;

  buffer = Heap_AllocEwram(768);
  cell = (s16)window->y * BG_SCREEN_WIDTH + (s16)window->x;
  vram = (u16 *)WINDOW_TILEMAP_VRAM + cell;
  shadow = &work->tilemap[0][cell];
  for (row = 0; row < TILE_GRID_ROWS; row++) {
    for (col = 0; col < TILE_GRID_COLUMNS; col++) {
      s16 tile =
          WINDOW_TILE(row * TILE_GRID_COLUMNS + col + TILE_GRID_FIRST_TILE);

      *vram++ = tile;
      *shadow++ = tile;
    }
    vram += BG_SCREEN_WIDTH - TILE_GRID_COLUMNS;
    shadow += BG_SCREEN_WIDTH - TILE_GRID_COLUMNS;
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

  // Bytes 0 to 6 hold the four levels, seven bits each, then the 28 Djinn, a
  // bit each, sharing byte 3.
  levelBits = in[0] | in[1] << 8 | in[2] << 16 | (in[3] & 0xf0) << 20;
  djinnBits = (in[3] & 0x0f) | in[4] << 4 | in[5] << 12 | in[6] << 20;
  flagBits = in[PASSWORD_FLAGS_BYTE];
  keyItemBits = in[PASSWORD_KEY_ITEMS_BYTE];

  // Silver and Gold carry the stats rows. Each packs two party members in
  // PASSWORD_STATS_ROW_BYTES: each one's first word and the top 28 bits of its
  // second, whose low four bits are always 0.
  if (mode != PASSWORD_LEVEL_BRONZE) {
    s32 statsOffset = PASSWORD_KEY_ITEMS_BYTE + (mode != PASSWORD_LEVEL_GOLD);

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
      statsOffset += PASSWORD_STATS_ROW_BYTES;
    }
    for (i = 0; i != 4; i++) {
      struct BattleBaseStats *stats =
          (struct BattleBaseStats *)&Unit_Get(gClear_ImportedMembers[i])
              ->baseHp;
      s32 at = i * 8;

#define WORD (*(u32 *)((u8 *)packedStats + at))
      stats->baseHp = WORD >> PASSWORD_HP_SHIFT;
      stats->basePp = WORD >> PASSWORD_PP_SHIFT & PASSWORD_PP_MASK;
      stats->baseAttack = WORD & PASSWORD_STAT_MASK;
      at += 4;
      stats->baseDefense = WORD >> PASSWORD_DEFENSE_SHIFT & PASSWORD_STAT_MASK;
      stats->baseAgility = WORD >> PASSWORD_AGILITY_SHIFT & PASSWORD_STAT_MASK;
      stats->baseLuck = WORD >> PASSWORD_LUCK_SHIFT & PASSWORD_LUCK_MASK;
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

    *(u8 *)&unit->level = levelBits >> (i * 7) & PASSWORD_LEVEL_MASK;
#if TLA_PATCH_1
    unit->experience = Unit_GetExperienceForLevel(i, unit->level);
#endif
  }

  Djinn_GetRecoveryTable(0)->list.count = 0;
  for (i = 0; i != 4; i++) {
    struct Unit *unit = Unit_Get(gClear_ImportedMembers[i]);

    // DEAD: the three clears after the first; the ROM keeps the loop.
    for (n = 0; n != 4; n++)
      Iwram_ClearWordsEntry(unit->djinnHeld, UNIT_DJINN_BYTES);
  }

  for (i = 0; i != ELEMENT_COUNT * TRANSFERRED_DJINN_PER_ELEMENT; i++) {
    s32 element = i / TRANSFERRED_DJINN_PER_ELEMENT;
    s32 number = i % TRANSFERRED_DJINN_PER_ELEMENT;

    GameFlag_Clear(FLAG_DJINNI(element * DJINN_PER_ELEMENT + number));
  }

  for (i = 0; i != ELEMENT_COUNT * TRANSFERRED_DJINN_PER_ELEMENT; i++) {
    if ((djinnBits >> i) & 1)
      Djinn_GiveToParty(i / TRANSFERRED_DJINN_PER_ELEMENT,
                        i % TRANSFERRED_DJINN_PER_ELEMENT);
  }

  if (mode != PASSWORD_LEVEL_GOLD) {
    Unit_Get(gClear_ImportedMembers[0]);
    for (i = 0; i != 8; i++) {
      if ((keyItemBits >> i) & 1)
        PartyInventory_Add(gClear_ImportedKeyItems[i]);
    }
  }

  if (mode == PASSWORD_LEVEL_GOLD) {
    position = PASSWORD_ITEMS_BYTE;
    bit = 0;
    for (i = 0; i != 4; i++) {
      struct Unit *unit = Unit_Get(gClear_ImportedMembers[i]);

      for (n = 0; n != INVENTORY_SLOTS; n++) {
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

    position = PASSWORD_QUANTITIES_BYTE;
    bit = -1;
    for (i = 0; i != 4; i++) {
      struct Unit *unit = Unit_Get(gClear_ImportedMembers[i]);

      for (m = 0; m != PASSWORD_STACK_ITEMS; m++) {
        j = 0;
        if (bit < 0) {
          j = in[position] << -bit & PASSWORD_QUANTITY_MASK;
          position++;
          bit += 8;
        }
        j += in[position] >> bit & PASSWORD_QUANTITY_MASK;
        bit -= 5;
        if (bit == -5) {
          position++;
          bit = 3;
        }
        j &= PASSWORD_QUANTITY_MASK;
        for (n = 0; n != INVENTORY_SLOTS; n++) {
          if ((unit->inventory[n] & INVENTORY_ITEM_ID_MASK) ==
              gClear_ImportedStackItems[m])
            unit->inventory[n] |= j << INVENTORY_QUANTITY_SHIFT;
        }
      }
    }

    gMainRoutineWork.coins = in[PASSWORD_COINS_BYTE] << 16 |
                             in[PASSWORD_COINS_BYTE + 1] << 8 |
                             in[PASSWORD_COINS_BYTE + 2];
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

  length = PASSWORD_BRONZE_BYTES;
  switch (mode) {
  case PASSWORD_LEVEL_GOLD:
    length = PASSWORD_GOLD_BYTES - PASSWORD_CRC_BYTES;
    break;
  case PASSWORD_LEVEL_SILVER:
    length = PASSWORD_SILVER_BYTES - PASSWORD_CRC_BYTES;
    break;
  case PASSWORD_LEVEL_BRONZE:
    length = PASSWORD_BRONZE_BYTES - PASSWORD_CRC_BYTES;
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
    struct BattleBaseStats *base = (struct BattleBaseStats *)&unit->baseHp;
    s32 value;

    if (base->baseHp > UNIT_CHARACTER_HP_MAX)
      base->baseHp = UNIT_CHARACTER_HP_MAX;
    value = (u16)base->baseHp;
    if ((s16)value < 0)
      base->baseHp = 0;
    if (base->basePp > UNIT_CHARACTER_HP_MAX)
      base->basePp = UNIT_CHARACTER_HP_MAX;
    value = (u16)base->basePp;
    if ((s16)value < 0)
      base->basePp = 0;
    if (base->baseAttack > UNIT_STAT_MAX)
      base->baseAttack = UNIT_STAT_MAX;
    value = base->baseAttack;
    if (value < 0)
      value = 0;
    base->baseAttack = value;
    if (base->baseDefense > UNIT_STAT_MAX)
      base->baseDefense = UNIT_STAT_MAX;
    value = base->baseDefense;
    if (value < 0)
      value = 0;
    base->baseDefense = value;
    if (base->baseAgility > UNIT_STAT_MAX)
      base->baseAgility = UNIT_STAT_MAX;
    value = base->baseAgility;
    if (value < 0)
      value = 0;
    base->baseAgility = value;
    if (base->baseLuck > UNIT_LUCK_MAX)
      base->baseLuck = UNIT_LUCK_MAX;
    value = base->baseLuck;
    if (value < 0)
      value = 0;
    base->baseLuck = value;
    packedStats[i * 2] = (u32)base->baseHp << PASSWORD_HP_SHIFT |
                         base->basePp << PASSWORD_PP_SHIFT | base->baseAttack;
    *(u32 *)((u8 *)packedStats + outputIndex) =
        (u32)base->baseDefense << PASSWORD_DEFENSE_SHIFT |
        base->baseAgility << PASSWORD_AGILITY_SHIFT |
        base->baseLuck << PASSWORD_LUCK_SHIFT;
    if (unit->level > UNIT_LEVEL_MAX)
      unit->level = UNIT_LEVEL_MAX;
    value = unit->level;
    if (value == 0)
      unit->level = 1;
    levelBits |= unit->level << (i * 7);
    for (j = 0; j != 4; j++)
      djinnBits += unit->djinnHeld[j] << (j * 7);
    // Which of the eight Psynergy items (gClear_PasswordKeyItems) any party
    // member carries.
    for (j = 0; j != INVENTORY_SLOTS; j++) {
      s32 item = unit->inventory[j] & INVENTORY_ITEM_ID_MASK;

      for (k = 0; k != 8; k++) {
        if (item == gClear_PasswordKeyItems[k])
          keyItemBits |= 1 << k;
      }
    }
    outputIndex += 8;
  }
  // The largest password also carries inventories, stack counts and coins.
  if (mode == PASSWORD_LEVEL_GOLD) {
    outputIndex = PASSWORD_ITEMS_BYTE;
    bitOffset = 0;
    for (i = 0; i != 4; i++) {
      struct Unit *unit = Unit_Get(gClear_PasswordMembers[i]);

      for (n = 0; n != INVENTORY_SLOTS; n++) {
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
    outputIndex = PASSWORD_QUANTITIES_BYTE;
    bitOffset = -1;
    for (i = 0; i != 4; i++) {
      struct Unit *unit = Unit_Get(gClear_PasswordMembers[i]);

      for (m = 0; m != PASSWORD_STACK_ITEMS; m++) {
        s32 count = 0;
        u16 quantity;
        /* FAKEMATCH(dead): the form kept; as no quantity copy. */

        for (n = 0; n != INVENTORY_SLOTS; n++) {
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
    out[PASSWORD_COINS_BYTE] = (u32)gMainRoutineWork.coins >> 16;
    out[PASSWORD_COINS_BYTE + 1] = (u32)gMainRoutineWork.coins >> 8;
    out[PASSWORD_COINS_BYTE + 2] = gMainRoutineWork.coins;
  }
  // Silver and Gold carry the stats rows. Each packs two party members in
  // PASSWORD_STATS_ROW_BYTES: each one's first word and the top 28 bits of its
  // second, whose low four bits are always 0.
  if (mode != PASSWORD_LEVEL_BRONZE) {
    s32 statsOffset = PASSWORD_KEY_ITEMS_BYTE + (mode != PASSWORD_LEVEL_GOLD);

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
      statsOffset += PASSWORD_STATS_ROW_BYTES;
    }
  }
  out[0] = levelBits;
  out[1] = levelBits >> 8;
  out[2] = levelBits >> 16;
  out[3] = (levelBits >> 20 & 0xf0) | (djinnBits & 0x0f);
  out[4] = djinnBits >> 4;
  out[5] = djinnBits >> 12;
  out[6] = djinnBits >> 20;
  out[PASSWORD_FLAGS_BYTE] = flagBits;
  if (mode != PASSWORD_LEVEL_GOLD)
    out[PASSWORD_KEY_ITEMS_BYTE] = keyItemBits;
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

/// The password's checksum, as The Broken Seal's
/// FileScreen_CalculatePasswordCrc16 makes it: over length bytes at p, shifted
/// a bit at a time from PASSWORD_CRC_SEED, PASSWORD_CRC_POLYNOMIAL subtracted
/// when the top bit falls out, and the result complemented. The password
/// screen checks it against a decoded password's last PASSWORD_CRC_BYTES.
u16 Clear_CalculatePasswordCrc16(u32 length, u8 *p) {
  u32 crc;
  u32 i;
  u32 j;

  crc = PASSWORD_CRC_SEED;
  i = 0;
  if (length != 0) {
    do {
      crc ^= *p << 8;
      j = 0;
      do {
        u32 high = crc & PASSWORD_CRC_TOP_BIT;
        u32 next = crc << 1;

        j++;
        crc = next - PASSWORD_CRC_POLYNOMIAL;
        if (high == 0)
          crc = next;
      } while (j != 8);
      i++;
      p++;
    } while (i != length);
  }
  return ~crc;
}

// clang-format off
/// The first secret code, as The Broken Seal's, ended by 0: Select three
/// times. With it a new game names Jenna, Sheba and Piers as well as Felix
/// (Scene_Initialize).
u16 gClear_ButtonCode[] = {
    KEY_SELECT, KEY_SELECT, KEY_SELECT, 0,
};

/// The second, longer code. With it a new game also names Garet, Ivan and Mia.
u16 gClear_ExtraButtonCode[] = {
    KEY_UP, KEY_DOWN, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_LEFT, KEY_RIGHT,
    KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_LEFT, KEY_UP, KEY_SELECT, 0,
#if TLA_PATCH_1
    0,
#endif
};

/// The grid cursor's bobbing offsets in pixels, which Clear_EnterPassword adds
/// to its x at step (gVBlankCount >> 1) & 7 and to its y five steps on, so
/// that it circles.
s8 gClear_CursorBob[] = {
    0, 1, 2, 3, 3, 2, 1, 0,
};

#if (LANGUAGE != LANGUAGE_JAPANESE)
/// The password grid's four rows of symbols as DRAW_KEYS writes them, five to
/// a group (the Japanese grid is kana, MsgFieldPasswordKana).
u8 gClear_FirstPasswordRow[] = "ABCDE FGHJK LMNPQ R";
u8 gClear_SecondPasswordRow[] = "STUVW XYZ23 45678 9";
u8 gClear_ThirdPasswordRow[] = "abcde fghij kmnpq r";
u8 gClear_FourthPasswordRow[] = "stuvw xyz!? #&$%+ =";
#endif

/// The six story flags of a password's PASSWORD_FLAGS_BYTE, by The Broken
/// Seal's numbers, which DataTransfer_Apply reads them by.
u16 gClear_ImportedFlags[] = {
    FLAG_BROKEN_SEAL_DODONPA_LOCKED_UP,
    FLAG_BROKEN_SEAL_WON_COLOSSO,
    FLAG_BROKEN_SEAL_HSU_RESCUED_WITHOUT_PARTY,
    FLAG_BROKEN_SEAL_CROSSBONE_ISLE_DEADBEARD_DEFEATED,
    FLAG_BROKEN_SEAL_VISITED_SICK_DORA,
    FLAG_BROKEN_SEAL_VAULT_WARNED_OF_ESCAPED_THIEVES,
};
// clang-format on

#include "shared/scene_entrances_mapless.c"

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

#undef YEAH_WIDTH
#undef COPY_SAVE_IMAGE
#undef SAVE_IMAGE_BYTES
#undef KEEP_SAVE_IMAGE
#undef RESTORE_SAVE_IMAGE
#undef RETURN_TO_SAVED
#undef TRANSFERRED_FLAGS
#undef TRANSFERRED_MAIN_ROUTINE_WORK
#undef TRANSFERRED_SAVE
#undef LINK_LANGUAGE_TAG
#undef RECEIVE_WINDOW_X
#undef RECEIVE_WINDOW_WIDTH
#undef STATUS_WINDOW_X
#undef STATUS_WINDOW_WIDTH
#undef SIDE_COLUMN
#undef SIDE_HIGHLIGHT_WIDTH
#undef CHECK_WINDOW_X
#undef CHECK_WINDOW_WIDTH
#undef CURSOR_ON_SIDE_COLUMN
#undef DRAW_KEYS
#undef TILE_GRID_COLUMNS
#undef TILE_GRID_ROWS
#undef TILE_GRID_FIRST_TILE
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

/// The same flags, as the unused Clear_PackPasswordData reads them.
LATE_DATA u16 gClear_PasswordFlags[] = {
    FLAG_BROKEN_SEAL_DODONPA_LOCKED_UP,
    FLAG_BROKEN_SEAL_WON_COLOSSO,
    FLAG_BROKEN_SEAL_HSU_RESCUED_WITHOUT_PARTY,
    FLAG_BROKEN_SEAL_CROSSBONE_ISLE_DEADBEARD_DEFEATED,
    FLAG_BROKEN_SEAL_VISITED_SICK_DORA,
    FLAG_BROKEN_SEAL_VAULT_WARNED_OF_ESCAPED_THIEVES,
};

// The same list as gClear_ImportedKeyItems, under another name.
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
