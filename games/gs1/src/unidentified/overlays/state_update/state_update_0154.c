#include "types.h"

struct Args_02003ffc {
    s32 first;
    s32 unused;
    s32 last;
};

void Func_02008054();

void Func_02003ffc(void)
{
    struct Args_02003ffc args;

    args.first = 0;
    args.last = 1;
    Func_02008054(&args);
}
