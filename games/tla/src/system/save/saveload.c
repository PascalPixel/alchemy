#include "types.h"
#include "ram_buffer.h"
#include "save_state.h"
#include "party_state.h"
#include "frame.h"
#include "system.h"
#include "menu.h"
#include "edition.h"

struct SaveGameWords {
  u32 buildStamp;
  u32 playTime;
};

#define SAVE_GAME_WORDS ((struct SaveGameWords *)&gPartyState)

extern u8 MsgNoBackupMemory;
extern u8 MsgSaveFailed;
extern u8 MsgLoadFailed;

extern u8 gData_02000000[];
extern s16 gData_020036d0;

s32 Func_0801596c(void);
s32 SaveState_LoadSummaryRecords(void);
void SaveState_ReleaseWorkspace(void);
s32 SaveState_ReadRecordPayload(s32 slot, void *buffer);
s32 Func_08043cd8(s32 slot, s32 mode);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);

// Select a file and load it.
s32 SaveState_SelectAndLoad(s32 mode) {
  s32 result = 0;
  s32 slot;

  if (Func_0801596c() != 0) {
    UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
    result = -9;
  } else {
    SaveState_LoadSummaryRecords();
    slot = Func_08043cd8(gData_020036d0, mode);
    if (slot == -1) {
      result = slot;
    } else if (SaveState_ReadRecordPayload(slot, gData_02000000) != 0) {
      UiText_ShowPositionedMessageAndWait((s32)&MsgLoadFailed, 1);
      result = -2;
    } else {
      gFrameWaitCount = SAVE_GAME_WORDS->playTime;
      gAutoSleepEnabled = gPartyState.unk244[6];
      gIdleFrameCount = 0;
      gData_020036d0 = slot;
    }
  }
  SaveState_ReleaseWorkspace();
  return result;
}

extern u8 MsgEraseFailed;
extern u8 MsgFileCopied;
u32 SaveState_FindFreeSummarySlot(void);
s32 SaveProgress_Open(s32 arg0);
void SaveProgress_Close(s32 arg0);
s32 Func_08042f10(s32 slot, void *buffer);

// Select a file and copy it to a free slot.
s32 SaveState_CopySlot(void) {
  s32 result = 0;
  s32 value;
  s32 token;

  if (Func_0801596c() != 0) {
    UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
    result = -9;
  } else {
    SaveState_LoadSummaryRecords();
    value = Func_08043cd8(0, 2);
    if (value == -1) {
      result = value;
    } else {
      void *buffer = gData_02000000;

      value = SaveState_ReadRecordPayload(value, buffer);
      if (value != 0) {
        UiText_ShowPositionedMessageAndWait((s32)&MsgLoadFailed, 1);
        result = -2;
      } else {
        value = SaveState_FindFreeSummarySlot();
        if (value == 999) {
          UiText_ShowPositionedMessageAndWait((s32)&MsgEraseFailed, 1);
          result = -5;
        } else {
          token = SaveProgress_Open(2);
          value = Func_08042f10(value, buffer);
          SaveProgress_Close(token);
          if (value != 0) {
            UiText_ShowPositionedMessageAndWait((s32)&MsgEraseFailed, 1);
            result = -3;
          } else {
            UiText_ShowPositionedMessageAndWait((s32)&MsgFileCopied, 1);
          }
        }
      }
    }
  }
  SaveState_ReleaseWorkspace();
  return result;
}

extern u8 MsgEraseConfirm;
extern u8 MsgFileErased;
s32 Func_08042ff0(s32 slot);
s32 UiText_OpenMessageWindow(s32 message, s32 x, s32 y, s32 style);
s32 UiWork_IsComplete(void);
void UiWork_FinalizePendingCore(void);

#if (LANGUAGE == LANGUAGE_FRENCH)
#define ERASE_CONFIRM_X 4 // The French confirmation window is farther left.
#else
#define ERASE_CONFIRM_X 8
#endif

// Select a file and erase it.
s32 SaveState_DeleteSelectedSlot(void) {
  s32 result = 0;
  s32 value;
  s32 token;
  s32 found;

  if (Func_0801596c() != 0) {
    UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
    result = -9;
  } else {
    SaveState_LoadSummaryRecords();
    value = Func_08043cd8(0, 3);
    if (value == -1) {
      result = value;
    } else {
      UiText_OpenMessageWindow((s32)&MsgEraseConfirm, ERASE_CONFIRM_X, 1, 2);
      while (UiWork_IsComplete() == 0)
        WaitFrames(1);
      if (Menu_RunConfirmSelection(1, 0, 3, 1) != 0) {
        UiWork_FinalizePendingCore();
      } else {
        UiWork_FinalizePendingCore();
        token = SaveProgress_Open(1);
        found = Func_08042ff0(value);
        SaveProgress_Close(token);
        if (found != 0) {
          UiText_ShowPositionedMessageAndWait((s32)&MsgEraseFailed, 1);
          result = -4;
        } else {
          UiText_ShowPositionedMessageAndWait((s32)&MsgFileErased, 1);
        }
      }
    }
  }
  SaveState_ReleaseWorkspace();
  return result;
}
