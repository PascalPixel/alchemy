#include "types.h"

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct Actor *Func_02004642(s32);
extern void Func_0200233e(void);

void Func_02001530(void)
{
    struct Actor *actor = Func_02004642(0);

    if ((actor->f08 >> 20) != 30 || (actor->f10 >> 20) != 52) {
        Func_0200233e();
    }
}
