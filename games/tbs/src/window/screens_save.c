/// @file
/// Save files and the file list.
// Part of screens.c.

#include "screens_internal.h"

/// A file's half in bytes: the first is written to the file's slot, the
/// second to the slot SAVE_FILE_COUNT on. Then the file number for no free
/// file.
#define SAVE_PART_BYTES 4096
#define SAVE_NO_FREE_FILE 999

// What the save functions return when they fail, after showing the message.
#define SAVE_ERROR_READ -2 ///< a part could not be read
/// Writing failed. "Failed to save." (or "Failed to erase.")
#define SAVE_ERROR_WRITE -3
#define SAVE_ERROR_ERASE -4   ///< erasing failed. "Failed to erase."
#define SAVE_ERROR_NO_FILE -5 ///< no empty file to copy into
/// No backup memory. "Unable to detect backup memory."
#define SAVE_ERROR_NO_MEMORY -9

/// What the file list is choosing a file for, which decides the files the
/// cursor may stop on.
enum FileMenuMode {
  FILE_LIST_SAVE,     ///< saving: any file, empty ones too
  FILE_LIST_CONTINUE, ///< continuing: files not yet cleared
  FILE_LIST_COPY,     ///< copying: any file with data
  FILE_LIST_ERASE,    ///< erasing: any file with data
  /// The title's Battle: files whose party has found a Djinni
  /// (SaveFileSummary.flagCount).
  FILE_LIST_BATTLE,
  FILE_LIST_CLEARED ///< the title's Send: cleared files only
};

/// FileList_SelectFile's result when no file can be chosen in its mode.
#define FILE_LIST_NO_FILE -2

/// The play time shown stops at PLAY_TIME_HOURS_MAX hours and 59 minutes.
#define PLAY_TIME_HOURS_MAX 999
#define MINUTES_PER_HOUR 60
#define SECONDS_PER_MINUTE 60

/// A two-digit field for String_FormatNumber: with 100 added, its last two
/// digits keep their leading zero.
#define WITH_LEADING_ZERO(n) ((n) + 100)

/// The words of gSceneLocalState (system/ewram.s), which gInventorySnapshot
/// follows.
#define SCENE_LOCAL_STATE_WORDS 30
/// The words from gGameFlagBytes that a file's checksum sums.
#define SAVE_CHECKSUM_WORDS 968

/// The file list's glow: the middle colour that HIGHLIGHT_PALETTE's colours 8
/// to 11 sway round, red twice as far as green and blue.
#define FILE_LIST_GLOW_RED 22
#define FILE_LIST_GLOW_GREEN 16
#define FILE_LIST_GLOW_BLUE 20

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

  totalMinutes = (u32)frames / (FRAMES_PER_SECOND * SECONDS_PER_MINUTE);
  if (totalMinutes >
      PLAY_TIME_HOURS_MAX * MINUTES_PER_HOUR + MINUTES_PER_HOUR - 1)
    totalMinutes =
        PLAY_TIME_HOURS_MAX * MINUTES_PER_HOUR + MINUTES_PER_HOUR - 1;

  hours = totalMinutes / MINUTES_PER_HOUR;
  minutes = totalMinutes % MINUTES_PER_HOUR;

  digitsCursor = String_FormatNumber(digits, hours, 3);
  *out = *digitsCursor;
  digitsCursor++;
  textCursor = out + 1;
  *textCursor = *digitsCursor;
  textCursor++;
  *textCursor = digitsCursor[1];
  minutes = WITH_LEADING_ZERO(minutes);
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

  summary = gSaveState->summary;
  for (file = 0; file < SAVE_FILE_COUNT; file++, summary++) {
    if (summary->level == 0)
      return file;
  }
  return SAVE_NO_FREE_FILE;
}

/// Returns how many save files there are, leaving out those whose summary has
/// its send flag set (the files saved at the end of the game) when
/// `excludeSent` is nonzero, or SAVE_ERROR_NO_MEMORY when the backup memory
/// cannot be read. It opens and releases the save state itself.
s32 Save_CountFilesExcludingSent(s32 excludeSent) {
  s32 file;
  s32 count;
  struct SaveFileSummary *summary;

  if (Save_InitializeState() != 0) {
    count = SAVE_ERROR_NO_MEMORY;
  } else {
    count = Save_LoadFileSummaries();
    if (excludeSent != 0) {
      summary = gSaveState->summary;
      for (file = 0; file < SAVE_FILE_COUNT; file++) {
        if ((s8)summary[file].sendFlag != 0)
          count--;
      }
    }
  }
  Save_ReleaseState();
  return count;
}

/// Loads the save file summaries for the File Screen and returns how many files
/// there are: plus SAVE_SCAN_ALL_CLEARED when every file is cleared (its send
/// flag set), or SAVE_ERROR_NO_MEMORY when the backup memory cannot be read.
s32 Save_ScanFileSummaries(void) {
  s32 error;
  s32 sentCount;
  s32 fileCount;

  error = Save_InitializeState();
  sentCount = 0;
  fileCount = SAVE_ERROR_NO_MEMORY;
  if (error == 0) {
    s32 i;
    struct SaveState *state;

    fileCount = Save_LoadFileSummaries();
    state = gSaveState;
    gTitleSendOptionEnabled = 0;
    gTitleBattleOptionEnabled = 0;
    for (i = 0; i < SAVE_FILE_COUNT; i++) {
      if (state->summary[i].sendFlag != 0) {
        gTitleSendOptionEnabled = 1;
        sentCount++;
      }
      if (state->summary[i].flagCount != 0) {
        gTitleBattleOptionEnabled = 1;
      }
    }

    if ((gKeysHeld & (KEY_R | KEY_LEFT)) != (KEY_R | KEY_LEFT)) {
      gTitleSendOptionEnabled = 0;
    }
  }
  Save_ReleaseState();
  if (fileCount != 0 && sentCount == fileCount) {
    return fileCount + SAVE_SCAN_ALL_CLEARED;
  }
  return fileCount;
}

/// Prepares the save buffer for writing: stamps the main routine's state with
/// the time and options, then fills the file summary shown on the file menus
/// (the leader's name, level and class, the play time, area, coins, Djinn of
/// each element, the party, the send flag and how many Djinn have been found)
/// and its checksum, the sum of SAVE_CHECKSUM_WORDS words from gGameFlagBytes
/// on.
u32 Save_BuildFileSummary(void) {
  u16 members[14];
  struct SaveFileSummary *summary;
  struct Unit *leader;
  u8 *source;
  u8 *destination;
  u32 *word;
  s32 i;
  u32 sum = 0;

  // The summary's header comes before the save buffer.
  summary = (struct SaveFileSummary *)((u32)gSaveBuffer -
                                       offsetof(struct SaveFileSummary, name));
  gMainRoutineState.buildStamp = BuildStamp_GetTimeFar(0);
  gMainRoutineState.playTime = gPlayTimeFrames;
  // A second copy of the play time in the file's second part, which
  // Save_LoadFileSummaries reads back into SaveFileSummary.secondPartPlayTime:
  // the file list lists a file whose two play times differ as corrupted.
  // UB: the copy is written through gSceneLocalState past its own bytes, into
  // gInventorySnapshot's last word; UBFIX writes it through gInventorySnapshot.
  {
#ifdef UBFIX
    u32 *copy = (u32 *)gInventorySnapshot;
    copy[SECOND_PART_PLAY_TIME_WORD - SCENE_LOCAL_STATE_WORDS] =
        gPlayTimeFrames;
#else
    u32 *copy = (u32 *)gSceneLocalState;
    copy[SECOND_PART_PLAY_TIME_WORD] = gPlayTimeFrames;
#endif
  }
  gMainRoutineState.idleSleepEnabled = gIdleSleepEnabled;

  leader = Unit_GetFar(gMainRoutineState.leaderActorId);
  destination = summary->name;
  source = leader->name;
  for (i = 0; i < (s32)sizeof(summary->name); i++)
    *destination++ = *source++;
  summary->level = leader->level;
  summary->playTime = gMainRoutineState.playTime;
  summary->area =
      Area_GetNameIndexFar(gMainRoutineState.next, gMainRoutineState.entrance);
  summary->classId = leader->classId;
  summary->coins = gMainRoutineState.holdings.coins;
  summary->djinn[ELEMENT_VENUS] = Party_SumDjinnCountsFar(ELEMENT_VENUS);
  summary->djinn[ELEMENT_MERCURY] = Party_SumDjinnCountsFar(ELEMENT_MERCURY);
  summary->djinn[ELEMENT_MARS] = Party_SumDjinnCountsFar(ELEMENT_MARS);
  summary->djinn[ELEMENT_JUPITER] = Party_SumDjinnCountsFar(ELEMENT_JUPITER);
  // Up to four party members, ended by -1.
  Party_ListMembersFar(members);
  for (i = 0; i <= 3 && members[i] != BATTLE_UNIT_LIST_END; i++)
    summary->party[i] = members[i];
  summary->party[i] = -1;
  summary->windowHue = gMainRoutineState.windowHue;
  summary->windowBrightness = gMainRoutineState.windowBrightness;
  summary->sendFlag = gMainRoutineState.sendFlag;
  summary->flagCount = 0;
  for (i = FLAG_DJINN_FIRST; i < FLAG_DJINNI(ELEMENT_COUNT * DJINN_PER_ELEMENT);
       i++) {
    if (GameFlag_GetFar(i))
      summary->flagCount++;
  }
  summary->childhood = GameFlag_GetFar(FLAG_CHILDHOOD) != 0;
  {
    s32 n;
    u32 frames;

    frames = gMainRoutineState.buildStamp;
    // The checksum's words, counted in fours and multiplied back below.
    n = SAVE_CHECKSUM_WORDS / 4;
    i = 0;
    n <<= 2;
    word = (u32 *)gGameFlagBytes;
    Io_Write32(frames, &summary->frames);
    // A while loop, entered at its test.
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
/// writes both halves of the save buffer, as parts gSaveSlot and gSaveSlot +
/// SAVE_FILE_COUNT.
s16 Save_WriteCurrentFile(void) {
  s16 file;
  s16 result;
  s32 error;

  result = 0;
  file = gSaveSlot;
  if (file != -1) {
    error = Save_InitializeState();
    if (error != 0) {
      FieldMessage_Show(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
      result = SAVE_ERROR_NO_MEMORY;
    } else {
      Save_BuildFileSummary();
      {
        void *half = &gSaveBuffer;
        s32 current;

        error = Save_WritePart(gSaveSlot, half);
        current = gSaveSlot;
        half = (char *)half + SAVE_PART_BYTES;
        error |= Save_WritePart(current + SAVE_FILE_COUNT, half);
        if (error != 0) {
          FieldMessage_Show(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
          result = SAVE_ERROR_WRITE;
        }
      }
    }
    Save_ReleaseState();
    file = result;
  }
  return file;
}

/// Writes both halves of the save buffer, as they are, to save file `file`.
/// Returns 0, or a SAVE_ERROR_* after showing its message.
s32 Save_WriteFile(s32 file) {
  s32 error;
  s16 result = 0;

  error = Save_InitializeState();
  if (error != 0) {
    FieldMessage_Show(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    void *half = &gSaveBuffer;

    error = Save_WritePart(file, half);
    half = (char *)half + SAVE_PART_BYTES;
    error |= Save_WritePart(file + SAVE_FILE_COUNT, half);
    if (error != 0) {
      FieldMessage_Show(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
      result = SAVE_ERROR_WRITE;
    }
  }
  Save_ReleaseState();
  return result;
}

/// Writes the 64-byte save stamp (gBattleArenaRecord, which lies in the first
/// half of the save buffer) into the current file's first part, leaving the
/// rest of the part as it was saved.
s32 Save_ProcessSelectedFile(void) {
  void *part;
  s32 result;
  s32 error;
  u8 *stamp;

  part = Heap_AllocEwram(SAVE_PART_BYTES);
  result = 0;
  // BUG: returns without freeing the part buffer.
  if (gSaveSlot == -1)
    return -1;
  error = Save_InitializeState();
  if (error != 0) {
    FieldMessage_Show(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    error = Save_ReadPart(gSaveSlot, part);
    // A failed read is reported, but the part is still written below from
    // whatever the buffer holds.
    if (error != 0) {
      FieldMessage_Show(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
      result = SAVE_ERROR_READ;
    }
    // The stamp's place in the part is its place in the save buffer.
    stamp = (u8 *)part + (s32)gBattleArenaRecord;
    stamp -= (s32)gSaveBuffer;
    Iwram_CopyWords(stamp, gBattleArenaRecord, 16);
    error = Save_WritePart(gSaveSlot, part);
    if (error != 0) {
      FieldMessage_Show(MsgSaveFailed, FIELD_MESSAGE_ZOOM);
      result = SAVE_ERROR_WRITE;
    }
  }
  Save_ReleaseState();
  Heap_Release(part);
  return result;
}

/// Asks whether to save over the current file and, on Yes, writes the save
/// stamp into it (Save_ProcessSelectedFile) and says so.
UNUSED s32 Save_ConfirmAndProcessSelectedFile(void) {
  s32 result;

  // UB: on No, result is returned without ever being set.
#ifdef UBFIX
  result = 0;
#endif

  Message_OpenWindow(MsgOverwriteConfirm, 8, 12, 2);
  while (Message_AllTyped() == 0) {
    wait(1);
  }
  if (YesNoMenu_Run(1, 0, 0, 1)) {
    Message_CloseFinishedWindows();
  } else {
    Message_CloseFinishedWindows();
    Sound_PlayFar(MUS_SAVED);
    result = Save_ProcessSelectedFile();
    if (result >= 0) {
      FieldMessage_Show(MsgGameSaved, FIELD_MESSAGE_ZOOM);
    }
  }
  return result;
}

/// The file list's Copy (scene/file_screen.c): lets the player choose a file,
/// reads both of its parts into the save buffer, overwriting it, and writes
/// them to the first empty file.
s32 Save_CopyFile(void) {
  u32 error;
  s32 file;
  s32 result;

  result = 0;
  error = Save_InitializeState();
  if (error != 0) {
    FieldMessage_Show(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    Save_LoadFileSummaries();
    file = FileList_SelectFile(0, FILE_LIST_COPY);
    if (file == -1) {
      result = file;
    } else {
      void *lower = &gSaveBuffer;
      void *upper;

      error = Save_ReadPart(file, lower);
      upper = (u8 *)lower + SAVE_PART_BYTES;
      error |= Save_ReadPart(file + SAVE_FILE_COUNT, upper);
      if (error != 0) {
        FieldMessage_Show(MsgLoadFailed, FIELD_MESSAGE_ZOOM);
        result = SAVE_ERROR_READ;
      } else {
        file = Save_FindFreeFile();
        if (file == SAVE_NO_FREE_FILE) {
          FieldMessage_Show(MsgEraseFailed, FIELD_MESSAGE_ZOOM);
          result = SAVE_ERROR_NO_FILE;
        } else {
          error = Save_WritePart(file, lower);
          error |= Save_WritePart(file + SAVE_FILE_COUNT, upper);
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

  Save_ReleaseState();
  return result;
}

/// The file list's Erase (scene/file_screen.c): lets the player choose a file,
/// asks whether to erase it and on Yes deletes both of its parts.
s32 Save_EraseSelectedFile(void) {
  s32 error;
  s32 file;
  s32 result = 0;

  error = Save_InitializeState();
  if (error != 0) {
    FieldMessage_Show(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    Save_LoadFileSummaries();
    file = FileList_SelectFile(0, FILE_LIST_ERASE);
    if (file == -1) {
      result = file;
    } else {
      Message_OpenWindow(MsgEraseConfirm, 8, 1, 2);
      while (Message_AllTyped() == 0) {
        wait(1);
      }
      if (YesNoMenu_Run(1, 0, 3, 1) != 0) {
        Message_CloseFinishedWindows();
      } else {
        Message_CloseFinishedWindows();
        error = Save_DeletePart(file);
        error |= Save_DeletePart(file + SAVE_FILE_COUNT);
        if (error != 0) {
          FieldMessage_Show(MsgEraseFailed, FIELD_MESSAGE_ZOOM);
          result = SAVE_ERROR_ERASE;
        } else {
          FieldMessage_Show(MsgFileErased, FIELD_MESSAGE_ZOOM);
        }
      }
    }
  }
  Save_ReleaseState();
  return result;
}

#if TBS_PATCH_1

/// A task that makes colours 8-11 of HIGHLIGHT_PALETTE glow: each follows a
/// sine wave of the frame counter, eight frames behind the one before.
void FileList_UpdatePulse(void) {
  s32 *vblankCount;
  volatile u16 *palette;
  s32 colour;
  FIXED sine;
  s32 i;

  vblankCount = (s32 *)&gVBlankCount;
  palette = &BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 8);
  i = 0;
  do {
#ifdef UBFIX
    // Wrap the frame phase before Trig_Sin takes its low sixteen bits.
    sine = Trig_Sin((((u32)*vblankCount + i * 8U) * (TRIG_TURN * 3 / 256U)) &
                    TRIG_ANGLE_MASK);
#else
    sine = Trig_Sin((*vblankCount + i * 8) * (TRIG_TURN * 3 / 256));
#endif
    colour = sine / (FIXED_ONE / 4);
    {
#ifdef UBFIX
      // A negative sine must be shifted as unsigned bits.
      u32 red = (u32)colour << 1;
#else
      u32 red = colour << 1;
#endif
      u32 green = colour;

      red += FILE_LIST_GLOW_RED;
      green += FILE_LIST_GLOW_GREEN;
      colour += FILE_LIST_GLOW_BLUE;
      Io_Write16(RGB(red, green, colour), palette++);
    }
  } while (++i <= 3);
}

#endif

#if TBS_PATCH_1

/// Starts the palette glow (FileList_UpdatePulse) as a task.
void FileList_StartPulse(void) {
  Task_Add(FileList_UpdatePulse, TASK_PASS_FRAME);
}

/// Stops the file list's glow (FileList_UpdatePulse).
void FileList_StopPulse(void) { Task_Remove(FileList_UpdatePulse); }

/// Fills the `width` x `height` tile rectangle at (x, y) inside `window`,
/// clipped to the screen as Window_SetInteriorPaletteBit clips, with the tile
/// entry of tile 6 in HIGHLIGHT_PALETTE, the palette FileList_UpdatePulse
/// animates.
void Window_FillTilemapRect(struct Window *window, s32 x, s32 y, s32 width,
                            s32 height) {
  u16 *tilemap = (u16 *)gWindowState[0];
  s32 sum;

  sum = x + window->x;
  x = sum + 1;
  sum = y + (window)->y;
  y = sum + 1;

  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > DISPLAY_TILE_WIDTH - 1)
    width = DISPLAY_TILE_WIDTH - x;

  if (y < 0) {
    height += y;
    y = 0;
  }
  // BUG: the bottom is tested against the screen's width, so a rectangle
  // ending below row DISPLAY_TILE_HEIGHT writes past WindowState.tilemap; the
  // one caller, the file list's highlight, never does.
#ifdef BUGFIX
  if (y + height > DISPLAY_TILE_HEIGHT - 1)
#else
  if (y + height > DISPLAY_TILE_WIDTH - 1)
#endif
    height = DISPLAY_TILE_HEIGHT - y;

  if (width > 0 && height > 0) {
    // Byte offsets: BG_SCREEN_WIDTH entries of 2 bytes a row.
    s32 yOffset = y * (BG_SCREEN_WIDTH * 2);
    s32 xOffset = x << 1;
    s32 offset = yOffset + xOffset;
    do {
      s32 remaining = width;
      u16 *cell = (u16 *)(offset + (s32)tilemap);
      for (; remaining != 0; remaining--)
        *cell++ = BG_TILE_PALETTE(HIGHLIGHT_PALETTE) | 6;
      height--;
      offset += BG_SCREEN_WIDTH * 2;
    } while (height != 0);
    ((struct WindowState *)tilemap)->dirty = 1;
  }
}

#endif

/// The file list shows the chosen file's party as standing sprites in a row,
/// and four Djinn sprites, one for each element, over the file's Djinn counts.
#define FILE_LIST_SPRITE_SPACING 3

/// Shows the party of save file `summary` (up to four party members) as sprites
/// in a row from tile (x, y) inside `window`, and starts placing them every
/// frame (FileList_PlacePartySprites).
void FileList_ShowPartySprites(struct Window *window, s32 x, s32 y,
                               struct SaveFileSummary *summary) {
  struct ShopState *menu = gMenuState;
  s32 i;

  for (i = 0; i < 4 && summary->party[i] != -1; i++) {
    struct Sprite *sprite =
        Sprite_CreateFar(Party_GetCharacterSpriteForChildhoodFar(
            summary->party[i], (s8)summary->childhood));

    if (sprite != NULL) {
      Sprite_StartAnimationFar(sprite, ANIM_STAND);
      sprite->flags = 0;
      sprite->image.priority = 0;
    }

    menu->partyMemberIcons[i] = sprite;
    menu->partyMemberX[i] =
        (x + window->x + i * FILE_LIST_SPRITE_SPACING) * 8 + 16;
    menu->partyMemberY[i] = (y + window->y) * 8 + 16;
    menu->partyMemberScale[i] = FIXED_ONE;
  }

  Task_Add(FileList_PlacePartySprites, TASK_PASS_FRAME);
}

/// Stops placing the party sprites of the file list and releases them.
void FileList_HidePartySprites(void) {
  struct ShopState *menu = gMenuState;
  s32 i;

  Task_Remove(FileList_PlacePartySprites);
  for (i = 0; i < 4; i++) {
    if (menu->partyMemberIcons[i] != NULL) {
      Sprite_DestroyFar(menu->partyMemberIcons[i]);
      menu->partyMemberIcons[i] = NULL;
    }
  }
}

/// A task: draws each party sprite of the file list at its
/// place and scale for this frame.
void FileList_PlacePartySprites(void) {
  struct ShopState *menu = gMenuState;
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
      position.x = toFIXED(menu->partyMemberX[index]);
      position.height = toFIXED(500);
      position.y = toFIXED(menu->partyMemberY[index]) + toFIXED(500);
      position.groundHeight = 0;
      Render_ApplyProjectedPlacementFar(sprite, &position, &scale,
                                        FACING_SOUTH);
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
  struct ItemMenuState *menu = gMenuState;
  s32 i;

  if (window != NULL) {
    for (i = 0; i < 4; i++) {
      struct Sprite *sprite = Sprite_CreateFar(gFileList_DjinnSpriteIds[i]);

      if (sprite != NULL) {
        Sprite_StartAnimationFar(sprite, ANIM_WALK);
        sprite->flags = 0;
        sprite->image.priority = 0;
      }

      menu->figures[i] = sprite;
      menu->figureX[i] =
          (window->x + x + i * FILE_LIST_SPRITE_SPACING) * 8 + 16;
      menu->figureY[i] = (window->y + y) * 8 + 16;
    }

    Task_Add(FileList_PlaceDjinnSprites, TASK_PASS_FRAME);
  }
}

/// Stops placing the Djinn sprites of the file list and releases them.
void FileList_HideDjinnSprites(void) {
  struct ItemMenuState *menu = gMenuState;
  s32 i;

  Task_Remove(FileList_PlaceDjinnSprites);
  for (i = 0; i < 4; i++) {
    if (menu->figures[i] != NULL) {
      Sprite_DestroyFar(menu->figures[i]);
      menu->figures[i] = NULL;
    }
  }
}

/// A task: draws each Djinn sprite of the file list at its
/// place, at scale 1.0, for this frame.
void FileList_PlaceDjinnSprites(void) {
  struct ItemMenuState *menu = gMenuState;
  struct SpriteScale scale;
  struct SpritePlacement position;
  s32 index;

  for (index = 0; index <= 3; index++) {
    struct Sprite *sprite = menu->figures[index];

    if (sprite != NULL) {
      FIXED fullSize = FIXED_ONE;
      FIXED height = toFIXED(500);

      scale.x = fullSize;
      scale.y = fullSize;
      position.x = toFIXED(menu->figureX[index]);
      position.height = height;
      position.y = toFIXED(menu->figureY[index]) + height;
      position.groundHeight = 0;
      Render_ApplyProjectedPlacementFar(sprite, &position, &scale,
                                        FACING_SOUTH);
    }
  }
}

/// Draws the Djinn counts of save file `summary`, one for each element, two
/// digits each, 24 pixels apart on row 16 of `window`, under the Djinn sprites.
void FileList_DrawDjinnCounts(struct Window *window,
                              struct SaveFileSummary *summary) {
  s32 i;

  if (window != NULL) {
    for (i = 0; i < 4; i++)
      Text_DrawNumberAtOffset((s8)summary->djinn[i], 2, window,
                              i * FILE_LIST_SPRITE_SPACING * 8, 16);
  }
}

/// Where each edition draws the "Lv" label and the coin label, in pixels.
#if LANGUAGE == LANGUAGE_SPANISH
#define SUMMARY_LEVEL_X 64
#define SUMMARY_COINS_X 56
#define SUMMARY_DRAW_LEVEL Text_DrawStringAtOffset
#elif LANGUAGE == LANGUAGE_FRENCH
#define SUMMARY_LEVEL_X 59
#define SUMMARY_COINS_X 56
#define SUMMARY_DRAW_LEVEL Text_DrawStringAtOffset
#else
#define SUMMARY_LEVEL_X 72
#define SUMMARY_COINS_X 48
#define SUMMARY_DRAW_LEVEL Text_DrawStringInWindow
#endif

/// Draws the summary of save file `summary` into `window`: the leader's name,
/// level and class, the play time and the coins.
void FileList_DrawSummary(struct Window *window,
                          struct SaveFileSummary *summary) {
  u32 playTime[4];
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
#if LANGUAGE == LANGUAGE_FRENCH
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

/// The file list: lists the three files with each one's leader and area (or why
/// it cannot be chosen) and, for the file under the cursor, its summary, its
/// party and its Djinn, with the file's own palette glow.
s32 FileList_SelectFile(s32 selectedFile, s32 menuMode) {
  struct Window *fileList;
#if TBS_PATCH_1
  struct SaveState *save;
  struct Window *summaryWindow;
  struct Window *partyWindow;
  struct Window *djinnWindow;
  struct SaveFileSummary *summary;
  struct WindowSprite *modePicture;
  s32 summaryDirty;
  struct WindowState *state;
  u32 buildStamp;
  struct ShopState *spriteState;
  s32 result;
  s32 i;
  s32 y;
#else
  struct Window *summaryWindow;
  struct SaveState *save;
  struct Window *partyWindow;
  struct Window *djinnWindow;
  struct SaveFileSummary *summary;
  struct WindowSprite *modePicture;
  s32 cursorStyle;
  s32 summaryDirty;
  struct WindowState *state;
  u32 buildStamp;
  struct ShopState *spriteState;
  struct MenuCursor fileCursor;
  s32 result;
  s32 i;
  s32 x;
  s32 y;
  s32 vramSlot;
  struct WindowSprite *cursorSprite;
#endif

  spriteState = HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(*spriteState));
  save = gSaveState;
  summaryWindow = NULL;
  partyWindow = NULL;
  djinnWindow = NULL;
  summaryDirty = 1;
  state = WINDOW_STATE;
  buildStamp = BuildStamp_GetTimeFar();
  // Start on the first file the mode allows.
  // BUG: when none is, the jump to done returns FILE_LIST_NO_FILE without
  // releasing spriteState's HEAP_SLOT_MENU block.
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
  } else if (menuMode == FILE_LIST_BATTLE) {
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
  Dma_Fill32(0, spriteState, sizeof(*spriteState));
  FileList_StartPulse();
  fileList = Window_Create(1, 2, 28, 7, WINDOW_FLAG_INSTANT);
  summary = save->summary;
  y = 0;
  for (i = 0; i < SAVE_FILE_COUNT; i++, y += 16, summary++) {
    if (summary->level == 0) {
      Text_DrawMessage(MsgSaveNoData, fileList, 10, y);
    } else if (summary->frames < buildStamp) {
      // Saved by an older build: "(Continue from a Sanctum)", since such a
      // file resumes at MainRoutineState.restartScene, not where it was saved
      // (scene/file_screen.c).
      Text_DrawMessage(MsgSaveContinue, fileList, 10, y);
    } else if (summary->playTime != summary->secondPartPlayTime) {
      Text_DrawMessage(MsgSaveCorrupted, fileList, 10, y);
    } else if (menuMode == FILE_LIST_CLEARED && (s8)summary->sendFlag == 0) {
      Text_DrawMessage(MsgSaveNotCleared, fileList, 10, y);
    } else {
      Text_DrawString(summary->name, fileList, FILE_LIST_NAME_X, y);
      Text_DrawMessage(summary->area + MsgDebugAreaName, fileList, 62, y);
      state->dirty = 1;
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
  if (menuMode == FILE_LIST_BATTLE) {
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
  Dma_Fill32(0, spriteState, sizeof(*spriteState));
  fileList = Window_Create(1, 2, 28, 7, WINDOW_FLAG_INSTANT);

  for (i = 0, summary = save->summary; i < SAVE_FILE_COUNT; i++, summary++) {
    if (save->summary[i].level == 0) {
      Text_DrawMessage(MsgSaveNoData, fileList, 10, i * 16);
    } else if (save->summary[i].frames < buildStamp) {
      // Saved by an older build: see the other editions' list above.
      Text_DrawMessage(MsgSaveContinue, fileList, 10, i * 16);
    } else if (save->summary[i].playTime !=
               save->summary[i].secondPartPlayTime) {
      Text_DrawMessage(MsgSaveCorrupted, fileList, 10, i * 16);
    } else if (menuMode == FILE_LIST_CLEARED &&
               (s8)save->summary[i].sendFlag == 0) {
      Text_DrawMessage(MsgSaveNotCleared, fileList, 10, i * 16);
    } else {
      y = i * 16;
      Text_DrawString(summary->name, fileList, FILE_LIST_NAME_X, y);
      Text_DrawMessage(save->summary[i].area + MsgDebugAreaName, fileList, 72,
                       y);
      state->dirty = 1;
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
    cursorSprite->motion = WINDOW_SPRITE_MOTION_STILL;
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
        wait(1);
        if (partyWindow == NULL)
          partyWindow = Window_Create(16, 10, 13, 3, WINDOW_FLAG_INSTANT);
        FileList_HidePartySprites();
        FileList_ShowPartySprites(partyWindow, 0, 0,
                                  &save->summary[selectedFile]);
        wait(1);
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
        WindowColor_Apply(gMainRoutineState.windowHue,
                          gMainRoutineState.windowBrightness);
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
    wait(1);
    // Up and down move to the previous or next file the mode allows; saving
    // allows every file, an empty one too.
    if (gKeysRepeat & KEY_UP) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      summaryDirty = 1;
      do {
        selectedFile = (selectedFile + SAVE_FILE_COUNT - 1) % SAVE_FILE_COUNT;
        if (menuMode == FILE_LIST_SAVE)
          break;
      } while ((save->summary[selectedFile].level == 0 ||
                (menuMode == FILE_LIST_CONTINUE &&
                 (s8)save->summary[selectedFile].sendFlag != 0) ||
                (menuMode == FILE_LIST_BATTLE &&
                 (s8)save->summary[selectedFile].flagCount == 0) ||
                (menuMode == FILE_LIST_CLEARED &&
                 (s8)save->summary[selectedFile].sendFlag == 0)));
    } else if (gKeysRepeat & KEY_DOWN) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      summaryDirty = 1;
      do {
        selectedFile = (selectedFile + SAVE_FILE_COUNT + 1) % SAVE_FILE_COUNT;
        if (menuMode == FILE_LIST_SAVE)
          break;
      } while ((save->summary[selectedFile].level == 0 ||
                (menuMode == FILE_LIST_CONTINUE &&
                 (s8)save->summary[selectedFile].sendFlag != 0) ||
                (menuMode == FILE_LIST_BATTLE &&
                 (s8)save->summary[selectedFile].flagCount == 0) ||
                (menuMode == FILE_LIST_CLEARED &&
                 (s8)save->summary[selectedFile].sendFlag == 0)));
    } else if (gKeysPressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    } else if (gKeysPressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
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
#endif
  HeapSlot_Release(HEAP_SLOT_MENU);
  WindowColor_Apply(gMainRoutineState.windowHue,
                    gMainRoutineState.windowBrightness);
  wait(1);
done:
  return result;
}

/// The Save command: lets the player choose a file (the current one first),
/// asks whether to save over it when it has data, then saves the game to it,
/// gActors included, and makes it the current file.
s32 Save_ToChosenFile(void) {
  s32 result;
  s32 file;
  s32 error;
  struct SaveState *state;

  result = 0;
  error = Save_InitializeState();
  if (error != 0) {
    FieldMessage_Show(MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    Save_LoadFileSummaries();
    state = gSaveState;
    file = FileList_SelectFile(gSaveSlot, FILE_LIST_SAVE);
    if (file == -1) {
      result = file;
    } else {
      if (state->summary[file].level != 0) {
        FieldMessage_Show(MsgOverwriteConfirm, FIELD_MESSAGE_ZOOM |
                                                   FIELD_MESSAGE_KEEP_OPEN |
                                                   FIELD_MESSAGE_LOWER);
        while (Message_AllTyped() == 0) {
          wait(1);
        }
        if (YesNoMenu_Run(1, 0, 0, 1) != 0) {
          Message_CloseFinishedWindows();
          goto done;
        }
        Message_CloseFinishedWindows();
      }
      gSaveSlot = file;
      Sound_PlayFar(MUS_SAVED);
      FieldMessage_Show(MsgSaving, FIELD_MESSAGE_ZOOM |
                                       FIELD_MESSAGE_KEEP_OPEN |
                                       FIELD_MESSAGE_LOWER);
      while (Message_AllTyped() == 0) {
        wait(1);
      }
      Save_BuildFileSummary();
      Actor_SnapshotFar();
      error = Save_WritePart(file, gSaveBuffer);
      error |=
          Save_WritePart(file + SAVE_FILE_COUNT, gSaveBuffer + SAVE_PART_BYTES);
      Message_CloseFinishedWindows();
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
  Save_ReleaseState();
  return result;
}

/// Lets the player choose a file in file list `mode` (see FileMenuMode) and
/// loads it into the save buffer as the current file, restoring its play time
/// and options.
s32 Save_ChooseAndLoadFile(s32 mode) {
  s32 result = 0;
  s32 error = Save_InitializeState();

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

      error = Save_ReadPart(file, half);
      half = (char *)half + SAVE_PART_BYTES;
      error |= Save_ReadPart(file + SAVE_FILE_COUNT, half);
      if (error != 0) {
        FieldMessage_Show(MsgLoadFailed, FIELD_MESSAGE_ZOOM);
        result = SAVE_ERROR_READ;
      } else {
        gPlayTimeFrames = gMainRoutineState.playTime;
        gIdleSleepEnabled = gMainRoutineState.idleSleepEnabled;
        gIdleFrames = 0;
        gSaveSlot = file;
      }
    }
  }
  Save_ReleaseState();
  return result;
}

/// Loads the pointing hand cursor's tiles (gPointingHandTiles, 128 bytes) into
/// a free VRAM slot and returns the slot, which the caller frees with
/// VramCache_FreeEntry.
s32 Resource_LoadFixedBlockBIntoFreeSlot(void) {
  s32 slot;

  slot = VramCache_FindFreeEntry();
  VramCache_Load(slot, 128, gPointingHandTiles);
  return slot;
}
