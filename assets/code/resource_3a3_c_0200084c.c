typedef signed int s32;

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
};

extern struct Actor *Func_0200171a(s32);
extern void Func_020016f0(s32);
extern void Func_02001700(s32);

void Func_0200084c(void)
{
    struct Actor *actor = Func_0200171a(19);

    if ((actor->f08 >> 20) == 22) {
        Func_020016f0(0x906);
    } else {
        Func_02001700(0x906);
    }
}
