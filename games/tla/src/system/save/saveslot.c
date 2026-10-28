#include "types.h"
#include "ram_buffer.h"
#include "save.h"
#include "mem.h"
#include "system.h"
#include "edition.h"

extern u8 MsgNoBackupMemory;
extern u8 MsgSaveFailed;
extern u8 MsgGameSaved;
extern u8 MsgSaving;

extern u8 gData_02000000[];
extern s16 gData_020036d0;

s32 Func_0801596c(void);
s32 Save_LoadFileSummaries(void);
void Save_ReleaseWork(void);
s32 Save_BuildFileSummary(void);
s32 Func_08042f10(s32 slot, void *buffer);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void Sound_PlayFar(s32 cue);
void ObjectTable_SnapshotFar(void);
s32 UiWork_IsComplete(void);
void UiWork_FinalizePendingCore(void);

/// Saves the game to slot (at most 2).
s32 Save_ToFile(u32 slot) {
  s32 result = 0;
  s32 found;

  if (slot > 2)
    slot = 2;
  if (Func_0801596c() != 0) {
    UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
    result = -9;
  } else {
    Save_LoadFileSummaries();
    gData_020036d0 = slot;
    Sound_PlayFar(85);
    UiText_ShowPositionedMessageAndWait((s32)&MsgSaving, 13);
    Save_BuildFileSummary();
    ObjectTable_SnapshotFar();
    found = Func_08042f10(slot, gData_02000000);
    while (UiWork_IsComplete() == 0)
      WaitFrames(1);
    UiWork_FinalizePendingCore();
    if (found != 0) {
      UiText_ShowPositionedMessageAndWait((s32)&MsgSaveFailed, 1);
      result = -3;
    } else {
      UiText_ShowPositionedMessageAndWait((s32)&MsgGameSaved, 9);
    }
  }
  Save_ReleaseWork();
  return result;
}

extern u8 MsgOverwriteConfirm;

s32 SaveProgress_Open(s32 arg0);
void SaveProgress_Close(s32 arg0);
s32 Func_08043cd8(s32 slot, s32 mode);
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
    UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
    result = -9;
  } else {
    Save_LoadFileSummaries();
    slot = Func_08043cd8(gData_020036d0, 0);
    if (slot == -1) {
      result = slot;
    } else {
      work = Ram_HeapSlots->serialWork;
      if (work->summary[slot].level != 0) {
        UiText_ShowPositionedMessageAndWait((s32)&MsgOverwriteConfirm, 13);
        while (UiWork_IsComplete() == 0)
          WaitFrames(1);
        if (Menu_RunConfirmSelection(1, 0, 0, 1) != 0) {
          UiWork_FinalizePendingCore();
          goto done;
        }
        UiWork_FinalizePendingCore();
      }
      gData_020036d0 = slot;
      token = SaveProgress_Open(0);
      Save_BuildFileSummary();
      ObjectTable_SnapshotFar();
      found = Func_08042f10(slot, gData_02000000);
      SaveProgress_Close(token);
      UiWork_FinalizePendingCore();
      if (found != 0) {
        UiText_ShowPositionedMessageAndWait((s32)&MsgSaveFailed, 1);
        result = -3;
      } else {
        UiText_ShowPositionedMessageAndWait((s32)&MsgGameSaved, 9);
      }
    }
  }
done:
  Save_ReleaseWork();
  return result;
}
