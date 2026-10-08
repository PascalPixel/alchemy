//--------------------------------------------------------------------------
// RADIAL.C
// フィールドのエナジー演出：14 番（運ばれる玉と、まわりを回る粒）ほか
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
#include "FX_SCENE.H"
#include "FIELDOBJ.H"
#include "FIXED_MATH.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"

#define BattleFx_ShrinkObjectScaleUntilHalf ((void (*)(void))BattleFx_ShrinkObjectScaleUntilHalf)
#define BattleFx_CircleAnchor ((void (*)(void))BattleFx_CircleAnchor)

struct EffectParticle
{
	u8					   reserved_00[6];
	s16					   angle; // 0x10000 で 1 周
	s32					   x;	  // 16.16 ドット
	s32					   y;
	s32					   z;
	u8					   reserved_14[4];
	s32					   scale_x; // 0x10000 で等倍
	s32					   scale_y;
	u8					   reserved_20[0x64 - 0x20];
	s16					   timer;  // BattleFx_CircleAnchor が数える
	s16					   pause;  // 同じく
	struct EffectParticle *anchor; // まわる中心
	void (*callback)(void);
};

extern struct BattleFxScene *gEffectWork;

void *Object_Spawn(s32, s32, s32, s32);
void  Object_SetMode(void *, s32);
void  Audio_PlayCue(s32);
void  Vector_AddPolarOffset(s32, s32, struct EffectVector *);
void  ObjectDispatch_ReleaseFar(void *);

s32 BattleEffect_RunFallbackObjectTransition();

//--------------------------------------------------------------------------
// エナジー 14 番 : 玉を使う者から scene の位置（4 ドット上）まで 10 フレームで運び、
// 0.25 倍から大きくする。scene->enabled がなければ、かけられた物を縮め、
// 玉のまわりに 6 フレームおきに粒を 16 個出して回らせる
//--------------------------------------------------------------------------
void RunBattleEffect14(void)
{
	struct BattleFxScene  *scene = gEffectWork;
	struct EffectParticle *src = scene->main_object;
	struct EffectParticle *tgt = scene->child;
	struct EffectVector	   pos;
	struct EffectVector	   from;
	struct EffectVector	   to;
	struct EffectParticle *tama;
	s32					   step;
	struct EffectParticle *particle;
	s32					   steps = 11;

	step = 0;
	from.x = src->x;
	from.y = src->y;
	from.z = src->z;
	to.x = scene->x;
	to.y = scene->y - (4 << 16);
	to.z = scene->z;
	tama = Object_Spawn(0xda, 0, 0, 0);
	if (tama == 0)
	{
		return;
	}
	BattleEffect_InitializeSharedScene();
	Object_SetMode(tama, 2);
	do
	{
		s32 scale;
		tama->x = from.x + step * (to.x - from.x) / 10;
		tama->y = from.y + step * (to.y - from.y) / 10;
		tama->z = from.z + step * (to.z - from.z) / 10;
		scale = 0x4000 + step * 0x10ccc / 10; // 0.25 倍から 1.3 倍へ
		tama->scale_x = scale;
		tama->scale_y = scale;
		WaitFrames(1);
		step++;
	} while (step < steps);
	tama->scale_x = 0x1b333; // 横 1.7 倍
	tama->scale_y = 0x14ccc; // 縦 1.3 倍
	Audio_PlayCue(163);
	WaitFrames(20);
	if (scene->enabled == 0)
	{
		if (tgt != 0)
			tgt->callback = BattleFx_ShrinkObjectScaleUntilHalf;

		// 粒は玉の 4 ドット上から 0.8 ドットずつ高く、3〜8 ドット離して出す
		step = 0;
		do
		{
			s32 dist;

			pos.x = tama->x;
			pos.y = tama->y + step * 0xcccc + (4 << 16);
			pos.z = tama->z;
			dist = Random16() * 5 + (3 << 16);
			Vector_AddPolarOffset(dist, Random16(), &pos);
			particle = Object_Spawn(0xf9, pos.x, pos.y, pos.z);
			if (particle != 0)
			{
				particle->callback = BattleFx_CircleAnchor;
				particle->anchor = tama;
				particle->timer = 0;
				particle->pause = 0;
				particle->angle = Random16();
			}
			WaitFrames(6);
			step++;
		} while (step <= 15);
		WaitFrames(20);
		WaitFrames(120);
	}
	Object_SetMode(tama, 1);
	WaitFrames(30);
	Audio_PlayCue(136);
	WaitFrames(20);
	ObjectDispatch_ReleaseFar(tama);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// スロットの粒の動き（毎フレーム）
// state 0 : 出た所から 40〜70 ドット離れた所へ向かう
// state 1 : 着くまで待ち、着いたら state 2 へ
// state 2 : scene の位置の 8 ドット上（画面の位置）のまわり 4 ドットへ向かう
// state 3 : 着いたらスロットを空ける
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialCamera(struct EffectSlot *effect)
{
	struct BattleFxScene *scene;
	struct EffectVector	  pos;
	s8					 *st;
	s16					  angle;
	s32					  state;

	scene = gEffectWork;
	st = &effect->state;
top:
	state = *st;
	if (state == 0)
	{
		pos.x = effect->origin_x;
		pos.z = effect->origin_z;
		angle = Random16();
		Vector_AddPolarOffset(Random16() * 30 + (40 << 16), (u16)angle, &pos);
		effect->target_x = pos.x;
		effect->target_z = pos.z;
		effect->acceleration = 4 << 16;
		effect->max_speed = 4 << 16;
		effect->flag42 = state;
		goto advance;
	}
	if (state == 1)
	{
		if (BattleFx_HasReachedTarget(effect) != 0)
			return;
		*st = *st + 1;
		goto top;
	}
	else if (state == 2)
	{
		pos.x = scene->x;
		pos.y = scene->y + (8 << 16);
		pos.z = scene->z;
		Camera_WorldToScreen(&pos);
		Vector_AddPolarOffset(4 << 16, Random16(), &pos);
		effect->target_x = pos.x;
		effect->target_z = pos.z;
		effect->max_turn_step = 0x1000;
		effect->flag42 = 1;
	advance:
		*st = *st + 1;
		return;
	}
	else if (state == 3)
	{
		if (BattleFx_HasReachedTarget(effect) == 0)
			BattleFx_ClearOwnedSlot(effect);
		return;
	}
	else
	{
		return;
	}
}

//--------------------------------------------------------------------------
// かけられた物（scene->child）に paused を立て、
// BattleEffect_RunFallbackObjectTransition の演出を動かす
//--------------------------------------------------------------------------
void BattleFx_MarkChildAndRunFallbackTransition(void)
{
	struct FieldActor *tgt = gEffectWork->child;

	tgt->paused = 1;
	BattleEffect_RunFallbackObjectTransition();
}
