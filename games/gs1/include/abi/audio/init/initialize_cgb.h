#ifndef ALCHEMY_ABI_AUDIO_INIT_INITIALIZE_CGB
#define ALCHEMY_ABI_AUDIO_INIT_INITIALIZE_CGB

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08006864(const void *source, void *destination, u32 control);

#define Audio_Place Func_08006864

#endif
