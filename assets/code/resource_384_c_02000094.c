#include "resource_384.h"

extern void Func_02000308(void);
extern void Func_02000342(s32, s32, s32);
extern void Func_02000310(s32);
extern void Func_02000352(s32, s32, s32);
extern void Func_02000320(s32);
/* 0x0200036e serves two imports: the one-argument dialogue line and the
 * two-argument presentation select. */
extern void Func_0200036e(s32);
extern void Func_0200036e_b(s32, s32);
/* 0x0200037e likewise: the two-argument act and the zero-argument bracket
 * close. */
extern void Func_0200037e(s32, s32);
extern void Func_0200037e_b(void);
extern void Func_0200035e(s32, s32);
extern void Func_02000344(s32);
extern void Func_0200038e(s32, s32, s32);
extern void Func_02000354(s32);
extern void Func_0200038c(s32, s32);
extern void Func_02000362(s32);
extern void Func_020003ba(s32, s32);

#define BeginSceneSequence       Func_02000308
#define StageActors8And9         Func_02000342
#define WaitAfterFirstStaging    Func_02000310
#define StageActors8And10        Func_02000352
#define WaitAfterSecondStaging   Func_02000320
#define PlayOpeningCue           Func_0200036e
#define SelectActor8             Func_0200037e
#define SetActor9Presentation    Func_0200035e
#define SetActor10Presentation   Func_0200036e_b
#define WaitForPresentation      Func_02000344
#define ResetActor8Pose          Func_0200038e
#define WaitAfterPoseReset       Func_02000354
#define SetActor8Motion          Func_0200038c
#define WaitAfterMotion          Func_02000362
#define FinishActor8Motion       Func_020003ba
#define EndSceneSequence         Func_0200037e_b

void Func_02000094(void)
{
    BeginSceneSequence();

    StageActors8And9(RESOURCE384_ACTOR_8, RESOURCE384_ACTOR_9, 0);
    WaitAfterFirstStaging(40);

    StageActors8And10(RESOURCE384_ACTOR_8, RESOURCE384_ACTOR_10, 0);
    WaitAfterSecondStaging(40);

    PlayOpeningCue(RESOURCE384_CUE_OPENING);
    SelectActor8(RESOURCE384_ACTOR_8, 0);

    SetActor9Presentation(RESOURCE384_ACTOR_9, 2);
    SetActor10Presentation(RESOURCE384_ACTOR_10, 2);
    WaitForPresentation(20);

    ResetActor8Pose(RESOURCE384_ACTOR_8, 0, 0);
    WaitAfterPoseReset(20);

    SetActor8Motion(RESOURCE384_ACTOR_8, 1);
    WaitAfterMotion(20);

    FinishActor8Motion(RESOURCE384_ACTOR_8, 0);

    EndSceneSequence();
}
