#ifndef ALCHEMY_ABI_PARTY_CHECK_MEMBER_VALUE_TOTAL
#define ALCHEMY_ABI_PARTY_CHECK_MEMBER_VALUE_TOTAL

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_08077148(s32);
extern s32 Func_080772d8(u8, s32);

#define Party_Apply Func_080772d8
#define Party_Check Func_08077148
#define gCell Data_02000240

#endif
