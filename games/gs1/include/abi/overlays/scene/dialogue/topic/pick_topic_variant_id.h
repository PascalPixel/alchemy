#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_DIALOGUE_TOPIC_PICK_TOPIC_VARIANT_ID
#define ALCHEMY_ABI_OVERLAYS_SCENE_DIALOGUE_TOPIC_PICK_TOPIC_VARIANT_ID

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_020025f8(void);
extern s32 Func_02002610(void);
extern s32 Func_02002608(s32, s32);

#define Talk_Apply Func_02002608
#define Talk_Check Func_020025f8
#define Talk_Check2 Func_02002610
#define gCell Data_02000240
#define gOv Data_0200a018

#endif
