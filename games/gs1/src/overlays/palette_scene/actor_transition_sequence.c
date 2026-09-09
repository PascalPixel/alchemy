#include "types.h"

#define PaletteScene_RunActorTransitionSequence Func_02000488

extern s32 Data_02009dd4;
extern s32 Data_02009dcc;
extern s32 Data_02009dc0[3];
extern const s32 SceneAction_ActorOneEntry[];
extern const s32 SceneAction_ActorTwoEntry[];
extern const s32 SceneAction_ActorThreeEntry[];
extern const s32 SceneAction_GroupFinish[];
void Func_020012b4(void);
void Func_020015a0(void);

/* The transition callback advances the polled state to 24. The palette
 * alternation runs four cycles; actor three follows the saved game flag. */

s32 Func_02001980();
void Func_020019e0();
void Func_02001a40();
void Func_02001ab0();
void Func_02001960();
void Func_020019b8();
u8 *Func_020019b0();
void Func_020019e8();
void Func_020019c0();
void Func_020019c8();
void Func_02001990();
void Func_020018b8();
void Func_02001a88();
void Func_02001908();
s32 Func_02001910();
void Func_02001a58();
void Func_02001a60();
void Func_02001a68();
s32 Func_02001918();
void Func_020012f4();
void Func_02001a20();
void Func_02001a30();
void Func_02001a08();
void Func_02001a10();
void Func_02001a50();
void Func_02001a38();
s32 Func_02001a28();
s32 Func_020019a8();
void Func_020019f0();
void Func_020019f8();
void Func_02001a78();
void Func_020019d0();
s32 Func_02001988();
void Func_0200172c();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void PaletteScene_RunActorTransitionSequence(void)
{
    s32 actorThreeEnabled;
    u8 *object;
    s32 *transitionState;
    s32 cycle;
    s32 sceneWorkSlot;
    s32 effectCallback;
    const s32 *finalActions;

    actorThreeEnabled = Func_02001980(3);
    Call3(Func_020019e0, 0, 0x148, 212);
    Call3(Func_02001a40, 0, 0xc000, 20);
    Func_02001ab0(17);
    Call2(Func_02001960, 0x14ed, 1);
    Call3(Func_020019b8, 1, 0x10000, 0x8000);
    Call3(Func_020019b8, 2, 0x10000, 0x8000);
    object = Func_020019b0(0);
    if (object != 0) {
        Func_020019e8(1, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    object = Func_020019b0(0);
    if (object != 0) {
        Func_020019e8(2, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    Func_020019c0(1, SceneAction_ActorOneEntry);
    Func_020019c0(2, SceneAction_ActorTwoEntry);
    if (actorThreeEnabled != 0) {
        Call3(Func_020019b8, 3, 0x10000, 0x8000);
        object = Func_020019b0(0);
        if (object != 0) {
            Func_020019e8(3, *(s32 *)(object + 8), *(s32 *)(object + 16));
        }
        Func_020019c0(3, SceneAction_ActorThreeEntry);
    }
    Func_020019c8(2);
    Func_02001990(40);
    Func_020018b8(0);
    Func_02001a88(32);
    Func_02001908(40);
    transitionState = &Data_02009dd4;
    *transitionState = 0;
    Value2(Func_02001910, (s32)Func_020012b4, 0xc80);
    Func_02001990(40);
    Call3(Func_02001a40, 1, 0x6000, 20);
    Call2(Func_02001a58, 0x33333, 0x6666);
    Call4(Func_02001a60, 0x1000000, -1, 0xfe0000, 1);
    Func_02001a68();
    Func_02001ab0(246);
    Func_02001990(40);
    Call3(Func_02001a40, 2, 0x2000, 20);
    Call4(Func_02001a60, 0x19d0000, -1, 0x1050000, 1);
    Func_02001a68();
    Func_02001ab0(246);
    Func_02001990(40);
    Call3(Func_02001a40, 0, 0x4000, 0);
    Call3(Func_02001a40, 3, 0x4000, 20);
    Call4(Func_02001a60, 0x1460000, -1, 0x1800000, 1);
    Func_02001a68();
    Func_02001ab0(246);
    if (*transitionState != 24) {
        do {
            Func_02001908(1);
        } while (*transitionState != 24);
    }
    Value1(Func_02001918, (s32)Func_020012b4);
    Func_02001908(10);
    cycle = 0;
    do {
        Func_020018b8(0);
        Func_02001a88(6);
        Func_02001908(6);
        Func_020018b8(1);
        Func_02001a88(6);
        cycle = (cycle + 1);
        Func_02001908(6);
    } while ((u32)cycle <= 3);
    Func_020018b8(0);
    Func_02001a88(40);
    Func_02001908(80);
    Call4(Func_02001a60, 0x1480000, 0x80000, 0xd40000, 1);
    Func_02001a68();
    Func_02001990(40);
    Func_020012f4(10, 1);
    Func_02001990(40);
    Func_02001ab0(7);
    Call1(Func_02001a20, 0x14ee);
    Func_02001a30(8, 0);
    Func_02001a08(0, 2);
    Func_02001a08(1, 2);
    Func_02001a08(3, 2);
    Func_02001a10(2, 2);
    Call3(Func_02001a40, 0, 0xc000, 0);
    Call3(Func_02001a40, 1, 0xc000, 0);
    Call3(Func_02001a40, 3, 0xc000, 0);
    Call3(Func_02001a40, 2, 0xc000, 20);
    Func_020012f4(10, 2);
    Func_02001990(20);
    Func_020012f4(10, 3);
    Func_02001990(40);
    Func_020012f4(10, 1);
    Func_02001990(20);
    Func_02001a30(8, 0);
    Call3(Func_02001a50, 0, 0x105, 0);
    Call3(Func_02001a50, 1, 0x105, 0);
    Call3(Func_02001a50, 3, 0x105, 0);
    Call3(Func_02001a50, 2, 0x105, 40);
    Call4(Func_02001a60, 0xea0000, 0, 0xe80000, 1);
    Func_02001a68();
    Func_02001990(40);
    Func_020012f4(11, 1);
    Func_02001990(40);
    Func_020012f4(11, 3);
    Func_02001990(20);
    Call3(Func_02001a38, 0x4009, 0, 20);
    Func_020012f4(11, 2);
    Func_02001990(10);
    Call3(Func_02001a40, 0, 0x6000, 0);
    Call3(Func_02001a40, 1, 0x6000, 0);
    Call3(Func_02001a40, 2, 0x6000, 0);
    Call3(Func_02001a40, 3, 0x6000, 20);
    Func_020012f4(11, 3);
    Func_02001990(20);
    Func_020012f4(11, 2);
    Func_02001990(20);
    Func_020012f4(11, 3);
    Call3(Func_02001a38, 0x4009, 0, 10);
    Func_020012f4(10, 0);
    Func_02001990(20);
    Call2(Func_02001a30, 0x8008, 0);
    Func_020012f4(10, 1);
    Func_02001990(20);
    Value2(Func_02001a28, 0x8008, 0);
    Call3(Func_02001a40, 0, 0xc000, 0);
    Call3(Func_02001a40, 1, 0xe000, 0);
    Call3(Func_02001a40, 2, 0xa000, 0);
    Call3(Func_02001a40, 3, 0xc000, 0);
    if (Value2(Func_020019a8, 0, 0) == 0) {
        Call2(Func_02001a30, 0x4009, 0);
        Call2(Func_02001a30, 0x8008, 0);
    } else {
        sceneWorkSlot = 0x3001ebc;
        *(u16 *)((*(s32 *)sceneWorkSlot + 0x1d8)) += 2;
        Call3(Func_02001a50, 3, 0x103, 0);
        Call3(Func_02001a50, 1, 0x103, 0);
        Call3(Func_02001a50, 2, 0x103, 40);
        Func_020019f0(1, 4);
        Func_02001a30(1, 0);
        if (actorThreeEnabled != 0) {
            Func_02001a10(3, 2);
            Func_02001a30(3, 0);
        } else {
            *(u16 *)((*(s32 *)sceneWorkSlot + 0x1d8)) += 1;
        }
        Func_020019f8(2, 3);
        Func_02001a30(2, 0);
        Call2(Func_02001a30, 0x4009, 0);
        Call2(Func_02001a30, 0x8008, 0);
    }
    Func_020019f0(0, 3);
    Func_020019f0(1, 3);
    Func_020019f0(3, 3);
    Func_020019f8(2, 3);
    Call4(Func_02001a60, 0x1480000, 0x80000, 0xd40000, 1);
    Func_02001a68();
    Func_02001990(20);
    Func_020012f4(10, 0);
    Func_02001990(20);
    Func_020018b8(0);
    Func_02001a88(1);
    Func_02001908(1);
    Call2(Func_02001a78, 0x406218, 1);
    Func_02001a88(40);
    Func_02001990(60);
    Data_02009dcc = 0;
    Data_02009dc0[0] = 0x1480000;
    Data_02009dc0[1] = 0x300000;
    effectCallback = (s32)Func_020015a0;
    Data_02009dc0[2] = 0xcd0000;
    Value2(Func_02001910, effectCallback, 0xc80);
    Func_02001990(100);
    Func_02001918(effectCallback);
    Call2(Func_02001a78, 0x7fff, 0);
    Func_02001a88(60);
    Func_02001990(100);
    Func_020018b8(0);
    Func_02001a88(20);
    Func_02001990(40);
    Func_020012f4(10, 1);
    Func_02001990(10);
    Call1(Func_02001a20, 0x14fb);
    Call2(Func_02001a30, 0x8008, 0);
    Func_020019f0(0, 3);
    Func_020019f0(1, 3);
    Func_020019f0(3, 3);
    Func_020019f8(2, 3);
    Call4(Func_02001a60, 0xea0000, 0, 0xe80000, 1);
    Func_02001a68();
    Func_02001990(20);
    Call2(Func_02001a30, 0x4009, 0);
    Call3(Func_02001a38, 0x8008, 0, 10);
    Func_02001a10(1, 2);
    Call3(Func_02001a40, 0, 0x6000, 0);
    Call3(Func_02001a40, 1, 0xe000, 10);
    Value2(Func_02001a28, 1, 0);
    if (Value2(Func_020019a8, 0, 0) == 0) {
        Call3(Func_02001a50, 1, 0x102, 40);
    } else {
        Func_020019f8(1, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02001a30(1, 0);
    Func_020012f4(10, 4);
    Func_02001990(20);
    Call1(Func_02001a20, 0x1501);
    Call2(Func_02001a30, 0x8008, 0);
    Call3(Func_02001a40, 0, 0xc000, 0);
    Call3(Func_02001a40, 1, 0xc000, 0);
    Call2(Func_02001a30, 0x8008, 0);
    Func_020019f0(0, 3);
    Func_020019f0(1, 3);
    Func_020019f0(3, 3);
    Func_020019f8(2, 3);
    Func_020012f4(10, 4);
    Func_02001990(20);
    Call3(Func_02001a38, 0x8008, 0, 20);
    Func_020012f4(11, 0);
    Call3(Func_02001a38, 0x4009, 0, 20);
    Func_020012f4(11, 3);
    Func_02001990(40);
    Func_020012f4(11, 1);
    Func_02001990(20);
    Call3(Func_02001a38, 0x4009, 0, 20);
    Func_020012f4(10, 2);
    Func_02001990(20);
    Call2(Func_02001a30, 0x8008, 0);
    Call3(Func_02001a50, 0, 0x102, 0);
    Call3(Func_02001a50, 1, 0x102, 0);
    Call3(Func_02001a50, 3, 0x102, 0);
    Call3(Func_02001a50, 2, 0x102, 80);
    Func_020012f4(11, 5);
    Func_02001990(60);
    Func_020012f4(11, 3);
    Func_02001990(20);
    Call3(Func_02001a38, 0x4009, 0, 20);
    Func_020012f4(10, 5);
    Func_02001990(40);
    Func_020012f4(10, 2);
    Func_02001990(20);
    Call3(Func_02001a38, 0x4008, 0, 20);
    Func_02001a10(1, 2);
    Call3(Func_02001a40, 1, 0x8000, 10);
    Func_02001a30(1, 0);
    Call3(Func_02001a40, 2, 0x8000, 20);
    Call2(Func_02001a30, 0x8002, 0);
    Func_020012f4(11, 4);
    Func_02001990(20);
    Call3(Func_02001a38, 0x4009, 0, 20);
    Call3(Func_02001a40, 1, 0xe000, 0);
    Call3(Func_02001a40, 0, 0x6000, 10);
    Func_02001a38(1, 0, 20);
    Func_020012f4(10, 1);
    Call3(Func_02001a38, 0x8008, 0, 10);
    Func_020012f4(10, 2);
    Func_02001990(20);
    Func_020012f4(11, 3);
    Func_02001990(40);
    Func_020012f4(11, 0);
    Func_02001990(20);
    Func_020018b8(0);
    Func_02001a88(1);
    Func_02001908(1);
    Call2(Func_02001a78, 0x406218, 1);
    Func_02001a88(40);
    Func_02001990(60);
    Data_02009dcc = 0;
    Data_02009dc0[0] = 0x880000;
    Data_02009dc0[1] = 0x140000;
    effectCallback = (s32)Func_020015a0;
    Data_02009dc0[2] = 0x1020000;
    Value2(Func_02001910, effectCallback, 0xc80);
    Func_02001990(100);
    Call3(Func_02001a40, 0, 0x6000, 0);
    Call3(Func_02001a40, 1, 0x6000, 0);
    Call3(Func_02001a40, 3, 0x6000, 0);
    Call3(Func_02001a40, 2, 0x6000, 40);
    Func_02001a08(2, 1);
    Call3(Func_02001a50, 2, 0x100, 20);
    Call3(Func_02001a38, 0x8002, 0, 10);
    Func_02001a10(0, 2);
    Call3(Func_02001a40, 0, 0x2000, 10);
    Func_020019f8(0, 3);
    Func_020012f4(10, 4);
    Func_02001990(20);
    Call2(Func_02001a30, 0x8008, 0);
    Call3(Func_02001a50, 2, 0x101, 60);
    Call3(Func_02001a40, 2, 0xc000, 10);
    Call3(Func_02001a38, 0x8002, 0, 10);
    Call3(Func_02001a40, 0, 0xc000, 0);
    Call3(Func_02001a40, 3, 0xc000, 0);
    Call3(Func_02001a40, 1, 0xc000, 20);
    Call3(Func_02001a38, 0x8008, 0, 10);
    Func_020019f0(0, 3);
    Func_020019f0(1, 3);
    Func_020019f0(3, 3);
    Func_020019f8(2, 3);
    Func_02001990(10);
    Call3(Func_02001a40, 0, 0x6000, 0);
    Call3(Func_02001a40, 1, 0x6000, 0);
    Call3(Func_02001a40, 3, 0x6000, 0);
    Call3(Func_02001a40, 2, 0x6000, 120);
    Func_02001918(effectCallback);
    Func_02001990(60);
    Func_020018b8(0);
    Func_02001a88(40);
    Func_020012f4(10, 2);
    Func_02001990(20);
    Call3(Func_02001a38, 0x8008, 0, 20);
    Func_020012f4(11, 3);
    Call2(Func_02001a30, 0x4009, 0);
    Call2(Func_02001a30, 0x8008, 0);
    Func_020012f4(11, 4);
    Func_02001990(20);
    Call3(Func_02001a38, 0x4009, 0, 10);
    Func_02001a08(0, 2);
    Func_02001a08(1, 2);
    Func_02001a08(3, 2);
    Func_02001a10(2, 2);
    Func_020012f4(10, 1);
    Value2(Func_02001a28, 0x8008, 0);
    Call3(Func_02001a40, 0, 0xc000, 0);
    Call3(Func_02001a40, 1, 0xe000, 0);
    Call3(Func_02001a40, 3, 0xc000, 0);
    Call3(Func_02001a40, 2, 0xa000, 0);
    if (Value2(Func_020019a8, 0, 0) == 1) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02001990(10);
    Func_020012f4(10, 2);
    Func_02001990(20);
    Func_020012f4(11, 3);
    Func_02001990(40);
    Func_020012f4(10, 1);
    Func_02001990(20);
    Call3(Func_02001a38, 0x8008, 0, 10);
    Call3(Func_02001a40, 0, 0x4000, 0);
    Func_02001a40(1, 0, 0);
    Call3(Func_02001a40, 3, 0xc000, 0);
    Call3(Func_02001a40, 2, 0x8000, 10);
    Func_020019f0(0, 3);
    Func_020019f0(1, 3);
    Func_020019f0(3, 3);
    Func_020019f8(2, 3);
    Func_02001ab0(17);
    finalActions = SceneAction_GroupFinish;
    Func_020019c0(1, finalActions);
    if (actorThreeEnabled != 0) {
        Func_020019c0(3, finalActions);
    }
    Call2(Func_020019d0, 2, (s32)finalActions);
    Func_020012f4(10, 4);
    Func_020012f4(10, 4);
    Func_02001990(20);
    Call1(Func_02001a20, 0x1519);
    Call2(Func_02001a30, 0x8008, 0);
    Func_020012f4(11, 4);
    Func_020012f4(11, 4);
    Func_02001990(20);
    Call3(Func_02001a38, 0x4009, 0, 10);
    Func_020019f8(0, 3);
    Value1(Func_02001988, 0x845);
    Func_02001ab0(1);
    Func_0200172c(184, 185);
}
