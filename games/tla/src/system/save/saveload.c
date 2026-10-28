#include "types.h"
#include "ram_buffer.h"
#include "save.h"
#include "adventure.h"
#include "frame.h"
#include "system.h"
#include "menu.h"
#include "edition.h"

struct SaveGameWords {
  u32 buildStamp;
  u32 playTime;
};

#define SAVE_GAME_WORDS ((struct SaveGameWords *)&gAdventure)

extern u8 MsgNoBackupMemory;
extern u8 MsgSaveFailed;
extern u8 MsgLoadFailed;

extern u8 gData_02000000[];
extern s16 gData_020036d0;

s32 Func_0801596c(void);
s32 Save_LoadFileSummaries(void);
void Save_ReleaseWork(void);
s32 Save_ReadPart(s32 partId, void *buffer);
s32 Func_08043cd8(s32 file, s32 mode);
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
      gAutoSleepEnabled = gAdventure.unk244[6];
      gIdleFrameCount = 0;
      gData_020036d0 = file;
    }
  }
  Save_ReleaseWork();
  return result;
}

extern u8 MsgEraseFailed;
extern u8 MsgFileCopied;
u32 Save_FindFreeFile(void);
s32 SaveProgress_Open(s32 arg0);
void SaveProgress_Close(s32 arg0);
s32 Func_08042f10(s32 partId, void *buffer);

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
          token = SaveProgress_Open(2);
          value = Func_08042f10(value, buffer);
          SaveProgress_Close(token);
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

extern u8 MsgEraseConfirm;
extern u8 MsgFileErased;
s32 Func_08042ff0(s32 partId);
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
        token = SaveProgress_Open(1);
        found = Func_08042ff0(value);
        SaveProgress_Close(token);
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
