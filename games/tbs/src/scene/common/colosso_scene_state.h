#include "types.h"

#define WORK_SLOT_STAGE 59

static void Colosso_SetGaugeHold(s16 v)
{
	*(s16 *)(*(u8 **)((u8 *)gWorkSlot + WORK_SLOT_STAGE * 4) + 0xdc) = v;
}
