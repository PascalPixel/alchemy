typedef signed int s32;

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct Actor *Func_020043da(s32);
extern struct Actor *Func_020043e2(s32);

void Func_02001e94(void)
{
    struct Actor *target = Func_020043da(14);
    struct Actor *source = Func_020043e2(9);

    target->f0c = 0x200000;
    target->f08 = source->f08;
    target->f10 = source->f10 + 0x10000;
}
