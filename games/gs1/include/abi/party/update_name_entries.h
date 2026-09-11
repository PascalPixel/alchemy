#ifndef ALCHEMY_ABI_PARTY_UPDATE_NAME_ENTRIES
#define ALCHEMY_ABI_PARTY_UPDATE_NAME_ENTRIES

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08002df0(void *);
void Func_08006488(void);
s32 Func_08006408(void);
void Func_08015020(s32, u16 *);

#define Party_Apply Func_08015020
#define Party_Check Func_08006408
#define Party_Do Func_08002df0
#define Party_Run Func_08077000
#define Party_Run2 Func_08006488

#endif
