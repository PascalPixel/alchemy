#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_DATA_RECORD_COUNT
#define ALCHEMY_ABI_OVERLAYS_SCENE_DATA_RECORD_COUNT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_020008f2();
void Func_0200090e();
void Func_0200090e_a(s32, s32);
s32 Func_0200092a(s32, s32, s32);
void Func_02000920(s32);
void Func_02000932(s32, s32);
void Func_02000910(s32);
s32 Func_02000902();
void Func_02000922();
void Func_0200093e();
void Func_02000978();
void Func_02000978_a();
void Func_02000964();
void Func_0200099c();
void Func_02000250();
void Func_02000a5e();
void Func_02000268();
void Func_02000a76();
void Func_02000a88();
void Func_020009f6();
void Func_020009b4();
void Func_020009cc();
void Func_02000a32();
void Func_02000a38();
void Func_02000a3e();
void Func_02000a44();
void Func_020009e6();
void Func_02000fca(void);
void Func_02000fec(s32 id, s32 arg1);
s32 Func_02000f6a(s32, s32, s32, s32, s32);
void Func_02000fa0(const void *, s32, s32, s32);
void Func_02000fac(const void *, s32, s32, s32);
void Func_02000fb8(const void *, s32, s32, s32);
int Func_0200100c(void);
int Func_02001020(void);
s32 Func_02000942(u16 *);
void Func_0200017e(u16, s32);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

#define SceneData_Apply Func_0200090e_a
#define SceneData_Apply2 Func_02000932
#define SceneData_Apply3 Func_0200017e
#define SceneData_Apply4 Func_02000fec
#define SceneData_Check Func_02000942
#define SceneData_Check2 Func_02000902
#define SceneData_Do2 Func_02000920
#define SceneData_Do3 Func_02000910
#define SceneData_GetTable8964 Func_02000030
#define SceneData_GetTable8994 Func_0200003c
#define SceneData_GetTable8998 Func_02000044
#define SceneData_GetTable8a58 Func_02000098
#define SceneData_Place2 Func_0200092a
#define SceneData_ReturnZero Func_02000038
#define SceneData_Run Func_020008f8
#define SceneData_Run10 Func_02000978_a
#define SceneData_Run11 Func_0200099c
#define SceneData_Run12 Func_02000250
#define SceneData_Run13 Func_02000a5e
#define SceneData_Run14 Func_02000268
#define SceneData_Run15 Func_02000a76
#define SceneData_Run16 Func_02000a88
#define SceneData_Run17 Func_020009f6
#define SceneData_Run18 Func_020009b4
#define SceneData_Run19 Func_020009cc
#define SceneData_Run2 Func_02000950
#define SceneData_Run20 Func_02000a32
#define SceneData_Run21 Func_02000a38
#define SceneData_Run22 Func_02000a3e
#define SceneData_Run23 Func_02000a44
#define SceneData_Run24 Func_020009e6
#define SceneData_Run25 Func_02000fca
#define SceneData_Run26 Func_02000fc8
#define SceneData_Run27 Func_02000fe4
#define SceneData_Run28 Func_0200100c
#define SceneData_Run29 Func_02001020
#define SceneData_Run3 Func_02000fd2
#define SceneData_Run4 Func_02000930
#define SceneData_Run5 Func_02000930_a
#define SceneData_Run6 Func_02000922
#define SceneData_Run7 Func_0200093e
#define SceneData_Run8 Func_02000978
#define SceneData_Run9 Func_02000964
#define SceneData_SetMode Func_02000fa0
#define SceneData_SetMode2 Func_02000fac
#define SceneData_SetMode3 Func_02000fb8
#define SceneData_SetRange Func_02000f6a
#define Scene_AddToListedRecordCounts Func_020000c0
#define Scene_DrawThreeCaptionWindow Func_02000768
#define Scene_RunActor13Mode102Step Func_0200004c
#define Scene_RunActor13Mode105Step Func_02000070
#define Scene_RunCountAdjustPanel Func_020000ec
#define Scene_RunEntrySetup Func_0200071c
#define State_AddToRecordCount Func_020000a0
#define State_GetFarResult100c Func_020007c8
#define State_GetFarResult1020 Func_020007d4
#define State_SetRecordFlag53 Func_020007b8
#define State_SetWorkWords1c0And1c8 Func_020006f8
#define gCell Data_02000240
#define gOv Data_02008a58
#define gOv2 Data_020088d0
#define gOv3 Data_02008920
#define gOv4 Data_0200893c
#define gOv5 Data_02008958
#define gWork Data_03001ebc
#define SceneData_Do(a0) Call1(Func_020008f2, a0)
#define SceneData_Place(a0, a1, a2) Call3(Func_0200090e, a0, a1, a2)

#endif
