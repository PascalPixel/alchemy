#include "types.h"

extern u8 gWorkSlot[];

#define WORK_SLOT_STAGE 59

void Korosseo_SetGaugeHold(s16 v)
{
	*(s16 *)(*(u8 **)(gWorkSlot + WORK_SLOT_STAGE * 4) + 0xdc) = v;
}
