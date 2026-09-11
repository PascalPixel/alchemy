#ifndef ALCHEMY_ABI_OBJECT_MOTION_AIM_APPROACH_LINKED_OBJECT
#define ALCHEMY_ABI_OBJECT_MOTION_AIM_APPROACH_LINKED_OBJECT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080072f0(s32, s32, s32, s32);
void Func_0800c300(void *object, s32 argument);
s32 Func_0800d14c(void *, s32, s32, s32);

#define Obj_Apply Func_0800c300
#define Obj_SetMode Func_080072f0
#define Obj_SetMode2 Func_0800d14c

#endif
