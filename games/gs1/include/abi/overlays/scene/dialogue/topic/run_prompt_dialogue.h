#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_DIALOGUE_TOPIC_RUN_PROMPT_DIALOGUE
#define ALCHEMY_ABI_OVERLAYS_SCENE_DIALOGUE_TOPIC_RUN_PROMPT_DIALOGUE

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_02001a2a(s32);
extern s32 Func_02001a3a(s32, s32);
extern s32 Func_02001a0a(s32, s32);
extern void Func_020019f4(s32);
extern void Func_02001a4a(s32);
extern void Func_02001a52(s32);
extern void Func_02001a6a(s32, s32);

#define Talk_Apply Func_02001a3a
#define Talk_Apply2 Func_02001a0a
#define Talk_Apply3 Func_02001a6a
#define Talk_Do Func_02001a2a
#define Talk_Do2 Func_020019f4
#define Talk_Do3 Func_02001a4a
#define Talk_Do4 Func_02001a52

#endif
