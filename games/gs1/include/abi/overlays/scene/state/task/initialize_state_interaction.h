#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STATE_TASK_INITIALIZE_STATE_INTERACTION
#define ALCHEMY_ABI_OVERLAYS_SCENE_STATE_TASK_INITIALIZE_STATE_INTERACTION

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_02005f72(s32, s32);
extern void Func_020060ba(s32);
extern void Func_020060d2(s32, s32);

#define State_Apply Func_02005f72
#define State_Apply2 Func_020060d2
#define State_Do Func_020060ba
#define State_Run Func_020020b8
#define gCell Data_02000240

#endif
