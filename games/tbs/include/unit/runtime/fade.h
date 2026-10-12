#ifndef ALCHEMY_UNIT_RUNTIME_FADE_H
#define ALCHEMY_UNIT_RUNTIME_FADE_H

#include "types.h"
#include "callback_scheduler.h"
#include "irq.h"

void BlendTransition_Update(void);
void Blend_ConfigureTransition(s8 bright, s32 layer, u32 start, s32 target, s32 frames);
void Blend_WaitForTransition(void);

#endif
