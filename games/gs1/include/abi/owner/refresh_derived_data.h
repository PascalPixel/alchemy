#ifndef ALCHEMY_ABI_OWNER_REFRESH_DERIVED_DATA
#define ALCHEMY_ABI_OWNER_REFRESH_DERIVED_DATA

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08078bf0(s32);
s8 Func_080799b0(u8, const u8 *);

#define Sys_Apply Func_080799b0
#define Sys_Check Func_08078bf0

#endif
