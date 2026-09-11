#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STORY_BEAT
#define ALCHEMY_ABI_OVERLAYS_SCENE_STORY_BEAT

/* Binding layer — not production source. Address / far-call ABI only. */

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

#define ApplyActor10Beat Func_0200047a
#define ApplyActor8Beat Func_0200043a
#define ApplyActor9Beat Func_0200045a
#define BeginActor10Beat Func_02000424
#define BeginActor10Intro Func_020003b4_a
#define BeginActor8Beat Func_020003e4
#define BeginActor9Beat Func_02000404
#define BeginActor9Intro Func_02000384
#define BeginSceneSequence Func_02000308
#define ClearStoryFlag Func_02000466
#define DefaultActorLayout Data_0200835c
#define DefaultSceneScript Data_020083ec
#define EndActor10Beat Func_0200043e
#define EndActor10Intro Func_020003dc
#define EndActor8Beat Func_020003fe
#define EndActor9Beat Func_0200041e
#define EndActor9Intro Func_020003ac
#define EndSceneSequence Func_0200037e_b
#define FinishActor10Intro Func_02000418
#define FinishActor8Motion Func_020003ba
#define FinishActor9Intro Func_020003e8
#define FinishClosingBeat Func_0200049c
#define GetFirstDefaultActor Func_020004a4
#define GetMode10Actor Func_02000492
#define GetSecondDefaultActor Func_020004b4
#define GetThirdDefaultActor Func_020004c2
#define PlayActor10BeatCue Func_0200046a
#define PlayActor10IntroCue Func_02000408
#define PlayActor8BeatCue Func_0200042a
#define PlayActor9BeatCue Func_0200044a
#define PlayActor9IntroCue Func_020003d8
#define PlayOpeningCue Func_0200036e
#define ResetActor8Pose Func_0200038e
#define SceneData_GetMessageTable Func_0200003c
#define SceneData_GetScriptTable Func_02000030
#define SceneData_ReturnZero Func_02000038
#define SceneData_SelectActorTableByStep10 Func_02000044
#define SceneData_SelectSceneScriptByStep10 Func_0200006c
#define Scene_InitSceneStateByStep Func_020001e4
#define Scene_RunActor10Beat Func_020001b0
#define Scene_RunActor10Intro Func_02000140
#define Scene_RunActor8Beat Func_02000170
#define Scene_RunActor9Beat Func_02000190
#define Scene_RunActor9Intro Func_02000110
#define Scene_RunActors8To10OpeningSequence Func_02000094
#define Scene_RunClosingBeat Func_020001d0
#define SelectActor8 Func_0200037e
#define SetActor10Mode Func_020003d4
#define SetActor10Presentation Func_0200036e_b
#define SetActor8Motion Func_0200038c
#define SetActor9Mode Func_020003b4
#define SetActor9Presentation Func_0200035e
#define SpecialStepActorLayout Data_020083bc
#define SpecialStepSceneScript Data_020084a0
#define StageActors8And10 Func_02000352
#define StageActors8And9 Func_02000342
#define WaitActor10Intro Func_020003ba_a
#define WaitActor9Intro Func_0200038a
#define WaitAfterFirstStaging Func_02000310
#define WaitAfterMotion Func_02000362
#define WaitAfterPoseReset Func_02000354
#define WaitAfterSecondStaging Func_02000320
#define WaitForClosingBeat Func_0200049e
#define WaitForPresentation Func_02000344
#define gCell Data_02000240
#define gWork Data_03001ebc

#endif
