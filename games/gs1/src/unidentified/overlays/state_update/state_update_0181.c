#include "types.h"

struct Args_02004014 {
    s32 first;
    s32 unused;
    s32 last;
};

void Func_0200806e();

void Func_02004014(void)
{
    struct Args_02004014 args;

    args.first = -1;
    args.last = 0;
    Func_0200806e(&args);
}
