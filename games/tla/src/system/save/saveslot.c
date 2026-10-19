#include "types.h"
#include "ram_buffer.h"
#include "save_state.h"
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
s32 SaveState_LoadSummaryRecords(void);
void SaveState_ReleaseWorkspace(void);
s32 SaveState_BuildSummaryHeader(void);
s32 Func_08042f10(s32 slot, void *buffer);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void Audio_PlayCue(s32 cue);
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
/// Spanish and Italian far-call tables name the entry to the same routine
/// differently.
void Object_BuildRecordSnapshotFar(void);
#define ObjectTable_SnapshotFar Object_BuildRecordSnapshotFar
#else
void ObjectTable_SnapshotFar(void);
#endif
s32 UiWork_IsComplete(void);
void UiWork_FinalizePendingCore(void);

/// Save to the file specified by index.
s32 SaveState_SaveToSlot(u32 slot) {
  s32 result = 0;
  s32 found;

  if (slot > 2)
    slot = 2;
  if (Func_0801596c() != 0) {
    UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
    result = -9;
  } else {
    SaveState_LoadSummaryRecords();
    gData_020036d0 = slot;
    Audio_PlayCue(85);
    UiText_ShowPositionedMessageAndWait((s32)&MsgSaving, 13);
    SaveState_BuildSummaryHeader();
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
  SaveState_ReleaseWorkspace();
  return result;
}
