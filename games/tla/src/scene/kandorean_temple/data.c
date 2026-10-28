#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "objcmd.h"

void Func_02000280(void);
void Func_0200028c(void);
void Func_020002a0(void);
void Func_020002dc(void);
void Func_0200035c(void);
void Func_020004a8(void);
void Func_02000710(void);
void Func_02001d00(void);
void Func_02001d20(void);
void Func_02001d40(void);
void Func_02001db4(void);
void Func_02001e0c(void);
void Func_02001e30(void);
void Func_02001e44(void);
void Func_02001e58(void);
void Func_02001f7c(void);
void Func_02002014(void);
void Func_020020c8(void);
void Func_020020d8(void);
void Func_02002120(void);
void Func_02002154(void);
void Func_020021b8(void);
void Func_020021dc(void);
void Func_02002248(void);
void Func_020022c0(void);
void Func_02002314(void);
void Func_020023e8(void);
void Func_02002674(void);
void Func_0200269c(void);
void Func_020026cc(void);
void Func_020027b4(void);
void Func_020027d4(void);
void Func_020027f4(void);
void Func_02002814(void);
void Func_02002834(void);
void Func_02003564(void);
void Func_02003574(void);
void Func_02003584(void);
void Func_0200393c(void);
void Func_020039a8(void);

extern u32 gData_020053e0[];
extern u32 gData_0200541c[];
extern u32 gData_02005458[];
extern u32 gData_02005514[];
extern s32 gData_02005634[];
extern s32 gData_0200564c[];
extern s32 gData_02005664[];

// clang-format off
u32 gData_020053e0[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_0200541c[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02005458[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

s32 gData_02005494[] = {
    0,
    44,
    22,
    38,
    0,
    3,
    0,
    1,
    38,
    46,
    (s32)Func_0200028c,
    22,
    38,
    0,
    3,
    0,
    1,
    38,
};

s16 gData_020054dc[] = {
    12, -1,
};

s16 gData_020054e0[] = {
    8, -1,
};

u32 gData_020054e4[] = {
    0, 24, 38,
};

u32 gData_020054f0[] = {
    0, 15, 3, 0, 1, 38,
};

s32 gData_02005508[] = {
    (s32)gData_020053e0,
    (s32)gData_0200541c,
    (s32)gData_02005458,
};

u32 gData_02005514[] = {
    SC_SET(SW_FACING, FACING_SOUTH + FACING_STEP),
    SC_WAIT(80),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_WAIT(80),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gData_02005548[] = {
    46,
    (s32)Func_020002a0,
};

s32 gData_02005550[] = {
    SC_CALL(Func_020002dc),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gData_02005564[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(456), 0, PIXELS(408)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_END,
};

u32 gData_020055a0[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(440), 0, PIXELS(392)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_WAIT(1),
    SC_END,
};

u32 gData_020055e4[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(496), 0, PIXELS(392)),
    SC_SYNC,
    SC_MOVE(PIXELS(504), 0, PIXELS(376)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

s32 gData_02005634[] = {
    SC_CALL(Func_0200035c),
    SC_SET(SW_Y, PIXELS(88)),
    SC_END,
};

s32 gData_0200564c[] = {
    SC_CALL(Func_0200035c),
    SC_SET(SW_Y, PIXELS(44)),
    SC_END,
};

s32 gData_02005664[] = {
    SC_CALL(Func_0200035c),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_020056a0[] = {
    0x4a0140, 0x1500060, 0x70005a, 0x2ffff, 0xffff, 0,
    0, 0,
};

u32 gData_020056c0[] = {
    0x2a0200, 0x2100110, 0x120003a, 0x2ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(14),
    SCENE_EXIT(1, 20, 1),
    SCENE_EXIT(2, 20, 3),
    SCENE_EXIT(3, 19, 1),
    SCENE_EXIT(4, 17, 2),
    SCENE_EXIT(5, 17, 5),
    SCENE_EXIT(6, 17, 3),
    SCENE_EXIT(7, 17, 4),
    SCENE_EXITS(15),
    SCENE_EXIT(1, 22, 9),
    SCENE_EXIT(2, 20, 4),
    SCENE_EXIT(3, 16, 1),
    SCENE_EXIT(4, 20, 2),
    SCENE_EXITS(16),
    SCENE_EXIT(1, 15, 3),
    SCENE_EXIT(2, 22, 8),
    SCENE_EXITS(17),
    SCENE_EXIT(1, 19, 2),
    SCENE_EXIT(2, 14, 4),
    SCENE_EXIT(3, 14, 5),
    SCENE_EXIT(4, 14, 6),
    SCENE_EXITS(18),
    SCENE_EXIT(1, 21, 3),
    SCENE_EXIT(2, 21, 4),
    SCENE_EXITS(19),
    SCENE_EXIT(1, 14, 3),
    SCENE_EXIT(2, 17, 1),
    SCENE_EXITS(20),
    SCENE_EXIT(1, 14, 1),
    SCENE_EXIT(2, 15, 4),
    SCENE_EXIT(3, 14, 2),
    SCENE_EXIT(4, 15, 2),
    SCENE_EXITS(21),
    SCENE_EXIT(1, 2, 4),
    SCENE_EXIT(2, 22, 1),
    SCENE_EXIT(3, 18, 1),
    SCENE_EXIT(4, 18, 2),
    SCENE_EXITS(22),
    SCENE_EXIT(1, 21, 2),
    SCENE_EXIT(2, 22, 4),
    SCENE_EXIT(3, 22, 5),
    SCENE_EXIT(4, 22, 2),
    SCENE_EXIT(5, 22, 3),
    SCENE_EXIT(6, 22, 7),
    SCENE_EXIT(7, 22, 6),
    SCENE_EXIT(8, 16, 2),
    SCENE_EXIT(9, 15, 1),
    SCENE_EXIT(13, 22, 13),
    SCENE_EXIT(14, 22, 14),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_020057a8[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020057c0[] = {
    {407, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_SOUTH, 2, 3},
    {272, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_SOUTH, 2, 2},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020058b0[] = {
    {368, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {368, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {450, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(504), FACING_SOUTH, 2, 0},
    {184, CONDITION_ALWAYS, 7, PIXELS(328), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {184, CONDITION_ALWAYS, 7, PIXELS(296), 0, PIXELS(140), FACING_SOUTH, 2, 0},
    {184, CONDITION_ALWAYS, 7, PIXELS(360), 0, PIXELS(140), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005958[] = {
    {272, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {351, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_WEST, 2, 2},
    {351, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_EAST, 2, 2},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005aa8[] = {
    {407, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 3},
    {91, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(340), FACING_SOUTH, 2, 0},
    {244, 0x4b, 1, PIXELS(632), 0, PIXELS(280), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005b20[] = {
    {307, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(224), FACING_SOUTH, 2, 0},
    {91, CONDITION_ALWAYS, (s32)gData_02005514, PIXELS(264), 0, PIXELS(424), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(424), FACING_SOUTH + FACING_STEP, 1, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(334), 0, PIXELS(317), FACING_SOUTHEAST + FACING_STEP, 2, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(284), 0, PIXELS(328), FACING_SOUTHEAST + FACING_STEP, 2, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(246), 0, PIXELS(318), FACING_SOUTH + FACING_STEP, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005be0[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(676), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(676), FACING_SOUTH, 2, 1},
    {407, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(376), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(376), FACING_SOUTH, 2, 1},
    {21, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(664), FACING_SOUTH, 3, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(696), FACING_SOUTH, 2, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(696), FACING_SOUTH, 2, 0},
    {91, CONDITION_ALWAYS, 1, PIXELS(400), 0, PIXELS(712), FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_02005634, PIXELS(439), 0, PIXELS(672), FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_02005634, PIXELS(359), 0, PIXELS(672), FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200564c, PIXELS(412), 0, PIXELS(324), FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_02005664, PIXELS(90), 0, PIXELS(230), FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_02005664, PIXELS(174), 0, PIXELS(72), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_02005d48[] = {
    {TRIGGERS_END},
};

struct Trigger gData_02005d54[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {0x31, 3, CONDITION_ALWAYS, 3},
    {0x31, 4, CONDITION_ALWAYS, 4},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_020020c8)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_020020d8)},
    {0x10008c15, 12, CONDITION_ALWAYS, EVENT(Func_020020c8)},
    {0x8c15, 12, CONDITION_ALWAYS, EVENT(Func_020020d8)},
    {2, 10, 0x211, EVENT(Func_02003584)},
    {2, 12, CONDITION_ALWAYS, EVENT(Func_0200393c)},
    {2, 13, CONDITION_ALWAYS, EVENT(Func_020039a8)},
    {TRIGGERS_END},
};

s16 gData_02005de4[] = {
    1, 0, 1, -1, 1, 0, 49, 0,
    2, -1, 2, 0, 33, 0, 3, -1,
    3, 0, 49, 0, 4, -1, 4, 0,
    -1, -1, 0, 0, 0, 0,
};

struct Trigger gData_02005e20[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {0x10008c15, 8, 0x304, EVENT(Func_020021b8)},
    {0x8c15, 8, 0x304, EVENT(Func_020021dc)},
    {0x602, 10, 0x304, EVENT(Func_02002248)},
    {0xc602, 11, 0x304, EVENT(Func_02002248)},
    {0x8602, 12, 0x304, EVENT(Func_02002248)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_02001d00)},
    {2, 6, 0x92d, EVENT(Func_020026cc)},
    {TRIGGERS_END},
};

struct Trigger gData_02005e8c[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_02002120)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02002154)},
    {0x10008c15, 8, CONDITION_ALWAYS, EVENT(Func_02002120)},
    {0x8c15, 8, CONDITION_ALWAYS, EVENT(Func_02002154)},
    {2, 20, CONDITION_ALWAYS, EVENT(Func_02003564)},
    {2, 21, CONDITION_ALWAYS, EVENT(Func_02003574)},
    {TRIGGERS_END},
};

struct Trigger gData_02005ef8[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gData_02005f10[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {2, 10, CONDITION_ALWAYS, EVENT(Func_02000710)},
    {TRIGGERS_END},
};

struct Trigger gData_02005f40[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {0x21, 4, CONDITION_ALWAYS, 4},
    {0x8515, 8, 0x20f, 0},
    {0, 11, CONDITION_ALWAYS, EVENT(Func_02000280)},
    {2, 10, CONDITION_ALWAYS, EVENT(Func_02002674)},
    {2, 11, CONDITION_ALWAYS, EVENT(Func_0200269c)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgFieldItSeemsIMayNotHave)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgFieldIsItReallyPossibleForAnyone)},
    {TRIGGERS_END},
};

struct Trigger gData_02005fc4[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {0x31, 3, CONDITION_ALWAYS, 3},
    {0x4602, 6, CONDITION_ALWAYS, EVENT(Func_02002014)},
    {0x1f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKandoreanTemple13))},
    {2, 20, 0x230, EVENT(Func_020004a8)},
    {0x4602, 4, 0x212, EVENT(Func_020022c0)},
    {0xc602, 5, 0x212, EVENT(Func_020022c0)},
    {3, 10, CONDITION_ALWAYS, EVENT(Func_02001d20)},
    {0x4e15, 8, CONDITION_ALWAYS, EVENT(Func_02002314)},
    {0x50008905, 30, CONDITION_ALWAYS, EVENT(Func_020027b4)},
    {0x50008905, 31, CONDITION_ALWAYS, EVENT(Func_020027d4)},
    {0x50008905, 32, CONDITION_ALWAYS, EVENT(Func_020027f4)},
    {0x50008905, 33, CONDITION_ALWAYS, EVENT(Func_02002814)},
    {0x50008905, 34, CONDITION_ALWAYS, EVENT(Func_02002834)},
    {0, 9, CONDITION_ALWAYS, EVENT(Func_02001d40)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_02001db4)},
    {0x8d15, 1033, 0x845, EVENT(Func_02001e30)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgFieldIDoNotUnderstandMasterPoi)},
    {0x8d15, 1034, 0x846, EVENT(Func_02001e44)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgFieldIWasHopingToHearMaster)},
    {0, 11, 0x849, MSGNO(MsgFieldIHopeToEnterTheTemple)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgFieldHeyWhatsAnOutsiderDoingOn)},
    {0, 12, 0x849, MSGNO(MsgFieldPleaseDontBreakMyConcentrationI)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgFieldLooksLikeNoneOfOurBrothers)},
    {0, 13, 0x849, MSGNO(MsgFieldIAmNotJustMeditatingI)},
    {0, 13, CONDITION_ALWAYS, MSGNO(MsgFieldSomedayImGoingToBeA)},
    {0x8d15, 11, 0x849, MSGNO(MsgFieldIStillCantBelieveThatThe)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgFieldGahIKeptMyEyesShut)},
    {0x8d15, 12, 0x849, MSGNO(MsgFieldMyMindIsEmptyMyBody)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgFieldIveBeenMeditatingAllDayBut)},
    {0x8d15, 13, 0x849, MSGNO(MsgFieldEmptyTheMindToLightenThe)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgFieldWheneverTheMasterGoesToTown)},
    {TRIGGERS_END},
};

struct Trigger gData_0200615c[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {1, 7, CONDITION_ALWAYS, 7},
    {0x202, 8, CONDITION_ALWAYS, EVENT(Func_02001e58)},
    {0xc602, 9, CONDITION_ALWAYS, EVENT(Func_02001f7c)},
    {0x8515, 8, 0x20f, 0},
    {0x8515, 10, 0x210, 0},
    {0, 13, 0x849, MSGNO(MsgFieldHeWentInBeforeMeWhat)},
    {0, 13, CONDITION_ALWAYS, EVENT(Func_02001e0c)},
    {0, 14, 0x849, MSGNO(MsgFieldEmptyMindEmptyBodyByClearing)},
    {0, 14, CONDITION_ALWAYS, MSGNO(MsgFieldIHearThatSomeonePassedThe)},
    {0, 15, CONDITION_ALWAYS, MSGNO(MsgFieldWhenIAwokeILearnedThat)},
    {0x8d15, 13, 0x849, MSGNO(MsgFieldTheresNoWayImRootingFor)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgFieldTheMasterSeemedSoDisappointedThat)},
    {0x8d15, 14, 0x849, MSGNO(MsgFieldArrghIWasSoCloseI)},
    {0x8d15, 14, CONDITION_ALWAYS, MSGNO(MsgFieldIKnewItItShouldHave)},
    {0x8d15, 15, CONDITION_ALWAYS, MSGNO(MsgFieldBeforeIPassedOutISaw)},
    {3, 16, 0x84a, EVENT(Func_020023e8)},
    {0, 12, 0x84a, MSGNO(MsgFieldTakeThePowerOfLashIt)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouWishToPracticeThese)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgFieldHoHoIFeelTheStrength)},
    {TRIGGERS_END},
};
