#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STATE_TASK_RAISE_LINKED_SCENE_EFFECT
#define ALCHEMY_ABI_OVERLAYS_SCENE_STATE_TASK_RAISE_LINKED_SCENE_EFFECT

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_0200718e(Obj *, s32, s32, s32);
extern void Func_02007176(Obj *, s32);
extern void Func_020073dc(s32);
s32 Func_020032f0(Obj *a)
{
    Obj *o = Func_02007254(a->f64);

#define State_Apply Func_02007176
#define State_Do Func_020073dc
#define State_Run Func_02007254
#define State_SetMode Func_0200718e
#define gOv Data_0200c288

#endif
