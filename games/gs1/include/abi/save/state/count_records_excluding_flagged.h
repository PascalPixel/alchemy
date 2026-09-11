#ifndef ALCHEMY_ABI_SAVE_STATE_COUNT_RECORDS_EXCLUDING_FLAGGED
#define ALCHEMY_ABI_SAVE_STATE_COUNT_RECORDS_EXCLUDING_FLAGGED

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080056cc();
s32 Func_08005c68();

#define State_Check Func_080056cc
#define State_Check2 Func_08005c68
#define State_Run Func_08005cf8

#endif
