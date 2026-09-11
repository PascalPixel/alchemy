#ifndef ALCHEMY_ABI_BATTLE_PLACEMENT_UPDATE_TIMED_ENTRIES
#define ALCHEMY_ABI_BATTLE_PLACEMENT_UPDATE_TIMED_ENTRIES

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080771b0(u8 id, u8 x, u8 y);
void Func_080771c0(u8 id, u8 x, u8 y);

#define Battle_Place Func_080771b0
#define Battle_Place2 Func_080771c0
#define Battle_Run Func_08077000

#endif
