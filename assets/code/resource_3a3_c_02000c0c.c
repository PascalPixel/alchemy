typedef unsigned char u8;
typedef signed int s32;

struct Actor02000c0c {
    u8 head[18];
    u8 gap18[6];
    s32 span;
    s32 reach;
    u8 gap32[48];
    u8 *attached;
    u8 gap84;
    u8 state;
};

extern void Func_02001b52(struct Actor02000c0c *, s32);
extern void Func_02001aaa(struct Actor02000c0c *, s32);

void Func_02000c0c(struct Actor02000c0c *actor)
{
    u8 *state = &actor->state;
    s32 clear = 0;
    u8 *attached;

    *state = (u8) clear;
    attached = actor->attached;
    clear -= 13;
    attached[9] = (clear & attached[9]) | 4;
    Func_02001b52(actor, 3);
    Func_02001aaa(actor, 0);
    actor->span = 0x4CCC;
    actor->reach = 0x4CCC;
}
