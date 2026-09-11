#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_SCENE_TRANSITION_FINISH_AND_RELEASE_HEAP_BLOCK
#define ALCHEMY_ABI_BATTLE_EFFECTS_SCENE_TRANSITION_FINISH_AND_RELEASE_HEAP_BLOCK

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08002dd8(s32);
void Func_0808e0b0(struct Outer0808e0b0 *outer, s32 value);
void Func_08097adc(void);

#define Battle_Apply Func_0808e0b0
#define Battle_Check Func_08002dd8
#define Battle_Run Func_08097adc
#define gRom Data_08097645

#endif
