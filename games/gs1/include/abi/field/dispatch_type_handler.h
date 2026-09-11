#ifndef ALCHEMY_ABI_FIELD_DISPATCH_TYPE_HANDLER
#define ALCHEMY_ABI_FIELD_DISPATCH_TYPE_HANDLER

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08096810(void);
s32 Func_08096960(void);
void Func_08096ab0(void);

#define Field_Check Func_08096810
#define Field_Check2 Func_08096960
#define Field_Run Func_08096ab0

#endif
