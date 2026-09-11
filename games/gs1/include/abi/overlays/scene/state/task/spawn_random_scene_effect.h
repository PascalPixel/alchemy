#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STATE_TASK_SPAWN_RANDOM_SCENE_EFFECT
#define ALCHEMY_ABI_OVERLAYS_SCENE_STATE_TASK_SPAWN_RANDOM_SCENE_EFFECT

/* Binding layer — not production source. Address / far-call ABI only. */

extern u32 Func_02007030(void);
extern s32 Func_0200704c(void);
extern s32 Func_02007052(void);
extern void Func_0200706e(s32, s32, s32 *);
extern void Func_02007166(Obj *, s32);
extern void Func_0200711e(Obj *, s32);
extern void Func_0200711e_a(Obj *, s32);
extern void Func_02007126(Obj *, s32);

#define State_Apply Func_02007166
#define State_Apply2 Func_0200711e
#define State_Apply3 Func_0200711e_a
#define State_Apply4 Func_02007126
#define State_Check Func_0200704c
#define State_Check2 Func_02007052
#define State_Place Func_0200706e
#define State_Run Func_0200710a
#define State_Run2 Func_0200325c
#define State_Run3 Func_02007030
#define gOv Data_0200c264

#endif
