typedef signed int s32;

struct Args_02003fe4 {
    s32 first;
    s32 unused;
    s32 last;
};

void Func_0200803c();

void Func_02003fe4(void)
{
    struct Args_02003fe4 args;

    args.first = 0;
    args.last = -1;
    Func_0200803c(&args);
}
