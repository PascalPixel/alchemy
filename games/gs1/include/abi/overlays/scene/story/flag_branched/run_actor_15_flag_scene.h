#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STORY_FLAG_BRANCHED_RUN_ACTOR_15_FLAG_SCENE
#define ALCHEMY_ABI_OVERLAYS_SCENE_STORY_FLAG_BRANCHED_RUN_ACTOR_15_FLAG_SCENE

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_020026ec();
void Func_02002700();
void Func_0200270c();
void Func_02002730();
void Func_020027ce();
void Func_020027de();
void Func_020027f6();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

#define Story_Run Func_02002700
#define Story_Run2 Func_020027f6
#define Story_Run3 Func_02002730
#define gWork Data_03001ebc
#define Story_Do(a0) Call1(Func_020027ce, a0)
#define Story_Check(a0) Value1(Func_020026ec, a0)
#define Story_Do2(a0) Call1(Func_020027de, a0)
#define Story_Do3(a0) Call1(Func_0200270c, a0)

#endif
