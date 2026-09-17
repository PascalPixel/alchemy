#ifndef ALCHEMY_AUDIO_ENGINE_SYMBOLS_H
#define ALCHEMY_AUDIO_ENGINE_SYMBOLS_H

#include "TYPES.H"

#define Audio_NoopStub Func_080fa260
#define Bios_CpuSet Func_08006864
#define Math_Div Func_080022ec
#define AUDIO_INITIAL_MODE 0x0097F800
#define AUDIO_MIXER_DESTINATION ((void *)0x03007000)

void Bios_CpuSet(const void *source, void *destination, u32 control);

#endif
