#ifndef ALCHEMY_RESOURCE_3A8_EFFECT_H
#define ALCHEMY_RESOURCE_3A8_EFFECT_H

#include "types.h"

struct Resource3a8Effect
{
	u8	unk00[0x06];
	u16 unk06;
	u8	unk08[0x5e];
	s16 actionTimer;
};

#define RESOURCE3A8_EFFECT_OFFSET(field) ((u32) & (((struct Resource3a8Effect *)0)->field))
typedef char
	Resource3a8Effect_unknown_06_offset[RESOURCE3A8_EFFECT_OFFSET(unk06) == 0x06 ? 1 : -1];
typedef char
	Resource3a8Effect_action_timer_offset[RESOURCE3A8_EFFECT_OFFSET(actionTimer) == 0x66 ? 1 : -1];
typedef char Resource3a8Effect_size[sizeof(struct Resource3a8Effect) == 0x68 ? 1 : -1];
#undef RESOURCE3A8_EFFECT_OFFSET

#endif
