#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STORY_FLAG_BRANCHED_RUN_ACTOR_16_MESSAGE
#define ALCHEMY_ABI_OVERLAYS_SCENE_STORY_FLAG_BRANCHED_RUN_ACTOR_16_MESSAGE

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_02002628(void);
extern void Func_020026f6(s32);
extern s32 Func_02002614(s32);
extern void Func_02002706(s32);
extern void Func_0200271e(s32, s32);
extern void Func_02002634(s32);
extern void Func_02002658(void);
void Func_020001b4(void)
{
    Func_02002628();
extern void Func_02002668(void);
extern void Func_02002736(s32);
extern s32 Func_0200275e(s32, s32);
extern void Func_02002682(void);

#define Story_Apply Func_0200271e
#define Story_Apply2 Func_0200275e
#define Story_Check Func_02002614
#define Story_Do Func_020026f6
#define Story_Do2 Func_02002706
#define Story_Do3 Func_02002634
#define Story_Do4 Func_02002736
#define Story_Run Func_02002658
#define Story_Run2 Func_02002668
#define Story_Run3 Func_02002682

#endif
