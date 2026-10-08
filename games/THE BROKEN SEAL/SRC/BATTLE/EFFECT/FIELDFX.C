//--------------------------------------------------------------------------
// FIELDFX.C
// フィールド効果：方向キーで対象を押す処理、出現と消滅の粒、範囲の見張り
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
#include "MOTOBJ.H"
#include "FX_SCENE.H"
#include "FIXED_MATH.H"
#include "OBJDISP.H"
#include "EFFECT_SLOT.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "ANIMSPR.H"
#include "GAME_STATE.H"
#include "OBJECT_EFX.H"
#include "SOUND_IDS.H"
#include "GLOBAL_CELLS.H"
#include "IWRAM_CALL.H"
#include "IO_REG.H"

#define gKeyState (*(volatile u32 *)&gKeyState)

#define Motion_SetTargetPositionFromMagnitudeAngle \
	((void (*)(void *, s32, s32))Motion_SetTargetPositionFromMagnitudeAngle)

#define BattleFx_UpdateItemBreakFragment ((void (*)(void *))BattleFx_UpdateItemBreakFragment)
#define Object_SetMoveTargetFar \
	((void (*)(struct FxObject *, s32, s32, s32))Object_SetMoveTargetFar)
#define ObjectGroup_ApplyRandomChildValues ((void (*)(void))ObjectGroup_ApplyRandomChildValues)
#define BattleFx_GetCycledTableWord ((s32 (*)(u32))BattleFx_GetCycledTableWord)

#define gObjectSlots ((u8 *)&gObjectSlots)

#define CELL_MASK 0xfff00000 // 16 ドットのマス

struct Output_08097f80
{
	s32 x;
	s32 y;
	s32 z;
};

void Vector_AddPolarOffset(s32, s32, struct Output_08097f80 *);

extern struct BattleFxScene *gEffectWork;
void						 Object_SetMode(void *object, s32 mode);

extern void *Object_Spawn(s32, s32, s32, s32);
extern void	 Object_SetMode(void *, s32);

extern void Audio_PlayCue(s32);
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
s32 Object_CommitPosition();

extern void ObjectDispatch_ReleaseFar(void *object);

extern u8 *gEventWork;

struct FxObject
{
	u8				 unknown_00[8];
	s32				 x;
	s32				 y;
	s32				 z;
	s32				 ground;
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
	u8				 unknown_5a; // bit 0 は押した後に操作キャラで立てる
	u8				 unknown_5b;
	u8				 unknown_5c[12];
	struct FxObject *linked;
	void			*callback;
};

extern volatile u32 gKeysHeld;
extern volatile u32 gFrameCount;

void AudioCommand_PlayFar(s32 cue);
s32	 Object_CheckMovementCollision(struct FxObject *object, struct Output_08097f80 *position);
struct FxObject *Object_GetById(s32 id);
void			 Animation_ApplyChildValuesFar(struct FxObject *object, s32 value);

//--------------------------------------------------------------------------
// 対象のそばに押すオブジェクトを出し、方向キーの向きへ対象を 16 ドット押す
// A, B, L, R で取りやめる。押せないときは 16 フレームごとにエラー音
// 押す先が使い手と重なるときは、使い手の位置しだいで押せないか、使い手も動かす
//--------------------------------------------------------------------------
void BattleFx_SteerPushedTarget(s32 target_id)
{
	struct Output_08097f80	position;
	struct Output_08097f80	beyond;
	struct BattleFxScene   *work;
	struct FxObject		   *user;
	struct FxObject		   *target;
	struct FxObject		   *pusher;
	struct FxObject		   *hit;
	s32						x;
	s32						z;
	s32						angle;
	s32						moved;
	s32						dir;
	s32						keys;
	s32						cell_x;
	s32						cell_z;
	struct Output_08097f80 *p;

	work = gEffectWork;
	user = work->main_object;
	target = work->child;
	angle = work->angle + 0x8000; // work->angle の反対向き
	moved = 0;
	if (target == NULL)
		return;
	BattleEffect_InitializeSharedScene();
	user->linked = target;
	ObjectDispatch_InitializeFar((struct DispatchObject *)user, BattleFx_SourceHoldScript);
	pusher = BattleFx_StartItemBreak(user);
	if (pusher == NULL)
	{
		BattleFx_PrepareBufferInterpolation();
		return;
	}

	// 押すオブジェクトは対象の 16 ドット上、angle の向きへ 16 ドットの位置
	pusher->linked = target;
	p = &position;
	p->x = target->x;
	p->y = target->y + (16 << 16);
	p->z = target->z;
	Vector_AddPolarOffset(16 << 16, angle, p);
	Object_SetMoveTargetFar(pusher, p->x, p->y, p->z);
	BattleFx_SnapScaleToFull((struct MotionObject *)pusher);
	pusher->speed = 4 << 16;
	pusher->lift = 0x8000;
	pusher->flag = 4;
	target->callback = ObjectGroup_ApplyRandomChildValues; // 押す間は色を巡らせる
	target->speed = 0x6666;
	target->lift = 0x3333;
	target->unknown_5a = moved;
	target->mode = 2;
	for (;;)
	{
		WaitFrames(1);
		keys = gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS);
		if (keys != 0)
			break;

		// 方向キーが離れていれば押すオブジェクトを元の位置へ戻す
		dir = (u16)BattleFx_GetCycledTableWord(gKeysHeld);
		if (dir == 0xffff)
		{
			position.x = target->x;
			position.y = target->y + (16 << 16);
			position.z = target->z;
			Vector_AddPolarOffset(16 << 16, angle, &position);
			Object_SetMoveTargetFar(pusher, position.x, position.y, position.z);
			Object_SetMode(pusher, 1);
			pusher->velocity_x = 0;
			pusher->velocity_y = 0;
			pusher->velocity_z = 0;
			continue;
		}

		// 押すオブジェクトを押す向きへ 2 ドット寄せ、対象の 16 ドット先と 32 ドット先を調べる
		position.x = target->x;
		position.y = target->y + (16 << 16);
		position.z = target->z;
		Vector_AddPolarOffset(16 << 16, angle, &position);
		Vector_AddPolarOffset(2 << 16, dir, &position);
		Object_SetMoveTargetFar(pusher, position.x, position.y, position.z);
		Object_CommitPosition(pusher);
		position.x = target->x;
		position.y = target->y;
		position.z = target->z;
		Vector_AddPolarOffset(16 << 16, dir, &position);
		beyond.x = target->x;
		beyond.y = target->y;
		beyond.z = target->z;
		Vector_AddPolarOffset(32 << 16, dir, &beyond);
		if (Object_CheckMovementCollision(target, &position) > 0)
		{
			Object_SetMode(pusher, 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}
		hit = ScriptObject_FindOverlappingEntryFar(target, p);
		if (hit == NULL)
			goto move;
		if (hit != user)
		{
			Object_SetMode(pusher, 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}

		// 押す先が使い手と重なる：使い手のマスが押す先なら押せない。使い手が 32 ドット先の
		// マスにいれば、そこが通れるときだけ使い手もそこへ動かす
		cell_x = user->x & CELL_MASK;
		cell_z = user->z & CELL_MASK;
		if (cell_x == (p->x & CELL_MASK) && cell_z == (p->z & CELL_MASK))
		{
			Object_SetMode(pusher, 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}
		if (cell_x != (beyond.x & CELL_MASK) || cell_z != (beyond.z & CELL_MASK))
			goto move;
		if (Map_GetCellAttributeLowNibbleFar(user->mode, beyond.x, beyond.z) != 0)
		{
			Object_SetMode(pusher, 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}
		moved = 1;

		// 押す：押すオブジェクトの動作は angle と押す向きの差 (90° 単位) で選ぶ
	move:
		AudioCommand_PlayFar(175);
		x = position.x;
		z = position.z;
		Object_SetMode(pusher, BattleFx_QuadrantModes[(u16)(angle - dir) >> 14]);
		WaitFrames(15);
		target->unknown_5b = 0;
		target->speed = 0x3333;
		target->lift = 0x3333;
		Object_SetMoveTargetFar(target, position.x, position.y, position.z);
		pusher->flag = 0;
		pusher->speed = 0x3333;
		pusher->lift = 0x3333;
		Vector_AddPolarOffset(16 << 16, dir, &position);
		Object_SetMoveTargetFar(pusher, position.x, position.y + (16 << 16), position.z);
		if (moved == 1)
		{
			Object_GetById(work->first_object_id)->unknown_5a &= 0xfe;
			user->speed = 0x3333;
			user->lift = 0x3333;
			Object_SetMoveTargetFar(user, beyond.x, beyond.y, beyond.z);
		}
		Object_CommitPosition(target);
		target->x = x;
		target->z = z;
		target->velocity_x = 0;
		target->velocity_z = 0;
		break;
	}

	// 対象の色、スクリプト、毎フレームの処理を元に戻し、押すオブジェクトを消す
	Animation_ApplyChildValuesFar(target, work->saved_palette);
	ObjectDispatch_InitializeFar((struct DispatchObject *)target, work->saved_script);
	target->callback = work->saved_callback;
	EffectRuntime_StopCurrentObject();
	if (moved == 1)
		Object_GetById(work->first_object_id)->unknown_5a |= 1;
	BattleFx_PrepareBufferInterpolation();
	UpdateRisingParticleBurst(pusher);
}

//--------------------------------------------------------------------------
// 粒の動き：元の位置から 30 ドット先へ飛び、また元の位置へ戻って消える
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitAndReturn(struct EffectSlot *slot)
{
	struct Output_08097f80 position;
	s8					  *state_ptr = &slot->state;
	s32					   state;

next_state:
	state = *state_ptr;
	if (state == 0)
	{
		u32 angle;

		position.x = slot->origin_x;
		position.z = slot->origin_z;
		angle = Random16();
		Vector_AddPolarOffset(30 << 16, (u16)angle, &position);
		slot->target_x = position.x;
		slot->target_z = position.z;
		slot->acceleration = 4 << 16;
		slot->max_speed = 4 << 16;
		slot->flag42 = state;
		(*state_ptr)++;
		return;
	}

	if (state == 1)
	{
		if (BattleFx_HasReachedTarget(slot) == 0)
		{
			(*state_ptr)++;
			goto next_state;
		}
		return;
	}

	if (state == 2)
	{
		slot->target_x = slot->origin_x;
		slot->target_z = slot->origin_z;
		{
			u32 value = 0x400;

			slot->max_turn_step = value;
		}
		slot->flag42 = 1;
		(*state_ptr)++;
		return;
	}

	if (state == 3 && BattleFx_HasReachedTarget(slot) == 0)
		BattleFx_ClearOwnedSlot(slot);
}

//--------------------------------------------------------------------------
// 使い手のそばにオブジェクトを出し、30 フレーム見せてから粒にして消す
//--------------------------------------------------------------------------
void BattleFx_RunItemBreakSequence(void)
{
	s32					  work[3];
	struct BattleFxScene *scene;
	void				 *object;

	scene = gEffectWork;
	object = scene->main_object;

	/* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
	do
	{
		BattleEffect_InitializeSharedScene();
	} while (0);
	object = BattleFx_StartItemBreak(object);
	BattleFx_SnapScaleToFull(object);
	if (object != 0)
	{
		Object_SetMode(object, 4);
		WaitFrames(30);
	}
	BattleFx_PrepareBufferInterpolation();
	UpdateRisingParticleBurst(object);
}

//--------------------------------------------------------------------------
// object の 16 ドット上にオブジェクト 215 を出し、向いている側
// (90° 単位) へ 16 ドット動かす。周りに粒 8 個を散らす
// 戻り値：出したオブジェクト (出せなければ 0)
//--------------------------------------------------------------------------
void *BattleFx_StartItemBreak(void *object)
{
	struct MotionObject *src = object;
	struct MotionObject *parent;
	struct MotionObject *child;
	s32					 angle;
	s32					 dist;
	s32					 speed;
	s32					 i;
	u32					 spread;
	u32					 jitter;

	angle = (src->angle + 0x2000) & 0xc000; // いちばん近い 90° の向き
	parent = Object_Spawn(215, src->x, src->y + (16 << 16), src->z);
	if (parent == 0)
		return 0;
	parent->scale_y = 0x4000; // 0.25 倍から
	parent->scale_x = 0x4000;
	parent->update = BattleFx_UpdateItemBreakFragment;
	parent->speed_limit = 2 << 16;
	parent->acceleration = 2 << 16;
	parent->motion_flags = 0;
	Object_SetMode(parent, 3);
	Motion_SetTargetPositionFromMagnitudeAngle(parent, 16 << 16, angle);

	// 粒：向きの ±45° へ 8〜32 ドット、速さ 1〜2 ドット
	for (i = 7; i >= 0; i--)
	{
		child = Object_Spawn(285, src->x, src->y + (16 << 16), src->z);
		if (child != 0)
		{
			ObjectDispatch_InitializeFar((struct DispatchObject *)child, BattleFx_FragmentScript);
			speed = Random16() + 0x10000;
			child->acceleration = 1 << 16;
			child->speed_limit = speed;
			child->motion_flags = 2;
			child->vertical_motion_strength = 0x51e;
			spread = Random16();
			child->velocity_y = spread - Random16();
			dist = Random16() * 24 + (8 << 16);
			jitter = Random16();
			Motion_SetTargetPositionFromMagnitudeAngle(
				child, dist, ((jitter - Random16()) >> 3) + src->angle);
		}
	}
	Audio_PlayCue(SOUND_ITEM_BREAK);
	return parent;
}

//--------------------------------------------------------------------------
// 拡大率を 1/16 刻みで等倍まで上げ、位置を確定する
//--------------------------------------------------------------------------
void BattleFx_SnapScaleToFull(struct MotionObject *obj)
{
	s32 next;
	s32 scale;

	if (obj != NULL)
	{
		scale = obj->scale_x;
		if (scale <= 0xffff)
		{
			do
			{
				next = scale + 0x1000;
				scale = next;
			} while (next <= 0xffff);
			obj->scale_x = next;
			obj->scale_y = next;
		}
		Object_CommitPosition();
	}
}

//--------------------------------------------------------------------------
// 31 フレームかけて回りながら上り、縮んでから粒 8 個になって消える
//--------------------------------------------------------------------------
void UpdateRisingParticleBurst(void *object)
{
	struct MotionObject *src = object;
	struct MotionObject *child;
	s32					 i;
	s32					 dist;
	u32					 spread;
	/* FAKEMATCH: holds 0x10000 in sl across the burst loop; a plain local folds to the constant. */
	register s32 unit asm("sl");

	Audio_PlayCue(154);
	for (i = 30; i >= 0; i--)
	{
		src->y += 1 << 16;
		src->angle += 0x2000; // 45°
		src->scale_x += -0x800;
		src->scale_y += -0x800;
		WaitFrames(1);
	}
	i = 7;
	unit = 0x10000;
	for (; i >= 0; i--)
	{
		child = Object_Spawn(285, src->x, src->y, src->z);
		if (child != 0)
		{
			s32 speed;

			ObjectDispatch_InitializeFar((struct DispatchObject *)child, BattleFx_FragmentScript);
			speed = Random16();
			child->acceleration = unit;
			child->speed_limit = speed + unit;
			child->motion_flags = 2;
			child->vertical_motion_strength = 0xa3d;
			spread = Random16();
			child->velocity_y = spread - Random16();
			dist = Random16() * 24;
			dist += 8 << 16;
			Motion_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
		}
	}
	Audio_PlayCue(131);
	ObjectDispatch_ReleaseFar(src);
}

//--------------------------------------------------------------------------
// 64 個のオブジェクトのうち、最初のパーツが絵 200 のものへ value を渡す
//--------------------------------------------------------------------------
void ObjectDispatch_ApplyValueToKind200Children(s32 value)
{
	struct ObjectRuntime *object = *(struct ObjectRuntime **)gObjectSlots;
	s32					  cnt = 63;

	do
	{
		if (object->script != NULL)
		{
			u8 kind = object->animation_kind;

			if (kind == 1)
			{
				struct AnimationObject *anim = object->animation;
				struct AnimationEntry  *entry = anim->entries[0];

				if (entry->anim_id == 200)
				{
					entry->param = value;
					anim->dirty = kind;
				}
			}
		}
		cnt--;
		object++;
	} while (cnt >= 0);
}

//--------------------------------------------------------------------------
// 効果の中心から操作キャラが 60 ドット離れるか、残り時間が尽きたら終了を頼む
// (中心と時間は RunBattleEffect08 が gEventWork の 0xcb8〜 に置く)
//--------------------------------------------------------------------------
void FieldEffect_WatchLeaderDistance(void)
{
	u8					 *work;
	struct ObjectRuntime *actor;
	s16					 *timer;
	s32					  dx;
	s32					  dz;

	work = gEventWork;
	actor = ObjectTable_Get(gGameState.selected_actor);
	if (*(s16 *)(work + 0xcc0) != 0) // 時間を減らすか
	{
		timer = (s16 *)(work + 0xcba); // 残りフレーム
		if (*timer != 0)
			(*timer)--;
	}

	// x は 0xd105 (約 0.82) 倍して比べる
	dx = Iwram_MulQ16(*(s16 *)(work + 0xcbc) - actor->x / 0x10000, 0xd105);
	dz = *(s16 *)(work + 0xcbe) - (actor->z - actor->y) / 0x10000;
	if (dx * dx + dz * dz >= 3600 || *(s16 *)(work + 0xcba) == 0)
	{
		u16 *slot = (u16 *)(work + 382); // psynergy_request
		u32	 request = 0x2090;

		*slot = request;
	}
}
