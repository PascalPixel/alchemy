#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_EFFECT_ORBITING
#define ALCHEMY_ABI_OVERLAYS_SCENE_EFFECT_ORBITING

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_02000aae(s32, s32);
s32 Func_02000b68(s32);
s32 Func_02000b7a(s32);
void Func_02000bd0(void);
s32 Func_02000c2a(s32, s32, s32);
void Func_02000bc8(s32);
void Func_02000cd8(s32, s32);
s32 Func_02000c00(s32, s32);
void Func_02000bfc(void);
void Func_02000c04(void);
void Func_02000bdc(s32, s32);
void Func_02000be4(s32, s32);
void Func_02000c20(void);
void Func_02000c5c(void);
void Func_02000ce2(s32);
s32 Func_02000d0a(s32, s32);
void Func_02000c76(void);
void Func_02000c7c(void);
void Func_02000d02(s32);
s32 Func_02000d2a(s32, s32);
void Func_02000c96(void);
void Func_02000c9c(void);
void Func_02000d22(s32);
s32 Func_02000d4a(s32, s32);
void Func_02000cb6(void);
void Func_02000d58();
s32 Func_02000f1c();
void Func_02000f34();
void Func_02000f72();
void Func_02000f7a();
void Func_02000f8c();
void Func_02000fb4();
void Func_02000fc0();
void Func_02001050();
void Func_02000a00(void);
void Func_020014ba(s32);
void Func_0200138c(s32, s32, s32);
void Func_020013bc(s32);
s32 Func_0200137a(s32);
s32 Func_02001390(s32);
s32 Func_020013aa(s32);
s32 Func_020013a6(void);
void Func_0200144e(OrbitingSceneObject *, s32);
s32 Func_0200147c(s32);
void Func_020014ac(s32);
s32 Func_02001494(u8, s32, u8 *);
void Func_02001492(s32);

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

#define Actor_UpdateFacingTowardTarget Func_02000030
#define AllocateEffectTransfer Func_0200146c
#define CalculateFacingAngle Func_02000aae
#define ConfigurePaletteTransfer Func_02001494
#define Dialogue_RunActor10Message13c3 Func_020001a4
#define Dialogue_RunActor11Message1751 Func_020001c4
#define Dialogue_RunActor9Message13c0 Func_02000184
#define Effect_Apply Func_02000cd8
#define Effect_Apply2 Func_02000c00
#define Effect_Apply3 Func_02000bdc
#define Effect_Apply4 Func_02000be4
#define Effect_Apply5 Func_02000d0a
#define Effect_Apply6 Func_02000d2a
#define Effect_Apply7 Func_02000d4a
#define Effect_Check2 Func_02000b68
#define Effect_Check3 Func_02000b7a
#define Effect_Check4 Func_0200137a
#define Effect_Check5 Func_02001390
#define Effect_Check6 Func_020013aa
#define Effect_Check7 Func_020013a6
#define Effect_Do Func_02000bc8
#define Effect_Do2 Func_02000ce2
#define Effect_Do3 Func_02000d02
#define Effect_Do4 Func_02000d22
#define Effect_Do5 Func_020014ba
#define Effect_Do6 Func_020013bc
#define Effect_Place3 Func_02000c2a
#define Effect_Place4 Func_0200138c
#define Effect_Run Func_020013a0
#define Effect_Run10 Func_02000c96
#define Effect_Run11 Func_02000c9c
#define Effect_Run12 Func_02000cb6
#define Effect_Run13 Func_02000f34
#define Effect_Run14 Func_02000d58
#define Effect_Run15 Func_02000fc0
#define Effect_Run16 Func_02000f72
#define Effect_Run17 Func_02001050
#define Effect_Run18 Func_02000f8c
#define Effect_Run19 Func_02000a00
#define Effect_Run2 Func_02000fc4
#define Effect_Run3 Func_02000bd0
#define Effect_Run4 Func_02000bfc
#define Effect_Run5 Func_02000c04
#define Effect_Run6 Func_02000c20
#define Effect_Run7 Func_02000c5c
#define Effect_Run8 Func_02000c76
#define Effect_Run9 Func_02000c7c
#define Effect_UpdateLobeOrbitEntity Func_0200090c
#define GetOrbitingSceneObject Func_02001474
#define InitializeOrbitingSceneEntity Func_02000974
#define IsGameFlagSet Func_0200147c
#define LoadEffectResource Func_020014ac
#define NormalizeOrbitingSceneObject Func_0200144e
#define ReleaseEffectTransfer Func_02001492
#define SceneData_GetTable8C50 Func_02000094
#define SceneData_GetTable8c08 Func_02000088
#define SceneData_SelectRecordByScene22 Func_0200009c
#define SceneData_SelectTable8d24ByState Func_02000154
#define SceneEntity Func_0200090c
#define SceneHandle Func_0200090c
#define Scene_RunEntryZeroAndScene22Body Func_020004bc
#define Scene_RunScene38e Func_0200045c
#define Scene_RunStepWithValue29de Func_0200012c
#define Scene_RunStepWithValueFd2 Func_020000f8
#define Scene_SetupWithDescriptor8BD4 Func_020008e8
#define Value Func_0200890d
#define gCell Data_02000240
#define gOv Data_02008c7c
#define gOv2 Data_02008c64
#define gOv3 Data_02008d30
#define gOv4 Data_02008d24
#define gWork Data_03001ebc
#define Effect_Check(a0) Value1(Func_02000f1c, a0)
#define Effect_Place(a0, a1, a2) Call3(Func_02000f7a, a0, a1, a2)
#define Effect_Place2(a0, a1, a2) Call3(Func_02000fb4, a0, a1, a2)

#endif
