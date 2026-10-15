#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void Func_02000038(void);
void Func_02000058(void);
void Func_020003f8(void);
void Func_02000584(void);
void Func_020005bc(void);
void Func_02000600(void);
void Func_02000610(void);
void Func_02000660(void);
void Func_020006f4(void);

extern u8 gData_02001324[];
extern u8 gData_02001360[];
extern u8 gData_0200139c[];

extern s32 gData_0200168c[];

s32 gData_0200168c[] = {
	46,
	(s32)Func_02000038,
	17,
};

s16 gData_02001698[] = {
	0, -1, 680, 0, 280, 0x4000, 0, -1,
	-1, -1, -1, 0, 1, -1, 536, 0,
	488, 0, 0, 512, 0, 0x800, 864, 0,
	2, -1, 0x7e8, 0, 792, 0x8000, 0, 512,
	0, 0x800, 864, 0, 3, -1, 0x628, 0,
	680, 0x4000, 0, 512, 0, 0x800, 864, 0,
	99, -1, 0x4f8, 0, 552, 0x4000, 0, 512,
	0, 0x800, 864, 0, -1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};

s32 gData_02001728[] = {
	-0x10000, 168, 0x40000118, -0x10000, -1, 0xffff,
	0xffff, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
	SCENE_EXITS(102),
	SCENE_EXIT_IF(1, 2, 21, 0xffffffff),
	SCENE_EXIT_IF(2, 2, 22, 0xffffffff),
	SCENE_EXIT_IF(3, 104, 1, 0xffffffff),
	SCENE_EXITS(103),
	SCENE_EXIT_IF(1, 2, 85, 0xffffffff),
	SCENE_EXIT_IF(2, 2, 86, 0xffffffff),
	SCENE_EXITS_END,
};

u32 gData_0200178c[] = {
	0xffff0197, 1, 0x2d80000, 0, 0xe80000, 0x2c000,
	0xffff0197, 1, 0x3880000, 0, 0xe80000, 0x2c000,
	0xffff0197, 1, 0x6180000, 0, 0x2f80000, 0x2c000,
	0xffff0197, 1, 0x6580000, 0, 0x2f80000, 0x2c000,
	0xffff0122, 1, 0x5180000, 0, 0x1c80000, 0x4000,
	0xffff0122, 1, 0x7580000, 0, 0x2380000, 0x4000,
	0xffff0133, 1, 0x6280000, 0, 0x2a00000, 0x2c000,
	0xffff0111, 1, 0x4d00000, 0, 0x2200000, 0x24000,
	0xffff00bd, 1, 0x7880000, 0, 0x2d80000, 0x28000,
	0xffff01e9, 1, 0, 0, 0, 0x4000,
	0x8ab0053, 1, 0x3780000, 0, 0xc80000, 0x18000,
	0x8ab0053, 1, 0x3780000, 0, 0xe80000, 0x18000,
	0x8e601a8, 1, 0x3300000, 0, 0xe80000, 0x28000,
	0x6100f5, 1, 0x5380000, 0, 0x2880000, 0x4000,
	0x18f30005, 1, 0, 0, 0, 0,
	0x18f30006, 1, 0, 0, 0, 0,
	0x18f30038, 1, 0, 0, 0, 0,
	0xffff, 0, 0, 0, 0, 0,
};

s32 gData_0200193c[] = {
	(s32)0xffff0008,
	(s32)gData_0200168c,
	0x14a0000,
	0,
	0x4f0000,
	0x20000,
	(s32)0xffff0197,
	1,
	0xd80000,
	0,
	0xe80000,
	0x2c000,
	(s32)0xffff0197,
	1,
	0x1880000,
	0,
	0xe80000,
	0x2c000,
	0x8e601a8,
	1,
	0x1300000,
	0,
	0xe80000,
	0x28000,
	0xffff,
	0,
	0,
	0,
	0,
	0,
};

s32 gData_020019b4[] = {
	(s32)gData_02001324,
	(s32)gData_02001360,
	(s32)gData_0200139c,
};

s32 gData_020019c0[] = {
	1,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	0x9815,
	(s32)0xffff000b,
	(s32)Func_020006f4,
	-1,
	0,
	0,
};

s32 gData_020019f0[] = {
	1,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	81,
	0x18850003,
	3,
	0,
	(s32)0xffff0010,
	(s32)Func_02000660,
	0x8d15,
	0x8860010,
	MSGNO(MsgDigHereYou),
	0x8d15,
	(s32)0xffff0010,
	MSGNO(MsgIKnewIt),
	0,
	(s32)0xffff0012,
	MSGNO(MsgITriedTo),
	0,
	(s32)0xffff0013,
	MSGNO(MsgIWonderWho),
	0x8d15,
	(s32)0xffff0012,
	MSGNO(MsgIfTheOther),
	0x8d15,
	(s32)0xffff0013,
	MSGNO(MsgILongTo),
	0,
	(s32)0xffff0015,
	(s32)Func_02000058,
	0x8515,
	0x200000a,
	0,
	0x1815,
	0x201000c,
	(s32)Func_02000584,
	0x1815,
	0x202000d,
	(s32)Func_020005bc,
	0x4e15,
	(s32)0xffff000e,
	(s32)Func_02000600,
	0x8c15,
	0x884000f,
	(s32)Func_020003f8,
	0x50008805,
	0x886000a,
	(s32)Func_02000610,
	-1,
	0,
	0,
};

s32 gData_02001ac8[] = {
	0x2000200, 0x3600400, 0x600, 0x1601000, -1, -1,
};
