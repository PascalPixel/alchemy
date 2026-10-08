//--------------------------------------------------------------------------
// impact.c
// 散って集まる粒の動きと、重い衝撃の破片
//--------------------------------------------------------------------------

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fixed_math.h"
#include "objdisp.h"
#include "types.h"
#include "object_runtime.h"
#include "motobj.h"
#include "fx_scene.h"
#include "object_efx.h"
#include "sound_ids.h"

#define Camera_WorldToScreen ((void (*)(struct EffectPosition *))Camera_WorldToScreen)

struct EffectPosition
{
	s32 x; // 16.16 固定小数
	s32 y;
	s32 z;
};

extern struct BattleFxScene *gEffectWork;

void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectPosition *output);

void BattleFx_ClearOwnedSlot(struct EffectSlot *effect);

void				  AudioCommand_PlayFar(s32 cue);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);

s32 RunBattleEffect04();

//--------------------------------------------------------------------------
// 粒の動き : 原点のまわりへ散ってから、主オブジェクトのそばへ集まって消える
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialBurst(struct EffectSlot *slot)
{
	struct BattleFxScene *scene = gEffectWork;
	struct EffectPosition pos;
	s32					  state;

again:
	state = slot->state;
	if (state == 0)
	{
		// 原点から 25 ドット、でたらめな方向へ
		pos.x = slot->origin_x;
		pos.z = slot->origin_z;
		Vector_AddPolarOffset(25 << 16, (u16)Random16(), &pos);
		slot->target_x = pos.x;
		slot->target_z = pos.z;
		slot->acceleration = 3 << 16;
		slot->max_speed = 3 << 16;
		slot->flag42 = 0;
		slot->state++;
		return;
	}

	if (state == 1)
	{
		// 着いたらすぐ次へ
		if (BattleFx_HasReachedTarget(slot) == 0)
		{
			slot->state++;
			goto again;
		}
		return;
	}

	if (state == 2)
	{
		// 主オブジェクトの 16 ドット上から scene->angle へ 8 ドット、
		// 画面座標にして 4 ドットの揺らぎを足す
		struct MotionObject *obj = scene->main_object;

		pos.x = obj->x;
		pos.y = obj->y + (16 << 16);
		pos.z = obj->z;
		Vector_AddPolarOffset(8 << 16, scene->angle, &pos);
		Camera_WorldToScreen(&pos);
		Vector_AddPolarOffset(4 << 16, Random16(), &pos);
		slot->target_x = pos.x;
		slot->target_z = pos.z;
		slot->max_turn_step = 0x800;
		slot->flag42 = 1;
		slot->state++;
		return;
	}

	if (state == 3 && BattleFx_HasReachedTarget(slot) == 0)
		BattleFx_ClearOwnedSlot(slot);
}

//--------------------------------------------------------------------------
// 重い衝撃 : y を 32 ドット下げた位置に 0x11b を置き、破片を 12 個まき散らす
// src : 衝撃の位置になるオブジェクト
//--------------------------------------------------------------------------
s32 SpawnHeavyImpactEffect(struct MotionObject *src)
{
	struct EffectPosition pos;
	struct ObjectRuntime *obj;
	struct ObjectRuntime *frag;
	s32					  i;

	AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
	pos.x = src->x;
	pos.y = src->y;
	pos.z = src->z;
	obj = Object_Spawn(0x11b, pos.x, pos.y - (32 << 16), pos.z);
	if (obj != NULL)
	{
		obj->flags = 0;
		obj->unknown_5e = 20;
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
	}

	// 破片 : 速さ 1〜2 ドット、8〜32 ドット先のでたらめな方向へ
	for (i = 0; i < 12; i++)
	{
		frag = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
		if (frag != NULL)
		{
			ObjectDispatch_InitializeFar((struct DispatchObject *)frag, BattleFx_FragmentScript);
			frag->speed_limit = Random16() + (1 << 16);
			frag->acceleration = 1 << 16;
			frag->flags = 0;
			Motion_SetTargetPositionFromMagnitudeAngle(
				frag, Random16() * 24 + (8 << 16), Random16());
		}
	}
	return 0;
}

//--------------------------------------------------------------------------
// 戦闘エフェクト 4 番を呼ぶ
//--------------------------------------------------------------------------
void BattleFx_CallEffect04(void)
{
	RunBattleEffect04();
}
