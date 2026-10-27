/// @file
/// Save files and the file menu.
// Part of screens.c.

#include "../screens_internal.h"

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
void *String_FormatPlayTime(s32 frames, u8 *out) {
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

  digitsCursor = String_FormatNumber(digits, hours, 3);
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

  digitsCursor = String_FormatNumber(digits, minutes, 2);
  textCursor++;
  *textCursor = digitsCursor[0];
  textCursor++;
  *textCursor = digitsCursor[1];
  textCursor[1] = 0;

  return out;
}

/// Returns the first save file whose summary shows it empty (level 0), or
/// SAVE_NO_FREE_FILE when all three are in use. The summaries must be loaded.
u32 Save_FindFreeFile(void) {
  u32 file;
  struct SaveFileSummary *summary;

  summary = gSaveWork->summary;
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
s32 Save_CountRecordsExcludingFlagged(s32 excludeSent) {
  s32 file;
  s32 count;
  struct SaveFileSummary *summary;

  if (Save_InitializeWork() != 0) {
    count = SAVE_ERROR_NO_MEMORY;
  } else {
    count = Save_LoadFileSummaries();
    if (excludeSent != 0) {
      summary = gSaveWork->summary;
      for (file = 0; file < SAVE_FILE_COUNT; file++) {
        if ((s8)summary[file].sendFlag != 0)
          count--;
      }
    }
  }
  Save_ReleaseWork();
  return count;
}

/// The distance in bytes from one field of a save summary to another.
#define SUMMARY_DISTANCE(from, to)                                             \
  ((u32)&((struct SaveFileSummary *)0)->to -                                   \
   (u32)&((struct SaveFileSummary *)0)->from)

/// Loads the save file summaries for the title menu and returns how many files
/// there are: plus 100 when every file has its send flag set, or
/// SAVE_ERROR_NO_MEMORY when the backup memory cannot be read. Sets
/// gTitleBattleOptionEnabled, which offers Battle, when a file has any of game
/// flags 48-127 set, and gTitleSendOptionEnabled, which offers Send, when a
/// file has its send flag set and R and Left are held.
/// FileList_SelectAction reads all three.
s32 Save_ScanRecordFlags(void) {
  s32 error;
  s32 sentCount;
  s32 fileCount;

  error = Save_InitializeWork();
  sentCount = 0;
  fileCount = SAVE_ERROR_NO_MEMORY;
  if (error == 0) {
    s32 i;
    s8 *flags;

    fileCount = Save_LoadFileSummaries();
    // A signed byte cursor from the party's end keeps the native literal base
    // 0x1070 and flag offsets 1 and 2.
    flags = (s8 *)&gSaveWork->summary[0].party[4];
    gTitleSendOptionEnabled = 0;
    (*(s16 *)&gTitleBattleOptionEnabled) = 0;
    for (i = 0; i < SAVE_FILE_COUNT; i++) {
      if (flags[i * sizeof(struct SaveFileSummary) +
                SUMMARY_DISTANCE(party[4], sendFlag)] != 0) {
        gTitleSendOptionEnabled = 1;
        sentCount++;
      }
      if (flags[i * sizeof(struct SaveFileSummary) +
                SUMMARY_DISTANCE(party[4], flagCount)] != 0) {
        (*(s16 *)&gTitleBattleOptionEnabled) = 1;
      }
    }

    if ((gKeysHeld & (KEY_R | KEY_LEFT)) != (KEY_R | KEY_LEFT)) {
      gTitleSendOptionEnabled = 0;
    }
  }
  Save_ReleaseWork();
  if (fileCount != 0 && sentCount == fileCount) {
    return fileCount + 100;
  }
  return fileCount;
}

/// Prepares the save buffer for writing: stamps the save block with the time
/// and options, then fills the file summary shown on the file menus (the
/// leader's name, level and class, the play time, area, coins, Djinn of each
/// element, the party, the send flag and how many of game flags 48-127 are
/// set) and its checksum, the sum of the 968 words from gGameFlagBytes on.
/// Returns the checksum.
u32 Save_BuildFileSummary(void) {
  u16 members[14];
  struct SaveFileSummary *summary;
  struct Unit *leader;
  u8 *source;
  u8 *destination;
  u32 *word;
  s32 i;
  u32 sum = 0;

  // The summary's 16-byte header comes before the save buffer.
  summary = (struct SaveFileSummary *)((u32)gSaveBuffer - 16);
  gSaveBlock.buildStamp = BuildStamp_GetTimeFar(0);
  gSaveBlock.playTime = gPlayTimeFrames;
  // A second copy of the play time, 256 bytes from gSceneLocalState: past its
  // 0x78 bytes, in gInventorySnapshot's last word. gSceneLocalState opens the
  // save buffer's second half, so this lands at offset 0x110 of the file's
  // second record (after its 16-byte header), which
  // Save_LoadFileSummaries reads back into
  // SaveFileSummary.secondRecordPlayTime: the file menu lists a file whose two
  // play times differ as corrupted.
  {
    u32 *copy = (u32 *)gSceneLocalState;
    copy[64] = gPlayTimeFrames;
  }
  gSaveBlock.idleSleepEnabled = gIdleSleepEnabled;

  leader = Unit_GetFar(gSaveBlock.leaderActorId);
  destination = summary->name;
  source = leader->name;
  for (i = 11; i >= 0; i--)
    *destination++ = *source++;
  summary->level = leader->level;
  summary->playTime = gSaveBlock.playTime;
  summary->area = Area_GetNameIndexFar(gSaveBlock.sceneId, gSaveBlock.entrance);
  summary->classId = leader->classId;
  summary->coins = gSaveBlock.coins;
  summary->djinn[0] = Party_SumDjinnCountsFar(0);
  summary->djinn[1] = Party_SumDjinnCountsFar(1);
  summary->djinn[2] = Party_SumDjinnCountsFar(2);
  summary->djinn[3] = Party_SumDjinnCountsFar(3);
  // Up to four party members, ended by -1.
  Party_ListMembersFar(members);
  for (i = 0; i <= 3 && members[i] != 255; i++)
    summary->party[i] = members[i];
  summary->party[i] = -1;
  summary->windowHue = gSaveBlock.windowColor[0];
  summary->windowBrightness = gSaveBlock.windowColor[1];
  summary->sendFlag = gSaveBlock.sendFlag;
  summary->flagCount = 0;
  for (i = 48; i <= 127; i++) {
    if (GameFlag_GetFar(i))
      summary->flagCount++;
  }
  summary->childhood = GameFlag_GetFar(FLAG_CHILDHOOD) != 0;
  {
    s32 n;
    u32 *frame = &gSaveBlock.buildStamp;
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
/// Save_ToChosenFile it asks nothing and does not capture the object
/// table first (Save_CaptureActorsFar).
s16 Save_WriteCurrentFile(void) {
  s16 file;
  s16 result;
  s32 error;

  result = 0;
  file = gSaveSlot;
  if (file != -1) {
    error = Save_InitializeWork();
    if (error != 0) {
      FieldMessage_Show(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
      result = SAVE_ERROR_NO_MEMORY;
    } else {
      Save_BuildFileSummary();
      {
        void *half = &gSaveBuffer;
        s32 current;

        error = Save_WriteRecord(gSaveSlot, half);
        current = gSaveSlot;
        half = (char *)half + SAVE_RECORD_BYTES;
        error |= Save_WriteRecord(current + SAVE_FILE_COUNT, half);
        if (error != 0) {
          FieldMessage_Show(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
          result = SAVE_ERROR_WRITE;
        }
      }
    }
    Save_ReleaseWork();
    file = result;
  }
  return file;
}

/// Writes both halves of the save buffer, as they are, to save file `file`.
/// Returns 0, or a SAVE_ERROR_* after showing its message.
s32 Save_WriteFile(s32 file) {
  s32 error;
  s16 result = 0;

  error = Save_InitializeWork();
  if (error != 0) {
    FieldMessage_Show(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    void *half = &gSaveBuffer;

    error = Save_WriteRecord(file, half);
    half = (char *)half + SAVE_RECORD_BYTES;
    error |= Save_WriteRecord(file + SAVE_FILE_COUNT, half);
    if (error != 0) {
      FieldMessage_Show(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
      result = SAVE_ERROR_WRITE;
    }
  }
  Save_ReleaseWork();
  return result;
}

/// Writes the 64-byte save stamp (gBattleArenaRecord, which lies in the first
/// half of the save buffer) into the current file's first record, leaving the
/// rest of the record as it was saved. Returns 0, -1 when no file is chosen, or
/// a SAVE_ERROR_* after showing its message. The Battle Arena saves its battle
/// results this way (BattleArena_SaveBattleResults). The record's 4 KB buffer
/// is taken from the EWRAM pool and freed before it returns, except when
/// no file is chosen.
s32 Save_ProcessSelectedFile(void) {
  void *record;
  s32 result;
  s32 error;
  u8 *stamp;

  record = AllocEwram(SAVE_RECORD_BYTES);
  result = 0;
  // BUG: returns without freeing the record buffer.
  if (gSaveSlot == -1)
    return -1;
  error = Save_InitializeWork();
  if (error != 0) {
    FieldMessage_Show(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    error = Save_ReadRecord(gSaveSlot, record);
    // A failed read is reported, but the record is still written below from
    // whatever the buffer holds.
    if (error != 0) {
      FieldMessage_Show(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
      result = SAVE_ERROR_READ;
    }
    // The stamp's place in the record is its place in the save buffer.
    stamp = (u8 *)record + (s32)gBattleArenaRecord;
    stamp -= (s32)gSaveBuffer;
    Iwram_CopyWords(stamp, gBattleArenaRecord, 16);
    error = Save_WriteRecord(gSaveSlot, record);
    if (error != 0) {
      FieldMessage_Show(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
      result = SAVE_ERROR_WRITE;
    }
  }
  Save_ReleaseWork();
  Free(record);
  return result;
}

/// Asks whether to save over the current file and, on Yes, writes the save
/// stamp into it (Save_ProcessSelectedFile) and says so. "OK to save over
/// this file?" and "Your game was saved." Returns that result.
UNUSED s32 Save_ConfirmAndProcessSelectedFile(void) {
  s32 result;

  // UB: on No, result is returned without ever being set.
#ifdef UBFIX
  result = 0;
#endif

  Message_OpenWindow(MsgOverwriteConfirm, 8, 12, 2);
  while (TextPrinter_AllFinished() == 0) {
    WaitFrames(1);
  }
  if (YesNoMenu_Run(1, 0, 0, 1)) {
    TextPrinter_CloseFinishedWindows();
  } else {
    TextPrinter_CloseFinishedWindows();
    Sound_PlaySongFar(MUS_SAVED);
    result = Save_ProcessSelectedFile();
    if (result >= 0) {
      FieldMessage_Show(MsgGameSaved, FIELD_MESSAGE_ZOOM);
    }
  }
  return result;
}

/// The file menu's Copy (menu/main_menu.c): lets the player choose a file,
/// reads both of its records into the save buffer, overwriting it, and writes
/// them to the first empty file. Returns 0, -1 when cancelled, or a
/// SAVE_ERROR_* after showing its message (the game says "Failed to erase."
/// both when no file is empty and when the write fails).
s32 Save_CopyFile(void) {
  u32 error;
  s32 file;
  s32 result;

  result = 0;
  error = Save_InitializeWork();
  if (error != 0) {
    FieldMessage_Show(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    Save_LoadFileSummaries();
    file = FileList_SelectFile(0, 2);
    if (file == -1) {
      result = file;
    } else {
      void *lower = &gSaveBuffer;
      void *upper;

      error = Save_ReadRecord(file, lower);
      upper = (u8 *)lower + SAVE_RECORD_BYTES;
      error |= Save_ReadRecord(file + SAVE_FILE_COUNT, upper);
      if (error != 0) {
        FieldMessage_Show(MsgLoadFailed, FIELD_MESSAGE_ZOOM);
        result = SAVE_ERROR_READ;
      } else {
        file = Save_FindFreeFile();
        if (file == SAVE_NO_FREE_FILE) {
          FieldMessage_Show(MsgEraseFailed, FIELD_MESSAGE_ZOOM);
          result = SAVE_ERROR_NO_FILE;
        } else {
          error = Save_WriteRecord(file, lower);
          error |= Save_WriteRecord(file + SAVE_FILE_COUNT, upper);
          if (error != 0) {
            FieldMessage_Show(MsgEraseFailed, FIELD_MESSAGE_ZOOM);
            result = SAVE_ERROR_WRITE;
          } else {
            FieldMessage_Show(MsgFileCopied, FIELD_MESSAGE_ZOOM);
          }
        }
      }
    }
  }

  Save_ReleaseWork();
  return result;
}

/// The file menu's Erase (menu/main_menu.c): lets the player choose a file,
/// asks whether to erase it and on Yes deletes both of its records. "OK to
/// erase this?" Returns 0 (also when the player answers No), -1 when cancelled,
/// or a SAVE_ERROR_* after showing its message.
s32 Save_EraseSelectedFile(void) {
  s32 error;
  s32 file;
  s32 result = 0;

  error = Save_InitializeWork();
  if (error != 0) {
    FieldMessage_Show(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    Save_LoadFileSummaries();
    file = FileList_SelectFile(0, 3);
    if (file == -1) {
      result = file;
    } else {
      Message_OpenWindow(MsgEraseConfirm, 8, 1, 2);
      while (TextPrinter_AllFinished() == 0) {
        WaitFrames(1);
      }
      if (YesNoMenu_Run(1, 0, 3, 1) != 0) {
        TextPrinter_CloseFinishedWindows();
      } else {
        TextPrinter_CloseFinishedWindows();
        error = Save_DeleteRecord(file);
        error |= Save_DeleteRecord(file + SAVE_FILE_COUNT);
        if (error != 0) {
          FieldMessage_Show(MsgEraseFailed, FIELD_MESSAGE_ZOOM);
          result = SAVE_ERROR_ERASE;
        } else {
          FieldMessage_Show(MsgFileErased, FIELD_MESSAGE_ZOOM);
        }
      }
    }
  }
  Save_ReleaseWork();
  return result;
}

#if TBS_PATCH_1

/// A task that makes colours 8-11 of HIGHLIGHT_PALETTE glow: each
/// follows a sine wave of the frame counter, eight frames behind the one
/// before. With n the sine as a whole number from -4 to 4 (Trig_Sin's 16.16
/// result over 16384, rounded toward zero), the colour is red 22 + 2n, green
/// 16 + n and blue 20 + n (of 31): red 14 to 30, green 12 to 20 and blue 16
/// to 24. The step of 0x300 a frame, in 1/65536 turns, makes a cycle of about
/// 85 frames. FileList_StartPulse runs it while the file menu is open.
void FileList_UpdatePulse(void) {
  s32 *tick;
  volatile u16 *palette;
  s32 colour;
  s32 sine;
  s32 i;

  tick = (s32 *)&gVBlankCount;
  palette = &BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 8);
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

/// Starts the palette glow (FileList_UpdatePulse) as a task.
void FileList_StartPulse(void) {
  Task_Add(FileList_UpdatePulse, TASK_PASS_FRAME);
}

void FileList_StopPulse(void) { Task_Remove(FileList_UpdatePulse); }

/// Fills the `width` x `height` tile rectangle at (x, y) inside `window`,
/// clipped to the screen as Window_SetTextTileBitRect clips, with the
/// tile entry 0xe006: tile 6 in HIGHLIGHT_PALETTE, the palette
/// FileList_UpdatePulse animates. The file menu marks the file under
/// the cursor with it.
void Window_FillTilemapRect(struct Window *window, s32 x, s32 y, s32 width,
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
    ((struct WindowWork *)tilemap)->dirty = 1;
  }
}

#endif

/// The file menu shows the chosen file's party as walking sprites in a
/// row, and four Djinn sprites, one for each element, over the file's Djinn
/// counts. Each sprite stands three tiles (24 pixels) from the one before.
#define FILE_LIST_SPRITE_SPACING 3
/// Task priority of the tasks that place the sprites every frame.
#define FILE_LIST_SPRITE_PRIORITY (200 << 4)

/// Shows the party of save file `summary` (up to four members) as sprites in
/// a row from tile (x, y) inside `window`, and starts placing them every
/// frame (FileList_PlacePartySprites).
void FileList_ShowPartySprites(struct Window *window, s32 x, s32 y,
                               struct SaveFileSummary *summary) {
  struct ShopWork *menu = gMenuWork;
  s32 i;
  s32 priority;

  for (i = 0; i < 4 && summary->party[i] != -1; i++) {
    struct Sprite *sprite = Sprite_CreateFar(Party_RemapCharacterIdIfEnabledFar(
        summary->party[i], (s8)summary->childhood));

    if (sprite != NULL) {
      Sprite_StartAnimFar(sprite, 1);
      sprite->flags = 0;
      sprite->image.priority = 0;
    }

    menu->partyMemberIcons[i] = sprite;
    menu->partyMemberX[i] =
        (x + window->x + i * FILE_LIST_SPRITE_SPACING) * 8 + 16;
    menu->partyMemberY[i] = (y + window->y) * 8 + 16;
    // 1.0 in 16.16.
    menu->partyMemberScale[i] = Q16_ONE;
  }

  priority = 200;
  priority <<= 4;
  Task_Add(FileList_PlacePartySprites, priority);
}

/// Stops placing the party sprites of the file menu and releases them.
void FileList_HidePartySprites(void) {
  struct ShopWork *menu = gMenuWork;
  s32 i;

  Task_Remove(FileList_PlacePartySprites);
  for (i = 0; i < 4; i++) {
    if (menu->partyMemberIcons[i] != NULL) {
      Sprite_DestroyFar(menu->partyMemberIcons[i]);
      menu->partyMemberIcons[i] = NULL;
    }
  }
}

/// A task: draws each party sprite of the file menu at its
/// place and scale for this frame.
void FileList_PlacePartySprites(void) {
  struct ShopWork *menu = gMenuWork;
  struct SpriteScale scale;
  struct SpritePlacement position;
  s32 index = 0;
  struct Sprite *sprite;

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
      Actor_ApplyProjectedPlacementFar(sprite, &position, &scale, FACING_SOUTH);
    }
    index += 1;
    if (index > 3) {
      break;
    }
  }
}

/// Shows the four Djinn sprites, one for each element, in a row from tile
/// (x, y) inside `window`, and starts placing them every frame
/// (FileList_PlaceDjinnSprites). Does nothing when `window` is NULL.
void FileList_ShowDjinnSprites(struct Window *window, s32 x, s32 y) {
  struct ItemMenuFiguresView *menu = gMenuWork;
  s32 i;

  if (window != NULL) {
    for (i = 0; i < 4; i++) {
      struct Sprite *sprite = Sprite_CreateFar(gMenu_PartySpriteResourceIds[i]);

      if (sprite != NULL) {
        Sprite_StartAnimFar(sprite, 2);
        sprite->flags = 0;
        sprite->image.priority = 0;
      }

      menu->animations[i] = sprite;
      menu->positionsX[i] =
          (window->x + x + i * FILE_LIST_SPRITE_SPACING) * 8 + 16;
      menu->positionsY[i] = (window->y + y) * 8 + 16;
    }

    Task_Add(FileList_PlaceDjinnSprites, FILE_LIST_SPRITE_PRIORITY);
  }
}

/// Stops placing the Djinn sprites of the file menu and releases them.
void FileList_HideDjinnSprites(void) {
  struct ItemMenuFiguresView *menu = gMenuWork;
  s32 i;

  Task_Remove(FileList_PlaceDjinnSprites);
  for (i = 0; i < 4; i++) {
    if (menu->animations[i] != NULL) {
      Sprite_DestroyFar(menu->animations[i]);
      menu->animations[i] = NULL;
    }
  }
}

/// A task: draws each Djinn sprite of the file menu at its
/// place, at scale 1.0, for this frame.
void FileList_PlaceDjinnSprites(void) {
  struct ItemMenuFiguresView *menu = gMenuWork;
  struct SpriteScale scale;
  struct SpritePlacement position;
  s32 index;

  index = 0;
  do {
    struct Sprite *sprite = menu->animations[index];

    if (sprite != NULL) {
      s32 one = Q16_ONE;
      s32 height = 500 << 16;

      scale.x = one;
      scale.y = one;
      position.x = menu->positionsX[index] << 16;
      position.height = height;
      position.y = (menu->positionsY[index] << 16) + height;
      position.groundHeight = 0;
      Actor_ApplyProjectedPlacementFar(sprite, &position, &scale, FACING_SOUTH);
    }
    index++;
  } while (index <= 3);
}

/// Draws the Djinn counts of save file `summary`, one for each element, two
/// digits each, 24 pixels apart on row 16 of `window`, under the Djinn
/// sprites. Does nothing when `window` is NULL.
void FileList_DrawDjinnCounts(struct Window *window,
                              struct SaveFileSummary *summary) {
  s32 i;

  if (window != NULL) {
    for (i = 0; i < 4; i++)
      Text_DrawNumberAtOffset((s8)summary->djinn[i], 2, window,
                              i * FILE_LIST_SPRITE_SPACING * 8, 16);
  }
}

// Where each edition draws the "Lv" label and the coin label, in pixels.
#if (LANGUAGE == LANGUAGE_SPANISH)
#define SUMMARY_LEVEL_X 64
#define SUMMARY_COINS_X 56
#define SUMMARY_DRAW_LEVEL Text_DrawStringAtOffset
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define SUMMARY_LEVEL_X 59
#define SUMMARY_COINS_X 56
#define SUMMARY_DRAW_LEVEL Text_DrawStringAtOffset
#else
#define SUMMARY_LEVEL_X 72
#define SUMMARY_COINS_X 48
#define SUMMARY_DRAW_LEVEL Text_DrawStringInWindow
#endif

/// Draws the summary of save file `summary` into `window`: the leader's name,
/// level and class, the play time and the coins. Does nothing when `window`
/// is 0.
void FileList_DrawSummary(struct Window *window,
                          struct SaveFileSummary *summary) {
  u32 playTime[4]; // the play time text
  s32 y;

  if (window != 0) {
    WindowSprite_PrepareForRedraw(window);
    Window_DrawDividerLine(window, 0, 4, 13, 4);
    Text_DrawStringAtOffset(summary->name, window, 0, 0);
    y = 0;
    SUMMARY_DRAW_LEVEL(gStatusScreen_LevelLetterString, window, SUMMARY_LEVEL_X,
                       0);
    Text_DrawNumberAtOffset(summary->level, 2, window, 80, y);
    Text_DrawMessageAsTiles(summary->classId + MsgClassName, window, 0, 16);
#if LANGUAGE == LANGUAGE_JAPANESE
    // Coins on the third row, then "Play Time:" and the time.
    y = 32;
    Text_DrawNumberInWindow(summary->coins, 6, window, 24, y);
    Text_DrawMessageAsTiles(MsgCoins, window, 72, 32);
    Text_DrawMessageAsTiles(MsgStatusLabel, window, 0, 48);
    Text_DrawStringInWindow(
        String_FormatPlayTime(summary->playTime, (u8 *)playTime), window, 48,
        48);
#else
    // "Play Time:" with the time under it, then the coins.
    Text_DrawMessageAsTiles(MsgStatusLabel, window, 0, 32);
    Text_DrawStringInWindow(
        String_FormatPlayTime(summary->playTime, (u8 *)playTime), window, 48,
        40);
    y = 48;
    Text_DrawNumberInWindow(summary->coins, 6, window, 0, y);
#if (LANGUAGE == LANGUAGE_FRENCH)
    // French takes the singular "Pièce" for one coin or none.
    if (summary->coins > 1)
      Text_DrawMessageAsTiles(MsgCoins, window, SUMMARY_COINS_X, 48);
    else
      Text_DrawMessageAsTiles(MsgPiece, window, SUMMARY_COINS_X, 48);
#else
    Text_DrawMessageAsTiles(MsgCoins, window, SUMMARY_COINS_X, 48);
#endif
#endif
  }
}

/// What the file menu is choosing a file for, which decides the files
/// the cursor may stop on. Each mode also indexes the heading the menu shows
/// (gMenuTitlePictures: Save, Continue, Copy, Erase, Battle,
/// Link).
enum FileMenuMode {
  FILE_LIST_SAVE,     ///< saving: any file, empty ones too
  FILE_LIST_CONTINUE, ///< continuing: files not yet cleared
  FILE_LIST_COPY,     ///< copying: any file with data
  FILE_LIST_ERASE,    ///< erasing: any file with data
  /// The title's Battle: files with any of game flags 48-127 set.
  FILE_LIST_EXTRA,
  FILE_LIST_CLEARED ///< the title's Send: cleared files only
};

/// FileList_SelectFile's result when no file can be chosen in its mode.
#define FILE_LIST_NO_FILE -2

// The file menu: lists the three files with each one's leader and area
// (or why it cannot be chosen) and, for the file under the cursor, its
// summary, its party and its Djinn, with the file's own palette glow. The
// cursor starts on `file` and moves up and down over the files `mode` allows
// (see FileMenuMode). Returns the file chosen with A, -1 when cancelled with
// B, or FILE_LIST_NO_FILE when no file is allowed.
s32 FileList_SelectFile(s32 selectedFile, s32 menuMode) {
  struct Window *fileList;
#if TBS_PATCH_1
  struct SaveWork *save;
  struct Window *summaryWindow;
  struct Window *partyWindow;
  struct Window *djinnWindow;
  struct SaveFileSummary *summary;
  struct WindowSprite *modePicture;
  s32 summaryDirty;
  struct WindowWork *work;
  u32 buildStamp;
  struct ShopWork *spriteWork;
  s32 result;
  s32 i;
  s32 y;
#else
  struct Window *summaryWindow;
  struct SaveWork *save;
  struct Window *partyWindow;
  struct Window *djinnWindow;
  struct SaveFileSummary *summary;
  struct WindowSprite *modePicture;
  s32 cursorStyle;
  s32 summaryDirty;
  struct WindowWork *work;
  u32 buildStamp;
  struct ShopWork *spriteWork;
  struct MenuCursor fileCursor;
  s32 result;
  s32 i;
  s32 x;
  s32 y;
  s32 vramSlot;
  struct WindowSprite *cursorSprite;
#endif

  spriteWork = AllocHeapSlot(HEAP_SLOT_MENU, sizeof(*spriteWork));
  save = *(gHeapSlots + HEAP_SLOT_SAVE);
  summaryWindow = NULL;
  partyWindow = NULL;
  djinnWindow = NULL;
  summaryDirty = 1;
  work = *(gHeapSlots + HEAP_SLOT_WINDOW);
  buildStamp = BuildStamp_GetTimeFar();
  // Start on the first file the mode allows.
  // BUG: when none is, the jump to done returns FILE_LIST_NO_FILE without
  // releasing spriteWork's HEAP_SLOT_MENU block.
  if (selectedFile < 0)
    selectedFile = 0;
#if TBS_PATCH_1
  if (menuMode == FILE_LIST_CONTINUE) {
    for (i = 0;
         i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
                                 (s8)save->summary[selectedFile].sendFlag != 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = FILE_LIST_NO_FILE;
      goto done;
    }
  } else if (menuMode == FILE_LIST_EXTRA) {
    for (i = 0; i < SAVE_FILE_COUNT &&
                (save->summary[selectedFile].level == 0 ||
                 (s8)save->summary[selectedFile].flagCount == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = FILE_LIST_NO_FILE;
      goto done;
    }
  } else if (menuMode == FILE_LIST_CLEARED) {
    for (i = 0;
         i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
                                 (s8)save->summary[selectedFile].sendFlag == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = FILE_LIST_NO_FILE;
      goto done;
    }
  } else if (menuMode != FILE_LIST_SAVE) {
    for (i = 0; i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = FILE_LIST_NO_FILE;
      goto done;
    }
  }

  // The file list, and the mode's sprite above it.
  Dma_Fill32(0, spriteWork, sizeof(*spriteWork));
  FileList_StartPulse();
  fileList = Window_Create(1, 2, 28, 7, WINDOW_FLAG_INSTANT);
  summary = save->summary;
  y = 0;
  for (i = 0; i < SAVE_FILE_COUNT; i++, y += 16, summary++) {
    if (summary->level == 0) {
      Text_DrawMessage(MsgSaveNoData, fileList, 10, y);
    } else if (summary->frames < buildStamp) {
      // Saved by an older build: "(Continue from a Sanctum)", since such a
      // file resumes at SaveBlock.restartScene, not where it was saved
      // (menu/main_menu.c).
      Text_DrawMessage(MsgSaveContinue, fileList, 10, y);
    } else if (summary->playTime != *(u32 *)summary->secondRecordPlayTime) {
      Text_DrawMessage(MsgSaveCorrupted, fileList, 10, y);
    } else if (menuMode == FILE_LIST_CLEARED && (s8)summary->sendFlag == 0) {
      Text_DrawMessage(MsgSaveNotCleared, fileList, 10, y);
    } else {
      Text_DrawString(summary->name, fileList, FILE_LIST_NAME_X, y);
      Text_DrawMessage(summary->area + MsgDebugEntryName, fileList, 62, y);
      work->dirty = 1;
    }
  }
  Window_DrawDividerLine(fileList, 0, 2, 27, 2);
  Window_DrawDividerLine(fileList, 0, 4, 27, 4);
#else
  if (menuMode != FILE_LIST_SAVE) {
    for (i = 0; i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = FILE_LIST_NO_FILE;
      goto done;
    }
  }
  if (menuMode == FILE_LIST_CONTINUE) {
    for (i = 0;
         i < SAVE_FILE_COUNT && ((s8)save->summary[selectedFile].sendFlag != 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = FILE_LIST_NO_FILE;
      goto done;
    }
  }
  if (menuMode == FILE_LIST_EXTRA) {
    for (i = 0; i < SAVE_FILE_COUNT &&
                ((s8)save->summary[selectedFile].flagCount == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = FILE_LIST_NO_FILE;
      goto done;
    }
  }
  if (menuMode == FILE_LIST_CLEARED) {
    for (i = 0;
         i < SAVE_FILE_COUNT && ((s8)save->summary[selectedFile].sendFlag == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = FILE_LIST_NO_FILE;
      goto done;
    }
  }

  // The file list, and the mode's sprite above it.
  Dma_Fill32(0, spriteWork, sizeof(*spriteWork));
  fileList = Window_Create(1, 2, 28, 7, WINDOW_FLAG_INSTANT);

  for (i = 0, summary = save->summary; i < SAVE_FILE_COUNT; i++, summary++) {
    if (save->summary[i].level == 0) {
      Text_DrawMessage(MsgSaveNoData, fileList, 10, i * 16);
    } else if (save->summary[i].frames < buildStamp) {
      // Saved by an older build: see the other editions' list above.
      Text_DrawMessage(MsgSaveContinue, fileList, 10, i * 16);
    } else if (save->summary[i].playTime !=
               *(u32 *)save->summary[i].secondRecordPlayTime) {
      Text_DrawMessage(MsgSaveCorrupted, fileList, 10, i * 16);
    } else if (menuMode == FILE_LIST_CLEARED &&
               (s8)save->summary[i].sendFlag == 0) {
      Text_DrawMessage(MsgSaveNotCleared, fileList, 10, i * 16);
    } else {
      y = i * 16;
      Text_DrawString(summary->name, fileList, FILE_LIST_NAME_X, y);
      Text_DrawMessage(save->summary[i].area + MsgDebugEntryName, fileList, 72,
                       y);
      work->dirty = 1;
    }
  }
  Window_DrawDividerLine(fileList, 0, 2, 27, 2);
  Window_DrawDividerLine(fileList, 0, 4, 27, 4);
  vramSlot = VramCache_FindFreeEntry();
  if (vramSlot < VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(vramSlot, 128, gPointingHandTiles);
    x = -12;
    y = selectedFile * 16 + 4;
    cursorSprite =
        WindowSprite_Create(vramSlot, OUTPUT_SIZE_16X16, fileList, x, y);
    cursorSprite->motion = 1;
    fileCursor.anchor = cursorSprite;
  }
  cursorStyle = 1;
#endif
  modePicture = MenuTitle_Create(menuMode, fileList, 72, -24);

  while (1) {
    // Show the file under the cursor after it moves.
    if (summaryDirty != 0) {
      summaryDirty = 0;
      if (save->summary[selectedFile].level != 0) {
        WindowColor_Apply(save->summary[selectedFile].windowHue,
                          save->summary[selectedFile].windowBrightness);
        if (summaryWindow == NULL)
          summaryWindow = Window_Create(1, 10, 14, 9, WINDOW_FLAG_INSTANT);
        FileList_DrawSummary(summaryWindow, &save->summary[selectedFile]);
        WaitFrames(1);
        if (partyWindow == NULL)
          partyWindow = Window_Create(16, 10, 13, 3, WINDOW_FLAG_INSTANT);
        FileList_HidePartySprites();
        FileList_ShowPartySprites(partyWindow, 0, 0,
                                  &save->summary[selectedFile]);
        WaitFrames(1);
        if ((s8)save->summary[selectedFile].djinn[0] +
                (s8)save->summary[selectedFile].djinn[1] +
                (s8)save->summary[selectedFile].djinn[2] +
                (s8)save->summary[selectedFile].djinn[3] !=
            0) {
          if (djinnWindow == NULL)
            djinnWindow = Window_Create(16, 14, 13, 5, WINDOW_FLAG_INSTANT);
          FileList_DrawDjinnCounts(djinnWindow, &save->summary[selectedFile]);
          FileList_HideDjinnSprites();
          FileList_ShowDjinnSprites(djinnWindow, 0, 0);
        } else {
          FileList_HideDjinnSprites();
          Window_Close(djinnWindow, 2);
          djinnWindow = NULL;
        }
      } else {
        WindowColor_Apply(gSaveBlock.windowColor[0], gSaveBlock.windowColor[1]);
        FileList_HideDjinnSprites();
        FileList_HidePartySprites();
        Window_Close(djinnWindow, 2);
        Window_Close(partyWindow, 2);
        Window_Close(summaryWindow, 2);
        djinnWindow = NULL;
        partyWindow = NULL;
        summaryWindow = NULL;
      }
#if TBS_PATCH_1
      WindowSprite_RedrawSavedRect(fileList);
      Window_DrawDividerLine(fileList, 0, 2, 27, 2);
      Window_DrawDividerLine(fileList, 0, 4, 27, 4);
      Window_FillTilemapRect(fileList, 0, selectedFile * 2, 26, 1);
    }
#else
      x = fileList->x * 8 - 4;
      y = (fileList->y << 3) + selectedFile * 16 + 12;
      MenuCursor_SetFar(&fileCursor, x, y, cursorStyle);
      cursorStyle = 3;
    }
    MenuCursor_AdvanceFar(&fileCursor);
#endif
    MenuTitle_Bob(modePicture);
    WaitFrames(1);
    // Up and down move to the previous or next file the mode allows; saving
    // allows every file, an empty one too.
    if (gKeysRepeat & KEY_UP) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      summaryDirty = 1;
      do {
        selectedFile = (selectedFile + SAVE_FILE_COUNT - 1) % SAVE_FILE_COUNT;
        if (menuMode == FILE_LIST_SAVE)
          break;
      } while ((save->summary[selectedFile].level == 0 ||
                (menuMode == FILE_LIST_CONTINUE &&
                 (s8)save->summary[selectedFile].sendFlag != 0) ||
                (menuMode == FILE_LIST_EXTRA &&
                 (s8)save->summary[selectedFile].flagCount == 0) ||
                (menuMode == FILE_LIST_CLEARED &&
                 (s8)save->summary[selectedFile].sendFlag == 0)));
    } else if (gKeysRepeat & KEY_DOWN) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      summaryDirty = 1;
      do {
        selectedFile = (selectedFile + SAVE_FILE_COUNT + 1) % SAVE_FILE_COUNT;
        if (menuMode == FILE_LIST_SAVE)
          break;
      } while ((save->summary[selectedFile].level == 0 ||
                (menuMode == FILE_LIST_CONTINUE &&
                 (s8)save->summary[selectedFile].sendFlag != 0) ||
                (menuMode == FILE_LIST_EXTRA &&
                 (s8)save->summary[selectedFile].flagCount == 0) ||
                (menuMode == FILE_LIST_CLEARED &&
                 (s8)save->summary[selectedFile].sendFlag == 0)));
    } else if (gKeysPressed & KEY_B) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      result = -1;
      break;
    } else if (gKeysPressed & KEY_A) {
      Sound_PlaySongFar(SE_MENU_CONFIRM);
      result = selectedFile;
      break;
    }
  }
  // Close the menu and return to the current game's palette glow.
  FileList_HideDjinnSprites();
  FileList_HidePartySprites();
  Window_Close(djinnWindow, 2);
  Window_Close(partyWindow, 2);
  Window_Close(summaryWindow, 2);
  Window_Close(fileList, 2);
#if TBS_PATCH_1
  FileList_StopPulse();
#else
#endif
  // 55 is HEAP_SLOT_MENU, spriteWork's block.
  FreeHeapSlot(HEAP_SLOT_MENU);
  WindowColor_Apply(gSaveBlock.windowColor[0], gSaveBlock.windowColor[1]);
  WaitFrames(1);
done:
  return result;
}

/// The Save command: lets the player choose a file (the current one first),
/// asks whether to save over it when it has data, then saves the game to it,
/// object table included, and makes it the current file. The question is "OK
/// to save over this file?" Returns 0 (also when the player answers No), -1
/// when cancelled, or a SAVE_ERROR_* after showing its message.
s32 Save_ToChosenFile(void) {
  s32 result;
  s32 file;
  s32 error;
  struct SaveWork *work;

  result = 0;
  error = Save_InitializeWork();
  if (error != 0) {
    FieldMessage_Show(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    Save_LoadFileSummaries();
    work = gSaveWork;
    file = FileList_SelectFile(gSaveSlot, FILE_LIST_SAVE);
    if (file == -1) {
      result = file;
    } else {
      if (work->summary[file].level != 0) {
        FieldMessage_Show(MsgOverwriteConfirm, FIELD_MESSAGE_ZOOM |
                                                   FIELD_MESSAGE_KEEP_OPEN |
                                                   FIELD_MESSAGE_LOWER);
        while (TextPrinter_AllFinished() == 0) {
          WaitFrames(1);
        }
        if (YesNoMenu_Run(1, 0, 0, 1) != 0) {
          TextPrinter_CloseFinishedWindows();
          goto done;
        }
        TextPrinter_CloseFinishedWindows();
      }
      gSaveSlot = file;
      Sound_PlaySongFar(MUS_SAVED);
      FieldMessage_Show(MsgSaving, FIELD_MESSAGE_ZOOM |
                                       FIELD_MESSAGE_KEEP_OPEN |
                                       FIELD_MESSAGE_LOWER);
      while (TextPrinter_AllFinished() == 0) {
        WaitFrames(1);
      }
      Save_BuildFileSummary();
      Save_CaptureActorsFar();
      error = Save_WriteRecord(file, gSaveBuffer);
      error |= Save_WriteRecord(file + SAVE_FILE_COUNT,
                                gSaveBuffer + SAVE_RECORD_BYTES);
      TextPrinter_CloseFinishedWindows();
      if (error != 0) {
        FieldMessage_Show(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
        result = SAVE_ERROR_WRITE;
      } else {
        FieldMessage_Show(MsgGameSaved,
                          FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
      }
    }
  }
done:
  Save_ReleaseWork();
  return result;
}

/// Lets the player choose a file in file menu `mode` (see FileMenuMode) and
/// loads it into the save buffer as the current file, restoring its play time
/// and options. Returns 0, -1 when cancelled, or a SAVE_ERROR_* after showing
/// its message. The title's Continue (mode 1), Battle (4) and Send (5) load
/// through here (menu/main_menu.c).
s32 Save_LoadRecordIntoWork(s32 mode) {
  s32 result = 0;
  s32 error = Save_InitializeWork();

  if (error != 0) {
    FieldMessage_Show(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    s32 file;

    Save_LoadFileSummaries();
    file = FileList_SelectFile(gSaveSlot, mode);
    if (file == -1) {
      result = file;
    } else {
      void *half = &gSaveBuffer;

      error = Save_ReadRecord(file, half);
      half = (char *)half + SAVE_RECORD_BYTES;
      error |= Save_ReadRecord(file + SAVE_FILE_COUNT, half);
      if (error != 0) {
        FieldMessage_Show(MsgLoadFailed, FIELD_MESSAGE_ZOOM);
        result = SAVE_ERROR_READ;
      } else {
        gPlayTimeFrames = gSaveBlock.playTime;
        gIdleSleepEnabled = gSaveBlock.idleSleepEnabled;
        gIdleFrames = 0;
        gSaveSlot = file;
      }
    }
  }
  Save_ReleaseWork();
  return result;
}

/// Loads the pointing hand cursor's tiles (gPointingHandTiles, 128
/// bytes) into a free VRAM slot and returns the slot, which the caller frees
/// with VramCache_FreeEntry. The slot is not checked: with the cache full
/// the load is given VRAM_CACHE_ENTRY_COUNT. Colosso's markers
/// (scene/common/colosso_marker.h) and, by far call, the world map's cursor
/// (overworld/event_commands/world_map.c) use it.
s32 Resource_LoadFixedBlockBIntoFreeSlot(void) {
  s32 slot;

  slot = VramCache_FindFreeEntry();
  VramCache_Load(slot, 0x80, gPointingHandTiles);
  return slot;
}
