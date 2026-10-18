#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"
#include "edition.h"

void Func_02000054(void);
void Func_02000058(void);
void Func_02000068(void);
void Func_02000074(void);
void Func_02000140(void);
void Func_02000158(void);
void Func_02000170(void);
void Func_0200018c(void);
void Func_020001a4(void);
void Func_020001b0(void);
void Func_020001bc(void);
void Func_020001c8(void);
void Func_020001d4(void);
void Func_020001e0(void);
void Func_020001e4(void);
void Func_02000224(void);
void Func_0200026c(void);
void Func_02000330(void);
void Func_02000444(void);
void Func_02000454(void);
void Func_02000464(void);
void Func_02000508(void);
void Func_0200055c(void);
void Func_020005b0(void);
void Func_020005dc(void);
void Func_02000604(void);
void Func_02000624(void);
void Func_02000650(void);
void Func_02000660(void);
void Func_02000688(void);
void Func_020006b0(void);
void Func_020006cc(void);
void Func_020006e8(void);
void Func_0200070c(void);
void Func_02000724(void);
void Func_02000740(void);
void Func_02000744(void);
void Func_02000748(void);
void Func_0200091c(void);
void Func_02000924(void);

// clang-format off
s32 gData_02000f44[] = {
    0, 0, 0, 0, 0, 0,
    0x70000000, (s32)0xf7700000, 0, 0, 0, 0,
    0x77770000, (s32)0xffff7770, -9, 0x7777ffff, 0, 0,
    0, 0, 0x7777, 0x777ffff, 0x7fffffff, (s32)0xffff7777,
    0, 0, 0, 0, 0, 0,
    7, 0x77f, -0x90000, -0x9000, 0x7ffff700, 0x7ffff70,
    0x7fff70, 0x7ffff7, 0x7fff7, 0x7fff7, 0x77ff, 119,
    0, 0, 0, 0, 0, 0,
    -0x890000, 0x77000000, 0, 0, 0, 0,
    0, 0, 0x7fff, 0x7ffff, 0x7ffff7, 0x7ffff70,
    0x7fff700, 0x7ffff700, 0x7fff7000, 0x7fff7000, 0, 0,
    0, 0, 0, 0, 0x40000000, (s32)0xf4400000,
    0, 0, 0, 0, 0x44440000, (s32)0xffff4440,
    -12, 0x4444ffff, 0, 0, 0, 0,
    0x4444, 0x444ffff, 0x4fffffff, (s32)0xffff4444, 0, 0,
    0, 0, 0, 0, 4, 0x44f,
    -0xc0000, -0xc000, 0x4ffff400, 0x4ffff40, 0x4fff40, 0x4ffff4,
    0x4fff4, 0x4fff4, 0x44ff, 68, 0, 0,
    0, 0, 0, 0, -0xbc0000, 0x44000000,
    0, 0, 0, 0, 0, 0,
    0x4fff, 0x4ffff, 0x4ffff4, 0x4ffff40, 0x4fff400, 0x4ffff400,
    0x4fff4000, 0x4fff4000, (s32)0xe9000000, 233, (s32)0xeb000000, 235,
    (s32)0xedeb0000, 0xebed, (s32)0xeeedebe9, (s32)0xe9ebedee, (s32)0xeeedebe9, (s32)0xe9ebedee,
    (s32)0xedeb0000, 0xebed, (s32)0xeb000000, 235, (s32)0xe9000000, 233,
    0xe900, 0xe90000, 0xebe9e9, (s32)0xe9e9eb00, (s32)0xedebeb00, 0xebebed,
    (s32)0xefed0000, 0xedef, (s32)0xefed0000, 0xedef, (s32)0xedebeb00, 0xebebed,
    0xebe9e9, (s32)0xe9e9eb00, 0xe900, 0xe90000, (s32)0xe9000000, 233,
    (s32)0xebe90000, 0xe9eb, (s32)0xedebe900, 0xe9ebed, (s32)0xeeedebe9, (s32)0xe9ebedee,
    (s32)0xeeedebe9, (s32)0xe9ebedee, (s32)0xedebe900, 0xe9ebed, (s32)0xebe90000, 0xe9eb,
    (s32)0xe9000000, 233,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 64, 0, 56, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 112, 0, 56, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 64, 0, 120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 112, 0, 120, FACING_NORTH, 0, 16, 16, 304, 224, 0},
    {4, CONDITION_ALWAYS, 344, 0, 104, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 88, 0, 184, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 184, 0, 200, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 424, 0, 136, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 184, 0, 200, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(322),
    SCENE_EXIT(20, 322, 2),
    SCENE_EXITS_END,
};

s16 gData_02001300[] = {
    1, -1, 1, 0, 0, 40, 0, 0,
    0, 40, 0x4000, 0, 5, -1, 1, 0,
    0, 40, 0, 0, 0, 72, 0x4000, 0,
    2, -1, 1, 0, 0, 40, 0, 0,
    0, 104, 0x4000, 0, 3, -1, 1, 0,
    0, 40, 0, 0, 0, 136, 0x4000, 0,
    205, -1, 1, 0, 0, 88, 0, 0,
    0, 40, 0x4000, 0, 71, -1, 1, 0,
    0, 82, 0, 0, 0, 72, 0x4000, 0,
    202, -1, 1, 0, 0, 88, 0, 0,
    0, 104, 0x4000, 0, 206, -1, 1, 0,
    0, 88, 0, 0, 0, 136, 0x4000, 0,
    245, -1, 1, 0, 0, 136, 0, 0,
    0, 40, 0x4000, 0, 307, -1, 1, 0,
    0, 136, 0, 0, 0, 72, 0x4000, 0,
    299, -1, 1, 0, 0, 136, 0, 0,
    0, 104, 0x4000, 0, 290, -1, 1, 0,
    0, 136, 0, 0, 0, 136, 0x4000, 0,
    364, -1, 1, 0, 0, 360, 0, 0,
    0, 104, 0x4000, 0, 56, -1, 1, 0,
    0, 392, 0, 0, 0, 104, 0x4000, 0,
    290, -1, 1, 0, 0, -64, 0, 0,
    0, -64, 0x4000, 0, 489, -1, 1, 0,
    0, -64, 0, 0, 0, -64, 0x4000, 2,
    252, -1, 1, 0, 0, -64, 0, 0,
    0, -64, 0x4000, 2, 305, -1, 1, 0,
    0, 456, 0, 0, 0, 104, 0x4000, 0,
    365, -1, 1, 0, 0, 392, 0, 0,
    0, 168, 0x4000, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
// clang-format on

struct SceneEvent gSceneEvents[] = {
    {0, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000054)},
    {0, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000054)},
    {0, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000748)},
    {0xc400, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020001a4)},
    {0x8400, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020001b0)},
    {0x400, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020001bc)},
    {0x4400, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020001c8)},
    {0, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020001e0)},
    {0x4400, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020001d4)},
    {0, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000924)},
    {0xc400, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000140)},
    {0x8400, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000158)},
    {0x400, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000170)},
    {0x4400, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Func_0200018c)},
    {0, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000054)},
    {0, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Func_0200091c)},
    {0, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000054)},
    {0xc400, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000068)},
    {0, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000058)},
    {0, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Func_0200070c)},
#if (LANGUAGE == LANGUAGE_JAPANESE) /* Rows exclusive to the Japanese edition. */
    {0x8d15, 10, CONDITION_ALWAYS, 5618},
#endif
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgOooogOw2)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgWhyDoThese)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgStupidBriggs2)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgBriggsSPirates)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgIJustCame)},
    {1, 20, CONDITION_ALWAYS, 20},
    {3, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Func_0200026c)},
    {3, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020001e4)},
    {3, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000224)},
    {3, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000330)},
    {19, 100, CONDITION_ALWAYS, 0x20014d},
    {0x8e15, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000074)},
    {0x8e15, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000074)},
    {0x4e15, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000074)},
    {0x9415, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000074)},
    {0x1815, 18, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000054)},
    {0x50008615, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020005dc)},
    {0x8b85, 0, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000444)},
    {0x40008b85, 0, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000454)},
    {0x10009a15, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000054)},
    {0x60009a15, 17, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000464)},
    {0x20009a15, 17, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000508)},
    {0x10009a15, 17, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000054)},
    {0x20009d15, 17, CONDITION_ALWAYS, EVENT_SCRIPT(Func_0200055c)},
    {0x20009d15, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020005b0)},
    {0x50008805, 50, 0x300, EVENT_SCRIPT(Func_02000688)},
    {0x50008805, 51, 0x301, EVENT_SCRIPT(Func_020006b0)},
    {0x50008805, 52, 0x302, EVENT_SCRIPT(Func_020006cc)},
    {0x50008805, 53, 0x303, EVENT_SCRIPT(Func_020006e8)},
    {0x50008a05, 60, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000724)},
    {0x50008905, 55, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000604)},
    {0x50008905, 56, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000624)},
    {0x50008905, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000650)},
    {0x50008905, 57, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000660)},
    {0x8715, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000740)},
    {0x8715, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000740)},
    {0x9815, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000744)},
    {0x9815, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000744)},
    {SCENE_EVENTS_END},
};
