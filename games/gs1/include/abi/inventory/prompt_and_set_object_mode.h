#ifndef ALCHEMY_ABI_INVENTORY_PROMPT_AND_SET_OBJECT_MODE
#define ALCHEMY_ABI_INVENTORY_PROMPT_AND_SET_OBJECT_MODE

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08015390(s32, s32, s32, s32);
void Func_080924d4(s32, s32);
void Func_08015100(s32);
void Func_08091c44(s32, s32);

#define Sys_Apply Func_080924d4
#define Sys_Apply2 Func_08091c44
#define Sys_Do Func_08015100
#define Sys_Run Func_0808d394
#define Sys_SetMode Func_08015390
#define gIw Data_03001c94
#define gWork Data_03001ebc

#endif
