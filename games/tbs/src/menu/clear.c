#include "scene_ids.h"
#include "unit/menu/clear.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
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
#include "global_cells.h"
#include "edition.h"
#include "tbs_edition.h"
#include "text_font.h"
#include "types.h"
#include "scene.h"
#include "dma.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "engine_call.h"
#include "resource_ids.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "field_scene.h"
#include "serial_runtime.h"
#include "mem.h"
#include "window.h"
#include "item.h"
#include "game_state.h"
#include "uiwindow.h"
#include "text/msg_ids.h"
#include "songs.h"
#include "frame.h"
#include "map_scroll.h"
#include "flags.h"
#include "title.h"

// Edition-specific text and window layout values.
#if TBS_PATCH_1
#define CLEAR_PASSWORD_BUFFER_SIZE 320
#else
#define CLEAR_PASSWORD_BUFFER_SIZE 260
#endif

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define CLEAR_LINK_WINDOW_X 4
#define CLEAR_LINK_WINDOW_WIDTH 22
#else
#define CLEAR_LINK_WINDOW_X 6
#define CLEAR_LINK_WINDOW_WIDTH 18
#endif

#if (LANGUAGE == LANGUAGE_GERMAN)
#define CLEAR_LINK_LANGUAGE_TAG 'E'
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define CLEAR_LINK_LANGUAGE_TAG 'F'
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define CLEAR_LINK_LANGUAGE_TAG 'S'
#elif (LANGUAGE == LANGUAGE_ITALIAN)
#define CLEAR_LINK_LANGUAGE_TAG 'I'
#elif (LANGUAGE == LANGUAGE_JAPANESE)
#define CLEAR_LINK_LANGUAGE_TAG 'C'
#else
#define CLEAR_LINK_LANGUAGE_TAG 'U'
#endif

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define CLEAR_LINK_ERROR_WINDOW_X 4
#define CLEAR_LINK_ERROR_WINDOW_WIDTH 22
#else
#define CLEAR_LINK_ERROR_WINDOW_X 5
#define CLEAR_LINK_ERROR_WINDOW_WIDTH 20
#endif

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define CLEAR_HELP_WINDOW_X 8
#define CLEAR_HELP_WINDOW_WIDTH 13
#else
#define CLEAR_HELP_WINDOW_X 10
#define CLEAR_HELP_WINDOW_WIDTH 10
#endif

#if TBS_PATCH_1
#define CLEAR_PAGE_LABEL_X 80
#else
#define CLEAR_PAGE_LABEL_X 0
#endif

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define CLEAR_MESSAGE_WINDOW_X 0
#define CLEAR_MESSAGE_WINDOW_WIDTH 30
#elif (LANGUAGE == LANGUAGE_JAPANESE)
#define CLEAR_MESSAGE_WINDOW_X 3
#define CLEAR_MESSAGE_WINDOW_WIDTH 24
#else
#define CLEAR_MESSAGE_WINDOW_X 1
#define CLEAR_MESSAGE_WINDOW_WIDTH 28
#endif

#define PASSWORD_SYMBOLS_PER_PAGE 50
#define PASSWORD_SYMBOL_MASK 0x3f

// Forward declarations
static s32 Clear_PackPasswordData(s32 unused, s32 mode, u8 *out);
static s32 Clear_EncodePassword(u8 *data, s32 size, u8 *out);
static u16 Clear_ComputePasswordCrc16(u32 byteCount, u8 *data);

// Returns the clear menu's entrance table through the scene entry ABI.
u8 *SceneData_GetScriptTable(void) { return gClear_ScriptTable; }

// Returns zero from this menu entry.
s32 SceneData_ReturnZero(void) { return 0; }

// Returns the clear menu's packed exit table.
u8 *SceneData_GetMessageTable(void) { return gClear_MessageTable; }

// Returns the empty actor placement table.
u8 *SceneData_GetActorTable(void) { return gClear_ActorTable; }

// Returns the empty event table.
u8 *SceneData_GetEffectTable(void) { return gClear_EffectTable; }

// Loads the Golden Sun logo, maps its tiles and resets background scroll.
static void Clear_LoadBackground(void) {
  u8 *resource;
  s32 resourceId;
  u16 *tilemap;
  u32 x;
  u32 y;
  u16 tile;
  struct BgScroll *scroll;
  s32 blankTile;

  resourceId = (s32)&ResourceId_GoldenSunLogo;
  Engine_BlendSetDarkenTarget16(0);
  REG_BG2CNT = LOGO_BGCNT;
  gBgScroll[2].y = 0;
  blankTile = LOGO_BLANK_TILE;
  resource = Engine_ResourceGetTableEntry(resourceId);
  Dma_Set(resource, (void *)BG_PLTT, (DMA_ENABLE | DMA_32BIT) << 16 | 0x70,
          REG_DMA3);
  resource += 0x1c0;
  FieldResource_DecodeType01(resource, Ram_MapCellBuffer);
  Dma_Set((void *)Ram_MapCellBuffer, LOGO_TILES,
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
  ((struct MapScrollWork *)gMapWork[0])->flags =
      DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  {
    struct FieldActor *leader;

    leader = Object_GetById(gGameState.selectedActor);
    leader->motionFlags = 0;
  }
}

// Advances the logo blend every two frames and removes itself at completion.
static void Clear_UpdateBlend(void) {
  s32 frame;
  s32 level;

  frame = *(u16 *)&gClear_BlendFrame + 1;
  *(u16 *)&gClear_BlendFrame = frame;
  level = (u16)frame >> 1;
  // Blend BG0 and the sprites over the other layers, level sixteenths of
  // the way in.
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT1_BG0 | BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND |
                          BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 |
                          BLDCNT_TGT2_BD,
                      0x20000);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                      BLEND_ALPHA((u16)level, 16 - (u16)level), 0x20000);
  if ((u16)level > 15)
    Engine_TaskRemoveCallback(Clear_UpdateBlend);
}

// Hidden command: sequence of pressed keys, terminated by 0.
// clang-format off
const u16 gClear_CodeSequence[] = {
    KEY_SELECT, KEY_SELECT, KEY_SELECT, 0,
};

const u16 gClear_ExtraCodeSequence[] = {
    KEY_UP, KEY_DOWN, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_LEFT, KEY_RIGHT,
    KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_LEFT, KEY_UP, KEY_SELECT, 0,
};
// clang-format on

// The actual arrays have four and fifteen entries. The original one-element
// view makes the second key an out-of-bounds access. Direct array reads also
// reorder both native held-key stores (same 0xf8-byte function extent).
#ifdef UBFIX
#define CLEAR_SEQUENCE_KEYS(sequence) (sequence)
#else
#define CLEAR_SEQUENCE_KEYS(sequence)                                          \
  (((struct { u16 key[1]; } *)(sequence))->key)
#endif

// Tracks two hidden key sequences, requiring a full release after each key.
static void Clear_CheckButtonCodes(void) {
  if (gClear_CodeUnlocked == 0) {
    if (gClear_CodeHeld != 0) {
      if (gKeysHeld == 0)
        gClear_CodeHeld = 0;
    } else if (gKeysHeld != 0) {
      if (gKeysHeld ==
          CLEAR_SEQUENCE_KEYS(gClear_CodeSequence)[gClear_CodeProgress]) {
        gClear_CodeProgress++;
        gClear_CodeHeld = 1;
        if (CLEAR_SEQUENCE_KEYS(gClear_CodeSequence)[gClear_CodeProgress] ==
            0) {
          gClear_CodeUnlocked = 1;
          Engine_AudioPlayCue(SE_110);
        }
      } else {
        gClear_CodeProgress = 0;
      }
    }
  }
  if (gClear_ExtraCodeUnlocked == 0) {
    if (*(s16 *)&gClear_ExtraCodeHeld != 0) {
      if (gKeysHeld == 0)
        *(s16 *)&gClear_ExtraCodeHeld = 0;
    } else if (gKeysHeld != 0) {
      if (gKeysHeld ==
          CLEAR_SEQUENCE_KEYS(
              gClear_ExtraCodeSequence)[gClear_ExtraCodeProgress]) {
        gClear_ExtraCodeProgress++;
        *(s16 *)&gClear_ExtraCodeHeld = 1;
        if (CLEAR_SEQUENCE_KEYS(
                gClear_ExtraCodeSequence)[gClear_ExtraCodeProgress] == 0) {
          gClear_ExtraCodeUnlocked = 1;
          Engine_AudioPlayCue(SE_110);
        }
      } else {
        gClear_ExtraCodeProgress = 0;
      }
    }
  }
}

#undef CLEAR_SEQUENCE_KEYS

// Converts a six-bit password symbol to the edition's one- or two-byte glyph.
static void Clear_NameEntryCharacter(s32 cell, u8 *text) {
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

// Returns -1 when the flagged saved scene blocks recovery, otherwise zero.
static s32 Clear_GetSaveRecoveryBlockMask(void) {
  s32 sceneDifference;

  if (Engine_GameFlagIsSet(324) == 0) {
    return 0;
  }
  if (gGameState.sceneVariant == 2) {
    return 0;
  }
  sceneDifference = gGameState.scene ^ (s32)&SceneId_WorldMap;
  return -((u32)(-sceneDifference | sceneDifference) >> 31);
}

struct SaveMenuSceneState {
  u8 unk000[0x100];
  s32 word100;
};

struct SaveLinkGreeting {
  u16 code[4];
  u8 reserved[16];
};

s32 __divsi3(s32, s32);

// Runs new/continue/copy/delete, arena entry and save-data transfer/password
// menus. Returns zero after choosing a destination; cancellation returns to
// selection.
s32 SaveMenu_Run(void) {
  s32 pageCount;
  s32 encodedSize;
  struct UiWindow *pageWindow;
  // Keep encoded integer handles: the earlier typed-window split changes
  // four native bytes. Integer windowHandle/passwordColumn and peerIndex/
  // requiredLinkMask split plainly without changing the output.
  s32 wrappedPage;
  s32 glyphColumn;
  s32 windowHandle;
  s32 passwordColumn;
  s32 tileValue;
  // Integer-only phase splits also change the native output: separate new-game
  // name count/index/result, save-answer/write-result, or transfer variables
  // reorder instructions at the same extent; separate password redraw/index/
  // divider variables shorten the function by four bytes. These three scalars
  // retain those measured cross-phase relationships for now.
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
  struct UiWindow *helpWindow;
  struct UiWindow *headingWindow;
  u8 saveData[200];
  u8 password[CLEAR_PASSWORD_BUFFER_SIZE];
  u8 glyph[4];

  blendStarted = 0;
  Clear_LoadBackground();
  FieldScheduler_AddCallback(Clear_CheckButtonCodes, 0xc80);
  gEventWork->startTransition = blendStarted;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  // The end-of-game entry saves before returning to the world map.
  if (gGameState.entrance == 2) {
  L_ask:
    UiText_ShowPositionedMessageAndWait(MsgSaveAdventureQuestion, 5);
    v7 = Menu_AskYesNo(1);
    UiWork_FinalizePendingCore();
    if (v7 == 0) {
      UiText_ShowPositionedMessageAndWait(MsgSaveAdventureQuestion + 1, 1);
      gGameState.unk20f = 1;
      v7 = Save_WriteSelectedSlot();
      if (v7 == -1) {
        goto L_ask;
      }
    }
    Engine_EventWait(60);
    Engine_EventCloseScreen();
    Engine_AudioPlayCue(MUS_017);
    Engine_EventWait(0x12c);
    Event_SetDestination((s32)&SceneId_WorldMap, 72);
    return 0;
  }
  gSleepDisabled = blendStarted;
// Choose a new game, existing save, arena entry, or data transfer.
menu_top:;
  v6 = SaveState_ScanRecordFlags();
  if (v6 < 0) {
    if (gDebugMode != 0) {
      gGameState.idleSleepEnabled = 1;
      gIdleSleepEnabled = 1;
      UiText_ShowCenteredMessage(MsgNoBackupMemory, 1, 8);
    }
  }
  if (v6 == 0) {
    if (blendStarted == 0) {
      Engine_TaskWait(30);
      FieldScheduler_AddCallback(Clear_UpdateBlend, 0xc80);
      Engine_TaskWait(1);
      blendStarted = 1;
    }
  }
  if (v6 > 0) {
    v6 = Menu_SelectSaveSlotAction();
  } else {
    v6 = 0;
  }
  if (v6 == 0) {
    // Hidden button codes expand the list of names offered for a new game.
    GameState_InitDefaults();
    PaletteGlow_Update(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
    v5 = 1;
    v7 = 0;
    for (;;) {
      Engine_TaskWait(6);
      v6 = NameEntry_EditOwnerName(v7);
      if (v6 == -1) {
        if (v7 == 0) {
          goto menu_top;
        }
        v7--;
        continue;
      }
      if (gClear_CodeUnlocked != 0) {
        v5 = 4;
      }
      if (gClear_ExtraCodeUnlocked != 0) {
        v5 = 7;
      }
      if (++v7 >= v5) {
        break;
      }
    }
    Party_BeginChildhood();
    gGameState.scene = (s32)&SceneId_ValeBabi;
    gGameState.entrance = 20;
    goto leave;
  }
  // Continue from a save, checking relocation/recovery conditions first.
  if (v6 == 1) {
    struct GameState *state;

    v6 = SaveState_LoadRecordIntoWork(1);
    if (v6 == -1) {
      goto menu_top;
    }
    GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
    state = &gGameState;
    PaletteGlow_Update(state->paletteGlow[0], state->paletteGlow[1]);
    if (state->buildStamp != Runtime_GetBuildStampTime()) {
      state->scene = state->savedScene;
      state->entrance = state->savedEntrance;
      GameFlag_ClearBit(FLAG_KEEP_PARTY_POSITION);
    } else if ((gKeysHeld & (KEY_L | KEY_START)) == (KEY_L | KEY_START)) {
      if (Clear_GetSaveRecoveryBlockMask() != 0) {
        UiText_ShowPositionedMessageAndWait(MsgSaveRecoveryFailed, 9);
        goto menu_top;
      }
      state->scene = state->savedScene;
      state->entrance = state->savedEntrance;
      GameFlag_ClearBit(FLAG_KEEP_PARTY_POSITION);
      Engine_GameFlagSet(0x13e);
    } else if (state->playTime !=
               ((struct SaveMenuSceneState *)gSceneState)->word100) {
      UiText_ShowPositionedMessageAndWait(MsgSaveDataCorrupted, 9);
      UiText_ShowPositionedMessageAndWait(MsgSaveDataCorrupted + 1, 13);
      if (Menu_RunConfirmSelection(1, 0, 0, 0) != 0) {
        UiWork_FinalizePendingCore();
        goto menu_top;
      }
      UiWork_FinalizePendingCore();
      if (Clear_GetSaveRecoveryBlockMask() != 0) {
        // MsgSaveRecoveryFailed, spelled as MsgSaveDataCorrupted + 2: the ROM
        // adds 2 to the register holding the earlier message's number, and by
        // name 108 of 921 instructions differ in tbs-en.
        UiText_ShowPositionedMessageAndWait(MsgSaveDataCorrupted + 2, 9);
        goto menu_top;
      }
      state->scene = state->savedScene;
      state->entrance = state->savedEntrance;
      GameFlag_ClearBit(FLAG_KEEP_PARTY_POSITION);
      Engine_GameFlagSet(0x13f);
    }
    GameFlag_ClearBit(0x106);
    goto leave;
  }
  if (v6 == 2) {
    SaveState_CopySlotPair();
    goto menu_top;
  }
  if (v6 == 3) {
    SaveState_DeleteSelectedSlot();
    goto menu_top;
  }
  if (v6 == 4) {
    v6 = SaveState_LoadRecordIntoWork(4);
    if (v6 == -1) {
      goto menu_top;
    }
    gGameState.selectedActor = 0;
    if (GameFlag_IsSet(0x952) != 0) {
      InventorySnapshot_Restore();
      Party_RemoveActiveOwner(0);
      Party_RemoveActiveOwner(1);
      Party_RemoveActiveOwner(2);
      Party_RemoveActiveOwner(3);
      Party_AddActiveOwner(0);
      Party_AddActiveOwner(1);
      Party_AddActiveOwner(2);
      Party_AddActiveOwner(3);
    }
    PaletteGlow_Update(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
    GameFlag_ClearBit(FLAG_KEEP_PARTY_POSITION);
    GameFlag_ClearBit(0x106);
    Engine_GameFlagSet(0x17e);
    gSleepDisabled = 1;
    Event_SetDestination((s32)&SceneId_LinkLobby, 1);
    goto leave;
  } else {
    if (v6 != 5) {
      goto menu_top;
    }
  link_load:;
    v6 = SaveState_LoadRecordIntoWork(5);
    if (v6 == -1) {
      goto menu_top;
    }
    PaletteGlow_Update(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
  link_select:;
    v6 = Menu_SelectResourceLayout(0);
    if (v6 == -1) {
      goto link_load;
    }
    // Link transfer waits for the other game's edition-specific greeting.
    if (v6 == 1) {
      v6 = (s32)UiWindow_Create(CLEAR_LINK_WINDOW_X, 5, CLEAR_LINK_WINDOW_WIDTH,
                                8, 2);
      linkMessage = MsgClearSaveLabel;

      UiText_DrawResource(linkMessage, (struct UiWindow *)v6, 0, 4);
      UiText_DrawResource((linkMessage + 1), (struct UiWindow *)v6, 0, 16);
      UiText_DrawResource((linkMessage + 3), (struct UiWindow *)v6, 0, 36);
      SerialRuntime_Initialize();
      Engine_TaskWait(10);
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
          Engine_AudioPlayCue(SE_MENU_CANCEL);
          break;
        }
        if ((v5 & gLinkStatus) == v5) {
          u32 control = REG_SIOCNT;
          u32 peer = ((control << 26) >> 30) ^ 1;
          u16 *received =
              ((struct SaveLinkGreeting *)gLinkPeerSignatures)[peer].code;

          if (received[0] == CLEAR_LINK_LANGUAGE_TAG) {
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
        Engine_TaskWait(1);
      }
      UiWork_Finalize((struct UiWindow *)v6, 2);
      if (v7 == 0) {
        goto link_select;
      }
      v6 = (s32)UiWindow_Create(CLEAR_LINK_ERROR_WINDOW_X, 10,
                                CLEAR_LINK_ERROR_WINDOW_WIDTH, 4, 2);

      UiText_DrawResource(MsgClearSendingData, (struct UiWindow *)v6, 0, 4);
      Engine_TaskWait(10);
      SerialRuntime_BeginSend(gSaveBuffer, 0x1004);
      Engine_TaskWait(10);
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
        Engine_TaskWait(1);
      }
      if (linkConnectionOk == 0) {
        RenderOutput_PrepareForRedraw((struct UiWindow *)v6);
        UiText_DrawResource(MsgClearCommunicationError, (struct UiWindow *)v6,
                            0, 4);
        v5 = 1;
        do {
          Engine_TaskWait(1);
        } while ((gKeysPressed & v5) == 0);
      }
      Engine_TaskWait(10);
      SerialRuntime_RemoveIrqHandlers();
      Engine_TaskWait(10);
#if TBS_PATCH_4
      UiWork_Finalize((struct UiWindow *)v6, 2);
      Engine_TaskWait(1);
#else
      RenderOutput_PrepareForRedraw((struct UiWindow *)v6);
      UiWork_Finalize((struct UiWindow *)v6, 2);
#endif
      goto menu_top;
    }
    if (v6 == 0) {
    // Password output packs save data, appends CRC bytes, then groups glyphs.
    page_select:;
      v6 = Menu_SelectResourceLayout(1);

      if (v6 == -1) {
        goto link_select;
      }
      {
        u8 *data = saveData;
        s16 sum;

        encodedSize = Clear_PackPasswordData(0, v6, data);
        sum = Clear_ComputePasswordCrc16(encodedSize, data);
        saveData[encodedSize] = (u16)sum >> 8;
        saveData[encodedSize + 1] = sum;
        encodedSize += 2;
        encodedSize = Clear_EncodePassword(data, encodedSize, password);
      }
      v6 = 2;
      Ui_LoadWindowGraphics();
      pageWindow = UiWindow_Create(5, 4, 20, 12, v6);
      pageIndex = 0;
      pageCount = ((encodedSize / PASSWORD_SYMBOLS_PER_PAGE) + 1);
      windowHandle = (s32)UiWindow_Create(CLEAR_HELP_WINDOW_X, 0,
                                          CLEAR_HELP_WINDOW_WIDTH, 4, v6);
      passwordHeadingMessage = MsgClearPasswordLabel;
      headingWindow = (struct UiWindow *)windowHandle;
      UiText_DrawResource(passwordHeadingMessage, headingWindow, 6, 4);
      v7 = 1;
      if (pageCount == 1) {
        windowHandle = (s32)UiWindow_Create(5, 16, 20, 3, v6);
        helpWindow = (struct UiWindow *)windowHandle;
        UiText_DrawCharacterAtOffset(passwordHeadingMessage - 2, helpWindow,
                                     CLEAR_PAGE_LABEL_X, 0);
      } else {
        windowHandle = (s32)UiWindow_Create(CLEAR_MESSAGE_WINDOW_X, 16,
                                            CLEAR_MESSAGE_WINDOW_WIDTH, 3, v6);
        helpWindow = (struct UiWindow *)windowHandle;
        UiText_DrawCharacterAtOffset(passwordHeadingMessage - 1, helpWindow, 0,
                                     0);
      }
      Graphics_ExpandVramTilesByColorTable((u16 *)0x06006000);
      RenderOutput_PrepareForRedraw(pageWindow);
      for (;;) {
        Link_DrawShiftedTilePair(0x6002500);
        if ((gKeysPressed & KEY_B) != 0) {
          Engine_AudioPlayCue(SE_MENU_CANCEL);
          RenderOutput_PrepareForRedraw(pageWindow);
          UiWork_Finalize(pageWindow, 2);
          UiWork_Finalize(helpWindow, 2);
          UiWork_Finalize(headingWindow, 2);
          goto page_select;
        }
        if ((gKeysPressed & KEY_A) != 0) {
          pageIndex = (pageIndex + 1);
          v7 = 1;
          if (pageIndex == pageCount) {
            Engine_AudioPlayCue(SE_MENU_CONFIRM);
            RenderOutput_PrepareForRedraw(pageWindow);
            UiWork_Finalize(pageWindow, 2);
            UiWork_Finalize(helpWindow, 2);
            UiWork_Finalize(headingWindow, 2);
            Engine_TaskWait(1);
            goto menu_top;
          }
          Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
        } else {
          if ((gKeysPressed & KEY_LEFT) != 0 && pageCount > 1) {
            Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
            wrappedPage = pageIndex + pageCount;
            wrappedPage--;
          } else if ((gKeysPressed & KEY_RIGHT) != 0 && pageCount > 1) {
            Engine_AudioPlayCue(SE_MENU_CURSOR_MOVE);
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
          RenderOutput_PrepareForRedraw(pageWindow);
          v7 = 0;
          v5 = 2;
          do {
            UiWindow_DrawDividerLine(pageWindow, 0, v5, 18, v5);
            v5 = (v5 + 2);
            v7 = (v7 + 1);
          } while (v7 != 4);
          if (pageCount > 1) {
            v7 = 0;
            if (pageCount != 0) {
              v6 = 0;
              do {
                tileValue = v7 + 0xf301;
                if (v7 == pageIndex) {
                  tileValue = v7 + 0xf30b;
                }
                UiWindow_SetTilemapEntry(pageWindow, tileValue,
                                         v7 + 18 - pageCount, -1, v6);
                v7 = (v7 + 1);
              } while (v7 != pageCount);
            }
            v5 = 0;
            UiWindow_SetTilemapEntry(pageWindow, 0xf128, (17 - (s32)pageCount),
                                     -1, 0);
            UiWindow_SetTilemapEntry(pageWindow, 0xf129, 18, -1, 0);
            UiRender_MarkRow((struct UiRenderWork *)gWindowWork[0],
                             ((struct UiWindow *)pageWindow)->y);
          }
          // Each page holds 50 symbols in five rows, split into two groups.
          v6 =
              pageIndex * PASSWORD_SYMBOLS_PER_PAGE + PASSWORD_SYMBOLS_PER_PAGE;
          if (v6 > encodedSize) {
            v6 = encodedSize;
          }
          for (v7 = pageIndex * PASSWORD_SYMBOLS_PER_PAGE, pageGlyphIndex = 0;
               v7 != v6; v7++, pageGlyphIndex++) {
            /* FAKEMATCH(one-pass): the glyph's decoding in a one-pass block,
             * which ends the CSE path so that each draw call rebuilds the
             * glyph's address as the ROM does; without the block, 98 of 921
             * instructions differ in tbs-en. */
            do {
              Clear_NameEntryCharacter(password[v7] & PASSWORD_SYMBOL_MASK,
                                       glyph);
            } while (0);
            passwordColumn = (v7 % 10);
            if (passwordColumn > 4) {
              s32 row;

              glyphColumn = (pageGlyphIndex % 10);
              row = (pageGlyphIndex / 10);
              UiText_DrawString(glyph, pageWindow, glyphColumn * 12 + 18,
                                (row << 4) + 2);
            } else {
              s32 row;

              glyphColumn = (pageGlyphIndex % 10);
              /* FAKEMATCH(wrapper): the division written as a call to
               * __divsi3; as pageGlyphIndex / 10, 45 of 921 instructions
               * differ in tbs-en. */
              row = __divsi3(pageGlyphIndex, 10);
              UiText_DrawString(glyph, pageWindow, glyphColumn * 12 + 8,
                                (row << 4) + 2);
            }
          }
          v7 = 0;
        }
        Engine_TaskWait(1);
      }
    }
    Engine_TaskWait(0x12c);
    goto menu_top;
  }
leave:;
  gEventWork->requestedExit = 0x3e7;
  Engine_EventWait(30);
  Engine_AudioPlayCue(MUS_017);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(60);
  return 0;
}

// UNUSED: decodes tiles and maps a 16-by-eight window region in VRAM and
// shadow.
UNUSED static void SaveMenu_FillTileGrid(struct UiWindow *window,
                                         const void *compressedTiles) {
  u16 *shadow;
  u16 *vram;
  void *decodedTiles;
  s32 row;
  s32 col;
  s32 offset;

  shadow = ((struct UiRenderWork *)gWindowWork[0])->tilemap;
  decodedTiles = Runtime_BumpAllocateAlternatePool(0x300);
  FieldResource_DecodeType01(compressedTiles, decodedTiles);
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
  Runtime_BumpFree(decodedTiles);
}

// UNUSED: fills a 16-by-eight region with palette 15 tiles beginning at
// tile 32.
UNUSED static void SaveMenu_FillTileBlock(const struct UiWindow *window) {
  s16 *mirror = ((struct UiRenderWork *)gWindowWork[0])->tilemap;
  s16 *buffer = Runtime_BumpAllocateAlternatePool(0x300);
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
  Runtime_BumpFree(buffer);
}

struct PasswordStats {
  s16 baseHp;
  s16 basePp;
  u8 unk14[4];
  u16 baseAttack;
  u16 baseDefense;
  u16 baseAgility;
  u8 baseLuck;
};

struct PasswordOwner {
  u8 unk00[0x0f];
  u8 level;
  struct PasswordStats stats;
  u8 unk20[0xb8];
  u16 inventory[15];
  u8 unkf6[2];
  u32 djinnAvailable[4];
};

// Packs selected flags, levels, Djinn and optional stats/inventories/coins.
// Clamps the source base stats and levels in place; returns the raw byte count.
// A direct BattleUnit owner plus the real member table shrinks the native
// function from 0x400 to 0x3f4. The private view remains pending owner
// reconciliation; its fields below follow BattleUnit's actual base stats.
static s32 Clear_PackPasswordData(s32 unused, s32 mode, u8 *out) {
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
  case 0:
    length = 173;
    break;
  case 1:
    length = 39;
    break;
  case 2:
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
    if (Engine_GameFlagIsSet(((const u16 *)gClear_PasswordFlags)[i])) {
      flagBits |= 1 << i;
    }
  }

  for (i = 0; i != 4; i++) {
    struct PasswordOwner *owner =
        UnitGet(((const struct { s32 id[4]; } *)gClear_PasswordMembers)->id[i]);
    struct PasswordStats *stats = &owner->stats;

    if (stats->baseHp > 1999) {
      stats->baseHp = 1999;
    }
    if (stats->baseHp < 0) {
      stats->baseHp = 0;
    }
    if (stats->basePp > 1999) {
      stats->basePp = 1999;
    }
    if (stats->basePp < 0) {
      stats->basePp = 0;
    }
    if (stats->baseAttack > 999) {
      stats->baseAttack = 999;
    }
    if (stats->baseDefense > 999) {
      stats->baseDefense = 999;
    }
    if (stats->baseAgility > 999) {
      stats->baseAgility = 999;
    }
    if (stats->baseLuck > 99) {
      stats->baseLuck = 99;
    }
    // Shift as unsigned: the permitted HP and Defense maxima exceed the
    // signed 32-bit range after packing, while their bit patterns are valid.
    packedStats[i * 2] =
        (u32)stats->baseHp << 21 | stats->basePp << 10 | stats->baseAttack;
    packedStats[i * 2 + 1] = (u32)stats->baseDefense << 22 |
                             stats->baseAgility << 12 | stats->baseLuck << 4;
    if (owner->level > 99) {
      owner->level = 99;
    }
    if (owner->level == 0) {
      owner->level = 1;
    }
    levelBits |= owner->level << (i * 7);
    for (j = 0; j != 4; j++) {
      djinnBits += owner->djinnAvailable[j] << (j * 7);
    }
    for (j = 0; j != 15; j++) {
      u16 item = owner->inventory[j] & 0x1ff;

      for (k = 0; k != 8; k++) {
        if (item == ((const u16 *)gClear_PasswordKeyItems)[k]) {
          keyItemBits |= 1 << k;
        }
      }
    }
  }

  // The largest password also carries inventories, stack counts and coins.
  if (mode == 0) {
    outputIndex = 39;
    bitOffset = 0;
    for (i = 0; i != 4; i++) {
      struct PasswordOwner *owner = UnitGet(
          ((const struct { s32 id[4]; } *)gClear_PasswordMembers)->id[i]);

      for (n = 0; n != 15; n++) {
        u16 item;

        Item_Get(owner->inventory[n]);
        item = owner->inventory[n] & 0x1ff;
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
      struct PasswordOwner *owner = UnitGet(
          ((const struct { s32 id[4]; } *)gClear_PasswordMembers)->id[i]);
      s32 m;

      for (m = 0; m != 23; m++) {
        u16 quantity;

        j = 0;
        for (n = 0; n != 15; n++) {
          if ((owner->inventory[n] & 0x1ff) ==
              ((const u16 *)gClear_PasswordStackItems)[m]) {
            j = (owner->inventory[n] & 0xf800) >> 11;
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
    out[165] = (u32)gGameState.coins >> 16;
    out[166] = (u32)gGameState.coins >> 8;
    out[167] = gGameState.coins;
  }

  // The two larger forms include the packed base-stat rows.
  if (mode != 2) {
    s32 statsOffset = 8 + (mode != 0);

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
  if (mode != 0) {
    out[8] = keyItemBits;
  }
  return length;
}

// Obscures the packed bytes and emits position-adjusted six-bit password
// symbols.
static s32 Clear_EncodePassword(u8 *data, s32 size, u8 *out) {
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

// Runs the original shift/subtract checksum, initially 0xffff, and returns
// its complemented low 16 bits.
static u16 Clear_ComputePasswordCrc16(u32 byteCount, u8 *data) {
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

struct SceneEntrance gClear_ScriptTable[] = {
    {0, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gClear_MessageTable[] = {
    SCENE_EXITS(511),
};

struct ScenePlacement gClear_ActorTable[] = {
    {SCENE_TABLE_END},
};

struct SceneEvent gClear_EffectTable[] = {
    {SCENE_EVENTS_END},
};

s16 gClear_BlendFrame = 0;

s16 gClear_CodeUnlocked = 0;

s16 gClear_ExtraCodeUnlocked = 0;

s16 gClear_CodeProgress = 0;

s16 gClear_ExtraCodeProgress = 0;

s16 gClear_CodeHeld = 0;

s32 gClear_ExtraCodeHeld = 0;

// clang-format off
u32 gClear_PasswordMembers[] = {
    0, 1, 2, 3,
};

u32 gClear_PasswordFlags[] = {
    0x9510941, 0x8d108b3, 0x868081e,
};

u32 gClear_PasswordKeyItems[] = {
    0xc900c8, 0xcb00ca, 0xcd00cc, 0xcf00ce,
};

u16 gClear_PasswordStackItems[] = {
    180, 181, 182, 183, 186, 187, 188, 189,
    191, 192, 193, 194, 195, 196, 226, 227,
    228, 229, 236, 238, 239, 240, 241,
};
// clang-format on
