#include "types.h"
#include "field_obj.h"

static s32 SceneActor_UpdateScalePulse(struct FieldActor *state)
{
	switch (state->timer)
	{
	case 6:
		state->scale_x -= 0x4000;
		state->scale_y += 0x2000;
		break;
	case 4:
		state->scale_x += 0x2000;
		state->scale_y += -0x1000;
		break;
	case 2:
		state->scale_x += 0x1000;
		state->scale_y -= 0x800;
		break;
	case 0:
		state->scale_x = 0x10000;
		state->scale_y = 0x10000;
		state->timer = (Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60);
		break;
	}
	state->timer--;
	return 1;
}
