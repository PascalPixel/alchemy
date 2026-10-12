#ifndef ALCHEMY_UNIT_FIELD_PALQUEUE_H
#define ALCHEMY_UNIT_FIELD_PALQUEUE_H

#include "types.h"

void Runtime_AllocateAndClearQueue(void);
s32 PaletteQueue_Add(s16 bank, s16 index, s16 delay, s16 count);
void Runtime_ScheduleCallbackAndReleaseBlock28(void);
void Scheduler_ScheduleCallbackBAfterFrames(void);
void PaletteQueue_StepCycles(void);

#endif
