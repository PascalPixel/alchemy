#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_RUNTIME_MISC_ENABLE_TWO_CALLBACKS
#define ALCHEMY_ABI_BATTLE_EFFECTS_RUNTIME_MISC_ENABLE_TWO_CALLBACKS

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080042c8(u32 value);

#define Battle_Check Func_080042c8
#define gRom Data_0808f52d
#define gRom2 Data_0808f499

#endif
