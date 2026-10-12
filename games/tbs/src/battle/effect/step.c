#include "unit/battle/effect/effect.h"
#include "unit/battle/effect/fxdirect.h"
#include "unit/battle/effect/twopic.h"
#include "unit/battle/effect/strike.h"
#include "unit/battle/effect/dispatch.h"
#include "unit/battle/effect/scene.h"
#include "unit/battle/effect/burst.h"
#include "unit/battle/effect/step.h"
#include "unit/battle/effect/djinnfx.h"
#include "unit/battle/effect/swirl.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/objtable.h"
#include "unit/battle/effect/evtable.h"
#include "unit/battle/effect/djinnev.h"
#include "unit/battle/effect/psyfx.h"
#include "unit/runtime/matrix.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "canvas.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "menu_list.h"
#include "types.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "scene.h"
#include "global_cells.h"
#include "dma.h"
#include "resource_ids.h"
#include "battle_efx.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "btldisp.h"
#include "ram_buffer.h"
#include "resource.h"
#include "b5_context.h"
#include "motobj.h"
#include "iwram_call.h"
#include "transform.h"
#include "btlwork.h"
#include "project.h"
#include "field_event.h"
#include "keystate.h"
#include "btltype.h"
#include "sound_ids.h"
#include "call.h"
#include "btlsys.h"
#include "battle_session.h"
#include "animspr.h"
#include "battle_unit.h"
#include "fixed_point_position.h"
#include "irq.h"

void EffectStep_AdvanceWithGravity3D(struct EffectStep *step, s32 damping, s32 gravity)
{
	step->x += step->velocity_x;
	step->y += step->velocity_y;
	step->z += step->velocity_z;
	step->velocity_y += gravity;
	step->velocity_x = step->velocity_x * damping / 64;
	step->velocity_y = step->velocity_y * damping / 64;
	step->velocity_z = step->velocity_z * damping / 64;
}

void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity)
{
	step->x += step->velocity_x;
	step->y += step->velocity_y;
	step->velocity_y += gravity;
	step->velocity_x = step->velocity_x * damping / 64;
	step->velocity_y = step->velocity_y * damping / 64;
}

s32 EffectPosition_ApplyBaseAndYOffset(s32 *point, struct EffectPosition *position)
{
	s32 result = Render_ProjectPoint(point, &position->x);
	position->y -= 16;
	return result;
}

void EffectPosition_ApplyAnimationAndYOffset(s32 id, struct EffectPosition *position)
{
	BattleUnit_ProjectToScreen(id, &position->x);
	position->y -= 16;
}

void EffectPosition_ApplyStepAndYOffset(s32 id, struct EffectPosition *position)
{
	BattleMotion_ProjectScaledPositionFar(id, position);
	position->y -= 16;
}

void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position)
{
	BattleMotion_ProjectPositionFar(id, position);
	position->y -= 16;
}

s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen)
{
	struct BattleCamera	   *camera;
	struct MotionObject	   *object;
	struct AnimationObject *info;
	s32						scale;
	/* FAKEMATCH: needed to match: the unused 12-byte vector preserves both stack adjustments and camera-load order. */
	s32 unused[3];

	camera = gCameraWork;
	object = GetBattleObjectSlotFar(unit)->object;
	info = GetMotionRecordFar(object, 0);
	Render_ResetTransformState();
	Graphics_PrepareTransferInIwramWork((u8 *)camera, (u8 *)camera->pos);
	scale = Iwram_MulQ16(Render_ProjectPoint(&object->x, screen), info->scale);
	screen[1] -= Iwram_MulQ16(scale, Battle_GetObjectTableValueFar(unit) >> 17);
	return 0;
}

void BattleFx_ProbeKeysForExtendedActor(void)
{
	struct BattleEffectWork		*work = *(struct BattleEffectWork **)gBattleFxWork;
	struct BattleEffectArgument *effect = work->effect;

	if (effect->actors[0] > 0x7f)
		(void)gKeysHeld;
}
