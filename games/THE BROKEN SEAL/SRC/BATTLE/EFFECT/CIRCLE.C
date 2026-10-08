//--------------------------------------------------------------------------
// CIRCLE.C
// バトル演出：輪に散る粒子と、中心のまわりを回るオブジェクト
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
#include "SCRIPT_MOTION.H"
#include "MOTOBJ.H"
#include "TYPES.H"
#include "OBJDISP.H"
#include "FIELD_EFFECT.H"
#include "FIXED_MATH.H"
#include "IWRAM_CALL.H"
#include "SYSTEM.H"

#define Object_SetPositionAndResetMotionFar \
	((void (*)(void *, s32, s32, s32))Object_SetPositionAndResetMotionFar)

extern const s32 BattleFx_CommonParticleScript[];

struct OrbitEffect
{
	u8					unknown_00[6];
	u16					angle; // 進む向き・回る角度
	s32					x;
	s32					y;
	s32					z;
	u8					unknown_14[20];
	s32					scale;
	u8					unknown_2c[28];
	s32					speed;
	u8					unknown_4c[24];
	u16					timer;	// 経過フレーム
	s16					pause;	// 向きを大きく変える残りフレーム
	struct OrbitEffect *anchor; // 回る中心
	void (*update)(struct OrbitEffect *);
};

s32 RunBattleEffect14();

//--------------------------------------------------------------------------
// 粒子の毎フレーム処理
// 速度で進み、x と z の速度を少しずつ落とし、拡大率と回転を変える
//--------------------------------------------------------------------------
void BattleFx_UpdateParticleMotionAndScale(union FieldObject *object)
{
	struct FieldEffect *efx = &object->effect;

	efx->x += efx->velocity_x;
	efx->y += efx->velocity_y;
	efx->z += efx->velocity_z;
	efx->velocity_x -= efx->velocity_x / 18;
	efx->velocity_z -= efx->velocity_z / 16;
	efx->scale_x += efx->scale_rate_x;
	efx->scale_y += efx->scale_rate_y;
	efx->sprite->rotation += efx->spin;
}

//--------------------------------------------------------------------------
// origin の位置から、水平に輪を描いて広がる粒子を 17 個出す
// x 方向の速度は z 方向の 1.5 倍
//--------------------------------------------------------------------------
void BattleFx_SpawnRadialParticleRing(struct MotionObject *origin)
{
	struct EffectOptions opt;
	s32					 vel[3];
	s32					 vz;
	s32					 angle;
	u32					 i;

	opt.palette = 0;
	opt.update = BattleFx_UpdateParticleMotionAndScale;
	opt.start_scale_x = 0xcccc; // 0.8 倍
	opt.start_scale_y = 0xcccc;
	i = 0;
	do
	{
		angle = i << 12; // 1/16 周ずつ
		vel[0] = Trig_Cos(angle) * 3 / 2;
		vel[1] = 0;
		vz = Trig_Sin(angle);
		vel[2] = vz;
		Effect_SpawnResident(origin->x, origin->y, origin->z, vel[0], vel[1], vz, 0x01090001, &opt);
		i++;
	} while (i <= 16);
}

//--------------------------------------------------------------------------
// 中心を離れてさまよう（毎フレーム）
// 1 フレームに 2〜3 ドット進み、ときどき向きを大きく変える
// 101 フレームで共通の粒子スクリプトへ移る
//--------------------------------------------------------------------------
void BattleFx_WanderAroundAnchor(struct OrbitEffect *efx)
{
	s32 len;
	s32 angle;
	s32 dx;
	s32 dz;

	len = Random16() + 0x20000;
	angle = efx->angle;
	dx = Iwram_MulQ16(len, Trig_Cos(angle));
	dz = Iwram_MulQ16(len, Trig_Sin(angle));
	efx->x += dx;
	efx->z += dz;
	efx->angle += 0xfff0;
	if (efx->pause != 0)
	{
		efx->pause--;
		efx->angle += 0x800;
	}
	else if ((Random16() << 5) >> 16 == 0) // 1/32 の確率
	{
		efx->pause = ((Random16() << 4) >> 16) + 8; // 8〜23 フレーム
	}
	if (++efx->timer == 101)
		ObjectDispatch_InitializeFar((struct DispatchObject *)efx, BattleFx_CommonParticleScript);
}

//--------------------------------------------------------------------------
// 中心のまわりを半径 8 ドットで回る（毎フレーム）
// 121 フレーム回ったら、さまよう動きに切り替える
//--------------------------------------------------------------------------
void BattleFx_CircleAnchor(struct OrbitEffect *efx)
{
	struct OrbitEffect *anchor;
	s32					angle;
	s32					r;
	s32					dx;
	s32					dz;

	anchor = efx->anchor;
	r = 0x80000;
	angle = efx->angle;
	dx = Iwram_MulQ16(r, Trig_Cos(angle));
	dz = Iwram_MulQ16(r, Trig_Sin(angle));
	efx->x = anchor->x + dx;
	efx->z = anchor->z + dz;
	efx->angle += 0x800;
	if (++efx->timer == 121)
	{
		efx->update = BattleFx_WanderAroundAnchor;
		efx->timer = 0;
		efx->pause = 0;
		efx->speed = 0x1999;
		efx->scale = 0x30000;
		efx->angle = Random16();
	}
}

//--------------------------------------------------------------------------
// 毎フレーム縮め、0.5 倍を切ったら位置を戻して処理を外す
//--------------------------------------------------------------------------
void BattleFx_ShrinkObjectScaleUntilHalf(struct MotionObject *obj)
{
	s32 scale;

	scale = obj->scale_x - 0x80;
	obj->scale_y = scale;
	obj->scale_x = scale;
	if (scale < 0x8000)
	{
		Object_SetPositionAndResetMotionFar(obj, 0, 0, 0);
		((struct ScriptMotionObject *)obj)->hook = 0;
	}
}

void BattleFx_CallEffect14(void)
{
	RunBattleEffect14();
}
