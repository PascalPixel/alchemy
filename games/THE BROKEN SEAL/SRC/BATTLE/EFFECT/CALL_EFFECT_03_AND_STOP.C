//--------------------------------------------------------------------------
// CALL_EFFECT_03_AND_STOP.C
// 演出 03：渦を巻いて集まる 8 個の粒と、火花を出して飛び去る 1 個
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
#include "RESOURCE.H"
#include "TYPES.H"
#include "FX_SCENE.H"

#define Motion_SetTargetPositionFromMagnitudeAngle \
	((void (*)(struct BattleEffect03Object *, s32, s32))Motion_SetTargetPositionFromMagnitudeAngle)

#define BattleFx_UpdateShrinkingOrbitObject ((void (*)(void))BattleFx_UpdateShrinkingOrbitObject)
#define BattleFx_RunSparkEmitter ((void (*)(void))BattleFx_RunSparkEmitter)
#define Object_ReplaceResourceEntry                                         \
	((struct BattleEffect03Link * (*)(void *, struct BattleEffect03Link *)) \
			Object_ReplaceResourceEntry)

//--------------------------------------------------------------------------
// 演出 03 を流し、いまのオブジェクトを止める
//--------------------------------------------------------------------------
void BattleFx_CallEffect03AndStop(void)
{
	RunBattleEffect03();
	EffectRuntime_StopCurrentObject();
}

struct BattleEffect03Object
{
	u8	  reserved_00[8];
	s32	  x;
	s32	  y;
	s32	  z;
	u8	  reserved_14[4];
	s32	  scale_x;
	s32	  scale_y;
	u8	  reserved_20[16];
	s32	  velocity_x;
	s32	  velocity_y;
	u8	  reserved_38[24];
	void *visual;
	u8	  reserved_54;
	u8	  mode;
	u8	  reserved_56[4];
	u8	  unknown_5a;
	u8	  reserved_5b[9];
	s16	  angle; // 渦の粒では残りフレーム（BattleFx_UpdateShrinkingOrbitObject）
	s16	  phase; // 渦の粒では最初の角度
	u8	  reserved_68[4];
	void *callback;
};

struct BattleEffect03Link
{
	u8 reserved_00[28];
	u8 marker;
};

extern struct BattleFxScene *gEffectWork;

struct BattleEffect03Object *Object_Spawn(s32, s32, s32, s32);
void						 WaitFrames(s32);
void						 Audio_PlayCue(s32);
void						 Animation_ApplyChildValuesFar(struct BattleEffect03Object *, s32);
void						 Object_CommitPosition(struct BattleEffect03Object *);
void						 ObjectDispatch_ReleaseFar(struct BattleEffect03Object *);

//--------------------------------------------------------------------------
// 演出 03
// 8 個の粒が渦を巻いて中心へ集まり、中心に現れた 1 個が火花を出してから
// 遠くへ飛び去る
//--------------------------------------------------------------------------
void RunBattleEffect03(void)
{
	struct BattleFxScene		*scn = gEffectWork;
	struct BattleEffect03Object *target = scn->main_object;
	struct BattleEffect03Object *object;
	struct BattleEffect03Link	*last;
	u8							 marker;
	s32							 i;

	BattleEffect_InitializeSharedScene();

	// 渦の粒：1 フレームに 1 個ずつ、1/8 周ずらして出す
	last = 0;
	for (i = 0; i < 8; i++)
	{
		object = Object_Spawn(233, target->x, target->y + 0x200000, target->z);
		if (object != 0)
		{
			object->scale_y = 0xb333; // 0.7 倍
			object->scale_x = 0xb333;
			object->callback = BattleFx_UpdateShrinkingOrbitObject;
			object->angle = 120;
			object->phase = i << 13;
			object->mode = 4;
			last = Object_ReplaceResourceEntry(object->visual, last);
		}
		WaitFrames(1);
	}

	marker = last->marker;
	Audio_PlayCue(130);
	WaitFrames(110);

	// 中心の 1 個
	target = Object_Spawn(233, 0, 0, 0);
	/* FAKEMATCH: the ROM keeps object as a second copy of the new spark (r5 beside r6); a plain object = target is copy-propagated away by gcse. */
	asm("" : "=r"(object) : "0"(target));
	if (target != 0)
	{
		target->scale_y = 0xb333;
		target->scale_x = 0xb333;
		target->x = scn->x;
		target->y = scn->y + 0x100000;
		target->z = scn->z;
		target->mode = 4;
		Animation_ApplyChildValuesFar(target, 7);
	}

	Audio_PlayCue(131);
	WaitFrames(12);
	if (target != 0)
	{
		// 2 フレームごとに子の値を切り替える（4 回に 1 回は 10）
		for (i = 0; i < 30; i++)
		{
			if (i & 3)
				Animation_ApplyChildValuesFar(object, 9);
			else
				Animation_ApplyChildValuesFar(object, 10);
			WaitFrames(2);
		}
	}

	// 火花
	Animation_ApplyChildValuesFar(object, 0);
	Audio_PlayCue(84);
	if (object != 0)
	{
		target->callback = BattleFx_RunSparkEmitter;
		target->angle = 0;
		if (scn->enabled != 0)
			WaitFrames(128);
		else
			WaitFrames(192);
	}

	// 飛び去る
	if (target != 0)
	{
		target->angle = -1;
		target->velocity_x = 0x50000;
		target->velocity_y = 0x6666;
		target->unknown_5a = 0;
		Motion_SetTargetPositionFromMagnitudeAngle(target, 0xc00000, 0xe800);
		Object_CommitPosition(target);
		ObjectDispatch_ReleaseFar(target);
	}
	if (marker != 0x60)
		Resource_ResetEntry(marker);
	if (scn->finish_callback != 0)
		scn->finish_callback();
	BattleFx_PrepareBufferInterpolation();
}
