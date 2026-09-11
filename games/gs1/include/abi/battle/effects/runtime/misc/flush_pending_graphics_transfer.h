#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_RUNTIME_MISC_FLUSH_PENDING_GRAPHICS_TRANSFER
#define ALCHEMY_ABI_BATTLE_EFFECTS_RUNTIME_MISC_FLUSH_PENDING_GRAPHICS_TRANSFER

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08005534(const void *source, void *destination, s32 size);
void Func_080054e4(const void *source, void *destination, s32 size);
void Func_08005490(const void *source, s32 mode, void *destination, s32 size);

#define Battle_Place Func_08005534
#define Battle_Place2 Func_080054e4
#define Battle_SetMode Func_08005490

#endif
