#ifndef ALCHEMY_ABI_OBJECT_GROUP_STORE_OBJECT_AND_RUN_STEP7
#define ALCHEMY_ABI_OBJECT_GROUP_STORE_OBJECT_AND_RUN_STEP7

/* Binding layer — not production source. Address / far-call ABI only. */

volatile int Func_080cd594(s32);
s32 Func_080cdbc0();
s32 Func_080de2f8(void *, s32, s32, s32, s32 *, s32 *);

#define Obj_Check Func_080cdbc0
#define Obj_Do Func_080cd594
#define Obj_SetRect Func_080de2f8

#endif
