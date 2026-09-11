#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_DIALOGUE_TOPIC_RUN_ACTOR_CUE_BRANCH
#define ALCHEMY_ABI_OVERLAYS_SCENE_DIALOGUE_TOPIC_RUN_ACTOR_CUE_BRANCH

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_020019e2(s32);
extern s32 Func_020019f2(s32, s32);
extern s32 Func_020019c2(s32, s32);
extern void Func_020019ac(s32);
extern void Func_02001a02(s32);
extern void Func_02001a0a(s32);
extern void Func_02001a22(s32, s32);

#define Talk_Apply Func_020019f2
#define Talk_Apply2 Func_020019c2
#define Talk_Apply3 Func_02001a22
#define Talk_Do Func_020019e2
#define Talk_Do2 Func_020019ac
#define Talk_Do3 Func_02001a02
#define Talk_Do4 Func_02001a0a

#endif
