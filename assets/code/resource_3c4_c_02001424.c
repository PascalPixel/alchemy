typedef unsigned char u8;
typedef signed int s32;

struct Actor02001424 {
    u8 head[8];
    s32 x;
    s32 y;
    s32 z;
};

extern struct Actor02001424 *Func_02004538(s32);
extern s32 Func_02002116(s32 *);
extern void Func_0200282e(void);

void Func_02001424(void)
{
    struct Actor02001424 *actor = Func_02004538(0);
    s32 target[3];

    target[0] = actor->x;
    target[1] = actor->y;
    target[2] = actor->z + 0x00200000;
    if (Func_02002116(target) != 0) {
        Func_0200282e();
    }
}
