//--------------------------------------------------------------------------
// NEAROBJ.C
// 向いている方の近くにあるオブジェクトを選ぶ
//--------------------------------------------------------------------------

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "FIXED_MATH.H"
#include "IWRAM_CALL.H"
#include "OBJECT_LOOKUP.H"
#include "SCENE.H"
#include "EFFECT_RUNTIME.H"
#include "GLOBAL_CELLS.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "OBJECT_EFFECT.H"

u16 ArcTan2(s32 deltaZ, s32 deltaX);

extern void Object_SetMode(struct ParticleEffectObject *, s32);
s32			Object_GetById(u32);
s32			Audio_PlayCue(s32);

#define OBJ_LAST 66 // 調べるオブジェクト番号の最後

//--------------------------------------------------------------------------
// mode に対する探す距離 (ドット)
// BattleFx_TargetRangeByMode は mode と距離の組の並びで、-1 で終わる。なければ 16
//--------------------------------------------------------------------------
s32 BattleFx_MapKeyThroughTable(s32 mode)
{
	s16 *p = BattleFx_TargetRangeByMode;
	s32	 range = 16;
	s32	 key = *p;

	while (key != -1)
	{
		++p;
		if (mode == key)
		{
			range = *p;
			break;
		}
		++p;
		key = *p;
	}
	return range;
}

//--------------------------------------------------------------------------
// id のオブジェクトにいちばん近いオブジェクトを選ぶ (なければ -1)
// 高さの差 48 ドット未満、距離 32 ドット未満のもの。11 ドットより遠ければ
// 向きの ±0x2fff の中にあるものだけ。unknown_59 の 8 は除き、4 は 10/13 の距離とみる
//--------------------------------------------------------------------------
s32 BattleEffect_SelectNearbyObject(s32 id)
{
	struct ObjectRuntime *src;
	struct ObjectRuntime *obj;
	s32					  best;
	s32					  best_dist;
	s32					  i;
	s32					  dx;
	s32					  dy;
	s32					  dz;
	s32					  xx;
	s32					  yy;
	s32					  zz;
	s32					  sq;
	s32					  dist;
	s32					  angle;

	best = -1;
	best_dist = 32;
	src = ObjectTable_Get(id);
	if (src == 0)
		goto done;

	for (i = 0; i <= OBJ_LAST; i++)
	{
		if (i == id)
			continue;

		obj = ObjectTable_Get(i);
		if (obj == 0 || (obj->unknown_59 & 8) != 0)
			continue;

		if (obj->y - src->y >= 0)
		{
			if (obj->y - src->y >= 48 << 16)
				continue;
		}
		else if (src->y - obj->y >= 48 << 16)
		{
			continue;
		}

		// ドット単位の距離
		dx = (obj->x - src->x) / 0x10000;
		dy = (obj->y - src->y) / 0x10000;
		dz = (obj->z - src->z) / 0x10000;

		xx = dx * dx;
		yy = dy * dy;
		zz = dz * dz;
		sq = xx + yy + zz;
		dist = Iwram_Sqrt(sq);
		if ((obj->unknown_59 & 4) != 0)
			dist = dist * 10 / 13;
		if (dist >= best_dist)
			continue;

		angle = ArcTan2(obj->z - src->z, obj->x - src->x);
		if (dist > 11)
		{
			s32 diff = (s16)(angle - src->angle);

			if (diff < -0x2fff)
				continue;
			if (diff > 0x2fff)
				continue;
		}

		best = i;
		best_dist = dist;
	}

done:
	return best;
}

//--------------------------------------------------------------------------
// id のオブジェクトの前にいちばん近いオブジェクトを選ぶ (なければ -1)
// 向きは 90 度単位に丸め、距離は x と z だけで測る。
// mode : 探す距離と、高さの差・向きの幅を決める
//--------------------------------------------------------------------------
s32 BattleEffect_SelectNearbyTargetObject(s32 id, s32 mode)
{
	struct ObjectRuntime *src;
	struct ObjectRuntime *obj;
	s32					  best;
	s32					  best_dist;
	s32					  dir;
	s32					  i;
	s32					  dy_max;
	s32					  dx;
	s32					  dz;
	s32					  cz;
	s32					  dist;
	s32					  angle;
	s32					  width;

	best = -1;
	best_dist = BattleFx_MapKeyThroughTable(mode);
	src = (struct ObjectRuntime *)ObjectTable_Get(id);
	if (src == 0)
		return best;

	dir = (src->angle + 0x2000) & 0xc000;
	for (i = 0; i <= OBJ_LAST; i++)
	{
		if (i == id)
			continue;

		obj = (struct ObjectRuntime *)ObjectTable_Get(i);
		if (obj == 0 || (obj->unknown_59 & 8) != 0)
			continue;

		// 高さの差 : ふつうは 8、mode 13 は 48、5 は 64、2 は 16 ドットまで
		dy_max = 8 << 16;
		if (mode == 13)
			dy_max = 48 << 16;
		if (mode == 5)
			dy_max = 64 << 16;
		if (mode == 2)
			dy_max = 16 << 16;

		{
			s32 dy = obj->y - src->y;
			if (dy >= 0)
			{
				if (dy > dy_max)
					continue;
			}
			else if (src->y - obj->y > dy_max)
			{
				continue;
			}
		}

		// ドット単位の距離 (0 へ向けて切り捨て)
		dx = obj->x - src->x;
		if (dx < 0)
			dx += 0xffff;
		dx >>= 16;

		dz = obj->z - src->z;
		if (dz < 0)
			dz += 0xffff;
		cz = dz >> 16;

		dist = Iwram_Sqrt(dx * dx + cz * cz);
		if ((obj->unknown_59 & 0x10) != 0)
			dist = dist * 2 / 3;
		if (dist >= best_dist)
			continue;

		// 向きの幅 : 19 ドットまでは ±0x1800、それより遠ければ ±0x1000、mode 2 は ±0x2000
		angle = ArcTan2(obj->z - src->z, obj->x - src->x);
		width = 0x1800;
		if (dist > 19)
			width = 0x1000;
		if (mode == 2)
			width = 0x2000;

		if (dist > 11)
		{
			s32 diff = (s16)(angle - dir);
			if (diff < 0)
				diff = -diff;
			if (diff >= width)
				continue;
		}

		best = i;
		best_dist = dist;
	}

	return best;
}

//--------------------------------------------------------------------------
// target のオブジェクトがいれば音を鳴らして mode 4 にし、12 フレームのちに
// 粒の発生を始める
//--------------------------------------------------------------------------
s32 BattleFx_PlayCueAndStartEmitterOnTarget(s32 effect, s32 target, s32 mode)
{
	s32 obj;
	s32 ret;

	obj = Object_GetById(target);
	ret = 0;
	if (obj != 0)
	{
		Audio_PlayCue(124);
		Object_SetMode(obj, 4);
		WaitFrames(12);
		ret = BattleFx_StartRandomParticleEmitter(effect, mode);
	}
	return ret;
}
