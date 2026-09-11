#ifndef ALCHEMY_ABI_PSYNERGY_MENU_INITIALIZE_ENTRY_OBJECTS
#define ALCHEMY_ABI_PSYNERGY_MENU_INITIALIZE_ENTRY_OBJECTS

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08077158(u16 *out);
s32 Func_0808a288(u16 value);
void Func_080a19a0(void);

#define Sys_Check Func_08077158
#define Sys_Check2 Func_0808a288
#define Sys_Run Func_08009030
#define Sys_Run2 Func_080a19a0
#define gIw Data_03001f2c

#endif
