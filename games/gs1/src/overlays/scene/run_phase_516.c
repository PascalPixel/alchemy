#include "types.h"

#define SCENE_WORKSPACE (*(u8 **)0x03001ebc)

struct SceneBeatSubject {
    u8 unknown_00[0x23];
    u8 marker;
};

s32 Func_020014ce();
s32 Func_02001a70();
struct SceneBeatSubject *Func_02001aa2();
void Func_02001a7c();
s32 Func_02000e90();
struct SceneBeatSubject *Func_02001ad6();
void Func_02001aac();
void Func_0200151e();
void Func_02001524();
s32 Func_02001ac6();
void Func_02001900();
#define RunSceneBeat10 Func_020014ce
#define IsSceneFlag0201Set Func_02001a70
#define GetSceneBeatSubject Func_02001aa2
#define DrawSceneBeatRectangle Func_02001a7c
#define FillSceneTileAttributes Func_02000e90
#define GetScenePresentationSubject Func_02001ad6
#define SetScenePresentationMode Func_02001aac
#define RunSceneBeat8 Func_0200151e
#define RunSceneBeat9 Func_02001524
#define IsSceneFlag0845Set Func_02001ac6
#define RunPhase516Followup Func_02001900
#define RunScenePhase516 Func_02000bf8

static __inline__ void DrawSceneBeat(s32 left, s32 top, s32 width, s32 height,
                                     s32 tile, s32 palette)
{
    DrawSceneBeatRectangle(left, top, width, height, tile, palette);
}

s32 RunScenePhase516(void)
{
    u8 *workspace = SCENE_WORKSPACE;

    *(s32 *)(workspace + 448) = 516;
    RunSceneBeat10(10);

    if (IsSceneFlag0201Set(0x201) != 0) {
        struct SceneBeatSubject *subject = GetSceneBeatSubject(10);

        subject->marker = 2;
        DrawSceneBeat(32, 20, 2, 4, 11, 16);
        FillSceneTileAttributes(2, 12, 16, 1, 4, 0);
        SetScenePresentationMode(GetScenePresentationSubject(10), 0);
    }

    RunSceneBeat8(8);
    RunSceneBeat9(9);

    if (IsSceneFlag0845Set(0x845) == 0) {
        RunPhase516Followup(6);
    }
    return 0;
}
