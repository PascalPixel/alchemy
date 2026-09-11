#ifndef ALCHEMY_ABI_BATTLE_PLACEMENT_UPDATE_ENTRIES
#define ALCHEMY_ABI_BATTLE_PLACEMENT_UPDATE_ENTRIES

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080b6a60(u16 *owners);
s32 Func_08077210(s32 id, s32 x, s32 y);
void Func_080771c8(s32 id, s32 x, s32 y);
s32 Func_080770c0(s32 message);
s32 Func_080771b0(s32 id, s32 x, s32 y);
s32 Func_080771c0(s32 id, s32 x, s32 y);

#define Battle_Check Func_080b6a60
#define Battle_Check2 Func_080770c0
#define Battle_Place Func_08077210
#define Battle_Place2 Func_080771c8
#define Battle_Place3 Func_080771b0
#define Battle_Place4 Func_080771c0
#define Battle_Run Func_08077000
#define Battle_Run2 Func_080b7dd0
#define Battle_Run3 Func_080b5c08

#endif
