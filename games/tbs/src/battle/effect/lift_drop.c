//--------------------------------------------------------------------------
// lift_drop.c
// 対象を 2 つの持ち手で持ち上げ、方向キーで動かして落とす
//--------------------------------------------------------------------------

#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "fx_scene.h"
#include "objdisp.h"
#include "callback_scheduler.h"
#include "sound_ids.h"


#define Random16 ((s32 (*)(void))Random16)

#define Motion_SetTargetPositionFromMagnitudeAngle \
	((void (*)(struct FxObject *, s32, s32))Motion_SetTargetPositionFromMagnitudeAngle)

#define BattleFx_SpawnItemBreakMode1 \
	((struct FxObject * (*)(s32, s32, s32, s32)) BattleFx_SpawnItemBreakMode1)
#define Object_SetMoveTargetFar \
	((void (*)(struct FxObject *, s32, s32, s32))Object_SetMoveTargetFar)
#define ObjectGroup_ApplyRandomChildValues ((void (*)(void))ObjectGroup_ApplyRandomChildValues)
#define BattleFx_GetCycledTableWord ((s32 (*)(u32))BattleFx_GetCycledTableWord)
#define ScriptObject_CheckOverlapFar \
	((s32 (*)(struct FxObject *, struct FxPosition *))ScriptObject_CheckOverlapFar)
#define BattleFx_SpawnRadialParticleRing \
	((void (*)(struct FxObject *))BattleFx_SpawnRadialParticleRing)

struct FxPosition
{
	s32 x; // 16.16 固定小数
	s32 y;
	s32 z;
};

struct FxObject
{
	u8				 unknown_00[8];
	s32				 x; // 座標 (16.16 固定小数、y が高さ)
	s32				 y;
	s32				 z;
	s32				 ground; // 足もとの地面の高さ
	u8				 unknown_18[10];
	u8				 mode;
	u8				 unknown_23;
	s32				 velocity_x;
	s32				 velocity_y;
	s32				 velocity_z;
	s32				 speed;
	s32				 lift;
	u8				 unknown_38[12];
	s32				 gravity;
	u8				 unknown_48[13];
	u8				 flag;
	u8				 unknown_56[4];
	u8				 unknown_5a;
	u8				 unknown_5b;
	u8				 unknown_5c[12];
	struct FxObject *linked;
	void			*callback;
};

extern struct BattleFxScene *gEffectWork;
extern const s32			 BattleFx_FragmentScript[];
extern volatile u32			 gFrameCount;

void			 WaitFrames(s32 frames);
void			 Script_WaitForEventTimeoutFar(struct FxObject *object);
void			 AudioCommand_PlayFar(s32 cue);
void			 ObjectDispatch_SetSingleChildFlagsFar(struct FxObject *object, s32 value);
void			 ObjectDispatch_ApplyArgumentToChildrenFar(struct FxObject *object, s32 mode);
void			 Vector_AddPolarOffset(s32 magnitude, s32 angle, struct FxPosition *position);
s32				 Map_CheckObjectStepFar(struct FxObject *object, struct FxPosition *position);
struct FxObject *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void			 ObjectDispatch_ReleaseFar(struct FxObject *object);
void			 Animation_ApplyChildValuesFar(struct FxObject *object, s32 value);

#define SHARD_MAX 20 // 破片の数

//--------------------------------------------------------------------------
// 足もとを 16 ドット上げて、pos へ動いたときにぶつかるかを調べる
//--------------------------------------------------------------------------
static __inline__ s32 ObjProbeUp(struct FxObject *obj, struct FxPosition *pos)
{
	s32 hit;

	obj->ground += 16 << 16;
	hit = Map_CheckObjectStepFar(obj, pos);
	obj->ground -= 16 << 16;
	return hit;
}

//--------------------------------------------------------------------------
// 対象を左右の持ち手で 24 ドットの高さまで持ち上げ、方向キーの向きへ
// 16 ドット動かして落とす (A・B・L・R ならその場で落とす)
// 行き先がふさがっていれば、ブザーを鳴らして待つ
//--------------------------------------------------------------------------
void BattleFx_SteerLiftedTarget(s32 id)
{
	struct BattleFxScene *work;
	struct FxObject		 *src;
	struct FxObject		 *target;
	struct FxObject		 *shards[SHARD_MAX];
	struct FxPosition	  pos;
	struct FxObject		 *hands[2];
	struct FxObject		 *obj;
	struct FxObject		 *right;
	s32					  i;
	s32					  angle;
	s32					  dist;
	struct FxPosition	 *p;

	work = gEffectWork;
	src = work->main_object;
	target = work->child;
	if (target == NULL)
		return;
	BattleEffect_InitializeSharedScene();
	src->linked = target;
	ObjectDispatch_InitializeFar((struct DispatchObject *)src, BattleFx_SourceHoldScript);

	// 持ち手を左右 32 ドットに出し、対象の左右 16 ドットへ動かす
	/* FAKEMATCH: needed to match: the position is reached through this pointer everywhere but in the steering loop's first part, which names the position itself */
	p = &pos;
	p->x = work->x;
	p->y = work->y + (16 << 16);
	p->z = work->z;
	obj = BattleFx_SpawnItemBreakMode1(p->x + (32 << 16), p->y, p->z, 0x8000);
	hands[0] = obj;
	right = BattleFx_SpawnItemBreakMode1(p->x - (32 << 16), p->y, p->z, 0);
	hands[1] = right;
	if (hands[0] == NULL || right == NULL)
	{
		BattleFx_PrepareBufferInterpolation();
		return;
	}
	WaitFrames(15);
	p->x = target->x;
	p->y = target->y + (16 << 16);
	p->z = target->z;
	Object_SetMoveTargetFar(obj, p->x + (16 << 16), p->y, p->z);
	Object_SetMoveTargetFar(right, p->x - (16 << 16), p->y, p->z);
	Script_WaitForEventTimeoutFar(obj);
	Script_WaitForEventTimeoutFar(right);
	obj->x = p->x + (16 << 16);
	obj->velocity_x = 0;
	right->x = p->x - (16 << 16);
	right->velocity_x = 0;

	// 地面から 24 ドットの高さまで持ち上げる
	target->callback = ObjectGroup_ApplyRandomChildValues;
	Scheduler_AddOrUpdateCallback(BattleFx_SpawnFallingParticles, 0xc80);
	AudioCommand_PlayFar(130);
	target->flag = 4;
	ObjectDispatch_SetSingleChildFlagsFar(target, 0);
	if (hands[0] != NULL && hands[1] != NULL)
	{
		while (target->y - target->ground <= 24 << 16)
		{
			obj->y += 0x6000;
			right->y += 0x6000;
			target->y += 0x6000;
			WaitFrames(1);
		}
	}
	hands[0]->speed = 4 << 16;
	hands[0]->lift = 0x8000;
	hands[1]->speed = 4 << 16;
	hands[1]->lift = 0x8000;
	target->speed = 0x6666;
	target->lift = 0x3333;
	target->unknown_5a = 0;
	target->mode = 2;

	//------------------------------------------------------------------
	// 方向キーで動かす
	//------------------------------------------------------------------
	for (;;)
	{
		WaitFrames(1);
		if ((gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) != 0)
			break;

		// 方向キーなし : 持ち手を対象の左右に戻す
		angle = (u16)BattleFx_GetCycledTableWord(gKeysHeld);
		if (angle == 0xffff)
		{
			pos.x = target->x;
			pos.y = target->y + (16 << 16);
			pos.z = target->z;
			Object_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
			Object_SetMoveTargetFar(hands[1], pos.x - (16 << 16), pos.y, pos.z);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 1);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 1);
			continue;
		}

		// 持ち手をその向きへ 2 ドット寄せ、16 ドット先の地面を調べる
		pos.x = target->x;
		pos.y = target->y + (16 << 16);
		pos.z = target->z;
		Vector_AddPolarOffset(2 << 16, angle, &pos);
		Object_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
		Object_SetMoveTargetFar(hands[1], pos.x - (16 << 16), pos.y, pos.z);
		Script_WaitForEventTimeoutFar(hands[0]);
		Script_WaitForEventTimeoutFar(hands[1]);
		pos.x = target->x;
		pos.y = target->ground;
		pos.z = target->z;
		Vector_AddPolarOffset(16 << 16, angle, &pos);
		i = ScriptObject_CheckOverlapFar(target, &pos);
		/* FAKEMATCH: needed to match: entering the blocked path from both tests keeps it ahead of the drop path, as the ROM lays them out */
		if (i != 0)
			goto blocked;
		if (ObjProbeUp(target, p) > 0)
		{
		blocked: // ふさがっている : 16 フレームごとにブザー
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}
		// 空いている : 対象をそこへ動かす
		{
			struct FxObject *second;
			s32				 x;
			s32				 z;

			AudioCommand_PlayFar(175);
			x = p->x;
			z = p->z;
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
			WaitFrames(15);
			target->unknown_5b = i;
			target->speed = 0x3333;
			target->lift = 0x3333;
			Object_SetMoveTargetFar(target, p->x, p->y, p->z);
			hands[0]->speed = 0x3333;
			hands[0]->lift = 0x3333;
			second = hands[1];
			second->speed = 0x3333;
			second->lift = 0x3333;
			Object_SetMoveTargetFar(hands[0], p->x + (16 << 16), p->y, p->z);
			Object_SetMoveTargetFar(second, p->x - (16 << 16), p->y, p->z);
			Script_WaitForEventTimeoutFar(target);
			target->x = x;
			target->z = z;
			target->velocity_x = i;
			target->velocity_z = i;
			WaitFrames(10);
			break;
		}
	}

	//------------------------------------------------------------------
	// 対象の位置から破片を飛ばして持ち手を消し、対象を跳ねさせて落とす
	//------------------------------------------------------------------
	ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
	ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
	Scheduler_RemoveCallback(BattleFx_SpawnFallingParticles);
	AudioCommand_PlayFar(135);
	WaitFrames(15);
	AudioCommand_PlayFar(135);
	WaitFrames(15);
	p->x = target->x;
	p->y = target->y + (16 << 16);
	p->z = target->z;
	{
		struct FxObject **walk = shards;

		// 破片 : 速さ 2〜3 ドット、8〜32 ドット先のでたらめな方向へ
		for (i = 0; i < SHARD_MAX; i++)
		{
			obj = Object_Spawn(0x11d, p->x, p->y, p->z);
			*walk++ = obj;
			if (obj != NULL)
			{
				ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_FragmentScript);
				obj->speed = Random16() + (2 << 16);
				obj->lift = 2 << 16;
				obj->flag = 0;
				dist = Random16() * 24 + (8 << 16);
				Motion_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());
			}
		}
	}
	AudioCommand_PlayFar(131);
	ObjectDispatch_ReleaseFar(hands[0]);
	ObjectDispatch_ReleaseFar(hands[1]);

	// 対象の見た目と動きを戻し、上へ 10 ドットの速さで跳ねさせる
	Animation_ApplyChildValuesFar(target, work->saved_palette);
	ObjectDispatch_InitializeFar((struct DispatchObject *)target, work->saved_script);
	target->callback = work->saved_callback;
	target->flag = 3;
	target->velocity_y = 10 << 16;
	target->gravity = 0x3333;
	target->mode = 0;
	src->callback = 0;
	Animation_ApplyChildValuesFar(src, 0);
	if (work->child_option != 0)
	{
		// 上がりきって落ちきるまで (それぞれ最長 90 フレーム) 待ち、輪を出す
		for (i = 0; i < 90 && target->velocity_y >= 0; i++)
			WaitFrames(1);
		WaitFrames(1);
		for (i = 0; i < 90 && target->velocity_y < 0; i++)
			WaitFrames(1);
		BattleFx_SpawnRadialParticleRing(target);
		BattleFx_PrepareBufferInterpolation();
		WaitFrames(30);
	}
	else
	{
		BattleFx_PrepareBufferInterpolation();
	}
}
