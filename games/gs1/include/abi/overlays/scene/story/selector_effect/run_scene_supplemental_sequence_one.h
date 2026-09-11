#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STORY_SELECTOR_EFFECT_RUN_SCENE_SUPPLEMENTAL_SEQUENCE_ONE
#define ALCHEMY_ABI_OVERLAYS_SCENE_STORY_SELECTOR_EFFECT_RUN_SCENE_SUPPLEMENTAL_SEQUENCE_ONE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02001998();
void Func_02001aa6();
s32 Func_02002122();
void Func_020022e0();
void Func_020023fc();
void Func_02002424();
void Func_0200248c();
void Func_02002e8e();
void Func_02002ea4();
void Func_0200326a();
void Func_02003316();
void Func_02003324();
void Func_02005264();
void Func_020053a2();
s32 Func_020071a6();
s32 Func_020071be();
void Func_020071ce();
void Func_020071de();
s32 Func_02007258();
void Func_020072a4();
s32 Func_020072ce();
void Func_02007310();
void Func_02007316();
void Func_0200731c();
void Func_02007326();
void Func_02007366();
void Func_02007394();
void Func_0200741c();
void Func_0200741e();
void Func_02007430();
void Func_02007442();
void Func_02007452();
void Func_02007462();
void Func_02007472();
void Func_02007476();
void Func_02007490();
void Func_020074a0();
void Func_020074a6();
void Func_020074b0();
void Func_020074c0();
void Func_020074d0();
void Func_020074e0();
void Func_020074f0();
void Func_02007500();
void Func_0200752e();
void Func_020075aa();
s32 Func_020075c2();
void Func_020075f4();
void Func_0200760e();
void Func_02007620();
void Func_02007640();
void Func_02007664();
void Func_0200766c();
void Func_020076fa();
void Func_02007712();
void Func_0200771e();

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

#define Story_Check2 Func_020071a6
#define Story_Check3 Func_020071be
#define Story_Run Func_02001998
#define Story_Run2 Func_02007310
#define Story_Run3 Func_020053a2
#define Story_Run4 Func_02002e8e
#define Story_Run5 Func_02007366
#define Story_Run6 Func_02002ea4
#define Story_Run7 Func_0200752e
#define Story_Run8 Func_0200731c
#define gWork Data_03001ebc
#define Story_SetRect(a0, a1, a2, a3, a4, a5) Call6(Func_020071ce, a0, a1, a2, a3, a4, a5)
#define Story_SetRect2(a0, a1, a2, a3, a4, a5) Call6(Func_020071de, a0, a1, a2, a3, a4, a5)
#define Story_SetRect3(a0, a1, a2, a3, a4, a5) Call6(Func_020072a4, a0, a1, a2, a3, a4, a5)
#define Story_Check(a0, a1) Value2(Func_02007258, a0, a1)

#endif
