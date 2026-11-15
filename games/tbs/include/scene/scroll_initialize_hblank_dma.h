#ifndef ALCHEMY_SCROLL_INITIALIZE_HBLANK_DMA_H
#define ALCHEMY_SCROLL_INITIALIZE_HBLANK_DMA_H

#include "fixed_math.h"
#include "types.h"

void LineScroll_InitWave(s32 mode, FIXED freqX, s32 stepX, FIXED ampX,
                         FIXED freqY, s32 stepY, FIXED ampY);

#endif
