#include "types.h"
#include "scene_tables.h"
#include "objcmd.h"

void Func_02000280(void);
void Func_0200028c(void);
void Func_02000364(void);
void Func_02000408(void);
void Func_0200047c(void);
void Func_02000550(void);
void Func_02000740(void);
void Func_02000784(void);
void Func_02000978(void);
void Func_02000ad4(void);
void Func_02000c38(void);
void Func_02000c8c(void);
void Func_02000cf0(void);
void Func_02000fc0(void);
void Func_02001284(void);
void Func_020012ec(void);
void Func_02001364(void);
void Func_020013bc(void);
void Func_02001430(void);
void Func_02001498(void);
void Func_02001500(void);
void Func_02001564(void);
void Func_020015b8(void);
void Func_02001630(void);
void Func_02001728(void);
void Func_0200176c(void);
void Func_020017dc(void);
void Func_02001820(void);
void Func_02001864(void);
void Func_020018a8(void);
void Func_02001990(void);
void Func_020019d0(void);
void Func_02001a6c(void);
void Func_02001a8c(void);
void Func_020043ec(void);
void Func_02004530(void);
void Func_020045d8(void);
void Func_02004620(void);
void Func_020046c8(void);

extern u8 gData_0200585c[];
extern u8 gData_02005898[];
extern u8 gData_020058d4[];

extern s32 gData_02005fbc[];
extern s32 gData_02005fc8[];

// clang-format off
u8 gData_02005aac[] = {
    0, 2, 2, 1, 1, 3, 3, 7, 5, 4,
    6, 5, 7, 6, 4, 9, 9, 8, 8, 10,
};

u32 gData_02005ac0[] = {
    266, 0xff01,
};

u32 gData_02005ac8[] = {
    0x50a00ff, 0xc06030f, 0xf0f0f09,
};

u32 gData_02005ad4[] = {
    0x60a0502, 0xa060107, 0x702040a, 0x1050a04,
};

s32 gData_02005ae4[] = {
    (s32)gData_0200585c,
    (s32)gData_02005898,
    (s32)gData_020058d4,
};

u32 gData_02005af0[] = {
    0,
};

u32 gData_02005af4[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(472), 0, PIXELS(184)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_02005b1c[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(488), 0, PIXELS(176)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02005b60[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(456), 0, PIXELS(176)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02005ba4[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(464), 0, PIXELS(208)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH + FACING_STEP),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02005be8[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(432), 0, PIXELS(286)),
    SC_SYNC,
    SC_MOVE(PIXELS(432), 0, PIXELS(312)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02005c40[] = {
    39, 2, 4, 0x15c0000, 0, 0x1380000,
    1, 39, 1, 3, 0, 1,
    17,
};

s32 gData_02005c74[] = {
    22,
    35,
    0xc000,
    22,
    36,
    0xe000,
    22,
    40,
    0,
    46,
    (s32)Func_0200028c,
};

s32 gData_02005ca0[] = {
    22,
    35,
    0xc000,
    22,
    36,
    0xa000,
    22,
    40,
    0,
    46,
    (s32)Func_0200028c,
};

u32 gData_02005ccc[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(472), 0, PIXELS(308)),
    SC_SYNC,
    SC_MOVE(PIXELS(440), 0, PIXELS(312)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02005d24[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(472), 0, PIXELS(308)),
    SC_SYNC,
    SC_MOVE(PIXELS(460), 0, PIXELS(312)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH + FACING_STEP),
    SC_WAIT(1),
    SC_END,
};

s16 gData_02005d7c[] = {
    12, 2, 0x8ce, 13, 2, 0x8cf, -1, 0,
};

u32 gData_02005d8c[] = {
    36, 122, 201, 312, 431, 504,
    619, 712, 823, 908, 0x41e, 0x461,
    0x4b7, 0x51f, 0x579, 0x613,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 392, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

s16 gData_02005dfc[] = {
    784, 16, 80, 800, 32, 96, -1, 1,
    80, 48, 368, 96, 64, 384, -1, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gData_02005e2c[] = {
    0x1001d0, 0x1e000f0, 0x1000020, 0x2ffff, 0xffff, 0,
    0, 0,
};

s16 gData_02005e4c[] = {
    528, 16, 64, 544, 32, 80, -1, 2,
    96, 16, 160, 112, 32, 176, -1, 3,
    672, 16, 160, 688, 32, 176, -1, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gData_02005e8c[] = {
    384, 16, 112, 400, 32, 128, -1, 4,
    96, 16, 144, 112, 32, 160, -1, 5,
    672, 16, 144, 688, 32, 160, -1, 6,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gData_02005ecc[] = {
    0x1001b0, 0x1c001c0, 0x1d00020, 0x4ffff, 0xffff, 0,
    0, 0,
};

u32 gData_02005eec[] = {
    0x100130, 0x1400090, 0xa00020, 0x2ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(123),
    SCENE_EXIT_IF(1, 111, 3, 0x86b),
    SCENE_EXIT_IF(1, 114, 3, -1),
    SCENE_EXIT_IF(2, 115, 1, -1),
    SCENE_EXITS(115),
    SCENE_EXIT(1, 123, 2),
    SCENE_EXIT(2, 116, 1),
    SCENE_EXITS(116),
    SCENE_EXIT(1, 115, 2),
    SCENE_EXIT(2, 117, 1),
    SCENE_EXIT(3, 117, 2),
    SCENE_EXIT(4, 117, 3),
    SCENE_EXITS(117),
    SCENE_EXIT(1, 116, 2),
    SCENE_EXIT(2, 116, 3),
    SCENE_EXIT(3, 116, 4),
    SCENE_EXIT(4, 118, 1),
    SCENE_EXIT(5, 118, 2),
    SCENE_EXIT(6, 118, 3),
    SCENE_EXITS(118),
    SCENE_EXIT(1, 117, 4),
    SCENE_EXIT(2, 117, 5),
    SCENE_EXIT(3, 117, 6),
    SCENE_EXIT(4, 119, 1),
    SCENE_EXIT(5, 117, 7),
    SCENE_EXIT(6, 117, 8),
    SCENE_EXITS(119),
    SCENE_EXIT(1, 118, 4),
    SCENE_EXIT(2, 120, 1),
    SCENE_EXITS(121),
    SCENE_EXIT(1, 120, 2),
    SCENE_EXIT(10, 125, 10),
    SCENE_EXIT(11, 125, 11),
    SCENE_EXIT(12, 114, 11),
    SCENE_EXIT(13, 114, 12),
    SCENE_EXIT(14, 114, 13),
    SCENE_EXIT(15, 114, 14),
    SCENE_EXIT(16, 114, 15),
    SCENE_EXIT(17, 125, 10),
    SCENE_EXIT(18, 120, 6),
    SCENE_EXITS_END,
};

s32 gData_02005fbc[] = {
    SC_CALL(Func_02000364),
    SC_END,
};

s32 gData_02005fc8[] = {
    SC_CALL(Func_02000408),
    SC_END,
};

struct ActorPlacement gData_02005fd4[] = {
    {337, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(248), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02006004[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(88), FACING_EAST, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_02005fc8, PIXELS(328), 0, PIXELS(88), FACING_EAST, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(152), FACING_EAST, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(152), FACING_EAST, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(488), FACING_EAST, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(488), FACING_EAST, 2, 0},
    {358, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(72), FACING_EAST, 2, 0},
    {358, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(72), FACING_WEST, 2, 1},
    {358, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(168), FACING_EAST, 2, 1},
    {358, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(248), FACING_EAST, 2, 1},
    {358, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(248), FACING_EAST, 2, 1},
    {358, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(520), FACING_EAST, 2, 1},
    {358, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(552), FACING_EAST, 2, 1},
    {357, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(88), FACING_WEST, 2, 0},
    {357, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(200), FACING_WEST, 2, 1},
    {357, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(216), FACING_WEST, 2, 1},
    {357, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(552), FACING_WEST, 2, 1},
    {357, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(488), FACING_WEST, 2, 1},
    {357, CONDITION_ALWAYS, (s32)gData_02005fbc, PIXELS(360), 0, PIXELS(488), FACING_EAST, 2, 0},
    {362, CONDITION_ALWAYS, 1, PIXELS(352), 0, PIXELS(168), FACING_WEST, 2, 0},
    {362, CONDITION_ALWAYS, 1, PIXELS(400), 0, PIXELS(168), FACING_EAST, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {337, CONDITION_ALWAYS, 1, PIXELS(680), 0, PIXELS(168), FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

s16 gData_02006244[] = {
    358, -1, 1, 0, 0, 56, 0, 0,
    0, 168, 0, 2, 358, -1, 1, 0,
    0, 152, 0, 0, 0, 152, 0x8000, 258,
    358, -1, 1, 0, 0, 136, 0, 0,
    0, 200, 0, 258, 358, -1, 1, 0,
    0, 344, 0, 0, 0, 120, 0, 258,
    358, -1, 1, 0, 0, 488, 0, 0,
    0, 216, 0, 258, 358, -1, 1, 0,
    0, 632, 0, 0, 0, 152, 0, 258,
    358, -1, 1, 0, 0, 728, 0, 0,
    0, 264, 0x8000, 258, 357, -1, 1, 0,
    0, 56, 0, 0, 0, 232, 0, 2,
    357, -1, 1, 0, 0, 248, 0, 0,
    0, 168, 0, 258, 357, -1, 1, 0,
    0, 264, 0, 0, 0, 248, 0x8000, 258,
    357, -1, 1, 0, 0, 488, 0, 0,
    0, 168, 0, 258, 357, -1, 1, 0,
    0, 712, 0, 0, 0, 200, 0, 258,
    359, -1, 1, 0, 0, 88, 0, 0,
    0, 184, 0, 2, 359, -1, 1, 0,
    0, 104, 0, 0, 0, 264, 0x8000, 258,
    359, -1, 1, 0, 0, 408, 0, 0,
    0, 296, 0, 258, 359, -1, 1, 0,
    0, 664, 0, 0, 0, 168, 0, 258,
    359, -1, 1, 0, 0, 664, 0, 0,
    0, 248, 0, 258, 360, -1, 1, 0,
    0, 152, 0, 0, 0, 232, 0, 2,
    360, -1, 1, 0, 0, 296, 0, 0,
    0, 216, 0, 258, 360, -1, 1, 0,
    0, 360, 0, 0, 0, 248, 0, 258,
    360, -1, 1, 0, 0, 632, 0, 0,
    0, 216, 0, 258, 361, -1, 1, 0,
    0, 424, 0, 0, 0, 136, 0, 2,
    361, -1, 1, 0, 0, 472, 0, 0,
    0, 248, 0, 258, 407, -1, 1, 0,
    0, 424, 0, 0, 0, 200, 0x4000, 2,
    407, -1, 1, 0, 0, 376, 0, 0,
    0, 200, 0x4000, 258, 243, 57, 1, 0,
    0, 504, 0, 0, 0, 216, 0x4000, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

s16 gData_020064cc[] = {
    358, -1, 1, 0, 0, 56, 0, 0,
    0, 152, 0, 2, 358, -1, 1, 0,
    0, 40, 0, 0, 0, 184, 0x8000, 258,
    358, -1, 1, 0, 0, 136, 0, 0,
    0, 168, 0x8000, 258, 358, -1, 1, 0,
    0, 312, 0, 0, 0, 120, 0x8000, 258,
    358, -1, 1, 0, 0, 360, 0, 0,
    0, 152, 0, 258, 358, -1, 1, 0,
    0, 360, 0, 0, 0, 184, 0x8000, 258,
    358, -1, 1, 0, 0, 440, 0, 0,
    0, 184, 0, 258, 358, -1, 1, 0,
    0, 488, 0, 0, 0, 136, 0, 258,
    358, -1, 1, 0, 0, 520, 0, 0,
    0, 136, 0x8000, 258, 358, -1, 1, 0,
    0, 344, 0, 0, 0, 312, 0, 258,
    358, -1, 1, 0, 0, 328, 0, 0,
    0, 408, 0, 258, 358, -1, 1, 0,
    0, 680, 0, 0, 0, 232, 0x8000, 258,
    358, -1, 1, 0, 0, 728, 0, 0,
    0, 248, 0x8000, 258, 357, -1, 1, 0,
    0, 72, 0, 0, 0, 232, 0, 2,
    357, -1, 1, 0, 0, 312, 0, 0,
    0, 152, 0, 258, 357, -1, 1, 0,
    0, 392, 0, 0, 0, 184, 0, 258,
    357, -1, 1, 0, 0, 440, 0, 0,
    0, 136, 0x8000, 258, 357, -1, 1, 0,
    0, 328, 0, 0, 0, 376, 0x8000, 258,
    357, -1, 1, 0, 0, 392, 0, 0,
    0, 424, 0, 258, 357, -1, 1, 0,
    0, 648, 0, 0, 0, 168, 0x8000, 258,
    357, -1, 1, 0, 0, 680, 0, 0,
    0, 264, 0, 258, 359, -1, 1, 0,
    0, 200, 0, 0, 0, 232, 0, 2,
    359, -1, 1, 0, 0, 312, 0, 0,
    0, 200, 0x8000, 258, 359, -1, 1, 0,
    0, 520, 0, 0, 0, 168, 0, 258,
    359, -1, 1, 0, 0, 376, 0, 0,
    0, 328, 0, 258, 359, -1, 1, 0,
    0, 712, 0, 0, 0, 200, 0x8000, 258,
    360, -1, 1, 0, 0, 120, 0, 0,
    0, 248, 0, 2, 360, -1, 1, 0,
    0, 152, 0, 0, 0, 200, 0, 258,
    360, -1, 1, 0, 0, 568, 0, 0,
    0, 216, 0, 258, 361, -1, 1, 0,
    0, 456, 0, 0, 0, 216, 0, 2,
    361, -1, 1, 0, 0, 488, 0, 0,
    0, 216, 0, 258, 132, 0xa6c, 1, 0,
    0, 0, 0, 0, 0, 0, 0xc000, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

s16 gData_020067e4[] = {
    407, -1, 1, 0, 0, 312, 0, 0,
    0, 232, 0, 2, 407, -1, 1, 0,
    0, 360, 0, 0, 0, 232, 0, 2,
    359, -1, 1, 0, 0, 392, 0, 0,
    0, 152, 0, 2, 359, -1, 1, 0,
    0, 360, 0, 0, 0, 296, 0, 258,
    362, -1, 1, 0, 0, 416, 0, 0,
    0, 216, 0x8000, 2, 362, -1, 1, 0,
    0, 464, 0, 0, 0, 216, 0, 258,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

s32 gData_0200e88c[] = {
    SC_CALL(Func_0200047c),
    SC_END,
};

struct ActorPlacement gData_02006898[] = {
    {376, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(232), FACING_SOUTH, 2, 0},
    {374, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(296), FACING_SOUTH, 2, 0},
    {374, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(296), FACING_SOUTH, 2, 0},
    {349, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {349, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {349, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {349, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {411, CONDITION_ALWAYS, 1, 0, PIXELS(8), 0, FACING_SOUTH, 2, 0},
    {411, CONDITION_ALWAYS, 1, 0, PIXELS(8), 0, FACING_SOUTH, 2, 1},
    {411, CONDITION_ALWAYS, 1, 0, PIXELS(8), 0, FACING_SOUTH, 2, 1},
    {411, CONDITION_ALWAYS, 1, 0, PIXELS(8), 0, FACING_SOUTH, 2, 1},
    {411, CONDITION_ALWAYS, 1, 0, PIXELS(8), 0, FACING_SOUTH, 2, 1},
    {411, CONDITION_ALWAYS, 1, 0, PIXELS(8), 0, FACING_SOUTH, 2, 1},
    {411, CONDITION_ALWAYS, 1, 0, PIXELS(8), 0, FACING_SOUTH, 2, 1},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {26, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(430), FACING_SOUTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

s16 gData_02006a90[] = {
    362, -1, 1, 0, 0, 616, 0, 0,
    0, 288, 0, 2, 362, -1, 1, 0,
    0, 616, 0, 0, 0, 336, 0, 258,
    362, -1, 1, 0, 0, 392, 0, 0,
    0, 392, 0, 258, 362, -1, 1, 0,
    0, 128, 0, 0, 0, 456, 0, 258,
    362, -1, 1, 0, 0, 176, 0, 0,
    0, 456, 0x8000, 258, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

struct ActorPlacement gData_02006b20[] = {
    {SCENE_TABLE_END},
};

s32 gData_02006b38[] = {
    0x10101, -0x10000, 255, 257, 0xffff, 0xff0001,
    0,
};

struct Trigger gData_02006b54[] = {
    {0x202, 1, CONDITION_ALWAYS, EVENT(Func_02001284)},
    {TRIGGERS_END},
};

struct Trigger gData_02006b6c[] = {
    {0x202, 1, CONDITION_ALWAYS, EVENT(Func_020012ec)},
    {2, 10, CONDITION_ALWAYS, EVENT(Func_020017dc)},
    {2, 11, CONDITION_ALWAYS, EVENT(Func_02001820)},
    {0x8515, 8, 0x201, EVENT(Func_02000550)},
    {0x8515, 10, 0x202, 0},
    {0x50008615, 12, 0x8ce, EVENT(Func_02001990)},
    {0x50008615, 13, 0x8cf, EVENT(Func_02001990)},
    {6, 200, 0x8cd, EVENT(Func_020019d0)},
    {TRIGGERS_END},
};

struct Trigger gData_02006bd8[] = {
    {0x202, 1, CONDITION_ALWAYS, EVENT(Func_02001364)},
    {0x202, 2, CONDITION_ALWAYS, EVENT(Func_020013bc)},
    {0x202, 3, CONDITION_ALWAYS, EVENT(Func_02001430)},
    {2, 80, CONDITION_ALWAYS, EVENT(Func_02000740)},
    {2, 81, CONDITION_ALWAYS, EVENT(Func_02000784)},
    {2, 82, CONDITION_ALWAYS, EVENT(Func_02000978)},
    {2, 83, CONDITION_ALWAYS, EVENT(Func_02000ad4)},
    {0x602, 84, CONDITION_ALWAYS, EVENT(Func_02000c38)},
    {0x8602, 84, CONDITION_ALWAYS, EVENT(Func_02000c38)},
    {0xc602, 84, CONDITION_ALWAYS, EVENT(Func_02000c8c)},
    {0x4602, 84, CONDITION_ALWAYS, EVENT(Func_02000c8c)},
    {0x8515, 31, 0x201, 0},
    {0, 33, 0x1204, EVENT(Func_02000280)},
    {TRIGGERS_END},
};

struct Trigger gData_02006c80[] = {
    {0x202, 1, CONDITION_ALWAYS, EVENT(Func_02001498)},
    {0x202, 2, CONDITION_ALWAYS, EVENT(Func_02001500)},
    {0x202, 3, CONDITION_ALWAYS, EVENT(Func_02001564)},
    {2, 80, CONDITION_ALWAYS, EVENT(Func_02000740)},
    {2, 81, CONDITION_ALWAYS, EVENT(Func_02000784)},
    {2, 82, CONDITION_ALWAYS, EVENT(Func_02000978)},
    {2, 83, CONDITION_ALWAYS, EVENT(Func_02000ad4)},
    {0x602, 84, CONDITION_ALWAYS, EVENT(Func_02000c38)},
    {0x8602, 84, CONDITION_ALWAYS, EVENT(Func_02000c38)},
    {0xc602, 84, CONDITION_ALWAYS, EVENT(Func_02000c8c)},
    {0x4602, 84, CONDITION_ALWAYS, EVENT(Func_02000c8c)},
    {2, 77, 0xa6c, EVENT(Func_02000cf0)},
    {TRIGGERS_END},
};

struct Trigger gData_02006d1c[] = {
    {0x202, 1, CONDITION_ALWAYS, EVENT(Func_020015b8)},
    {2, 80, CONDITION_ALWAYS, EVENT(Func_02000740)},
    {2, 81, CONDITION_ALWAYS, EVENT(Func_02000784)},
    {2, 82, CONDITION_ALWAYS, EVENT(Func_02000978)},
    {2, 83, CONDITION_ALWAYS, EVENT(Func_02000ad4)},
    {0x602, 84, CONDITION_ALWAYS, EVENT(Func_02000c38)},
    {0x8602, 84, CONDITION_ALWAYS, EVENT(Func_02000c38)},
    {0xc602, 84, CONDITION_ALWAYS, EVENT(Func_02000c8c)},
    {0x4602, 84, CONDITION_ALWAYS, EVENT(Func_02000c8c)},
    {2, 10, CONDITION_ALWAYS, EVENT(Func_02001864)},
    {2, 11, CONDITION_ALWAYS, EVENT(Func_020018a8)},
    {0x8515, 8, CONDITION_ALWAYS, 0},
    {TRIGGERS_END},
};

struct Trigger gData_02006db8[] = {
    {0x202, 1, CONDITION_ALWAYS, EVENT(Func_02001630)},
    {2, 80, CONDITION_ALWAYS, EVENT(Func_020045d8)},
    {2, 81, CONDITION_ALWAYS, EVENT(Func_02004530)},
    {2, 82, CONDITION_ALWAYS, EVENT(Func_020046c8)},
    {2, 83, CONDITION_ALWAYS, EVENT(Func_02004620)},
    {6, 201, 0x205, EVENT(Func_02000fc0)},
    {0x50008615, 15, 0x8fb, EVENT(Func_02001a6c)},
    {6, 200, CONDITION_ALWAYS, EVENT(Func_02001a8c)},
    {0x50008615, 16, CONDITION_ALWAYS, EVENT(Func_020043ec)},
    {0x50008615, 17, CONDITION_ALWAYS, EVENT(Func_020043ec)},
    {0x50008615, 18, CONDITION_ALWAYS, EVENT(Func_020043ec)},
    {0x50008615, 19, CONDITION_ALWAYS, EVENT(Func_020043ec)},
    {0x50008615, 20, CONDITION_ALWAYS, EVENT(Func_020043ec)},
    {0x50008615, 21, CONDITION_ALWAYS, EVENT(Func_020043ec)},
    {0x50008615, 22, CONDITION_ALWAYS, EVENT(Func_020043ec)},
    {TRIGGERS_END},
};

struct Trigger gData_02006e78[] = {
    {2, 10, CONDITION_ALWAYS, EVENT(Func_02001728)},
    {2, 11, CONDITION_ALWAYS, EVENT(Func_0200176c)},
    {TRIGGERS_END},
};

struct Trigger gData_02006e9c[] = {
    {TRIGGERS_END},
};

u32 gData_02006ea8[] = {
    0x2020000, 0x3030101, 0x6040507, 0x4060705, 0x8080909, 0xb0b0a0a,
};

u32 gData_02006ec0[] = {
    SC_ARG(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 0xa00),
    SC_ADD(SW_SCALE_Y, 0xa00),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_SET(SW_BYTE_63, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gData_02006f38[] = {
    SC_ARG(0),
    SC_SET(SW_SCALE_X, 0x20000),
    SC_SET(SW_SCALE_Y, 0x20000),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -2560),
    SC_ADD(SW_SCALE_Y, -2560),
    SC_WAIT(1),
    SC_LOOP(48, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_SET(SW_BYTE_63, 0),
    SC_WAIT(1),
    SC_END,
};
// clang-format on
