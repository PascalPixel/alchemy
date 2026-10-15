#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void Func_02000474(void);
void Func_020004cc(void);
void Func_0200051c(void);
void Func_0200052c(void);
void Func_02000558(void);
void Func_02000568(void);
void Func_0200072c(void);
void Func_02000778(void);
void Func_020007a0(void);
void Func_02000874(void);
void Func_020008f4(void);
void Func_0200092c(void);
void Func_0200093c(void);
void Func_02000a88(void);
void Func_02000c24(void);
void Func_02000c40(void);
void Func_02000c5c(void);
void Func_02000c78(void);

s32 gData_0200121c[] = {
	22, 26, 0, 23, 7, -0x8000,
	23, 9, -1024, 23, 10, -1024,
	0, 1, 13, 24, 0, 17,
};

u32 gData_02001264[] = {
	22, 9, 0x1000, 22, 10, 0x1000,
	0, 1, 0x80010000, 23, 9, 0x4000,
	23, 10, 0x2000, 0, 1, 13,
	4, 0xc0010000, 0x80020000, 23, 10, 0x2000,
	0, 1, 13, 4, 0xc0020000, 22,
	9, 0x10000, 22, 10, 0x10000, 17,
};

s32 gData_020012f4[] = {
	22, 9, 0x1000, 22, 10, 0x1000,
	0, 1, (s32)0x80010000, 23, 9, 0x8000,
	23, 10, 0x4000, 0, 1, 13,
	4, (s32)0xc0010000, (s32)0x80020000, 23, 9, -0x2000,
	23, 10, 0x6000, 0, 1, 13,
	4, (s32)0xc0020000, (s32)0x80030000, 23, 9, -0x2000,
	23, 10, -0x4000, 0, 1, 13,
	4, (s32)0xc0030000, 22, 9, 0x10000, 22,
	10, 0x10000, 17,
};

s16 gData_020013c0[] = {
	12, -1,
};

s16 gData_020013c4[] = {
	13, 514, -1,
};

s16 gData_020013ca[] = {
	8, -1, 0,
};

s32 gData_020013d0[] = {
	-0x10000, 172, 0x40000095, -0x10000, -1, 0xffff,
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02001400[] = {
	0xffff0003, 512, 0xc00000e0, 0x1880000, 0x2780028, 280,
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02001430[] = {
	0xffff0005, 728, 0x40000058, 0x2a80000, 0x3980020, 192,
	0xffff0006, 872, 0x40000058, 0x2a80000, 0x3980020, 192,
	0xffff0007, 872, 120, 0x2a80000, 0x3980020, 192,
	0xffff0008, 792, 0xc0000098, 0x2a80000, 0x3980020, 192,
	0xffff, 0, 0, 0, 0, 0,
};

s16 gData_020014a8[] = {
	112, 48, 144, 128, 64, 160, -1, 1,
	336, 48, 144, 352, 64, 160, -1, 3,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
	SCENE_EXITS(47),
	SCENE_EXIT(1, 51, 8),
	SCENE_EXIT(2, 50, 7),
	SCENE_EXIT(3, 49, 1),
	SCENE_EXIT(4, 50, 13),
	SCENE_EXIT(5, 49, 6),
	SCENE_EXITS(48),
	SCENE_EXIT(1, 44, 15),
	SCENE_EXIT(2, 49, 7),
	SCENE_EXIT(3, 44, 16),
	SCENE_EXIT(4, 49, 4),
	SCENE_EXITS(49),
	SCENE_EXIT(1, 47, 3),
	SCENE_EXIT(2, 49, 3),
	SCENE_EXIT(3, 49, 2),
	SCENE_EXIT(4, 48, 4),
	SCENE_EXIT(5, 49, 8),
	SCENE_EXIT(6, 47, 5),
	SCENE_EXIT(7, 48, 2),
	SCENE_EXIT(8, 49, 5),
	SCENE_EXITS(50),
	SCENE_EXIT(1, 51, 1),
	SCENE_EXIT(2, 50, 11),
	SCENE_EXIT(3, 50, 9),
	SCENE_EXIT(4, 51, 2),
	SCENE_EXIT(5, 52, 4),
	SCENE_EXIT(6, 52, 1),
	SCENE_EXIT(7, 47, 2),
	SCENE_EXIT(8, 50, 12),
	SCENE_EXIT(9, 50, 3),
	SCENE_EXIT(10, 51, 7),
	SCENE_EXIT(11, 50, 2),
	SCENE_EXIT(12, 50, 8),
	SCENE_EXIT(13, 47, 4),
	SCENE_EXITS(51),
	SCENE_EXIT(1, 50, 1),
	SCENE_EXIT(2, 50, 4),
	SCENE_EXIT(3, 51, 5),
	SCENE_EXIT(4, 51, 6),
	SCENE_EXIT(5, 51, 3),
	SCENE_EXIT(6, 51, 4),
	SCENE_EXIT(7, 50, 10),
	SCENE_EXIT(8, 47, 1),
	SCENE_EXITS(52),
	SCENE_EXIT(1, 50, 6),
	SCENE_EXIT(2, 52, 3),
	SCENE_EXIT(3, 52, 2),
	SCENE_EXIT(4, 50, 5),
	SCENE_EXITS_END,
};

u32 gData_0200159c[] = {
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_020015b4[] = {
	0xffff00fa, 1, 0x2280000, 0, 0x1180000, 0x24000,
	0xffff0197, 1, 0x2480000, 0, 0x1a80000, 0x24000,
	0xffff0197, 1, 0x1e80000, 0, 0x1a80000, 0x1024000,
	0xffff0110, 1, 0x2780000, 0, 0xd80000, 0x24000,
	0xffff0110, 1, 0x2b80000, 0, 0x1a80000, 0x24000,
	0xffff0122, 1, 0x1880000, 0, 0x1680000, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_0200165c[] = {
	0xffff0197, 1, 0x3080000, 0, 0xe80000, 0x24000,
	0xffff0197, 1, 0x2b80000, 0, 0xe80000, 0x1024000,
	0xffff0047, 1, 0x31a0000, 0, 0xec0000, 0x18000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_020016bc[] = {
	0xffff02a7, 1, 0xca0000, 0, 0x1980000, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_020016ec[] = {
	0xffff014a, 7, 0x960000, 0, 0x16e0000, 0x24000,
	0xffff014a, 7, 0xaa0000, 0, 0x16e0000, 0x24000,
	0xffff0132, 7, 0, 0, 0, 0x24000,
	0xffff0132, 7, 0, 0, 0, 0x24000,
	0xffff0132, 7, 0, 0, 0, 0x24000,
	0xffff0132, 7, 0, 0, 0, 0x24000,
	0xffff0132, 7, 0, 0, 0, 0x24000,
	0xffff0132, 7, 0, 0, 0, 0x24000,
	0xffff0132, 7, 0, 0, 0, 0x24000,
	0xffff0132, 7, 0, 0, 0, 0x24000,
	0xffff0132, 7, 0, 0, 0, 0x24000,
	0xffff0132, 7, 0, 0, 0, 0x24000,
	0xffff019a, 7, 0, 0, 0, 0x24000,
	0xffff019a, 7, 0, 0, 0, 0x24000,
	0xffff019a, 7, 0, 0, 0, 0x24000,
	0xffff019a, 7, 0, 0, 0, 0x24000,
	0xffff019a, 7, 0, 0, 0, 0x24000,
	0xffff019a, 7, 0, 0, 0, 0x24000,
	0xffff019a, 7, 0, 0, 0, 0x24000,
	0xffff019a, 7, 0, 0, 0, 0x24000,
	0xffff019a, 7, 0, 0, 0, 0x24000,
	0xffff019a, 7, 0, 0, 0, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

s32 gData_02001914[] = {
	-1, 0, 0,
};

s32 gData_02001920[] = {
	0xc602,
	(s32)0xffff0001,
	(s32)Func_02000a88,
	0xc602,
	(s32)0xffff0002,
	(s32)Func_02000a88,
	0xc602,
	(s32)0xffff0003,
	(s32)Func_02000a88,
	0xc602,
	(s32)0xffff0004,
	(s32)Func_02000a88,
	1,
	(s32)0xffff0005,
	5,
	0x8515,
	0x2010009,
	0,
	9,
	0x8c8000a,
	(s32)Func_020004cc,
	0x8c15,
	0x8c8000b,
	(s32)Func_020004cc,
	0x10008c15,
	0x8ca000c,
	(s32)Func_0200051c,
	0x8c15,
	0x8ca000c,
	(s32)Func_0200052c,
	0x602,
	0x9ef000b,
	(s32)Func_020008f4,
	0x10008c15,
	0x9ef0008,
	(s32)Func_0200092c,
	0x8c15,
	0x9ef0008,
	(s32)Func_0200093c,
	0x1815,
	0x202000d,
	(s32)Func_02000558,
	0x50009085,
	-0x10000,
	(s32)Func_0200072c,
	0x40009085,
	-0x10000,
	(s32)Func_02000778,
	0xc403,
	(s32)0xffff000a,
	(s32)Func_020007a0,
	-1,
	0,
	0,
};

s32 gData_020019f8[] = {
	1, (s32)0xffff0002, 2, 1, (s32)0xffff0004, 4,
	-1, 0, 0,
};

s32 gData_02001a1c[] = {
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
	1,
	(s32)0xffff0008,
	8,
	0x8515,
	0x2000008,
	0,
	0,
	(s32)0xffff000a,
	MSGNO(MsgFieldCaveFoundByChampa),
	0x8d15,
	(s32)0xffff000a,
	MSGNO(MsgFieldWhoDugCave),
	-1,
	0,
	0,
};

s32 gData_02001aac[] = {
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
	1,
	(s32)0xffff0008,
	8,
	0xc602,
	(s32)0xffff0009,
	(s32)Func_02000c24,
	1,
	(s32)0xffff000a,
	10,
	1,
	(s32)0xffff000b,
	11,
	1,
	(s32)0xffff000c,
	12,
	1,
	(s32)0xffff000d,
	13,
	-1,
	0,
	0,
};

s32 gData_02001b54[] = {
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
	0xc602,
	(s32)0xffff0005,
	(s32)Func_02000c40,
	0xc602,
	(s32)0xffff0006,
	(s32)Func_02000c5c,
	1,
	(s32)0xffff0007,
	7,
	1,
	(s32)0xffff0008,
	8,
	0x8715,
	(s32)0xffff0008,
	(s32)Func_02000874,
	-1,
	0,
	0,
};

s32 gData_02001bcc[] = {
	1,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	0xc602,
	(s32)0xffff0003,
	(s32)Func_02000c78,
	1,
	(s32)0xffff0004,
	4,
	0x1cb04,
	(s32)0xffff000a,
	(s32)Func_02000568,
	3,
	0x8c9000a,
	(s32)Func_02000474,
	-1,
	0,
	0,
};
