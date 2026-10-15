#include "types.h"
#include "fieldobj.h"
#include "field_effect.h"

static s32 Mercury_TrailSparks(struct FieldActor *actor)
{
	struct EffectOptions options;
	s32					 velocity_y;

	options.palette = 7;
	if ((gFrameCount & 1) == 0)
	{
		options.palette = 5;
	}
	options.start_scale_x = 0xcccc;
	options.start_scale_y = 0xcccc;
	options.priority = 0;
	velocity_y = -((((u32)Engine_RandomNext() * 8) >> 16) * 0x3333);
	Effect_Spawn(actor->x.fixed + ((8 - (gFrameCount & 15)) << 16), actor->y.fixed + 0x1a0000,
		actor->z.fixed, 0, velocity_y, 0,
		EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
	return 0;
}
