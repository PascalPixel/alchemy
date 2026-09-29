/* Draft, not exact: complete 160-byte owner [0801faa8, 0801fb48).
   Positive error magnitudes and shared negation recover the missing movs 9
   and shared negs. A narrow result adds two sign-extension instructions
   and gives 164 bytes.
   2026-09-29: callees, save globals and the two message symbols the text
   build defines carry the build's names; the IWRAM copier is reached from
   the bank's first routine as the item menu reaches it. The copy source,
   0x020004e4, has no EWRAM label yet: gSaveStamp below needs one in
   recon/tbs/sym_ewram.s (inside gPlayerObjectId's span) before adoption.
   Allocation: with one result variable (7 references over 84 insns) the
   result outranks the slot address (4 over 54, doubled for its constant
   equivalence) and takes r6. Splitting the positive error code into its
   own variable gives the reference's slot r6 / result r7, and alchemy
   permute scores 35: 15 for the error code in r6 instead of r7, 20 for the
   missing gSaveStamp label. Writing the codes as -9/-2/-3 also gives the
   reference allocation, but move2add then rewrites -9 as subs r7, #9 from
   the known zero. Ten minutes of permutation from here: none below 35. */
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
    s32 error;

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
