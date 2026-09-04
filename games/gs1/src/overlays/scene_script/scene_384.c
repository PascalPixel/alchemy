#include "types.h"

#define DefaultActorLayout     Data_0200835c
#define SpecialStepActorLayout Data_020083bc
#define DefaultSceneScript     Data_020083ec
#define SpecialStepSceneScript Data_020084a0
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
#define BeginActor9Intro     Func_02000384
#define SetActor9Mode        Func_020003b4
#define WaitActor9Intro      Func_0200038a
#define PlayActor9IntroCue   Func_020003d8
#define FinishActor9Intro    Func_020003e8
#define EndActor9Intro       Func_020003ac
#define BeginActor10Intro    Func_020003b4
#define SetActor10Mode       Func_020003d4
#define WaitActor10Intro     Func_020003ba
#define PlayActor10IntroCue  Func_02000408
#define FinishActor10Intro   Func_02000418
#define EndActor10Intro      Func_020003dc
#define BeginActor8Beat    Func_020003e4
#define PlayActor8BeatCue  Func_0200042a
#define ApplyActor8Beat    Func_0200043a
#define EndActor8Beat      Func_020003fe
#define BeginActor9Beat    Func_02000404
#define PlayActor9BeatCue  Func_0200044a
#define ApplyActor9Beat    Func_0200045a
#define EndActor9Beat      Func_0200041e
#define BeginActor10Beat    Func_02000424
#define PlayActor10BeatCue  Func_0200046a
#define ApplyActor10Beat    Func_0200047a
#define EndActor10Beat      Func_0200043e
#define WaitForClosingBeat  Func_0200049e
#define FinishClosingBeat   Func_0200049c
#define ClearStoryFlag         Func_02000466
#define GetMode10Actor         Func_02000492
#define GetFirstDefaultActor   Func_020004a4
#define GetSecondDefaultActor  Func_020004b4
#define GetThirdDefaultActor   Func_020004c2

#include "resource_384.h"

#define FieldScene_InitSceneStateByStep Func_020001e4

#define FieldScene_RunClosingBeat Func_020001d0

#define FieldScene_RunActor10Beat Func_020001b0

#define FieldScene_RunActor9Beat Func_02000190

#define FieldScene_RunActor8Beat Func_02000170

#define FieldScene_RunActor10Intro Func_02000140

#define FieldScene_RunActor9Intro Func_02000110

#define FieldScene_RunActors8To10OpeningSequence Func_02000094

#define SceneData_SelectSceneScriptByStep10 Func_0200006c

#define SceneData_SelectActorTableByStep10 Func_02000044

#define SceneData_GetMessageTable Func_0200003c

#define SceneData_ReturnZero Func_02000038

#define SceneData_GetScriptTable Func_02000030

extern s16 Data_02000240[];
extern u8 Data_0200835c[];
extern u8 Data_020083bc[];
extern u8 Data_020084a0[];
extern u8 Data_020083ec[];
extern u8 *Data_03001ebc;

void Func_02000308(void);
void Func_02000342(s32, s32, s32);
void Func_02000310(s32);
void Func_02000352(s32, s32, s32);
void Func_02000320(s32);
void Func_0200036e(s32);
void Func_0200036e_b(s32, s32);
void Func_0200037e(s32, s32);
void Func_0200037e_b(void);
void Func_0200035e(s32, s32);
void Func_02000344(s32);
void Func_0200038e(s32, s32, s32);
void Func_02000354(s32);
void Func_0200038c(s32, s32);
void Func_02000362(s32);
void Func_02000384(void);
void Func_0200038a(s32);
void Func_020003d8(s32);
void Func_020003e8(s32, s32);
void Func_020003ac(void);
s32 Func_020003d4(s32, s32);
void Func_02000408(s32);
void Func_02000418(s32, s32);
void Func_020003dc(void);
void Func_020003e4(void);
void Func_0200042a(s32);
void Func_0200043a(s32, s32);
void Func_020003fe(void);
void Func_02000404(void);
void Func_0200044a(s32);
void Func_0200045a(s32, s32);
void Func_0200041e(void);
void Func_02000424(void);
void Func_0200046a(s32);
void Func_0200047a(s32, s32);
void Func_0200043e(void);
void Func_0200049e(s32);
void Func_0200049c(s32);
void Func_02000466(s32);
u8 *Func_02000492(s32);
u8 *Func_020004a4(s32);
u8 *Func_020004b4(s32);
u8 *Func_020004c2(s32);

/* Overlay-owned descriptor block exported through the header veneer table. */

/* Scene layout selected by this overlay's exported interface. */

/* Scene-state halfword 225 is the current scene step. */

/* 0x0200036e serves two imports: the one-argument dialogue line and the
 * two-argument presentation select. */

/* 0x0200037e likewise: the two-argument act and the zero-argument bracket
 * close. */

/* Keeping this as a small ordinary-C helper preserves the standalone store in
 * the special-step branch; folding it into the root enables tail merging. */

/* Publish this scene's script state, then update actor flags for the current
 * scene step. Data_02000240[225] is the shared signed scene-step field. */

static inline void SetActorFlag(u8 *record, u8 bits)
{
    *record |= bits;
}

u8 *SceneData_GetScriptTable(void)
{
    return RESOURCE384_HEADER_DATA;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return RESOURCE384_SCENE_LAYOUT;
}

u8 *SceneData_SelectActorTableByStep10(void)
{
    if (Data_02000240[225] == RESOURCE384_SPECIAL_STEP) {
        return SpecialStepActorLayout;
    }
    return DefaultActorLayout;
}

u8 *SceneData_SelectSceneScriptByStep10(void)
{
    if (Data_02000240[225] == RESOURCE384_SPECIAL_STEP) {
        return SpecialStepSceneScript;
    }
    return DefaultSceneScript;
}

void FieldScene_RunActors8To10OpeningSequence(void)
{
    void Func_020003ba(s32, s32);

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

void FieldScene_RunActor9Intro(void)
{
    s32 Func_020003b4(s32, s32);

    BeginActor9Intro();
    SetActor9Mode(RESOURCE384_ACTOR_9, 2);
    WaitActor9Intro(RESOURCE384_ACTOR_FLAG);
    PlayActor9IntroCue(RESOURCE384_CUE_ACTOR_9_INTRO);
    FinishActor9Intro(RESOURCE384_ACTOR_9, 0);
    EndActor9Intro();
}

void FieldScene_RunActor10Intro(void)
{
    void Func_020003b4(void);
    void Func_020003ba(s32);

    BeginActor10Intro();
    SetActor10Mode(RESOURCE384_ACTOR_10, 4);
    WaitActor10Intro(RESOURCE384_ACTOR_FLAG);
    PlayActor10IntroCue(RESOURCE384_CUE_ACTOR_10_INTRO);
    FinishActor10Intro(RESOURCE384_ACTOR_10, 0);
    EndActor10Intro();
}

void FieldScene_RunActor8Beat(void)
{
    BeginActor8Beat();
    PlayActor8BeatCue(RESOURCE384_CUE_ACTOR_8_BEAT);
    ApplyActor8Beat(RESOURCE384_ACTOR_8, 0);
    EndActor8Beat();
}

void FieldScene_RunActor9Beat(void)
{
    BeginActor9Beat();
    PlayActor9BeatCue(RESOURCE384_CUE_ACTOR_9_BEAT);
    ApplyActor9Beat(RESOURCE384_ACTOR_9, 0);
    EndActor9Beat();
}

void FieldScene_RunActor10Beat(void)
{
    BeginActor10Beat();
    PlayActor10BeatCue(RESOURCE384_CUE_ACTOR_10_BEAT);
    ApplyActor10Beat(RESOURCE384_ACTOR_10, 0);
    EndActor10Beat();
}

void FieldScene_RunClosingBeat(void)
{
    WaitForClosingBeat(123);
    FinishClosingBeat(1);
}

s32 FieldScene_InitSceneStateByStep(void)
{
    s16 mode;

    *(s32 *)(Data_03001ebc + 448) = RESOURCE384_SCRIPT_STATE;
    mode = Data_02000240[225];

    if (mode == 2) {
        ClearStoryFlag(0x12f);
    } else if (mode == RESOURCE384_SPECIAL_STEP) {
        SetActorFlag(GetMode10Actor(RESOURCE384_ACTOR_8) + 89, RESOURCE384_ACTOR_FLAG);
    } else {
        u8 bits = RESOURCE384_ACTOR_FLAG;
        u8 *record = GetFirstDefaultActor(RESOURCE384_ACTOR_8) + 89;
        u8 value = *record;
        value |= bits;
        *record = value;
        record = GetSecondDefaultActor(RESOURCE384_ACTOR_9) + 89;
        value = *record;
        value |= bits;
        *record = value;
        record = GetThirdDefaultActor(RESOURCE384_ACTOR_10) + 89;
        bits |= *record;
        *record = bits;
    }

    return 0;
}
