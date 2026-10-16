/* Near miss, differ 2 of 82: the native starts the level offset (movs r1, #192), loads the workspace
 * (ldr r2, [r3]) and then finishes the offset (lsls r1, r1, #6); behind the barrier on the workspace the
 * load comes before the movs. Without the barrier the whole offset is built before the load (differ 6).
 * Measured: the offset half-built behind an in-out barrier changes the register allocation (differ 22);
 * a constant 192 named in the barrier is folded back (differ 28). */
#include "types.h"
#include "ram_buffer.h"
#include "save_state.h"
#include "mem.h"
#include "system.h"
#include "menu.h"

extern u8 MsgNoBackupMemory;
extern u8 MsgSaveFailed;
extern u8 MsgOverwriteConfirm;
extern u8 MsgGameSaved;
extern u8 MsgSaving;

extern u8 gData_02000000[];
extern s16 gData_020036d0;

s32 Func_0801596c(void);
s32 SaveState_LoadSummaryRecords(void);
void SaveState_ReleaseWorkspace(void);
s32 SaveState_BuildSummaryHeader(void);
s32 SaveProgress_Open(s32 arg0);
void SaveProgress_Close(s32 arg0);
s32 Func_08042f10(s32 slot, void *buffer);
s32 Func_08043cd8(s32 slot, s32 mode);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void ObjectTable_SnapshotFar(void);
s32 UiWork_IsComplete(void);
void UiWork_FinalizePendingCore(void);

// Select a file and save.
s32 SaveState_SelectAndSave(void)
{
    s32 result = 0;
    s32 slot;
    s32 found;
    s32 token;
    struct SaveWorkspace *work;

    if (Func_0801596c() != 0) {
        UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
        result = -9;
    } else {
        SaveState_LoadSummaryRecords();
        slot = Func_08043cd8(gData_020036d0, 0);
        if (slot == -1) {
            result = slot;
        } else {
            work = Ram_HeapSlots->serialWork;
            /* FAKEMATCH: see the header */
            __asm__("" : : "r"(work));
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
            SaveState_BuildSummaryHeader();
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
    SaveState_ReleaseWorkspace();
    return result;
}
