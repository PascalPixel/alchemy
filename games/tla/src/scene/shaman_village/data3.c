#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"

void Func_02000280(void);
void Func_02000d08(void);
void Func_02000d28(void);
void Func_02000d48(void);
void Func_02000d68(void);
void Func_02000e3c(void);
void Func_02000ef8(void);
void Func_02000f2c(void);
void Func_02000f60(void);
void Func_02000fc4(void);
void Func_020010c8(void);
void Func_02001118(void);
void Func_0200113c(void);
void Func_020011c0(void);
void Func_0200122c(void);
void Func_02001380(void);
void Func_02001644(void);
void Func_02001764(void);
void Func_02001880(void);
void Func_02001aa4(void);
void Func_02001cc8(void);
void Func_02001d3c(void);
void Func_02002068(void);
void Func_0200279c(void);
void Func_020027c0(void);
void Func_020027e4(void);
void Func_02002808(void);
void Func_02002878(void);
void Func_020030bc(void);
void Func_02003c58(void);
void Func_02003c74(void);
void Func_02003c90(void);
void Func_02003cac(void);
void Func_02003cc8(void);
void Func_02003ce4(void);

// clang-format off
u32 gData_02005204[] = {
    0x801000, 0x20000001, 0x10000, 0x1001000, 0x20000006, 0x60010,
    0x3c7fff, 0x1801000, 0x7fff0006, 0x10000006, 0x60110, 0x67fff,
    0x1901000, 0x7fff0006, 0x10000006, 0x60120, 0x67fff, 0x2000,
    0x1000001e, 0x601a0, 0x67fff, 0x1301000, 0x7fff0006, 0x10000006,
    0x601b0, 0x67fff, 0x1401000, 0x7fff0006, 0x10000006, 0x601c0,
    0x67fff, 0xffff,
};

struct SceneEntrance gData_02005284[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gData_020052b4[] = {
    {15, CONDITION_ALWAYS, 544, 0, 680, FACING_NORTH, 0, 420, 560, 660, 720, 0},
    {16, CONDITION_ALWAYS, 752, 0, 680, FACING_NORTH, 0, 630, 560, 870, 720, 0},
    {17, CONDITION_ALWAYS, 544, 0, 904, FACING_NORTH, 0, 420, 780, 660, 940, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gData_02005314[] = {
    {6, CONDITION_ALWAYS, 720, 0, 544, FACING_NORTH, 0, 600, 380, 840, 600, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(241),
    SCENE_EXIT(1, 247, 31),
    SCENE_EXIT(2, 247, 32),
    SCENE_EXIT(3, 243, 1),
    SCENE_EXIT(10, 247, 50),
    SCENE_EXIT(11, 247, 31),
    SCENE_EXIT(12, 247, 32),
    SCENE_EXITS(243),
    SCENE_EXIT(1, 241, 3),
    SCENE_EXIT(2, 242, 8),
    SCENE_EXIT(3, 242, 11),
    SCENE_EXIT(4, 245, 17),
    SCENE_EXIT(5, 242, 14),
    SCENE_EXIT(6, 242, 9),
    SCENE_EXITS(242),
    SCENE_EXIT(1, 2, 58),
    SCENE_EXIT(2, 245, 15),
    SCENE_EXIT(3, 245, 16),
    SCENE_EXIT(4, 244, 6),
    SCENE_EXIT(5, 244, 8),
    SCENE_EXIT(6, 244, 11),
    SCENE_EXIT(7, 244, 13),
    SCENE_EXIT(8, 243, 2),
    SCENE_EXIT(9, 243, 6),
    SCENE_EXIT(10, 246, 6),
    SCENE_EXIT(11, 243, 3),
    SCENE_EXIT(12, 244, 3),
    SCENE_EXIT(13, 244, 1),
    SCENE_EXIT(14, 243, 5),
    SCENE_EXITS(244),
    SCENE_EXIT(1, 242, 13),
    SCENE_EXIT(2, 244, 4),
    SCENE_EXIT(3, 242, 12),
    SCENE_EXIT(4, 244, 2),
    SCENE_EXIT(5, 244, 9),
    SCENE_EXIT(6, 242, 4),
    SCENE_EXIT(7, 244, 10),
    SCENE_EXIT(8, 242, 5),
    SCENE_EXIT(9, 244, 5),
    SCENE_EXIT(10, 244, 7),
    SCENE_EXIT(11, 242, 6),
    SCENE_EXIT(12, 244, 14),
    SCENE_EXIT(13, 242, 7),
    SCENE_EXIT(14, 244, 12),
    SCENE_EXITS(245),
    SCENE_EXIT(15, 242, 2),
    SCENE_EXIT(16, 242, 3),
    SCENE_EXIT(17, 243, 4),
    SCENE_EXITS(246),
    SCENE_EXIT(6, 242, 10),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_02005410[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005428[] = {
    {305, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_EAST, 2, 2},
    {305, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_EAST, 2, 2},
    {382, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_EAST, 2, 2},
    {41, CONDITION_ALWAYS, 1, PIXELS(224), 0, PIXELS(280), FACING_SOUTH, 3, 0},
    {40, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(264), FACING_SOUTH, 3, 0},
    {40, CONDITION_ALWAYS, 1, PIXELS(240), 0, PIXELS(264), FACING_SOUTH, 3, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005590[] = {
    {305, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(120), FACING_EAST, 2, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(120), FACING_EAST, 2, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(216), FACING_EAST, 2, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(632), 0, PIXELS(216), FACING_EAST, 2, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(552), 0, PIXELS(264), FACING_EAST, 2, 0},
    {305, CONDITION_ALWAYS, 1, PIXELS(648), 0, PIXELS(264), FACING_EAST, 2, 0},
    {272, CONDITION_ALWAYS, 7, PIXELS(536), 0, PIXELS(184), FACING_EAST, 2, 0},
    {112, CONDITION_ALWAYS, 3, PIXELS(88), 0, PIXELS(336), FACING_EAST, 1, 0},
    {365, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(296), FACING_SOUTH, 0, 0},
    {46, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(280), FACING_SOUTH, 0, 0},
    {246, 0x79, 1, PIXELS(680), 0, PIXELS(248), FACING_WEST, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020056b0[] = {
    {115, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(428), 0x3000, 3, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(248), FACING_EAST, 1, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(364), 0, PIXELS(248), FACING_WEST, 1, 0},
    {107, CONDITION_ALWAYS, 1, PIXELS(254), 0, PIXELS(434), 0x5000, 1, 0},
    {113, CONDITION_ALWAYS, 3, PIXELS(434), 0, PIXELS(446), 0xb000, 1, 0},
    {136, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(200), 0x5000, 1, 0},
    {138, CONDITION_ALWAYS, 3, PIXELS(412), 0, PIXELS(230), 0x3000, 1, 0},
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 1, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 1, 0},
    {40, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 1, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(216), FACING_SOUTH, 2, 1},
    {299, CONDITION_ALWAYS, 7, PIXELS(40), 0, PIXELS(288), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005860[] = {
    {116, CONDITION_ALWAYS, 1, PIXELS(112), 0, PIXELS(90), 0xb000, 1, 0},
    {136, CONDITION_ALWAYS, 3, PIXELS(110), 0, PIXELS(172), FACING_SOUTH, 1, 0},
    {138, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(126), 0x3000, 1, 0},
    {41, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 3, 0},
    {489, CONDITION_ALWAYS, 7, PIXELS(264), 0, PIXELS(381), 0x5000, 0, 0},
    {134, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(381), 0x5000, 0, 0},
    {135, CONDITION_ALWAYS, 3, PIXELS(140), 0, PIXELS(388), 0x3000, 1, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(694), FACING_WEST, 0, 0},
    {128, CONDITION_ALWAYS, 3, PIXELS(144), 0, PIXELS(658), FACING_NORTH, 1, 0},
    {115, CONDITION_ALWAYS, 1, PIXELS(164), 0, PIXELS(432), 0x5000, 1, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(460), FACING_EAST, 1, 0},
    {211, CONDITION_ALWAYS, 1, PIXELS(114), 0, PIXELS(364), 0xb000, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005998[] = {
    {112, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(678), 0xb000, 0, 0},
    {113, CONDITION_ALWAYS, 1, PIXELS(508), 0, PIXELS(656), 0x3000, 0, 0},
    {136, CONDITION_ALWAYS, 1, PIXELS(542), 0, PIXELS(628), 0x3000, 0, 0},
    {138, CONDITION_ALWAYS, 1, PIXELS(550), 0, PIXELS(640), 0xb000, 0, 0},
    {111, CONDITION_ALWAYS, 1, PIXELS(752), 0, PIXELS(648), 0x5000, 1, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(724), 0, PIXELS(628), 0x3000, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005a40[] = {
    {201, CONDITION_ALWAYS, 1, PIXELS(720), 0, PIXELS(466), 0x3000, 1, 0},
    {112, CONDITION_ALWAYS, 1, PIXELS(746), 0, PIXELS(516), 0xb000, 1, 0},
    {113, CONDITION_ALWAYS, 1, PIXELS(700), 0, PIXELS(524), 0xd000, 1, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gData_02005aa0[] = {
    {TRIGGERS_END},
};

struct Trigger gData_02005aac[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {0x10004e15, 526, 0x94c, EVENT(Func_020030bc)},
    {0x4e15, 1038, 0x94c, 0},
    {2, 10, 0x9e0, EVENT(Func_02001380)},
    {2, 10, 0x201, EVENT(Func_02001764)},
    {2, 10, CONDITION_ALWAYS, EVENT(Func_02001644)},
    {2, 11, CONDITION_ALWAYS, EVENT(Func_02001880)},
    {2, 12, CONDITION_ALWAYS, EVENT(Func_02001aa4)},
    {0, 15, 0x94c, MSGNO(MsgShamanYegelosSandChallenge)},
    {0, 15, CONDITION_ALWAYS, MSGNO(MsgShamanChooseEitherRoad)},
    {0x8d15, 15, 0x94c, MSGNO(MsgShamanRodWasStolen)},
    {0x8d15, 15, CONDITION_ALWAYS, MSGNO(MsgShamanMoapaCannotLose)},
    {0, 17, 0x94c, MSGNO(MsgShamanTrialRoadAncientBattle)},
    {0, 17, CONDITION_ALWAYS, EVENT(Func_02000d68)},
    {0x8d15, 17, 0x94c, MSGNO(MsgShamanNeedContigoOrShamanPower)},
    {0x8d15, 17, CONDITION_ALWAYS, MSGNO(MsgShamanMoapaGreatestWarrior)},
    {0, 16, 0x94c, MSGNO(MsgShamanAncientHeroesVanishedSand)},
    {0, 16, CONDITION_ALWAYS, EVENT(Func_02000e3c)},
    {0x8d15, 16, 0x94c, MSGNO(MsgShamanSandChallengeWillFail)},
    {0x8d15, 16, CONDITION_ALWAYS, MSGNO(MsgShamanJadeNeedsYegelosHeir)},
    {TRIGGERS_END},
};

struct Trigger gData_02005bc0[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {0xce01, 4, 0x194d, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {0, 18, CONDITION_ALWAYS, EVENT(Func_02000280)},
    {0x8c15, 14, 0x94e, EVENT(Func_02001cc8)},
    {0xc403, 22, 0x94d, EVENT(Func_02000d28)},
    {3, 10, CONDITION_ALWAYS, EVENT(Func_02000d48)},
    {2, 11, 0x203, EVENT(Func_02002808)},
    {0xc400, 17, CONDITION_ALWAYS, EVENT(Func_0200122c)},
    {0x8d15, 17, 0xa21, MSGNO(MsgShamanCloudyFortuneBeforeTrial)},
    {0x8d15, 17, CONDITION_ALWAYS, MSGNO(MsgShamanCloudyFortuneAfterTrial)},
    {0, 15, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {0, 15, CONDITION_ALWAYS, MSGNO(MsgShamanNewLegendaryBattle)},
    {0x8d15, 15, 0x9fe, MSGNO(MsgShamanAncestorsDistrustOutsiders)},
    {0x8d15, 15, CONDITION_ALWAYS, MSGNO(MsgShamanContigoHeroVictorySurprise)},
    {TRIGGERS_END},
};

struct Trigger gData_02005ca4[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {0xce01, 2, 0x194d, 2},
    {0xce01, 3, 0x194d, 3},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_02003c58)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_02003c74)},
    {0xc602, 6, 0x194d, EVENT(Func_02003c90)},
    {0xc602, 7, 0x194d, EVENT(Func_02003cac)},
    {1, 8, CONDITION_ALWAYS, 8},
    {1, 9, CONDITION_ALWAYS, 9},
    {0xce01, 10, CONDITION_ALWAYS, 10},
    {1, 11, CONDITION_ALWAYS, 11},
    {0xc602, 12, CONDITION_ALWAYS, EVENT(Func_02003cc8)},
    {0xc602, 13, 0x94f, EVENT(Func_02001d3c)},
    {0xc602, 13, CONDITION_ALWAYS, EVENT(Func_02003ce4)},
    {1, 14, CONDITION_ALWAYS, 14},
    {0x8515, 19, 0x204, 0},
    {0xc15, 21, 0x205, EVENT(Func_02002878)},
    {0xc403, 20, 0x94d, EVENT(Func_02000d08)},
    {0xc403, 22, 0x94d, EVENT(Func_02000d28)},
    {0xc403, 21, 0x94f, EVENT(Func_02001d3c)},
    {0, 8, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgShamanMoapaImpressedByChallenge)},
    {0x8d15, 8, 0x9fe, MSGNO(MsgShamanWaitForOutsidersToLeave)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgShamanMoapaPraisesOpponents)},
    {0, 9, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgShamanRespectForeignWarriors)},
    {0x8d15, 9, 0x9fe, MSGNO(MsgShamanChiefForbidsTalkingToOutsiders)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgShamanDoubtMoapaDefeat)},
    {0, 10, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgShamanVictoryProvesSkill)},
    {0x8d15, 10, 0x9fe, MSGNO(MsgShamanWhoInvitedOutsiders)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgShamanHopeForStrongerShamanChild)},
    {0, 11, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgShamanForeignersCanBeHeroes)},
    {0x8d15, 11, 0x9fe, MSGNO(MsgShamanRefuseToTalkToOutsiders)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgShamanWorldShouldWelcomeStrangers)},
    {0, 12, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgShamanHoabnaPromiseFulfilled)},
    {0x8d15, 12, 0x9fe, MSGNO(MsgShamanTrustContigoFriends)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgShamanRemainFriendsWithContigo)},
    {0, 13, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {0, 13, CONDITION_ALWAYS, EVENT(Func_02001118)},
    {0x8d15, 13, 0x9fe, MSGNO(MsgShamanDistrustFriendlyOutsiders)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgShamanWishToBreakSandWalls)},
    {0, 14, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {0, 14, CONDITION_ALWAYS, MSGNO(MsgShamanChosenFewHavePower)},
    {0x8d15, 14, 0x9fe, MSGNO(MsgShamanShutDoorsAgainstOutsiders)},
    {0x8d15, 14, CONDITION_ALWAYS,
     MSGNO(MsgShamanOutskirtsHaveDifferentPowers)},
    {0, 15, CONDITION_ALWAYS, EVENT(Func_02000ef8)},
    {0x8d15, 15, CONDITION_ALWAYS, MSGNO(MsgShamanOutsidersAreNotContigo)},
    {0, 17, 0x9fe, EVENT(Func_02000f2c)},
    {0, 17, CONDITION_ALWAYS, MSGNO(MsgShamanGuardPromisesRematchVictory)},
    {0x8d15, 17, 0x9fe, MSGNO(MsgShamanContigoTempleTreasure)},
    {0x8d15, 17, CONDITION_ALWAYS, MSGNO(MsgShamanClimbingTrialRoadIsHard)},
    {0, 16, 0x9fe, EVENT(Func_02000f60)},
    {0, 16, CONDITION_ALWAYS, MSGNO(MsgShamanVillagersBlameGuardDefeat)},
    {0x8d15, 16, 0x9fe, MSGNO(MsgShamanMoapaSparedOutsiders)},
    {0x8d15, 16, CONDITION_ALWAYS, MSGNO(MsgShamanGuardBlamesMountainClimb)},
    {0x4114, 15, 0x9e1, EVENT(Func_02002068)},
    {0x1ff14, 15, CONDITION_ALWAYS, EVENT(Func_0200279c)},
    {0x1ff14, 16, CONDITION_ALWAYS, EVENT(Func_020027c0)},
    {0x1ff14, 17, CONDITION_ALWAYS, EVENT(Func_020027e4)},
    {TRIGGERS_END},
};

struct Trigger gData_02005f98[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {1, 7, CONDITION_ALWAYS, 7},
    {1, 8, CONDITION_ALWAYS, 8},
    {1, 9, CONDITION_ALWAYS, 9},
    {1, 10, CONDITION_ALWAYS, 10},
    {1, 11, CONDITION_ALWAYS, 11},
    {1, 12, CONDITION_ALWAYS, 12},
    {1, 13, CONDITION_ALWAYS, 13},
    {1, 14, CONDITION_ALWAYS, 14},
    {0, 8, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgShamanMoapaWantsToKnowNames)},
    {0x8d15, 8, 0x9fe, MSGNO(MsgShamanMoapaLeftDoorOpen)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgShamanShameNotKnowingVictors)},
    {0, 9, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgShamanFormalBattleNeedsNames)},
    {0x8d15, 9, 0x9fe, MSGNO(MsgShamanFatherNeverLoses)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgShamanFatherLostToRudeOutsiders)},
    {0, 10, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgShamanNamesDoNotMatter)},
    {0x8d15, 10, 0x9fe, MSGNO(MsgShamanFirstForeignVisitors)},
    {0x8d15, 10, CONDITION_ALWAYS,
     MSGNO(MsgShamanNamesBeforeBattleOldFashioned)},
    {0, 11, CONDITION_ALWAYS, EVENT(Func_0200113c)},
    {0x8d15, 11, 0x9ed, MSGNO(MsgShamanVictorsDeserveRespect)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgShamanVictorsNamesOnTrialColumns)},
    {0, 13, CONDITION_ALWAYS, EVENT(Func_02000fc4)},
    {0x8d15, 13, 0x9fe, MSGNO(MsgShamanPayingStrangersMayStay)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgShamanMoapaMiserableAfterDefeat)},
    {0, 14, 0x9fe, MSGNO(MsgShamanSilentVillager)},
    {0, 14, CONDITION_ALWAYS, MSGNO(MsgShamanForgiveUnkindVillagers)},
    {0x8d15, 14, 0x9fe, MSGNO(MsgShamanMoapaAllowsStrangersToStay)},
    {0x8d15, 14, CONDITION_ALWAYS, MSGNO(MsgShamanVillagersWorryTooMuch)},
    {0, 15, CONDITION_ALWAYS, MSGNO(MsgShamanFinallyOpenLockedDoor)},
    {0x8d15, 15, CONDITION_ALWAYS, MSGNO(MsgShamanQuietWhileLockedInside)},
    {0, 16, CONDITION_ALWAYS, MSGNO(MsgShamanMissedTrialRoadChallenge)},
    {0x8d15, 16, CONDITION_ALWAYS, MSGNO(MsgShamanHidingLawPreventedWatching)},
    {0, 17, CONDITION_ALWAYS, MSGNO(MsgShamanEnjoyGettingOutside)},
    {0x8d15, 17, CONDITION_ALWAYS, MSGNO(MsgShamanChildrenIgnoreTradition)},
    {0, 18, CONDITION_ALWAYS, MSGNO(MsgShamanPlanPicnicMeal)},
    {0x8d15, 18, CONDITION_ALWAYS, MSGNO(MsgShamanEagerForDinner)},
    {0, 19, CONDITION_ALWAYS, MSGNO(MsgShamanSimpleMeatPotatoesBeans)},
    {0x8d15, 19, CONDITION_ALWAYS, MSGNO(MsgShamanSaltAndPepperOnly)},
    {0x173, 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanRoastedRiverFish))},
    {0x173, 201, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanTraditionalBoneBroth))},
    {0x173, 202, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanStrangeBoiledVegetables))},
    {TRIGGERS_END},
};

struct Trigger gData_020061f0[] = {
    {1, 15, CONDITION_ALWAYS, 15},
    {1, 16, CONDITION_ALWAYS, 16},
    {1, 17, CONDITION_ALWAYS, 17},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgShamanAncientHesperiaAttekaWar)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgShamanNewContigoHeroStronger)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgShamanHoabnaYegelosDecideWar)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgShamanDoubtShamanSuperiority)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgShamanTownsChooseChampions)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgShamanGrandparentsUpsetByDefeat)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgShamanYegelosBraveJourney)},
    {0x8d15, 11, CONDITION_ALWAYS,
     MSGNO(MsgShamanGrandparentsTiredOfWarStories)},
    {0, 12, CONDITION_ALWAYS, EVENT(Func_020011c0)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgShamanHusbandWorriesAboutWeapons)},
    {0, 13, CONDITION_ALWAYS, MSGNO(MsgShamanWarehouseOwnerFeelsResponsible)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgShamanWeaponsDidNotCauseLoss)},
    {0x173, 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanRoastedRiverFish))},
    {0x173, 201, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanTraditionalBoneBroth))},
    {0x173, 202, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgShamanStrangeBoiledVegetables))},
    {TRIGGERS_END},
};

struct Trigger gData_020062d4[] = {
    {1, 6, CONDITION_ALWAYS, 6},
    {0, 8, CONDITION_ALWAYS, EVENT(Func_020010c8)},
    {0x8d15, 8, 0x9fe, MSGNO(MsgShamanVisitorsMustFindMoapa)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgShamanFearRenewedContigoWar)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgShamanThreatenContigoRematch)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgShamanMoapaMustHaveBeenHungry)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgShamanPrayForShamanWarrior)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgShamanPrayForContigoConqueror)},
    {TRIGGERS_END},
};

u32 gDriftScene_SpawnScript[] = {
    0,
    46,
    38,
};
