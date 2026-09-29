/* SaveState_ProcessSelectedSlot: rewrite the selected save slot with the
   current save stamp, reporting a missing backup chip or a failed read or
   write as a negative code. */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RUNTIME_MEM.H"

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);

extern s16 gSaveSlot;
extern u8 gSaveBuffer[];
extern u8 gSaveStamp[];
extern u8 MsgNoBackupMemory;
extern u8 MsgSaveFailed;

void *Runtime_BumpAllocateAlternatePool(s32 size);
s32 SaveState_InitializeWorkspace(void);
u32 SaveState_ReadRecordPayload(s32 slot, void *buffer);
s32 SaveState_WriteRecord(s32 slot, void *buffer);
void SaveState_ReleaseWorkspace(void);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 mode);

s32 SaveState_ProcessSelectedSlot(void)
{
    void *buffer;
    s32 value;
    s32 result;
    s32 found;
    /* FAKEMATCH: keeps the error code in r7 */
    register s32 error asm("r7");

    buffer = Runtime_BumpAllocateAlternatePool(0x1000);
    result = 0;
    value = gSaveSlot;
    if (value != -1) {
        found = SaveState_InitializeWorkspace();
        if (found != 0) {
            error = 9;
            UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
            goto negate;

        } else {
            u8 *dst;

            found = SaveState_ReadRecordPayload(gSaveSlot, buffer);
            if (found != 0) {
                UiText_ShowPositionedMessageAndWait((s32)&MsgSaveFailed, 1);
                result = -2;
            }
            dst = (u8 *)buffer + (s32)gSaveStamp;
            dst -= (s32)gSaveBuffer;
            ((WordCopyFn)(IwramIrqMain + Iwram_CopyWordsOffset))(dst, gSaveStamp, 16);
            found = SaveState_WriteRecord(gSaveSlot, buffer);
            if (found != 0) {
                UiText_ShowPositionedMessageAndWait((s32)&MsgSaveFailed, 1);
                error = 3;
negate:
                result = -error;
            }
        }
        SaveState_ReleaseWorkspace();
        Sys_Free(buffer);
        value = result;
    }
    return value;
}
