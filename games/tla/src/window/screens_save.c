/// @file
/// Save files, the file list and the saving screen's progress bar.
// Part of screens.c.

#include "window_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "system/io_write.h"
#include "gamepak/far_call/game.h"
#include "system/system.h"
#include "system/save.h"
#include "window/screens.h"

/// What Save_FindFreeFile returns when all SAVE_FILE_COUNT files are in use.
#define SAVE_NO_FREE_FILE 999

// What the save functions return when they fail, after showing the message.
#define SAVE_ERROR_READ -2 ///< a part could not be read
/// Writing failed. "Failed to save." (or "Failed to erase.")
#define SAVE_ERROR_WRITE -3
#define SAVE_ERROR_ERASE -4   ///< erasing failed. "Failed to erase."
#define SAVE_ERROR_NO_FILE -5 ///< no empty file to copy into
/// No backup memory. "Unable to detect backup memory."
#define SAVE_ERROR_NO_MEMORY -9

/// The play time shown stops at PLAY_TIME_HOURS_MAX hours and 59 minutes.
#define PLAY_TIME_HOURS_MAX 999
#define MINUTES_PER_HOUR 60
#define SECONDS_PER_MINUTE 60

/// A two-digit field for String_FormatNumber: with 100 added, its last two
/// digits keep their leading zero.
#define WITH_LEADING_ZERO(n) ((n) + 100)

/// The words from gGameFlagBytes that a file's checksum sums.
#define SAVE_CHECKSUM_WORDS 968

/// The file list's glow: the middle colour that HIGHLIGHT_PALETTE's colours 8
/// to 11 sway round, red twice as far as green and blue.
#define FILE_LIST_GLOW_RED 22
#define FILE_LIST_GLOW_GREEN 16
#define FILE_LIST_GLOW_BLUE 20

/// The tiles between two of the file list's party sprites, and between two of
/// its Djinn counts, one for each element.
#define FILE_LIST_SPRITE_SPACING 3
/// How high the file list lifts a party sprite, in pixels, and how far it
/// brings it forward: the two cancel on screen, and its depth runs past the
/// field's last slot, so it is drawn in front of every field sprite and behind
/// the windows' (OAM_DEPTH_WINDOW).
#define FILE_LIST_SPRITE_LIFT 500

#include "window/window.h"
#include "gamepak/far_call/field.h"

/// The progress bar: twelve tiles wide at tile column 9, row 9 of the window
/// map, drawn from tiles 128 on (SaveProgress_Open loads them), in seven
/// stages.
#define PROGRESS_BAR_X 9
#define PROGRESS_BAR_Y 9
#define PROGRESS_BAR_WIDTH 12
#define PROGRESS_BAR_LAST_ROW 6
#define PROGRESS_BAR_FIRST_TILE 128

/// The music fades out under the progress screen and back after it by this
/// much of BGM_UNITY_SCALE a VBlank.
#define PROGRESS_MUSIC_FADE_STEP 30

/// The progress bar's serial callback: advances the stored progress by step
/// and draws row min(progress, 6) of gWindow_ProgressBarRows.
void SaveProgress_Advance(s32 step) {
  struct SaveWorkspace *save = Ram_HeapSlots->saveState;
  u32 row;
  const u8 *src;
  u16 *dst;
  s32 i;

  // A three-sector write moves the bar twice as far a sector.
  if ((s16)save->sectorTotal <= 3)
    step <<= 1;
  row = (s16)(save->sectorCount += step);
  if (row > PROGRESS_BAR_LAST_ROW)
    row = PROGRESS_BAR_LAST_ROW;
  dst = (u16 *)BG_SCREEN_BLOCK(4) + PROGRESS_BAR_Y * BG_SCREEN_WIDTH +
        PROGRESS_BAR_X;
  src = gWindow_ProgressBarRows + row * PROGRESS_BAR_WIDTH;
  for (i = 0; i < PROGRESS_BAR_WIDTH; i++)
    *dst++ = *src++ + WINDOW_TILE(PROGRESS_BAR_FIRST_TILE);
}

/// Heading x coordinate: only French differs.
#if (LANGUAGE == LANGUAGE_FRENCH)
#define SAVING_X 40
#define ERASING_X 48
#define COPYING_X 48
#else
#define SAVING_X 76
#define ERASING_X 92
#define COPYING_X 84
#endif

/// What the progress screen says it is doing.
enum SaveProgressKind {
  SAVE_PROGRESS_SAVING = 0,  ///< "Saving..." (MsgSaving)
  SAVE_PROGRESS_ERASING = 1, ///< "Erasing..." (MsgErasing)
  SAVE_PROGRESS_COPYING = 2  ///< "Copying..." (MsgCopying)
};

/// Opens the progress screen for a SaveProgressKind.
struct Window *SaveProgress_Open(u32 kind) {
  struct Window *window;
  void *tiles;
  s32 message;

  window = Window_Create(0, 0, 30, 20, 0);
  tiles = Heap_AllocEwram(256);
  Decompress_TaggedLz(gWindow_ProgressBarTiles, tiles);
  Dma_Set(tiles, (void *)BG_SCREEN_BLOCK(2), DMA_COPY_WORDS(64), REG_DMA3);
  switch (kind) {
  case SAVE_PROGRESS_SAVING:
    Text_DrawMessage((s32)&MsgSaving, (s32)window, SAVING_X, 40);
    break;
  case SAVE_PROGRESS_ERASING:
    Text_DrawMessage((s32)&MsgErasing, (s32)window, ERASING_X, 40);
    break;
  case SAVE_PROGRESS_COPYING:
    Text_DrawMessage((s32)&MsgCopying, (s32)window, COPYING_X, 40);
    break;
  }
  message = (s32)&MsgDoNotTurnThePower;
  Text_DrawMessage(message, (s32)window, 40, 88);
  Text_DrawMessage(message + 1, (s32)window, 40, 104);
  Sound_SetBgmVolumeTargetFar(0, PROGRESS_MUSIC_FADE_STEP);
  Sound_PlayFar(SE_STOP_EXIT_SE);
  wait(10);
  Heap_Release(tiles);
  return window;
}

/// Closes the progress screen.
struct Window *SaveProgress_Close(struct Window *window) {
  wait(10);
  Scene_PlayQueuedMusicFar();
  Sound_SetBgmVolumeTargetFar(BGM_UNITY_SCALE, PROGRESS_MUSIC_FADE_STEP);
  Window_Close(window, 0);
  return window;
}

/// Stops DMA channel n.
#define DMA_STOP(n)                                                            \
  do {                                                                         \
    volatile u16 *dma = REG_DMA0 + (n) * 6;                                    \
    dma[5] &= (u16)~DMA_START_TIMING_DRQ_REPEAT;                               \
    dma[5] &= (u16)~DMA_ENABLE;                                                \
    dma[5];                                                                    \
  } while (0)

/// Runs Save_WritePart with interrupts masked and all four DMA channels
/// stopped, setting SaveProgress_Advance as the save's progress callback around
/// it, then restores IE and IME. Save_DeletePartMasked does the same around
/// Save_DeletePart.
s32 Save_WritePartMasked(s32 partId, void *buffer) {
  volatile u32 ime;
  volatile u32 ie;
  s32 result;

  IRQ_LOCK(ime);
  ie = REG_IE;
  Io_Write16(0, &REG_IE);
  DMA_STOP(0);
  DMA_STOP(1);
  DMA_STOP(2);
  DMA_STOP(3);
  IRQ_UNLOCK(ime);
  Save_SetProgressCallback(SaveProgress_Advance);
  result = Save_WritePart(partId, buffer);
  Save_SetProgressCallback(0);
  IRQ_LOCK(ime);
  REG_IE = ie;
  IRQ_UNLOCK(ime);
  return result;
}

s32 Save_DeletePart(s32 partId);

/// Deletes a part of a save file (Save_DeletePart) with interrupts masked and
/// all four DMA channels stopped, setting SaveProgress_Advance as the save's
/// progress callback around it, as Save_WritePartMasked does around
/// Save_WritePart.
s32 Save_DeletePartMasked(s32 partId) {
  volatile u32 ime;
  volatile u32 ie;
  s32 result;

  IRQ_LOCK(ime);
  ie = REG_IE;
  Io_Write16(0, &REG_IE);
  DMA_STOP(0);
  DMA_STOP(1);
  DMA_STOP(2);
  DMA_STOP(3);
  IRQ_UNLOCK(ime);
  Save_SetProgressCallback(SaveProgress_Advance);
  result = Save_DeletePart(partId);
  Save_SetProgressCallback(0);
  IRQ_LOCK(ime);
  REG_IE = ie;
  IRQ_UNLOCK(ime);
  return result;
}

/// Unused: writes frames as "HHH:MM:SS:CC", hours to hundredths, into out and
/// returns out. Its far call String_FormatStopwatchFar is exported, but no
/// overlay imports it and nothing calls it.
u8 *String_FormatStopwatch(u32 frames, u8 *out) {
  u8 digits[16];
  u8 *digitsCursor;
  u8 *textCursor;
  u32 fraction;
  u32 seconds;
  u32 minutes;
  u32 hours;
  u32 hundredths;

  fraction = frames % FRAMES_PER_SECOND;
  frames /= FRAMES_PER_SECOND;
  hundredths = fraction * 100 / FRAMES_PER_SECOND;
  seconds = frames % SECONDS_PER_MINUTE;
  frames /= SECONDS_PER_MINUTE;
  minutes = frames % MINUTES_PER_HOUR;
  hours = frames / MINUTES_PER_HOUR;

  digitsCursor = String_FormatNumber(digits, hours, 3);
  *out = *digitsCursor;
  digitsCursor++;
  textCursor = out + 1;
  *textCursor = *digitsCursor;
  textCursor++;
  *textCursor = digitsCursor[1];
  textCursor++;
  *textCursor = ':';

  digitsCursor = String_FormatNumber(digits, WITH_LEADING_ZERO(minutes), 2);
  textCursor++;
  *textCursor = digitsCursor[0];
  textCursor++;
  *textCursor = digitsCursor[1];
  textCursor++;
  *textCursor = ':';

  digitsCursor = String_FormatNumber(digits, WITH_LEADING_ZERO(seconds), 2);
  textCursor++;
  *textCursor = digitsCursor[0];
  textCursor++;
  *textCursor = digitsCursor[1];
  textCursor++;
  *textCursor = ':';

  digitsCursor = String_FormatNumber(digits, WITH_LEADING_ZERO(hundredths), 2);
  textCursor++;
  *textCursor = digitsCursor[0];
  textCursor++;
  *textCursor = digitsCursor[1];
  textCursor[1] = 0;

  return out;
}

/// Writes the play time `frames` into `out` as hours and minutes, "hhh:mm"
/// with the hours right-aligned in three characters, and returns `out`. The
/// time stops at 999:59.
void *String_FormatPlayTime(s32 frames, u8 *out) {
  u8 digits[16];
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
  struct SaveWorkspace *state = Ram_HeapSlots->saveState;
  struct SaveSummary *summary;
  u32 i;

  summary = state->summary;
  for (i = 0; i < SAVE_FILE_COUNT; i++, summary++) {
    if (summary->level == 0)
      return i;
  }
  return SAVE_NO_FREE_FILE;
}

/// Returns how many save files there are, leaving out those whose summary has
/// its send flag set (the files saved at the end of the game) when
/// `excludeSent` is nonzero, or SAVE_ERROR_NO_MEMORY when the backup memory
/// cannot be read. It opens and releases the save workspace itself.
s32 Save_CountFilesExcludingSent(s32 excludeSent) {
  s32 i;
  s32 count;
  struct SaveSummary *summary;
  struct SaveWorkspace *state;

  if (Save_InitializeState() != 0) {
    count = SAVE_ERROR_NO_MEMORY;
  } else {
    count = Save_LoadFileSummaries();
    if (excludeSent != 0) {
      state = Ram_HeapSlots->saveState;
      summary = state->summary;
      for (i = 0; i < SAVE_FILE_COUNT; i++) {
        if (summary[i].sendFlag != 0)
          count--;
      }
    }
  }
  Save_ReleaseState();
  return count;
}

/// Loads the save file summaries for the File Screen and returns how many files
/// there are: plus SAVE_SCAN_ALL_CLEARED when every file is cleared (its send
/// flag set), or SAVE_ERROR_NO_MEMORY when the backup memory cannot be
/// read. Sets the title's options from them: Battle when a file has found a
/// Djinni, Update when a file has a level and FLAG_REUNION clear.
s32 Save_ScanFileSummaries(void) {
  s32 error;
  s32 sentCount;
  s32 fileCount;

  error = Save_InitializeState();
  sentCount = 0;
  fileCount = SAVE_ERROR_NO_MEMORY;
  if (error == 0) {
    s32 i;
    struct SaveWorkspace *state;

    fileCount = Save_LoadFileSummaries();
    state = Ram_HeapSlots->saveState;
    gAnyFileCleared = 0;
    gTitleBattleOptionEnabled = 0;
    gTitleUpdateOptionEnabled = 0;
    for (i = 0; i < SAVE_FILE_COUNT; i++) {
      if (state->summary[i].sendFlag != 0) {
        gAnyFileCleared = 1;
        sentCount++;
      }
      if (state->summary[i].djinnCount != 0)
        gTitleBattleOptionEnabled = 1;
      if (state->summary[i].level != 0 && state->summary[i].reunited == 0)
        gTitleUpdateOptionEnabled = 1;
    }
  }
  Save_ReleaseState();
  if (fileCount != 0 && sentCount == fileCount)
    return fileCount + SAVE_SCAN_ALL_CLEARED;
  return fileCount;
}

/// Prepares the save buffer for writing: stamps the main routine's state with
/// the time and options, then fills the file summary shown on the file menus
/// (the leader's name, level and class, the play time, area, coins, Djinn of
/// each element, the party, the send flag and how many Djinn it has found) and
/// its checksum, the sum of the 968 words from gGameFlagBytes on.
s32 Save_BuildFileSummary(void) {
  u16 members[14];
  struct SaveSummary *summary;
  struct Unit *leader;
  u8 *source;
  u8 *destination;
  u32 *word;
  s32 i;
  u32 sum = 0;

  // The summary's header comes before the save buffer.
  summary =
      (struct SaveSummary *)(gSaveBuffer - offsetof(struct SaveSummary, name));
  gMainRoutineState.buildStamp = BuildStamp_GetTimeFar(0);
  gMainRoutineState.playTime = gPlayTimeFrames;
  // A second copy of the play time, at the start of gSceneLocalState.
  *(u32 *)gSceneLocalState = gPlayTimeFrames;
  gMainRoutineState.idleSleepEnabled = gIdleSleepEnabled;
  leader = Unit_Get(gMainRoutineState.leaderActorId);
  destination = summary->name;
  source = leader->name;
  for (i = 0; i < (s32)sizeof(summary->name); i++)
    *destination++ = *source++;
  summary->level = leader->level;
  summary->playTime = gMainRoutineState.playTime;
  summary->area =
      Area_GetNameIndexFar(gMainRoutineState.next, gMainRoutineState.entrance);
  summary->classId = leader->classId;
  summary->coins = gMainRoutineState.coins;
  summary->djinn[0] = Party_SumDjinnCountsFar(0);
  summary->djinn[1] = Party_SumDjinnCountsFar(1);
  summary->djinn[2] = Party_SumDjinnCountsFar(2);
  summary->djinn[3] = Party_SumDjinnCountsFar(3);
  // Up to four party members, ended by -1.
  Party_ListMembersFar((s16 *)members);
  for (i = 0; i <= 3 && members[i] != BATTLE_UNIT_LIST_END; i++)
    summary->party[i] = members[i];
  summary->party[i] = -1;
  summary->windowHue = gMainRoutineState.windowHue;
  summary->windowBrightness = gMainRoutineState.windowBrightness;
  summary->sendFlag = gMainRoutineState.sendFlag;
  summary->djinnCount = 0;
  for (i = FLAG_DJINN_FIRST; i <= FLAG_DJINNI(DJINNI_WIND19); i++) {
    if (GameFlag_Get(i))
      summary->djinnCount++;
  }
  summary->reunited = GameFlag_Get(FLAG_REUNION) != 0;
  {
    s32 n;
    u32 frames;

    frames = gMainRoutineState.buildStamp;
    word = (u32 *)gGameFlagBytes;
    // The checksum's words, counted in fours and multiplied back below.
    n = SAVE_CHECKSUM_WORDS / 4;
    i = 0;
    n <<= 2;
    Io_Write32(frames, &summary->buildStamp);
    goto test;
    do {
      sum += *word++;
      i++;
    test:;
    } while (i < n);
  }
  summary->checksum = sum;
  return 0;
}

s32 Save_ReadPart(s32 partId, void *buffer);

/// Writes both halves of the save buffer, as they are, to save file `file`.
/// Returns 0, or a SAVE_ERROR_* after showing its message.
s32 Save_WriteFile(s32 file) {
  s32 result = 0;

  if (Save_InitializeState() != 0) {
    FieldMessage_Show((s32)&MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else if (Save_WritePartMasked(file, gSaveBuffer) != 0) {
    FieldMessage_Show((s32)&MsgSaveFailed, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_WRITE;
  }
  Save_ReleaseState();
  return result;
}

/// Writes the link lobby's tallies (gMainRoutineState.linkTallies) into the
/// current file's first part (gSaveSlot), under the saving screen, leaving the
/// rest of the part as it was saved. Returns -1 when no file is current.
s32 Save_WriteLinkTallies(void) {
  void *part;
  s32 value;
  s32 result;
  s32 error;
  s32 progress;

  result = 0;
  part = Heap_AllocEwram(SAVE_SECTORS_PER_PART * SAVE_SECTOR_SIZE);
  value = gSaveSlot;
  if (value != -1) {
    error = Save_InitializeState();
    if (error != 0) {
      FieldMessage_Show((s32)&MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
      result = SAVE_ERROR_NO_MEMORY;
    } else {
      u8 *tallies;

      error = Save_ReadPart(gSaveSlot, part);
      // A failed read is reported, but the part is still written below from
      // whatever the buffer holds.
      if (error != 0) {
        FieldMessage_Show((s32)&MsgSaveFailed, FIELD_MESSAGE_ZOOM);
        result = SAVE_ERROR_READ;
      }
      // The tallies' place in the part is their place in the save buffer.
      tallies = (u8 *)part + (s32)gMainRoutineState.linkTallies;
      tallies -= (s32)gSaveBuffer;
      Iwram_CopyWordsEntry(tallies, gMainRoutineState.linkTallies,
                           sizeof(gMainRoutineState.linkTallies));
      progress = (s32)SaveProgress_Open(SAVE_PROGRESS_SAVING);
      error = Save_WritePartMasked(gSaveSlot, part);
      SaveProgress_Close((struct Window *)progress);
      if (error != 0) {
        FieldMessage_Show((s32)&MsgSaveFailed, FIELD_MESSAGE_ZOOM);
        result = SAVE_ERROR_WRITE;
      }
    }
    Save_ReleaseState();
    Heap_Release(part);
    value = result;
  }
  return value;
}

/// Saves the game to slot (at most 2).
s32 Save_ToFile(u32 slot) {
  s32 result = 0;
  s32 error;

  if (slot > 2)
    slot = 2;
  if (Save_InitializeState() != 0) {
    FieldMessage_Show((s32)&MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    Save_LoadFileSummaries();
    gSaveSlot = slot;
    Sound_PlayFar(MUS_SAVED);
    FieldMessage_Show((s32)&MsgSaving, FIELD_MESSAGE_ZOOM |
                                           FIELD_MESSAGE_KEEP_OPEN |
                                           FIELD_MESSAGE_LOWER);
    Save_BuildFileSummary();
    ActorTable_SnapshotFar();
    error = Save_WritePartMasked(slot, gSaveBuffer);
    while (Message_AllTyped() == 0)
      wait(1);
    Message_CloseFinishedWindows();
    if (error != 0) {
      FieldMessage_Show((s32)&MsgSaveFailed, FIELD_MESSAGE_ZOOM);
      result = SAVE_ERROR_WRITE;
    } else {
      FieldMessage_Show((s32)&MsgGameSaved,
                        FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
    }
  }
  Save_ReleaseState();
  return result;
}

/// Lets the player choose a file and saves the game to it, asking first before
/// it writes over a file in use.
s32 Save_ToChosenFile(void) {
  s32 result = 0;
  s32 slot;
  s32 error;
  s32 progress;
  struct SaveWorkspace *state;

  if (Save_InitializeState() != 0) {
    FieldMessage_Show((s32)&MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    Save_LoadFileSummaries();
    slot = FileList_SelectFile(gSaveSlot, FILE_LIST_SAVE);
    if (slot == -1) {
      result = slot;
    } else {
      state = Ram_HeapSlots->saveState;
      if (state->summary[slot].level != 0) {
        FieldMessage_Show((s32)&MsgOverwriteConfirm,
                          FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN |
                              FIELD_MESSAGE_LOWER);
        while (Message_AllTyped() == 0)
          wait(1);
        if (YesNoMenu_Run(1, 0, 0, NO) != YES) {
          Message_CloseFinishedWindows();
          goto done;
        }
        Message_CloseFinishedWindows();
      }
      gSaveSlot = slot;
      progress = (s32)SaveProgress_Open(SAVE_PROGRESS_SAVING);
      Save_BuildFileSummary();
      ActorTable_SnapshotFar();
      error = Save_WritePartMasked(slot, gSaveBuffer);
      SaveProgress_Close((struct Window *)progress);
      Message_CloseFinishedWindows();
      if (error != 0) {
        FieldMessage_Show((s32)&MsgSaveFailed, FIELD_MESSAGE_ZOOM);
        result = SAVE_ERROR_WRITE;
      } else {
        FieldMessage_Show((s32)&MsgGameSaved,
                          FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_LOWER);
      }
    }
  }
done:
  Save_ReleaseState();
  return result;
}

/// Selects a file and loads it.
s32 Save_ChooseAndLoadFile(s32 mode) {
  s32 result = 0;
  s32 file;

  if (Save_InitializeState() != 0) {
    FieldMessage_Show((s32)&MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    Save_LoadFileSummaries();
    file = FileList_SelectFile(gSaveSlot, mode);
    if (file == -1) {
      result = file;
    } else if (Save_ReadPart(file, gSaveBuffer) != 0) {
      FieldMessage_Show((s32)&MsgLoadFailed, FIELD_MESSAGE_ZOOM);
      result = SAVE_ERROR_READ;
    } else {
      gPlayTimeFrames = gMainRoutineState.playTime;
      gIdleSleepEnabled = gMainRoutineState.idleSleepEnabled;
      gIdleFrames = 0;
      gSaveSlot = file;
    }
  }
  Save_ReleaseState();
  return result;
}

/// Selects a file and copies it to a free file. Its failures say "Failed to
/// erase." (MsgEraseFailed), as erasing's do.
s32 Save_CopyFile(void) {
  s32 result = 0;
  s32 value;
  s32 progress;

  if (Save_InitializeState() != 0) {
    FieldMessage_Show((s32)&MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    Save_LoadFileSummaries();
    value = FileList_SelectFile(0, FILE_LIST_COPY);
    if (value == -1) {
      result = value;
    } else {
      void *buffer = gSaveBuffer;

      value = Save_ReadPart(value, buffer);
      if (value != 0) {
        FieldMessage_Show((s32)&MsgLoadFailed, FIELD_MESSAGE_ZOOM);
        result = SAVE_ERROR_READ;
      } else {
        value = Save_FindFreeFile();
        if (value == SAVE_NO_FREE_FILE) {
          FieldMessage_Show((s32)&MsgEraseFailed, FIELD_MESSAGE_ZOOM);
          result = SAVE_ERROR_NO_FILE;
        } else {
          progress = (s32)SaveProgress_Open(SAVE_PROGRESS_COPYING);
          value = Save_WritePartMasked(value, buffer);
          SaveProgress_Close((struct Window *)progress);
          if (value != 0) {
            FieldMessage_Show((s32)&MsgEraseFailed, FIELD_MESSAGE_ZOOM);
            result = SAVE_ERROR_WRITE;
          } else {
            FieldMessage_Show((s32)&MsgFileCopied, FIELD_MESSAGE_ZOOM);
          }
        }
      }
    }
  }
  Save_ReleaseState();
  return result;
}

#if (LANGUAGE == LANGUAGE_FRENCH)
#define ERASE_CONFIRM_X 4 ///< The French confirmation window is farther left.
#else
#define ERASE_CONFIRM_X 8
#endif

/// Selects a file and erases it.
s32 Save_EraseSelectedFile(void) {
  s32 result = 0;
  s32 value;
  s32 progress;
  s32 error;

  if (Save_InitializeState() != 0) {
    FieldMessage_Show((s32)&MsgNoBackupMemory, FIELD_MESSAGE_ZOOM);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    Save_LoadFileSummaries();
    value = FileList_SelectFile(0, FILE_LIST_ERASE);
    if (value == -1) {
      result = value;
    } else {
      Message_OpenWindow((s32)&MsgEraseConfirm, ERASE_CONFIRM_X, 1, 2);
      while (Message_AllTyped() == 0)
        wait(1);
      if (YesNoMenu_Run(1, 0, 3, NO) != YES) {
        Message_CloseFinishedWindows();
      } else {
        Message_CloseFinishedWindows();
        progress = (s32)SaveProgress_Open(SAVE_PROGRESS_ERASING);
        error = Save_DeletePartMasked(value);
        SaveProgress_Close((struct Window *)progress);
        if (error != 0) {
          FieldMessage_Show((s32)&MsgEraseFailed, FIELD_MESSAGE_ZOOM);
          result = SAVE_ERROR_ERASE;
        } else {
          FieldMessage_Show((s32)&MsgFileErased, FIELD_MESSAGE_ZOOM);
        }
      }
    }
  }
  Save_ReleaseState();
  return result;
}

/// A task that makes colours 8-11 of HIGHLIGHT_PALETTE glow: each follows a
/// sine wave of the frame counter, eight frames behind the one before.
void FileList_UpdatePulse(void) {
  s32 *vblankCount;
  volatile u16 *palette;
  s32 colour;
  FIXED sine;
  s32 i;

  vblankCount = (s32 *)&gVBlankCount;
  palette = (volatile u16 *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 8);
  for (i = 0; i <= 3; i++) {
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
  }
}

/// Starts the palette glow (FileList_UpdatePulse) as a task.
void FileList_StartPulse(void) {
  Task_Add((s32)FileList_UpdatePulse, TASK_PASS_FRAME);
}

/// Stops the palette glow.
void FileList_StopPulse(void) { Task_Remove((u32)((s32)FileList_UpdatePulse)); }

/// The window tile the file list's chosen row is filled with, drawn in
/// HIGHLIGHT_PALETTE, whose colours FileList_UpdatePulse makes glow.
#define FILE_LIST_HIGHLIGHT_TILE 6

/// Fills the `width` x `height` tile rectangle at (x, y) inside `window`,
/// clipped to the screen as Window_SetTextTileBitRect clips, with
/// FILE_LIST_HIGHLIGHT_TILE: the file list's glowing bar under the chosen
/// file.
void FileList_FillHighlight(struct Window *window, s32 x, s32 y, s32 width,
                            s32 height) {
  struct WindowState *state = (struct WindowState *)Ram_HeapSlots->windowState;
  s32 sum;

  sum = x + (s16)window->x;
  x = sum + 1;
  sum = y + (s16)window->y;
  y = sum + 1;

  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > (DISPLAY_TILE_WIDTH - 1))
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
    s32 offset = yOffset + ((s32)state + xOffset);
    do {
      s32 remaining = width;
      u16 *cell = (u16 *)(offset + offsetof(struct WindowState, tilemap));
      if (remaining != 0) {
        do {
          remaining--;
          *cell++ =
              BG_TILE_PALETTE(HIGHLIGHT_PALETTE) | FILE_LIST_HIGHLIGHT_TILE;
        } while (remaining != 0);
      }
      height--;
      offset += BG_SCREEN_WIDTH * 2;
    } while (height != 0);
    state->dirty = 1;
  }
}

/// Shows the party of save file `summary` (up to four party members) as sprites
/// in a row from tile (x, y) inside `window`, and starts placing them every
/// frame (FileList_PlacePartySprites).
void FileList_ShowPartySprites(struct Window *window, s32 x, s32 y,
                               struct SaveSummary *summary) {
  struct FileListState *menu = Ram_HeapSlots->menuState;
  s32 i;

  for (i = 0; i < 4 && summary->party[i] != -1; i++) {
    struct FieldSprite *sprite =
        (struct FieldSprite *)Sprite_CreateFar(summary->party[i]);

    if (sprite != 0) {
      Sprite_StartAnimationFar(sprite, ANIM_STAND);
      sprite->flags = 0;
      sprite->priority = 0;
    }

    menu->partyMemberIcons[i] = sprite;
    menu->partyMemberX[i] =
        (x + (s16)window->x + i * FILE_LIST_SPRITE_SPACING) * 8 + 16;
    menu->partyMemberY[i] = (y + (s16)window->y) * 8 + 16;
    menu->partyMemberScale[i] = FIXED_ONE;
  }

  Task_Add(FileList_PlacePartySprites, TASK_PASS_FRAME);
}

/// Stops placing the party sprites of the file list and releases them.
void FileList_HidePartySprites(void) {
  struct FileListState *menu = Ram_HeapSlots->menuState;
  s32 i;

  Task_Remove((s32)FileList_PlacePartySprites);
  for (i = 0; i < 4; i++) {
    if (menu->partyMemberIcons[i] != NULL) {
      Sprite_DestroyFar(menu->partyMemberIcons[i]);
      menu->partyMemberIcons[i] = NULL;
    }
  }
}

/// A task: draws each party sprite of the file list at its place and scale for
/// this frame.
void FileList_PlacePartySprites(void) {
  struct FileListState *state =
      (struct FileListState *)Ram_HeapSlots->menuState;
  s32 scale[2];
  FIXED position[4];
  s32 index = 0;
  s32 sprite;

  while (1) {
    sprite = (s32)state->partyMemberIcons[index];
    if (sprite != 0) {
      scale[0] = state->partyMemberScale[index];
      scale[1] = state->partyMemberScale[index];
      position[0] = toFIXED(state->partyMemberX[index]);
      position[1] = toFIXED(FILE_LIST_SPRITE_LIFT);
      position[2] =
          toFIXED(state->partyMemberY[index]) + toFIXED(FILE_LIST_SPRITE_LIFT);
      position[3] = 0;
      Render_ApplyProjectedPlacementFar((struct Sprite *)sprite, position,
                                        scale, FACING_SOUTH);
    }
    index += 1;
    if (index > 3) {
      break;
    }
  }
}

/// Draws a saved game's Djinn of each element, four two-digit numbers 24 pixels
/// apart at y = 16, into win; nothing when win is null.
void FileList_DrawDjinnCounts(struct Window *win, struct SaveSummary *summary) {
  s32 i;

  if (win != 0) {
    for (i = 0; i < 4; i++)
      Text_DrawNumberAtOffset((s8)summary->djinn[i], 2, win,
                              i * FILE_LIST_SPRITE_SPACING * 8, 16);
  }
}

/// Edition-specific x positions of the level and coin labels, in pixels.
#if (LANGUAGE == LANGUAGE_SPANISH)
#define SUMMARY_LEVEL_X 64
#define SUMMARY_COINS_X 56
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define SUMMARY_LEVEL_X 59
#define SUMMARY_COINS_X 56
#elif (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_ITALIAN)
#define SUMMARY_COINS_X 56
#else
#define SUMMARY_COINS_X 48
#endif

/// Draws a file's summary into window, when there is one: the leader's name,
/// level and class, the play time and the coins.
void FileList_DrawSummary(struct Window *win, struct SaveSummary *summary) {
  u32 playTime[4]; // the play time text
  s32 y;

  if (win != 0) {
    WindowSprite_PrepareForRedraw(win);
    Window_DrawDividerLine(win, 0, 4, 13, 4);
    Text_DrawStringAtOffset(summary->name, win, 0, 0);
    y = 0;
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
    Text_DrawStringAtOffset((u8 *)gStatusMenu_LevelLetterString, win,
                            SUMMARY_LEVEL_X, 0);
#else
    Text_DrawStringInWindow((u8 *)gStatusMenu_LevelLetterString, (s32)win, 72,
                            0);
#endif
    Text_DrawNumberAtOffset(summary->level, 2, win, 80, y);
    Text_DrawMessageAsTiles(summary->classId + (s32)MsgClassName, win, 0, 16);
#if (LANGUAGE == LANGUAGE_JAPANESE)
    y = 32;
    Text_DrawNumberInWindow(summary->coins, 6, (s32)win, 24, y);
    Text_DrawMessageAsTiles((s32)MsgCoins, win, 72, 32);
    Text_DrawMessageAsTiles((s32)MsgStatusLabel, win, 0, 48);
    Text_DrawStringInWindow(
        String_FormatPlayTime(summary->playTime, (u8 *)playTime), (s32)win, 48,
        48);
#else
    Text_DrawMessageAsTiles((s32)MsgStatusLabel, win, 0, 32);
    Text_DrawStringInWindow(
        String_FormatPlayTime(summary->playTime, (u8 *)playTime), (s32)win, 48,
        40);
    y = 48;
    Text_DrawNumberInWindow(summary->coins, 6, (s32)win, 0, y);
#if (LANGUAGE == LANGUAGE_FRENCH)
    // French takes the singular "Pièce" for one coin or none.
    if (summary->coins > 1)
      Text_DrawMessageAsTiles((s32)MsgCoins, win, SUMMARY_COINS_X, 48);
    else
      Text_DrawMessageAsTiles((s32)MsgPiece, win, SUMMARY_COINS_X, 48);
#else
    Text_DrawMessageAsTiles((s32)MsgCoins, win, SUMMARY_COINS_X, 48);
#endif
#endif
  }
}

/// Where the leader's name and the area name start in a file's row of the file
/// list, in pixels: the European languages start both farther left.
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
#define FILE_LIST_LEADER_X 12
#define FILE_LIST_AREA_X 72
#else
#define FILE_LIST_LEADER_X 6
#define FILE_LIST_AREA_X 62
#endif

/// FileList_SelectFile's result when no file can be chosen in its mode.
#define SAVE_MENU_NO_FILE -2

/// The file list: lists the three files with each one's leader and area (or
/// why it cannot be chosen) and, for the file under the cursor, its summary,
/// its party and its Djinn, with the file's own palette glow. Returns the file
/// chosen in mode `menuMode` (enum FileMenuMode), -1 when cancelled.
s32 FileList_SelectFile(s32 selectedFile, s32 menuMode) {
  struct Window *fileList;
  struct SaveWorkspace *save;
  struct Window *summaryWindow;
  struct Window *partyWindow;
  struct Window *djinnWindow;
  struct SaveSummary *summary;
  void *modePicture;
  s32 summaryDirty;
  u32 buildStamp;
  void *spriteState;
  s32 result;
  s32 i;
  s32 y;

  spriteState = HeapSlot_Alloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
  save = Ram_HeapSlots->saveState;
  summaryWindow = NULL;
  partyWindow = NULL;
  djinnWindow = NULL;
  summaryDirty = 1;
  buildStamp = BuildStamp_GetTimeFar();
  if (selectedFile < 0)
    selectedFile = 0;
  if (menuMode == FILE_LIST_CONTINUE) {
    for (i = 0;
         i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
                                 save->summary[selectedFile].sendFlag != 0);
         i++) {
      selectedFile++;
      if (selectedFile > 2)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (menuMode == FILE_LIST_BATTLE) {
    for (i = 0;
         i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
                                 save->summary[selectedFile].djinnCount == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (menuMode == FILE_LIST_CLEARED) {
    for (i = 0;
         i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
                                 save->summary[selectedFile].sendFlag == 0);
         i++) {
      selectedFile++;
      if (selectedFile > 2)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (menuMode == FILE_LIST_UPDATE) {
    for (i = 0;
         i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
                                 save->summary[selectedFile].reunited != 0);
         i++) {
      selectedFile++;
      if (selectedFile > 2)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (menuMode != FILE_LIST_SAVE) {
    for (i = 0; i < SAVE_FILE_COUNT && save->summary[selectedFile].level == 0;
         i++) {
      selectedFile++;
      if (selectedFile > 2)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }

  Dma_Fill32(0, spriteState, MENU_STATE_SIZE);
  FileList_StartPulse();
  fileList = Window_Create(1, 2, 28, 7, WINDOW_FLAG_INSTANT);
  for (i = 0, summary = save->summary; i < SAVE_FILE_COUNT; i++, summary++) {
    if (save->summary[i].level == 0) {
      Text_DrawMessage((s32)MsgSaveNoData, (s32)fileList, 10, i * 16);
    } else if (save->summary[i].buildStamp < buildStamp) {
      Text_DrawMessage((s32)MsgSaveContinue, (s32)fileList, 10, i * 16);
    } else if (save->summary[i].playTime !=
               save->summary[i].secondPartPlayTime) {
      Text_DrawMessage((s32)MsgSaveCorrupted, (s32)fileList, 10, i * 16);
    } else if (menuMode == FILE_LIST_CLEARED &&
               save->summary[i].sendFlag == 0) {
      Text_DrawMessage((s32)MsgSaveNotCleared, (s32)fileList, 10, i * 16);
    } else if (menuMode == FILE_LIST_UPDATE && save->summary[i].reunited != 0) {
      Text_DrawMessage((s32)MsgSavePostReunion, (s32)fileList, 10, i * 16);
    } else {
      y = i * 16;
      Text_DrawString(summary->name, (s32)fileList, FILE_LIST_LEADER_X, y);
      Text_DrawMessage(save->summary[i].area + (s32)MsgDebugAreaName,
                       (s32)fileList, FILE_LIST_AREA_X, y);
    }
  }
  modePicture = MenuTitle_Create(menuMode, (struct Window *)fileList, 72, -24);
  DjinnFigures_InitializeTopRowFar();
  for (i = 0, y = 144; i < 4; i++, y += 24)
    DjinnFigures_SetSlotPositionFar(i, y, 128);
  DjinnFigures_SetShownFar(0);
  wait(1);

  while (1) {
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
        FileList_ShowPartySprites((struct Window *)partyWindow, 0, 0,
                                  &save->summary[selectedFile]);
        wait(1);
        if ((s8)save->summary[selectedFile].djinn[0] +
                (s8)save->summary[selectedFile].djinn[1] +
                (s8)save->summary[selectedFile].djinn[2] +
                (s8)save->summary[selectedFile].djinn[3] !=
            0) {
          if (djinnWindow == NULL)
            djinnWindow = Window_Create(16, 14, 13, 5, WINDOW_FLAG_INSTANT);
          FileList_DrawDjinnCounts((struct Window *)djinnWindow,
                                   &save->summary[selectedFile]);
          DjinnFigures_SetShownFar(1);
        } else {
          DjinnFigures_SetShownFar(0);
          Window_Close(djinnWindow, 2);
          djinnWindow = NULL;
        }
      } else {
        WindowColor_Apply(gMainRoutineState.windowHue,
                          gMainRoutineState.windowBrightness);
        DjinnFigures_SetShownFar(0);
        FileList_HidePartySprites();
        Window_Close(djinnWindow, 2);
        Window_Close(partyWindow, 2);
        Window_Close(summaryWindow, 2);
        djinnWindow = NULL;
        partyWindow = NULL;
        summaryWindow = NULL;
      }
      WindowSprite_RedrawSavedRect(fileList);
      Window_DrawDividerLine(fileList, 0, 2, 27, 2);
      Window_DrawDividerLine(fileList, 0, 4, 27, 4);
      FileList_FillHighlight(fileList, 0, selectedFile * 2, 26, 1);
    }
    MenuTitle_Bob(modePicture);
    wait(1);
    if (gInput.repeat & KEY_UP) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      summaryDirty = 1;
      do {
        selectedFile = (selectedFile + SAVE_FILE_COUNT - 1) % SAVE_FILE_COUNT;
        if (menuMode == FILE_LIST_SAVE)
          break;
      } while (save->summary[selectedFile].level == 0 ||
               (menuMode == FILE_LIST_CONTINUE &&
                save->summary[selectedFile].sendFlag != 0) ||
               (menuMode == FILE_LIST_BATTLE &&
                save->summary[selectedFile].djinnCount == 0) ||
               (menuMode == FILE_LIST_CLEARED &&
                save->summary[selectedFile].sendFlag == 0) ||
               (menuMode == FILE_LIST_UPDATE &&
                save->summary[selectedFile].reunited != 0));
    } else if (gInput.repeat & KEY_DOWN) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      summaryDirty = 1;
      do {
        selectedFile = (selectedFile + SAVE_FILE_COUNT + 1) % SAVE_FILE_COUNT;
        if (menuMode == FILE_LIST_SAVE)
          break;
      } while (save->summary[selectedFile].level == 0 ||
               (menuMode == FILE_LIST_CONTINUE &&
                save->summary[selectedFile].sendFlag != 0) ||
               (menuMode == FILE_LIST_BATTLE &&
                save->summary[selectedFile].djinnCount == 0) ||
               (menuMode == FILE_LIST_CLEARED &&
                save->summary[selectedFile].sendFlag == 0) ||
               (menuMode == FILE_LIST_UPDATE &&
                save->summary[selectedFile].reunited != 0));
    } else if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    } else if (gInput.pressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = selectedFile;
      break;
    }
  }
  DjinnFigures_ReleaseTopRowFar();
  FileList_HidePartySprites();
  Window_Close(djinnWindow, 2);
  Window_Close(partyWindow, 2);
  Window_Close(summaryWindow, 2);
  Window_Close(fileList, 2);
  FileList_StopPulse();
  HeapSlot_Release(HEAP_SLOT_MENU);
  WindowColor_Apply(gMainRoutineState.windowHue,
                    gMainRoutineState.windowBrightness);
  wait(1);
done:
  return result;
}

#undef SAVING_X
#undef ERASING_X
#undef COPYING_X
#undef DMA_STOP
#undef ERASE_CONFIRM_X
#undef SUMMARY_LEVEL_X
#undef SUMMARY_COINS_X
#undef FILE_LIST_LEADER_X
#undef FILE_LIST_AREA_X
#undef SAVE_MENU_NO_FILE
#undef FILE_LIST_HIGHLIGHT_TILE
#undef SAVE_CHECKSUM_WORDS
#undef FILE_LIST_GLOW_RED
#undef FILE_LIST_GLOW_GREEN
#undef FILE_LIST_GLOW_BLUE
