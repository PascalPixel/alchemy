#ifndef ALCHEMY_UNIT_RUNTIME_IOQUEUE_H
#define ALCHEMY_UNIT_RUNTIME_IOQUEUE_H

#include "types.h"
#include "callback_scheduler.h"
#include "irq.h"

void IoWriteQueue_FlushPending(void);

#endif
