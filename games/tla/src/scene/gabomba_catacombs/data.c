#include "types.h"
#include "scene_tables.h"
#include "objcmd.h"

void Func_02000038(void);
void Func_02000044(void);
void Func_02000060(void);
void Func_02000080(void);
void Func_020000a0(void);
void Func_020000c0(void);
void Func_020000e0(void);
void Func_02000100(void);
void Func_0200018c(void);
void Func_020001b0(void);
void Func_020001d4(void);
void Func_020001e0(void);
void Func_02000260(void);
void Func_020002e0(void);
void Func_02000364(void);
void Func_020003e4(void);
void Func_02000464(void);
void Func_020004e4(void);
void Func_02000564(void);
void Func_020005e8(void);
void Func_02000608(void);
void Func_0200066c(void);
void Func_0200069c(void);
void Func_020006cc(void);
void Func_02000704(void);
void Func_02000740(void);
void Func_0200077c(void);
void Func_020007b8(void);
void Func_020007f0(void);
void Func_0200082c(void);
void Func_02000868(void);
void Func_0200094c(void);
void Func_020009d4(void);
void Func_02000d90(void);

// clang-format off
u32 gData_0200128c[] = {
    0, 15, 3, 0, 1, 38,
};

u32 gData_020012a4[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_02001310[] = {
    0x2e0184, 0x18c0114, 0x11c0036, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(126),
    SCENE_EXIT(1, 124, 2),
    SCENE_EXIT(2, 127, 1),
    SCENE_EXIT(3, 127, 3),
    SCENE_EXIT(4, 127, 4),
    SCENE_EXIT(5, 127, 5),
    SCENE_EXITS(127),
    SCENE_EXIT(1, 126, 2),
    SCENE_EXIT(2, 128, 1),
    SCENE_EXITS(128),
    SCENE_EXIT(1, 127, 2),
    SCENE_EXIT(2, 130, 1),
    SCENE_EXITS(129),
    SCENE_EXIT(1, 130, 2),
    SCENE_EXITS(130),
    SCENE_EXIT(1, 128, 2),
    SCENE_EXIT(2, 129, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_02001378[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02001390[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020013a8[] = {
    {243, 0x3a, 1, PIXELS(728), 0, PIXELS(312), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020013d8[] = {
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02001498[] = {
    {375, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {386, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {386, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02001510[] = {
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gData_02001540[] = {
    {TRIGGERS_END},
};

struct Trigger gData_0200154c[] = {
    {0x31, 2, CONDITION_ALWAYS, 2},
    {0x50008905, 41, CONDITION_ALWAYS, EVENT(Func_02000044)},
    {0x50008905, 43, CONDITION_ALWAYS, EVENT(Func_0200018c)},
    {0x50008905, 44, CONDITION_ALWAYS, EVENT(Func_020001b0)},
    {0x50008905, 45, CONDITION_ALWAYS, EVENT(Func_020001d4)},
    {TRIGGERS_END},
};

struct Trigger gData_02001594[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {0xc400, 8, CONDITION_ALWAYS, EVENT(Func_02000038)},
    {TRIGGERS_END},
};

struct Trigger gData_020015c4[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {0x50009085, 0, CONDITION_ALWAYS, EVENT(Func_0200066c)},
    {0x40009085, 0, CONDITION_ALWAYS, EVENT(Func_0200069c)},
    {0x50008905, 40, CONDITION_ALWAYS, EVENT(Func_02000060)},
    {0x50008905, 41, CONDITION_ALWAYS, EVENT(Func_02000080)},
    {0x50008905, 42, CONDITION_ALWAYS, EVENT(Func_020001e0)},
    {0x50008905, 43, CONDITION_ALWAYS, EVENT(Func_02000260)},
    {0x50008905, 44, CONDITION_ALWAYS, EVENT(Func_020002e0)},
    {0x50008905, 45, CONDITION_ALWAYS, EVENT(Func_02000364)},
    {0x50008905, 46, CONDITION_ALWAYS, EVENT(Func_020003e4)},
    {0x50008905, 47, CONDITION_ALWAYS, EVENT(Func_02000464)},
    {0x50008905, 48, CONDITION_ALWAYS, EVENT(Func_020004e4)},
    {0x50008905, 49, CONDITION_ALWAYS, EVENT(Func_02000100)},
    {0x1815, 8, 0x200, EVENT(Func_020006cc)},
    {0x1815, 9, 0x201, EVENT(Func_02000704)},
    {0x1815, 10, 0x202, EVENT(Func_02000740)},
    {0x1815, 11, 0x203, EVENT(Func_0200077c)},
    {0x1815, 12, 0x204, EVENT(Func_020007b8)},
    {0x1815, 13, 0x205, EVENT(Func_020007f0)},
    {0x1815, 14, 0x206, EVENT(Func_0200082c)},
    {TRIGGERS_END},
};

struct Trigger gData_020016cc[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {2, 40, 0x9ce, EVENT(Func_020009d4)},
    {0, 8, 0xf93, EVENT(Func_02000d90)},
    {0x50008905, 30, CONDITION_ALWAYS, EVENT(Func_020005e8)},
    {0x50008905, 31, CONDITION_ALWAYS, EVENT(Func_02000608)},
    {TRIGGERS_END},
};

struct Trigger gData_02001714[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {0x50008905, 40, CONDITION_ALWAYS, EVENT(Func_020000a0)},
    {0x50008905, 41, CONDITION_ALWAYS, EVENT(Func_020000c0)},
    {0x50008905, 43, CONDITION_ALWAYS, EVENT(Func_020000e0)},
    {0x50008905, 42, CONDITION_ALWAYS, EVENT(Func_02000564)},
    {2, 50, CONDITION_ALWAYS, EVENT(Func_0200094c)},
    {0x1815, 8, 0x200, EVENT(Func_02000868)},
    {TRIGGERS_END},
};
