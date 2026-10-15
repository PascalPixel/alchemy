#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void Func_02000278(void);
void Func_02000308(void);
void Func_02000398(void);
void Func_02000428(void);
void Func_02000448(void);
void Func_02000468(void);
void Func_02000488(void);
void Func_020004a8(void);
void Func_02000560(void);
void Func_020005dc(void);
void Func_020005ec(void);
void Func_020005fc(void);
void Func_0200060c(void);
void Func_0200062c(void);
void Func_020008e8(void);
void Func_02000934(void);
void Func_02000e7c(void);
void Func_0200109c(void);
void Func_02001160(void);

extern s16 gData_02002184[];
extern s16 gData_0200219a[];
extern s16 gData_020021b0[];

// clang-format off
u32 gData_02001d84[] = {
    39, 2, 22, 15, 0x10000, 22,
    16, 0x10000, 0x80010000, 4, 0x280000, 0,
    0x3b80000, 1, 4, 0x280000, 0, 0x3880000,
    1, 4, 0x580000, 0, 0x3880000, 1,
    4, 0x580000, 0, 0x3b80000, 1, 13,
    0xffff, 0xc0010000,
};

u32 gData_02001e04[] = {
    39, 2, 22, 15, 0x10000, 22,
    16, 0x10000, 0x80010000, 4, 0x580000, 0,
    0x3880000, 1, 4, 0x580000, 0, 0x3b80000,
    1, 4, 0x280000, 0, 0x3b80000, 1,
    4, 0x280000, 0, 0x3880000, 1, 13,
    0xffff, 0xc0010000,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 460, 0, 550, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(202),
    SCENE_EXIT_IF(1, 203, 1, 0xffffffff),
    SCENE_EXIT_IF(2, 203, 2, 0xffffffff),
    SCENE_EXIT_IF(3, 203, 3, 0xffffffff),
    SCENE_EXIT_IF(4, 201, 3, 0xffffffff),
    SCENE_EXIT_IF(5, 205, 1, 0xffffffff),
    SCENE_EXIT_IF(12, 203, 12, 0xffffffff),
    SCENE_EXIT_IF(15, 203, 15, 0xffffffff),
    SCENE_EXIT_IF(16, 203, 16, 0xffffffff),
    SCENE_EXIT_IF(11, 204, 1, 0xffffffff),
    SCENE_EXIT_IF(10, 204, 10, 0xffffffff),
    SCENE_EXIT_IF(17, 204, 17, 0xffffffff),
    SCENE_EXITS_END,
};

struct ScenePlacement gScenePlacements[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(952), 0, PIXELS(920), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(904), 0, PIXELS(920), FACING_SOUTH, 2, 1},
    {372, CONDITION_ALWAYS, 1, PIXELS(856), 0, PIXELS(1016), FACING_SOUTH, 2, 0},
    {450, CONDITION_ALWAYS, 1, PIXELS(128), 0, PIXELS(880), FACING_SOUTH, 2, 0},
    {157, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(1256), FACING_WEST, 1, 0},
    {156, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(1040), FACING_SOUTH, 1, 0},
    {160, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(1064), FACING_SOUTH, 1, 0},
    {165, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(856), 0x5000, 1, 0},
    {164, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(920), FACING_WEST, 1, 0},
    {166, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(920), 0x5000, 1, 0},
    {167, CONDITION_ALWAYS, 1, PIXELS(40), 0, PIXELS(952), FACING_NORTH, 2, 0},
    {168, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(904), FACING_SOUTH, 2, 0},
    {216, CONDITION_ALWAYS, 2, PIXELS(80), 0, PIXELS(1104), FACING_SOUTH, 0, 0},
    {189, CONDITION_ALWAYS, 2, PIXELS(392), 0, PIXELS(712), FACING_SOUTH, 0, 0},
    {80, CONDITION_ALWAYS, 1, PIXELS(840), 0, PIXELS(1080), FACING_EAST, 2, 0},
    {203, CONDITION_ALWAYS, 2, PIXELS(864), 0, PIXELS(984), FACING_SOUTH, 0, 0},
    {231, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(520), 0x3000, 1, 0},
    {231, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(520), 0x5000, 1, 0},
    {166, CONDITION_ALWAYS, 2, PIXELS(552), 0, PIXELS(440), FACING_SOUTH, 0, 0},
    {7, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 1},
    {407, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {157, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(1008), FACING_WEST, 1, 0},
    {160, CONDITION_ALWAYS, 1, PIXELS(464), 0, PIXELS(1016), 0xd000, 2, 0},
    {489, CONDITION_ALWAYS, 7, PIXELS(488), 0, PIXELS(984), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

s16 gData_02002184[] = {
    2, 1, 1, 1, 6, 2, 2, 1,
    1, 6, -1,
};

s16 gData_0200219a[] = {
    0, 1, 2, 1, 6, 0, 2, 2,
    1, 6, -1,
};

s16 gData_020021b0[] = {
    3, 1, 1, 1, 0, -1,
};
// clang-format on

s32 gData_020021bc[] = {
    (s32)gData_02002184,
    0x2f0016,
};

s32 gData_020021c4[] = {
    (s32)gData_02002184,
    0x2f0023,
};

s32 gData_020021cc[] = {
    (s32)gData_0200219a,
    0x280009,
};

s32 gData_020021d4[] = {
    (s32)gData_020021b0,
    0x2d0036,
};

s32 gData_020021dc[] = {
    (s32)gData_0200219a,
    0x14001c,
};

s32 gData_020021e4[] = {
    (s32)gData_0200219a,
    0xd001c,
};

s32 gData_020021ec[] = {
    (s32)gData_02002184,
    0x320036,
};

struct SceneEvent gSceneEvents[] = {
    { 0xc602, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000278) },
    { 0xc602, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000278) },
    { 0xc602, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000278) },
    { 49, 4, CONDITION_ALWAYS, 4 },
    { 0xc401, 5, 0x1908, 5 },
    { 0xc602, 12, 0x1966, EVENT_SCRIPT(Func_02000278) },
    { 1, 15, CONDITION_ALWAYS, 15 },
    { 0xc602, 16, 0x1969, EVENT_SCRIPT(Func_02000278) },
    { 0xc602, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000278) },
    { 0xc602, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000278) },
    { 1, 17, CONDITION_ALWAYS, 17 },
    { 2, 20, 0x964, EVENT_SCRIPT(Func_02001160) },
    { 2, 21, 0x964, EVENT_SCRIPT(Func_02000e7c) },
    { 2, 22, 0x964, EVENT_SCRIPT(Func_02000e7c) },
    { 2, 23, 0x964, EVENT_SCRIPT(Func_0200109c) },
    { 0, 12, CONDITION_ALWAYS, MSGNO(MsgICannotRemember) },
    { 0, 13, CONDITION_ALWAYS, MSGNO(MsgLemuriaHasNo) },
    { 0, 14, CONDITION_ALWAYS, MSGNO(MsgInLemuriaWe) },
    { 0, 15, CONDITION_ALWAYS, MSGNO(MsgLifeNeverChanges) },
    { 0, 16, CONDITION_ALWAYS, MSGNO(MsgIOughtTo) },
    { 0, 17, CONDITION_ALWAYS, MSGNO(MsgHowOldAm) },
    { 0, 18, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000308) },
    { 0, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000398) },
    { 0, 20, CONDITION_ALWAYS, MSGNO(MsgMeowMrrrfle2) },
    { 0, 21, CONDITION_ALWAYS, MSGNO(MsgWoofRrrrrroof2) },
    { 0, 22, CONDITION_ALWAYS, MSGNO(MsgMoo2) },
    { 0, 23, CONDITION_ALWAYS, MSGNO(MsgBukbukbuk2) },
    { 0, 7, CONDITION_ALWAYS, MSGNO(MsgBlank4) },
    { 0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgMoreOfThe) },
    { 0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgIHavenT) },
    { 0x8d15, 14, CONDITION_ALWAYS, MSGNO(MsgIVeAged) },
    { 0x8d15, 15, CONDITION_ALWAYS, MSGNO(MsgSometimesIThink) },
    { 0x8d15, 16, CONDITION_ALWAYS, MSGNO(MsgLongAgoA) },
    { 0x8d15, 17, CONDITION_ALWAYS, MSGNO(MsgKingHydrosIs) },
    { 0x8d15, 18, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000308) },
    { 0x8d15, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000398) },
    { 0x8d15, 20, CONDITION_ALWAYS, MSGNO(MsgNoOneIn) },
    { 0x8d15, 21, CONDITION_ALWAYS, MSGNO(MsgThereSSo) },
    { 0x8d15, 22, CONDITION_ALWAYS, MSGNO(MsgLemuriansAreSo) },
    { 0x8d15, 23, CONDITION_ALWAYS, MSGNO(MsgIfItWeren) },
    { 0x8d15, 7, CONDITION_ALWAYS, MSGNO(MsgMotherWhyHave) },
    { 0, 24, 0x196a, MSGNO(MsgIVeNever) },
    { 0, 25, 0x196a, MSGNO(MsgLordConservatoHas) },
    { 0x8d15, 24, 0x196a, MSGNO(MsgIMSureThe) },
    { 0x8d15, 25, 0x196a, MSGNO(MsgHowCouldLord) },
    { 0, 24, 0x1969, MSGNO(MsgKingHydrosAwaits) },
    { 0, 25, 0x1969, MSGNO(MsgHisHighnessThe) },
    { 0x8d15, 24, 0x1969, MSGNO(MsgTheyAreClearly) },
    { 0x8d15, 25, 0x1969, MSGNO(MsgWhatDoesHis) },
    { 0, 24, CONDITION_ALWAYS, MSGNO(MsgTheKingIs) },
    { 0, 25, CONDITION_ALWAYS, MSGNO(MsgTheKingWill) },
    { 0x8d15, 24, CONDITION_ALWAYS, MSGNO(MsgHowMuchLongerWill) },
    { 0x8d15, 25, CONDITION_ALWAYS, MSGNO(MsgIHadHeard) },
    { 0, 26, 0x196a, MSGNO(MsgIfYouAnd) },
    { 0x8d15, 26, 0x196a, MSGNO(MsgIKnowWhy) },
    { 0, 26, CONDITION_ALWAYS, MSGNO(MsgTheSenateHolds) },
    { 0x8d15, 26, CONDITION_ALWAYS, MSGNO(MsgITrustThe) },
    { 0xc400, 11, CONDITION_ALWAYS, MSGNO(MsgHouseOfThe) },
    { 0, 29, CONDITION_ALWAYS, MSGNO(MsgDrinkingFromThis) },
    { 0x8d15, 29, CONDITION_ALWAYS, MSGNO(MsgTheseTurtlesHave) },
    { 0, 30, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000934) },
    { 0x8d15, 30, CONDITION_ALWAYS, MSGNO(MsgNoOneIs) },
    { 0, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020008e8) },
    { 3, 25, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020008e8) },
    { 3, 40, CONDITION_ALWAYS, EVENT_SCRIPT(Func_0200062c) },
    { 0x8515, 8, 0x962, 0 },
    { 0xc15, 10, 0x963, EVENT_SCRIPT(Func_02000560) },
    { 0x50008905, 30, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000428) },
    { 0x50008905, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000448) },
    { 0x50008905, 32, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000468) },
    { 0x50008905, 33, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000488) },
    { 0x50008905, 38, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020004a8) },
    { 0x50008805, 104, 0x303, EVENT_SCRIPT(Func_020005dc) },
    { 0x50008805, 105, 0x304, EVENT_SCRIPT(Func_020005ec) },
    { 0x50008805, 107, 0x306, EVENT_SCRIPT(Func_020005fc) },
    { 0x50008805, 108, 0x307, EVENT_SCRIPT(Func_0200060c) },
    { SCENE_EVENTS_END },
};
