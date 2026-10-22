/// @file
/// Save files and the save file menu.
// Part of battle_command.c.

#include "../battle_command_internal.h"

// The three save files. Each is two 4 KB records of backup memory: record
// `file` holds the first half of the 8 KB save buffer and record
// `file + SAVE_FILE_COUNT` the second.
#define SAVE_FILE_COUNT 3
#define SAVE_RECORD_BYTES 0x1000
#define SAVE_NO_FREE_FILE 999

// What the save functions return when they fail, after showing the message.
#define SAVE_ERROR_READ -2 ///< a record could not be read
/// Writing failed. "Failed to save." (or "Failed to erase.")
#define SAVE_ERROR_WRITE -3
#define SAVE_ERROR_ERASE -4   ///< erasing failed. "Failed to erase."
#define SAVE_ERROR_NO_FILE -5 ///< no empty file to copy into
/// No backup memory. "Unable to detect backup memory."
#define SAVE_ERROR_NO_MEMORY -9

/// Writes the play time `frames` (at 60 frames a second) into `out` as hours
/// and minutes, "hhh:mm" with the hours right-aligned in three characters, and
/// returns `out`. The time stops at 999:59.
void *Text_FormatPlayTime(s32 frames, u8 *out) {
  u8 digits[64];
  u32 totalMinutes;
  u32 hours;
  s32 minutes;
  u8 *digitsCursor;
  u8 *textCursor;

  totalMinutes = (u32)frames / (60 * 60);
  if (totalMinutes > 999 * 60 + 59)
    totalMinutes = 999 * 60 + 59;

  hours = totalMinutes / 60;
  minutes = totalMinutes % 60;

  digitsCursor = UiText_FormatNumber(digits, hours, 3);
  *out = *digitsCursor;
  digitsCursor++;
  textCursor = out + 1;
  *textCursor = *digitsCursor;
  textCursor++;
  *textCursor = digitsCursor[1];
  // Adding 100 gives the minutes a leading zero in their last two digits.
  minutes += 100;
  textCursor++;
  *textCursor = ':';

  digitsCursor = UiText_FormatNumber(digits, minutes, 2);
  textCursor++;
  *textCursor = digitsCursor[0];
  textCursor++;
  *textCursor = digitsCursor[1];
  textCursor[1] = 0;

  return out;
}

/// Returns the first save file whose summary shows it empty (level 0), or
/// SAVE_NO_FREE_FILE when all three are in use. The summaries must be loaded.
u32 SaveState_FindFreeSummarySlot(void) {
  u32 file;
  struct SaveSummary *summary;

  summary = gSaveWorkspace->summary;
  for (file = 0; file < SAVE_FILE_COUNT; file++, summary++) {
    if (summary->level == 0)
      return file;
  }
  return SAVE_NO_FREE_FILE;
}

/// Returns how many save files there are, leaving out those whose summary has
/// its send flag set (the files saved at the end of the game) when
/// `excludeSent` is nonzero, or SAVE_ERROR_NO_MEMORY when the backup memory
/// cannot be read. It opens and releases the save workspace itself.
s32 SaveState_CountRecordsExcludingFlagged(s32 excludeSent) {
  s32 file;
  s32 count;
  struct SaveSummary *summary;

  if (SaveState_InitializeWorkspace() != 0) {
    count = SAVE_ERROR_NO_MEMORY;
  } else {
    count = SaveState_LoadSummaryRecords();
    if (excludeSent != 0) {
      summary = gSaveWorkspace->summary;
      for (file = 0; file < SAVE_FILE_COUNT; file++) {
        if ((s8)summary[file].sendFlag != 0)
          count--;
      }
    }
  }
  SaveState_ReleaseWorkspace();
  return count;
}

/// The distance in bytes from one field of a save summary to another.
#define SUMMARY_DISTANCE(from, to)                                             \
  ((u32)&((struct SaveSummary *)0)->to - (u32)&((struct SaveSummary *)0)->from)

/// Loads the save file summaries for the title menu and returns how many files
/// there are: plus 100 when every file has its send flag set, or
/// SAVE_ERROR_NO_MEMORY when the backup memory cannot be read. Sets
/// gTitleExtraOptionEnabled, which offers Battle, when a file has any of game
/// flags 48-127 set, and gTitleSendOptionEnabled, which offers Send, when a
/// file has its send flag set and R and Left are held.
/// Menu_SelectSaveSlotAction reads all three.
s32 SaveState_ScanRecordFlags(void) {
  s32 error;
  s32 sentCount;
  s32 fileCount;

  error = SaveState_InitializeWorkspace();
  sentCount = 0;
  fileCount = SAVE_ERROR_NO_MEMORY;
  if (error == 0) {
    s32 i;
    s8 *flags;

    fileCount = SaveState_LoadSummaryRecords();
    // A signed byte cursor from the party's end keeps the native literal base
    // 0x1070 and flag offsets 1 and 2.
    flags = (s8 *)&gSaveWorkspace->summary[0].party[4];
    gTitleSendOptionEnabled = 0;
    (*(s16 *)&gTitleExtraOptionEnabled) = 0;
    for (i = 0; i < SAVE_FILE_COUNT; i++) {
      if (flags[i * sizeof(struct SaveSummary) +
                SUMMARY_DISTANCE(party[4], sendFlag)] != 0) {
        gTitleSendOptionEnabled = 1;
        sentCount++;
      }
      if (flags[i * sizeof(struct SaveSummary) +
                SUMMARY_DISTANCE(party[4], flagCount)] != 0) {
        (*(s16 *)&gTitleExtraOptionEnabled) = 1;
      }
    }

    if ((gKeysHeld & (KEY_R | KEY_LEFT)) != (KEY_R | KEY_LEFT)) {
      gTitleSendOptionEnabled = 0;
    }
  }
  SaveState_ReleaseWorkspace();
  if (fileCount != 0 && sentCount == fileCount) {
    return fileCount + 100;
  }
  return fileCount;
}

/// Prepares the save buffer for writing: stamps the game state with the time
/// and options, then fills the file summary shown on the file menus (the
/// leader's name, level and class, the play time, area, coins, Djinn of each
/// element, the party, the send flag and how many of game flags 48-127 are
/// set) and its checksum, the sum of the 968 words from gGameFlagBytes on.
/// Returns the checksum.
u32 SaveState_BuildSummaryHeader(void) {
  u16 members[14];
  struct SaveSummary *summary;
  struct BattleUnit *leader;
  u8 *source;
  u8 *destination;
  u32 *word;
  s32 i;
  u32 sum = 0;

  // The summary's 16-byte header comes before the save buffer.
  summary = (struct SaveSummary *)((u32)gSaveBuffer - 16);
  gGameState.buildStamp = Runtime_GetBuildStampTimeFar(0);
  gGameState.playTime = gPlayTimeFrames;
  // A second copy of the play time, 256 bytes from gSceneState: past its
  // 0x78 bytes, in gInventorySnapshot's last word. gSceneState opens the save
  // buffer's second half, so this lands at offset 0x110 of the file's second
  // record (after its 16-byte header), which
  // SaveState_LoadSummaryRecords reads back into SaveSummary.unk38: the file
  // menu lists a file whose two play times differ as corrupted.
  {
    u32 *copy = (u32 *)gSceneState;
    copy[64] = gPlayTimeFrames;
  }
  gGameState.idleSleepEnabled = gIdleSleepEnabled;

  leader = Unit_GetFar(gGameState.selectedActor);
  destination = summary->name;
  source = leader->name;
  for (i = 11; i >= 0; i--)
    *destination++ = *source++;
  summary->level = leader->level;
  summary->playTime = gGameState.playTime;
  summary->area =
      Scene_GetAreaNameIndexFar(gGameState.scene, gGameState.entrance);
  summary->classId = leader->classIndex;
  summary->coins = gGameState.coins;
  summary->djinn[0] = Party_SumDjinnCountsFar(0);
  summary->djinn[1] = Party_SumDjinnCountsFar(1);
  summary->djinn[2] = Party_SumDjinnCountsFar(2);
  summary->djinn[3] = Party_SumDjinnCountsFar(3);
  // Up to four party members, ended by -1.
  Party_ListActiveOwnersFar(members);
  for (i = 0; i <= 3 && members[i] != 255; i++)
    summary->party[i] = members[i];
  summary->party[i] = -1;
  summary->paletteGlow = gGameState.paletteGlow[0];
  summary->paletteGlowSub = gGameState.paletteGlow[1];
  summary->sendFlag = gGameState.unk20f;
  summary->flagCount = 0;
  for (i = 48; i <= 127; i++) {
    if (GameFlag_TestFar(i))
      summary->flagCount++;
  }
  summary->hasFlag20 = GameFlag_TestFar(FLAG_CHILDHOOD) != 0;
  {
    s32 n;
    u32 *frame = &gGameState.buildStamp;
    u32 frames;

    // The checksum covers 242 * 4 = 968 words (3872 bytes) from the flags.
    n = 242;
    /* FAKEMATCH(one-pass): a never-taken null arm's one-pass block ends CSE
     * with the bound constant ahead of the frame load; with no arm, 2 of 174
     * instructions differ in tbs-en. */
    if (!frame) {
      do {
        n = 242;
      } while (0);
    } else {
      n = 242;
      frames = *frame;
    }
    frames = *frame;
    word = (u32 *)gGameFlagBytes;
    i = 0;
    n <<= 2;
    summary->frames = frames;
    // Entered at its test: as a while loop it gains a second test (4 bytes).
    goto test;
    do {
      sum += *word++;
      i++;
    test:;
    } while (i < n);
  }
  summary->checksum = sum;
  return sum;
}

/// Saves the game to the current file (gSaveSlot): builds the summary and
/// writes both halves of the save buffer, as records gSaveSlot and
/// gSaveSlot + SAVE_FILE_COUNT. Returns 0, gSaveSlot itself (-1) when no
/// file is chosen, or a SAVE_ERROR_* after showing its message. Unlike
/// Save_WriteSelectedSlot it asks nothing and does not capture the object
/// table first (SaveState_CaptureObjectTableFar).
s16 SaveState_WriteCurrentSlotPair(void) {
  s16 file;
  s16 result;
  s32 error;

  result = 0;
  file = gSaveSlot;
  if (file != -1) {
    error = SaveState_InitializeWorkspace();
    if (error != 0) {
      UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
      result = SAVE_ERROR_NO_MEMORY;
    } else {
      SaveState_BuildSummaryHeader();
      {
        void *half = &gSaveBuffer;
        s32 current;

        error = SaveState_WriteRecord(gSaveSlot, half);
        current = gSaveSlot;
        half = (char *)half + SAVE_RECORD_BYTES;
        error |= SaveState_WriteRecord(current + SAVE_FILE_COUNT, half);
        if (error != 0) {
          UiText_ShowPositionedMessageAndWait(MsgSaveFailed, 1);
          result = SAVE_ERROR_WRITE;
        }
      }
    }
    SaveState_ReleaseWorkspace();
    file = result;
  }
  return file;
}

/// Writes both halves of the save buffer, as they are, to save file `file`.
/// Returns 0, or a SAVE_ERROR_* after showing its message.
s32 SaveState_WriteSlotPair(s32 file) {
  s32 error;
  s16 result = 0;

  error = SaveState_InitializeWorkspace();
  if (error != 0) {
    UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    void *half = &gSaveBuffer;

    error = SaveState_WriteRecord(file, half);
    half = (char *)half + SAVE_RECORD_BYTES;
    error |= SaveState_WriteRecord(file + SAVE_FILE_COUNT, half);
    if (error != 0) {
      UiText_ShowPositionedMessageAndWait(MsgSaveFailed, 1);
      result = SAVE_ERROR_WRITE;
    }
  }
  SaveState_ReleaseWorkspace();
  return result;
}

/// Writes the 64-byte save stamp (gSaveStamp, which lies in the first half of
/// the save buffer) into the current file's first record, leaving the rest of
/// the record as it was saved. Returns 0, -1 when no file is chosen, or a
/// SAVE_ERROR_* after showing its message. The link lobby saves its battle
/// results this way (LinkLobby_SaveBattleResults). The record's 4 KB buffer
/// is taken from the alternate pool and freed before it returns, except when
/// no file is chosen.
s32 SaveState_ProcessSelectedSlot(void) {
  void *record;
  s32 result;
  s32 error;
  u8 *stamp;

  record = Runtime_BumpAllocateAlternatePool(SAVE_RECORD_BYTES);
  result = 0;
  // BUG: returns without freeing the record buffer.
  if (gSaveSlot == -1)
    return -1;
  error = SaveState_InitializeWorkspace();
  if (error != 0) {
    UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    error = SaveState_ReadRecordPayload(gSaveSlot, record);
    // A failed read is reported, but the record is still written below from
    // whatever the buffer holds.
    if (error != 0) {
      UiText_ShowPositionedMessageAndWait(MsgSaveFailed, 1);
      result = SAVE_ERROR_READ;
    }
    // The stamp's place in the record is its place in the save buffer.
    stamp = (u8 *)record + (s32)gSaveStamp;
    stamp -= (s32)gSaveBuffer;
    Iwram_CopyWords(stamp, gSaveStamp, 16);
    error = SaveState_WriteRecord(gSaveSlot, record);
    if (error != 0) {
      UiText_ShowPositionedMessageAndWait(MsgSaveFailed, 1);
      result = SAVE_ERROR_WRITE;
    }
  }
  SaveState_ReleaseWorkspace();
  MemFree(record);
  return result;
}

/// Asks whether to save over the current file and, on Yes, writes the save
/// stamp into it (SaveState_ProcessSelectedSlot) and says so. "OK to save over
/// this file?" and "Your game was saved." Returns that result.
/// Nothing calls it.
UNUSED s32 SaveState_ConfirmAndProcessSelectedSlot(void) {
  s32 result;

  // UB: on No, result is returned without ever being set.
#ifdef UBFIX
  result = 0;
#endif

  UiText_OpenMessageWindow(MsgOverwriteConfirm, 8, 12, 2);
  while (UiWork_IsComplete() == 0) {
    WaitFrames(1);
  }
  if (Menu_RunConfirmSelection(1, 0, 0, 1)) {
    UiWork_FinalizePendingCore();
  } else {
    UiWork_FinalizePendingCore();
    AudioCommand_PlayFar(MUS_SAVED);
    result = SaveState_ProcessSelectedSlot();
    if (result >= 0) {
      UiText_ShowPositionedMessageAndWait(MsgGameSaved, 1);
    }
  }
  return result;
}

/// The file menu's Copy (menu/clear.c): lets the player choose a file, reads
/// both of its records into the save buffer, overwriting it, and writes them
/// to the first empty file.
/// Returns 0, -1 when cancelled, or a SAVE_ERROR_* after showing its message
/// (the game says "Failed to erase." both when no file is empty and when the
/// write fails).
s32 SaveState_CopySlotPair(void) {
  u32 error;
  s32 file;
  s32 result;

  result = 0;
  error = SaveState_InitializeWorkspace();
  if (error != 0) {
    UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    SaveState_LoadSummaryRecords();
    file = SaveMenu_SelectSlot(0, 2);
    if (file == -1) {
      result = file;
    } else {
      void *lower = &gSaveBuffer;
      void *upper;

      error = SaveState_ReadRecordPayload(file, lower);
      upper = (u8 *)lower + SAVE_RECORD_BYTES;
      error |= SaveState_ReadRecordPayload(file + SAVE_FILE_COUNT, upper);
      if (error != 0) {
        UiText_ShowPositionedMessageAndWait(MsgLoadFailed, 1);
        result = SAVE_ERROR_READ;
      } else {
        file = SaveState_FindFreeSummarySlot();
        if (file == SAVE_NO_FREE_FILE) {
          UiText_ShowPositionedMessageAndWait(MsgEraseFailed, 1);
          result = SAVE_ERROR_NO_FILE;
        } else {
          error = SaveState_WriteRecord(file, lower);
          error |= SaveState_WriteRecord(file + SAVE_FILE_COUNT, upper);
          if (error != 0) {
            UiText_ShowPositionedMessageAndWait(MsgEraseFailed, 1);
            result = SAVE_ERROR_WRITE;
          } else {
            UiText_ShowPositionedMessageAndWait(MsgFileCopied, 1);
          }
        }
      }
    }
  }

  SaveState_ReleaseWorkspace();
  return result;
}

/// The file menu's Erase (menu/clear.c): lets the player choose a file, asks
/// whether to erase it and on Yes deletes both of its records. "OK to erase
/// this?" Returns 0 (also when the player answers No), -1 when cancelled, or
/// a SAVE_ERROR_* after showing its message.
s32 SaveState_DeleteSelectedSlot(void) {
  s32 error;
  s32 file;
  s32 result = 0;

  error = SaveState_InitializeWorkspace();
  if (error != 0) {
    UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    SaveState_LoadSummaryRecords();
    file = SaveMenu_SelectSlot(0, 3);
    if (file == -1) {
      result = file;
    } else {
      UiText_OpenMessageWindow(MsgEraseConfirm, 8, 1, 2);
      while (UiWork_IsComplete() == 0) {
        WaitFrames(1);
      }
      if (Menu_RunConfirmSelection(1, 0, 3, 1) != 0) {
        UiWork_FinalizePendingCore();
      } else {
        UiWork_FinalizePendingCore();
        error = SaveState_DeleteRecord(file);
        error |= SaveState_DeleteRecord(file + SAVE_FILE_COUNT);
        if (error != 0) {
          UiText_ShowPositionedMessageAndWait(MsgEraseFailed, 1);
          result = SAVE_ERROR_ERASE;
        } else {
          UiText_ShowPositionedMessageAndWait(MsgFileErased, 1);
        }
      }
    }
  }
  SaveState_ReleaseWorkspace();
  return result;
}

#if TBS_PATCH_1

/// A scheduler callback that makes colours 8-11 of BG palette 14 glow: each
/// follows a sine wave of the frame counter, eight frames behind the one
/// before. With n the sine as a whole number from -4 to 4 (Trig_Sin's 16.16
/// result over 16384, rounded toward zero), the colour is red 22 + 2n, green
/// 16 + n and blue 20 + n (of 31): red 14 to 30, green 12 to 20 and blue 16
/// to 24. The step of 0x300 a frame, in 1/65536 turns, makes a cycle of about
/// 85 frames. PaletteGlow_Start runs it while the save file menu is open.
void PaletteGlow_UpdateSine(void) {
  s32 *tick;
  volatile u16 *palette;
  s32 colour;
  s32 sine;
  s32 i;

  tick = (s32 *)&gFrameTick;
  palette = &BG_PLTT_COLOR(14, 8);
  i = 0;
  do {
#ifdef UBFIX
    // Wrap the frame phase before Trig_Sin takes its low sixteen bits.
    sine = Trig_Sin((((u32)*tick + i * 8U) * 0x300U) & 0xffffU);
#else
    sine = Trig_Sin((*tick + i * 8) * 0x300);
#endif
    colour = sine / 16384;
    {
#ifdef UBFIX
      // A negative sine must be shifted as unsigned bits.
      u32 red = (u32)colour << 1;
#else
      u32 red = colour << 1;
#endif
      u32 green = colour;

      red += 22;
      green += 16;
      colour += 20;
      /* FAKEMATCH(one-pass): the blue shift in a block of its own stays after
       * the three adds, in source order; as a plain shift, the red add moves
       * below the shifts and 6 of 33 instructions differ in tbs-en. */
      do {
        colour <<= 10;
      } while (0);
      green <<= 5;
      colour |= green;
      colour |= red;
    }
    /* FAKEMATCH(one-pass): the store in a block of its own stays ahead of the
     * loop index's increment; as a plain store, 3 of 33 instructions differ
     * in tbs-en. */
    do {
      *palette++ = colour;
    } while (0);
  } while (++i <= 3);
}

#endif

#if TBS_PATCH_1

/// Starts the palette glow (PaletteGlow_UpdateSine) in the scheduler.
void PaletteGlow_Start(void) {
  Scheduler_AddOrUpdateCallback(PaletteGlow_UpdateSine, 0xC80);
}

void PaletteGlow_Stop(void) {
  Scheduler_RemoveCallback(PaletteGlow_UpdateSine);
}

/// Fills the `width` x `height` tile rectangle at (x, y) inside `window`,
/// clipped to the screen as UiWindow_SetTileAttributeBitRect clips, with the
/// tile entry 0xe006: tile 6 in BG palette 14, the palette
/// PaletteGlow_UpdateSine animates. The save file menu marks the file under
/// the cursor with it.
void UiWindow_FillTilemapRect(struct UiWindow *window, s32 x, s32 y, s32 width,
                              s32 height) {
  u16 *tilemap = (u16 *)gWindowWork[0];
  s32 sum;

  sum = x + window->x;
  x = sum + 1;
  sum = y + (window)->y;
  y = sum + 1;

  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29)
    width = 30 - x;

  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29)
    height = 20 - y;

  if (width > 0 && height > 0) {
    // Byte offsets: 32 entries of 2 bytes a row.
    s32 yOffset = y << 6;
    s32 xOffset = x << 1;
    s32 offset = yOffset + xOffset;
    do {
      s32 remaining = width;
      u16 *cell = (u16 *)(offset + (s32)tilemap);
      if (remaining != 0) {
        do {
          remaining--;
          *cell++ = 0xE006;
        } while (remaining != 0);
      }
      height--;
      offset += 64;
    } while (height != 0);
    ((struct UiRenderWork *)tilemap)->dirty = 1;
  }
}

#endif

/// The save file menu shows the chosen file's party as walking sprites in a
/// row, and four Djinn sprites, one for each element, over the file's Djinn
/// counts. Each sprite stands three tiles (24 pixels) from the one before.
#define SAVE_MENU_SPRITE_SPACING 3
/// Scheduler priority of the callbacks that place the sprites every frame.
#define SAVE_MENU_SPRITE_PRIORITY (200 << 4)

/// Shows the party of save file `summary` (up to four members) as sprites in
/// a row from tile (x, y) inside `window`, and starts placing them every
/// frame (SaveMenu_PlacePartySprites).
void SaveMenu_ShowPartySprites(struct UiWindow *window, s32 x, s32 y,
                               struct SaveSummary *summary) {
  struct ShopRuntime *menu = gMenuWork;
  s32 i;
  s32 priority;

  for (i = 0; i < 4 && summary->party[i] != -1; i++) {
    struct AnimationObject *sprite =
        ResourceObject_CreateFar(Party_RemapCharacterIdIfEnabledFar(
            summary->party[i], (s8)summary->hasFlag20));

    if (sprite != NULL) {
      AnimationObjects_SelectAnimationFar(sprite, 1);
      sprite->flags = 0;
      sprite->image.priority = 0;
    }

    menu->partyMemberIcons[i] = sprite;
    menu->partyMemberX[i] =
        (x + window->x + i * SAVE_MENU_SPRITE_SPACING) * 8 + 16;
    menu->partyMemberY[i] = (y + window->y) * 8 + 16;
    // 1.0 in 16.16.
    menu->partyMemberScale[i] = 0x10000;
  }

  priority = 200;
  priority <<= 4;
  Scheduler_AddOrUpdateCallback(SaveMenu_PlacePartySprites, priority);
}

/// Stops placing the party sprites of the save file menu and releases them.
void SaveMenu_HidePartySprites(void) {
  struct ShopRuntime *menu = gMenuWork;
  s32 i;

  Scheduler_RemoveCallback(SaveMenu_PlacePartySprites);
  for (i = 0; i < 4; i++) {
    if (menu->partyMemberIcons[i] != NULL) {
      ResourceObject_ReleaseFar(menu->partyMemberIcons[i]);
      menu->partyMemberIcons[i] = NULL;
    }
  }
}

/// A scheduler callback: draws each party sprite of the save file menu at its
/// place and scale for this frame.
void SaveMenu_PlacePartySprites(void) {
  struct ShopRuntime *menu = gMenuWork;
  struct SpriteScale scale;
  struct SpritePlacement position;
  s32 index = 0;
  struct AnimationObject *sprite;

  while (1) {
    sprite = menu->partyMemberIcons[index];
    if (sprite != 0) {
      // Offset height and y equally to keep the visible row fixed.
      scale.x = menu->partyMemberScale[index];
      scale.y = menu->partyMemberScale[index];
      position.x = menu->partyMemberX[index] << 16;
      position.height = 500 << 16;
      position.y = (menu->partyMemberY[index] << 16) + (500 << 16);
      position.groundHeight = 0;
      Object_ApplyProjectedPlacementFar(sprite, &position, &scale, 0x4000);
    }
    index += 1;
    if (index > 3) {
      break;
    }
  }
}

/// Shows the four Djinn sprites, one for each element, in a row from tile
/// (x, y) inside `window`, and starts placing them every frame
/// (SaveMenu_PlaceDjinnSprites). Does nothing when `window` is NULL.
void SaveMenu_ShowDjinnSprites(struct UiWindow *window, s32 x, s32 y) {
  struct FourObjectMotionState *menu = gMenuWork;
  s32 i;

  if (window != NULL) {
    for (i = 0; i < 4; i++) {
      struct AnimationObject *sprite =
          ResourceObject_CreateFar(gMenu_PartySpriteResourceIds[i]);

      if (sprite != NULL) {
        AnimationObjects_SelectAnimationFar(sprite, 2);
        sprite->flags = 0;
        sprite->image.priority = 0;
      }

      menu->objects[i] = sprite;
      menu->positionsX[i] =
          (window->x + x + i * SAVE_MENU_SPRITE_SPACING) * 8 + 16;
      menu->positionsY[i] = (window->y + y) * 8 + 16;
    }

    Scheduler_AddOrUpdateCallback(SaveMenu_PlaceDjinnSprites,
                                  SAVE_MENU_SPRITE_PRIORITY);
  }
}

/// Stops placing the Djinn sprites of the save file menu and releases them.
void SaveMenu_HideDjinnSprites(void) {
  struct FourObjectMotionState *menu = gMenuWork;
  s32 i;

  Scheduler_RemoveCallback(SaveMenu_PlaceDjinnSprites);
  for (i = 0; i < 4; i++) {
    if (menu->objects[i] != NULL) {
      ResourceObject_ReleaseFar(menu->objects[i]);
      menu->objects[i] = NULL;
    }
  }
}

/// A scheduler callback: draws each Djinn sprite of the save file menu at its
/// place, at scale 1.0, for this frame.
void SaveMenu_PlaceDjinnSprites(void) {
  struct FourObjectMotionState *menu = gMenuWork;
  struct SpriteScale scale;
  struct SpritePlacement position;
  s32 index;

  index = 0;
  do {
    struct AnimationObject *sprite = menu->objects[index];

    if (sprite != NULL) {
      s32 one = 0x10000;
      s32 height = 500 << 16;

      scale.x = one;
      scale.y = one;
      position.x = menu->positionsX[index] << 16;
      position.height = height;
      position.y = (menu->positionsY[index] << 16) + height;
      position.groundHeight = 0;
      Object_ApplyProjectedPlacementFar(sprite, &position, &scale, 0x4000);
    }
    index++;
  } while (index <= 3);
}

/// Draws the Djinn counts of save file `summary`, one for each element, two
/// digits each, 24 pixels apart on row 16 of `window`, under the Djinn
/// sprites. Does nothing when `window` is NULL.
void SaveMenu_DrawDjinnCounts(struct UiWindow *window,
                              struct SaveSummary *summary) {
  s32 i;

  if (window != NULL) {
    for (i = 0; i < 4; i++)
      UiText_DrawNumberAtOffset((s8)summary->djinn[i], 2, window,
                                i * SAVE_MENU_SPRITE_SPACING * 8, 16);
  }
}

// Where each edition draws the "Lv" label and the coin label, in pixels.
#if (LANGUAGE == LANGUAGE_SPANISH)
#define SUMMARY_LEVEL_X 64
#define SUMMARY_COINS_X 56
#define SUMMARY_DRAW_LEVEL UiText_DrawStringAtOffset
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define SUMMARY_LEVEL_X 59
#define SUMMARY_COINS_X 56
#define SUMMARY_DRAW_LEVEL UiText_DrawStringAtOffset
#else
#define SUMMARY_LEVEL_X 72
#define SUMMARY_COINS_X 48
#define SUMMARY_DRAW_LEVEL UiText_DrawStringInWindow
#endif

/// Draws the summary of save file `summary` into `window`: the leader's name,
/// level and class, the play time and the coins. Does nothing when `window`
/// is 0.
void SaveMenu_DrawFileSummary(struct UiWindow *window,
                              struct SaveSummary *summary) {
  u32 playTime[4]; // the play time text
  s32 y;

  if (window != 0) {
    RenderOutput_PrepareForRedraw(window);
    UiWindow_DrawDividerLine(window, 0, 4, 13, 4);
    UiText_DrawStringAtOffset(summary->name, window, 0, 0);
    y = 0;
    SUMMARY_DRAW_LEVEL(gStatusMenu_LevelLetterString, window, SUMMARY_LEVEL_X,
                       0);
    UiText_DrawNumberAtOffset(summary->level, 2, window, 80, y);
    UiText_DrawCharacterAtOffset(summary->classId + MsgClassName, window, 0,
                                 16);
#if LANGUAGE == LANGUAGE_JAPANESE
    // Coins on the third row, then "Play Time:" and the time.
    y = 32;
    UiText_DrawNumberInWindow(summary->coins, 6, window, 24, y);
    UiText_DrawCharacterAtOffset(MsgCoins, window, 72, 32);
    UiText_DrawCharacterAtOffset(MsgStatusLabel, window, 0, 48);
    UiText_DrawStringInWindow(
        Text_FormatPlayTime(summary->playTime, (u8 *)playTime), window, 48, 48);
#else
    // "Play Time:" with the time under it, then the coins.
    UiText_DrawCharacterAtOffset(MsgStatusLabel, window, 0, 32);
    UiText_DrawStringInWindow(
        Text_FormatPlayTime(summary->playTime, (u8 *)playTime), window, 48, 40);
    y = 48;
    UiText_DrawNumberInWindow(summary->coins, 6, window, 0, y);
#if (LANGUAGE == LANGUAGE_FRENCH)
    // French takes the singular "Pièce" for one coin or none.
    if (summary->coins > 1)
      UiText_DrawCharacterAtOffset(MsgCoins, window, SUMMARY_COINS_X, 48);
    else
      UiText_DrawCharacterAtOffset(MsgPiece, window, SUMMARY_COINS_X, 48);
#else
    UiText_DrawCharacterAtOffset(MsgCoins, window, SUMMARY_COINS_X, 48);
#endif
#endif
  }
}

/// What the save file menu is choosing a file for, which decides the files
/// the cursor may stop on. Each mode also indexes the heading the menu shows
/// (gRenderResource_PairSourceTable: Save, Continue, Copy, Erase, Battle,
/// Link).
enum SaveMenuMode {
  SAVE_MENU_SAVE,     ///< saving: any file, empty ones too
  SAVE_MENU_CONTINUE, ///< continuing: files not yet cleared
  SAVE_MENU_COPY,     ///< copying: any file with data
  SAVE_MENU_ERASE,    ///< erasing: any file with data
  /// The title's Battle: files with any of game flags 48-127 set.
  SAVE_MENU_EXTRA,
  SAVE_MENU_CLEARED ///< the title's Send: cleared files only
};

/// SaveMenu_SelectSlot's result when no file can be chosen in its mode.
#define SAVE_MENU_NO_FILE -2

// The save file menu: lists the three files with each one's leader and area
// (or why it cannot be chosen) and, for the file under the cursor, its
// summary, its party and its Djinn, with the file's own palette glow. The
// cursor starts on `file` and moves up and down over the files `mode` allows
// (see SaveMenuMode). Returns the file chosen with A, -1 when cancelled with
// B, or SAVE_MENU_NO_FILE when no file is allowed.
s32 SaveMenu_SelectSlot(s32 selectedFile, s32 menuMode) {
  struct UiWindow *fileList;
#if TBS_PATCH_1
  struct SaveWorkspace *save;
  struct UiWindow *summaryWindow;
  struct UiWindow *partyWindow;
  struct UiWindow *djinnWindow;
  struct SaveSummary *summary;
  struct RenderOutput *modePicture;
  s32 summaryDirty;
  struct UiRenderWork *render;
  u32 buildStamp;
  struct ShopRuntime *spriteWork;
  s32 result;
  s32 i;
  s32 y;
#else
  struct UiWindow *summaryWindow;
  struct SaveWorkspace *save;
  struct UiWindow *partyWindow;
  struct UiWindow *djinnWindow;
  struct SaveSummary *summary;
  struct RenderOutput *modePicture;
  s32 cursorStyle;
  s32 summaryDirty;
  struct UiRenderWork *render;
  u32 buildStamp;
  struct ShopRuntime *spriteWork;
  struct ShopCursor fileCursor;
  s32 result;
  s32 i;
  s32 x;
  s32 y;
  s32 vramSlot;
  struct RenderOutput *cursorSprite;
#endif

  spriteWork = Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, sizeof(*spriteWork));
  save = *(gWorkSlot + HEAP_SLOT_SAVE);
  summaryWindow = NULL;
  partyWindow = NULL;
  djinnWindow = NULL;
  summaryDirty = 1;
  render = *(gWorkSlot + HEAP_SLOT_WINDOW);
  buildStamp = Runtime_GetBuildStampTimeFar();
  // Start on the first file the mode allows.
  // BUG: when none is, the jump to done returns SAVE_MENU_NO_FILE without
  // releasing spriteWork's HEAP_SLOT_MENU block.
  if (selectedFile < 0)
    selectedFile = 0;
#if TBS_PATCH_1
  if (menuMode == SAVE_MENU_CONTINUE) {
    for (i = 0;
         i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
                                 (s8)save->summary[selectedFile].sendFlag != 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (menuMode == SAVE_MENU_EXTRA) {
    for (i = 0; i < SAVE_FILE_COUNT &&
                (save->summary[selectedFile].level == 0 ||
                 (s8)save->summary[selectedFile].flagCount == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (menuMode == SAVE_MENU_CLEARED) {
    for (i = 0;
         i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
                                 (s8)save->summary[selectedFile].sendFlag == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (menuMode != SAVE_MENU_SAVE) {
    for (i = 0; i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }

  // The file list, and the mode's sprite above it.
  Dma_Fill32(0, spriteWork, sizeof(*spriteWork));
  PaletteGlow_Start();
  fileList = UiWindow_Create(1, 2, 28, 7, 2);
  summary = save->summary;
  y = 0;
  for (i = 0; i < SAVE_FILE_COUNT; i++, y += 16, summary++) {
    if (summary->level == 0) {
      UiText_DrawResource(MsgSaveNoData, fileList, 10, y);
    } else if (summary->frames < buildStamp) {
      // Saved by an older build: "(Continue from a Sanctum)", since such a
      // file resumes at GameState.savedScene, not where it was saved
      // (menu/clear.c).
      UiText_DrawResource(MsgSaveContinue, fileList, 10, y);
    } else if (summary->playTime != *(u32 *)summary->unk38) {
      UiText_DrawResource(MsgSaveCorrupted, fileList, 10, y);
    } else if (menuMode == SAVE_MENU_CLEARED && (s8)summary->sendFlag == 0) {
      UiText_DrawResource(MsgSaveNotCleared, fileList, 10, y);
    } else {
      UiText_DrawString(summary->name, fileList, SAVE_SLOT_NAME_X, y);
      UiText_DrawResource(summary->area + MsgDebugEntryName, fileList, 62, y);
      render->dirty = 1;
    }
  }
  UiWindow_DrawDividerLine(fileList, 0, 2, 27, 2);
  UiWindow_DrawDividerLine(fileList, 0, 4, 27, 4);
#else
  if (menuMode != SAVE_MENU_SAVE) {
    for (i = 0; i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }
  if (menuMode == SAVE_MENU_CONTINUE) {
    for (i = 0;
         i < SAVE_FILE_COUNT && ((s8)save->summary[selectedFile].sendFlag != 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }
  if (menuMode == SAVE_MENU_EXTRA) {
    for (i = 0; i < SAVE_FILE_COUNT &&
                ((s8)save->summary[selectedFile].flagCount == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }
  if (menuMode == SAVE_MENU_CLEARED) {
    for (i = 0;
         i < SAVE_FILE_COUNT && ((s8)save->summary[selectedFile].sendFlag == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }

  // The file list, and the mode's sprite above it.
  Dma_Fill32(0, spriteWork, sizeof(*spriteWork));
  fileList = UiWindow_Create(1, 2, 28, 7, 2);

  for (i = 0, summary = save->summary; i < SAVE_FILE_COUNT; i++, summary++) {
    if (save->summary[i].level == 0) {
      UiText_DrawResource(MsgSaveNoData, fileList, 10, i * 16);
    } else if (save->summary[i].frames < buildStamp) {
      // Saved by an older build: see the other editions' list above.
      UiText_DrawResource(MsgSaveContinue, fileList, 10, i * 16);
    } else if (save->summary[i].playTime != *(u32 *)save->summary[i].unk38) {
      UiText_DrawResource(MsgSaveCorrupted, fileList, 10, i * 16);
    } else if (menuMode == SAVE_MENU_CLEARED &&
               (s8)save->summary[i].sendFlag == 0) {
      UiText_DrawResource(MsgSaveNotCleared, fileList, 10, i * 16);
    } else {
      y = i * 16;
      UiText_DrawString(summary->name, fileList, SAVE_SLOT_NAME_X, y);
      UiText_DrawResource(save->summary[i].area + MsgDebugEntryName, fileList,
                          72, y);
      render->dirty = 1;
    }
  }
  UiWindow_DrawDividerLine(fileList, 0, 2, 27, 2);
  UiWindow_DrawDividerLine(fileList, 0, 4, 27, 4);
  vramSlot = Resource_FindFreeEntry();
  if (vramSlot < VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(vramSlot, 128, gResource_FixedBlockBTiles);
    x = -12;
    y = selectedFile * 16 + 4;
    cursorSprite =
        RenderOutput_Create(vramSlot, OUTPUT_SIZE_16X16, fileList, x, y);
    cursorSprite->active = 1;
    fileCursor.anchor = cursorSprite;
  }
  cursorStyle = 1;
#endif
  modePicture = RenderResource_CreatePair(menuMode, fileList, 72, -24);

  while (1) {
    // Show the file under the cursor after it moves.
    if (summaryDirty != 0) {
      summaryDirty = 0;
      if (save->summary[selectedFile].level != 0) {
        PaletteGlow_Update(save->summary[selectedFile].paletteGlow,
                           save->summary[selectedFile].paletteGlowSub);
        if (summaryWindow == NULL)
          summaryWindow = UiWindow_Create(1, 10, 14, 9, 2);
        SaveMenu_DrawFileSummary(summaryWindow, &save->summary[selectedFile]);
        WaitFrames(1);
        if (partyWindow == NULL)
          partyWindow = UiWindow_Create(16, 10, 13, 3, 2);
        SaveMenu_HidePartySprites();
        SaveMenu_ShowPartySprites(partyWindow, 0, 0,
                                  &save->summary[selectedFile]);
        WaitFrames(1);
        if ((s8)save->summary[selectedFile].djinn[0] +
                (s8)save->summary[selectedFile].djinn[1] +
                (s8)save->summary[selectedFile].djinn[2] +
                (s8)save->summary[selectedFile].djinn[3] !=
            0) {
          if (djinnWindow == NULL)
            djinnWindow = UiWindow_Create(16, 14, 13, 5, 2);
          SaveMenu_DrawDjinnCounts(djinnWindow, &save->summary[selectedFile]);
          SaveMenu_HideDjinnSprites();
          SaveMenu_ShowDjinnSprites(djinnWindow, 0, 0);
        } else {
          SaveMenu_HideDjinnSprites();
          UiWork_Finalize(djinnWindow, 2);
          djinnWindow = NULL;
        }
      } else {
        PaletteGlow_Update(gGameState.paletteGlow[0],
                           gGameState.paletteGlow[1]);
        SaveMenu_HideDjinnSprites();
        SaveMenu_HidePartySprites();
        UiWork_Finalize(djinnWindow, 2);
        UiWork_Finalize(partyWindow, 2);
        UiWork_Finalize(summaryWindow, 2);
        djinnWindow = NULL;
        partyWindow = NULL;
        summaryWindow = NULL;
      }
#if TBS_PATCH_1
      RenderOutput_RedrawSavedRect(fileList);
      UiWindow_DrawDividerLine(fileList, 0, 2, 27, 2);
      UiWindow_DrawDividerLine(fileList, 0, 4, 27, 4);
      UiWindow_FillTilemapRect(fileList, 0, selectedFile * 2, 26, 1);
    }
#else
      x = fileList->x * 8 - 4;
      y = (fileList->y << 3) + selectedFile * 16 + 12;
      Shop_SetCursorFar(&fileCursor, x, y, cursorStyle);
      cursorStyle = 3;
    }
    ShopCursor_AdvanceFar(&fileCursor);
#endif
    RenderOutput_BobPair(modePicture);
    WaitFrames(1);
    // Up and down move to the previous or next file the mode allows; saving
    // allows every file, an empty one too.
    if (gKeysRepeat & KEY_UP) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      summaryDirty = 1;
      do {
        selectedFile = (selectedFile + SAVE_FILE_COUNT - 1) % SAVE_FILE_COUNT;
        if (menuMode == SAVE_MENU_SAVE)
          break;
      } while ((save->summary[selectedFile].level == 0 ||
                (menuMode == SAVE_MENU_CONTINUE &&
                 (s8)save->summary[selectedFile].sendFlag != 0) ||
                (menuMode == SAVE_MENU_EXTRA &&
                 (s8)save->summary[selectedFile].flagCount == 0) ||
                (menuMode == SAVE_MENU_CLEARED &&
                 (s8)save->summary[selectedFile].sendFlag == 0)));
    } else if (gKeysRepeat & KEY_DOWN) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      summaryDirty = 1;
      do {
        selectedFile = (selectedFile + SAVE_FILE_COUNT + 1) % SAVE_FILE_COUNT;
        if (menuMode == SAVE_MENU_SAVE)
          break;
      } while ((save->summary[selectedFile].level == 0 ||
                (menuMode == SAVE_MENU_CONTINUE &&
                 (s8)save->summary[selectedFile].sendFlag != 0) ||
                (menuMode == SAVE_MENU_EXTRA &&
                 (s8)save->summary[selectedFile].flagCount == 0) ||
                (menuMode == SAVE_MENU_CLEARED &&
                 (s8)save->summary[selectedFile].sendFlag == 0)));
    } else if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    } else if (gKeysPressed & KEY_A) {
      AudioCommand_PlayFar(SE_MENU_CONFIRM);
      result = selectedFile;
      break;
    }
  }
  // Close the menu and return to the current game's palette glow.
  SaveMenu_HideDjinnSprites();
  SaveMenu_HidePartySprites();
  UiWork_Finalize(djinnWindow, 2);
  UiWork_Finalize(partyWindow, 2);
  UiWork_Finalize(summaryWindow, 2);
  UiWork_Finalize(fileList, 2);
#if TBS_PATCH_1
  PaletteGlow_Stop();
#else
#endif
  // 55 is HEAP_SLOT_MENU, spriteWork's block.
  Runtime_ReleaseHeapBlock(HEAP_SLOT_MENU);
  PaletteGlow_Update(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
  WaitFrames(1);
done:
  return result;
}

/// The Save command: lets the player choose a file (the current one first),
/// asks whether to save over it when it has data, then saves the game to it,
/// object table included, and makes it the current file. The question is "OK
/// to save over this file?" Returns 0 (also when the player answers No), -1
/// when cancelled, or a SAVE_ERROR_* after showing its message.
s32 Save_WriteSelectedSlot(void) {
  s32 result;
  s32 file;
  s32 error;
  struct SaveWorkspace *save;

  result = 0;
  error = SaveState_InitializeWorkspace();
  if (error != 0) {
    UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    SaveState_LoadSummaryRecords();
    save = gSaveWorkspace;
    file = SaveMenu_SelectSlot(gSaveSlot, SAVE_MENU_SAVE);
    if (file == -1) {
      result = file;
    } else {
      if (save->summary[file].level != 0) {
        UiText_ShowPositionedMessageAndWait(MsgOverwriteConfirm, 13);
        while (UiWork_IsComplete() == 0) {
          WaitFrames(1);
        }
        if (Menu_RunConfirmSelection(1, 0, 0, 1) != 0) {
          UiWork_FinalizePendingCore();
          goto done;
        }
        UiWork_FinalizePendingCore();
      }
      gSaveSlot = file;
      AudioCommand_PlayFar(MUS_SAVED);
      UiText_ShowPositionedMessageAndWait(MsgSaving, 13);
      while (UiWork_IsComplete() == 0) {
        WaitFrames(1);
      }
      SaveState_BuildSummaryHeader();
      SaveState_CaptureObjectTableFar();
      error = SaveState_WriteRecord(file, gSaveBuffer);
      error |= SaveState_WriteRecord(file + SAVE_FILE_COUNT,
                                     gSaveBuffer + SAVE_RECORD_BYTES);
      UiWork_FinalizePendingCore();
      if (error != 0) {
        UiText_ShowPositionedMessageAndWait(MsgSaveFailed, 1);
        result = SAVE_ERROR_WRITE;
      } else {
        UiText_ShowPositionedMessageAndWait(MsgGameSaved, 9);
      }
    }
  }
done:
  SaveState_ReleaseWorkspace();
  return result;
}

/// Lets the player choose a file in save menu `mode` (see SaveMenuMode) and
/// loads it into the save buffer as the current file, restoring its play time
/// and options. Returns 0, -1 when cancelled, or a SAVE_ERROR_* after showing
/// its message. The title's Continue (mode 1), Battle (4) and Send (5) load
/// through here (menu/clear.c).
s32 SaveState_LoadRecordIntoWork(s32 mode) {
  s32 result = 0;
  s32 error = SaveState_InitializeWorkspace();

  if (error != 0) {
    UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    s32 file;

    SaveState_LoadSummaryRecords();
    file = SaveMenu_SelectSlot(gSaveSlot, mode);
    if (file == -1) {
      result = file;
    } else {
      void *half = &gSaveBuffer;

      error = SaveState_ReadRecordPayload(file, half);
      half = (char *)half + SAVE_RECORD_BYTES;
      error |= SaveState_ReadRecordPayload(file + SAVE_FILE_COUNT, half);
      if (error != 0) {
        UiText_ShowPositionedMessageAndWait(MsgLoadFailed, 1);
        result = SAVE_ERROR_READ;
      } else {
        gPlayTimeFrames = gGameState.playTime;
        gIdleSleepEnabled = gGameState.idleSleepEnabled;
        gIdleFrames = 0;
        gSaveSlot = file;
      }
    }
  }
  SaveState_ReleaseWorkspace();
  return result;
}

/// Loads the pointing hand cursor's tiles (gResource_FixedBlockBTiles, 128
/// bytes) into a free VRAM slot and returns the slot, which the caller frees
/// with Resource_ResetEntry. The slot is not checked: with the cache full
/// the load is given VRAM_CACHE_ENTRY_COUNT. Colosso's markers
/// (scene/common/colosso_marker.h) and, by far call, the world map's cursor
/// (object/psynergy_effects/world_map.c) use it.
s32 Resource_LoadFixedBlockBIntoFreeSlot(void) {
  s32 slot;

  slot = Resource_FindFreeEntry();
  VramCache_Load(slot, 0x80, gResource_FixedBlockBTiles);
  return slot;
}
