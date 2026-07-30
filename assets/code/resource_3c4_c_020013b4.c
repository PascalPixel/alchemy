typedef signed int s32;

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct Actor *Func_020044c8(s32);
extern void Func_020020a6(s32 *);

void Func_020013b4(void)
{
    s32 point[3];
    struct Actor *actor = Func_020044c8(0);

    point[0] = actor->f08 + 0x200000;
    point[1] = actor->f0c;
    point[2] = actor->f10;
    Func_020020a6(point);
}
