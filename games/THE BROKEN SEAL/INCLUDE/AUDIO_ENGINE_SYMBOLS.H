#ifndef ALCHEMY_AUDIO_ENGINE_SYMBOLS_H
#define ALCHEMY_AUDIO_ENGINE_SYMBOLS_H

#include "TYPES.H"

#define Audio_NoopStub Func_080fa260
#define Bios_CpuSet Func_08006864

void Bios_CpuSet(const void *source, void *destination, u32 control);

#endif
