/// @file
/// Save files, the file list and the saving screen's progress bar.
// Part of screens.c.

#include "../screens_internal.h"
#include "system/io_write.h"

s32 Func_080cc67c(s32);
s32 Func_080cc7c4(void);
extern const u8 gWindow_ProgressBarRows[];

/// The progress bar's serial callback: advances the stored progress by step
/// and draws row min(progress, 6) of gWindow_ProgressBarRows.
void Func_08042dc4(s32 step) {
  u8 *save = Ram_HeapSlots->serialWork;
  u32 row;
  const u8 *src;
  u16 *dst;
  s32 i;

  if (*(s16 *)(save + 0x3142) <= 3)
    step <<= 1;
  row = (s16)(*(u16 *)(save + 0x3140) += step);
  if (row > 6)
    row = 6;
  dst = (u16 *)0x06002252;
  src = gWindow_ProgressBarRows + row * 12;
  for (i = 11; i >= 0; i--)
    *dst++ = *src++ + 0xf080;
}

extern const u8 gWindow_ProgressBarTiles[];

void Sound_SetBgmVolumeTargetFar(s32 target, s32 step);
void Sound_PlayFar(s32 cue);
void Func_080c8698(void);

// Heading x coordinate: only French differs.
#if (LANGUAGE == LANGUAGE_FRENCH)
#define SAVING_X 40
#define ERASING_X 48
#define COPYING_X 48
#else
#define SAVING_X 76
#define ERASING_X 92
#define COPYING_X 84
#endif

/// Opens the progress screen: 0 saves, 1 erases, 2 copies.
struct Window *SaveProgress_Open(u32 kind) {
  struct Window *window;
  void *tiles;
  s32 message;

  window = Window_Create(0, 0, 30, 20, 0);
  tiles = Heap_AllocEwram(0x100);
  Resource_DecodeType01(gWindow_ProgressBarTiles, tiles);
  Dma_Set(tiles, (void *)0x06001000, DMA_COPY_WORDS(64), REG_DMA3);
  switch (kind) {
  case 0:
    Text_DrawMessage((s32)&MsgSaving, (s32)window, SAVING_X, 40);
    break;
  case 1:
    Text_DrawMessage((s32)&MsgErasing, (s32)window, ERASING_X, 40);
    break;
  case 2:
    Text_DrawMessage((s32)&MsgCopying, (s32)window, COPYING_X, 40);
    break;
  }
  message = (s32)&MsgDoNotTurnThePower;
  Text_DrawMessage(message, (s32)window, 40, 88);
  Text_DrawMessage(message + 1, (s32)window, 40, 104);
  Sound_SetBgmVolumeTargetFar(0, 30);
  Sound_PlayFar(298);
  wait(10);
  Heap_Release(tiles);
  return window;
}

/// Closes the progress screen.
struct Window *SaveProgress_Close(struct Window *window) {
  wait(10);
  Func_080c8698();
  Sound_SetBgmVolumeTargetFar(0x100, 30);
  Window_Close(window, 0);
  return window;
}

void Serial_QueueCommand(u32 command);
s32 Func_08015ca4(s32 partId, void *buffer);

#define DMA_STOP(n)                                                            \
  do {                                                                         \
    volatile u16 *dma = REG_DMA0 + (n) * 6;                                    \
    dma[5] &= 0xc5ff;                                                          \
    dma[5] &= 0x7fff;                                                          \
    dma[5];                                                                    \
  } while (0)

/// Runs Func_08015ca4 with interrupts masked and all four DMA channels
/// stopped, queueing Func_08042dc4 as the serial command around it, then
/// restores IE and IME. Func_08042ff0 does the same around
/// Save_DeletePart.
s32 Func_08042f10(s32 partId, void *buffer) {
  volatile u32 ime;
  volatile u32 ie;
  s32 result;

  /* FAKEMATCH(temporary): IRQ_LOCK written out with the saved copy's address
   * taken first; as IRQ_LOCK(ime), 4 of 102 instructions differ in tla-en. */
  do {
    volatile u32 *saved = &ime;
    volatile u16 *imeReg = &REG_IME;

    *saved = *imeReg;
    *imeReg = (u16)(u32)imeReg;
  } while (0);
  ie = REG_IE;
  Io_Write16(0, &REG_IE);
  DMA_STOP(0);
  DMA_STOP(1);
  DMA_STOP(2);
  DMA_STOP(3);
  IRQ_UNLOCK(ime);
  Serial_QueueCommand((u32)Func_08042dc4);
  result = Func_08015ca4(partId, buffer);
  Serial_QueueCommand(0);
  IRQ_LOCK(ime);
  REG_IE = ie;
  IRQ_UNLOCK(ime);
  return result;
}

void Serial_QueueCommand(u32 command);
s32 Save_DeletePart(s32 partId);

/// Deletes a part of a save file (Save_DeletePart) with interrupts masked and
/// all four DMA channels stopped, queueing Func_08042dc4 as the serial command
/// around it, as Func_08042f10 does around Func_08015ca4.
s32 Func_08042ff0(s32 partId) {
  volatile u32 ime;
  volatile u32 ie;
  s32 result;

  /* FAKEMATCH(temporary): IRQ_LOCK written out with the saved copy's address
   * taken first; as IRQ_LOCK(ime), 4 of 97 instructions differ in tla-en. */
  do {
    volatile u32 *saved = &ime;
    volatile u16 *imeReg = &REG_IME;

    *saved = *imeReg;
    *imeReg = (u16)(u32)imeReg;
  } while (0);
  ie = REG_IE;
  Io_Write16(0, &REG_IE);
  DMA_STOP(0);
  DMA_STOP(1);
  DMA_STOP(2);
  DMA_STOP(3);
  IRQ_UNLOCK(ime);
  Serial_QueueCommand((u32)Func_08042dc4);
  result = Save_DeletePart(partId);
  Serial_QueueCommand(0);
  IRQ_LOCK(ime);
  REG_IE = ie;
  IRQ_UNLOCK(ime);
  return result;
}

// Clock texts: a frame count written as hours, minutes, seconds and
// hundredths, and the save files' play time.
//
// Each field comes from String_FormatNumber; a two-digit field is formatted
// with 100 added, which gives it a leading zero in its last two digits.

u8 *String_FormatNumber(u8 *buffer, s32 input, s32 width);

// XXX: what shows this clock; the overlays that call Func_08038400 would
// settle it.

/// Writes frames (60 a second) as "HHH:MM:SS:CC" into out and returns out.
/// Reached only through the far call Func_08038400.
u8 *Func_080430c4(u32 frames, u8 *out) {
  u8 digits[16];
  u8 *digitsCursor;
  u8 *textCursor;
  u32 fraction;
  u32 seconds;
  u32 minutes;
  u32 hours;
  u32 hundredths;

  fraction = frames % 60;
  frames /= 60;
  hundredths = fraction * 100 / 60;
  seconds = frames % 60;
  frames /= 60;
  minutes = frames % 60;
  hours = frames / 60;

  digitsCursor = String_FormatNumber(digits, hours, 3);
  *out = *digitsCursor;
  digitsCursor++;
  /* FAKEMATCH(barrier): the hours' first store comes before the cursor's
   * step; as plain statements, the six barriers of this function make 20 of
   * 106 instructions differ in tla-en. */
  __asm__ volatile("");
  textCursor = out + 1;
  /* FAKEMATCH(barrier): the load waits for the cursor's step. */
  __asm__("" : "+l"(digitsCursor) : "l"(textCursor));
  *textCursor = *digitsCursor;
  textCursor++;
  *textCursor = digitsCursor[1];
  textCursor++;
  *textCursor = ':';

  digitsCursor = String_FormatNumber(digits, minutes + 100, 2);
  textCursor++;
  *textCursor = digitsCursor[0];
  textCursor++;
  *textCursor = digitsCursor[1];
  textCursor++;
  *textCursor = ':';

  digitsCursor = String_FormatNumber(digits, seconds + 100, 2);
  textCursor++;
  *textCursor = digitsCursor[0];
  textCursor++;
  *textCursor = digitsCursor[1];
  textCursor++;
  *textCursor = ':';

  digitsCursor = String_FormatNumber(digits, hundredths + 100, 2);
  textCursor++;
  *textCursor = digitsCursor[0];
  textCursor++;
  *textCursor = digitsCursor[1];
  textCursor[1] = 0;

  return out;
}

/// Writes a save file's play time (in frames) as "HHH:MM" into out and
/// returns out; the clock stops at 999:59.
void *Text_FormatPlayTime(s32 frames, u8 *out) {
  u8 digits[16];
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

u32 Save_FindFreeFile(void) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  struct SaveSummary *p;
  u32 i;

  p = work->summary;
  for (i = 0; i < 3; i++, p++) {
    if (p->level == 0)
      return i;
  }
  return 999;
}

s32 Func_0801596c(void);
s32 Save_LoadFileSummaries(void);
void Save_ReleaseWork(void);

s32 Save_CountFilesExcludingSent(s32 flag) {
  s32 i;
  s32 cnt;
  struct SaveSummary *summary;
  struct SaveWorkspace *work;

  if (Func_0801596c() != 0) {
    cnt = -9;
  } else {
    cnt = Save_LoadFileSummaries();
    if (flag != 0) {
      work = Ram_HeapSlots->serialWork;
      summary = work->summary;
      for (i = 0; i < 3; i++) {
        if (summary[i].sendFlag != 0)
          cnt--;
      }
    }
  }
  Save_ReleaseWork();
  return cnt;
}

extern u16 gData_02003860;
extern u16 gData_020036d8;
extern u16 gData_02005350;

s32 Func_0801596c(void);
s32 Save_LoadFileSummaries(void);
void Save_ReleaseWork(void);

s32 Func_080432a4(void) {
  s32 err;
  s32 cnt;
  s32 ret;

  err = Func_0801596c();
  cnt = 0;
  ret = -9;
  if (err == 0) {
    s32 i;
    struct SaveWorkspace *work;

    ret = Save_LoadFileSummaries();
    work = Ram_HeapSlots->serialWork;
    gData_02003860 = 0;
    gData_020036d8 = 0;
    gData_02005350 = 0;
    for (i = 0; i < 3; i++) {
      if (work->summary[i].sendFlag != 0) {
        gData_02003860 = 1;
        cnt++;
      }
      if (work->summary[i].flagCount != 0)
        gData_020036d8 = 1;
      if (work->summary[i].level != 0 && work->summary[i].hasFlag34 == 0)
        gData_02005350 = 1;
    }
  }
  Save_ReleaseWork();
  if (ret != 0 && cnt == ret)
    return ret + 100;
  return ret;
}

struct SaveGameWords {
  u32 buildStamp;
  u32 playTime;
};

#define SAVE_GAME_WORDS ((struct SaveGameWords *)&gMainRoutineWork)

extern u8 gData_02000000[];
extern u32 gData_02001000;

// No prototype, as The Broken Seal declares it: this caller passes 0, and
// FileList_SelectFile's twin passes nothing.
u32 BuildStamp_GetTimeFar();
s32 GameFlag_Test(s32 flag);

// Builds the save header.
s32 Save_BuildFileSummary(void) {
  u16 owners[14];
  struct SaveSummary *summary;
  struct BattleUnit *owner;
  u8 *source;
  u8 *destination;
  u32 *word;
  s32 i;
  u32 sum = 0;

  summary = (struct SaveSummary *)(gData_02000000 - 16);
  SAVE_GAME_WORDS->buildStamp = BuildStamp_GetTimeFar(0);
  SAVE_GAME_WORDS->playTime = gFrameWaitCount;
  gData_02001000 = gFrameWaitCount;
  gMainRoutineWork.unk244[6] = gAutoSleepEnabled;
  owner = Unit_Get(gMainRoutineWork.currentOwner);
  destination = summary->name;
  source = owner->name;
  for (i = 11; i >= 0; i--)
    *destination++ = *source++;
  summary->level = owner->level;
  summary->playTime = SAVE_GAME_WORDS->playTime;
  summary->area = Scene_GetAreaNameIndexFar(gMainRoutineWork.scene,
                                            gMainRoutineWork.entrance);
  summary->classId = owner->classIndex;
  summary->coins = gMainRoutineWork.coins;
  summary->djinn[0] = Party_SumDjinnCountsFar(0);
  summary->djinn[1] = Party_SumDjinnCountsFar(1);
  summary->djinn[2] = Party_SumDjinnCountsFar(2);
  summary->djinn[3] = Party_SumDjinnCountsFar(3);
  Party_ListActiveOwnersFar((s16 *)owners);
  for (i = 0; i <= 3 && owners[i] != 255; i++)
    summary->party[i] = owners[i];
  summary->party[i] = -1;
  summary->paletteGlow = gMainRoutineWork.unk220[5];
  summary->paletteGlowSub = gMainRoutineWork.unk220[6];
  summary->sendFlag = gMainRoutineWork.unk22d[2];
  summary->flagCount = 0;
  for (i = 48; i <= 127; i++) {
    if (GameFlag_Test(i))
      summary->flagCount++;
  }
  summary->hasFlag34 = GameFlag_Test(34) != 0;
  {
    s32 n;
    u32 frames;

    frames = SAVE_GAME_WORDS->buildStamp;
    word = (u32 *)gGameFlagBytes;
    /* FAKEMATCH(barrier): an empty asm reading frames and word, so both load
     * before the bound; as the same statements without it, 10 of 179
     * instructions differ in tla-en. */
    __asm__("" : : "r"(frames), "r"(word));
    n = 242;
    i = 0;
    n <<= 2;
    summary->frames = frames;
    // Entered at its test: as a while loop, 32 of 179 instructions differ (it
    // counts down and loses the entry jump).
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

extern u8 gData_02000000[];
extern u8 gData_02000504[];
extern s16 gData_020036d0;

s32 Func_0801596c(void);
s32 Save_LoadFileSummaries(void);
void Save_ReleaseWork(void);
s32 Save_ReadPart(s32 partId, void *buffer);
void FieldMessage_Show(s32 message, s32 position);

s32 Save_WriteFile(s32 file) {
  s32 result = 0;

  if (Func_0801596c() != 0) {
    FieldMessage_Show((s32)&MsgNoBackupMemory, 1);
    result = -9;
  } else if (Func_08042f10(file, gData_02000000) != 0) {
    FieldMessage_Show((s32)&MsgSaveFailed, 1);
    result = -3;
  }
  Save_ReleaseWork();
  return result;
}

s32 Func_08043534(void) {
  void *buffer;
  s32 value;
  s32 result;
  s32 found;
  s32 token;

  result = 0;
  buffer = Heap_AllocEwram(0x3000);
  value = gData_020036d0;
  if (value != -1) {
    found = Func_0801596c();
    if (found != 0) {
      FieldMessage_Show((s32)&MsgNoBackupMemory, 1);
      result = -9;
    } else {
      u8 *dst;

      found = Save_ReadPart(gData_020036d0, buffer);
      if (found != 0) {
        FieldMessage_Show((s32)&MsgSaveFailed, 1);
        result = -2;
      }
      dst = (u8 *)buffer + (s32)gData_02000504;
      dst -= (s32)gData_02000000;
      Iwram_CopyWords(dst, gData_02000504, 16);
      token = (s32)SaveProgress_Open(0);
      found = Func_08042f10(gData_020036d0, buffer);
      SaveProgress_Close((struct Window *)token);
      if (found != 0) {
        FieldMessage_Show((s32)&MsgSaveFailed, 1);
        result = -3;
      }
    }
    Save_ReleaseWork();
    Heap_Release(buffer);
    value = result;
  }
  return value;
}

extern u8 gData_02000000[];
extern s16 gData_020036d0;

s32 Func_0801596c(void);
s32 Save_LoadFileSummaries(void);
void Save_ReleaseWork(void);
void FieldMessage_Show(s32 message, s32 position);
void Sound_PlayFar(s32 cue);
void ObjectTable_SnapshotFar(void);
s32 Message_AllTyped(void);
void Message_CloseFinishedWindows(void);

/// Saves the game to slot (at most 2).
s32 Save_ToFile(u32 slot) {
  s32 result = 0;
  s32 found;

  if (slot > 2)
    slot = 2;
  if (Func_0801596c() != 0) {
    FieldMessage_Show((s32)&MsgNoBackupMemory, 1);
    result = -9;
  } else {
    Save_LoadFileSummaries();
    gData_020036d0 = slot;
    Sound_PlayFar(85);
    FieldMessage_Show((s32)&MsgSaving, 13);
    Save_BuildFileSummary();
    ObjectTable_SnapshotFar();
    found = Func_08042f10(slot, gData_02000000);
    while (Message_AllTyped() == 0)
      wait(1);
    Message_CloseFinishedWindows();
    if (found != 0) {
      FieldMessage_Show((s32)&MsgSaveFailed, 1);
      result = -3;
    } else {
      FieldMessage_Show((s32)&MsgGameSaved, 9);
    }
  }
  Save_ReleaseWork();
  return result;
}

s32 Menu_RunConfirmSelection(s32 low, s32 message, s32 w, s32 selection);

/// Lets the player choose a file and saves the game to it, asking first before
/// it writes over a file in use.
s32 Func_08043690(void) {
  s32 result = 0;
  s32 slot;
  s32 found;
  s32 token;
  struct SaveWorkspace *work;

  if (Func_0801596c() != 0) {
    FieldMessage_Show((s32)&MsgNoBackupMemory, 1);
    result = -9;
  } else {
    Save_LoadFileSummaries();
    slot = Func_08043cd8(gData_020036d0, 0);
    if (slot == -1) {
      result = slot;
    } else {
      work = Ram_HeapSlots->serialWork;
      if (work->summary[slot].level != 0) {
        FieldMessage_Show((s32)&MsgOverwriteConfirm, 13);
        while (Message_AllTyped() == 0)
          wait(1);
        if (Menu_RunConfirmSelection(1, 0, 0, 1) != 0) {
          Message_CloseFinishedWindows();
          goto done;
        }
        Message_CloseFinishedWindows();
      }
      gData_020036d0 = slot;
      token = (s32)SaveProgress_Open(0);
      Save_BuildFileSummary();
      ObjectTable_SnapshotFar();
      found = Func_08042f10(slot, gData_02000000);
      SaveProgress_Close((struct Window *)token);
      Message_CloseFinishedWindows();
      if (found != 0) {
        FieldMessage_Show((s32)&MsgSaveFailed, 1);
        result = -3;
      } else {
        FieldMessage_Show((s32)&MsgGameSaved, 9);
      }
    }
  }
done:
  Save_ReleaseWork();
  return result;
}

extern u8 gData_02000000[];
extern s16 gData_020036d0;

s32 Func_0801596c(void);
s32 Save_LoadFileSummaries(void);
void Save_ReleaseWork(void);
s32 Save_ReadPart(s32 partId, void *buffer);
void FieldMessage_Show(s32 message, s32 position);

/// Selects a file and loads it.
s32 Save_ChooseAndLoadFile(s32 mode) {
  s32 result = 0;
  s32 file;

  if (Func_0801596c() != 0) {
    FieldMessage_Show((s32)&MsgNoBackupMemory, 1);
    result = -9;
  } else {
    Save_LoadFileSummaries();
    file = Func_08043cd8(gData_020036d0, mode);
    if (file == -1) {
      result = file;
    } else if (Save_ReadPart(file, gData_02000000) != 0) {
      FieldMessage_Show((s32)&MsgLoadFailed, 1);
      result = -2;
    } else {
      gFrameWaitCount = SAVE_GAME_WORDS->playTime;
      gAutoSleepEnabled = gMainRoutineWork.unk244[6];
      gIdleFrameCount = 0;
      gData_020036d0 = file;
    }
  }
  Save_ReleaseWork();
  return result;
}

/// Selects a file and copies it to a free file.
s32 Save_CopyFile(void) {
  s32 result = 0;
  s32 value;
  s32 token;

  if (Func_0801596c() != 0) {
    FieldMessage_Show((s32)&MsgNoBackupMemory, 1);
    result = -9;
  } else {
    Save_LoadFileSummaries();
    value = Func_08043cd8(0, 2);
    if (value == -1) {
      result = value;
    } else {
      void *buffer = gData_02000000;

      value = Save_ReadPart(value, buffer);
      if (value != 0) {
        FieldMessage_Show((s32)&MsgLoadFailed, 1);
        result = -2;
      } else {
        value = Save_FindFreeFile();
        if (value == 999) {
          FieldMessage_Show((s32)&MsgEraseFailed, 1);
          result = -5;
        } else {
          token = (s32)SaveProgress_Open(2);
          value = Func_08042f10(value, buffer);
          SaveProgress_Close((struct Window *)token);
          if (value != 0) {
            FieldMessage_Show((s32)&MsgEraseFailed, 1);
            result = -3;
          } else {
            FieldMessage_Show((s32)&MsgFileCopied, 1);
          }
        }
      }
    }
  }
  Save_ReleaseWork();
  return result;
}

s32 Message_OpenWindow(s32 message, s32 x, s32 y, s32 style);
s32 Message_AllTyped(void);
void Message_CloseFinishedWindows(void);

#if (LANGUAGE == LANGUAGE_FRENCH)
#define ERASE_CONFIRM_X 4 ///< The French confirmation window is farther left.
#else
#define ERASE_CONFIRM_X 8
#endif

/// Selects a file and erases it.
s32 Save_EraseSelectedFile(void) {
  s32 result = 0;
  s32 value;
  s32 token;
  s32 found;

  if (Func_0801596c() != 0) {
    FieldMessage_Show((s32)&MsgNoBackupMemory, 1);
    result = -9;
  } else {
    Save_LoadFileSummaries();
    value = Func_08043cd8(0, 3);
    if (value == -1) {
      result = value;
    } else {
      Message_OpenWindow((s32)&MsgEraseConfirm, ERASE_CONFIRM_X, 1, 2);
      while (Message_AllTyped() == 0)
        wait(1);
      if (Menu_RunConfirmSelection(1, 0, 3, 1) != 0) {
        Message_CloseFinishedWindows();
      } else {
        Message_CloseFinishedWindows();
        token = (s32)SaveProgress_Open(1);
        found = Func_08042ff0(value);
        SaveProgress_Close((struct Window *)token);
        if (found != 0) {
          FieldMessage_Show((s32)&MsgEraseFailed, 1);
          result = -4;
        } else {
          FieldMessage_Show((s32)&MsgFileErased, 1);
        }
      }
    }
  }
  Save_ReleaseWork();
  return result;
}

extern u32 gVBlankCount;

/// Updates four RGB555 colors with an eight-VBlank phase offset between them.
/// For s = sin / 16384, red is 22 + 2s, green 16 + s, and blue 20 + s.
void PaletteGlow_UpdateSine(void) {
  s32 *vblankCount;
  volatile u16 *pal;
  s32 col;
  s32 s;
  s32 i;

  vblankCount = (s32 *)&gVBlankCount;
  pal = (volatile u16 *)0x050001D0; // Background palette 14, color 8.
  i = 0;
  do {
#ifdef UBFIX
    // The unsigned VBlank count wraps without overflowing signed arithmetic.
    s = Trig_Sin((((u32)*vblankCount + i * 8U) * 0x300U) & 0xffffU);
#else
    s = Trig_Sin((*vblankCount + i * 8) * 0x300);
#endif
    col = s / 16384;
    {
#ifdef UBFIX
      // The sine can be negative; shift its bits as an unsigned value.
      u32 r = (u32)col << 1;
#else
      u32 r = col << 1;
#endif
      u32 g = col;

      r += 22;
      g += 16;
      col += 20;
      Io_Write16(RGB(r, g, col), pal++);
    }
  } while (++i <= 3);
}

void PaletteGlow_Start(void) {
  Task_Add((s32)PaletteGlow_UpdateSine, TASK_PASS_VBLANK);
}

void PaletteGlow_Stop(void) { Task_Remove((u32)((s32)PaletteGlow_UpdateSine)); }

void Window_FillTilemapRect(struct Window *window, s32 x, s32 y, s32 width,
                            s32 height) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 sum;

  sum = x + (s16)window->x;
  x = sum + 1;
  sum = y + (s16)window->y;
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
    s32 yOffset = y << 6;
    s32 xOffset = x << 1;
    s32 offset = yOffset + ((s32)work + xOffset);
    do {
      s32 remaining = width;
      u16 *cell = (u16 *)(offset + 8);
      if (remaining != 0) {
        do {
          remaining--;
          *cell++ = 0xE006;
        } while (remaining != 0);
      }
      height--;
      offset += 64;
    } while (height != 0);
    work->dirty = 1;
  }
}

struct ObjectRowWork {
  u8 unk000[0xf8];
  void *icons[4];
  u8 unk108[0x10];
  s16 x[8];
  s16 y[8];
  s32 scale[4];
};

void Animation_ApplyChildArgumentFar(void *, s32);

void Func_08043a64(struct RenderInput *origin, s32 x, s32 y,
                   struct SaveSummary *descriptor) {
  struct ObjectRowWork *state = Ram_HeapSlots->menuRuntime;
  s32 i;

  for (i = 0; i < 4 && descriptor->party[i] != -1; i++) {
    struct FieldSprite *object =
        (struct FieldSprite *)ResourceObject_CreateFar(descriptor->party[i]);

    if (object != 0) {
      Animation_ApplyChildArgumentFar(object, 1);
      object->nextFrame = 0;
      object->priority = 0;
    }

    state->icons[i] = object;
    state->x[i] = (x + (s16)origin->x + i * 3) * 8 + 0x10;
    state->y[i] = (y + (s16)origin->y) * 8 + 0x10;
    state->scale[i] = 0x10000;
  }

  Task_Add(Func_08043b70, TASK_PASS_VBLANK);
}

void Menu_ClearFirstObjectRowAndScheduleUpdate(void) {
  u8 *base = (u8 *)Ram_HeapSlots->menuRuntime;
  s32 offset;
  s32 zero;
  s32 count;

  Task_Remove((s32)Func_08043b70);
  zero = 0;
  offset = 248;
  count = 3;
  do {
    void *entry = *(void **)(offset + (unsigned int)base);

    if (entry != 0) {
      ResourceObject_ReleaseFar(entry);
      *(s32 *)(offset + (unsigned int)base) = zero;
    }
    count--;
    offset += 4;
  } while (count >= 0);
}

struct ObjectRowState {
  u8 unk000[248];
  void *objects[4];
  u8 unk108[16];
  s16 x[8];
  s16 y[8];
  s32 scale[4];
};

void Func_08043b70(void) {
  struct ObjectRowState *state =
      (struct ObjectRowState *)Ram_HeapSlots->menuRuntime;
  s32 source[2];
  s32 request[4];
  s32 index = 0;
  s32 handle;

  while (1) {
    handle = (s32)state->objects[index];
    if (handle != 0) {
      source[0] = state->scale[index];
      source[1] = state->scale[index];
      request[0] = state->x[index] << 16;
      request[1] = 0x01f40000;
      request[2] = (state->y[index] << 16) + 0x01f40000;
      request[3] = 0;
      Render_ApplyProjectedPlacementFar((struct ScaledSprite *)handle, request,
                                        source, 0x4000);
    }
    index += 1;
    if (index > 3) {
      break;
    }
  }
}

// Saves: draw the recorded Djinn counts in a row, one per element. The first
// section was written as draw_four_numbers_in_row.c.

/// The portion of the file summary (SaveSummary) read here.
struct NumberRow {
  u8 unk00[0x28];
  s8 values[4]; ///< Djinn count for each element.
};

/// Draws four two-digit numbers 24 pixels apart at y = 16.
/// win: destination; nothing is drawn if zero.
void FileList_DrawDjinnCounts(struct RenderInput *win, struct NumberRow *row) {
  s32 i;

  if (win != 0) {
    for (i = 0; i < 4; i++)
      Text_DrawNumberAtOffset(row->values[i], 2, win, i * 24, 16);
  }
}

//--------------------------------------------------------------------------

// draw_character_summary.c

// Saves: draw the recorded name, level, class, coins, and play time.

//--------------------------------------------------------------------------

#define Text_DrawStringAtOffset                                                \
  ((void (*)(void *, s32, s32, s32))Text_DrawStringAtOffset)
#define Text_DrawNumberAtOffset                                                \
  ((void (*)(s32, s32, s32, s32, s32))Text_DrawNumberAtOffset)
#define Text_DrawCharacterAtOffset                                             \
  ((void (*)(s32, s32, s32, s32))Text_DrawCharacterAtOffset)

// Edition-specific x positions of the level and coin labels, in pixels.
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

/// win: destination; do nothing if zero.
/// rec: the file summary (SaveSummary).
///      +16 name, +28 level, +29 class, +32 play time, +36 coins.
void StatusMenu_DrawCharacterSummary(s32 win, u8 *rec) {
  u32 buf[4]; // Formatted play time.
  s32 y;

  if (win != 0) {
    WindowSprite_PrepareForRedraw((struct Window *)win);
    Window_DrawDividerLine((struct Window *)win, 0, 4, 13, 4);
    Text_DrawStringAtOffset(rec + 16, win, 0, 0);
    y = 0;
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
    Text_DrawStringAtOffset(gStatusMenu_LevelLetterString, win, SUMMARY_LEVEL_X,
                            0);
#else
    Text_DrawStringInWindow((u8 *)gStatusMenu_LevelLetterString, win, 72, 0);
#endif
    Text_DrawNumberAtOffset(rec[28], 2, win, 80, y);
    Text_DrawCharacterAtOffset(rec[29] + (s32)MsgClassName, win, 0, 16);
#if (LANGUAGE == LANGUAGE_JAPANESE)
    y = 32;
    Text_DrawNumberInWindow(*(s32 *)(rec + 36), 6, win, 24, y);
    Text_DrawCharacterAtOffset((s32)MsgCoins, win, 72, 32);
    Text_DrawCharacterAtOffset((s32)MsgStatusLabel, win, 0, 48);
    Text_DrawStringInWindow(Text_FormatPlayTime(*(s32 *)(rec + 32), (u8 *)buf),
                            win, 48, 48);
#else
    Text_DrawCharacterAtOffset((s32)MsgStatusLabel, win, 0, 32);
    Text_DrawStringInWindow(Text_FormatPlayTime(*(s32 *)(rec + 32), (u8 *)buf),
                            win, 48, 40);
    y = 48;
    Text_DrawNumberInWindow(*(s32 *)(rec + 36), 6, win, 0, y);
#if (LANGUAGE == LANGUAGE_FRENCH)
    // French uses the singular form for zero or one coin.
    if (*(u32 *)(rec + 36) > 1)
      Text_DrawCharacterAtOffset((s32)MsgCoins, win, SUMMARY_COINS_X, 48);
    else
      Text_DrawCharacterAtOffset((s32)MsgPiece, win, SUMMARY_COINS_X, 48);
#else
    Text_DrawCharacterAtOffset((s32)MsgCoins, win, SUMMARY_COINS_X, 48);
#endif
#endif
  }
}

// Where the leader's name and the area name start in a file's row of the file
// list, in pixels: the European languages start both farther left.
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
#define FILE_LIST_LEADER_X 12
#define FILE_LIST_AREA_X 72
#else
#define FILE_LIST_LEADER_X 6
#define FILE_LIST_AREA_X 62
#endif

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Func_080f8070(void);
void Func_080f8078(void);
void Func_080f8080(s32 shown);
void Func_080f8090(s32 index, s32 x, s32 y);
void wait(s32 frames);
void Func_0803f9c0(s32 glow, s32 sub);
void Sound_PlayFar(s32 cue);

// Messages 0 to 4 and 3672, the first area name.
extern u8 MsgSaveNoData[];
extern u8 MsgSaveContinue[];
extern u8 MsgSaveNotCleared[];
extern u8 MsgSaveCorrupted[];
extern u8 MsgSavePostReunion[];
extern u8 MsgDebugAreaName[];

#define SAVE_FILE_COUNT 3
#define SAVE_MENU_NO_FILE -2

s32 Func_08043cd8(s32 selectedFile, s32 menuMode) {
  struct Window *fileList;
  struct SaveWorkspace *save;
  struct Window *summaryWindow;
  struct Window *partyWindow;
  struct Window *djinnWindow;
  struct SaveSummary *summary;
  void *modePicture;
  s32 summaryDirty;
  u32 buildStamp;
  void *spriteWork;
  s32 result;
  s32 i;
  s32 y;

  spriteWork = HeapSlot_Alloc(220, 0xcec);
  save = Ram_HeapSlots->serialWork;
  summaryWindow = NULL;
  partyWindow = NULL;
  djinnWindow = NULL;
  summaryDirty = 1;
  buildStamp = BuildStamp_GetTimeFar();
  if (selectedFile < 0)
    selectedFile = 0;
  if (menuMode == 1) {
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
  } else if (menuMode == 4) {
    for (i = 0;
         i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
                                 save->summary[selectedFile].flagCount == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (menuMode == 5) {
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
  } else if (menuMode == 6) {
    for (i = 0;
         i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
                                 save->summary[selectedFile].hasFlag34 != 0);
         i++) {
      selectedFile++;
      if (selectedFile > 2)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (menuMode != 0) {
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

  Dma_Fill32(0, spriteWork, 0xcec);
  PaletteGlow_Start();
  fileList = Window_Create(1, 2, 28, 7, 2);
  for (i = 0, summary = save->summary; i < SAVE_FILE_COUNT; i++, summary++) {
    if (save->summary[i].level == 0) {
      Text_DrawMessage((s32)MsgSaveNoData, (s32)fileList, 10, i * 16);
    } else if (save->summary[i].frames < buildStamp) {
      Text_DrawMessage((s32)MsgSaveContinue, (s32)fileList, 10, i * 16);
    } else if (save->summary[i].playTime != *(u32 *)save->summary[i].unk38) {
      Text_DrawMessage((s32)MsgSaveCorrupted, (s32)fileList, 10, i * 16);
    } else if (menuMode == 5 && save->summary[i].sendFlag == 0) {
      Text_DrawMessage((s32)MsgSaveNotCleared, (s32)fileList, 10, i * 16);
    } else if (menuMode == 6 && save->summary[i].hasFlag34 != 0) {
      Text_DrawMessage((s32)MsgSavePostReunion, (s32)fileList, 10, i * 16);
    } else {
      y = i * 16;
      Text_DrawString(summary->name, (s32)fileList, FILE_LIST_LEADER_X, y);
      Text_DrawMessage(save->summary[i].area + (s32)MsgDebugAreaName,
                       (s32)fileList, FILE_LIST_AREA_X, y);
    }
  }
  modePicture = RenderResource_CreatePair(
      menuMode, (struct RenderInput *)fileList, 72, -24);
  Func_080f8070();
  for (i = 0, y = 144; i < 4; i++, y += 24)
    Func_080f8090(i, y, 128);
  Func_080f8080(0);
  wait(1);

  while (1) {
    if (summaryDirty != 0) {
      summaryDirty = 0;
      if (save->summary[selectedFile].level != 0) {
        Func_0803f9c0(save->summary[selectedFile].paletteGlow,
                      save->summary[selectedFile].paletteGlowSub);
        if (summaryWindow == NULL)
          summaryWindow = Window_Create(1, 10, 14, 9, 2);
        StatusMenu_DrawCharacterSummary((s32)summaryWindow,
                                        (u8 *)&save->summary[selectedFile]);
        wait(1);
        if (partyWindow == NULL)
          partyWindow = Window_Create(16, 10, 13, 3, 2);
        Menu_ClearFirstObjectRowAndScheduleUpdate();
        Func_08043a64((struct RenderInput *)partyWindow, 0, 0,
                      &save->summary[selectedFile]);
        wait(1);
        if ((s8)save->summary[selectedFile].djinn[0] +
                (s8)save->summary[selectedFile].djinn[1] +
                (s8)save->summary[selectedFile].djinn[2] +
                (s8)save->summary[selectedFile].djinn[3] !=
            0) {
          if (djinnWindow == NULL)
            djinnWindow = Window_Create(16, 14, 13, 5, 2);
          FileList_DrawDjinnCounts(
              (struct RenderInput *)djinnWindow,
              (struct NumberRow *)&save->summary[selectedFile]);
          Func_080f8080(1);
        } else {
          Func_080f8080(0);
          Window_Close(djinnWindow, 2);
          djinnWindow = NULL;
        }
      } else {
        Func_0803f9c0(gMainRoutineWork.unk220[5], gMainRoutineWork.unk220[6]);
        Func_080f8080(0);
        Menu_ClearFirstObjectRowAndScheduleUpdate();
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
      Window_FillTilemapRect(fileList, 0, selectedFile * 2, 26, 1);
    }
    Func_08045018(modePicture);
    wait(1);
    if (gInput.repeat & 0x40) {
      Sound_PlayFar(111);
      summaryDirty = 1;
      do {
        selectedFile = (selectedFile + SAVE_FILE_COUNT - 1) % SAVE_FILE_COUNT;
        if (menuMode == 0)
          break;
      } while (save->summary[selectedFile].level == 0 ||
               (menuMode == 1 && save->summary[selectedFile].sendFlag != 0) ||
               (menuMode == 4 && save->summary[selectedFile].flagCount == 0) ||
               (menuMode == 5 && save->summary[selectedFile].sendFlag == 0) ||
               (menuMode == 6 && save->summary[selectedFile].hasFlag34 != 0));
    } else if (gInput.repeat & 0x80) {
      Sound_PlayFar(111);
      summaryDirty = 1;
      do {
        selectedFile = (selectedFile + SAVE_FILE_COUNT + 1) % SAVE_FILE_COUNT;
        if (menuMode == 0)
          break;
      } while (save->summary[selectedFile].level == 0 ||
               (menuMode == 1 && save->summary[selectedFile].sendFlag != 0) ||
               (menuMode == 4 && save->summary[selectedFile].flagCount == 0) ||
               (menuMode == 5 && save->summary[selectedFile].sendFlag == 0) ||
               (menuMode == 6 && save->summary[selectedFile].hasFlag34 != 0));
    } else if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      result = -1;
      break;
    } else if (gInput.pressed & 1) {
      Sound_PlayFar(112);
      result = selectedFile;
      break;
    }
  }
  Func_080f8078();
  Menu_ClearFirstObjectRowAndScheduleUpdate();
  Window_Close(djinnWindow, 2);
  Window_Close(partyWindow, 2);
  Window_Close(summaryWindow, 2);
  Window_Close(fileList, 2);
  PaletteGlow_Stop();
  HeapSlot_Release(220);
  Func_0803f9c0(gMainRoutineWork.unk220[5], gMainRoutineWork.unk220[6]);
  wait(1);
done:
  return result;
}

#undef SAVING_X
#undef ERASING_X
#undef COPYING_X
#undef DMA_STOP
#undef SAVE_GAME_WORDS
#undef ERASE_CONFIRM_X
#undef Text_DrawStringAtOffset
#undef Text_DrawNumberAtOffset
#undef Text_DrawCharacterAtOffset
#undef SUMMARY_LEVEL_X
#undef SUMMARY_COINS_X
#undef FILE_LIST_LEADER_X
#undef FILE_LIST_AREA_X
#undef SAVE_FILE_COUNT
#undef SAVE_MENU_NO_FILE
