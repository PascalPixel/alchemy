#ifndef ALCHEMY_UNIT_SYSTEM_SCHEDULER_OAMLIST_H
#define ALCHEMY_UNIT_SYSTEM_SCHEDULER_OAMLIST_H

#include "types.h"
#include "callback_scheduler.h"
#include "irq.h"

struct AffineTransform;

void Graphics_ResetFrameState(void);
s32 AffineMatrix_BuildForEffect(struct AffineTransform *source);
void Runtime_PushSlotEntry(void *slot_entry, s32 slot);
void Render_BuildOamList(void *work);

#endif
