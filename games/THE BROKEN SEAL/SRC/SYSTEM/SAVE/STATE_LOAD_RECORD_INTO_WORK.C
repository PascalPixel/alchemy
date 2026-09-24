#include "TYPES.H"
#include "RUNTIME_INTERFACES.H"

struct State_080208e4 {
    u8 padding0[4];
    s32 value;
};

s32 SaveState_InitializeWorkspace(void);
void SaveState_LoadSummaryRecords(void);
void UiText_ShowPositionedMessageAndWait(s32, s32);
s32 SaveMenu_SelectSlot(s16, s32);
s32 SaveState_ReadRecordPayload(s32, void *);

extern char MsgNoBackupMemory;
extern char Value_0000000c;
extern char gSaveBuffer;
extern s16 gSaveSlot;
extern struct State_080208e4 gGameState;
#define PARTY_STATE gGameState
extern s32 gLoadedStateWord;
extern u8 gOptionMirror;
extern s16 gPostLoadCounter;

s32 SaveState_LoadRecordIntoWork(s32 arg)
{
    s32 ret = 0;
    s32 err = SaveState_InitializeWorkspace();

    if (err != 0) {
        UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
        ret = -9;
    } else {
        s32 value;

        SaveState_LoadSummaryRecords();
        value = SaveMenu_SelectSlot(gSaveSlot, arg);
        if (value == -1) {
            ret = value;
        } else {
            void *base = &gSaveBuffer;

            err = SaveState_ReadRecordPayload(value, base);
            base = (char *)base + 0x1000;
            err |= SaveState_ReadRecordPayload(value + 3, base);
            if (err != 0) {
                UiText_ShowPositionedMessageAndWait((s32)&Value_0000000c, 1);
                ret = -2;
            } else {
                gLoadedStateWord = PARTY_STATE.value;
                gOptionMirror = ((u8 *)&PARTY_STATE)[0x22a];
                gPostLoadCounter = 0;
                gSaveSlot = value;
            }
        }
    }
    SaveState_ReleaseWorkspace();
    return ret;
}
