#include "types.h"
#include "ram_buffer.h"
#include "save_state.h"
#include "mem.h"

extern u8 MsgNoBackupMemory;
extern u8 MsgSaveFailed;

extern u8 gData_02000000[];
extern u8 gData_02000504[];
extern s16 gData_020036d0;

s32 Func_0801596c(void);
s32 SaveState_LoadSummaryRecords(void);
void SaveState_ReleaseWorkspace(void);
s32 SaveState_ReadRecordPayload(s32 slot, void *buffer);
s32 SaveProgress_Open(s32 arg0);
void SaveProgress_Close(s32 arg0);
s32 Func_08042f10(s32 slot, void *buffer);
s32 Func_08043cd8(s32 slot, s32 mode);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);

s32 SaveState_WriteSlot(s32 slot)
{
    s32 result = 0;

    if (Func_0801596c() != 0) {
        UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
        result = -9;
    } else if (Func_08042f10(slot, gData_02000000) != 0) {
        UiText_ShowPositionedMessageAndWait((s32)&MsgSaveFailed, 1);
        result = -3;
    }
    SaveState_ReleaseWorkspace();
    return result;
}
