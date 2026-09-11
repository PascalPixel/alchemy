#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_ARRIVAL
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_ARRIVAL

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02000558(void);
void Func_020005a6(s32);
s32 Func_020005b6(s32, s32);
void Func_0200055e(s32);
void Func_0200057a(void);
void Func_02000580(void);
void Func_020005ce(s32);
s32 Func_020005e6(s32, s32);
void Func_0200059a(void);
void Func_02000638(s32, s32);
void Func_020005be(void);
s32 Func_020005ac(s32);
void Func_02000616(s32);
void Func_02000626(s32, s32);
void Func_02000626_b(s32);
void Func_0200063e(s32, s32);
void Func_020005de(s32);
void Func_020005fa(void);
void Func_0200069e(int);
void Func_0200062c(void);
void Func_0200068a(int, int);
void Func_02000646(void);
void Func_02000658();
void Func_02000710();
void Func_020006e6_a(s32);
void Func_020006f0();
void Func_02000718();
void Func_02000714();
void Func_0200075c();
void Func_02000768();
void Func_02000774();
void Func_02000692(u32, u32, u32);
void Func_02000730();
void Func_0200065e(void);
void Func_020007b4();
void Func_020007ba();
void Func_020007c0();
void Func_020007c6();
void Func_020007cc();
void Func_020007d2();
void Func_020007d8();
void Func_020007de();
void Func_020007e4();
void Func_02000800();
void Func_02000806();
void Func_0200080e();
void Func_02000762(s32, s32, s32, s32, s32, s32);
s32 Func_02000772();
s32 Func_020007be();
void Func_020004d0(void *);

#define Actor_Apply Func_020005b6
#define Actor_Apply2 Func_020005e6
#define Actor_Apply3 Func_02000626
#define Actor_Apply4 Func_02000638
#define Actor_Apply5 Func_0200063e
#define Actor_Apply6 Func_0200068a
#define Actor_Check Func_020005ac
#define Actor_Check2 Func_02000772
#define Actor_Check3 Func_020007be
#define Actor_Do Func_020004d0
#define Actor_Do2 Func_020005a6
#define Actor_Do3 Func_0200055e
#define Actor_Do4 Func_020005ce
#define Actor_Do5 Func_02000626_b
#define Actor_Do6 Func_02000616
#define Actor_Do7 Func_020005de
#define Actor_Do8 Func_0200069e
#define Actor_Do9 Func_020006e6_a
#define Actor_Place Func_02000692
#define Actor_Run Func_020005ba
#define Actor_Run10 Func_0200067a
#define Actor_Run11 Func_0200062c
#define Actor_Run12 Func_02000646
#define Actor_Run13 Func_0200067a_a
#define Actor_Run14 Func_02000658
#define Actor_Run15 Func_0200075c
#define Actor_Run16 Func_02000730
#define Actor_Run17 Func_020006f0
#define Actor_Run18 Func_02000714
#define Actor_Run19 Func_02000718
#define Actor_Run2 Func_0200062a
#define Actor_Run20 Func_02000768
#define Actor_Run21 Func_02000774
#define Actor_Run22 Func_02000710
#define Actor_Run23 Func_0200065e
#define Actor_Run24 Func_020007b4
#define Actor_Run25 Func_020007ba
#define Actor_Run26 Func_020007c0
#define Actor_Run27 Func_020007c6
#define Actor_Run28 Func_020007cc
#define Actor_Run29 Func_020007d2
#define Actor_Run3 Func_020006e6
#define Actor_Run30 Func_020007d8
#define Actor_Run31 Func_020007de
#define Actor_Run32 Func_020007e4
#define Actor_Run33 Func_0200080e
#define Actor_Run34 Func_02000800
#define Actor_Run35 Func_02000806
#define Actor_Run4 Func_02000558
#define Actor_Run5 Func_0200057a
#define Actor_Run6 Func_02000580
#define Actor_Run7 Func_0200059a
#define Actor_Run8 Func_020005be
#define Actor_Run9 Func_020005fa
#define Actor_SetRect Func_02000762
#define Dialogue_RunActor12DialogueAndSetFlag910 Func_02000144
#define Dialogue_RunActor16Dialogue Func_0200016c
#define Dialogue_RunActor8FacingDialogue Func_020001fc
#define Dialogue_RunActor8FlaggedDialogue Func_0200018c
#define SceneData_GetTable8728 Func_02000074
#define SceneData_ReturnZero Func_02000070
#define SceneData_SelectAndPrepareTable Func_0200007c
#define SceneData_SelectSubStateTable Func_020000e4
#define SceneData_SelectTableBySceneId Func_02000030
#define Scene_RunArrivalPlacement Func_02000240
#define State_ClearSlotsBySubState Func_0200033c
#define State_SetRuntimeWord448To521 Func_02000308
#define gCell Data_02000240
#define gOv Data_020084d0
#define gOv10 Data_02008a48
#define gOv11 Data_02008eb0
#define gOv12 Data_02008a3c
#define gOv2 Data_020086c8
#define gOv3 Data_020084a0
#define gOv4 Data_02008ef8
#define gOv5 Data_020088d4
#define gOv6 Data_0200879c
#define gOv7 Data_02008a0c
#define gOv8 Data_02008784
#define gOv9 Data_02008c88
#define gWork Data_03001ebc

#endif
