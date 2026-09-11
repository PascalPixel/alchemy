#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_EFFECT_TABLE_AUDIO
#define ALCHEMY_ABI_OVERLAYS_SCENE_EFFECT_TABLE_AUDIO

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_020014c2(s32 cue);
void Func_020011fc(void);
void Func_02001172(s32, s32, s32, s32, s32, s32, s32);
void Func_020024ba(void *);
void Func_020024c0(void *);

#define Audio_PlayCue Func_020014c2
#define Effect_Do Func_020024ba
#define Effect_Do2 Func_020024c0
#define Effect_LoadTablesAndStopDma0 Func_02001188
#define Effect_Run Func_0200179c
#define Effect_Run2 Func_020017b8
#define Effect_Run3 Func_020017d4
#define Effect_Run4 Func_020017f0
#define Effect_Run5 Func_020011fc
#define Effect_SetRect Func_02001172
#define PlayWorkspaceCueAndClearPaletteZero Func_0200004c
#define SceneData_GetTable96c8 Func_02000030
#define SceneData_GetTable9710 Func_0200003c
#define SceneData_GetTable971c Func_02000044
#define SceneData_GetTable97AC Func_020000a8
#define SceneData_StoreRecord1Field12 Func_020003dc
#define SceneData_StoreRecord2Field12 Func_02000414
#define Scene_ConfigureFixedPointValues Func_0200007c
#define Scene_Forward Func_02000070
#define State_CountDownEveryFortyTicks Func_02000168
#define State_StoreSlotThreeField12 Func_020003f8
#define State_StoreSlotZeroField12 Func_020003c0
#define gOv Data_020097f4
#define gOv2 Data_020097f0
#define gOv3 Data_0200980c
#define gOv4 Data_02009810
#define gOv5 Data_02009814
#define gOv6 Data_02009818
#define gOv7 Data_02008f31
#define gOv8 Data_02008f81
#define gWork Data_03001ebc

#endif
