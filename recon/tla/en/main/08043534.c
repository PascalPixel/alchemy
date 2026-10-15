#include "types.h"
#include "ram_buffer.h"
#include "save_state.h"
#include "mem.h"
#include "iwram_call.h"

extern u8 MsgNoBackupMemory;
extern u8 MsgSaveFailed;
extern u8 MsgOverwriteConfirm;
extern u8 MsgGameSaved;
extern u8 MsgSaving;

extern u8 Data_02000000[];
extern u8 Data_02000504[];
extern s16 Data_020036d0;

s32 Func_0801596c(void);
s32 SaveState_LoadSummaryRecords(void);
void SaveState_ReleaseWorkspace(void);
s32 SaveState_ReadRecordPayload(s32 slot, void *buffer);
s32 SaveProgress_Open(s32 arg0);
void SaveProgress_Close(s32 arg0);
s32 Func_08042f10(s32 slot, void *buffer);
s32 Func_08043cd8(s32 slot, s32 mode);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);

s32 SaveState_ProcessSelectedSlot(void)
{
    void *buffer;
    s32 value;
    s32 result;
    s32 found;
    s32 error;
    s32 token;

    result = 0;
    buffer = Runtime_BumpAllocateAlternatePool(0x3000);
    value = Data_020036d0;
    if (value != -1) {
        found = Func_0801596c();
        if (found != 0) {
            UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
            error = 9;
            goto negate;
        } else {
            u8 *dst;

            found = SaveState_ReadRecordPayload(Data_020036d0, buffer);
            if (found != 0) {
                UiText_ShowPositionedMessageAndWait((s32)&MsgSaveFailed, 1);
                result = -2;
            }
            dst = (u8 *)buffer + (s32)Data_02000504;
            dst -= (s32)Data_02000000;
            Iwram_CopyWords(dst, Data_02000504, 16);
            token = SaveProgress_Open(0);
            found = Func_08042f10(Data_020036d0, buffer);
            SaveProgress_Close(token);
            if (found != 0) {
                UiText_ShowPositionedMessageAndWait((s32)&MsgSaveFailed, 1);
                error = 3;
negate:
                result = -error;
            }
        }
        SaveState_ReleaseWorkspace();
        MemFree(buffer);
        value = result;
    }
    return value;
}
