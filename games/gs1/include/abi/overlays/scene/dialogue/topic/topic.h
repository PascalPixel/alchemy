#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_DIALOGUE_TOPIC_TOPIC
#define ALCHEMY_ABI_OVERLAYS_SCENE_DIALOGUE_TOPIC_TOPIC

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_02001a74(void);
extern void Func_02001ac2(s32);
extern s32 Func_02001ad4(s32, s32);
extern void Func_02001a90(void);
extern void Func_02001a98(void);
extern void Func_02001ae6(s32);
extern s32 Func_02001af8(s32, s32);
extern void Func_02001ab4(void);
extern void Func_02000a9a(s32);

#define Talk_Apply Func_02001ad4
#define Talk_Apply2 Func_02001af8
#define Talk_Do Func_02001ac2
#define Talk_Do2 Func_02001ae6
#define Talk_Do3 Func_02000a9a
#define Talk_Run Func_02001a74
#define Talk_Run2 Func_02001a90
#define Talk_Run3 Func_02001a98
#define Talk_Run4 Func_02001ab4
#define gCell Data_02000240
#define gOv Data_02009f30
#define gOv2 Data_02009e1c
#define gOv3 Data_02009aec
#define gOv4 Data_02009cfc

#endif
