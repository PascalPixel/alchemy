#ifndef ALCHEMY_ABI_INN_SLEEP
#define ALCHEMY_ABI_INN_SLEEP

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08077158(s16 *);
void Func_08077230(s32);
void Func_08077128(s32);
void Func_0808a368(void);
void Func_0808a370(void);
void Func_0808a360(void);

#define Sys_Check Func_08077158
#define Sys_Do Func_08077230
#define Sys_Do2 Func_08077128
#define Sys_Run Func_0808a368
#define Sys_Run2 Func_0808a370
#define Sys_Run3 Func_0808a360
#define gWork Data_03001ebc

#endif
