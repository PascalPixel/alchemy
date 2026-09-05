#include "types.h"

struct Args_02004030 {
    s32 first;
    s32 unused;
    s32 last;
};

void Func_02008088();

void SceneState_CallHandlerWithFlagPair(void)
{
    struct Args_02004030 args;

    args.first = 1;
    args.last = 0;
    Func_02008088(&args);
}
