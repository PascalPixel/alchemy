#ifndef ALCHEMY_ABI_OBJECT_EFFECTS_RUN_PENDING_FLAG_EVENT
#define ALCHEMY_ABI_OBJECT_EFFECTS_RUN_PENDING_FLAG_EVENT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08094380(s32 arg0);
void Func_0809335c(s32 arg0, s32 arg1);

#define Obj_Apply Func_0809335c
#define Obj_Do Func_08094380
#define gCell Data_02000240

#endif
