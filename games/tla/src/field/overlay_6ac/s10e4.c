#include "ov6ac.h"

void Func_020010e4(void)
{
    volatile u16 *color = (volatile u16 *)0x0500021e;

    Func_020031e0();
    Func_02003280(0);
    Call2((void (*)())Func_02003250, 0x10000, 0);
    Call2((void (*)())Func_02003248, 0x10003, 0);
    Func_02003258(20);
    WaitFrames(20);
    *color = 0x7fff;
    Call1((void (*)())WaitFrames, 1);
    Func_020031c0((s32)MsgMankindWithThe, 0, 0);
    Call2((void (*)())Func_02003248, 0x10000, 0);
    Func_02003258(20);
    WaitFrames(20);
    Func_020031e8();
}

void Func_02001154(void)
{
    volatile u16 *color = (volatile u16 *)0x0500021e;

    Func_020031e0();
    Func_02003280(0);
    Call2((void (*)())Func_02003250, 0x10000, 0);
    Call2((void (*)())Func_02003248, 0x10003, 0);
    Func_02003258(20);
    WaitFrames(20);
    *color = 0x7fff;
    Call1((void (*)())WaitFrames, 1);
    Func_020031c0((s32)MsgFishWithCold, 0, 0);
    Call2((void (*)())Func_02003248, 0x10000, 0);
    Func_02003258(20);
    WaitFrames(20);
    Func_020031e8();
}

void Func_020011c4(void)
{
    volatile u16 *color = (volatile u16 *)0x0500021e;

    Func_020031e0();
    Func_02003280(0);
    Call2((void (*)())Func_02003250, 0x10000, 0);
    Call2((void (*)())Func_02003248, 0x10003, 0);
    Func_02003258(20);
    WaitFrames(20);
    *color = 0x7fff;
    Call1((void (*)())WaitFrames, 1);
    Func_020031c0((s32)MsgDragonsBurningWith, 0, 0);
    Call2((void (*)())Func_02003248, 0x10000, 0);
    Func_02003258(20);
    WaitFrames(20);
    Func_020031e8();
}

void Func_02001234(void)
{
    volatile u16 *color = (volatile u16 *)0x0500021e;

    Func_020031e0();
    Func_02003280(0);
    Call2((void (*)())Func_02003250, 0x10000, 0);
    Call2((void (*)())Func_02003248, 0x10003, 0);
    Func_02003258(20);
    WaitFrames(20);
    *color = 0x7fff;
    Call1((void (*)())WaitFrames, 1);
    Func_020031c0((s32)MsgBirdOnThe, 0, 0);
    Call2((void (*)())Func_02003248, 0x10000, 0);
    Func_02003258(20);
    WaitFrames(20);
    Func_020031e8();
}

void Func_020012a4(s32 x, s32 z, s32 kind)
{
    struct ObjectRuntime *object = (struct ObjectRuntime *)Value4((s32 (*)())Func_02003128, 403, x << 16, 0, z << 16);
    struct Ov6acAnimation *animation;

    if (object != 0) {
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Func_02003118(object, 3);
        animation = object->animation;
        if (kind != 0) {
            animation->layer = 2;
            Func_02003120(object, Data_0200387c);
        } else {
            animation->layer = 1;
            object->velocity_x = (((u32)(Random16Far() * 8) >> 16) - 4) << 16;
            object->velocity_y = 0x40000;
            Func_02003120(object, Data_02003900);
        }
    }
}

void Func_02001324(s32 x, s32 z, s32 c, s32 d, s32 e, s32 f)
{
    u32 i;
    s32 px;
    s32 pz;

    Func_020032b8(144);
    Func_020012a4(x, z + 12, 1);
    for (i = 0; i < 3; i++) {
        px = x + ((u32)(Random16Far() * 12) >> 16) - 6;
        pz = z + ((u32)(Random16Far() * 12) >> 16) - 6;
        Func_020012a4(px, pz, 0);
    }
    WaitFrames(4);
    Func_02003148(c, d, e + 64, f, 1, 1);
}
