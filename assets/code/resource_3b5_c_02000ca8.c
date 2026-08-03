typedef unsigned char u8;
typedef signed int s32;
struct Actor {
    u8 pad00[0x23];
    u8 field23;
    u8 pad24[0x31];
    u8 field55;
};
extern struct Actor *Func_02001a2c(s32);
extern void Func_020019e0(struct Actor *, s32);
extern void Func_02001a90(s32, s32);
extern void Func_02001a04(s32, s32, s32, s32, s32, s32);
extern void Func_02001a3a(s32);
void Func_02000ca8(void)
{
    struct Actor *actor = Func_02001a2c(9);
    if (actor != 0) {
        Func_020019e0(actor, 0);
        actor->field23 = 2;
        actor->field55 = 0;
    }
    Func_02001a90(9, 5);
    {
        s32 first = 34;
        s32 second = 16;
        Func_02001a04(36, 16, 1, 1, first, second);
    }
    Func_02001a3a(0x201);
}
