#ifndef ALCHEMY_ABI_AUDIO_MIX_DISABLE_PCM_DMA
#define ALCHEMY_ABI_AUDIO_MIX_DISABLE_PCM_DMA

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08006864(s32 *, u8 *, void *);

#define Audio_Place Func_08006864

#endif
