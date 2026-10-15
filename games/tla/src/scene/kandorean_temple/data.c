#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

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

u32 gData_020053e0[] = {
	0, 1, 13, 22, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38, 17,
};

u32 gData_0200541c[] = {
	0, 1, 13, 44, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38, 17,
};

u32 gData_02005458[] = {
	0, 1, 13, 126, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38, 17,
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
	22, 3, 0x5000, 0, 80, 22,
	3, 0x3000, 0, 80, 13, 0xffff,
	0,
};

s32 gData_02005548[] = {
	46,
	(s32)Func_020002a0,
};

s32 gData_02005550[] = {
	46,
	(s32)Func_020002dc,
	13,
	0xffff,
	0,
};

u32 gData_02005564[] = {
	39, 2, 4, 0x1c80000, 0, 0x1980000,
	1, 39, 1, 0, 1, 22,
	3, 0xd000, 17,
};

u32 gData_020055a0[] = {
	39, 2, 4, 0x1b80000, 0, 0x1880000,
	1, 39, 1, 0, 1, 22,
	3, 0xd000, 0, 1, 17,
};

u32 gData_020055e4[] = {
	39, 2, 4, 0x1f00000, 0, 0x1880000,
	1, 4, 0x1f80000, 0, 0x1780000, 1,
	39, 1, 22, 3, 0x8000, 0,
	1, 17,
};

s32 gData_02005634[] = {
	46,
	(s32)Func_0200035c,
	22,
	6,
	0x580000,
	17,
};

s32 gData_0200564c[] = {
	46,
	(s32)Func_0200035c,
	22,
	6,
	0x2c0000,
	17,
};

s32 gData_02005664[] = {
	46,
	(s32)Func_0200035c,
	17,
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

u32 gData_020057a8[] = {
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_020057c0[] = {
	0xffff0197, 1, 1, 0, 0, 0x2024000,
	0xffff0197, 1, 2, 0, 0, 0x3024000,
	0xffff0197, 1, 3, 0, 0, 0x3024000,
	0xffff0197, 1, 4, 0, 0, 0x3024000,
	0xffff0110, 1, 5, 0, 0, 0x2024000,
	0xffff015f, 1, 0, 0, 0, 0x24000,
	0xffff015f, 1, 0, 0, 0, 0x24000,
	0xffff015f, 1, 0, 0, 0, 0x24000,
	0xffff015f, 1, 0, 0, 0, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_020058b0[] = {
	0xffff0170, 1, 1, 0, 0, 0x2024000,
	0xffff0170, 1, 1, 0, 0, 0x2024000,
	0xffff01c2, 1, 0x1380000, 0, 0x1f80000, 0x24000,
	0xffff00b8, 7, 0x1480000, 0, 0x980000, 0x24000,
	0xffff00b8, 7, 0x1280000, 0, 0x8c0000, 0x24000,
	0xffff00b8, 7, 0x1680000, 0, 0x8c0000, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02005958[] = {
	0xffff0110, 1, 1, 0, 0, 0x2024000,
	0xffff015f, 1, 2, 0, 0, 0x2028000,
	0xffff015f, 1, 3, 0, 0, 0x2020000,
	0xffff015f, 1, 0, 0, 0, 0x24000,
	0xffff015f, 1, 0, 0, 0, 0x24000,
	0xffff015f, 1, 0, 0, 0, 0x24000,
	0xffff015f, 1, 0, 0, 0, 0x24000,
	0xffff015f, 1, 0, 0, 0, 0x24000,
	0xffff015f, 1, 0, 0, 0, 0x24000,
	0xffff015f, 1, 0, 0, 0, 0x24000,
	0xffff015f, 1, 0, 0, 0, 0x24000,
	0xffff015f, 1, 0, 0, 0, 0x24000,
	0xffff015f, 1, 0, 0, 0, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02005aa8[] = {
	0xffff0197, 1, 1, 0, 0, 0x2024000,
	0xffff0197, 1, 2, 0, 0, 0x3024000,
	0xffff005b, 1, 0xa80000, 0, 0x1540000, 0x24000,
	0x4b00f4, 1, 0x2780000, 0, 0x1180000, 0x4000,
	0xffff, 0, 0, 0, 0, 0,
};

s32 gData_02005b20[] = {
	(s32)0xffff0133,
	1,
	0x580000,
	0,
	0xe00000,
	0x24000,
	(s32)0xffff005b,
	(s32)gData_02005514,
	0x1080000,
	0,
	0x1a80000,
	0x13000,
	(s32)0xffff005b,
	1,
	0x1380000,
	0,
	0x1a80000,
	0x15000,
	(s32)0xffff005b,
	1,
	0x14e0000,
	0,
	0x13d0000,
	0x23000,
	(s32)0xffff005b,
	1,
	0x11c0000,
	0,
	0x1480000,
	0x23000,
	(s32)0xffff005b,
	1,
	0xf60000,
	0,
	0x13e0000,
	0x25000,
	(s32)0xffff0038,
	1,
	0,
	0,
	0,
	0x24000,
	0xffff,
	0,
	0,
	0,
	0,
	0,
};

s32 gData_02005be0[] = {
	(s32)0xffff0197,
	1,
	0x1c80000,
	0,
	0x2a40000,
	0x24000,
	(s32)0xffff0197,
	1,
	0x1f80000,
	0,
	0x2a40000,
	0x1024000,
	(s32)0xffff0197,
	1,
	0x1e80000,
	0,
	0x1780000,
	0x24000,
	(s32)0xffff0197,
	1,
	0x1b80000,
	0,
	0x1780000,
	0x1024000,
	(s32)0xffff0015,
	1,
	0x1c80000,
	0,
	0x2980000,
	0x34000,
	(s32)0xffff005b,
	1,
	0x1e80000,
	0,
	0x2b80000,
	0x24000,
	(s32)0xffff005b,
	1,
	0x1380000,
	0,
	0x2b80000,
	0x24000,
	(s32)0xffff005b,
	1,
	0x1900000,
	0,
	0x2c80000,
	0x24000,
	(s32)0xffff0038,
	1,
	0,
	0,
	0,
	0x24000,
	(s32)0xffff0132,
	(s32)gData_02005634,
	0x1b70000,
	0,
	0x2a00000,
	0x24000,
	(s32)0xffff0132,
	(s32)gData_02005634,
	0x1670000,
	0,
	0x2a00000,
	0x24000,
	(s32)0xffff0132,
	(s32)gData_0200564c,
	0x19c0000,
	0,
	0x1440000,
	0x24000,
	(s32)0xffff0132,
	(s32)gData_02005664,
	0x5a0000,
	0,
	0xe60000,
	0x24000,
	(s32)0xffff0132,
	(s32)gData_02005664,
	0xae0000,
	0,
	0x480000,
	0x24000,
	0xffff,
	0,
	0,
	0,
	0,
	0,
};

s32 gData_02005d48[] = {
	-1, 0, 0,
};

s32 gData_02005d54[] = {
	1,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	49,
	(s32)0xffff0003,
	3,
	49,
	(s32)0xffff0004,
	4,
	8,
	-0x10000,
	(s32)Func_020020c8,
	9,
	-0x10000,
	(s32)Func_020020d8,
	0x10008c15,
	(s32)0xffff000c,
	(s32)Func_020020c8,
	0x8c15,
	(s32)0xffff000c,
	(s32)Func_020020d8,
	2,
	0x211000a,
	(s32)Func_02003584,
	2,
	(s32)0xffff000c,
	(s32)Func_0200393c,
	2,
	(s32)0xffff000d,
	(s32)Func_020039a8,
	-1,
	0,
	0,
};

s16 gData_02005de4[] = {
	1, 0, 1, -1, 1, 0, 49, 0,
	2, -1, 2, 0, 33, 0, 3, -1,
	3, 0, 49, 0, 4, -1, 4, 0,
	-1, -1, 0, 0, 0, 0,
};

s32 gData_02005e20[] = {
	49,
	(s32)0xffff0001,
	1,
	0x10008c15,
	0x3040008,
	(s32)Func_020021b8,
	0x8c15,
	0x3040008,
	(s32)Func_020021dc,
	0x602,
	0x304000a,
	(s32)Func_02002248,
	0xc602,
	0x304000b,
	(s32)Func_02002248,
	0x8602,
	0x304000c,
	(s32)Func_02002248,
	0,
	(s32)0xffff000a,
	(s32)Func_02001d00,
	2,
	0x92d0006,
	(s32)Func_020026cc,
	-1,
	0,
	0,
};

s32 gData_02005e8c[] = {
	1,
	(s32)0xffff0001,
	1,
	33,
	(s32)0xffff0002,
	2,
	8,
	-0x10000,
	(s32)Func_02002120,
	9,
	-0x10000,
	(s32)Func_02002154,
	0x10008c15,
	(s32)0xffff0008,
	(s32)Func_02002120,
	0x8c15,
	(s32)0xffff0008,
	(s32)Func_02002154,
	2,
	(s32)0xffff0014,
	(s32)Func_02003564,
	2,
	(s32)0xffff0015,
	(s32)Func_02003574,
	-1,
	0,
	0,
};

s32 gData_02005ef8[] = {
	33, (s32)0xffff0001, 1, -1, 0, 0,
};

s32 gData_02005f10[] = {
	33,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	2,
	(s32)0xffff000a,
	(s32)Func_02000710,
	-1,
	0,
	0,
};

s32 gData_02005f40[] = {
	1,
	(s32)0xffff0001,
	1,
	33,
	(s32)0xffff0002,
	2,
	1,
	(s32)0xffff0003,
	3,
	33,
	(s32)0xffff0004,
	4,
	0x8515,
	0x20f0008,
	0,
	0,
	(s32)0xffff000b,
	(s32)Func_02000280,
	2,
	(s32)0xffff000a,
	(s32)Func_02002674,
	2,
	(s32)0xffff000b,
	(s32)Func_0200269c,
	0,
	(s32)0xffff000a,
	MSGNO(MsgFieldItSeemsIMayNotHave),
	0x8d15,
	(s32)0xffff000a,
	MSGNO(MsgFieldIsItReallyPossibleForAnyone),
	-1,
	0,
	0,
};

s32 gData_02005fc4[] = {
	1,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	49,
	(s32)0xffff0003,
	3,
	0x4602,
	(s32)0xffff0006,
	(s32)Func_02002014,
	499,
	(s32)0xffff00c8,
	0x00400000 | MSGNO(MsgKandoreanTemple13),
	2,
	0x2300014,
	(s32)Func_020004a8,
	0x4602,
	0x2120004,
	(s32)Func_020022c0,
	0xc602,
	0x2120005,
	(s32)Func_020022c0,
	3,
	(s32)0xffff000a,
	(s32)Func_02001d20,
	0x4e15,
	(s32)0xffff0008,
	(s32)Func_02002314,
	0x50008905,
	(s32)0xffff001e,
	(s32)Func_020027b4,
	0x50008905,
	(s32)0xffff001f,
	(s32)Func_020027d4,
	0x50008905,
	(s32)0xffff0020,
	(s32)Func_020027f4,
	0x50008905,
	(s32)0xffff0021,
	(s32)Func_02002814,
	0x50008905,
	(s32)0xffff0022,
	(s32)Func_02002834,
	0,
	(s32)0xffff0009,
	(s32)Func_02001d40,
	0,
	(s32)0xffff000a,
	(s32)Func_02001db4,
	0x8d15,
	0x8450409,
	(s32)Func_02001e30,
	0x8d15,
	(s32)0xffff0009,
	MSGNO(MsgFieldIDoNotUnderstandMasterPoi),
	0x8d15,
	0x846040a,
	(s32)Func_02001e44,
	0x8d15,
	(s32)0xffff000a,
	MSGNO(MsgFieldIWasHopingToHearMaster),
	0,
	0x849000b,
	MSGNO(MsgFieldIHopeToEnterTheTemple),
	0,
	(s32)0xffff000b,
	MSGNO(MsgFieldHeyWhatsAnOutsiderDoingOn),
	0,
	0x849000c,
	MSGNO(MsgFieldPleaseDontBreakMyConcentrationI),
	0,
	(s32)0xffff000c,
	MSGNO(MsgFieldLooksLikeNoneOfOurBrothers),
	0,
	0x849000d,
	MSGNO(MsgFieldIAmNotJustMeditatingI),
	0,
	(s32)0xffff000d,
	MSGNO(MsgFieldSomedayImGoingToBeA),
	0x8d15,
	0x849000b,
	MSGNO(MsgFieldIStillCantBelieveThatThe),
	0x8d15,
	(s32)0xffff000b,
	MSGNO(MsgFieldGahIKeptMyEyesShut),
	0x8d15,
	0x849000c,
	MSGNO(MsgFieldMyMindIsEmptyMyBody),
	0x8d15,
	(s32)0xffff000c,
	MSGNO(MsgFieldIveBeenMeditatingAllDayBut),
	0x8d15,
	0x849000d,
	MSGNO(MsgFieldEmptyTheMindToLightenThe),
	0x8d15,
	(s32)0xffff000d,
	MSGNO(MsgFieldWheneverTheMasterGoesToTown),
	-1,
	0,
	0,
};

s32 gData_0200615c[] = {
	1,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	1,
	(s32)0xffff0003,
	3,
	1,
	(s32)0xffff0004,
	4,
	1,
	(s32)0xffff0005,
	5,
	1,
	(s32)0xffff0006,
	6,
	1,
	(s32)0xffff0007,
	7,
	514,
	(s32)0xffff0008,
	(s32)Func_02001e58,
	0xc602,
	(s32)0xffff0009,
	(s32)Func_02001f7c,
	0x8515,
	0x20f0008,
	0,
	0x8515,
	0x210000a,
	0,
	0,
	0x849000d,
	MSGNO(MsgFieldHeWentInBeforeMeWhat),
	0,
	(s32)0xffff000d,
	(s32)Func_02001e0c,
	0,
	0x849000e,
	MSGNO(MsgFieldEmptyMindEmptyBodyByClearing),
	0,
	(s32)0xffff000e,
	MSGNO(MsgFieldIHearThatSomeonePassedThe),
	0,
	(s32)0xffff000f,
	MSGNO(MsgFieldWhenIAwokeILearnedThat),
	0x8d15,
	0x849000d,
	MSGNO(MsgFieldTheresNoWayImRootingFor),
	0x8d15,
	(s32)0xffff000d,
	MSGNO(MsgFieldTheMasterSeemedSoDisappointedThat),
	0x8d15,
	0x849000e,
	MSGNO(MsgFieldArrghIWasSoCloseI),
	0x8d15,
	(s32)0xffff000e,
	MSGNO(MsgFieldIKnewItItShouldHave),
	0x8d15,
	(s32)0xffff000f,
	MSGNO(MsgFieldBeforeIPassedOutISaw),
	3,
	0x84a0010,
	(s32)Func_020023e8,
	0,
	0x84a000c,
	MSGNO(MsgFieldTakeThePowerOfLashIt),
	0,
	(s32)0xffff000c,
	MSGNO(MsgFieldIfYouWishToPracticeThese),
	0x8d15,
	(s32)0xffff000c,
	MSGNO(MsgFieldHoHoIFeelTheStrength),
	-1,
	0,
	0,
};
