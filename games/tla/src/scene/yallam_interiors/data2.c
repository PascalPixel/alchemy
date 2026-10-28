#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"

void Func_0200008c(void);
void Func_020000cc(void);
void Func_0200010c(void);
void Func_0200014c(void);
void Func_0200018c(void);
void Func_020001b4(void);
void Func_020001dc(void);
void Func_02000244(void);
void Func_0200029c(void);
void Func_020002fc(void);
void Func_0200035c(void);
void Func_020003bc(void);
void Func_02000524(void);
void Func_0200077c(void);

// clang-format off
struct ActorPlacement gData_02000dac[] = {
    {112, CONDITION_ALWAYS, 1, PIXELS(144), 0, PIXELS(188), FACING_EAST, 1, 0},
    {113, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(152), FACING_SOUTH, 1, 0},
    {108, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(405), FACING_SOUTH, 1, 0},
    {109, CONDITION_ALWAYS, 1, PIXELS(112), 0, PIXELS(405), FACING_SOUTH, 1, 0},
    {110, CONDITION_ALWAYS, 1, PIXELS(348), 0, PIXELS(405), FACING_SOUTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(408), FACING_SOUTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(112), 0, PIXELS(408), FACING_SOUTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(348), 0, PIXELS(408), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_02000e84[] = {
    {1, 10, CONDITION_ALWAYS, 10},
    {1, 11, CONDITION_ALWAYS, 11},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gData_02000ea8[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors09)},
    {0, 9, CONDITION_ALWAYS, EVENT(Func_02000244)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors13)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors14)},
    {0, 12, CONDITION_ALWAYS, EVENT(Func_020003bc)},
    {0, 13, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors28)},
    {0, 14, CONDITION_ALWAYS, EVENT(Func_02000524)},
    {0, 15, CONDITION_ALWAYS, EVENT(Func_0200077c)},
    {0, 16, CONDITION_ALWAYS, EVENT(Func_0200014c)},
    {0, 17, CONDITION_ALWAYS, EVENT(Func_02000524)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors11)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors12)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors15)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors16)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors29)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors30)},
    {0x8d15, 14, CONDITION_ALWAYS, EVENT(Func_0200008c)},
    {0x8d15, 15, CONDITION_ALWAYS, EVENT(Func_020000cc)},
    {0x8d15, 16, CONDITION_ALWAYS, EVENT(Func_0200010c)},
    {0x8d15, 17, CONDITION_ALWAYS, EVENT(Func_0200008c)},
    {0x173, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgYallamInteriors40))},
    {TRIGGERS_END},
};

struct Trigger gData_02000fe0[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors17)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors18)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_0200029c)},
    {0, 11, CONDITION_ALWAYS, EVENT(Func_020002fc)},
    {0, 12, CONDITION_ALWAYS, EVENT(Func_0200035c)},
    {0, 13, CONDITION_ALWAYS, EVENT(Func_0200029c)},
    {0, 14, CONDITION_ALWAYS, EVENT(Func_020002fc)},
    {0, 15, CONDITION_ALWAYS, EVENT(Func_0200035c)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors19)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors20)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors22)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors24)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors26)},
    {0x8d15, 13, CONDITION_ALWAYS, EVENT(Func_0200018c)},
    {0x8d15, 14, CONDITION_ALWAYS, EVENT(Func_020001b4)},
    {0x8d15, 15, CONDITION_ALWAYS, EVENT(Func_020001dc)},
    {TRIGGERS_END},
};
