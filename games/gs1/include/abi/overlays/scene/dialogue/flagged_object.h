#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_DIALOGUE_FLAGGED_OBJECT
#define ALCHEMY_ABI_OVERLAYS_SCENE_DIALOGUE_FLAGGED_OBJECT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_020006b4(void *, s32);
void Func_020006cc(void *, s32);
void Func_02000714(void *, s32);
void Func_02000710(void *, s32);
void Func_02000770(void *, s32);
void Func_020007ba(struct Effect *effect, s32 mode);
void Func_020007d4(struct Effect *effect, struct Descriptor *desc);
void Func_020008a6(struct Effect *effect, s32 mode);
s32 Func_02000890(s32 delta, s32 dur);
s32 Func_020008a8(s32 delta, s32 dur);
s32 Func_020008b6(s32 delta, s32 dur);
void Func_020008d4(struct Effect *effect, s32 mode);
void Func_020008e4(struct Effect *effect, s32 callback_arg);
void Func_020009b0(void *);
void Func_020009c4(void);
void Func_02000a2e(s32, s32);
s32 Func_020009c6(s32);
void Func_02000a20(s32);
void Func_02000a28(s32);
void Func_02000a38(s32, s32);
void Func_02000a04(void);
void Func_02000a24(void);
void Func_02000a8e(s32, s32);
s32 Func_02000a26(s32);
void Func_02000a80(s32);
void Func_02000a88(s32);
void Func_02000a98(s32, s32);
void Func_02000a64(void);
void Func_02000a84(void);
void Func_02000aee(s32, s32);
s32 Func_02000a86(s32);
void Func_02000ae0(s32);
void Func_02000b10(s32, s32);
void Func_02000acc(void);
void Func_02000aec(void);
void Func_02000b60(s32, s32);
void Func_02000b4a(s32);
void Func_02000b52(s32);
void Func_02000b62(s32, s32);
void Func_02000b2e(void);
void Func_02000b44(void);
void Func_02000b82(s32);
void Func_02000b5e(void);
void Func_02000b64(void);
void Func_02000b7e(void);
void Func_02000b84(void);
void Func_02000bdc(s32, s32, s32);
s32 Func_02000bc6(s32, s32, s32);
void Func_02000b9c(s32);
s32 Func_02000bd6(s32, s32, s32);
void Func_02000bac(s32);
void Func_02000bfc(s32, s32);
void Func_02000bc8(void);
s32 Func_02000678(s32, s32, s32, s32);
void Func_02000c3e(s32);
void Func_02000c76(s32, s32);

#define CreateOverlayObject Func_020006de
#define Dialogue_RunActor10Dialogue Func_020004e4
#define Dialogue_RunActor12Dialogue Func_02000504
#define Dialogue_RunActor14FlaggedDialogue Func_0200035c
#define Dialogue_RunActor15FlaggedDialogue Func_020003bc
#define Dialogue_RunActor16FlaggedDialogue Func_0200041c
#define Dialogue_RunActor17FlaggedDialogue Func_02000484
#define Dialogue_RunActor9MotionDialogue Func_02000524
#define Effect_SpawnConfiguredEffect Func_0200013c
#define Object Func_02000030
#define OvObj_CreateConfigured Func_020000a0
#define OvObj_IntegrateVelocities Func_02000104
#define OvObj_SetEntryField Func_02000030
#define OvObj_SpawnWithMode14 Func_02000048
#define SceneData_GetTable8778 Func_02000314
#define SceneData_GetTable8868 Func_02000320
#define SceneData_ReturnZero Func_0200031c
#define SceneData_SelectTable89c8Or8890 Func_02000328
#define SceneData_SelectTable8d4cOr8a28 Func_02000570
#define SetOverlayObjectMode Func_02000710
#define SetOverlayObjectSlot Func_02000770
#define State_SetRuntimeWord448To521 Func_02000598
#define Talk_Apply Func_020006b4
#define Talk_Apply10 Func_020008d4
#define Talk_Apply11 Func_020008e4
#define Talk_Apply12 Func_02000a2e
#define Talk_Apply13 Func_02000a38
#define Talk_Apply14 Func_02000a8e
#define Talk_Apply15 Func_02000a98
#define Talk_Apply16 Func_02000aee
#define Talk_Apply17 Func_02000b10
#define Talk_Apply18 Func_02000b60
#define Talk_Apply19 Func_02000b62
#define Talk_Apply2 Func_02000714
#define Talk_Apply20 Func_02000bfc
#define Talk_Apply21 Func_02000c76
#define Talk_Apply3 Func_020006cc
#define Talk_Apply4 Func_020007ba
#define Talk_Apply5 Func_020007d4
#define Talk_Apply6 Func_020008a6
#define Talk_Apply7 Func_02000890
#define Talk_Apply8 Func_020008a8
#define Talk_Apply9 Func_020008b6
#define Talk_Check Func_020009c6
#define Talk_Check2 Func_02000a26
#define Talk_Check3 Func_02000a86
#define Talk_Do Func_020009b0
#define Talk_Do10 Func_02000b9c
#define Talk_Do11 Func_02000bac
#define Talk_Do12 Func_02000c3e
#define Talk_Do2 Func_02000a20
#define Talk_Do3 Func_02000a28
#define Talk_Do4 Func_02000a80
#define Talk_Do5 Func_02000a88
#define Talk_Do6 Func_02000ae0
#define Talk_Do7 Func_02000b4a
#define Talk_Do8 Func_02000b52
#define Talk_Do9 Func_02000b82
#define Talk_Place Func_02000bdc
#define Talk_Place2 Func_02000bc6
#define Talk_Place3 Func_02000bd6
#define Talk_Run Func_020007d2
#define Talk_Run10 Func_02000a24
#define Talk_Run11 Func_02000a64
#define Talk_Run12 Func_02000af0
#define Talk_Run13 Func_02000af0_a
#define Talk_Run14 Func_02000a84
#define Talk_Run15 Func_02000acc
#define Talk_Run16 Func_02000af0_b
#define Talk_Run17 Func_02000aec
#define Talk_Run18 Func_02000b2e
#define Talk_Run19 Func_02000ba2
#define Talk_Run2 Func_020007b0
#define Talk_Run20 Func_02000b44
#define Talk_Run21 Func_02000b5e
#define Talk_Run22 Func_02000ba2_a
#define Talk_Run23 Func_02000bc2
#define Talk_Run24 Func_02000b64
#define Talk_Run25 Func_02000b7e
#define Talk_Run26 Func_02000bc2_a
#define Talk_Run27 Func_02000b84
#define Talk_Run28 Func_02000bc8
#define Talk_Run29 Func_02000686_a
#define Talk_Run3 Func_020009d6
#define Talk_Run4 Func_02000a36
#define Talk_Run5 Func_02000a96
#define Talk_Run6 Func_02000afe
#define Talk_Run7 Func_02000686
#define Talk_Run8 Func_020009c4
#define Talk_Run9 Func_02000a04
#define Talk_SetMode Func_02000678
#define gCell Data_02000240
#define gOv Data_0200876c
#define gOv2 Data_02008778
#define gOv3 Data_02008868
#define gOv4 Data_020089c8
#define gOv5 Data_02008890
#define gOv6 Data_02008d4c
#define gOv7 Data_02008a28
#define gWork Data_03001ebc

#endif
