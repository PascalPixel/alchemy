#include "TYPES.H"
#include "RUNTIME_INTERFACES.H"

s32 SaveState_InitializeWorkspace(void);
s32 SaveState_WriteRecord(s32, void *);
void UiText_ShowPositionedMessageAndWait(s32, s32);
void SaveState_BuildSummaryHeader(void);
extern char gSaveBuffer;
extern char MsgNoBackupMemory;
extern char MsgSaveFailed;

s16 SaveState_WriteCurrentSlotPair(void)
{
    s16 value;
    s16 result;
    s32 found;
    s32 error;

    result = 0;
    value = *(s16 *)0x02002004;
    if (value != -1) {
        found = SaveState_InitializeWorkspace();
        if (found != 0) {
            UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
            error = 9;
            goto set_error;
        }
        SaveState_BuildSummaryHeader();
        {
            void *base = &gSaveBuffer;
            s32 next;

            found = SaveState_WriteRecord(*(s16 *)0x02002004, base);
            next = *(s16 *)0x02002004;
            base = (char *)base + 0x1000;
            found |= SaveState_WriteRecord(next + 3, base);
            if (found != 0) {
                UiText_ShowPositionedMessageAndWait((s32)&MsgSaveFailed, 1);
                error = 3;
set_error:
                result = 0 - error;
            }
        }
        SaveState_ReleaseWorkspace();
        value = result;
    }
    return value;
}



s32 SaveState_WriteSlotPair(s32 arg0)
{
    s32 found;
    s16 result = 0;

    found = SaveState_InitializeWorkspace();
    if (found != 0) {
        UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
        result = -9;
    } else {
        void *base = &gSaveBuffer;

        found = SaveState_WriteRecord(arg0, base);
        base = (char *)base + 0x1000;
        found |= SaveState_WriteRecord(arg0 + 3, base);
        if (found != 0) {
            UiText_ShowPositionedMessageAndWait((s32)&MsgSaveFailed, 1);
            result = -3;
        }
    }
    SaveState_ReleaseWorkspace();
    return result;
}
