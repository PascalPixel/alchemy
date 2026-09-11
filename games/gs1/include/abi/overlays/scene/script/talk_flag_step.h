#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_39A_02001B1C
#define ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_39A_02001B1C

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_02003de6();
s32 Func_02003df4();
void Func_02003e1a();
s32 Func_02003e1c();
void Func_02003e7c();

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

#define Scene_RunTalk Func_02001b1c
#define Script_Run Func_02003e7c
#define Script_Run2 Func_02003e1a
#define gOv Data_0200a488
#define Script_Check(a0, a1, a2, a3) Value4(Func_02003de6, a0, a1, a2, a3)
#define Script_Check2(a0, a1) Value2(Func_02003df4, a0, a1)
#define Script_Place(a0, a1, a2) Value3(Func_02003e1c, a0, a1, a2)

#endif
