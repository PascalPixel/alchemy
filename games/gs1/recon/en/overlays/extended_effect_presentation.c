#include "types.h"

#define FieldScene_RunExtendedEffectPresentation Func_02001410

extern u8 SceneMessage_ActorOneChoiceBase;

void Func_0200135c();
void Func_020010c4();
void Func_0200293c();
void Func_02002970();
void Func_0200300c();

u8 * Func_020034ec();
s32 Func_020033f4();
void Func_020034cc();
void Func_02003624();
void Func_020035ec();
void Func_020035f4();
void Func_02003534();
void Func_020033e4();
void Func_02002820();
void Func_02003494();
void Func_02002c9c();
u8 * Func_020035d4();
void Func_02003464();
void Func_020035a4();
void Func_020034fc();
void Func_0200354c();
void Func_0200350c();
void Func_02003514();
void Func_02003504();
void Func_0200359c();
void Func_0200355c();
void Func_0200357c();
s32 Func_02003584();
s32 Func_020034e4();
void Func_02003544();
void Func_02003594();
void Func_020035ac();
void Func_0200353c();
void Func_020035bc();
void Func_020035c4();
void Func_020035cc();
void Func_02002d48();
void Func_0200352c();
void Func_0200351c();
void Func_02003524();
void Func_0200358c();
void Func_02003484();
void Func_020035b4();
void Func_02003554();
s32 Func_020033ec();
void Func_02002ff0();
void Func_02002d94();
void Func_0200284c();
void Func_0200349c();
void Func_0200356c();
void Func_020029a4();
void Func_020034ac();
u8 * Func_02003454();
u8 * Func_02003414();
void Func_020034b4();
s32 Func_02003434();
void Func_02003424();
void Func_0200360c();
void Func_0200345c();
void Func_02002c1c();
void Func_020028dc();
void Func_02003000();
s32 Func_020034c4();
void Func_020035dc();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ u8 * Pointer0(u8 * (*f)())
{
    return f();
}

static __inline__ u8 * Pointer1(u8 * (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ u8 * Pointer2(u8 * (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ u8 * Pointer4(u8 * (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void FieldScene_RunExtendedEffectPresentation(void)
{
    u8 *work;
    u8 *effect;
    u8 *actor0;
    u8 *actor15;
    u8 *textureBuffer;
    u8 *actor1;
    u8 *record;
    s32 step;
    s32 frame;
    s32 flags;
    s32 horizontalStep;
    s32 actor0Sprite;
    s32 prompt;
    s32 zero;
    s32 delay;
    s32 scale;
    s32 tileX;
    s32 shortDelay;
    s32 actorStepCallback;
    s32 savedHorizontalStep;
    s32 savedActor0Sprite;
    u8 *actor1Sprite;
    u8 *effectSprite;

    work = *(u8 **)0x03001ec4;
    actor15 = Pointer1(Func_020034ec, 15);
    Value1(Func_020033f4, (s32)Func_0200135c);
    *(s32 *)0x0200bb68 = 3;
    Func_020034cc(80);
    Func_02003624(17);
    Call2(Func_020035ec, 0x7fff, 0);
    Func_020035f4(40);
    Func_020034cc(40);
    Value1(Func_020033f4, (s32)Func_020010c4);
    step = 0;
    do {
        Func_02003534((step + 16), 0, 0);
        step = ((u32)((step + 1) << 24) >> 24);
    } while ((u32)step <= 15);
    Func_020033e4(1);
    Func_02003534(15, 0, 0);
    Func_02002820(0);
    frame = 0;
    Func_02002820(1);
    *(s32 *)((0x40c + work)) = frame;
    Call3(Func_02003494, 0x10000, 0x10000, 0x10000);
    Func_020034cc(80);
    Func_02002c9c();
    record = Pointer0(Func_020035d4);
    record[85] = frame;
    *(s32 *)((s32)record + 8) = 0xe70000;
    *(s32 *)((s32)record + 16) = 0x900000;
    *(s32 *)((s32)record + 56) = -0x80000000;
    *(s32 *)((s32)record + 60) = -0x80000000;
    *(s32 *)((s32)record + 64) = -0x80000000;
    *(s32 *)((s32)record + 12) = frame;
    *(s32 *)((s32)record + 36) = frame;
    *(s32 *)((s32)record + 44) = frame;
    Func_020033e4(4);
    Func_02003464();
    Func_020033e4(4);
    Func_020035a4(0, 3);
    Func_020035a4(1, 3);
    Call2(Func_020035ec, 0x10000, 0);
    Func_020035f4(40);
    Func_020034cc(40);
    actor0 = Pointer1(Func_020034ec, 0);
    actor1 = Pointer1(Func_020034ec, 1);
    actor1Sprite = *(u8 **)(actor1 + 80);
    horizontalStep = 0x6000;
    actor0Sprite = *(s32 *)(actor0 + 80);
    do {
        *(u16 *)(actor0Sprite + 30) += 0x100;
        *(u16 *)((s32)actor1Sprite + 30) += -256;
        *(s32 *)(actor0 + 8) += horizontalStep;
        *(s32 *)(actor1 + 8) = (*(s32 *)(actor1 + 8) - horizontalStep);
        savedHorizontalStep = horizontalStep;
        savedActor0Sprite = actor0Sprite;
        Func_020033e4(1);
        frame = (frame + 1);
        horizontalStep = savedHorizontalStep;
        actor0Sprite = savedActor0Sprite;
    } while ((u32)frame <= 19);
    Func_020034cc(40);
    Call3(Func_020034fc, 0, 0x20000, 0x10000);
    Call3(Func_020034fc, 1, 0x20000, 0x10000);
    record = Pointer1(Func_020034ec, 0);
    {
        s32 target = *(s32 *)((s32)record + 80);
        s32 shown = 0;

        *(u16 *)(target + 30) = shown;
    }
    record = Pointer1(Func_020034ec, 1);
    {
        s32 target = *(s32 *)((s32)record + 80);
        s32 shown = 0;

        *(u16 *)(target + 30) = shown;
    }
    Func_0200354c(0, 6, 0);
    Func_0200354c(1, 6, 0);
    Call3(Func_0200350c, 0, 246, 150);
    Func_02003514(1, 220, 150);
    Func_020035a4(0, 2);
    flags = 1;
    Func_020035a4(1, 2);
    *(u8 *)(Func_020034ec(0) + 35) |= flags;
    {
        u8 *record = Func_020034ec(1);
        u8 value = *(u8 *)&record[35];

        record[35] = (u8)(value | flags);
    }
    Func_02003504(0);
    Func_02003504(1);
    Call3(Func_0200359c, 0, 0x2000, 0);
    Func_020034cc(20);
    Call3(Func_0200359c, 1, 0xe000, 0);
    Func_020034cc(40);
    Call3(Func_0200359c, 0, 0x9000, 0);
    Func_020034cc(40);
    Call3(Func_0200359c, 1, 0x5000, 0);
    Func_020034cc(80);
    Call3(Func_0200359c, 0, 0x8000, 0);
    Func_020034cc(10);
    Call3(Func_0200359c, 1, 0x1000, 0);
    Func_020034cc(60);
    Func_0200355c(1, 2);
    prompt = (s32)&SceneMessage_ActorOneChoiceBase;
    Func_0200357c(prompt);
    Value2(Func_02003584, 1, 0);
    if (Value2(Func_020034e4, 0, 0) == 0) {
        Func_02003544(1, 3);
        Func_0200357c((prompt + 1));
    } else {
        Func_0200355c(1, 1);
        Func_0200357c((prompt + 2));
    }
    Func_02003594(1, 0, 60);
    Call3(Func_0200359c, 1, 0x3000, 0);
    Func_020034cc(40);
    Func_0200354c(1, 2, 0);
    Call3(Func_020035ac, 1, 0x102, 0);
    Func_020034cc(40);
    Call1(Func_0200357c, 0x10fb);
    Func_02003594(1, 0, 10);
    Func_0200354c(0, 2, 0);
    Func_020034cc(10);
    Call3(Func_0200359c, 0, 0x2000, 0);
    Func_020034cc(60);
    Call3(Func_020035ac, 0, 0x102, 0);
    Func_020034cc(80);
    Call3(Func_0200359c, 1, 0x1000, 0);
    (Func_020034cc)(10);
    Func_0200355c(1, 2);
    Value2(Func_02003584, 1, 0);
    if (Value2(Func_020034e4, 0, 0) == 0) {
        Func_020034cc(10);
        Func_0200355c(1, 2);
        (Func_020034cc)(10);
        Call3(Func_0200359c, 1, 0x3000, 0);
        Func_020034cc(60);
        Call3(Func_0200359c, 1, 0x1000, 0);
        Func_020034cc(10);
        Func_0200353c(1, 4);
        Call1(Func_0200357c, 0x10fd);
    } else {
        Func_020034cc(10);
        Func_0200355c(1, 2);
        Func_020034cc(10);
        Call3(Func_0200359c, 1, 0x3000, 0);
        Func_020034cc(60);
        Call3(Func_0200359c, 1, 0x1000, 0);
        Func_020034cc(10);
        Func_0200353c(1, 4);
        Call1(Func_0200357c, 0x10fe);
    }
    Func_02003594(1, 0, 40);
    Func_0200353c(0, 3);
    Func_02003544(1, 3);
    Func_020034cc(40);
    Call3(Func_0200359c, 0, 0x8000, 0);
    Call3(Func_0200359c, 1, 0x7000, 0);
    Call2(Func_020035bc, 0x20000, 0x4000);
    Call4(Func_020035c4, 0x11c0000, -1, 0x5c0000, 1);
    Func_020035cc();
    Func_020034cc(20);
    Call3(Func_0200359c, 0, 0x6000, 0);
    Call3(Func_0200359c, 1, 0x5000, 0);
    Call2(Func_020035bc, 0x18000, 0x3000);
    Call4(Func_020035c4, 0x7f0000, -1, 0xa20000, 1);
    Func_020035cc();
    Func_020034cc(40);
    Call3(Func_0200359c, 0, 0x2000, 0);
    Call3(Func_0200359c, 1, 0x3000, 0);
    Call2(Func_020035bc, 0x40000, 0x8000);
    Call4(Func_020035c4, 0x1300000, -1, 0x1260000, 1);
    Func_020035cc();
    Func_020034cc(20);
    Func_0200359c(0, 0, 0);
    Call3(Func_0200359c, 1, 0x1000, 0);
    Call2(Func_020035bc, 0x10000, 0x2000);
    Call4(Func_020035c4, 0x1900000, -1, 0xd70000, 1);
    Func_020035cc();
    Func_020034cc(60);
    Call2(Func_020035bc, 0x40000, 0x8000);
    Call4(Func_020035c4, 0x1110000, -1, 0x910000, 1);
    Func_020035cc();
    Func_020034cc(20);
    Func_0200355c(1, 2);
    Call1(Func_0200357c, 0x10ff);
    Func_02003594(1, 0, 20);
    Call3(Func_0200359c, 1, 0x3000, 0);
    Func_020034cc(40);
    Call3(Func_0200359c, 1, 0xd000, 0);
    Func_020034cc(20);
    Call3(Func_0200359c, 1, 0xf000, 0);
    Func_020034cc(60);
    Call3(Func_0200359c, 1, 0x3000, 0);
    Func_020034cc(40);
    Call3(Func_0200359c, 1, 0xf000, 0);
    Func_020034cc(10);
    Func_02003594(1, 0, 10);
    Func_02003544(0, 3);
    Func_020034cc(10);
    Func_02003624(23);
    Func_02002d48(1, 4, 0);
    *(s32 *)((0x40c + work)) = 0;
    Call3(Func_02003494, 0x50000, 0x50000, 0x10000);
    Func_020034cc(10);
    Func_02002d48(0, 40, 0);
    Call3(Func_020034fc, 0, 0x20000, 0x10000);
    Call3(Func_020034fc, 1, 0x20000, 0x10000);
    Func_0200354c(0, 6, 0);
    Func_0200354c(1, 6, 0);
    Func_0200350c(0, 243, 144);
    Func_0200350c(1, 202, 144);
    Func_0200352c(0);
    Func_020034cc(20);
    *(s32 *)((0x40c + work)) = 1;
    Call3(Func_02003494, 0x10000, 0x10000, 0x10000);
    Func_020034cc(60);
    Call3(Func_020035ac, 0, 0x102, 0);
    Call3(Func_020035ac, 1, 0x102, 0);
    Func_020034cc(80);
    Call3(Func_020034fc, 0, 0x6666, 0x3333);
    Call3(Func_020034fc, 1, 0x6666, 0x3333);
    Func_0200351c(1, 220, 150);
    Func_02003524(0, 246, 150);
    Func_0200353c(1, 1);
    Call3(Func_0200359c, 0, 0x8000, 0);
    Call3(Func_0200359c, 1, 0x7000, 0);
    Func_020034cc(60);
    Func_0200355c(1, 2);
    Func_020034cc(10);
    Call3(Func_0200359c, 1, 0x3000, 0);
    Func_020034cc(40);
    Value2(Func_02003584, 1, 0);
    if (Value2(Func_020034e4, 0, 0) == 0) {
        Call3(Func_0200359c, 1, 0x1000, 0);
        Func_020034cc(10);
        Func_02003544(1, 3);
    } else {
        Call3(Func_0200359c, 1, 0x1000, 0);
        Func_020034cc(10);
        Func_0200353c(1, 4);
        Call1(Func_0200357c, 0x1103);
    }
    Func_020034cc(20);
    Func_0200358c(1, 0);
    Call3(Func_02003534, 15, 0x6e0000, 0x980000);
    Func_020033e4(1);
    Call3(Func_020034fc, 15, 0x13333, 0x9999);
    Func_0200350c(15, 171, 152);
    Func_0200359c(15, 0, 0);
    record = Func_020034ec(15);
    Func_02003484((s32)record, 1);
    Call3(Func_020034fc, 1, 0x10000, 0x8000);
    Func_0200351c(1, 217, 182);
    Call3(Func_0200359c, 0, 0x7000, 0);
    Func_020034cc(10);
    Call2(Func_020035bc, 0x20000, 0x4000);
    Call4(Func_020035c4, 0xd90000, -1, 0xb00000, 1);
    Func_020035cc();
    Func_020034cc(20);
    Call2(Func_020035b4, 0, 0x102);
    Func_020034cc(20);
    Func_0200354c(0, 2, 0);
    Func_020034cc(10);
    Func_0200354c(0, 4, 0);
    Func_020034cc(30);
    Call3(Func_020035ac, 1, 0x100, 0);
    Func_0200353c(1, 1);
    Func_020034cc(40);
    Call3(Func_0200359c, 1, 0x2000, 0);
    Func_020034cc(4);
    Func_02003524(1, 231, 175);
    Call3(Func_0200359c, 1, 0xe000, 0);
    Call1(Func_0200357c, 0x1104);
    Func_0200358c(1, 0);
    Func_020034cc(40);
    Call3(Func_020035ac, 1, 0x101, 0);
    Func_020034cc(40);
    Call3(Func_0200359c, 1, 0xa000, 0);
    Func_020034cc(60);
    Func_02003554(1, 1);
    Call2(Func_020035b4, 1, 0x102);
    Func_020034cc(40);
    Func_02002d48(1, 20, 0);
    *(s32 *)((0x40c + work)) = 0;
    zero = 0;
    Call3(Func_02003494, 0x10000, 0x10000, 0x10000);
    Func_020034cc(40);
    Func_02003554(0, 2);
    Func_02003554(1, 2);
    Call1(Func_02003624, 0x121);
    Call2(Func_020035ec, 0x10000, 1);
    Call1(Func_020035f4, 40);
    *(s32 *)((0x40c + work)) = 1;
    Call3(Func_02003494, -1, -1, 0xe666);
    Func_020034cc(120);
    Func_0200358c(15, 0);
    Func_020034cc(20);
    Call3(Func_020035ac, 0, 0x102, 0);
    Call3(Func_020035ac, 1, 0x102, 0);
    Func_020034cc(100);
    Func_0200358c(15, 0);
    Func_020034cc(20);
    Call3(Func_02002d48, 1, 10, 0);
    *(s32 *)((0x40c + work)) = zero;
    Call3(Func_02003494, 0x20000, 0x20000, 0x10000);
    Func_020034cc(20);
    Call3(Func_020034fc, 0, 0x20000, 0x10000);
    Call3(Func_020034fc, 1, 0x20000, 0x10000);
    *(u8 *)(Func_020034ec(0) + 90) &= 254;
    *(u8 *)(Func_020034ec(1) + 90) &= 254;
    Func_0200354c(0, 4, 0);
    Func_0200354c(1, 4, 0);
    Call3(Func_0200350c, 0, 0x100, 150);
    Func_0200350c(1, 231, 180);
    Func_0200352c(1);
    Func_02002d48(0, 40, 0);
    flags = 1;
    Func_020034cc(20);
    *(u8 *)(Func_020034ec(0) + 90) |= flags;
    {
        u8 *record = Func_020034ec(1);
        u8 value = *(u8 *)&record[90];

        record[90] = (u8)(value | flags);
    }
    Func_02003554(0, 2);
    Func_0200355c(1, 2);
    Func_020034cc(40);
    Call3(Func_020035ac, 1, 0x103, 0);
    Func_020034cc(40);
    Func_0200358c(1, 0);
    Func_020034cc(20);
    Call3(Func_020035ac, 15, 0x101, 0);
    Func_020034cc(60);
    Call3(Func_0200359c, 0, 0x5000, 0);
    Call3(Func_0200359c, 1, 0xd000, 0);
    Func_020034cc(20);
    Call3(Func_0200359c, 0, 0x8000, 0);
    Call3(Func_0200359c, 1, 0xb000, 0);
    Func_020034cc(10);
    Func_02003624(107);
    Value2(Func_020033ec, (s32)Func_0200293c, 0xc80);
    Func_020034cc(10);
    Call3(Func_020035ac, 15, 0x100, 0);
    Func_020034cc(40);
    Call2(Func_020035bc, 0x10000, 0x2000);
    Call4(Func_020035c4, 0xba0000, -1, 0xa60000, 1);
    Func_0200350c(15, 130, 113);
    Func_0200352c(15);
    Call3(Func_0200359c, 15, 0x3000, 0);
    Func_020034cc(20);
    *(s32 *)((0x40c + work)) = zero;
    Call3(Func_02003494, 0x20000, 0x20000, 0x10000);
    Call2(Func_020035ec, 0x20119e, 1);
    Func_020035f4(20);
    Func_020034cc(20);
    Func_02002ff0();
    Func_02003544(15, 2);
    step = 0;
    do {
        Func_02002d94(actor15);
        step = (step + 1);
        Func_020033e4(1);
    } while ((u32)step <= 39);
    Value2(Func_020033ec, (s32)Func_0200300c, 0xc80);
    Call2(Func_020035ec, 0x10000, 1);
    Func_020035f4(60);
    Func_020034cc(30);
    Call1(Func_02003624, 0x121);
    Value1(Func_020033f4, (s32)Func_0200293c);
    Call3(Func_02003494, 0x10000, 0x10000, 0x10000);
    delay = 15;
    do {
        Func_0200284c(0);
        Func_020033e4(delay);
        Func_0200284c(1);
        Func_020033e4(delay);
        delay = (delay - 1);
    } while (delay != -1);
    Func_0200284c(0);
    *(s32 *)((0x40c + work)) = 1;
    Call3(Func_02003494, delay, delay, 0xe666);
    Func_0200349c();
    Func_02003544(15, 3);
    Value1(Func_020033f4, (s32)Func_0200300c);
    Func_020033e4(1);
    Func_0200356c(15, 0);
    Func_020034cc(60);
    Func_0200358c(15, 0);
    Func_020034cc(40);
    Call3(Func_020035ac, 0, 0x102, 0);
    Call3(Func_020035ac, 1, 0x102, 0);
    Func_020034cc(60);
    Call2(Func_020035bc, 0x10000, 0x2000);
    Call4(Func_020035c4, 0xda0000, delay, 0xb50000, 1);
    Call3(Func_020034fc, 15, 0x10000, 0x8000);
    Func_0200350c(15, 169, 151);
    Func_0200352c(15);
    Func_020034cc(10);
    Call3(Func_0200359c, 15, 0x1000, 0);
    Func_020034cc(40);
    Func_0200358c(15, 0);
    Func_020034cc(20);
    Call3(Func_0200359c, 0, 0x5000, 0);
    Call3(Func_0200359c, 1, 0xd000, 0);
    Func_020034cc(40);
    Call3(Func_0200359c, 15, 0xf000, 0);
    Func_020034cc(10);
    Call3(Func_0200359c, 0, 0x7000, 0);
    Call3(Func_0200359c, 1, 0xb000, 0);
    Call4(Func_020035c4, 0xe00000, delay, 0x9e0000, 1);
    Func_020035cc();
    Func_020029a4(0);
    Func_0200355c(15, 2);
    Func_020034cc(20);
    Func_0200358c(15, 0);
    Func_020034cc(40);
    Value2(Func_02003584, 15, 0);
    if (Value2(Func_020034e4, 0, 0) == 0) {
        Func_020034cc(40);
    } else {
        Call2(Func_020034ac, 0x110c, 1);
        Func_020034cc(40);
    }
    record = Func_020034ec(0);
    effect = Pointer4(Func_02003454, 22, *(s32 *)((s32)record + 8), (*(s32 *)((s32)record + 12) + 0x240000), *(s32 *)((s32)record + 16));
    if (effect != 0) {
        textureBuffer = Pointer2(Func_02003414, 17, 0x608);
        effectSprite = *(u8 **)(effect + 80);
        effectSprite[38] = 0;
        *(u8 *)(((s32)effectSprite + 38) + 1) = 0;
        effectSprite[5] &= -33;
        effectSprite[9] = ((s32)((s32)(15 & effectSprite[9]) & -13) | 4);
        Func_020034b4(222);
        Func_02003434(effectSprite[28], 128, (textureBuffer + 0x400));
        Func_02003424(17);
        Func_0200353c(0, 28);
        Func_0200360c(effect, 3);
        Func_0200353c(0, 28);
    }
    scale = 128;
    Func_02002d48(1, 20, 0);
    step = 0;
    do {
        Func_02002d94(actor15);
        Func_020033e4(1);
        Func_02002d94(actor15);
        Func_020033e4(1);
        *(s32 *)(effect + 24) = 0x6666;
        *(s32 *)(effect + 28) = 0x6666;
        Func_02002d94(actor15);
        Func_020033e4(1);
        Func_02002d94(actor15);
        step = (step + 1);
        Func_020033e4(1);
        *(s32 *)(effect + 24) = (scale << 9);
        *(s32 *)(effect + 28) = (scale << 9);
    } while ((u32)step <= 23);
    Func_0200356c(15, 0);
    Func_02002d48(0, 20, 0);
    Call1(Func_0200357c, 0x110d);
    Func_02003594(15, 0, 20);
    if (effect != 0) {
        Func_0200345c(effect);
    }
    Func_0200353c(0, 1);
    Func_020034cc(20);
    Call3(Func_0200359c, 0, 0x8000, 0);
    Call3(Func_0200359c, 1, 0xa000, 60);
    Func_0200358c(15, 0);
    Func_020034cc(40);
    Call3(Func_0200359c, 15, 0x4000, 0);
    Call2(Func_020035bc, 0x40000, 0x8000);
    Func_02002c1c(232, 0x1d0);
    tileX = 0x2c7;
    Func_020029a4(1);
    Func_0200358c(15, 0);
    Func_02002c1c(tileX, 144);
    Func_020029a4(2);
    Func_0200358c(15, 0);
    Func_02002c1c(tileX, 0x1d0);
    Func_020029a4(3);
    (Func_0200358c)(15, 0);
    Call3(Func_0200359c, 15, 0x1000, 0);
    Call3(Func_02003534, 1, 0x2460000, 0x1590000);
    Func_020034cc(20);
    Func_0200358c(1, 0);
    Call3(Func_02003534, 1, 0xe70000, 0xb40000);
    Call3(Func_0200359c, 1, 0xb000, 0);
    Func_020033e4(20);
    Func_02002c1c(219, 171);
    Func_0200358c(15, 0);
    Func_020034cc(10);
    Func_02003554(0, 2);
    Func_0200355c(1, 2);
    Call2(Func_0200358c, 15, 0);
    Func_020034cc(20);
    Func_0200358c(15, 0);
    Func_020034cc(40);
    *(s32 *)((0x40c + work)) = 0;
    Call3(Func_02003494, 0x40000, 0x40000, 0x10000);
    Call2(Func_020035ec, 0x20119e, 1);
    Func_020035f4(20);
    Func_020034cc(20);
    Func_02003624(107);
    Value2(Func_020033ec, (s32)Func_02002970, 0xc80);
    Func_020034cc(20);
    Call2(Func_020035bc, 0x10000, 0x2000);
    Call4(Func_020035c4, 0xb80000, -1, 0x840000, 1);
    Call3(Func_0200354c, 0, 6, 0);
    flags = 254;
    Func_0200354c(1, 6, 0);
    *(u8 *)(Func_020034ec(0) + 90) &= flags;
    *(u8 *)(Func_020034ec(1) + 90) &= flags;
    Func_0200350c(0, 245, 145);
    Func_0200350c(1, 215, 168);
    Func_0200352c(1);
    flags = 1;
    Func_020034cc(80);
    *(u8 *)(Func_020034ec(0) + 90) |= flags;
    {
        u8 *record = Func_020034ec(1);
        u8 value = *(u8 *)&record[90];

        record[90] = (u8)(value | flags);
    }
    Func_0200350c(15, 184, 87);
    Func_0200352c(15);
    Call3(Func_0200359c, 15, 0x4000, 0);
    Func_020034cc(20);
    Func_02003544(15, 2);
    step = 0;
    do {
        Func_02002d94(actor15);
        step = (step + 1);
        Func_020033e4(1);
    } while ((u32)step <= 39);
    Value2(Func_020033ec, (s32)Func_0200300c, 0xc80);
    Call2(Func_020035ec, 0x10000, 1);
    Func_020035f4(60);
    Func_020034cc(30);
    Call1(Func_02003624, 0x121);
    Value1(Func_020033f4, (s32)Func_02002970);
    Call3(Func_02003494, 0x20000, 0x20000, 0x10000);
    shortDelay = 7;
    do {
        Func_020028dc(0);
        Func_020033e4(shortDelay);
        Func_020028dc(1);
        Func_020033e4(shortDelay);
        shortDelay = (shortDelay - 1);
    } while (shortDelay != -1);
    Func_020028dc(0);
    Call3(Func_02003494, 0x10000, 0x10000, 0x10000);
    Func_02003544(15, 3);
    Value1(Func_020033f4, (s32)Func_0200300c);
    Func_020033e4(1);
    Func_0200356c(15, 0);
    Func_020034cc(60);
    Call3(Func_02003494, 0x40000, 0x40000, 0x10000);
    Call2(Func_020035ec, 0x20119e, 1);
    Func_020035f4(20);
    Func_020034cc(20);
    Func_02003624(107);
    Value2(Func_020033ec, (s32)Func_0200293c, 0xc80);
    Func_020034cc(40);
    Func_02003514(15, 127, 110);
    Call3(Func_0200359c, 15, 0x4000, 0);
    Func_020034cc(20);
    Func_02003544(15, 2);
    step = 0;
    do {
        Func_02002d94(actor15);
        step = (step + 1);
        Func_020033e4(1);
    } while ((u32)step <= 39);
    Value2(Func_020033ec, (s32)Func_0200300c, 0xc80);
    Call2(Func_020035ec, 0x10000, 1);
    Func_020035f4(60);
    Call1(Func_02003624, 0x121);
    Func_020034cc(30);
    Value1(Func_020033f4, (s32)Func_0200293c);
    Call3(Func_02003494, 0x20000, 0x20000, 0x10000);
    shortDelay = 7;
    do {
        Func_0200284c(0);
        Func_020033e4(shortDelay);
        Func_0200284c(1);
        Func_020033e4(shortDelay);
        shortDelay = (shortDelay - 1);
    } while (shortDelay != -1);
    Func_0200284c(0);
    Call3(Func_02003494, 0x10000, 0x10000, 0x10000);
    Func_02003624(107);
    Func_02003624(63);
    Call3(Func_02003494, 0x40000, 0x40000, 0x10000);
    Call2(Func_020035ec, 0x20119e, 1);
    Func_020035f4(20);
    Func_020034cc(20);
    Func_02003624(107);
    Value2(Func_020033ec, (s32)Func_02002970, 0xc80);
    Func_02003544(15, 3);
    actorStepCallback = (s32)Func_0200300c;
    Func_020033f4(actorStepCallback);
    Func_020033e4(1);
    Func_0200356c(15, 0);
    Func_020034cc(60);
    Func_02003514(15, 184, 87);
    Call3(Func_0200359c, 15, 0x4000, 0);
    Func_020034cc(10);
    Func_02003544(15, 3);
    Func_020033f4(actorStepCallback);
    Func_020033e4(1);
    Func_0200356c(15, 0);
    Func_02003624(141);
    Func_020034cc(100);
    Call3(Func_020035ac, 0, 0x102, 0);
    Call3(Func_020035ac, 1, 0x102, 0);
    Func_020034cc(60);
    Func_0200358c(15, 0);
    Func_0200355c(1, 3);
    Func_0200358c(1, 0);
    Func_020034cc(20);
    Call1(Func_02003624, 0x121);
    Call3(Func_0200359c, 15, 0x3000, 0);
    Func_020034cc(20);
    Func_0200358c(15, 0);
    Func_020034cc(20);
    step = 0;
    do {
        Func_02002d94(actor15);
        step = (step + 1);
        Func_020033e4(1);
    } while ((u32)step <= 39);
    actorStepCallback = (s32)Func_0200300c;
    Value2(Func_020033ec, actorStepCallback, 0xc80);
    Func_020034cc(20);
    Call2(Func_020035ec, 0x7fff, 2);
    Func_020035f4(60);
    Func_020033e4(100);
    Call2(Func_020035ec, 0x7fff, 1);
    Func_020035f4(60);
    Func_020033e4(60);
    Func_020033f4(actorStepCallback);
    *(s32 *)((0x40c + work)) = 1;
    Call3(Func_02003494, -1, -1, 0xe666);
    Func_0200349c();
    Func_02003000();
    Value1(Func_020034c4, 0x814);
    Value1(Func_020034c4, 0x83f);
    Func_020035dc(5);
    Value1(Func_020034c4, 0x100);
}
