#ifndef ALCHEMY_ABI_OBJECT_PLACEMENT_CREATE_GROUP
#define ALCHEMY_ABI_OBJECT_PLACEMENT_CREATE_GROUP

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0808a5f0(s8 kind, s8 variant);
void Func_0801ff58(void);

#define Obj_Apply Func_0808a5f0
#define Obj_Run Func_08009030
#define Obj_Run2 Func_0801ff58
#define gIw Data_03001f2c

#endif
