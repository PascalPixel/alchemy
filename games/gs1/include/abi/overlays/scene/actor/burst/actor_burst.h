#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_BURST_ACTOR_BURST
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_BURST_ACTOR_BURST

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0200076c(void *, s32);
void Func_02000784(void *, s32);
void Func_020007dc(void *, s32);
void Func_020007c8(void *, s32);
void Func_02000838(void *, s32);
void Func_02000892();
void Func_02000aa6();
s32 Func_02000aac();
void Func_02000aba();
void Func_02000ae8();
void Func_02000b5e();
void Func_02000b64();
s32 Func_02000b66();
void Func_02000b88();
s32 Func_02000ba2();
void Func_02000bb0();
void Func_02000bdc();
void Func_02000b98(s32 frames);
void Func_02000bce(s32);
s32 Func_02000bf0(s32);
s32 Func_02000c8c(s32);
void Func_02000c9c();
void Func_02000cba(s32);
void Func_02000a5a(union SceneActor *, s32);
void Func_02000ce8(s32);
void Func_02000c96();
void Func_02000cfc(s32);
void Func_02000caa();
void Func_02000740();
void Func_02000d56();
void Func_02000d64(s32);
void Func_02000b04(union SceneActor *, s32);
void Func_020007ac();
void Func_02000dd2(s32);
void Func_02000dca(s32, s32);
void Func_02000d86(void);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];

    f(a0, a1, a2);
}

#define AcquireOverlayObject Func_02000736
#define Actor_Apply Func_02000a5a
#define Actor_Apply2 Func_02000b04
#define Actor_Apply3 Func_02000dca
#define Actor_Check3 Func_02000ba2
#define Actor_Check4 Func_02000bf0
#define Actor_Check5 Func_02000c8c
#define Actor_Do3 Func_02000b98
#define Actor_Do4 Func_02000bce
#define Actor_Do5 Func_02000cba
#define Actor_Do6 Func_02000ce8
#define Actor_Do7 Func_02000cfc
#define Actor_Do8 Func_02000d64
#define Actor_Do9 Func_02000dd2
#define Actor_Run Func_02000ac8
#define Actor_Run10 Func_02000740
#define Actor_Run11 Func_020007ac
#define Actor_Run12 Func_02000d86
#define Actor_Run2 Func_02000aec
#define Actor_Run3 Func_02000b6e
#define Actor_Run4 Func_02000b96
#define Actor_Run5 Func_02000892
#define Actor_Run6 Func_02000aba
#define Actor_Run7 Func_02000b64
#define Actor_Run8 Func_02000bdc
#define Actor_Run9 Func_02000c2c
#define CreateOverlayObject Func_0200078e
#define EffectDescriptorTable Data_0200880c
#define EffectRecord Func_020004c4
#define EffectWork Func_020004c4
#define Effect_RunActorBurst Func_020004f4
#define Effect_UpdateMotion Func_02000104
#define GetPartyEffect Func_02000882
#define OvObj_PrepareObject Func_02000048
#define RunOverlayObjectCommand0 Func_0200076c
#define RunOverlayObjectCommand1 Func_02000784
#define RunOverlayObjectCommand14 Func_020007dc
#define ScaleEffectDeltaFromAccumulated Func_02000928
#define ScaleEffectDeltaFromOrigin Func_02000940
#define ScaleEffectVerticalDelta Func_0200094e
#define SetEffectCallbackArgument Func_02000994
#define SetEffectCallbackMode Func_02000984
#define SetEffectDescriptor Func_02000884
#define SetEffectMode Func_0200096e
#define SetEffectVariant Func_0200086a
#define SetOverlayObjectMode Func_020007c8
#define SetOverlayObjectSlot Func_02000838
#define SpawnEffect Func_02000860
#define gCell Data_02000240
#define gOv Data_020088d8
#define gOv2 Data_02008818
#define gOv3 Data_02008920
#define gOv4 Data_02008978
#define gOv5 Data_02008948
#define gOv6 Data_020089f0
#define gOv7 Data_02008990
#define gVal Data_0000001c
#define gWork Data_03001ebc
#define Actor_Check(a0) Value1(Func_02000aac, a0)
#define Actor_SetRect(a0, a1, a2, a3, a4, a5) Call6(Func_02000aa6, a0, a1, a2, a3, a4, a5)
#define Actor_Do(a0) Call1(Func_02000ae8, a0)
#define Actor_Do2(a0) Call1(Func_02000b5e, a0)
#define Actor_Check2(a0) Value1(Func_02000b66, a0)
#define Actor_Place(a0, a1, a2) Call3(Func_02000bb0, a0, a1, a2)
#define Actor_SetRect2(a0, a1, a2, a3, a4, a5) Call6(Func_02000b88, a0, a1, a2, a3, a4, a5)
#define Actor_Place2(a0, a1, a2) Call3(Actor_SetSpeed, a0, a1, a2)
#define Actor_Place3(a0, a1, a2) Call3(Func_02000c9c, a0, a1, a2)
#define Actor_Place4(a0, a1, a2) Call3(Func_02000c96, a0, a1, a2)
#define Actor_Place5(a0, a1, a2) Call3(Func_02000caa, a0, a1, a2)
#define Actor_Place6(a0, a1, a2) Call3(Func_02000d56, a0, a1, a2)

#endif
