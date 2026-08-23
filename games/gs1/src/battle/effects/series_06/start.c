#include "types.h"

struct Effect06Child {
    u8 padding[35];
    u8 flags;
};

struct Effect06State {
    u8 padding_00[20];
    struct Effect06Child *child;
    u8 padding_18[8];
    u8 active;
    u8 padding_21[20];
    s8 mode;
};

extern struct Effect06State *Data_03001f30;
#define RunBattleEffect06 Func_08099160
void RunBattleEffect06(void);

#define StartBattleEffect06 Func_08099128
void StartBattleEffect06(void)
{
    struct Effect06State *state = Data_03001f30;
    struct Effect06Child *child = state->child;

    if (child != 0) {
        if (state->mode != 0) {
            state->active = 1;
        }
        child->flags |= 2;
        RunBattleEffect06();
    }
}
