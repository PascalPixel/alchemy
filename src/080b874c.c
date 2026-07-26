#include "types.h"

struct Transition_080b874c {
    s32 value;
    s32 timer;
    u8 pad08[12];
    s32 flag;
};

extern struct Transition_080b874c *Data_03001f00;

u8 *Func_08077008(s32);
s32 Func_080b8f08(u8 *);
void Func_08015118(void);
void Func_080151c8(s32);
s32 Func_080b8824(s16 *);
void Func_080030f8(s32);
s32 Func_080b8888(s16 *);
s32 Func_080b8c1c(s16 *);
s32 Func_080b88d0(s16 *);
void Func_08015220(void);

s32 Func_080b874c(s16 *argument)
{
    struct Transition_080b874c *transition;
    s32 id;
    s32 mode;
    u8 *object;

    id = argument[0];
    object = Func_08077008(id);
    if (*(s16 *)(object + 0x38) == 0)
        return -1;

    argument[5] = Func_080b8f08((u8 *)argument);
    transition = Data_03001f00;
    if (argument[0] > 4)
        mode = -0x2000;
    else
        mode = 0x2000;
    transition->value = mode;
    transition->timer = 60;
    Func_08015118();

    switch (argument[3]) {
    case 99:
        Func_080151c8(0x843);
        if (Func_080b8824(argument) != 0)
            return 1;
        break;
    case 3:
        Func_080030f8(45);
        Func_080b8888(argument);
        break;
    case 2:
        Func_080030f8(45);
        Func_080b8c1c(argument);
        break;
    case 0:
    default: {
        struct Transition_080b874c *other_transition = Data_03001f00;
        other_transition->flag = 0;
        Func_080b8c1c(argument);
        other_transition->flag = 0;
        break;
    }
    case 1:
        Func_080b88d0(argument);
        break;
    }

    Func_08015220();
    return 0;
}
