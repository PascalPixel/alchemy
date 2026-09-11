#ifndef ALCHEMY_ABI_BATTLE_COMMAND_EXECUTE_SELECTED_ACTION
#define ALCHEMY_ABI_BATTLE_COMMAND_EXECUTE_SELECTED_ACTION

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0808e5d8(s32);
void Func_08077120(s32, s32);
s32 Func_0808e4b4(s32, s32, void *);

#define Battle_Apply Func_08077120
#define Battle_Check Func_0808e5d8
#define Battle_Place Func_0808e4b4
#define Battle_Run Func_08077080
#define Battle_Run10 Func_08015140
#define Battle_Run11 Func_080770c8
#define Battle_Run12 Func_0808df1c
#define Battle_Run13 Func_0808b8e8
#define Battle_Run14 Func_08096fb0
#define Battle_Run15 Func_080970f8
#define Battle_Run16 Func_0809728c
#define Battle_Run17 Func_08096b28
#define Battle_Run18 Func_08096960
#define Battle_Run19 Func_08096810
#define Battle_Run2 Func_08077008
#define Battle_Run20 Func_08097174
#define Battle_Run21 Func_08096ab0
#define Battle_Run22 Func_08097194
#define Battle_Run23 Func_0808b98c
#define Battle_Run3 Func_0808ba1c
#define Battle_Run4 Func_08091660
#define Battle_Run5 Func_080770d0
#define Battle_Run6 Func_080770c0
#define Battle_Run7 Func_08015120
#define Battle_Run8 Func_08015040
#define Battle_Run9 Func_08091d84
#define gCell Data_02000240
#define gWork Data_03001ebc

#endif
