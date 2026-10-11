//--------------------------------------------------------------------------
// run_effect13_hook.c
// エナジー演出 13：オブジェクト 215 を目標まで伸ばし、押し下げて戻す
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
#include "fx_scene.h"
#include "motobj.h"
#include "fixed_point_position.h"
#include "field_event.h"
#include "menu_list.h"
#include "scene.h"
#include "fixed_math.h"
#include "effect_slot.h"
#include "fieldobj.h"
#include "object_lookup.h"
#include "resource.h"
#include "vram_block.h"
#include "scene_ids.h"
#include "game_state.h"
#include "animspr.h"
#include "text/msg_ids.h"
#include "callback_scheduler.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "btlsys.h"
#include "mem.h"
#include "battle_effect_runtime.h"
#include "ui.h"
#include "edition.h"
#include "io_write_queue.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "resource_ids.h"
#include "global_cells.h"
#include "map.h"
#include "event_runtime.h"
#include "object_runtime.h"
#include "heap_state.h"

#define Object_SetPositionAndResetMotionFar \
	((void (*)(struct MotionObject *, s32, s32, s32))Object_SetPositionAndResetMotionFar)

extern struct BattleFxScene *gEffectWork;
struct MotionObject			*Object_Spawn(s32, s32, s32, s32);
void						 ObjectDispatch_ReleaseFar(struct MotionObject *);

//--------------------------------------------------------------------------
// 対象への演出 (BattleFx_DispatchRequestKind から)
//--------------------------------------------------------------------------
void BattleFx_RunEffect13Hook(void)
{
	RunBattleEffect13();
}

//--------------------------------------------------------------------------
// オブジェクト 215 を使う者の 16 ドット上から目標へ 10 段で運びながら
// 1/4 倍から等倍にし、20 ドット押し下げて child を (-9, -9) へ移し、
// 19.5 ドット上げてから縮めて戻す
// 目標は scene->enabled なら使う者の前 32 ドット、ほかは scene の位置で、
// どちらも 32 ドット上
//--------------------------------------------------------------------------
void RunBattleEffect13(void)
{
	void Animation_ApplyChildValuesFar(struct MotionObject *, u32);
	void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor * object, s32 animation);
	void Vector_AddPolarOffset(s32, s32, s32 *);
	struct BattleFxScene	 *scene = gEffectWork;
	struct MotionObject		 *actor = scene->main_object;
	struct MotionObject		 *child = scene->child;
	struct MotionObject		 *object;
	struct FixedPointPosition spawn;
	struct FixedPointPosition origin;
	struct FixedPointPosition target;
	s32						  step;
	s32						  i;
	s32						  steps = 11;

	origin.x = actor->x;
	origin.y = actor->y + (16 << 16);
	origin.z = actor->z;
	if (scene->enabled)
	{
		target.x = actor->x;
		target.y = actor->y + (32 << 16);
		target.z = actor->z;
		Vector_AddPolarOffset(32 << 16, scene->angle, &target.x);
	}
	else
	{
		target.x = scene->x;
		target.y = scene->y + (32 << 16);
		target.z = scene->z;
	}
	spawn.x = scene->x;
	spawn.y = scene->y + (32 << 16);
	spawn.z = scene->z;
	object = Object_Spawn(215, spawn.x, spawn.y, spawn.z);
	if (object == 0)
		return;
	BattleEffect_InitializeSharedScene();
	AudioCommand_PlayFar(138);
	object->angle = actor->angle;
	object->speed_limit = 0x14ccc;
	object->motion_flags = 0;
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
	Animation_ApplyChildValuesFar(object, 1);

	// 目標へ伸ばす
	step = 0;
	do
	{
		s32 scale;

		object->x = origin.x + (target.x - origin.x) * step / 10;
		object->y = origin.y + (target.y - origin.y) * step / 10;
		object->z = origin.z + (target.z - origin.z) * step / 10;
		scale = BattleFx_Interpolate(0x10000, 0x4000, step);
		object->scale_x = scale;
		object->scale_y = scale;
		WaitFrames(1);
		step++;
	} while (step < steps);
	WaitFrames(10);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 6);
	WaitFrames(15);

	// 2 ドットずつ 10 フレーム押し下げる
	for (i = 0; i < 10; i++)
	{
		object->y -= 2 << 16;
		WaitFrames(1);
	}
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
	AudioCommand_PlayFar(132);
	if (child != 0)
		Object_SetPositionAndResetMotionFar(child, -(9 << 16), child->y, -(9 << 16));
	WaitFrames(20);

	// 1.5 ドットずつ 13 フレーム上げる
	for (i = 0; i < 13; i++)
	{
		object->y += 0x18000;
		WaitFrames(1);
	}
	WaitFrames(10);
	AudioCommand_PlayFar(114);

	// 使う者のところへ縮めて戻す
	step = 0;
	do
	{
		s32 scale;

		object->x = target.x + (origin.x - target.x) * step / 10;
		object->y = target.y + (origin.y - target.y) * step / 10;
		object->z = target.z + (origin.z - target.z) * step / 10;
		scale = BattleFx_Interpolate(0x4000, 0x10000, step);
		object->scale_x = scale;
		object->scale_y = scale;
		WaitFrames(1);
		step++;
	} while (step < steps);
	ObjectDispatch_ReleaseFar(object);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------

// launch.c

// 戦闘エフェクト 15 番：回って縮む主オブジェクトと、扇状に飛び出す粒

//--------------------------------------------------------------------------

#define Camera_WorldToScreen ((void (*)(struct Output *))Camera_WorldToScreen)
#define EffectSlot_Initialize ((void (*)(void *, s32, s32, s32))EffectSlot_Initialize)
#define ObjectGroup_SetChildValueUnlessFifteenFar \
	((void (*)(void *, s32))ObjectGroup_SetChildValueUnlessFifteenFar)
#define ObjectGroup_ApplyRandomChildValues ((void (*)(void))ObjectGroup_ApplyRandomChildValues)

#define SLOT_MAX 24 // エフェクト枠の数 (粒の数)

//--------------------------------------------------------------------------
// 毎フレームの更新 : 0x1000 + timer × 0x50 ずつ回し、timer × 0x50 が 0x1000 に
// 達するまで timer を増やして速める
//--------------------------------------------------------------------------
void BattleFx_UpdateSpinAngle(struct FieldActor *obj)
{
	u32 step;

	step = (s16)obj->timer * 0x50;
	obj->facing = obj->facing + step + 0x1000;
	if (step < 0x1000)
		obj->timer = obj->timer + 1;
}

//--------------------------------------------------------------------------
// 毎フレームの更新 : 45 度ずつ回りながら縮み、y を 1 ドットずつ増やす
// 拡大率が 0x3000 を下回ったら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateDescendingObject(struct FieldActor *obj)
{
	u32 scale;

	obj->scale_y -= 0x280;
	scale = obj->scale_x - 0x280;
	obj->facing += 0x2000;
	obj->y.fixed += 1 << 16;
	obj->scale_x = scale;
	if ((s32)scale < 0x3000)
		obj->active = 0;
}

struct Output
{
	s32 x; // 16.16 固定小数
	s32 y;
	s32 z;
};
#ifndef MODERN // modern GCC keeps the headers' volatile declaration
#endif
void AudioCommand_PlayFar(s32);

//--------------------------------------------------------------------------
// 粒の動き : 主オブジェクトの画面位置 (z + 8 ドット) から、0xc000 の向きの ±45 度へ
// 120 ドット飛ぶ。状態 1 は着いたら飛びなおし、状態 2 は着いたら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialLaunch(struct EffectSlot *slot)
{
	void			   Vector_AddPolarOffset(s32, s32, struct Output *);
	struct Output	   pos;
	struct FieldActor *src;
	s32				   state;
	u32				   r;

	src = gEffectWork->main_object;
	state = slot->state;

	if (state == 0)
	{
		pos.x = src->x.fixed;
		pos.y = src->y.fixed;
		pos.z = src->z.fixed;
		Camera_WorldToScreen(&pos);

		slot->x = pos.x;
		slot->z = pos.z + (8 << 16);
		slot->origin_z = slot->z;
		slot->origin_x = slot->x;
		pos.z = slot->origin_z;
		pos.x = slot->origin_x;

		r = Random16();
		Vector_AddPolarOffset(
			120 << 16, ((r << 13) >> 16) - ((Random16() << 13) >> 16) + 0xc000, &pos);

		slot->target_x = pos.x;
		slot->target_z = pos.z;
		slot->acceleration = 5 << 16;
		slot->max_speed = 5 << 16;
		slot->flag42 = state;
		slot->state++;

		if ((gFrameCount & 2) != 0)
			AudioCommand_PlayFar(246);
	}
	else if (state == 1)
	{
		if (BattleFx_HasReachedTarget(slot) == 0)
			slot->state--;
	}
	else if (state == 2)
	{
		if (BattleFx_HasReachedTarget(slot) == 0)
			BattleFx_ClearOwnedSlot(slot);
	}
}

void Battle_Reset(void);
void ObjectMotion_ArmCallback(s32, s32, s32);

//--------------------------------------------------------------------------
// 戦闘エフェクト 15 番
// 主オブジェクトを回して縮めながら、粒を 1 フレームに 1 つずつ 24 個飛ばす
//--------------------------------------------------------------------------
void BattleFx_RunEffect15(void)
{
	void				  Animation_ApplyChildValuesFar(void *, s32);
	void				  ObjectDispatch_ApplyArgumentToChildrenFar(void *, s32);
	void				  ObjectDispatch_SetSingleChildFlagsFar(void *, s32);
	struct BattleFxScene *scene = gEffectWork;
	struct FieldActor	 *obj = scene->main_object;
	struct Output		  pos;
	struct EffectSlot	 *slot;
	u8					  last;
	u32					  i;

	Battle_Reset();
	Motion_CamBounds(-1, -1, -1, 0);
	BattleEffect_InitializeSharedScene();
	WaitFrames(10);
	ObjectMotion_ArmCallback(scene->first_object_id, 0x4000, 0);
	WaitFrames(30);
	obj->update = (void (*)(union FieldObject *))ObjectGroup_ApplyRandomChildValues;
	AudioCommand_PlayFar(131);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 28);
	WaitFrames(40);

	// 回りはじめる
	AudioCommand_PlayFar(220);
	Animation_ApplyChildValuesFar(obj, 0);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 3);
	obj->update = (void (*)(union FieldObject *))BattleFx_UpdateSpinAngle;
	obj->timer = 0;
	WaitFrames(70);

	// 縮みはじめ、その画面位置から粒を飛ばす
	ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
	obj->motion_flags = 0;
	obj->update = (void (*)(union FieldObject *))BattleFx_UpdateDescendingObject;
	obj->target_x = ACTOR_NO_TARGET;
	pos.x = obj->x.fixed;
	pos.y = obj->y.fixed;
	pos.z = obj->z.fixed;
	Camera_WorldToScreen(&pos);
	i = 0;
	slot = scene->slots;
	do
	{
		s32 scale;
		EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
		EffectSlot_SetCallback(slot, BattleFx_UpdateRadialLaunch);
		EffectSlot_SetObjectMode(slot, 7);
		ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, (Random16() * 7) >> 16);
		scale = (Random16() >> 1) + 0x13333; // 1.2〜1.7 倍
		slot->scale_y = scale;
		slot->scale_x = scale;
		i++;
		WaitFrames(1);
		slot++;
	} while (i < SLOT_MAX);
	WaitFrames(70);

	// 飛んでいる粒を、着いたら消える状態にする
	i = 0;
	last = 2;
	do
	{
		if (scene->slots[i].active != 0)
			scene->slots[i].state = last;
		i++;
	} while (i < SLOT_MAX);
	WaitFrames(40);
	BattleFx_PrepareBufferInterpolation();
	WaitFrames(10);
}

struct ArcParticle
{
	u8					unknown_00[8];
	s32					x;
	s32					y;
	s32					z;
	s32					floor;
	s32					scale_x;
	s32					scale_y;
	u8					unknown_20[0x48];
	struct ArcParticle *source;
};

void BattleFx_UpdateDescendingParticlePositiveArc(struct ArcParticle *particle)
{
	struct ArcParticle *source;
	s32					y;
	s32					limit;
	s32					scale;

	limit = particle->floor + 0xA0000;
	source = particle->source;
	if (gGameState.map == (s32)&SceneId_Clear)
		limit = particle->floor + 0x40000;

	y = particle->y;
	if (y <= limit)
	{
		ObjectDispatch_ReleaseFar(particle);
		return;
	}

	scale = particle->scale_x + 0xC00;
	if (scale > 0x10000)
		scale = 0x10000;
	particle->scale_x = scale;
	particle->scale_y = scale;
	particle->x = source->x;
	particle->y = y - 0x20000;
	particle->z = source->z + ((0x10000 - scale) * 5) + 0x90000;
}

void BattleFx_UpdateDescendingParticleNegativeArc(struct ArcParticle *particle)
{
	struct ArcParticle *source;
	s32					y;
	s32					limit;
	s32					scale;

	limit = particle->floor + 0xA0000;
	source = particle->source;
	if (gGameState.map == (s32)&SceneId_Clear)
		limit = particle->floor + 0x40000;

	y = particle->y;
	if (y <= limit)
	{
		ObjectDispatch_ReleaseFar(particle);
		return;
	}

	scale = particle->scale_x + 0xC00;
	if (scale > 0x10000)
		scale = 0x10000;
	particle->scale_x = scale;
	particle->scale_y = -scale;
	particle->x = source->x;
	particle->y = y - 0x20000;
	particle->z = source->z - ((0x10000 - scale) * 5) + 0x100000;
}

struct ArcParticle;

struct ArcFrame
{
	u8 padding00[22];
	u8 visible;
};

struct ArcSprite
{
	u8 padding00[5];
	u8 flags05_0 : 5;
	u8 flags05_5 : 1;
	u8 layer : 2;
	u8 padding06;
	u8 flags07_0 : 6;
	u8 size : 2;
	union
	{
		struct
		{
			u16 tile : 10;
			u16 flags08_10 : 6;
		} half;
		struct
		{
			u8 tile_low;
			u8 flags09_0 : 2;
			u8 variant : 2;
			u8 flags09_4 : 4;
		} byte;
	} attr;
	u8				 padding0c[16];
	u8				 resource;
	u8				 active : 1;
	u8				 flags1d_1 : 7;
	u8				 padding1e[8];
	u8				 animation;
	u8				 padding27;
	struct ArcFrame *frame;
};

struct ArcObject
{
	u8				  padding00[8];
	s32				  x;
	s32				  y;
	s32				  z;
	s32				  unknown_14;
	s32				  scale_x;
	s32				  scale_y;
	u8				  padding20[0x30];
	struct ArcSprite *sprite;
	u8				  padding54;
	u8				  unknown_55;
	u8				  padding56[0x0e];
	u16				  unknown_64;
	u8				  padding66[2];
	struct ArcObject *source;
	void			 *callback;
};

struct ArcObject *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);

void BattleFx_SpawnDescendingArcParticles(struct ArcObject *source)
{
	struct BattleFxScene *scene = gEffectWork;
	struct ArcObject	 *leader = scene->main_object;
	struct ArcObject	 *objects[2];
	struct ArcObject	 *object;
	struct ArcSprite	 *sprite;
	s32					  i;
	u8					  zero;

	for (i = 0; i <= 1; i++)
	{
		object = Object_CreateFar(26, source->x, source->y + 0x400000, source->z);
		objects[i] = object;
		if (object == NULL)
			continue;
		object->unknown_14 = source->unknown_14;
		sprite = object->sprite;
		object->unknown_55 = 0;
		object->unknown_64 = 0;
		zero = 0;
		object->source = source;
		object->scale_y = 0x6666;
		object->scale_x = 0x6666;
		if (sprite == NULL)
			continue;
		AnimationObjects_SelectAnimationFar(sprite, 0);
		sprite->animation = zero;
		Resource_ResetEntry(sprite->resource);
		sprite->resource = scene->arc_tile_slot;
		sprite->active = 1;
		sprite->attr.half.tile = gVramBlockCache[sprite->resource].offset >> 5;
		sprite->flags05_5 = 0;
		sprite->layer = 1;
		sprite->size = 2;
		sprite->frame->visible = zero;
	}
	objects[0]->callback = (void *)BattleFx_UpdateDescendingParticleNegativeArc;
	objects[0]->sprite->attr.byte.variant = 0;
	objects[1]->callback = (void *)BattleFx_UpdateDescendingParticlePositiveArc;
	objects[1]->sprite->attr.byte.variant = leader->sprite->attr.byte.variant;
}

//--------------------------------------------------------------------------

// arcstate.c

// エナジー演出 16 (プルーフ)：使う者の動きと、効果が切れたときのメッセージ

//--------------------------------------------------------------------------

void UiText_ShowPositionedMessageAndWaitFar(s32 no, s32 flags);

#define BattleFx_SpawnDescendingArcParticles \
	((void (*)(void *))BattleFx_SpawnDescendingArcParticles)
#define UiText_ShowPositionedMessageAndWaitFar \
	((void (*)(void *, s32))UiText_ShowPositionedMessageAndWaitFar)

#define ENA_PURUFU 150 // プルーフ
#define ITEM_HANE 236  // せいなるはね

#ifndef MODERN // modern GCC keeps the headers' volatile declaration
#endif

void UiWork_PushValueSlotFar(s32 arg0, s32 arg1);

//--------------------------------------------------------------------------
// 演出の終わり：使う者の表示の flags を 5 フレームの周期で 3 と 1 に切り替える
//--------------------------------------------------------------------------
void BattleFx_UpdateEffect16State(void)
{
	struct ArcEffectObject *obj;
	struct AnimationObject *rec;
	struct AnimationEntry  *entry;
	s32						phase;

	obj = ObjectTable_Get(gGameState.selected_actor);
	rec = obj->records;
	entry = rec->entries[0];
	phase = gFrameCount % 5;
	if (phase == 0)
	{
		rec->dirty = 1;
		rec->flags = 3;
	}
	else if (phase == 2)
	{
		entry->param = 0; // 色を戻す
		rec->dirty = 1;
		rec->flags = 1;
	}
}

//--------------------------------------------------------------------------
// 演出中の使う者：5 フレームごと (ワールドマップでは 7) に降る弧の粒を出し、
// phase 1 では回る
//--------------------------------------------------------------------------
void BattleFx_UpdatePairedArcSpawner(struct ArcEffectObject *obj)
{
	s16 phase;
	s16 cnt;

	phase = obj->phase;
	cnt = obj->step++;

	if (gGameState.map == (s32)&SceneId_Clear)
	{
		if (__modsi3(cnt, 7) == 0)
			BattleFx_SpawnDescendingArcParticles(obj);
	}
	else if (__modsi3(cnt, 5) == 0)
	{
		BattleFx_SpawnDescendingArcParticles(obj);
	}

	if (phase == 1)
		obj->angle += 0xc00;
}

//--------------------------------------------------------------------------
// プルーフ (せいなるはね) の効果が切れた
//--------------------------------------------------------------------------
void FieldEvent_ShowStatusMessage(void)
{
	gGameState.avoid_distance = 0;
	if (gGameState.avoid_by_item == 0)
	{
		UiWork_PushValueSlotFar(ENA_PURUFU, 4);
		UiText_ShowPositionedMessageAndWaitFar(MsgAbilityWoreOff, 1);
		return;
	}
	UiWork_PushValueSlotFar(ITEM_HANE, 2);
	UiText_ShowPositionedMessageAndWaitFar(MsgItemWoreOff, 1);
}

#undef EffectSlot_Initialize
#undef UiText_ShowPositionedMessageAndWaitFar

#define BattleFx_UpdatePairedArcSpawner ((void (*)(void))BattleFx_UpdatePairedArcSpawner)
void WaitFrames(s32);

void Object_ApplyProjectedPlacementFar(
	struct AnimationObject *sprite, s32 *position, s32 *scale, s32 mode);
s32 ArcTan2(s32 x, s32 y);

u32 Random16(void);

void RunBattleEffect16(void)
{
	void					Animation_ApplyChildValuesFar(struct ArcEffectObject *, s32);
	void					ObjectDispatch_SetSingleChildFlagsFar(struct ArcEffectObject *, s32);
	struct BattleFxScene   *scene;
	struct ArcEffectObject *object;
	struct AnimationObject *group;
	struct AnimationEntry  *entry;
	u32						angle;
	s32						slot;
	s32						count;

	scene = gEffectWork;
	object = scene->main_object;
	group = object->records;
	entry = group->entries[0];
	angle = object->angle;
	slot = Resource_FindFreeEntry();
	{
		s16 *tile_slot = &scene->arc_tile_slot;
		s32	 zero = 0;

		*tile_slot = slot;
		VramBlock_LoadCached((s16)slot, 0x100, BattleFx_ArcSparkTiles);
		gGameState.avoid_distance = 0x09600000;
		gGameState.avoid_by_item = GameFlag_TestFar(0x145);
		Animation_ApplyChildValuesFar(object, zero);
		object->callback = BattleFx_UpdatePairedArcSpawner;
		object->phase = zero;
		object->step = zero;
	}
	AudioCommand_PlayFar(0x8c);
	WaitFrames(15);
	object->phase = 1;
	WaitFrames(10);
	for (count = 0; count < 20; count++)
	{
		entry->param = 7;
		group->dirty = 1;
		WaitFrames(2);
		group->dirty = 1;
		entry->param = 0;
		group->flags = 1;
		WaitFrames(3);
	}
	object->callback = NULL;
	object->angle = angle;
	Scheduler_AddOrUpdateCallback(BattleFx_UpdateEffect16State, 0xc80);
	WaitFrames(15);
	AudioCommand_PlayFar(0xae);
	WaitFrames(55);
	Scheduler_RemoveCallback(BattleFx_UpdateEffect16State);
	if (gGameState.cloaked != 0)
		ObjectDispatch_SetSingleChildFlagsFar(object, 2);
	else
		ObjectDispatch_SetSingleChildFlagsFar(object, 1);
	Animation_ApplyChildValuesFar(object, 0);
	Resource_ResetEntry(scene->arc_tile_slot);
	UiText_ShowPositionedMessageAndWaitFar((s32)MsgMonstersAttackLess, 1);
}

void EffectSlot_Update(struct EffectSlot *effect)
{
	if (effect->active != 0)
	{
		effect->age++;
		if (effect->callback_delay != 0)
			effect->callback_delay--;
		else if (effect->callback != 0)
			effect->callback(effect);
		if (effect->active != 0)
		{
			if (effect->update_motion != 0)
				EffectSlot_UpdateMotion(effect);
			if (effect->render != 0)
				BattleFx_DrawScaledObject(effect);
		}
	}
}

void BattleFx_DrawScaledObject(struct EffectSlot *object)
{
	struct AnimationObject *sprite;
	s32						offset;
	s32						scale[2];
	s32						position[4];

	sprite = object->object;
	offset = 0;
	if (object->flags & 4)
		offset = 0x1fc0000 - object->z;
	scale[0] = Iwram_MulQ16(object->scale_x, sprite->scale);
	scale[1] = Iwram_MulQ16(object->scale_y, sprite->scale);
	position[0] = object->x;
	position[1] = offset;
	position[2] = object->z + offset;
	position[3] = 0;
	if (object->x > -0x200000 && object->x < 0x1100000 && object->z > -0x200000
		&& object->z < 0xe00000)
		Object_ApplyProjectedPlacementFar(sprite, position, scale, 0);
}

void EffectSlot_UpdateMotion(struct EffectSlot *effect)
{
	s32 dx;
	s32 dz;
	s32 distance;
	s32 ix;
	s32 iz;
	s32 speed;
	s16 angle;
	s32 heading;
	s32 facing;
	s16 turn;

	if (effect->target_x == EFFECT_NO_TARGET)
		return;
	dx = effect->target_x - effect->x;
	dz = effect->target_z - effect->z;
	if (effect->stop_at_target != 0)
	{
		ix = dx / 0x10000;
		iz = dz / 0x10000;
		distance = Iwram_Sqrt(ix * ix + iz * iz) << 16;
		if (distance < 0x800000)
			distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
		if (distance <= 0x80000)
		{
			EffectSlot_SetPosition(effect, effect->target_x, effect->target_z);
			return;
		}
	}
	angle = ArcTan2(dz, dx);
	if (effect->flag42 != 0)
	{
		heading = effect->heading;
		turn = angle - heading;
		if ((turn >= 0 ? turn : -turn) >= effect->max_turn_step)
		{
			if (turn < 0)
			{
				if (-turn > effect->max_turn_step)
					turn = -effect->max_turn_step;
			}
			else if (turn > effect->max_turn_step)
			{
				turn = effect->max_turn_step;
			}
			angle = turn + heading;
		}
	}
	facing = (u16)angle;
	effect->heading = facing;
	speed = effect->speed + effect->acceleration;
	if (speed > effect->max_speed)
		speed = effect->max_speed;
	angle = facing;
	effect->speed = speed;
	effect->x += Iwram_MulQ16(Trig_Cos(angle), speed);
	effect->z += Iwram_MulQ16(Trig_Sin(angle), speed);
}

u32 BattleFx_HasReachedTarget(struct EffectSlot *effect)
{
	u32 value;

	if (effect->stop_at_target == 0)
	{
		return 0;
	}
	value = effect->target_x ^ 0x80000000;
	return ((0u - value) | value) >> 31;
}

void EffectSlot_SetPosition(struct EffectSlot *effect, s32 x, s32 z)
{
	effect->target_x = EFFECT_NO_TARGET;
	effect->target_z = EFFECT_NO_TARGET;
	effect->x = x;
	effect->z = z;
	effect->speed = 0;
}

void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode)
{
	AnimationObjects_SelectAnimationFar(effect->object, mode);
}

void EffectSlot_SetCallback(struct EffectSlot *effect, EffectCallback callback)
{
	effect->callback = callback;
	effect->callback_delay = 0;
	effect->age = 0;
	effect->state = 0;
}

void EffectSlot_Initialize(struct EffectSlot *effect, s32 kind, s32 x, s32 z)
{
	s32			 unit;
	s8			*object;
	volatile u32 zero;

	zero = 0;
	Dma_Set((const void *)&zero, effect, 0x85000000 | (sizeof *effect / 4), REG_DMA3);
	object = ResourceObject_CreateFar(kind);
	effect->object = object;
	if (object != NULL)
		object[9] &= ~0x0c;
	EffectSlot_SetPosition(effect, x, z);
	/* FAKEMATCH: needed to match: 1.0 is held in a local, set before max_speed, so the object reload is scheduled ahead of the three unit stores */
	unit = 0x10000;
	effect->max_speed = 0x20000;
	effect->acceleration = effect->scale_y = effect->scale_x = unit;
	effect->origin_x = x;
	effect->origin_z = z;
	((s8 *)effect->object)[38] = 0;
	effect->stop_at_target = 1;
	effect->flag42 = 1;
	effect->update_motion = 1;
	effect->render = 1;
	effect->active = 1;
	effect->random_value = Random16();
	effect->flags = 4;
	EffectSlot_SetObjectMode(effect, 1);
}

void BattleFx_ClearOwnedSlot(struct EffectSlot *slot)
{
	volatile u32 zero;
	if (slot->object)
		ResourceObject_ReleaseFar((struct ResourceObjectWork *)slot->object);
	zero = 0;
	Dma_Set(&zero, slot, 0x85000000 | (sizeof *slot / 4), REG_DMA3);
}

struct FxWindowEntry
{
	u32 a;
	u32 b;
	u32 attr;
};

struct FxWindowWork
{
	u16					 tiles;
	u16					 window_slot;
	s32					 x;
	s32					 y;
	u8					 padding0c[6];
	u16					 unknown_12;
	u8					 padding14[4];
	s32					 unknown_18;
	s32					 window;
	struct FxWindowEntry entries[67];
};

struct FxObject
{
	u8	padding00[8];
	s32 x;
	u8	padding0c[6];
	s16 y;
};

void BattleFx_SetupResourcesAndWindow(void)
{
	extern struct FxWindowWork gMapCellBuffer;
	u32						  *oam;
	volatile u32			   fill;
	u32						  *pal;
	struct FxWindowWork		  *work;
	struct FxObject			  *object;
	s32						   attr;
	u32						   i;
	u32						   tile;
	u32						   attr2;

	pal = (u32 *)Runtime_BumpAllocateAlternatePool(32);
	work = &gMapCellBuffer;
	oam = (u32 *)work->entries;
	object = Object_GetById(gGameState.selected_actor);
	work->tiles = Resource_FindFreeEntry();
	fill = 0;
	Dma_Set((const void *)&fill, pal, 0x85000020, REG_DMA3);
	pal[0] = 255;
	pal[1] = 0x1ff;
	pal[2] = 272;
	pal[8] = 68;
	pal[9] = 324;
	pal[10] = 272;
	pal[16] = 119;
	pal[17] = 0x177;
	pal[18] = 272;
	pal[24] = 0xff0;
	pal[25] = 0xffff;
	pal[26] = 0x1ffff;
	pal[27] = 0x11ff0;
	pal[28] = 0x1100;
	attr = VramBlock_LoadCached(work->tiles, 128, pal) | 0x400;
	for (i = 0; i < 66; i++)
	{
		*oam++ = 0;
		*oam++ = 0;
		*oam++ = attr;
	}
	MemFree(pal);
	if (GameFlag_TestFar(284))
	{
		work->x = 240 << 15;
		work->y = 160 << 15;
	}
	else
	{
		work->x = (((object->x - 0x10000000) >> 16) * 240 / 4096) << 16;
		work->y = ((object->y * 160) / 4096) << 16;
	}
	work->window_slot = Resource_LoadFixedBlockBIntoFreeSlotFar();
	tile = gVramBlockCache[work->window_slot].offset;
	work->window = UiWindow_CreateFar(0, 0, 0, 0, 2);
	work->unknown_12 = 0xffff;
	work->unknown_18 = 0x10000;
	*oam++ = 0;
	*oam++ = 0x40000000;
	attr2 = 0x400;
	attr2 |= tile >> 5;
	*oam = attr2;
}

//--------------------------------------------------------------------------

// cleanup_resources_and_window.c

// ワールドマップ：地名の印・カーソル・地名ウインドウ

//--------------------------------------------------------------------------

void RenderOutput_PrepareForRedrawFar(s32 window);
#define ObjectTable_Get ((struct MapObject * (*)(s32)) ObjectTable_Get)

extern void UiWork_FinalizeFar(u32 arg0, u32 arg1);

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
// キーを離すとカーソルが近くの地名へ寄る
#define WORLD_MAP_CURSOR_SNAPS 1
#define WORLD_MAP_NAME_RANGE 64 // 地名を出す距離の 2 乗
#define WORLD_MAP_NAME_LEFT 3	// 地名ウインドウを印から左へずらす量
#else
#define WORLD_MAP_CURSOR_SNAPS 0
#define WORLD_MAP_NAME_RANGE 100
#define WORLD_MAP_NAME_LEFT 1
#endif

#define FLAG_NOT_ON_MAP 284 // いまいる場所が地図にのっていない

// OBJ の属性（2 ワード目からが OAM と同じ並び）
struct MapMarker
{
	struct MapMarker *link;
	u16				  y : 8;
	u16				  affine : 2;
	u16				  blend_mode : 2; // 1 で半透明
	u16				  mosaic : 1;
	u16				  full_color : 1;
	u16				  shape : 2;
	u16				  x : 9;
	u16				  affine_index : 5;
	u16				  size : 2;
	u16				  tile : 10;
	u16				  priority : 2;
	u16				  palette : 4;
	u16				  padding0a;
};

// 16.16 固定小数（整数部がドット）
union MapFixed
{
	s32 value;
	struct
	{
		u16 fraction;
		s16 whole;
	} part;
};

// ワールドマップの作業領域（gMapCellBuffer に置く）
struct WorldMapWork
{
	u16				 vram_block; // 印の OBJ キャラクタの VRAM ブロック
	u16				 padding02;	 // もうひとつの VRAM ブロック（終わりに返す）
	union MapFixed	 x;			 // カーソル（ドット）
	union MapFixed	 z;
	u8				 padding0c[6];
	s16				 shown; // いま地名ウインドウに出している印（-1 はなし）
	u8				 padding14[4];
	s32				 speed; // カーソルの速さ
	s32				 window;
	struct MapMarker markers[67];
};

struct MapObject
{
	u8			   padding00[8];
	s32			   x;
	u8			   padding0c[4];
	union MapFixed z;
};

//--------------------------------------------------------------------------
// ワールドマップを閉じる：VRAM ブロックを返し、地名ウインドウを消す
//--------------------------------------------------------------------------
void BattleFx_CleanupResourcesAndWindow(void)
{
	extern u8			 gMapCellBuffer[];
	struct WorldMapWork *work = (struct WorldMapWork *)gMapCellBuffer;

	Resource_ResetEntry(work->vram_block);
	Resource_ResetEntry(work->padding02);
	UiWork_FinalizeFar(work->window, 2);
}
#ifndef MODERN // modern GCC keeps the headers' volatile declaration
#endif
s32		   ArcTan2(s32 dz, s32 dx);

// IME を止めて I/O 書き込みキューに 1 件積む（QueueIoWriteDelay2 と同じ）
#define QUEUE_IO_WRITE_DELAY2(address, value)                                                         \
	do                                                                                                \
	{                                                                                                 \
		u32 saved;                                                                                    \
		s32 count;                                                                                    \
                                                                                                      \
		/* FAKEMATCH: a block of its own keeps the saved IME read first. */                           \
		do                                                                                            \
		{                                                                                             \
			/* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
			saved = *ime;                                                                             \
		} while (0);                                                                                  \
		*ime = (u16)ime;                                                                              \
		count = q->count;                                                                             \
		if (count <= 31)                                                                              \
		{                                                                                             \
			u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);                                     \
			*(u16 *)&q->count = count + 1;                                                            \
			*destination++ = (value);                                                                 \
			*destination++ = (address);                                                               \
			*destination = 0x20000;                                                                   \
		}                                                                                             \
		*ime = saved;                                                                                 \
	} while (0)

//--------------------------------------------------------------------------
// ワールドマップの毎フレーム処理（スケジューラのコールバック）
// フラグの立った場所に印を出し、十字キーでカーソルを動かし、
// カーソルにいちばん近い印の地名をウインドウに出す
// L・R を押している間はカーソルを現在地に合わせる
//--------------------------------------------------------------------------
void Map_UpdateWorldMapMarkers(void)
{
	void				 Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 * position);
	s32					 leader;
	const s32			*place;
	s32					 tile_base;
	s32					 blend;
	s32					 best;
	s32					 best_dist;
	s32					 cursor_x;
	s32					 best_x;
	s32					 best_y;
	s32					 mes;
	s32					 best_mes;
	s32					 height;
	s32					 width;
	s32					 pos[3];
	struct WorldMapWork *work;
	struct MapObject	*obj;
	s32					 cursor_y;
	s32					 tile;
	s32					 mode;
	s32					 flag;
	s32					 i;
	struct MapMarker	*marker;
	s32					 x;
	s32					 y;
	s32					 angle;
	struct IoWriteQueue *q;
	volatile u16		*ime;

#if WORLD_MAP_CURSOR_SNAPS
	leader = gGameState.selected_actor;
	work = (struct WorldMapWork *)Ram_MapCellBuffer;
	marker = work->markers;
	place = WorldMap_PlaceMarkers;
	tile_base = gVramBlockCache[work->vram_block].offset >> 5;
#else
	work = (struct WorldMapWork *)Ram_MapCellBuffer;
	leader = gGameState.selected_actor;
	place = WorldMap_PlaceMarkers;
	tile_base = gVramBlockCache[work->vram_block].offset >> 5;
	marker = work->markers;
#endif
	best = -1;
	best_dist = WORLD_MAP_NAME_RANGE;
	blend = WorldMap_MarkerBlendCycle[(gFrameCount >> 1) & 31];

	//------------------------------------------------------------------
	// カーソル
	//------------------------------------------------------------------
	if (!GameFlag_TestFar(FLAG_NOT_ON_MAP) && (gKeysHeld & KEYS_SHOULDERS))
	{
		// L・R：現在地
		obj = ObjectTable_Get(gGameState.selected_actor);
		if (obj == NULL)
			goto markers;
		cursor_x = ((obj->x - 0x10000000) >> 16) * 240 / 4096;
		cursor_y = obj->z.part.whole * 160 / 4096;
	}
	else
	{
		// 十字キー：押し続けると速くなる
		angle = WorldMap_CursorDirectionAngles[(gKeysHeld >> 4) & 15];
		if (angle != 0xffff)
		{
			pos[0] = work->x.value;
			pos[1] = 0;
			pos[2] = work->z.value;
			Vector_AddPolarOffset(work->speed, angle, pos);
			if (pos[0] < 0x100000) // x は 16〜239 ドット
				pos[0] = 0x100000;
			if (pos[0] > 0xef0000)
				pos[0] = 0xef0000;
			if (pos[2] < 0) // y は 0〜141 ドット
				pos[2] = 0;
			if (pos[2] > 0x8d0000)
				pos[2] = 0x8d0000;
			work->x.value = pos[0];
			work->z.value = pos[2];
			if (work->speed < 0x60000)
				work->speed += 0x2000;
		}
		else
		{
			work->speed = 0x10000;
		}
		cursor_x = work->x.part.whole;
		cursor_y = work->z.part.whole;
	}

	//------------------------------------------------------------------
	// 印：0 番は現在地、11〜65 番は WorldMap_PlaceMarkers の場所
	// 表は（フラグ, 地名）の組で、フラグの上位 16bit はキャラクタ番号
	//------------------------------------------------------------------
markers:
	for (i = 0; i <= 65; i++)
	{
		if (i != leader && i <= 10)
			continue;
		if (i == 0)
		{
			if (GameFlag_TestFar(FLAG_NOT_ON_MAP))
				continue;
			tile = 0;
			flag = leader;
			mode = 0;
		}
		else
		{
			flag = *place++;
			mes = *place++;
			if (flag == 0)
				continue;
#if WORLD_MAP_CURSOR_SNAPS
			if (flag == -1)
				break;
			tile = flag >> 16;
			mode = 1;
#else
			tile = flag >> 16;
			mode = 1;
			if (flag == -1)
				break;
#endif
		}
		if (!GameFlag_TestFar(flag))
			continue;
		obj = ObjectTable_Get(i);
		if (obj == NULL)
			continue;

		// ワールドの 4096 ドット四方を 240x160 に縮める
		x = ((obj->x - 0x10000000) >> 16) * 240 / 4096;
		y = obj->z.part.whole * 160 / 4096;
		marker->blend_mode = mode;
		marker->tile = tile_base + tile;
		marker->x = x - 1;
		marker->y = y - 1;
		width = x - cursor_x;
		height = y - cursor_y;
		if (width * width + height * height < best_dist)
		{
			best_mes = mes;
			best = i;
			best_dist = width * width + height * height;
			best_x = x;
			best_y = y;
		}
		if (i != 0 || (gFrameCount & 15) <= 7) // 現在地は 8 フレームごとに点滅
			Runtime_PushSlotEntry(marker++, y);
	}
#if WORLD_MAP_CURSOR_SNAPS
	// キーを離していれば、カーソルをいちばん近い印へ寄せる
	if (gKeysHeld == 0)
	{
		if (best != -1)
		{
			if (best_dist <= 1)
			{
				work->x.value = best_x << 16;
				work->z.value = best_y << 16;
				work->speed = 0x10000;
			}
			else
			{
				if (work->speed < 0x20000)
					work->speed = 0x20000;
				angle = ArcTan2(best_y - cursor_y, best_x - cursor_x);
				pos[0] = work->x.value;
				pos[1] = 0;
				pos[2] = work->z.value;
				Vector_AddPolarOffset(work->speed, (u16)angle, pos);
				work->x.value = pos[0];
				work->z.value = pos[2];
				if (work->speed < 0x60000)
					work->speed += 0x2000;
			}
			cursor_x = work->x.part.whole;
			cursor_y = work->z.part.whole;
		}
		else
		{
			work->speed = 0x10000;
		}
	}
#endif

	// いちばん近い印を囲む枠（点滅）
	if (best != -1 && (gFrameCount & 15) <= 7)
	{
		marker->blend_mode = 0;
		marker->tile = tile_base + 3;
		marker->x = best_x - 2;
		marker->y = best_y - 2;
		Runtime_PushSlotEntry(marker, best_y);
	}

	// カーソル
	marker = &work->markers[66];
	marker->affine_index = 0;
	marker->x = cursor_x - 17;
	marker->y = cursor_y + 1;
	Runtime_PushSlotEntry(marker, 246);

	//------------------------------------------------------------------
	// 地名ウインドウ：印が変わったときだけ書き直す
	//------------------------------------------------------------------
	if (work->shown != best)
	{
		work->shown = best;
		RenderOutput_PrepareForRedrawFar(work->window);
		if (best != -1)
		{
			if (best == 0)
				best_mes = (s32)MsgPresentLocation;
			else
				best_mes = Scene_GetAreaNameIndex(best_mes, 1) + (s32)MsgDebugEntryName;
			UiText_MeasureResourceEntriesFar(best_mes, &width, &height);
			cursor_x = best_x;
			cursor_y = best_y;
			cursor_x -= WORLD_MAP_NAME_LEFT;
			cursor_y -= 11;
			if (cursor_x + width > 240)
			{
				// 右へはみ出すなら、印の左上へ回す
				cursor_x = 232 - width;
				cursor_y = best_y - 20;
			}
			if (cursor_y < 0)
				cursor_y = 0;
			UiText_DrawResourceFar(best_mes, work->window, cursor_x, cursor_y);
		}
	}

	// 印の半透明を明滅させる
	q = &gIoWriteQueue;
	ime = &REG_IME;
	QUEUE_IO_WRITE_DELAY2((u32)&REG_BLDCNT, 0x3f00);
	QUEUE_IO_WRITE_DELAY2((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend);
}

#undef ObjectTable_Get

void Event_FinishScreenTransition(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
void BattleFx_ScheduleRatioTransition(s32, s32);

void Map_ShowWorldMap(s32 picture)
{
	extern u8 gMapCellBuffer[];
	/* FAKEMATCH: needed to match: one-pass loops order display-control setup, the spilled layer load and the IME read; the count uses a halfword store. */
	struct MapState			*map = WORK_SLOT(8);
	struct EventWork		*field = Runtime_AllocateBlock(27, 0xccc);
	struct ObjectSystemWork *menu = WORK_SLOT(HEAP_SLOT_MENU_CONTROL);
	s32					 resource = (s32)&ResourceId_WorldMapPicture;
	struct MapAnimation *layer;
	u8					 saved_flags[16];
	s32					 i;
	u8					*graphics;
	s32					 saved_status;
	s16					 blend;

	if (field->mode != 3)
		return;
	field->view_center->paused = 1;
	saved_status = field->transition_frames;
	field->transition_frames = 6;
	Event_FinishScreenTransition();
	Event_WaitTransitionFrames();
	layer = map->anim;
	for (i = 0; i < 16; i++)
	{
		saved_flags[i] = layer->paused;
		layer->paused = 1;
		layer++;
	}
	menu->suspended = 1;
	WaitFrames(1);
	blend = REG_BLDALPHA;
	graphics = Resource_GetTableEntry(resource);
	Dma_Set(graphics, (void *)BG_PLTT, DMA_ENABLE32 | DMA_32BIT | (448 / 4), REG_DMA3);
	BG_PLTT[0] = 0;
	Resource_DecodeType01(graphics + 448, gMapCellBuffer);
	Dma_Set(gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00, DMA_ENABLE32 | DMA_32BIT | (240 * 160 / 4),
		REG_DMA3);
	{
		/* FAKEMATCH: needed to match: a word temporary keeps the halfword values as word constants; direct stores load them by halfword from the pool. */
		s32 v = BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6) | 2;
		REG_BG1CNT = v;
		v = DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
		REG_DISPCNT = v;
	}
	BattleFx_SetupResourcesAndWindow();
	Scheduler_AddOrUpdateCallback(Map_UpdateWorldMapMarkers, 0xc80);
	if (GameFlag_TestFar(284))
		UiText_ShowPositionedMessageAndWaitFar((s32)MsgNotOnMap, 1);
	do
	{
		WaitFrames(1);
	} while ((gKeysRepeat & (KEY_A | KEY_B)) == 0);
	Scheduler_RemoveCallback(Map_UpdateWorldMapMarkers);
	BattleFx_CleanupResourcesAndWindow();
	{
		s32 v;
		/* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling. */
		do
		{
			v = DISPCNT_OBJ_1D_MAP;
		} while (0);
		REG_DISPCNT = v;
	}
	Map_LoadAreaGraphicsFar();
	{
		volatile u16		*ime;
		struct IoWriteQueue *q;
		u32					 saved;
		s32					 count;

		q = &gIoWriteQueue;
		/* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling. */
		do
		{
			ime = &REG_IME;
			saved = *ime;
		} while (0);
		*ime = (u16)ime;
		count = q->count;
		if (count <= 31)
		{
			u32 *destination = q->entries[count];
			*(u16 *)&q->count = count + 1;
			*destination++ = (u16)blend;
			*destination++ = (u32)&REG_BLDALPHA;
			*destination = 0x20000;
		}
		*ime = saved;
	}
	/* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling. */
	do
	{
		layer = map->anim;
	} while (0);
	for (i = 0; i < 16; i++)
	{
		layer->paused = saved_flags[i];
		layer++;
	}
	menu->suspended = 0;
	Event_StartScreenTransition();
	Event_WaitTransitionFrames();
	field->transition_frames = saved_status;
	field->view_center->paused = 0;
}

void BattleFx_UpdateObjectVisibilityBounds(void)
{
	struct ObjectRuntime *object;
	s32					  x;
	s32					  y;
	s32					  left;
	s32					  right;
	s32					  top;
	s32					  bottom;
	u32					  id;

	// the selected actor is looked up and the result discarded, as the ROM does
	Object_GetById(gGameState.selected_actor);
	object = (struct ObjectRuntime *)gEventWork->view_center;
	x = object->x;
	left = x - (320 << 16);
	right = x + (320 << 16);
	y = object->z;
	top = y - (600 << 16);
	bottom = y + (400 << 16);

	id = 8;
	do
	{
		object = ObjectTable_Get(id);

		if (object != 0)
		{
			s32 ox = object->x;
			s32 oy = object->z;

			if (ox < left || ox > right || oy < top || oy > bottom)
			{
				object->animation_kind = 0;
			}
			else
			{
				object->animation_kind = 1;
			}
		}
		id++;
	} while (id <= 65);
}

void BattleFx_RunVisibilityTransition(void)
{
	struct EventWork *field = Runtime_AllocateBlock(27, 0xccc);

	if (field->mode == 3)
	{
		Scheduler_EnableUnmaskedOverlayCallbacks();
		BattleFx_UpdateObjectVisibilityBounds();
		Battle_InitializeRenderObject();
		BattleFx_ScheduleRatioTransition(0x9d89, 6);
		if (gKeysHeld & KEY_L)
		{
			do
			{
				WaitFrames(1);
			} while (gKeysHeld & KEY_L);
		}
		BattleFx_ScheduleRatioTransition(0x10000, 6);
		Scheduler_DisableOverlayCallbacks();
	}
}
