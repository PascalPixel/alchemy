#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "objcmd.h"

void Func_020001a0(void);
void Func_0200065c(void);
void Func_02004bc4(void);
void Func_02004c2c(void);
void Func_02004c94(void);
void Func_02004d14(void);
void Func_02004da0(void);
void Func_02004df0(void);
void Func_02004e20(void);
void Func_02004e50(void);
void Func_02004ec0(void);
void Func_02004ecc(void);
void Func_02004ed8(void);
void Func_02004ee4(void);

// clang-format off
s32 gData_020057e4[] = {
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

s16 gData_02005aa4[] = {
    137, 138, 139, 140, 141, 142, 143, -1,
};

s16 gData_02005ab4[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gData_02005ae0[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u32 gData_02005b0c[] = {
    0x89008e, 0x8b008a, 0x8d008c, 0x8e008d, 0x8a0089, 0x8c008b,
    0x8d008c, 0x89008e, 0x8b008a, 0x8c008b, 0x8e008d, 0x8a0089,
    0x8b008a, 0x8d008c, 0x89008e, 0x8a0089, 0x8c008b, 0x8e008d,
};

s32 gData_02005b54[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -1536),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, 0x600),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gData_02005ba8[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(496), 0, PIXELS(328)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_02005bd0[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(480), 0, PIXELS(328)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_02005bf8[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(488), 0, PIXELS(320)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_02005c20[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(528), 0, PIXELS(328)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_02005c48[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(512), 0, PIXELS(328)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gData_02005c70[] = {
    23, 9, -1024, 23, 10, -1024,
    0, 1, 13, 64, 0, 3,
    0, 1, 17,
};

u32 gData_02005cac[] = {
    SC_ADD(SW_SCALE_X, 512),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_Y, 0x2000),
    SC_WAIT(1),
    SC_LOOP(128, SC_TOP),
    SC_END,
};

u32 gData_02005ce8[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(500), 0, PIXELS(306)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(10),
    SC_END,
};

u32 gData_02005d2c[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(524), 0, PIXELS(306)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

u32 gData_02005d70[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(488), 0, PIXELS(320)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(10),
    SC_END,
};

u32 gData_02005db4[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(472), 0, PIXELS(304)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(10),
    SC_END,
};

u32 gData_02005df8[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(464), 0, PIXELS(280)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

u32 gData_02005e3c[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(538), 0, PIXELS(320)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

u32 gData_02005e80[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(552), 0, PIXELS(304)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

u32 gData_02005ec4[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(560), 0, PIXELS(280)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

u32 gData_02005f08[] = {
    39, 2, 4, 0x1d80000, 0, 0x1400000,
    1, 4, 0x1d80000, 0, 0x1300000, 1,
    39, 1, 22, 39, 1, 17,
};

u32 gData_02005f50[] = {
    39, 2, 4, 0x1dc0000, 0, 0x1480000,
    1, 4, 0x1d80000, 0, 0x1400000, 1,
    39, 1, 22, 39, 1, 17,
};

u32 gData_02005f98[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(486), 0, PIXELS(286)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

u32 gData_02005fdc[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(504), 0, PIXELS(300)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_02006004[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(528), 0, PIXELS(292)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_0200602c[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(560), 0, PIXELS(280)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

u32 gData_02006070[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(548), 0, PIXELS(292)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

s32 gData_020060b4[] = {
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_WAIT(1),
    SC_LOOP(32, SC_TOP),
    SC_END,
    SC_ADD(SW_SCALE_X, -896),
    SC_WAIT(1),
    SC_LOOP(140, SC_TOP),
    SC_END,
};

s32 gData_020060fc[] = {
    SC_ADD(SW_SCALE_X, -896),
    SC_WAIT(1),
    SC_LOOP(160, SC_TOP),
    SC_END,
};

u32 gData_02006120[] = {
    23, 6, 0x40000, 0, 1, 13,
    30, 0, 3, 0, 1, 17,
};

u32 gData_02006150[] = {
    23, 6, 0x20000, 0, 1, 13,
    160, 0, 3, 0, 1, 17,
};

s32 gData_02006180[] = {
    22, 9, 0xcccc, 22, 10, 0xcccc,
    39, 1, (s32)0x80010000, 23, 6, -0x10000,
    0, 1, 13, 16, (s32)0xc0010000, 3,
    0, 1, 38,
};

s32 gData_020061d4[] = {
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_WAIT(1),
    SC_LOOP(8, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -4096),
    SC_WAIT(1),
    SC_LOOP(8, SC_TO(1)),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(297),
    SCENE_EXIT(1, 287, 1),
    SCENE_EXIT(2, 297, 3),
    SCENE_EXIT(3, 309, 2),
    SCENE_EXIT(4, 272, 20),
    SCENE_EXITS_END,
};

s16 gData_0200626c[] = {
    489, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 489, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    302, -1, 7, 0, 0, 584, 0, 16,
    0, 440, 0x4000, 2, 302, -1, 7, 0,
    0, 616, 0, 16, 0, 440, 0x4000, 258,
    302, -1, 7, 0, 0, 648, 0, 16,
    0, 440, 0x4000, 258, 302, -1, 7, 0,
    0, 648, 0, 16, 0, 408, 0x4000, 258,
    302, -1, 7, 0, 0, 648, 0, 16,
    0, 376, 0x4000, 258, 302, -1, 7, 0,
    0, 648, 0, 16, 0, 344, 0x4000, 258,
    302, -1, 7, 0, 0, 648, 0, 16,
    0, 312, 0x4000, 258, 182, -1, 7, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    311, -1, 7, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 56, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    5, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 6, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    7, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 0, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    1, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 3, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    2, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

struct ActorPlacement gData_0200644c[] = {
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 1},
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 1},
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 1},
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 1},
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 1},
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 1},
    {182, CONDITION_ALWAYS, 7, PIXELS(512), 0, PIXELS(236), FACING_SOUTH, 2, 0},
    {311, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(544), 0, PIXELS(354), FACING_NORTHWEST + FACING_STEP, 2, 0},
    {48, CONDITION_ALWAYS, 1, PIXELS(496), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {49, CONDITION_ALWAYS, 1, PIXELS(480), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {50, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(320), FACING_NORTH, 2, 0},
    {51, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(320), FACING_NORTH, 2, 0},
    {52, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(320), FACING_NORTH, 2, 0},
    {54, CONDITION_ALWAYS, 1, PIXELS(544), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {53, CONDITION_ALWAYS, 1, PIXELS(528), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {183, CONDITION_ALWAYS, 7, PIXELS(478), 0, PIXELS(266), FACING_SOUTH, 2, 0},
    {183, CONDITION_ALWAYS, 7, PIXELS(506), 0, PIXELS(280), FACING_SOUTH, 2, 0},
    {183, CONDITION_ALWAYS, 7, PIXELS(536), 0, PIXELS(276), FACING_SOUTH, 2, 0},
    {61, CONDITION_ALWAYS, 7, PIXELS(480), 0, PIXELS(282), FACING_SOUTH, 2, 0},
    {60, CONDITION_ALWAYS, 7, PIXELS(506), 0, PIXELS(292), FACING_SOUTH, 2, 0},
    {62, CONDITION_ALWAYS, 7, PIXELS(540), 0, PIXELS(284), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

s16 gData_020066bc[] = {
    303, -1, 7, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 303, -1, 7, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 258,
    302, -1, 7, 0, 0, 584, 0, 16,
    0, 440, 0x4000, 2, 302, -1, 7, 0,
    0, 616, 0, 16, 0, 440, 0x4000, 258,
    302, -1, 7, 0, 0, 648, 0, 16,
    0, 440, 0x4000, 258, 302, -1, 7, 0,
    0, 648, 0, 16, 0, 408, 0x4000, 258,
    302, -1, 7, 0, 0, 648, 0, 16,
    0, 376, 0x4000, 258, 302, -1, 7, 0,
    0, 648, 0, 16, 0, 344, 0x4000, 258,
    302, -1, 7, 0, 0, 648, 0, 16,
    0, 312, 0x4000, 258, 489, -1, 7, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    489, -1, 7, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 56, -1, 1, 0,
    0, 504, 0, 0, 0, 328, 0xb000, 2,
    5, -1, 1, 0, 0, 458, 0, 0,
    0, 288, 0, 1, 6, -1, 1, 0,
    0, 476, 0, 0, 0, 312, 0xe000, 1,
    7, -1, 1, 0, 0, 482, 0, 0,
    0, 276, 0x4000, 1, 0, -1, 1, 0,
    0, 529, 0, 0, 0, 296, 0, 1,
    1, -1, 1, 0, 0, 538, 0, 0,
    0, 312, 0xe000, 1, 3, -1, 1, 0,
    0, 552, 0, 0, 0, 274, 0x4000, 1,
    2, -1, 1, 0, 0, 566, 0, 0,
    0, 286, 0x8000, 1, 61, -1, 7, 0,
    0, 480, 0, 0, 0, 304, 0x4000, 2,
    60, -1, 7, 0, 0, 502, 0, 0,
    0, 312, 0x4000, 2, 62, -1, 7, 0,
    0, 548, 0, 0, 0, 304, 0x4000, 2,
    294, -1, 7, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 294, -1, 7, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 258,
    294, -1, 7, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 258, 294, -1, 7, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 258,
    294, -1, 7, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 258, 59, -1, 7, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    489, -1, 7, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 489, -1, 7, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

struct Trigger gSceneEvents[] = {
    {0x9c05, 1, CONDITION_ALWAYS, EVENT(Func_020001a0)},
    {2, 10, 0xa3d, EVENT(Func_0200065c)},
    {0, 0, CONDITION_ALWAYS, MSGNO(MsgOurQuestIs)},
    {0, 1, CONDITION_ALWAYS, MSGNO(MsgIWishICould)},
    {0, 3, CONDITION_ALWAYS, MSGNO(MsgIfAlexSet)},
    {0, 5, CONDITION_ALWAYS, MSGNO(MsgIThoughtID)},
    {0, 6, CONDITION_ALWAYS, MSGNO(MsgIDonTKnowMy)},
    {0, 7, CONDITION_ALWAYS, MSGNO(MsgIMSorryThings)},
    {0, 19, CONDITION_ALWAYS, MSGNO(MsgBothKyleAnd)},
    {0x4400, 28, CONDITION_ALWAYS, EVENT(Func_02004da0)},
    {0x4400, 17, CONDITION_ALWAYS, EVENT(Func_02004df0)},
    {0x6400, 17, CONDITION_ALWAYS, EVENT(Func_02004df0)},
    {0xe400, 30, CONDITION_ALWAYS, EVENT(Func_02004e20)},
    {0x400, 30, CONDITION_ALWAYS, EVENT(Func_02004e20)},
    {0x2400, 30, CONDITION_ALWAYS, EVENT(Func_02004e20)},
    {0, 29, CONDITION_ALWAYS, EVENT(Func_02004e50)},
#if (LANGUAGE != LANGUAGE_JAPANESE) /* Absent from the Japanese edition. */
    {0x9115, 28, CONDITION_ALWAYS, EVENT(Func_02004ec0)},
    {0x9115, 17, CONDITION_ALWAYS, EVENT(Func_02004ecc)},
    {0x9115, 30, CONDITION_ALWAYS, EVENT(Func_02004ed8)},
    {0x9115, 29, CONDITION_ALWAYS, EVENT(Func_02004ee4)},
#endif
    {2, 20, CONDITION_ALWAYS, EVENT(Func_02004bc4)},
    {2, 21, CONDITION_ALWAYS, EVENT(Func_02004c2c)},
    {2, 22, 0x202, EVENT(Func_02004c94)},
    {2, 23, 0x1202, EVENT(Func_02004d14)},
    {TRIGGERS_END},
};

u32 gData_02006ad0[] = {
    SC_SET(SW_SCALE_Y, 0x16666),
    SC_SET(SW_SCALE_X, 0x16666),
    SC_ARG(4),
    SC_WAIT(30),
    SC_FREE,
};

u32 gData_02006afc[] = {
    SC_SET(SW_SCALE_X, 0xcccc),
    SC_SET(SW_SCALE_Y, 0xcccc),
    SC_ARG(1),
    SC_LABEL(1),
    SC_ADD(SW_Y, PIXELS(1)),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};
// clang-format on
