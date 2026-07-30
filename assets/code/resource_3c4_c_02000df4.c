typedef signed int s32;

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct Actor *Func_02003f08(s32);
extern void Func_02001ae4(s32 *);

void Func_02000df4(void)
{
    s32 point[3];
    struct Actor *actor = Func_02003f08(0);

    point[0] = actor->f08;
    point[1] = actor->f0c;
    point[2] = actor->f10 + 0xFFE00000;
    Func_02001ae4(point);
}
