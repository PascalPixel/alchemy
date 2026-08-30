#include "types.h"

struct SceneRecordHeading {
    u8 pad[6];
    u16 heading;
};

extern void Func_02004e58(s32 arg0);
extern s32 Func_02004f16(s32 flagId);
extern s32 Func_02004f28(s32 flagId);
extern void Func_02004edc(void);
extern void Func_02004fa0(void);
extern u8 *Func_02004f0a(s32 slot);
extern u8 *Func_02004f1e(s32 slot);
extern u8 *Func_02004f2a(s32 slot);
extern u8 *Func_02004f34(s32 slot);
extern u8 *Func_02004f40(s32 slot);
extern u8 *Func_02004f48(s32 slot);
extern u8 *Func_02004fa2(s32 slot);
extern u8 *Func_02004fae(s32 slot);
extern void Func_02004fd4(s32 slot, u8 *descriptor);
extern void Func_02004fd0(s32 slot, s32 a1, s32 a2);
extern void Func_02005000(s32 dialogueId);
extern void Func_02005012(s32 dialogueId);
extern void Func_0200502c(s32 dialogueId);
extern void Func_02005044(s32 slot, s32 a1);

#define OpenSceneExit Func_02004edc
#define WaitSceneExitStep Func_02004e58
#define ClearSceneExitGateAtEntry Func_02004f0a
#define GetSceneExitPendingWork Func_02004f1e
#define ResetSceneExitPendingWork Func_02004f2a
#define SetSceneExitCompletionMode Func_02004f34
#define ClearSceneExitField40 Func_02004f40
#define SetSceneExitGate Func_02004f48
#define TransitionSceneExitSlot Func_02004fd0
#define IsFlag0895Set Func_02004f16
#define IsFlag089bSet Func_02004f28
#define ShowSceneExitDialogue1a5b Func_02005000
#define ShowSceneExitDialogue189e Func_02005012
#define ShowSceneExitDialogue182a Func_0200502c
#define FinalizeSceneExitSlot Func_02005044
#define SetSceneExitHeading Func_02004fa2
#define ClearSceneExitGateBeforeDescriptor Func_02004fae
#define InstallSceneExitDescriptor Func_02004fd4
#define CloseSceneExit Func_02004fa0
#define FieldScene_RunRoofSceneExit Func_02000afc

void FieldScene_RunRoofSceneExit(void)
{
    OpenSceneExit();

    ClearSceneExitGateAtEntry(12)[91] = 0;

    goto testPendingWork;
waitPendingWork:
        WaitSceneExitStep(1);
testPendingWork:
    if (*(s32 *)(GetSceneExitPendingWork(12) + 12) > 0) {
        goto waitPendingWork;
    }

    *(s32 *)(ResetSceneExitPendingWork(12) + 12) = 0;

    *(s32 *)(SetSceneExitCompletionMode(12) + 60) = 128 << 24;

    *(s32 *)(ClearSceneExitField40(12) + 40) = 0;

    SetSceneExitGate(12)[91] = 1;

    TransitionSceneExitSlot(12, 0, 0);

    if (IsFlag0895Set(0x895) != 0) {
        ShowSceneExitDialogue1a5b(0x1a5b);
    } else if (IsFlag089bSet(0x89b) != 0) {
        ShowSceneExitDialogue189e(0x189e);
    } else {
        ShowSceneExitDialogue182a(0x182a);
    }

    FinalizeSceneExitSlot(12, 0);

    ((struct SceneRecordHeading *)SetSceneExitHeading(12))->heading = 128 << 7;

    ClearSceneExitGateBeforeDescriptor(12)[91] = 0;

    InstallSceneExitDescriptor(12, (u8 *)0x0200c638);
    CloseSceneExit();
}
