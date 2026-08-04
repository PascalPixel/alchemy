#include "types.h"

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct Actor *Func_02004622(s32);
extern void Func_02002872(void);

void Func_02001510(void)
{
    struct Actor *actor = Func_02004622(0);

    if ((actor->f08 >> 20) != 32 || (actor->f10 >> 20) != 50) {
        Func_02002872();
    }
}
