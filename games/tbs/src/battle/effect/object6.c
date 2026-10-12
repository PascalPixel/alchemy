#include "unit/battle/effect/btlcue.h"
#include "unit/battle/effect/zonedata.h"
#include "vram_tab.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object5.h"
#include "unit/battle/effect/object6.h"
#include "unit/battle/effect/object7.h"
#include "unit/battle/effect/object8.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/field/common/effect/move.h"
#include "unit/graphics/display/scroll_initialize_hblank_dma.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "types.h"
#include "scene.h"
#include "btltype.h"
#include "game_state.h"
#include "fieldrun.h"
#include "btlsys.h"
#include "battle.h"
#include "battle_unit.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "iowrite.h"
#include "irq.h"
#include "callback_scheduler.h"
#include "flag_ids.h"
#include "iwram_call.h"
#include "eventwrk.h"
#include "resource_ids.h"
#include "battle_effect_runtime.h"
#include "resource.h"
#include "field_scene.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "global_cells.h"
#include "object_runtime.h"
#include "script_object_runtime.h"
#include "animspr.h"
#include "fieldobj.h"
#include "object_dispatch.h"
#include "field_event.h"
#include "event_runtime.h"
#include "field_sprite.h"
#include "battle_effect_counters.h"
#include "heap_state.h"
#include "keystate.h"
#include "ownerval.h"
#include "party_state.h"
#include "sound_ids.h"
#include "fixed_math.h"
#include "itemgift.h"
#include "btlcalc.h"
#include "disptran.h"
#include "text/msg_ids.h"
#include "object_lookup.h"
#include "objdisp.h"
#include "effect_runtime.h"
#include "object_effect.h"
#include "item.h"
#include "motobj.h"
#include "layout_guard.h"
#include "frame.h"
#include "io_write_queue.h"
#include "palbuf.h"
#include "vram_block.h"
#include "item_ids.h"
#include "inventory.h"
#include "ui.h"
#include "window.h"
#include "btlpty.h"
#include "b5_context.h"
#include "edition.h"
#include "facing_object.h"
#include "script_motion.h"
#include "tbs_edition.h"
#include "metadata_lookup.h"
#include "fieldcoo.h"
#include "scroll.h"
#include "fxblend.h"
#include "fx_scene.h"
#include "effect_slot.h"
#include "object_efx.h"
#include "menu_list.h"
#include "field_effect.h"
#include "fixed_point_position.h"

// イベントのワーク (gEventWork) のこのファイルでの見え方。共通の EventWork に欄のない 0xcb6 以降も持つ
struct FieldWork
{
	u8				   unknown_000[0x10];
	s32				   view;
	struct FieldActor *actors[86];
	s16				   action;
	s16				   input;
	s16				   exit;
	s16				   menu;
	s16				   examine;
	s16				   start;
	u16				   descriptor;
	s16				   tile;
	s16				   scene;
	s16				   command;
	s16				   item;
	s16				   event;
	s16				   count;
	s16				   survived;
	s16				   quantities[9];
	s16				   item_argument;
	u8				   unknown_19c[2];
	s16				   mode;
	u8				   unknown_1a0[8];
	s32				   unknown_1a8;
	u8				   unknown_1ac[4];
	s32				   unknown_1b0;
	s32				   unknown_1b4;
	u8				   unknown_1b8[4];
	u8				  *cells;
	s32				   transition;
	u16				   unknown_1c4;
	u16				   transition_active;
	s32				   transition_frames;
	u8				   unknown_1cc[0xaea];
	u16				   busy;
	s16				   effect_active;	// 効果が動いていれば 0 以外
	s16				   effect_frames;	// 残りのフレーム
	s16				   effect_center_x; // 効果の中心 (ドット)
	s16				   effect_center_y;
	s16				   unknown_cc0;
	u8				   unknown_cc2[4];
	s8				   resolving_action; // エナジーの演出中
	s8				   unknown_cc7;
	s16				   queued_sound;
};

struct MarkerSlot
{
	struct ScriptObjectRuntime *object;
	u8							id; // イベントのトリガー番号
	u8							unknown_05;
	u8							column; // 16 ドットのマス
	u8							row;
};

// イベントワークのうち、マーカーの記録
struct MarkerWork
{
	u8				  unknown_000[0x11c];
	struct MarkerSlot slots[MARKER_MAX];
};

struct EfxVisual
{
	u8 pad0[5];
	u8 flatbs;
	u8 pad1[3];
	u8 flatla;
	u8 pad2[18];
	u8 slot; // VRAM のタイル枠
	u8 pad3[9];
	u8 unk1;
	u8 unk2;
};

struct TileBits
{
	u16 tile : 10;
};

struct EfxObj
{
	s32				  data; // スクリプト
	u8				  pad0[0x24];
	u32				  travel;
	u8				  pad1[0x1c];
	u32				  dy;
	u8				  pad2[4];
	struct EfxVisual *vis;
	u8				  pad3[24];
	void (*proc)(void); // 毎フレームの処理
};

// オブジェクト 64 個の表 (gEventWork の 88 バイト前)
#define EfxPool (*(struct EfxObj **)((u8 *)&gEventWork - 88))

// マップのイベント (flags が -1 で終わり)
struct MapEventEntry
{
	s32 flags; // 下位 4 ビットが種類
	s16 id;
	s16 flag;
	s32 position;
};

//--------------------------------------------------------------------------
// 操作キャラが印のマスの中心から 16 ドット以内にいれば、
// 中心から操作キャラの向きへ 20 ドット離れた位置へ出す
//--------------------------------------------------------------------------
void BattleFx_EmitRandomParticle(void)
{
	struct ObjectRuntime *actor = ObjectTable_Get(gGameState.selected_actor);
	struct MarkerSlot	 *cell = ((struct MarkerWork *)gEventWork)->slots;
	s32					  i = 0;
	s32					  x;
	s32					  z;
	s32					  cx;
	s32					  cz;
	s32					  dx;
	s32					  dz;

	if (cell->id != 0)
	{
		x = actor->x;
		z = actor->z;
		for (;;)
		{
			cx = cell->column << 20;
			dx = x - cx - (8 << 16);
			cz = cell->row << 20;
			dz = z - cz - (8 << 16);
			if ((dx >= -0xfffff && dx <= 0xfffff) && (dz >= -0xfffff && dz <= 0xfffff))
			{
				actor->x = cx + (8 << 16);
				actor->z = cz + (8 << 16);
				Vector_AddPolarOffset(
					20 << 16, (u16)ArcTan2(dz, dx), (struct EffectVector *)&actor->x);
				actor->target_x = ACTOR_NO_TARGET; // 移動先なし
				actor->target_y = ACTOR_NO_TARGET;
				actor->target_z = ACTOR_NO_TARGET;
				return;
			}
			i++;
			cell++;
			if (i > 9 || cell->id == 0)
				break;
		}
	}
}

//--------------------------------------------------------------------------
// アイテム表示の毎フレームの処理：1/10 の確率で、周り 16 ドット以内にきらめきを出す
//--------------------------------------------------------------------------
void BattleFx_EmitRandomParticleFromEmitter(struct ObjectRuntime *emitter)
{
	struct EffectVector	  pos;
	struct ObjectRuntime *obj;
	u32					  random_angle;

	if (emitter->velocity_y >= -255 && emitter->velocity_y <= 255)
		emitter->flags = 0;

	if ((100 * Random16() >> 16) > 9)
		return;

	pos.x = emitter->x;
	pos.y = emitter->y;
	pos.z = emitter->z;
	random_angle = Random16();
	Vector_AddPolarOffset(random_angle << 4, Random16(), &pos);
	obj = Object_Spawn(285, pos.x, pos.y, pos.z);
	if (obj != 0)
	{
		s32 mask;
		u8	flags;

		// パーツ 0 の 9 バイト目：bit 2〜3 を 1 にする
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_ParticleScript);
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
		mask = 13;
		flags = ((u8 *)&((struct AnimationObject *)obj->animation)->part[0])[9];
		mask = -mask;
		mask &= flags;
		mask |= 4;
		((u8 *)&((struct AnimationObject *)obj->animation)->part[0])[9] = mask;
	}
}

//--------------------------------------------------------------------------
// キャラ actor の 16 ドット前のマスの中心、高さ 16 ドットにアイテム item の絵を出す
// 効果が動いていれば、先に残っているきらめきを消す
// 戻り値：出したオブジェクト (出せなければ 0)
//--------------------------------------------------------------------------
struct EfxObj *BattleFx_StartRandomParticleEmitter(s32 actor, s32 item)
{
	struct EfxObj *obj;

	s32 work = (s32)gEventWork;
	u32 off = (actor * 4) + 0x14; // EventWork の actors[actor]

	s32					src_z = (s32) * (struct FieldActor **)(work + off);
	struct EffectVector pos;
	s32					x;

	if (src_z == 0)
		return 0;

	pos.x = ((struct FieldActor *)src_z)->x.fixed;
	pos.y = ((struct FieldActor *)src_z)->y.fixed;
	pos.z = ((struct FieldActor *)src_z)->z.fixed;
	Vector_AddPolarOffset(16 << 16, ((struct FieldActor *)src_z)->facing, &pos);
	x = (pos.x & 0xfff00000) + (8 << 16);
	src_z = (pos.z & 0xfff00000) + (8 << 16);

	if (((struct FieldWork *)work)->effect_active != 0)
	{
		struct EfxObj *ent = EfxPool;

		work = 63;

		do
		{
			if (ent->data != 0)
			{
				if (ent->proc == (void (*)(void))BattleFx_SpawnRandomParticleAtPosition)
				{
					ObjectDispatch_ReleaseFar(ent);
				}
				if (ent->data == (s32)BattleFx_ParticleScript)
				{
					ObjectDispatch_ReleaseFar(ent);
				}
			}
			work--;
			ent++;
		} while (work >= 0);
	}

	WaitFrames(3);
	obj = (struct EfxObj *)Object_CreateFar(22, x, 16 << 16, src_z);
	if (obj == 0)
		return 0;

	ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_ParticleEmitterScript);
	{
		struct EfxVisual *vis = obj->vis;
		void			 *buf;
		s32				  mask;
		s32				  zero = 0;

		vis->unk1 = zero;
		vis->unk2 = zero;

		vis->flatbs &= zero - 33;

		mask = vis->flatla & 0x0f;
		{
			s32 clr = 13;
			clr = -clr;
			mask &= clr;
		}
		mask |= 4;
		vis->flatla = mask;

		obj->travel = 0x20000;
		obj->dy = 0x4000;

		// アイテムの絵を作業領域 17 へ読み、VRAM のタイルへ送る
		buf = Runtime_AllocateHeapBlock(17, 0x608);
		Ui_PrepareTransferForItemFar(item);

		((struct TileBits *)((u8 *)vis + 8))->tile =
			VramBlock_LoadCached(vis->slot, 128, (u8 *)buf + 0x400);

		Runtime_ReleaseHeapBlock(17);
		obj->proc = (void (*)(void))BattleFx_EmitRandomParticleFromEmitter;
	}

	return obj;
}

void Object_DestroyIfPresent(void *object)
{
	if (object != 0)
		ObjectDispatch_ReleaseFar(object);
}

//--------------------------------------------------------------------------
// アイテムの絵を操作キャラの 36 ドット上に置いて動かし、
// 操作キャラに動作 28 をとらせて向きを 0x4000 (90°) にする
//--------------------------------------------------------------------------
void EffectRuntime_PrepareRisingObject(struct ObjectRuntime *object)
{
	struct ObjectRuntime *actor;

	if (object == 0)
		return;

	actor = ObjectTable_Get(gGameState.selected_actor);
	object->acceleration = 1 << 16;
	object->speed_limit = 2 << 16;
	object->flags = 0;
	Object_SetMoveTargetFar(object, actor->x, actor->y + (36 << 16), actor->z);
	WaitFrames(3);
	ObjectDispatch_ApplyArgumentToChildrenFar(actor, 28);
	ObjectDispatch_InitializeFar((struct DispatchObject *)object, BattleFx_RisingPulseScript);
	actor->angle = 0x4000;
}

//--------------------------------------------------------------------------
// flags の bit 0 できらめきを付け、bit 1 で EffectRuntime_PrepareRisingObject
// 両方なら前後で 60 と 80 フレーム待つ。最後に操作キャラを動作 1 にする
//--------------------------------------------------------------------------
void EffectRuntime_RunRisingObjectSequence(void *object, s32 flags)
{
	void *actor;

	if (object != NULL)
	{
		actor = ObjectTable_Get(gGameState.selected_actor);
		if (flags & 1)
		{
			ObjectDispatch_SetSingleChildFlagsFar(object, 0);
			ObjectDispatch_InitializeFar(object, BattleFx_ParticleEmitterScript);
			((struct EfxObj *)object)->travel = 0x20000;
			((struct EfxObj *)object)->dy = 0x4000;
			((struct EfxObj *)object)->proc =
				(void (*)(void))BattleFx_EmitRandomParticleFromEmitter;
		}
		if (flags == 3)
		{
			WaitFrames(60);
		}
		if (flags & 2)
		{
			EffectRuntime_PrepareRisingObject(object);
		}
		if (flags == 3)
		{
			WaitFrames(80);
		}
		ObjectDispatch_ApplyArgumentToChildrenFar(actor, 1);
	}
}

//--------------------------------------------------------------------------
// 操作キャラの 36 ドット上にアイテム value の絵を 80 フレーム出す
// flags の bit 0 できらめきを付け、bit 1 で EffectRuntime_PrepareRisingObject
//--------------------------------------------------------------------------
void BattleFx_StartEffectObject22(s32 value, s32 flags)
{
	struct FieldActor *actor = ObjectTable_Get(gGameState.selected_actor);
	void			  *handle = Runtime_AllocateHeapBlock(17, 0x608);
	struct EfxObj	  *object = (struct EfxObj *)Object_CreateFar(
		22, actor->x.fixed, actor->y.fixed + (36 << 16), actor->z.fixed);

	if (object != 0)
	{
		struct EfxVisual *visual = object->vis;
		s32				  mask;

		s32 zero = 0;
		visual->unk1 = zero;
		visual->unk2 = zero;

		visual->flatbs &= zero - 33;

		mask = visual->flatla & 0x0f;
		{
			s32 clear = 13;
			clear = -clear;
			mask &= clear;
		}
		mask |= 4;
		visual->flatla = mask;

		Ui_PrepareTransferForItemFar(value);
		VramBlock_LoadCached(visual->slot, 128, (u8 *)handle + 0x400);
		Runtime_ReleaseHeapBlock(17);

		if (flags & 1)
			object->proc = (void (*)(void))BattleFx_EmitRandomParticleFromEmitter;
		if (flags & 2)
			EffectRuntime_PrepareRisingObject((struct ObjectRuntime *)object);

		WaitFrames(80);
		ObjectDispatch_ApplyArgumentToChildrenFar(actor, 1);
		ObjectDispatch_ReleaseFar(object);
	}
}

// ドイツ語・スペイン語・フランス語・イタリア語版はメニューを開いている間きらめきを止める
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)

#define PARTICLES_PAUSE_FOR_MENU 1
#endif

#if defined(PARTICLES_PAUSE_FOR_MENU)
#define MENU_OPEN_OFS 0xcca // gEventWork：メニューを開いていれば 0 以外

//--------------------------------------------------------------------------
// 効果が動いていれば、きらめきのオブジェクトをすべて消す
//--------------------------------------------------------------------------
void BattleFx_ClearRandomParticles(void)
{
	if (((struct FieldWork *)gEventWork)->effect_active != 0)
	{
		struct EfxObj *ent = EfxPool;
		s32			   n = 63;

		do
		{
			if (ent->data != 0)
			{
				if (ent->data == (s32)BattleFx_ParticleScript)
					ObjectDispatch_ReleaseFar(ent);
			}
			n--;
			ent++;
		} while (n >= 0);
	}
}

#endif

//--------------------------------------------------------------------------
// 印の毎フレームの処理：1/10 の確率で、周り 16 ドット以内にきらめきを出す
//--------------------------------------------------------------------------
void BattleFx_SpawnRandomParticleAtPosition(const struct FieldActor *source)
{
	struct EffectVector	  pos;
	struct ObjectRuntime *obj;
	u32					  rnd;

#if defined(PARTICLES_PAUSE_FOR_MENU)
	if (*(s8 *)((u8 *)gEventWork + MENU_OPEN_OFS) != 0)
		return;
#endif
	if ((100 * Random16() >> 16) > 9)
		return;

	pos.x = source->x.fixed;
	pos.y = source->y.fixed;
	pos.z = source->z.fixed;
	rnd = Random16();
	Vector_AddPolarOffset(rnd << 4, Random16(), &pos);
	obj = Object_Spawn(285, pos.x, pos.y, pos.z);
	if (obj != 0)
	{
		s32 mask;
		u8	flags;

		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_ParticleScript);
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
		mask = 13;
		flags = ((u8 *)&((struct AnimationObject *)obj->animation)->part[0])[9];
		mask = -mask;
		mask &= flags;
		mask |= 4;
		((u8 *)&((struct AnimationObject *)obj->animation)->part[0])[9] = mask;
	}
}

//--------------------------------------------------------------------------
// 効果が動いていれば 1、止まっていれば 0
//--------------------------------------------------------------------------
u32 EffectRuntime_IsActive(void)
{
	s16					  count;
	u32					  active = 0;
	struct EffectRuntime *runtime = (struct EffectRuntime *)gEventWork;

	if (runtime != NULL)
	{
		count = ((struct FieldWork *)runtime)->effect_active;
		active = (0 - count) | count;
		active >>= 31;
	}
	return active;
}

//--------------------------------------------------------------------------
// 操作キャラから横 8 マス、縦 5 マス以内にある番号 100〜239 の場所のうち、
// 種類 3 のイベントがあって、そのフラグが立っていないものに印を出す
// (position の上位 12 ビットが 0〜3 か 5 のものだけ)
//--------------------------------------------------------------------------
void FieldEffect_SpawnNearbyMarkers(void)
{
	u8					 *list;
	struct ObjectRuntime *actor;
	struct MapEventEntry *entry;
	s32					  actor_x;
	s32					  actor_z;
	s32					  x;
	s32					  z;
	s32					  id;

	// マップの場所の表：(x, z, 番号) の 3 バイトずつ、x と z が 0xff で終わり
	list = *(u8 **)(((u8 *)gMapWork[0]) + 16);
	actor = ObjectTable_Get(gGameState.selected_actor);
	actor_x = actor->x >> 20;
	actor_z = actor->z >> 20;
	if (list != 0)
	{
		while (x = *list++, z = *list++, x != 0xff || z != 0xff)
		{
			id = *list++;
			if (EffectRuntime_GetCurrentObject(id) != NULL)
				continue;
			if (id < 100 || id > 239)
				continue;
			if (actor_x - x >= 0)
			{
				if (actor_x - x > 8)
					continue;
			}
			else if (x - actor_x > 8)
				continue;
			if (actor_z - z >= 0)
			{
				if (actor_z - z > 5)
					continue;
			}
			else if (z - actor_z > 5)
				continue;
			entry = (struct MapEventEntry *)gOverlayArea.events();
			if (entry->flags == -1)
				continue;
			do
			{
				if (entry->id == id && (entry->flags & 15) == 3)
				{
					switch (entry->position & 0xfff00000)
					{
					case 0:
					case 0x100000:
					case 0x200000:
					case 0x300000:
					case 0x500000:
						if (entry->flag != -1 && GameFlag_TestFar(entry->flag) == 0)
						{
							/* FAKEMATCH: needed to match: the spawned object reuses the actor variable, which keeps both in r5 */
							actor = (struct ObjectRuntime *)Object_CreateFar(
								22, (x << 20) + (8 << 16), 0, (z << 20) + (8 << 16));
							if (actor != 0)
							{
								ObjectDispatch_InitializeFar(
									(struct DispatchObject *)actor, BattleFx_MarkerParticleScript);
								ObjectDispatch_SetSingleChildFlagsFar(actor, 0);
								((struct EfxObj *)actor)->proc =
									(void (*)(void))BattleFx_SpawnRandomParticleAtPosition;
							}
						}
						break;
					}
				}
				entry++;
			} while (entry->flags != -1);
		}
	}
}
