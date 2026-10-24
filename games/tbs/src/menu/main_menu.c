/// @file
/// The main menu overlay: the Golden Sun logo, the start menu (new game,
/// continue, copy and delete), the door to the Battle Arena and the transfer of
/// a save's data, by link or as a password.

#include "map_ids.h"
#include "unit/menu/main_menu.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "heap_slots.h"
#include "edition.h"
#include "tbs_edition.h"
#include "text_font.h"
#include "types.h"
#include "dma.h"
#include "ram_buffer.h"
#include "script_layer.h"
#include "engine_call.h"
#include "resource_ids.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "map_tables.h"
#include "serial_runtime.h"
#include "mem.h"
#include "window.h"
#include "item.h"
#include "save_block.h"
#include "text/msg_ids.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "frame.h"
#include "map_grid_scroll.h"
#include "flags.h"
#include "title.h"

// Edition-specific text and window layout values.
// The password buffer: a Gold password is 260 symbols
// (MainMenu_EncodePassword) in every edition, which the Japanese buffer holds
// exactly. XXX (no evidence): why the other editions reserve 320; nothing in
// this game writes or reads past 260, so the bytes show only the space.
#if LANGUAGE != LANGUAGE_JAPANESE
#define MAIN_MENU_PASSWORD_BUFFER_SIZE 320
#else
#define MAIN_MENU_PASSWORD_BUFFER_SIZE 260
#endif

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define MAIN_MENU_LINK_WINDOW_X 4
#define MAIN_MENU_LINK_WINDOW_WIDTH 22
#else
#define MAIN_MENU_LINK_WINDOW_X 6
#define MAIN_MENU_LINK_WINDOW_WIDTH 18
#endif

// The first letter of the greeting a cable transfer waits for, before V, T
// and S (MainMenu_Run).
#if (LANGUAGE == LANGUAGE_GERMAN)
#define MAIN_MENU_LINK_LANGUAGE_TAG 'E'
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define MAIN_MENU_LINK_LANGUAGE_TAG 'F'
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define MAIN_MENU_LINK_LANGUAGE_TAG 'S'
#elif (LANGUAGE == LANGUAGE_ITALIAN)
#define MAIN_MENU_LINK_LANGUAGE_TAG 'I'
#elif (LANGUAGE == LANGUAGE_JAPANESE)
#define MAIN_MENU_LINK_LANGUAGE_TAG 'C'
#else
#define MAIN_MENU_LINK_LANGUAGE_TAG 'U'
#endif

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define MAIN_MENU_LINK_ERROR_WINDOW_X 4
#define MAIN_MENU_LINK_ERROR_WINDOW_WIDTH 22
#else
#define MAIN_MENU_LINK_ERROR_WINDOW_X 5
#define MAIN_MENU_LINK_ERROR_WINDOW_WIDTH 20
#endif

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define MAIN_MENU_HELP_WINDOW_X 8
#define MAIN_MENU_HELP_WINDOW_WIDTH 13
#else
#define MAIN_MENU_HELP_WINDOW_X 10
#define MAIN_MENU_HELP_WINDOW_WIDTH 10
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
#define MAIN_MENU_PAGE_LABEL_X 80
#else
#define MAIN_MENU_PAGE_LABEL_X 0
#endif

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define MAIN_MENU_MESSAGE_WINDOW_X 0
#define MAIN_MENU_MESSAGE_WINDOW_WIDTH 30
#elif (LANGUAGE == LANGUAGE_JAPANESE)
#define MAIN_MENU_MESSAGE_WINDOW_X 3
#define MAIN_MENU_MESSAGE_WINDOW_WIDTH 24
#else
#define MAIN_MENU_MESSAGE_WINDOW_X 1
#define MAIN_MENU_MESSAGE_WINDOW_WIDTH 28
#endif

#define PASSWORD_SYMBOLS_PER_PAGE 50 ///< five rows of ten
#define PASSWORD_SYMBOL_MASK 0x3f    ///< six bits: 64 symbols

/// Whether a key of the extra code is still held. UB: the flag is a word the
/// code reads and writes as its low halfword; UBFIX uses the word.
#ifdef UBFIX
#define MAIN_MENU_EXTRA_CODE_HELD gMainMenu_ExtraCodeHeld
#else
#define MAIN_MENU_EXTRA_CODE_HELD (*(s16 *)&gMainMenu_ExtraCodeHeld)
#endif

static s32 MainMenu_PackPasswordData(s32 unused, s32 mode, u8 *out);
static s32 MainMenu_EncodePassword(u8 *data, s32 size, u8 *out);
static u16 MainMenu_CalculatePasswordCrc16(u32 byteCount, u8 *data);

/// The main menu overlay's map tables, for the map loader: one warp
/// destination, the warp, and no object event templates or map events.
u8 *MainMenu_GetWarpDests(void) { return (u8 *)gMainMenu_WarpDests; }

/// The warp regions entry: the main menu has no warp boxes.
s32 Map_GetWarpRegions(void) { return 0; }

u8 *MainMenu_GetWarps(void) { return (u8 *)gMainMenu_Warps; }

u8 *MainMenu_GetObjectEventTemplates(void) {
  return (u8 *)gMainMenu_ObjectEventTemplates;
}

u8 *MainMenu_GetEvents(void) { return (u8 *)gMainMenu_Events; }

/// Loads the Golden Sun logo onto BG2 as Title_LoadBackground (title.c) does,
/// and stops the leader's motion. MainMenu_Run calls it first.
static void MainMenu_LoadBackground(void) {
  u8 *resource;
  s32 resourceId;
  u16 *tilemap;
  u32 x;
  u32 y;
  u16 tile;
  struct BgScroll *scroll;
  s32 blankTile;

  resourceId = (s32)&ResourceId_GoldenSunLogo;
  ScreenFade_ToBlack(0);
  REG_BG2CNT = LOGO_BGCNT;
  gBgScroll[2].y = 0;
  blankTile = LOGO_BLANK_TILE;
  resource = Resource_GetTableEntry(resourceId);
  Dma_Set(resource, (void *)BG_PLTT, (DMA_ENABLE | DMA_32BIT) << 16 | 0x70,
          REG_DMA3);
  resource += 0x1c0;
  FieldDecompress_TaggedLz(resource, Ram_MapGridCellBuffer);
  Dma_Set((void *)Ram_MapGridCellBuffer, LOGO_TILES,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x2580, REG_DMA3);
  // Build the visible 30-column logo map, padding to the 32-cell screen stride.
  tilemap = LOGO_MAP;
  tile = LOGO_FIRST_TILE;
  for (y = 0; y < 20; y++) {
    for (x = 0; x < 30; x++)
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
  Dma_Set(gBgScroll, REG_BG0HOFS, (DMA_ENABLE | DMA_32BIT) << 16 | 4, REG_DMA3);
  ((struct MapGridScrollWork *)gMapGridWork[0])->flags =
      DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  {
    struct ObjectEvent *leader;

    leader = ObjectEvent_GetById(gSaveBlock.leaderLocalId);
    leader->motionFlags = 0;
  }
}

/// The task that fades BG0 and the sprites in over the logo:
/// their weight rises by a sixteenth every two frames, and the callback
/// removes itself at full weight, 32 frames on. MainMenu_Run schedules it
/// once, when there is no save file to choose.
static void MainMenu_UpdateBlend(void) {
  s32 frame;
  s32 level;

  frame = *(u16 *)&gMainMenu_BlendFrame + 1;
  *(u16 *)&gMainMenu_BlendFrame = frame;
  level = (u16)frame >> 1;
  // Blend BG0 and the sprites over the other layers, level sixteenths of
  // the way in.
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT1_BG0 | BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND |
                          BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 |
                          BLDCNT_TGT2_BD,
                      IO_WRITE_16);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                      BLEND_ALPHA((u16)level, 16 - (u16)level), IO_WRITE_16);
  if ((u16)level > 15)
    Task_Remove(MainMenu_UpdateBlend);
}

// clang-format off
/// The first hidden code, entered on the main menu and ended by 0:
/// Select three times. With it a new game names Garet, Ivan and Mia as well
/// as Isaac (MainMenu_Run).
const u16 gMainMenu_CodeSequence[] = {
    KEY_SELECT, KEY_SELECT, KEY_SELECT, 0,
};

/// The second, longer code: with it a new game also names Felix, Jenna and
/// Sheba.
const u16 gMainMenu_ExtraCodeSequence[] = {
    KEY_UP, KEY_DOWN, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_LEFT, KEY_RIGHT,
    KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_LEFT, KEY_UP, KEY_SELECT, 0,
};
// clang-format on

// The actual arrays have four and fifteen entries. The original one-element
// view makes the second key an out-of-bounds access. Direct array reads also
// reorder both native held-key stores (same 0xf8-byte function extent).
#ifdef UBFIX
#define MAIN_MENU_SEQUENCE_KEYS(sequence) (sequence)
#else
#define MAIN_MENU_SEQUENCE_KEYS(sequence)                                      \
  (((struct { u16 key[1]; } *)(sequence))->key)
#endif

/// The task that follows the two hidden codes each frame while
/// the main menu runs. Each key must be held alone and then every
/// key released; any other key starts that code over. A finished code sets
/// its unlocked flag, which stays set, and plays SE_CHIME.
static void MainMenu_CheckButtonCodes(void) {
  if (gMainMenu_CodeUnlocked == 0) {
    if (gMainMenu_CodeHeld != 0) {
      if (gKeysHeld == 0)
        gMainMenu_CodeHeld = 0;
    } else if (gKeysHeld != 0) {
      if (gKeysHeld == MAIN_MENU_SEQUENCE_KEYS(
                           gMainMenu_CodeSequence)[gMainMenu_CodeProgress]) {
        gMainMenu_CodeProgress++;
        gMainMenu_CodeHeld = 1;
        if (MAIN_MENU_SEQUENCE_KEYS(
                gMainMenu_CodeSequence)[gMainMenu_CodeProgress] == 0) {
          gMainMenu_CodeUnlocked = 1;
          Sound_PlaySong(SE_CHIME);
        }
      } else {
        gMainMenu_CodeProgress = 0;
      }
    }
  }
  if (gMainMenu_ExtraCodeUnlocked == 0) {
    if (MAIN_MENU_EXTRA_CODE_HELD != 0) {
      if (gKeysHeld == 0)
        MAIN_MENU_EXTRA_CODE_HELD = 0;
    } else if (gKeysHeld != 0) {
      if (gKeysHeld ==
          MAIN_MENU_SEQUENCE_KEYS(
              gMainMenu_ExtraCodeSequence)[gMainMenu_ExtraCodeProgress]) {
        gMainMenu_ExtraCodeProgress++;
        MAIN_MENU_EXTRA_CODE_HELD = 1;
        if (MAIN_MENU_SEQUENCE_KEYS(
                gMainMenu_ExtraCodeSequence)[gMainMenu_ExtraCodeProgress] ==
            0) {
          gMainMenu_ExtraCodeUnlocked = 1;
          Sound_PlaySong(SE_CHIME);
        }
      } else {
        gMainMenu_ExtraCodeProgress = 0;
      }
    }
  }
}

#undef MAIN_MENU_SEQUENCE_KEYS

/// Converts a six-bit password symbol to the edition's one- or two-byte glyph.
/// Writes the glyph, a second byte (Japanese's voiced mark, else 0) and a
/// terminator into text. Outside Japanese the 64 symbols are A to Z without I
/// and O, 2 to 9, a to z without l and o, and ! ? # & $ % + =; Japanese uses
/// hiragana, the last 18 with the voiced mark. The six left out are the three
/// pairs that look alike (I, l and 1; O, o and 0), so a password copied by hand
/// reads back one way.
static void MainMenu_NamingScreenCharacter(s32 cell, u8 *text) {
  text[1] = 0;
  text[2] = 0;
#if (LANGUAGE != LANGUAGE_JAPANESE)
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
#else
  if (cell <= 14)
    text[0] = cell - 111;
  else if (cell <= 44)
    text[0] = cell - 47;
  else if (cell == 45)
    text[0] = 0x86;
  else {
    if (cell <= 55)
      text[0] = cell + 104;
    else if (cell == 56)
      text[0] = 0xe0;
    else if (cell == 57)
      text[0] = 0xe3;
    else if (cell == 58)
      text[0] = 0xe4;
    else
      text[0] = cell - 81;
    text[1] = TEXT_MARK_VOICED;
  }
#endif
}

/// Returns -1 when the loaded game may not restart at its last Sanctum, 0
/// otherwise. It may not when it was saved where a map script blocked
/// Retreat (FLAG_RETREAT_BLOCKED), except in towns (MAP_TYPE_TOWN), which block
/// it by themselves, and on the world map.
static s32 MainMenu_GetSaveRecoveryBlockMask(void) {
  s32 mapDifference;

  if (GameFlag_Get(FLAG_RETREAT_BLOCKED) == 0) {
    return 0;
  }
  if (gSaveBlock.mapType == MAP_TYPE_TOWN) {
    return 0;
  }
  // -1 unless the map is the world map: -d | d has its top bit set for any
  // d but 0.
  mapDifference = gSaveBlock.mapId ^ (s32)&MapId_WorldMap;
  return -((u32)(-mapDifference | mapDifference) >> 31);
}

/// A view of the map state that names its copy of the play time, checked
/// against the loaded game's.
struct SaveMenuMapLocalState {
  u8 mapAndInventory[0x100]; ///< gMapLocalState, then gInventorySnapshot
  /// The play time in frames, which Save_BuildFileSummary copies here
  /// when saving.
  s32 playTime;
};

/// A console's link payload as the cable transfer reads it from
/// gLinkPeerSignatures.
struct SaveLinkGreeting {
  /// The greeting MainMenu_Run waits for: MAIN_MENU_LINK_LANGUAGE_TAG, then V,
  /// T and S. MainMenu_Run sets every console's to "0000" before it waits.
  u16 code[4];
  u8 reserved[16]; ///< the rest of the 24 bytes; not read
};

/// The compiler library's signed division, which MainMenu_Run calls by name
/// at one site (its FAKEMATCH tag).
s32 __divsi3(s32, s32);

/// The overlay's initialize entry: the main menu. At warp destination
/// 2, after the credits, it offers to save the cleared game and goes on to the
/// world map. Otherwise it loops on the start menu until the player starts a
/// new game, continues a file or enters the Battle Arena; copying, erasing
/// and sending a cleared file's data, by cable or as a password, come back to
/// the menu. Returns 0 once a destination is set.
s32 MainMenu_Run(void) {
  s32 pageCount;
  s32 encodedSize;
  struct Window *pageWindow;
  // Keep encoded integer handles: the earlier typed-window split changes
  // four native bytes. Integer windowHandle/passwordColumn and peerIndex/
  // requiredLinkMask split plainly without changing the output.
  s32 wrappedPage;
  s32 glyphColumn;
  s32 windowHandle;
  s32 passwordColumn;
  s32 tileValue;
  // These three scalars each serve several phases: separate per-phase
  // variables reorder the instructions or shorten the function.
  s32 v5;
  s32 v7;
  s32 v6;
  s32 linkMessage;
  s32 peerIndex;
  s32 requiredLinkMask;
  s32 linkConnectionOk;
  s32 pageGlyphIndex;
  s32 passwordHeadingMessage;
  s32 pageIndex;
  s32 blendStarted;
  struct Window *helpWindow;
  struct Window *headingWindow;
  u8 saveData[200];
  u8 password[MAIN_MENU_PASSWORD_BUFFER_SIZE];
  u8 glyph[4];

  blendStarted = 0;
  MainMenu_LoadBackground();
  FieldScheduler_AddCallback(MainMenu_CheckButtonCodes,
                             TASK_PRIORITY_MAIN_LOOP);
  gFieldWork->transition = blendStarted;
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  // Warp destination 2, after the credits (Title_Run): save the cleared game,
  // then the world map's warp destination 72. Cancelling the file choice asks
  // again.
  if (gSaveBlock.warpDest == 2) {
  L_ask:
    FieldMessage_Show(MsgSaveAdventureQuestion,
                      FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN);
    v7 = FieldYesNo_Ask(1);
    TextPrinter_CloseFinishedWindows();
    if (v7 == 0) {
      FieldMessage_Show(MsgSaveAdventureQuestion + 1, FIELD_MESSAGE_ZOOM);
      // Marks the file cleared (SaveFileSummary's sendFlag): its data can be
      // sent, and the message says it cannot be continued.
      gSaveBlock.sendFlag = 1;
      v7 = Save_ToChosenFile();
      if (v7 == -1) {
        goto L_ask;
      }
    }
    FieldScript_WaitFrames(60);
    FieldScript_FinishScreenTransition();
    Sound_PlaySong(SONG_FADE_OUT_BGM);
    FieldScript_WaitFrames(0x12c);
    FieldScript_SetDestination((s32)&MapId_WorldMap, WORLD_MAP_FROM_MAIN_MENU);
    return 0;
  }
  gSleepDisabled = blendStarted;
  // Choose a new game, existing save, arena entry, or data transfer. This
  // menu, the transfer's choices and the save question above are loops
  // written with goto (MATCHING.md): with the save question as for (;;) and
  // continue, 101 of 920 instructions differ in tbs-en, and with this menu as
  // for (;;), 462.
menu_top:;
  v6 = Save_ScanRecordFlags();
  // No backup memory: in debug mode, say so and turn idle sleep on.
  if (v6 < 0) {
    if (gDebugMode != 0) {
      gSaveBlock.idleSleepEnabled = 1;
      gIdleSleepEnabled = 1;
      FieldMessage_ShowCentered(MsgNoBackupMemory, CENTERED_MESSAGE_WINDOW, 8);
    }
  }
  // No save file: fade the menu layers in, once, and start a new game.
  if (v6 == 0) {
    if (blendStarted == 0) {
      WaitFrames(30);
      FieldScheduler_AddCallback(MainMenu_UpdateBlend, TASK_PRIORITY_MAIN_LOOP);
      WaitFrames(1);
      blendStarted = 1;
    }
  }
  // The start menu's choices (gFileMenu_ActionByPosition): 0 New Game,
  // 1 Continue, 2 Copy, 3 Erase, 4 Battle and 5 Send; any other shows the
  // menu again.
  if (v6 > 0) {
    v6 = FileMenu_SelectAction();
  } else {
    v6 = 0;
  }
  if (v6 == 0) {
    // Hidden button codes expand the list of names offered for a new game:
    // Isaac alone, with the first code to Mia, with the second to Sheba. B on
    // the first name goes back to the menu.
    SaveBlock_InitializeDefaults();
    WindowColor_Apply(gSaveBlock.windowColor[0], gSaveBlock.windowColor[1]);
    v5 = 1;
    v7 = 0;
    for (;;) {
      WaitFrames(6);
      v6 = NamingScreen_EditMemberName(v7);
      if (v6 == -1) {
        if (v7 == 0) {
          goto menu_top;
        }
        v7--;
        continue;
      }
      if (gMainMenu_CodeUnlocked != 0) {
        v5 = 4;
      }
      if (gMainMenu_ExtraCodeUnlocked != 0) {
        v5 = 7;
      }
      if (++v7 >= v5) {
        break;
      }
    }
    Party_StartChildhood();
    gSaveBlock.mapId = (s32)&MapId_ValeInn;
    gSaveBlock.warpDest = 20;
    goto leave;
  }
  // Continue a file where it was saved, except: a file from another build of
  // the game, or one loaded with L and Start held, restarts at its last
  // Sanctum (savedMap); a file whose two play times differ is reported
  // corrupted, with the offer to restart there. The two chosen restarts set
  // flags that nothing reads.
  if (v6 == 1) {
    struct SaveBlock *state;

    v6 = Save_LoadRecordIntoWork(1);
    if (v6 == -1) {
      goto menu_top;
    }
    Script_SetFlag(FLAG_KEEP_PARTY_POSITION);
    state = &gSaveBlock;
    WindowColor_Apply(state->windowColor[0], state->windowColor[1]);
    if (state->buildStamp != BuildStamp_GetTime()) {
      state->mapId = state->savedMap;
      state->warpDest = state->savedWarpDest;
      GameFlag_Clear(FLAG_KEEP_PARTY_POSITION);
    } else if ((gKeysHeld & (KEY_L | KEY_START)) == (KEY_L | KEY_START)) {
      if (MainMenu_GetSaveRecoveryBlockMask() != 0) {
        FieldMessage_Show(MsgSaveRecoveryFailed,
                          FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
        goto menu_top;
      }
      state->mapId = state->savedMap;
      state->warpDest = state->savedWarpDest;
      GameFlag_Clear(FLAG_KEEP_PARTY_POSITION);
      GameFlag_Set(FLAG_SAVE_RESTARTED_AT_SANCTUM);
    } else if (state->playTime !=
               ((struct SaveMenuMapLocalState *)gMapLocalState)->playTime) {
      FieldMessage_Show(MsgSaveDataCorrupted,
                        FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
      FieldMessage_Show(MsgSaveDataCorrupted + 1, FIELD_MESSAGE_ZOOM |
                                                      FIELD_MESSAGE_KEEP_OPEN |
                                                      FIELD_MESSAGE_LOWER);
      if (YesNoMenu_Run(1, 0, 0, YES) != YES) {
        TextPrinter_CloseFinishedWindows();
        goto menu_top;
      }
      TextPrinter_CloseFinishedWindows();
      if (MainMenu_GetSaveRecoveryBlockMask() != 0) {
        // MsgSaveRecoveryFailed, spelled as MsgSaveDataCorrupted + 2: the ROM
        // adds 2 to the register holding the earlier message's number, and by
        // name 108 of 921 instructions differ in tbs-en.
        FieldMessage_Show(MsgSaveDataCorrupted + 2,
                          FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
        goto menu_top;
      }
      state->mapId = state->savedMap;
      state->warpDest = state->savedWarpDest;
      GameFlag_Clear(FLAG_KEEP_PARTY_POSITION);
      GameFlag_Set(FLAG_SAVE_RECOVERED_AT_SANCTUM);
    }
    GameFlag_Clear(FLAG_MENU_OPEN);
    goto leave;
  }
  if (v6 == 2) {
    Save_CopyFile();
    goto menu_top;
  }
  if (v6 == 3) {
    Save_EraseSelectedFile();
    goto menu_top;
  }
  // Battle: go to the Battle Arena with a file. One saved while the Colosso
  // held the party's equipment gets it back, and its party is reset to Isaac,
  // Garet, Ivan and Mia, in order.
  if (v6 == 4) {
    v6 = Save_LoadRecordIntoWork(4);
    if (v6 == -1) {
      goto menu_top;
    }
    gSaveBlock.leaderLocalId = 0;
    if (Script_GetFlag(FLAG_COLOSSO_HOLDS_EQUIPMENT) != 0) {
      InventorySnapshot_Restore();
      Party_RemoveMember(0);
      Party_RemoveMember(1);
      Party_RemoveMember(2);
      Party_RemoveMember(3);
      Party_AddMember(0);
      Party_AddMember(1);
      Party_AddMember(2);
      Party_AddMember(3);
    }
    WindowColor_Apply(gSaveBlock.windowColor[0], gSaveBlock.windowColor[1]);
    GameFlag_Clear(FLAG_KEEP_PARTY_POSITION);
    GameFlag_Clear(FLAG_MENU_OPEN);
    GameFlag_Set(FLAG_IN_BATTLE_ARENA);
    gSleepDisabled = 1;
    FieldScript_SetDestination((s32)&MapId_BattleArena,
                               BATTLE_ARENA_FROM_MAIN_MENU);
    goto leave;
  } else {
    if (v6 != 5) {
      goto menu_top;
    }
    // Send: choose a cleared file, then Password (0) or Cable (1).
  link_load:;
    v6 = Save_LoadRecordIntoWork(5);
    if (v6 == -1) {
      goto menu_top;
    }
    WindowColor_Apply(gSaveBlock.windowColor[0], gSaveBlock.windowColor[1]);
  link_select:;
    v6 = IconMenu_ChooseSendOption(RESOURCE_LAYOUT_TRANSFER_METHOD);
    if (v6 == -1) {
      goto link_load;
    }
    // Link transfer waits for the other game's edition-specific greeting in
    // the partner's payload (struct SaveLinkGreeting); B gives up.
    if (v6 == TRANSFER_METHOD_CABLE) {
      v6 = (s32)Window_Create(MAIN_MENU_LINK_WINDOW_X, 5,
                              MAIN_MENU_LINK_WINDOW_WIDTH, 8,
                              WINDOW_FLAG_INSTANT);
      linkMessage = MsgClearSaveLabel;

      Text_DrawMessage(linkMessage, (struct Window *)v6, 0, 4);
      Text_DrawMessage((linkMessage + 1), (struct Window *)v6, 0, 16);
      Text_DrawMessage((linkMessage + 3), (struct Window *)v6, 0, 36);
      Serial_Initialize();
      WaitFrames(10);
      {
        u16 *sync = (u16 *)gSerialTransfer.payload;

        sync[0] = '0';
        sync[1] = '0';
        sync[2] = '0';
        sync[3] = '0';
      }
      v7 = 0;
      v5 = 3;
      for (peerIndex = 0; peerIndex != 4; peerIndex++) {
        u16 *greeting =
            ((struct SaveLinkGreeting *)gLinkPeerSignatures)[peerIndex].code;

        greeting[0] = '0';
        greeting[1] = '0';
        greeting[2] = '0';
        greeting[3] = '0';
      }
      for (;;) {
        if ((gKeysPressed & KEY_B) != 0) {
          Sound_PlaySong(SE_MENU_CANCEL);
          break;
        }
        if ((v5 & gLinkStatus) == v5) {
          u32 control = REG_SIOCNT;
          u32 peer = ((control << 26) >> 30) ^ 1;
          u16 *received =
              ((struct SaveLinkGreeting *)gLinkPeerSignatures)[peer].code;

          if (received[0] == MAIN_MENU_LINK_LANGUAGE_TAG) {
            if (received[1] == 'V') {
              if (received[2] == 'T') {
                if (received[3] == 'S') {
                  v7 = 1;
                  break;
                }
              }
            }
          }
        }
        WaitFrames(1);
      }
      Window_Close((struct Window *)v6, 2);
      if (v7 == 0) {
        goto link_select;
      }
      v6 = (s32)Window_Create(MAIN_MENU_LINK_ERROR_WINDOW_X, 10,
                              MAIN_MENU_LINK_ERROR_WINDOW_WIDTH, 4,
                              WINDOW_FLAG_INSTANT);

      Text_DrawMessage(MsgClearSendingData, (struct Window *)v6, 0, 4);
      WaitFrames(10);
      Serial_StartSend(gSaveBuffer, 0x1004);
      WaitFrames(10);
      // Wait for the send to finish, at most 600000 frames (about 2.8
      // hours); ten frames in a row without both consoles is an error.
      v5 = 0;
      requiredLinkMask = 3;
      linkConnectionOk = 1;
      for (v7 = 0; v7 <= 0x927bf; v7++) {
        v5 = (v5 + 1);
        if ((requiredLinkMask & gLinkStatus) == requiredLinkMask) {
          v5 = 0;
        }
        if (v5 == 10) {
          linkConnectionOk = 0;
          break;
        }
        if (gSerialSendSource == 0) {
          break;
        }
        WaitFrames(1);
      }
      if (linkConnectionOk == 0) {
        WindowSprite_PrepareForRedraw((struct Window *)v6);
        Text_DrawMessage(MsgClearCommunicationError, (struct Window *)v6, 0, 4);
        // Wait for A: v5 is KEY_A here.
        v5 = 1;
        do {
          WaitFrames(1);
        } while ((gKeysPressed & v5) == 0);
      }
      WaitFrames(10);
      Serial_RemoveIrqHandlers();
      WaitFrames(10);
#if TBS_PATCH_4
      Window_Close((struct Window *)v6, 2);
      WaitFrames(1);
#else
      WindowSprite_PrepareForRedraw((struct Window *)v6);
      Window_Close((struct Window *)v6, 2);
#endif
      goto menu_top;
    }
    if (v6 == TRANSFER_METHOD_PASSWORD) {
    // Password output packs save data, appends CRC bytes, then groups glyphs.
    // The level is Gold, Silver or Bronze (enum PasswordLevel): 260, 61 or 16
    // symbols, shown 50 to a page.
    page_select:;
      v6 = IconMenu_ChooseSendOption(RESOURCE_LAYOUT_PASSWORD_LEVEL);

      if (v6 == -1) {
        goto link_select;
      }
      {
        u8 *data = saveData;
        s16 sum;

        encodedSize = MainMenu_PackPasswordData(0, v6, data);
        sum = MainMenu_CalculatePasswordCrc16(encodedSize, data);
        saveData[encodedSize] = (u16)sum >> 8;
        saveData[encodedSize + 1] = sum;
        encodedSize += 2;
        encodedSize = MainMenu_EncodePassword(data, encodedSize, password);
      }
      v6 = 2;
      Window_LoadGraphics();
      pageWindow = Window_Create(5, 4, 20, 12, v6);
      pageIndex = 0;
      pageCount = ((encodedSize / PASSWORD_SYMBOLS_PER_PAGE) + 1);
      windowHandle = (s32)Window_Create(MAIN_MENU_HELP_WINDOW_X, 0,
                                        MAIN_MENU_HELP_WINDOW_WIDTH, 4, v6);
      passwordHeadingMessage = MsgClearPasswordLabel;
      headingWindow = (struct Window *)windowHandle;
      Text_DrawMessage(passwordHeadingMessage, headingWindow, 6, 4);
      v7 = 1;
      // The help line below: B alone for one page, with Left and Right for
      // more (the two messages before MsgClearPasswordLabel).
      if (pageCount == 1) {
        windowHandle = (s32)Window_Create(5, 16, 20, 3, v6);
        helpWindow = (struct Window *)windowHandle;
        Text_DrawMessageAsTiles(passwordHeadingMessage - 2, helpWindow,
                                MAIN_MENU_PAGE_LABEL_X, 0);
      } else {
        windowHandle =
            (s32)Window_Create(MAIN_MENU_MESSAGE_WINDOW_X, 16,
                               MAIN_MENU_MESSAGE_WINDOW_WIDTH, 3, v6);
        helpWindow = (struct Window *)windowHandle;
        Text_DrawMessageAsTiles(passwordHeadingMessage - 1, helpWindow, 0, 0);
      }
      // Copy the font's ten digit tiles to tiles 0x300 to 0x313 in two
      // colourings, for the page numbers.
      Graphics_ExpandVramTilesByColorTable((u16 *)0x06006000);
      WindowSprite_PrepareForRedraw(pageWindow);
      // A turns the page, and on the last returns to the menu; Left and Right
      // turn back and forth, wrapping round; B goes back to the level choice.
      // v7 is 1 when the page must be drawn again.
      for (;;) {
        Link_DrawPageArrows(PAGE_ARROW_VRAM);
        if ((gKeysPressed & KEY_B) != 0) {
          Sound_PlaySong(SE_MENU_CANCEL);
          WindowSprite_PrepareForRedraw(pageWindow);
          Window_Close(pageWindow, 2);
          Window_Close(helpWindow, 2);
          Window_Close(headingWindow, 2);
          goto page_select;
        }
        if ((gKeysPressed & KEY_A) != 0) {
          pageIndex = (pageIndex + 1);
          v7 = 1;
          if (pageIndex == pageCount) {
            Sound_PlaySong(SE_MENU_CONFIRM);
            WindowSprite_PrepareForRedraw(pageWindow);
            Window_Close(pageWindow, 2);
            Window_Close(helpWindow, 2);
            Window_Close(headingWindow, 2);
            WaitFrames(1);
            goto menu_top;
          }
          Sound_PlaySong(SE_MENU_CURSOR_MOVE);
        } else {
          // Left and Right share the wrap: with a copy in each arm, the
          // copies are not folded back and 210 of 920 instructions differ in
          // tbs-en.
          if ((gKeysPressed & KEY_LEFT) != 0 && pageCount > 1) {
            Sound_PlaySong(SE_MENU_CURSOR_MOVE);
            wrappedPage = pageIndex + pageCount;
            wrappedPage--;
          } else if ((gKeysPressed & KEY_RIGHT) != 0 && pageCount > 1) {
            Sound_PlaySong(SE_MENU_CURSOR_MOVE);
            wrappedPage = pageIndex + pageCount;
            wrappedPage++;
          } else {
            goto keys_done;
          }
          wrappedPage = (wrappedPage % pageCount);
          v7 = 1;
          pageIndex = wrappedPage;
        }
      keys_done:;
        if (v7 != 1) {
          v5 = passwordHeadingMessage;
        } else {
          WindowSprite_PrepareForRedraw(pageWindow);
          v7 = 0;
          v5 = 2;
          do {
            Window_DrawDividerLine(pageWindow, 0, v5, 18, v5);
            v5 = (v5 + 2);
            v7 = (v7 + 1);
          } while (v7 != 4);
          // The page numbers beside the arrows, on WINDOW_PALETTE: tile 0x301 +
          // n is the digit n + 1, and 0x30b + n the same digit in the second
          // colouring, for the page shown.
          if (pageCount > 1) {
            v7 = 0;
            if (pageCount != 0) {
              v6 = 0;
              do {
                tileValue = v7 + 0xf301;
                if (v7 == pageIndex) {
                  tileValue = v7 + 0xf30b;
                }
                Window_SetTilemapEntry(pageWindow, tileValue,
                                       v7 + 18 - pageCount, -1, v6);
                v7 = (v7 + 1);
              } while (v7 != pageCount);
            }
            v5 = 0;
            Window_SetTilemapEntry(pageWindow, PAGE_ARROW_LEFT,
                                   (17 - (s32)pageCount), -1, 0);
            Window_SetTilemapEntry(pageWindow, PAGE_ARROW_RIGHT, 18, -1, 0);
            Window_MarkTilemapRow((struct WindowWork *)gWindowWork[0],
                                  ((struct Window *)pageWindow)->y);
          }
          // Each page holds 50 symbols in five rows, split into two groups.
          // Rows are 16 pixels apart; the second five of a row sit 10 pixels
          // further right.
          v6 =
              pageIndex * PASSWORD_SYMBOLS_PER_PAGE + PASSWORD_SYMBOLS_PER_PAGE;
          if (v6 > encodedSize) {
            v6 = encodedSize;
          }
          for (v7 = pageIndex * PASSWORD_SYMBOLS_PER_PAGE, pageGlyphIndex = 0;
               v7 != v6; v7++, pageGlyphIndex++) {
            /* FAKEMATCH(one-pass): the block makes each draw call rebuild the
             * glyph's address as the ROM does; without it, 98 of 921
             * instructions differ in tbs-en. */
            do {
              MainMenu_NamingScreenCharacter(
                  password[v7] & PASSWORD_SYMBOL_MASK, glyph);
            } while (0);
            passwordColumn = (v7 % 10);
            if (passwordColumn > 4) {
              s32 row;

              glyphColumn = (pageGlyphIndex % 10);
              row = (pageGlyphIndex / 10);
              Text_DrawString(glyph, pageWindow, glyphColumn * 12 + 18,
                              (row << 4) + 2);
            } else {
              s32 row;

              glyphColumn = (pageGlyphIndex % 10);
              /* FAKEMATCH(wrapper): the division written as a call to
               * __divsi3; as pageGlyphIndex / 10, 45 of 921 instructions
               * differ in tbs-en. */
              row = __divsi3(pageGlyphIndex, 10);
              Text_DrawString(glyph, pageWindow, glyphColumn * 12 + 8,
                              (row << 4) + 2);
            }
          }
          v7 = 0;
        }
        WaitFrames(1);
      }
    }
    WaitFrames(0x12c);
    goto menu_top;
  }
leave:;
  gFieldWork->warp = 0x3e7;
  FieldScript_WaitFrames(30);
  Sound_PlaySong(SONG_FADE_OUT_BGM);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(60);
  return 0;
}

/// UNUSED: decodes tiles and maps a 16-by-eight window region in VRAM and
/// shadow.
UNUSED static void FileMenu_FillTileGrid(struct Window *window,
                                         const void *compressedTiles) {
  u16 *shadow;
  u16 *vram;
  void *decodedTiles;
  s32 row;
  s32 col;
  s32 offset;

  shadow = ((struct WindowWork *)gWindowWork[0])->tilemap;
  decodedTiles = AllocEwram(0x300);
  FieldDecompress_TaggedLz(compressedTiles, decodedTiles);
  offset = (window->y * 32 + window->x) * 2;
  vram = (u16 *)(0x06002000 + offset);
  shadow = (u16 *)((u8 *)shadow + offset);
  for (row = 0; row <= 7; row++) {
    for (col = 0; col <= 15; col++) {
      s16 tile = (window->width * row + col) | 0xf000;

      *vram++ = tile;
      *shadow++ = tile;
    }
    vram += 16;
    shadow += 16;
  }
  Free(decodedTiles);
}

/// UNUSED: fills a 16-by-eight region with WINDOW_PALETTE tiles beginning at
/// tile 32.
UNUSED static void FileMenu_FillTileBlock(const struct Window *window) {
  s16 *mirror = ((struct WindowWork *)gWindowWork[0])->tilemap;
  s16 *buffer = AllocEwram(0x300);
  s16 *vram;
  s32 cell;
  s32 row;
  s32 base;

  cell = window->y * 32 + window->x;
  vram = (s16 *)0x06002000 + cell;
  mirror += cell;
  row = 0;
  base = 0;
  do {
    s32 tile = base + 32;
    s32 remaining = 15;

    do {
      s16 value = tile | -0x1000;

      remaining--;
      *vram = value;
      tile++;
      *mirror = value;
      vram++;
      mirror++;
    } while (remaining >= 0);
    row++;
    vram += 16;
    mirror += 16;
    base += 16;
  } while (row <= 7);
  Free(buffer);
}

/// A party member's base stats, which the password clamps and packs: struct
/// Unit's from baseHp to baseLuck, at the same offsets.
struct PasswordStats {
  s16 baseHp;      ///< clamped to 0 to UNIT_CHARACTER_HP_MAX; 11 bits in the
                   ///< password
  s16 basePp;      ///< clamped to 0 to UNIT_CHARACTER_HP_MAX; 11 bits
  s16 hpGauge;     ///< Unit's; not read here
  s16 ppGauge;     ///< Unit's; not read here
  u16 baseAttack;  ///< clamped to UNIT_STAT_MAX; 10 bits
  u16 baseDefense; ///< clamped to UNIT_STAT_MAX; 10 bits
  u16 baseAgility; ///< clamped to UNIT_STAT_MAX; 10 bits
  u8 baseLuck;     ///< clamped to UNIT_LUCK_MAX; 8 bits, the top one always 0
};

/// A party member as the password reads them: their level, base stats,
/// inventory and Djinn. A view of struct Unit at its offsets;
/// MainMenu_PackPasswordData's opening note says why it is kept.
struct PasswordMember {
  u8 name[0x0f]; ///< Unit's; not read here
  u8 level;      ///< clamped to 1 to 99; 7 bits
  struct PasswordStats stats;
  u8 regenToActions[0xb8]; ///< Unit's baseHpRegen to psynergy
  /// bits: 15..11 quantity | 10 broken | 9 equipped | 8..0 item
  u16 inventory[15];
  u8 padding[2];    ///< Unit's paddingBeforeDjinn
  u32 djinnHeld[4]; ///< the Djinn held, a bit per Djinni, by element
};

/// Packs the four heroes' data for a password of level mode (enum
/// PasswordLevel): PASSWORD_LEVEL_GOLD 173 bytes, _SILVER 39 or _BRONZE 9; any
/// other mode clears 11. Clamps
/// each member's base stats and level in place first, and returns the byte
/// count. Bytes 0 to 2 and the high half of 3 hold the four levels, 7 bits
/// each from the lowest bit; the low half of 3 and bytes 4 to 6 the Djinn
/// held, 7 bits an element; byte 7 the story flags of gMainMenu_PasswordFlags.
/// Silver and Bronze keep in byte 8 which Psynergy items the party carries.
/// Gold and Silver add the base stats, 60 bits a member, high bits first,
/// from byte 8 (Gold) or 9 (Silver). Gold alone adds the 60 inventory slots'
/// items, 9 bits each from byte 39, with a spare bit after every seventh;
/// from the low half of byte 107, the 5-bit quantity fields of the 23
/// gMainMenu_PasswordStackItems for each member; and the coins in bytes 165 to
/// 167.
static s32 MainMenu_PackPasswordData(s32 unused, s32 mode, u8 *out) {
  // A direct Unit member plus the real member table shrinks the native
  // function from 0x400 to 0x3f4. The private view remains pending member
  // reconciliation; its fields below follow Unit's actual base stats.
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
  for (i = 0; i != length; i++) {
    out[i] = 0;
  }

  levelBits = 0;
  djinnBits = 0;
  keyItemBits = 0;
  flagBits = 0;
  for (i = 0; i != 8; i++) {
    packedStats[i] = 0;
  }

  // Preserve the selected story flags and four heroes' transfer data.
  for (i = 0; i != 6; i++) {
    if (GameFlag_Get(((const u16 *)gMainMenu_PasswordFlags)[i])) {
      flagBits |= 1 << i;
    }
  }

  for (i = 0; i != 4; i++) {
    struct PasswordMember *member = (struct PasswordMember *)Unit_Get(
        ((const struct { s32 id[4]; } *)gMainMenu_PasswordMembers)->id[i]);
    struct PasswordStats *stats = &member->stats;

    if (stats->baseHp > UNIT_CHARACTER_HP_MAX) {
      stats->baseHp = UNIT_CHARACTER_HP_MAX;
    }
    if (stats->baseHp < 0) {
      stats->baseHp = 0;
    }
    if (stats->basePp > UNIT_CHARACTER_HP_MAX) {
      stats->basePp = UNIT_CHARACTER_HP_MAX;
    }
    if (stats->basePp < 0) {
      stats->basePp = 0;
    }
    if (stats->baseAttack > UNIT_STAT_MAX) {
      stats->baseAttack = UNIT_STAT_MAX;
    }
    if (stats->baseDefense > UNIT_STAT_MAX) {
      stats->baseDefense = UNIT_STAT_MAX;
    }
    if (stats->baseAgility > UNIT_STAT_MAX) {
      stats->baseAgility = UNIT_STAT_MAX;
    }
    if (stats->baseLuck > UNIT_LUCK_MAX) {
      stats->baseLuck = UNIT_LUCK_MAX;
    }
    // Shift as unsigned: the permitted HP and Defense maxima exceed the
    // signed 32-bit range after packing, while their bit patterns are valid.
    packedStats[i * 2] =
        (u32)stats->baseHp << 21 | stats->basePp << 10 | stats->baseAttack;
    packedStats[i * 2 + 1] = (u32)stats->baseDefense << 22 |
                             stats->baseAgility << 12 | stats->baseLuck << 4;
    if (member->level > UNIT_LEVEL_MAX) {
      member->level = UNIT_LEVEL_MAX;
    }
    if (member->level == 0) {
      member->level = 1;
    }
    levelBits |= member->level << (i * 7);
    for (j = 0; j != 4; j++) {
      djinnBits += member->djinnHeld[j] << (j * 7);
    }
    // Which of the eight Psynergy items (gMainMenu_PasswordKeyItems) any member
    // carries.
    for (j = 0; j != 15; j++) {
      u16 item = member->inventory[j] & 0x1ff;

      for (k = 0; k != 8; k++) {
        if (item == ((const u16 *)gMainMenu_PasswordKeyItems)[k]) {
          keyItemBits |= 1 << k;
        }
      }
    }
  }

  // The largest password also carries inventories, stack counts and coins.
  if (mode == PASSWORD_LEVEL_GOLD) {
    outputIndex = 39;
    bitOffset = 0;
    for (i = 0; i != 4; i++) {
      struct PasswordMember *member = (struct PasswordMember *)Unit_Get(
          ((const struct { s32 id[4]; } *)gMainMenu_PasswordMembers)->id[i]);

      for (n = 0; n != 15; n++) {
        u16 item;

        // Item_GetDirect only looks the definition up; its result is not used.
        Item_GetDirect(member->inventory[n]);
        item = member->inventory[n] & 0x1ff;
        out[outputIndex] += item >> (bitOffset + 1);
        out[outputIndex + 1] += item << (7 - bitOffset);
        bitOffset++;
        outputIndex++;
        if (bitOffset == 7) {
          bitOffset = 0;
          outputIndex++;
        }
      }
    }

    outputIndex = 107;
    bitOffset = -1;
    for (i = 0; i != 4; i++) {
      struct PasswordMember *member = (struct PasswordMember *)Unit_Get(
          ((const struct { s32 id[4]; } *)gMainMenu_PasswordMembers)->id[i]);
      s32 m;

      for (m = 0; m != 23; m++) {
        UNUSED u16 quantity;

        j = 0;
        for (n = 0; n != 15; n++) {
          if ((member->inventory[n] & 0x1ff) ==
              ((const u16 *)gMainMenu_PasswordStackItems)[m]) {
            j = (member->inventory[n] & 0xf800) >> 11;
          }
        }
        /* FAKEMATCH(dead): a halfword copy of the quantity nothing reads,
         * which leaves the shift hoisted above the sign test; without it, 71
         * of 485 instructions differ in tbs-en. */
        quantity = j;
        if (bitOffset < 0) {
          out[outputIndex] += (u16)j >> -bitOffset;
          outputIndex++;
          bitOffset += 8;
        }
        out[outputIndex] += (u16)j << bitOffset;
        bitOffset -= 5;
        if (bitOffset == -5) {
          outputIndex++;
          bitOffset = 3;
        }
      }
    }
    out[165] = (u32)gSaveBlock.coins >> 16;
    out[166] = (u32)gSaveBlock.coins >> 8;
    out[167] = gSaveBlock.coins;
  }

  // The two larger forms include the packed base-stat rows. Each pass packs
  // two members in 15 bytes: each one's first word and the top 28 bits of
  // its second, whose low four bits are always 0.
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
  out[3] = (levelBits >> 20 & 0xf0) | (djinnBits & 0x0f);
  out[4] = djinnBits >> 4;
  out[5] = djinnBits >> 12;
  out[6] = djinnBits >> 20;
  out[7] = flagBits;
  if (mode != PASSWORD_LEVEL_GOLD) {
    out[8] = keyItemBits;
  }
  return length;
}

/// Encodes size bytes of data as password symbols in out and returns how
/// many there are. XORs every byte but the last with the last (in place),
/// cuts the bits into six-bit symbols, high bits first, adds a check symbol
/// after every nine (their sum, mod 64; a shorter last group gets none), and
/// then adds each symbol's position, mod 64.
static s32 MainMenu_EncodePassword(u8 *data, s32 size, u8 *out) {
  s32 xorKey;
  s32 symbolCount;
  s32 i;
  s32 j;
  s32 inputBit;
  s32 inputIndex;
  s32 symbol;
  s32 groupSum;
  s32 groupSymbols;
  s32 sourceBit;

  // The final input byte obscures the preceding bytes.
  xorKey = data[size - 1];
  symbolCount = 0;
  for (i = 0; i != size - 1; i++)
    data[i] ^= xorKey;
  inputBit = 0;
  inputIndex = 0;
  groupSum = 0;
  groupSymbols = 0;
  // Extract six-bit symbols and append a checksum after every nine symbols.
  do {
    symbol = 0;
    for (j = 0; j != 6; j++) {
      sourceBit = (data[inputIndex] >> (7 - inputBit)) & 1;
      inputBit++;
      if (inputBit == 8) {
        inputBit = 0;
        inputIndex++;
      }
      symbol |= sourceBit << (5 - j);
      if (inputIndex == size)
        break;
    }
    groupSymbols++;
    out[symbolCount++] = symbol;
    groupSum += symbol;
    if (groupSymbols == 9) {
      out[symbolCount++] = groupSum & PASSWORD_SYMBOL_MASK;
      groupSum = 0;
      groupSymbols = 0;
    }
  } while (inputIndex != size);
  // Add each symbol's position, wrapping within the 64-character alphabet.
  for (i = 0; i != symbolCount; i++)
    out[i] = (out[i] + i) & PASSWORD_SYMBOL_MASK;
  return symbolCount;
}

/// Runs the original shift/subtract checksum, initially 0xffff, and returns
/// its complemented low 16 bits. 0xFFFFEFDF is -0x1021, CRC-16-CCITT's
/// polynomial, subtracted where that CRC XORs it; bits above 15 never reach
/// the result, since carries and borrows only move up.
static u16 MainMenu_CalculatePasswordCrc16(u32 byteCount, u8 *data) {
  u32 crc;
  u32 byteIndex;
  u32 bitIndex;

  byteIndex = 0;
  crc = 0xFFFF;
  if (byteCount != 0) {
    do {
      crc ^= *data << 8;
      bitIndex = 0;
      do {
        if ((crc & 0x8000) != 0) {
          crc = (crc << 1) + 0xFFFFEFDF;
        } else {
          crc <<= 1;
        }
        bitIndex++;
      } while (bitIndex != 8);
      byteIndex++;
      data++;
    } while (byteIndex != byteCount);
  }
  return ~crc;
}

struct MapWarpDest gMainMenu_WarpDests[] = {
    {0, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

u32 gMainMenu_Warps[] = {
    MAP_WARPS_END,
};

struct ObjectEventTemplate gMainMenu_ObjectEventTemplates[] = {
    {MAP_TABLE_END},
};

struct MapEvent gMainMenu_Events[] = {
    {MAP_EVENTS_END},
};

/// Frames since MainMenu_UpdateBlend began; the blend level is half of it.
s16 gMainMenu_BlendFrame = 0;

/// Set once gMainMenu_CodeSequence has been entered; nothing here clears it.
s16 gMainMenu_CodeUnlocked = 0;

/// Set once gMainMenu_ExtraCodeSequence has been entered.
s16 gMainMenu_ExtraCodeUnlocked = 0;

/// The index in gMainMenu_CodeSequence of the key to press next.
s16 gMainMenu_CodeProgress = 0;

/// The index in gMainMenu_ExtraCodeSequence of the key to press next.
s16 gMainMenu_ExtraCodeProgress = 0;

/// Set while the first code's last correct key is still held.
s16 gMainMenu_CodeHeld = 0;

/// The same for the second code, read through MAIN_MENU_EXTRA_CODE_HELD.
s32 gMainMenu_ExtraCodeHeld = 0;

// clang-format off
/// The members the password packs: Isaac, Garet, Ivan and Mia.
u32 gMainMenu_PasswordMembers[] = {
    0, 1, 2, 3,
};

/// The six story flags of the password's byte 7, as halfwords in pairs, bits
/// 0 to 5: FLAG_DODONPA_LOCKED_UP, FLAG_WON_COLOSSO,
/// FLAG_HSU_RESCUED_WITHOUT_PARTY, FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED,
/// FLAG_VISITED_SICK_DORA and FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES.
u32 gMainMenu_PasswordFlags[] = {
    0x9510941, 0x8d108b3, 0x868081e,
};

/// The eight Psynergy items, ITEM_ORB_OF_FORCE to ITEM_CATCH_BEADS (as
/// halfwords in pairs), which Silver and Bronze record in byte 8.
u32 gMainMenu_PasswordKeyItems[] = {
    0xc900c8, 0xcb00ca, 0xcd00cc, 0xcf00ce,
};

/// The 23 items whose quantity a Gold password keeps for each member: Herb,
/// Nut, Vial, Potion, Psy Crystal, Antidote, Elixir, Water of Life, Power
/// Bread, Cookie, Apple, Hard Nut, Mint, Lucky Pepper, Smoke Bomb, Sleep
/// Bomb, Game Ticket, Lucky Medal, Sacred Feather, Oil Drop, Weasel's Claw,
/// Bramble Seed and Crystal Powder.
u16 gMainMenu_PasswordStackItems[] = {
    180, 181, 182, 183, 186, 187, 188, 189,
    191, 192, 193, 194, 195, 196, 226, 227,
    228, 229, 236, 238, 239, 240, 241,
};
// clang-format on
