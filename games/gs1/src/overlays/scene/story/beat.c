#include "types.h"
#include "scene.h"

#include "resource_384.h"

extern s16 gCell[];
extern u8 DefaultActorLayout[];
extern u8 SpecialStepActorLayout[];
extern u8 SpecialStepSceneScript[];
extern u8 DefaultSceneScript[];
extern u8 *gWork;

u8 *GetMode10Actor(s32);
u8 *GetFirstDefaultActor(s32);
u8 *GetSecondDefaultActor(s32);
u8 *GetThirdDefaultActor(s32);

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
 * scene step. gCell[225] is the shared signed scene-step field. */
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
    if (gCell[225] == RESOURCE384_SPECIAL_STEP) {
        return SpecialStepActorLayout;
    }
    return DefaultActorLayout;
}

u8 *SceneData_SelectSceneScriptByStep10(void)
{
    if (gCell[225] == RESOURCE384_SPECIAL_STEP) {
        return SpecialStepSceneScript;
    }
    return DefaultSceneScript;
}

void Scene_RunActors8To10OpeningSequence(void)
{
    void FinishActor8Motion(s32, s32);

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

void Scene_RunActor9Intro(void)
{
    s32 SetActor9Mode(s32, s32);

    BeginActor9Intro();
    SetActor9Mode(RESOURCE384_ACTOR_9, 2);
    WaitActor9Intro(RESOURCE384_ACTOR_FLAG);
    PlayActor9IntroCue(RESOURCE384_CUE_ACTOR_9_INTRO);
    FinishActor9Intro(RESOURCE384_ACTOR_9, 0);
    EndActor9Intro();
}

void Scene_RunActor10Intro(void)
{
    void BeginActor10Intro(void);
    void WaitActor10Intro(s32);

    BeginActor10Intro();
    SetActor10Mode(RESOURCE384_ACTOR_10, 4);
    WaitActor10Intro(RESOURCE384_ACTOR_FLAG);
    PlayActor10IntroCue(RESOURCE384_CUE_ACTOR_10_INTRO);
    FinishActor10Intro(RESOURCE384_ACTOR_10, 0);
    EndActor10Intro();
}

void Scene_RunActor8Beat(void)
{
    BeginActor8Beat();
    PlayActor8BeatCue(RESOURCE384_CUE_ACTOR_8_BEAT);
    ApplyActor8Beat(RESOURCE384_ACTOR_8, 0);
    EndActor8Beat();
}

void Scene_RunActor9Beat(void)
{
    BeginActor9Beat();
    PlayActor9BeatCue(RESOURCE384_CUE_ACTOR_9_BEAT);
    ApplyActor9Beat(RESOURCE384_ACTOR_9, 0);
    EndActor9Beat();
}

void Scene_RunActor10Beat(void)
{
    BeginActor10Beat();
    PlayActor10BeatCue(RESOURCE384_CUE_ACTOR_10_BEAT);
    ApplyActor10Beat(RESOURCE384_ACTOR_10, 0);
    EndActor10Beat();
}

void Scene_RunClosingBeat(void)
{
    WaitForClosingBeat(123);
    FinishClosingBeat(1);
}

s32 Scene_InitSceneStateByStep(void)
{
    s16 mode;

    *(s32 *)(gWork + 448) = RESOURCE384_SCRIPT_STATE;
    mode = gCell[225];

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
