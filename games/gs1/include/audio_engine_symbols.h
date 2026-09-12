#ifndef ALCHEMY_AUDIO_ENGINE_SYMBOLS_H
#define ALCHEMY_AUDIO_ENGINE_SYMBOLS_H

#include "types.h"

#define Audio_NoopStub Func_080fa260
#define Audio_Place Func_08006864

void Audio_Place(const void *source, void *destination, u32 control);

#endif
