#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "animspr.h"
#include "motobj.h"
#include "object_runtime.h"
#include "types.h"
#include "objdisp.h"
#include "game_state.h"
#include "fixed_math.h"
#include "object_effect.h"
#include "object_lookup.h"
#include "scene.h"
#include "global_cells.h"
#include "effect_runtime.h"
#include "dma.h"
#include "io_reg.h"
#include "disptran.h"
#include "btlsys.h"
#include "layout_guard.h"
#include "callback_scheduler.h"
#include "frame.h"
#include "iwram_call.h"
#include "map_scroll.h"
#include "io_write_queue.h"
#include "ram_buffer.h"
#include "irq.h"
#include "battle_effect_runtime.h"
#include "palbuf.h"
#include "vram_block.h"
#include "heap_state.h"
#include "keystate.h"
#include "field_event.h"
#include "event_runtime.h"
#include "field_scene.h"
#include "fieldrun.h"
#include "party_state.h"
#include "battle_unit.h"
#include "item_ids.h"
#include "item.h"
#include "itemgift.h"
#include "text/msg_ids.h"
#include "inventory.h"
#include "ui.h"
#include "window.h"
#include "btlpty.h"
#include "b5_context.h"
#include "edition.h"
#include "resource_ids.h"
#include "field_sprite.h"
#include "facing_object.h"
#include "object_dispatch.h"
#include "script_motion.h"
#include "fieldobj.h"
#include "tbs_edition.h"

void ObjectMotion_SetActionVariant(s32 object_id, s32 priority);
s32 ObjectTable_ReadActiveValue(s32 key);

//--------------------------------------------------------------------------
// emitter.c
// フィールド効果：手に入れたアイテムの表示ときらめき、近くの印、ウィンドウの HBlank DMA
//--------------------------------------------------------------------------
void Ui_PrepareTransferForItemFar(s32 item);
void UiWork_FinalizeEntityMatchingLocalizedIdFar(s32);
void BattleFx_ApplyColorToSourceBuffer(s32 mode, s32 half);
void Object_SetModeById(u32 object_id, s32 action);

typedef struct MapEventEntry *(*EventListFn)(s32);

struct ParticlePosition
{
	s32 x;
	s32 y;
	s32 z;
};

LAYOUT_SIZE_GUARD(ParticlePosition_size, struct ParticlePosition, 0x0c);
LAYOUT_OFFSET_GUARD(ParticleEmitter_travel_offset_offset, struct ObjectRuntime, velocity_y, 0x28);
LAYOUT_OFFSET_GUARD(ParticleEmitter_active_offset, struct ObjectRuntime, flags, 0x55);
LAYOUT_OFFSET_GUARD(ParticleEffectObject_child_offset, struct ObjectRuntime, animation, 0x50);

extern void Vector_AddPolarOffset(s32, s32, struct ParticlePosition *);
extern void ObjectDispatch_ApplyArgumentToChildrenFar(struct ObjectRuntime *, s32);

// イベントのキャラ (向きと位置)
struct EfxSrc
{
	u8	pad0[6];
	u16 ang;
	s32 x;
	s32 y;
	s32 z;
};

struct EfxPos
{
	s32 x;
	s32 y;
	s32 z;
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
void		   ObjectDispatch_ReleaseFar(void *object);

// オブジェクト 64 個の表 (gEventWork の 88 バイト前)
#define EfxPool (*(struct EfxObj **)((u8 *)&gEventWork - 88))
#define BATTLE_ACTIVE_OFS 0xcb8 // gEventWork：効果が動いていれば 0 以外
struct EffectObject_0808f1c0;

void Object_SetMoveTargetFar(struct ObjectRuntime *, s32, s32, s32);

void ObjectDispatch_SetSingleChildFlagsFar(void *, s32);

struct EffectVisual_0808f1c0
{
	u8 unknown_00[5];
	u8 flags_a;
	u8 unknown_06[3];
	u8 flags_b;
	u8 unknown_0a[18];
	u8 value_1c; // VRAM のタイル枠
	u8 unknown_1d[9];
	u8 value_26;
	u8 value_27;
};

struct EffectResource_0808f1c0
{
	u8	unknown_00[8];
	s32 x;
	s32 y;
	s32 z;
};

struct EffectObject_0808f1c0
{
	u8							  unknown_00[80];
	struct EffectVisual_0808f1c0 *visual;
	u8							  unknown_54[24];
	void (*callback)(void);
};

struct Values_0808f28c
{
	u32 first;
	u32 second;
	u32 third;
};

struct Source_0808f28c
{
	u8					   padding[8];
	struct Values_0808f28c values; // x, y, z
};

extern struct ObjectRuntime *Object_Spawn(s32, u32, u32, u32);

// マップのイベント (flags が -1 で終わり)
struct MapEventEntry
{
	s32 flags; // 下位 4 ビットが種類
	s16 id;
	s16 flag;
	s32 position;
};

struct FieldMapState
{
	u8	unknown_00[36];
	s32 events;
};

extern struct DisplayTransitionState *gDisplayTransition;

// gEventWork + 0x11c の印の表 (最大 10 個、active が 0 で終わり)
struct ParticleCell
{
	u8 unknown_0[4];
	u8 active;
	u8 unknown_5;
	u8 x; // 16 ドットのマス
	u8 z;
};

s32 ArcTan2(s32, s32);

//--------------------------------------------------------------------------
// 操作キャラが印のマスの中心から 16 ドット以内にいれば、
// 中心から操作キャラの向きへ 20 ドット離れた位置へ出す
//--------------------------------------------------------------------------
void BattleFx_EmitRandomParticle(void)
{
	struct ObjectRuntime *actor = ObjectTable_Get(gGameState.selected_actor);
	struct ParticleCell	 *cell = (struct ParticleCell *)((u8 *)gEventWork + 0x11c);
	s32					  i = 0;
	s32					  x;
	s32					  z;
	s32					  cx;
	s32					  cz;
	s32					  dx;
	s32					  dz;

	if (cell->active != 0)
	{
		x = actor->x;
		z = actor->z;
		for (;;)
		{
			cx = cell->x << 20;
			dx = x - cx - (8 << 16);
			cz = cell->z << 20;
			dz = z - cz - (8 << 16);
			if ((dx >= -0xfffff && dx <= 0xfffff) && (dz >= -0xfffff && dz <= 0xfffff))
			{
				actor->x = cx + (8 << 16);
				actor->z = cz + (8 << 16);
				Vector_AddPolarOffset(
					20 << 16, (u16)ArcTan2(dz, dx), (struct ParticlePosition *)&actor->x);
				actor->target_x = 0x80000000; // 移動先なし
				actor->target_y = 0x80000000;
				actor->target_z = 0x80000000;
				return;
			}
			i++;
			cell++;
			if (i > 9 || cell->active == 0)
				break;
		}
	}
}

//--------------------------------------------------------------------------
// アイテム表示の毎フレームの処理：1/10 の確率で、周り 16 ドット以内にきらめきを出す
//--------------------------------------------------------------------------
void BattleFx_EmitRandomParticleFromEmitter(struct ObjectRuntime *emitter)
{
	struct ParticlePosition pos;
	struct ObjectRuntime   *obj;
	u32						random_angle;

	if (emitter->velocity_y >= -255 && emitter->velocity_y <= 255)
		emitter->flags = 0;

	if ((100 * Random16() >> 16) > 9)
		return;

	pos.x = emitter->x;
	pos.y = emitter->y;
	pos.z = emitter->z;
	random_angle = Random16();
	Vector_AddPolarOffset(random_angle << 4, Random16(), &pos);
	obj = (struct ObjectRuntime *)Object_Spawn(285, pos.x, pos.y, pos.z);
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

	s32			  src_z = (s32) * (struct EfxSrc **)(work + off);
	struct EfxPos pos;
	s32			  x;

	if (src_z == 0)
		return 0;

	pos.x = ((struct EfxSrc *)src_z)->x;
	pos.y = ((struct EfxSrc *)src_z)->y;
	pos.z = ((struct EfxSrc *)src_z)->z;
	Vector_AddPolarOffset(16 << 16, ((struct EfxSrc *)src_z)->ang, (struct ParticlePosition *)&pos);
	x = (pos.x & 0xfff00000) + (8 << 16);
	src_z = (pos.z & 0xfff00000) + (8 << 16);

	if (*(s16 *)(work + BATTLE_ACTIVE_OFS) != 0)
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
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)object, BattleFx_ParticleEmitterScript);
			FIELD_AT_OFFSET(object, u32 *, 0x28) = 0x20000; // travel
			FIELD_AT_OFFSET(object, u32 *, 0x48) = 0x4000;	// dy
			FIELD_AT_OFFSET(object, s32 *, 0x6c) =
				(s32)&BattleFx_EmitRandomParticleFromEmitter; // proc
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
	struct EffectResource_0808f1c0 *actor = ObjectTable_Get(gGameState.selected_actor);
	void						   *handle = Runtime_AllocateHeapBlock(17, 0x608);
	struct EffectObject_0808f1c0   *object =
		(struct EffectObject_0808f1c0 *)Object_CreateFar(22, actor->x, actor->y + (36 << 16), actor->z);

	if (object != 0)
	{
		struct EffectVisual_0808f1c0 *visual = object->visual;
		s32							  mask;

		s32 zero = 0;
		visual->value_26 = zero;
		visual->value_27 = zero;

		visual->flags_a &= zero - 33;

		mask = visual->flags_b & 0x0f;
		{
			s32 clear = 13;
			clear = -clear;
			mask &= clear;
		}
		mask |= 4;
		visual->flags_b = mask;

		Ui_PrepareTransferForItemFar(value);
		VramBlock_LoadCached(visual->value_1c, 128, (u8 *)handle + 0x400);
		Runtime_ReleaseHeapBlock(17);

		if (flags & 1)
			object->callback = (void (*)(void))BattleFx_EmitRandomParticleFromEmitter;
		if (flags & 2)
			EffectRuntime_PrepareRisingObject((struct ObjectRuntime *)object);

		WaitFrames(80);
		ObjectDispatch_ApplyArgumentToChildrenFar((struct ObjectRuntime *)actor, 1);
		ObjectDispatch_ReleaseFar(object);
	}
}

// ドイツ語・スペイン語・フランス語・イタリア語版はメニューを開いている間きらめきを止める
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)

#define PARTICLES_PAUSE_FOR_MENU 1
#endif

#if defined(PARTICLES_PAUSE_FOR_MENU)
#define BATTLE_ACTIVE_OFS 0xcb8
#define MENU_OPEN_OFS 0xcca // gEventWork：メニューを開いていれば 0 以外

struct ParticleSlot
{
	s32 script;
	u8	pad[0x6c];
};


void ObjectDispatch_ReleaseFar(void *object);

#define ParticlePool (*(struct ParticleSlot **)((u8 *)&gEventWork - 88))

//--------------------------------------------------------------------------
// 効果が動いていれば、きらめきのオブジェクトをすべて消す
//--------------------------------------------------------------------------
void BattleFx_ClearRandomParticles(void)
{
	if (*(s16 *)((u8 *)gEventWork + BATTLE_ACTIVE_OFS) != 0)
	{
		struct ParticleSlot *ent = ParticlePool;
		s32					 n = 63;

		do
		{
			if (ent->script != 0)
			{
				if (ent->script == (s32)BattleFx_ParticleScript)
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
void BattleFx_SpawnRandomParticleAtPosition(const struct Source_0808f28c *source)
{
	struct Values_0808f28c pos;
	struct ObjectRuntime  *obj;
	u32					   rnd;

#if defined(PARTICLES_PAUSE_FOR_MENU)
	if (*(s8 *)((u8 *)gEventWork + MENU_OPEN_OFS) != 0)
		return;
#endif
	if ((100 * Random16() >> 16) > 9)
		return;

	pos.first = source->values.first;
	pos.second = source->values.second;
	pos.third = source->values.third;
	rnd = Random16();
	Vector_AddPolarOffset(rnd << 4, Random16(), (struct ParticlePosition *)&pos);
	obj = Object_Spawn(285, pos.first, pos.second, pos.third);
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
		count = *(s16 *)((u8 *)runtime + 0xcb8);
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
			/* FAKEMATCH: the map's event list function is called through _call_via_r0 as a typed function */
			entry = ((EventListFn)_call_via_r0)(((struct FieldMapState *)&gOverlayArea)->events);
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
								*(s32 *)((u8 *)actor + 108) =
									(s32)BattleFx_SpawnRandomParticleAtPosition;
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

//--------------------------------------------------------------------------
// ウィンドウ 0/1 を有効にし、ページの値を WININ, WINOUT, WIN0H, WIN1H に入れてから
// 残りを HBlank ごとに DMA0 で WIN0H と WIN1H へ 1 ワードずつ送る
//--------------------------------------------------------------------------
void BattleFx_StartWindowHBlankDma(void)
{
	struct DisplayTransitionState *work = gDisplayTransition;
	u16							  *src = work->pages[work->page];
	volatile u16				  *channel = REG_DMA0;
	s32							   value;
	s32							   other;

	channel[5] &= 0xc5ff;
	channel[5] &= 0x7fff;
	(void)channel[5];
	REG_DISPCNT |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
	/* FAKEMATCH: needed to match: a do-while(0) around the WININ copy makes the reference load the value before the WININ address */
	do
	{
		value = *src++;
		REG_WININ = value;
	} while (0);
	value = *src++;
	REG_WINOUT = value;
	REG_WIN0H = *src++;
	other = *src++;
	REG_WIN1H = other;
	other = 160;
	REG_WIN0V = other;
	REG_WIN1V = other;
	Dma_Set(src, (void *)&REG_WIN0H, 0xa6600001, (volatile u32 *)channel);
}


struct TransitionSpan {
    u16 cnt;
    u16 val;
    u8 x0;
    u8 x1;
    u8 y0;
    u8 y1;
};

struct TransitionShape {
    u16 first;
    u16 second;
    struct TransitionSpan spans[3];
};

extern struct TransitionShape DisplayTransition_Shapes[];

void DisplayTransition_UpdateScanlineTable(void)
{
    struct DisplayTransitionState *state;
    struct TransitionSpan *span;
    struct TransitionShape *shape;
    struct MapScrollWork *map;
    u16 *dst;
    s32 *view7;
    s32 *view8;
    s32 *view9;
    s32 *view77;
    struct ObjectRuntime *obj;
    u32 i;
    s32 k;
    s32 cnt;
    s32 v;
    s32 rr;
    s32 d;
    u32 h;
    s32 lo;
    s32 hi;
    s32 cx;
    s32 cy;

    s32 ay;
    s32 bx;
    s32 by;
    u32 w;
    s32 e;

    map = gMapWork[0];
    state = gMapWork[23];

    if (state->duration != 0) {
        if (state->step >= state->duration) {
            state->duration = 0;
            if (state->next_mode == 0) {
                if (state->end == 64) {
                    REG_DISPCNT &= 0x81ff;
                } else {
                    REG_DISPCNT &= 0x9fff;
                }
                Scheduler_RemoveCallback((SchedulerCallback)BattleFx_StartWindowHBlankDma);
                Scheduler_RemoveCallback((SchedulerCallback)DisplayTransition_UpdateScanlineTable);
                {
                    volatile u16 *dma0 = REG_DMA0;

                    dma0[5] &= 0xc5ff;
                    dma0[5] &= 0x7fff;
                    dma0[5];
                }
                return;
            }
            state->mode = state->next_mode;
        } else {
            state->value = state->start
                + Iwram_SignedDivide((state->end - state->start) * ++state->step,
                                     state->duration);
        }
    }

    dst = state->pages[1 ^ state->page];
    UiWindow_FillFromSceneFar(dst + 2);

    switch (state->mode) {
    case 0:
        *dst++ = 0x7f7f;
        *dst++ = 1;
        v = state->value;
        if (v & 32) {
            v = 32 - (v & 31);
        } else {
            v = v & 31;
        }
        v = DisplayTransition_NoiseWidths[v];
        for (i = 0; i < 160; i++) {
            w = (Random16() * (u32)(241 - v)) >> 16;
            dst[0] = (u16)((w << 8) | (w + v));
            dst += 2;
        }
        break;

    case 1:
        v = state->value;
        shape = &DisplayTransition_Shapes[v & 31];
        if (v & 32) {
            *dst++ = shape->first;
            *dst++ = shape->second;
        } else {
            *dst++ = shape->second;
            *dst++ = shape->first;
        }
        for (i = 0; i < 3; i++) {
            span = &shape->spans[i];
            cnt = span->cnt;
            hi = span->val;
            if (cnt != 0) {
                if (hi != 0) {
                    for (k = 0; k < cnt; k++) {
                        dst[0] = (u16)hi;
                        dst += 2;
                    }
                } else {
                    rr = span->x0;
                    lo = span->y0;
                    d = span->x1;
                    hi = span->y1;
                    for (k = 0; k < cnt; k++) {
                        dst[0] = (u16)(((rr + (d - rr) * k / cnt) << 8)
                                       + (lo + (hi - lo) * k / cnt));
                        dst += 2;
                    }
                }
            }
        }
        break;

    case 2:
        v = state->value - 1;
        if (v & 32) {
            *dst++ = 1;
            *dst++ = 0x7f7f;
        } else {
            *dst++ = 0x7f7f;
            *dst++ = 1;
        }
        v &= 31;
        for (i = 0; i < 160; i++) {
            h = (v << 4) + ((16 * Random16()) >> 16);
            if (h > 255) {
                h = 255;
            }
            dst[0] = (u16)h;
            dst += 2;
        }
        break;

    case 3:
        v = state->value;
        *dst++ = 0x7f7f;
        *dst++ = 1;
        if ((u32)v > 32) {
            *dst++ = 1;
            *dst++ = 0x7f7f;
            v = 64 - v;
        }
        v = v * 5;
        rr = (v * v) << 16;
        for (i = 0; i < 160; i++) {
            v = i - 80;
            h = Iwram_Sqrt(rr - ((v * v) << 16)) >> 8;
            lo = 120 - h;
            hi = h + 120;
            if (lo < 0) {
                lo = 0;
            }
            if (hi < 0) {
                hi = 0;
            }
            if (lo > 240) {
                lo = 240;
            }
            if (hi > 240) {
                hi = 240;
            }
            dst[0] = (u16)((lo << 8) + hi);
            dst += 2;
        }
        break;

    case 4:
        v = state->value;
        if (v & 32) {
            *dst++ = 1;
            *dst++ = 0x7f7f;
        } else {
            *dst++ = 0x7f7f;
            *dst++ = 1;
        }
        v = (u32)(240 * (v & 31)) >> 5;
        for (i = 0; i < 160; i += 2) {
            e = ((240 - v) << 8) + 240;
            dst[0] = (u16)v;
            dst[2] = (u16)e;
            dst += 4;
        }
        break;

    case 7:
    {
        s32 ax;

        view7 = &map->view_x;
        ax = view7[0] & 0xffff0000;
        ay = view7[1] & 0xffff0000;
        obj = ObjectTable_Get(gGameState.selected_actor);
        cx = (obj->x - ax) / 0x10000;
        cy = ((obj->z - obj->y) - ay) / 0x10000 - 16;
        // the frame counter is read here, where mode 8 forms its index, and the value is not used
        gFrameCount;
        *dst++ = state->active;
        *dst++ = state->mask;
        v = state->value;
        if ((v & 32) == 0) {
            v = 32 - (v & 31);
        } else {
            v = v & 31;
        }
        if (gFrameCount & 1) {
            v = 0;
        }
        v = v * 5;
        rr = (v * v) << 16;
        for (i = 0; i < 160; i++) {
            v = i - cy;
            h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
            lo = cx - h;
            hi = cx + h;
            if (lo < 0) {
                lo = 0;
            }
            if (hi < 0) {
                hi = 0;
            }
            if (lo > 240) {
                lo = 240;
            }
            if (hi > 240) {
                hi = 240;
            }
            dst[0] = (u16)((lo << 8) + hi);
            dst += 2;
        }
        break;

    }
    case 8:
        view8 = &map->view_x;
        bx = view8[0] & 0xffff0000;
        by = view8[1] & 0xffff0000;
        cx = (state->centre_x - bx) / 0x10000;
        cy = (state->centre_y - by) / 0x10000 - 16;
        k = -cy * 2 + gFrameCount;
        *dst++ = state->mask;
        *dst++ = state->active;
        v = state->value;
        if (v & 32) {
            v = 32 - (v & 31);
        } else {
            v = v & 31;
        }
        v = v * 5;
        rr = (v * v) << 16;
        for (i = 0; i < 160; i++) {
            v = i - cy;
            h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
            lo = cx - h;
            hi = cx + h;
            if (lo < hi) {
                d = DisplayTransition_EdgeOffsets[k & 31];
                lo -= d;
                hi += d;
                if (lo >= hi) {
                    lo = 240;
                    hi = 240;
                }
            }
            if (lo < 0) {
                lo = 0;
            }
            if (hi < 0) {
                hi = 0;
            }
            if (lo > 240) {
                lo = 240;
            }
            if (hi > 240) {
                hi = 240;
            }
            dst[0] = (u16)((lo << 8) + hi);
            dst += 2;
            k += 2;
        }
        break;

    case 9:
    {
        s32 ax;

        view9 = &map->view_x;
        ax = view9[0] & 0xffff0000;
        ay = view9[1] & 0xffff0000;
        obj = ObjectTable_Get(gGameState.selected_actor);
        cx = (obj->x - ax) / 0x10000;
        cy = ((obj->z - obj->y) - ay) / 0x10000 - 16;
        // the frame counter is read here, where mode 8 forms its index, and the value is not used
        gFrameCount;
        *dst++ = state->mask;
        *dst++ = state->active;
        v = state->value;
        if (v & 32) {
            v = 32 - (v & 31);
        } else {
            v = v & 31;
        }
        v = v * 5;
        rr = (v * v) << 16;
        for (i = 0; i < 160; i++) {
            v = i - cy;
            h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
            lo = cx - h;
            hi = cx + h;
            if (lo < 0) {
                lo = 0;
            }
            if (hi < 0) {
                hi = 0;
            }
            if (lo > 240) {
                lo = 240;
            }
            if (hi > 240) {
                hi = 240;
            }
            dst[0] = (u16)((lo << 8) + hi);
            dst += 2;
        }
        break;

    }
    case 77:
    {
        s32 ax;

        view77 = &map->view_x;
        ax = view77[0] & 0xffff0000;
        ay = view77[1] & 0xffff0000;
        obj = ObjectTable_Get(gGameState.selected_actor);
        cx = (obj->x - ax) / 0x10000;
        cy = ((obj->z - obj->y) - ay) / 0x10000 - 8;
        // the frame counter is read here, where mode 8 forms its index, and the value is not used
        gFrameCount;
        *dst++ = state->mask;
        *dst++ = state->active;
        v = state->value;
        if (v & 32) {
            v = 32 - (v & 31);
        } else {
            v = v & 31;
        }
        v = v * 5;
        rr = (v * v) << 16;
        for (i = 0; i < 160; i++) {
            v = i - cy;
            h = Iwram_Sqrt(rr - ((v * v * 3) << 14)) >> 8;
            lo = cx - h;
            hi = cx + h;
            if (lo < 0) {
                lo = 0;
            }
            if (hi < 0) {
                hi = 0;
            }
            if (lo > 240) {
                lo = 240;
            }
            if (hi > 240) {
                hi = 240;
            }
            dst[0] = (u16)((lo << 8) + hi);
            dst += 2;
        }
        break;
    }
    }

    state->page ^= 1;
}

void DisplayTransition_InitializeBattleEffectState(s32 mode)
{
    struct DisplayTransitionState *state = Runtime_AllocateBlock(31, sizeof(*state));
    volatile u32 zero;

    zero = 0;
    Dma_Set((const void *)&zero, state, 0x85000000 | (sizeof(*state) / 4), REG_DMA3);
    state->mode = mode;
    state->value = 0;
    state->mask = 0x3f3f;
    state->active = 1;
    Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
    Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
}

void BattleFx_EnableTwoCallbacks(void)
{
    Scheduler_EnableCallbacks(&DisplayTransition_UpdateScanlineTable);
    Scheduler_EnableCallbacks(&BattleFx_StartWindowHBlankDma);
}

struct DisplayTransitionState *DisplayTransition_AllocateAndClearState(void)
{
    volatile u32 clear_value;
    struct DisplayTransitionState *state;

    state = Runtime_AllocateBlock(31, sizeof(*state));
    clear_value = 0;
    Dma_Set((const void *)&clear_value, state, 0x85000000 | (sizeof(*state) / 4), REG_DMA3);
    return state;
}

void WaitFrames(s32 frames);

extern volatile u32 gFrameCount;

void DisplayTransition_Start(s32 mode, s32 frames)
{
    void **work;
    struct DisplayTransitionWindow *display;
    s32 value;
    s32 kind;

    /* FAKEMATCH: needed to match: the map-work word prefix preserves address order. */
    kind = (mode >> 8) & 0xff;
    work = Ram_MapWork;
    display = *(struct DisplayTransitionWindow **)work;
    value = mode & 0xff;
    switch (kind) {
    case 0:
        Blend_SetDarkenTarget16(0);
        Blend_SetDarkenTarget0(frames);
        WaitFrames(1);
        break;
    case 1:
        BattleFx_ApplyColorToSourceBuffer(0x8000, PLTT_BACKDROP);
        BattleFx_StartBufferInterpolation(frames);
        WaitFrames(1);
        IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, 0x20000);
        BattleFx_SetPrimaryBufferValue(0);
        return;
    case 2: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        state->mode = value;
        state->value = 0;
        state->mask = 63;
        state->active = 1;
        Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
        Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
        WaitFrames(1);
        IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, 0x20000);
        state->start = 0;
        state->end = 32;
        state->duration = frames;
        state->step = 0;
        return;
    }
    case 3: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        state->mode = value;
        state->value = 32;
        DisplayTransition_FillTilemapAndSolidTile(15);
        WaitFrames(1);
        Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
        IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, 0x20000);
        state->start = 0;
        state->end = 32;
        state->duration = frames;
        state->step = 0;
        return;
    }
    case 4: {
        struct DisplayTransitionState *state;

        display = (struct DisplayTransitionWindow *)*work;
        state = DisplayTransition_AllocateAndClearState();
        display->first_line = 80;
        display->second_line = 80;
        WaitFrames(1);
        if (value == 0) {
            Scheduler_AddOrUpdateCallback(DisplayTransition_Update, 0xc80);
            Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
            state->start = 80;
            state->end = 0;
            state->duration = frames;
            state->step = 0;
        } else {
            Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFromCentre, 0xc80);
            Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
            state->start = 80;
            state->end = 0;
            state->duration = frames;
            state->step = 0;
        }
        break;
    }
    }
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, 0x20000);
}

void DisplayTransition_Finish(s32 mode, s32 frames)
{
    s32 value;
    s32 kind;

    kind = (mode >> 8) & 0xff;
    value = mode & 0xff;
    switch (kind) {
    case 0:
        Blend_SetDarkenTarget0(0);
        Blend_SetDarkenTarget16(frames);
        break;
    case 1:
        BattleFx_ApplyColorToTargetBuffer(0x8000, 0);
        BattleFx_StartBufferInterpolation(frames);
        break;
    case 2: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        state->mode = value;
        state->value = 32;
        state->mask = 63;
        state->active = 1;
        Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
        Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
        WaitFrames(1);
        state->start = 32;
        state->end = 64;
        state->duration = frames;
        state->step = 0;
        break;
    }
    case 3: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        state->mode = value;
        state->value = 32;
        DisplayTransition_FillTilemapAndSolidTile(0);
        WaitFrames(1);
        Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
        state->start = 32;
        state->end = 64;
        state->duration = frames;
        state->step = 0;
        break;
    }
    case 4: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        if (value == 0) {
            Scheduler_AddOrUpdateCallback(DisplayTransition_Update, 0xc80);
            Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
            state->start = 0;
            state->end = 80;
            state->duration = frames;
            state->step = 0;
        } else {
            Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFromCentre, 0xc80);
            Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
            state->start = 0;
            state->end = 80;
            state->duration = frames;
            state->step = 0;
        }
        break;
    }
    }
}

void DisplayState_ClearFlags(s32 clear_0800, s32 clear_0400, s32 clear_0200)
{
    struct DisplayTransitionWindow *state;

    state = gMapWork[0];
    if (state != NULL) {
        if (clear_0200 != 0) {
            state->dispcnt &= 0xFDFF;
        }
        if (clear_0400 != 0) {
            state->dispcnt &= 0xFBFF;
        }
        if (clear_0800 != 0) {
            state->dispcnt &= 0xF7FF;
        }
    }
}

void DisplayTransition_Update(void)
{
    struct DisplayTransitionFrame *state =
        *Ram_DisplayWork;
    struct DisplayTransitionWindow *display =
        *(struct DisplayTransitionWindow **)Ram_MapWork;
    s8 *duration = &state->duration;
    u32 display_value;

    if (*duration != 0) {
        s8 *step = &state->step;

        if (*step >= *duration) {
            *duration = 0;
            Scheduler_RemoveCallback(DisplayTransition_Update);
            Runtime_SetIrqHandler(1, 0, 0);
            return;
        } else {
            s32 delta = state->end - state->start;
            s32 value;

            (*step)++;
            value = Iwram_SignedDivide(delta * *step, *duration);
            state->value = state->start + value;
        }
    }

    display_value = state->value;
    if (display_value > 79) {
        display->first_line = 200;
        display->second_line = 250;
    } else {
        display->first_line = display_value;
        display->second_line = 159 - display_value;
    }
}

void DisplayTransition_UpdateFromCentre(void)
{
    struct DisplayTransitionFrame *state =
        *Ram_DisplayWork;
    struct DisplayTransitionWindow *window =
        *(struct DisplayTransitionWindow **)Ram_MapWork;
    s8 *duration = &state->duration;
    u16 value;

    if (*duration != 0) {
        s8 *step = &state->step;

        if (*step >= *duration) {
            *duration = 0;
            Scheduler_RemoveCallback(DisplayTransition_UpdateFromCentre);
            Runtime_SetIrqHandler(1, 0, 0);
            return;
        } else {
            s32 delta = state->end - state->start;
            s32 offset;

            (*step)++;
            offset = Iwram_SignedDivide(delta * *step, *duration);
            state->value = state->start + offset;
        }
    }

    value = state->value;
    if (value > 79) {
        window->first_line = 200;
        window->second_line = 250;
    } else if (value != 0 && (gFrameCount & 1)) {
        window->first_line = value + 80;
        window->second_line = 80 - value;
    } else {
        window->first_line = 0;
        window->second_line = 159;
    }
}

void DisplayTransition_UpdateScanline(void)
{
    u32 line;
    struct DisplayTransitionWindow *state;
    u16 value;

    line = REG_VCOUNT;
    state = gMapWork[0];

again:
    switch (state->irq_phase) {
    case 3:
        if (line >= state->irq_first_line) {
            value = REG_DISPCNT;
            REG_DISPCNT = (value & 0xFFF8) | 2;
            value = 9;
            state->irq_phase = value;
        }
        break;
    case 2:
        if (line >= state->irq_second_line) {
            value = REG_DISPCNT;
            REG_DISPCNT = value & 0xFFF8;
            value = 9;
            state->irq_phase = value;
        }
        break;
    case 1:
        if (line >= state->irq_first_line) {
            value = REG_DISPCNT;
            REG_DISPCNT = (value & 0xFFF8) | 2;
            state->irq_phase++;
            goto again;
        }
        if (line >= state->irq_second_line) {
            value = REG_DISPCNT;
            REG_DISPCNT = value & 0xFFF8;
            value = 3;
            state->irq_phase = value;
            goto again;
        }
        break;
    case 0:
        if (line <= 158) {
            value = 1;
            state->irq_phase = value;
            goto again;
        }
        break;
    }
}

void DisplayTransition_UpdateFrame(void)
{
    struct DisplayTransitionFrame *state = *Ram_DisplayWork;
    s32 value;
    s32 blend;
    u32 i;

    if (state->duration != 0) {
        if (state->step >= state->duration) {
            volatile u16 *dma0;
            state->duration = 0;
            Scheduler_RemoveCallback(DisplayTransition_UpdateFrame);
            dma0 = REG_DMA0;
            dma0[5] &= 0xc5ff;
            dma0[5] &= 0x7fff;
            (void)dma0[5];
            return;
        } else {
            s32 delta = state->end - state->start;
            state->step++;
            state->value = state->start
                + Iwram_SignedDivide(delta * state->step, state->duration);
        }
    }

    value = state->value - 1;
    state->dither_toggle ^= 1;
    blend = 0;
    if (value & 32)
        blend = 15;
    value = (value & 31) * 2;
    for (i = 0; i <= 1; i++) {
        u8 entry = DisplayTransition_DitherTable[value & 63];
        u8 *p = &state->palette_nibbles[entry >> 1];
        if (entry & 1)
            *p = (*p & 0x0f) | (blend << 4);
        else
            *p = (*p & 0xf0) | blend;
        value++;
    }

    IO_WRITE_QUEUE_PUSH((u32)BG_CHAR_BLOCK(0), (u32)state->palette_nibbles, 0x84000008);
}

s32 GameFlag_TestFar(s32 flag);

typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);

void DisplayTransition_FillTilemapAndSolidTile(s32 color)
{
    struct DisplayTransitionState *work = gDisplayTransition;
    volatile u32 fill = 0xf000f000;
    Dma_Set((const void *)&fill, BG_SCREEN_BLOCK(4), 0x85000140, REG_DMA3);
    if (color != -1) {
        u32 pattern = 0;
        s32 cnt;
        u32 *tile;
        for (cnt = 7; cnt >= 0; --cnt) pattern = (pattern << 4) | color;
        tile = work->solid_tile;
        for (cnt = 7; cnt >= 0; --cnt) *tile++ = pattern;
        Dma_Set(work->solid_tile, BG_CHAR_BLOCK(0), 0x84000008, REG_DMA3);
    }
}

void DisplayTransition_InitializeState(s32 value)
{
    struct DisplayTransitionState *state;
    volatile u32 zero;

    state = Runtime_AllocateBlock(0x1f, sizeof(*state));
    zero = 0;
    Dma_Set((const void *)&zero, state, 0x85000000 | (sizeof(*state) / 4), REG_DMA3);
    DisplayTransition_FillTilemapAndSolidTile(0);
    state->mode = value;
    state->value = 0;
    Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
    WaitFrames(0x78);
}

void BattleFx_InterpolateBuffers(s16 *source, s16 *target, s16 *step, s32 frames)
{
    s32 index;
    s32 first;
    s32 second;

    if (frames > 0) {
        index = 0x53F;
        do {
            first = *source;
            second = *target;
            *step = Iwram_SignedDivide(second - first, frames);
            index--;
            source++;
            target++;
            step++;
        } while (index >= 0);
    }
}

void BattlePalette_UpdateBlend(void)
{
    /* FAKEMATCH: needed to match: the packed bank is selected by its byte offset from the work; direct packed-bank selection swaps the queue and IME registers. */
    struct BattleEffectBuffers *work = gPaletteBufferWork;
    u16 *add = work->delta;
    u8 *base;
    s32 i;

    if (GameFlag_TestFar(0x152) != 0) {
        return;
    }
    if (work->duration == 0) {
        return;
    }
    if (++work->step < work->duration) {
        u16 *sum = work->current;
        for (i = 0; i < 0x540; i++) {
            *sum++ += *add++;
        }
    } else {
        Iwram_CopyWords(work->current, work->target, sizeof(work->current));
        work->duration = 0;
    }

    {
        u16 *packed = work->packed[1 ^ work->page];
        s32 blue = 0x7c00;
        u16 *current;

        i = 0x1c0;
        current = work->current;
        for (; i != 0; i--) {
            *packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0) | (((s16)current[2] >> 10) & 0x1f);
            current += 3;
        }
    }

    work->page ^= 1;
    base = (u8 *)work + work->page * sizeof(work->packed[0]);
    {
        u32 bg = (u32)(base + 0x2300);
        IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, bg, 0x84000070);
    }
    IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(base + 0x24c0), 0x84000070);
}

//--------------------------------------------------------------------------
// bufcolor.c
// エナジー演出：パレットの色変え (目標の色を作り、少しずつ近づける)
//--------------------------------------------------------------------------

#define HEAP_SLOT_PALBUF 32 // 色のバッファ (gPaletteBufferWork)

#define COLOR_MAX 448 // BG 224 色 + OBJ 224 色
#define COLOR_HALF 224

s32 BattleFx_ClampRgb555Channel(s32 value);
s32 BattleFx_ClampRgb555Component(s32 value);

// 5 ビットの値を、バッファの 10 ビット上げた値に写す表
extern u16 BattlePalette_ChannelCurve[];
extern u16 BattlePalette_ChannelCurveLight[];
extern u16 BattlePalette_ChannelCurveContrast[];

//--------------------------------------------------------------------------
// 元の色 src から、色変えの目標 dst を作る
// dst は一色に B, G, R の三つを、5 ビットを 10 ビット上げた値で持つ
// mode : 0x8000 未満 = その色一色、0x8000 = 背景色一色、
//        PALETTE_TRANSFORM_* = 色の変換、それ以外 = 作ってある表のアドレス
// half : 0 = すべて、1 = BG の 224 色、2 = OBJ の 224 色
//--------------------------------------------------------------------------
void BattleFx_BuildBuffer(u32 mode, u16 *src, u16 *dst, s32 half)
{
	u32 i;
	u32 cnt = COLOR_MAX;
	s32 r;
	s32 b;
	u32 c;
	s32 g;

	if (mode == PALETTE_TRANSFORM_FROM_SOURCE)
		mode = *(u16 *)BG_PLTT;
	if (half == 1)
	{
		cnt = COLOR_HALF;
	}
	else if (half == 2)
	{
		dst += COLOR_HALF * 3;
		src += COLOR_HALF;
		cnt = COLOR_HALF;
	}

	// 一色：最初の一色を書き、DMA で後ろへ広げる
	if (mode < PALETTE_TRANSFORM_FROM_SOURCE)
	{
		*dst++ = mode & 0x7c00;
		*dst++ = (mode & 0x03e0) << 5;
		*dst++ = (mode & 0x001f) << 10;
		Dma_Set(dst - 3, dst, (((cnt - 1) * 6) >> 1) | DMA_ENABLE32, REG_DMA3);
	}
	else if (mode < PALETTE_TRANSFORM_FLAGS)
	{
		switch (mode)
		{
		case PALETTE_TRANSFORM_GREY: // 明るさ (2R + 4G + B) / 7 の灰色
			for (i = 0; i < cnt; i++)
			{
				c = *src++;
				r = ((c << 11) & 0xf800);
				g = ((c << 7) & 0x1f000);
				b = (c & 0x7c00);
				c = Iwram_SignedDivide(r + g + b, 7);
				*dst++ = c;
				*dst++ = c;
				*dst++ = c;
			}
			break;
		case PALETTE_TRANSFORM_SEPIA: // 明るさから R を強めた色、8-28 に収める
			for (i = 0; i < cnt; i++)
			{
				c = *src++;
				r = c & 31;
				c = Iwram_SignedDivide(r + ((c >> 5) & 31) + ((c >> 10) & 31), 10);
				r = c * 4 + 5;
				b = c * 3 + 5;
				g = b;
				if (r < 8)
					r = 8;
				if (g < 8)
					g = 8;
				if (b < 8)
					b = 8;
				if (r > 28)
					r = 28;
				if (g > 28)
					g = 28;
				if (b > 28)
					b = 28;
				*dst++ = BattlePalette_ChannelCurveLight[b];
				*dst++ = BattlePalette_ChannelCurveLight[g];
				*dst++ = BattlePalette_ChannelCurveLight[r];
			}
			break;
		case PALETTE_TRANSFORM_WARM: // R を半分 +10、G を 2/3 +8、B を -7 (黒はそのまま)
			for (i = 0; i < cnt; i++)
			{
				c = *src++;
				r = c & 31;
				g = (c >> 5) & 31;
				b = (c >> 10) & 31;
				if ((r | g | b) != 0)
				{
					r -= (u32)r >> 1;
					g -= g / 3;
					r += 10;
					g += 8;
					b -= 7;
					r = BattleFx_ClampRgb555Channel(r);
					g = BattleFx_ClampRgb555Channel(g);
					b = BattleFx_ClampRgb555Channel(b);
				}
				*dst++ = BattlePalette_ChannelCurveContrast[b];
				*dst++ = BattlePalette_ChannelCurveLight[g];
				*dst++ = BattlePalette_ChannelCurve[r];
			}
			break;
		case PALETTE_TRANSFORM_MUTED: // 範囲を狭め、G と B を +2
			for (i = 0; i < cnt; i++)
			{
				c = *src++;
				r = c & 31;
				g = (c >> 5) & 31;
				b = (c >> 10) & 31;
				if (r < 10)
					r = 10;
				if (g < 16)
					g = 16;
				if (b < 16)
					b = 16;
				if (r > 28)
					r = 28;
				if (g > 24)
					g = 24;
				if (b > 26)
					b = 26;
				g += 2;
				b += 2;
				r = BattleFx_ClampRgb555Channel(r);
				g = BattleFx_ClampRgb555Channel(g);
				b = BattleFx_ClampRgb555Channel(b);
				*dst++ = BattlePalette_ChannelCurve[b];
				*dst++ = BattlePalette_ChannelCurve[g];
				*dst++ = BattlePalette_ChannelCurve[r];
			}
			break;
		case PALETTE_TRANSFORM_PALE: // 半分にして平均を足す
			for (i = 0; i < cnt; i++)
			{
				s32 mean;

				c = *src++;
				r = c & 31;
				g = (c >> 5) & 31;
				b = (c >> 10) & 31;
				mean = BattleFx_ClampRgb555Channel((r + g + b) / 3);
				r = (r >> 1) + mean;
				g = (g >> 1) + mean;
				b = (b >> 1) + mean;
				r = BattleFx_ClampRgb555Channel(r);
				g = BattleFx_ClampRgb555Channel(g);
				b = BattleFx_ClampRgb555Channel(b);
				dst[0] = BattlePalette_ChannelCurveContrast[b];
				dst[1] = BattlePalette_ChannelCurveContrast[g];
				dst[2] = BattlePalette_ChannelCurveContrast[r];
				dst += 3;
			}
			break;
		case PALETTE_TRANSFORM_RED: // G と B の 1/8 を R へ、G と B は 2/3
			for (i = 0; i < cnt; i++)
			{
				c = *src++;
				r = c & 31;
				g = (c >> 5) & 31;
				b = (c >> 10) & 31;
				r += ((g >> 3) + (b >> 3));
				r = BattleFx_ClampRgb555Channel(r);
				g -= g / 3;
				b -= b / 3;
				dst[0] = BattlePalette_ChannelCurve[b];
				dst[1] = BattlePalette_ChannelCurve[g];
				dst[2] = BattlePalette_ChannelCurveLight[r];
				dst += 3;
			}
			break;
		case PALETTE_TRANSFORM_WARM2: // R を半分 +6、G を 2/3 +4、B を -6
			for (i = 0; i < cnt; i++)
			{
				c = *src++;
				r = c & 31;
				g = (c >> 5) & 31;
				b = (c >> 10) & 31;
				r -= (u32)r >> 1;
				g -= g / 3;
				r += 6;
				g += 4;
				b -= 6;
				r = BattleFx_ClampRgb555Channel(r);
				g = BattleFx_ClampRgb555Channel(g);
				b = BattleFx_ClampRgb555Channel(b);
				dst[0] = BattlePalette_ChannelCurveContrast[b];
				dst[1] = BattlePalette_ChannelCurveLight[g];
				dst[2] = BattlePalette_ChannelCurve[r];
				dst += 3;
			}
			break;
		default: // そのまま
			for (i = 0; i < cnt; i++)
			{
				c = *src++;
				dst[i * 3 + 0] = c & 0x7c00;
				dst[i * 3 + 1] = (c & 0x03e0) << 5;
				dst[i * 3 + 2] = (c & 0x001f) << 10;
			}
			break;
		}
	}
	else if (mode & PALETTE_TRANSFORM_TINT)
	{
		// 明るさ (2R + 4G + B) に、mode の色をかける
		u32 tr;
		u32 tg;
		u32 tb;
		s32 r;
		s32 g;

		tr = mode & 31;
		tg = (mode >> 5) & 31;
		tb = (mode >> 10) & 31;
		for (i = 0; i < cnt; i++)
		{
			c = *src++;
			r = ((c << 11) & 0xf800);
			g = ((c << 7) & 0x1f000);
			b = (c & 0x7c00);
			c = Iwram_SignedDivide(r + g + b, 96);
			r = c * tr;
			g = c * tg;
			b = c * tb;
			r = BattleFx_ClampRgb555Component(r);
			g = BattleFx_ClampRgb555Component(g);
			b = BattleFx_ClampRgb555Component(b);
			dst[0] = b;
			dst[1] = g;
			dst[2] = r;
			dst += 3;
		}
	}
	else if (mode & PALETTE_TRANSFORM_BLEND)
	{
		// 明るさ R + G + B を、mode の色の比で配り直す
		u32 tr;
		u32 tg;
		u32 tb;
		s32 nr;
		s32 ng;
		s32 nb;

		tr = mode & 31;
		tg = (mode >> 5) & 31;
		tb = (mode >> 10) & 31;
		for (i = 0; i < cnt; i++)
		{
			c = *src++;
			r = c & 31;
			c = Iwram_SignedDivide((r + ((c >> 5) & 31) + ((c >> 10) & 31)) << 4, tr + tg + tb);
			nr = Iwram_MulQ16(((c * tr) >> 4) << 16, (s32)(tr << 16) >> 4);
			ng = Iwram_MulQ16(((c * tg) >> 4) << 16, (s32)(tg << 16) >> 4);
			nb = Iwram_MulQ16(((c * tb) >> 4) << 16, (s32)(tb << 16) >> 4);
			nr = BattleFx_ClampRgb555Channel((u32)nr >> 16);
			ng = BattleFx_ClampRgb555Channel((u32)ng >> 16);
			nb = BattleFx_ClampRgb555Channel((u32)nb >> 16);
			*dst++ = BattlePalette_ChannelCurve[nb];
			*dst++ = BattlePalette_ChannelCurve[ng];
			*dst++ = BattlePalette_ChannelCurve[nr];
		}
	}
	else if (mode & PALETTE_TRANSFORM_SPLIT)
	{
		for (i = 0; i < cnt; i++)
		{
			c = *src++;
			dst[i * 3 + 0] = c & 0x7c00;
			dst[i * 3 + 1] = (c & 0x03e0) << 5;
			dst[i * 3 + 2] = (c & 0x001f) << 10;
		}
	}
	else
	{
		// 作ってある表を写す
		if (half == 2)
			mode += COLOR_HALF * 3 * sizeof(u16);
		Dma_Set((const void *)mode, dst, ((cnt * 6) >> 2) | DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
	}
}

//--------------------------------------------------------------------------
// 色のバッファを用意し、今の BG と OBJ のパレットを写す
//--------------------------------------------------------------------------
void BattleEffect_InitializeBuffers(void)
{
	volatile s32				zero;
	struct BattleEffectBuffers *buf;

	buf = Runtime_AllocateBlock(HEAP_SLOT_PALBUF, sizeof(*buf));
	zero = 0;
	Dma_Set((const void *)&zero, buf, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | (sizeof(*buf) >> 2), REG_DMA3);
	Dma_Set(
		(void *)BG_PLTT, &buf->palette[0], DMA_ENABLE32 | DMA_32BIT | (COLOR_HALF / 2), REG_DMA3);
	Dma_Set((void *)OBJ_PLTT, &buf->palette[COLOR_HALF],
		DMA_ENABLE32 | DMA_32BIT | (COLOR_HALF / 2), REG_DMA3);
	BattleFx_BuildBuffer(PALETTE_TRANSFORM_PLAIN, buf->palette, buf->target, 0);
	Scheduler_AddOrUpdateCallback((void (*)(void))BattlePalette_UpdateBlend, 0xc8f);
}

//--------------------------------------------------------------------------
// 色変えをやめ、バッファを返す
//--------------------------------------------------------------------------
void Runtime_ScheduleCallbackAndReleaseBlock32B(void)
{
	Scheduler_RemoveCallback(&BattlePalette_UpdateBlend);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_PALBUF);
}

//--------------------------------------------------------------------------
// 目標の色を作る
//--------------------------------------------------------------------------
void BattleFx_ApplyColorToTargetBuffer(s32 mode, s32 half)
{
	struct BattleEffectBuffers *buf = gPaletteBufferWork;

	if (buf != NULL)
	{
		BattleFx_BuildBuffer(mode, buf->palette, buf->target, half);
	}
}

//--------------------------------------------------------------------------
// 今の色を作る
//--------------------------------------------------------------------------
void BattleFx_ApplyColorToSourceBuffer(s32 mode, s32 half)
{
	struct BattleEffectBuffers *buf = gPaletteBufferWork;

	if (buf != NULL)
	{
		BattleFx_BuildBuffer(mode, buf->palette, buf->current, half);
	}
}

//--------------------------------------------------------------------------
// 元の背景色を変える
//--------------------------------------------------------------------------
void BattleFx_SetPrimaryBufferValue(u32 col)
{
	struct BattleEffectBuffers *buf = gPaletteBufferWork;

	if (buf != NULL)
	{
		buf->palette[0] = col;
	}
}

//--------------------------------------------------------------------------
// 今の色から目標の色へ、frames フレームで変え始める
//--------------------------------------------------------------------------
void BattleFx_StartBufferInterpolation(s32 frames)
{
	struct BattleEffectBuffers *buf = gPaletteBufferWork;

	if (buf != NULL)
	{
		buf->duration = frames;
		buf->step = 0;
		BattleFx_InterpolateBuffers(buf->current, buf->target, buf->delta, frames);
	}
}

//--------------------------------------------------------------------------
// 5 ビットの色の値を 0-31 に収める
//--------------------------------------------------------------------------
s32 BattleFx_ClampRgb555Channel(s32 value)
{
	if (value > 31)
		return 31;
	if (value < 0)
		value = 0;
	return value;
}

//--------------------------------------------------------------------------
// 10 ビット上げた色の値を 31 << 10 までに収める
//--------------------------------------------------------------------------
s32 BattleFx_ClampRgb555Component(s32 value)
{
	if (value > 31744)
		value = 31744;
	return value;
}


struct FadeMark
{
	void *next;
	u16	  y : 8;
	u16	  affine : 2;
	u16	  mode : 2;
	u16	  mosaic : 1;
	u16	  colors : 1;
	u16	  shape : 2;
	u16	  x : 9;
	u16	  param : 5;
	u16	  size : 2;
	u16	  tile : 10;
	u16	  priority : 2;
	u16	  palette : 4;
	u8	  unknown_0a[2];
};

struct FadeActor
{
	u8	unknown_00[8];
	s32 x;
	s32 y;
	s32 z;
	u8	unknown_14[2];
	s16 floor;
	u8	unknown_18[10];
	u8	layer;
};

struct FadeWork
{
	struct FadeMark	  marks[2];
	struct FadeActor *actor;
};

struct FadeView
{
	u8	unknown_00[228];
	s16 camera[4];
};

void Object_EffectSpawnCallback(void)
{
	struct FadeWork *work = WORK_SLOT(36);
	s16				 *camera = ((struct FadeView *)WORK_SLOT(8))->camera;
	s32				  camera_x = camera[1];
	s32				  camera_z = camera[3];
	struct FadeMark	 *mark = work->marks;
	struct FadeActor *actor = work->actor;
	s32				  x;
	s32				  z;
	s32				  floor;
	s32				  layer;
	u32				  tile;
	s32				  height;
	s32				  other;
	s32				  row;
	/* FAKEMATCH: needed to match: nothing reads or writes these four bytes */
	s16 unused[2];

	if (actor == 0)
		return;
	x = actor->x;
	z = actor->z;
	floor = actor->floor;
	layer = actor->layer;
	tile = gVramBlockCache[94].offset >> 5;
	x -= 0x80000;
	height = Map_GetTerrainHeightFar(layer, x, z + 0x100000) >> 16;
	other = (Map_GetTerrainHeightFar(layer, x, z + 0x200000) >> 16) - 16;
	if (other > height)
		height = other;
	if (height > 0 && height > floor)
	{
		((u32 *)mark)[1] = 0x40000800;
		((u32 *)mark)[2] = 0x400;
		mark->priority = 0;
		mark->tile = tile;
		mark->mode = 1;
		mark->x = ((u32)(x >> 16) & 0xfff0) - camera_x;
		row = (z >> 16) & 0xf0;
		mark->y = row - camera_z - height + 16;
		Runtime_PushSlotEntry(mark, 0);
	}
	x += 0x100000;
	height = Map_GetTerrainHeightFar(layer, x, z + 0x100000) >> 16;
	mark = &work->marks[1];
	other = (Map_GetTerrainHeightFar(layer, x, z + 0x200000) >> 16) - 16;
	if (other > height)
		height = other;
	if (height > 0 && height > floor)
	{
		((u32 *)mark)[1] = 0x40000800;
		((u32 *)mark)[2] = 0;
		mark->priority = 0;
		mark->tile = tile;
		mark->mode = 1;
		mark->x = ((u32)(x >> 16) & 0xfff0) - camera_x;
		row = (z >> 16) & 0xf0;
		mark->y = row - camera_z - height + 16;
		Runtime_PushSlotEntry(mark, 0);
	}
}

void BattleFx_StartFadeOverlay(void *object)
{
	u32			 *work;
	u8			 *tiles;
	volatile u32  fill;
	volatile u16 *blend;
	s32			  value;

	work = (u32 *)Runtime_AllocateBlock(36, 28);
	tiles = Runtime_AllocateHeapBlock(14, 0x400);
	fill = 0x11111111;
	Dma_Set((const void *)&fill, tiles, 0x85000080, REG_DMA3);
	VramBlock_LoadCached(94, 0x200, tiles);
	Runtime_ReleaseHeapBlock(14);
	Scheduler_AddOrUpdateCallback(Object_EffectSpawnCallback, 0xc80);
	/* FAKEMATCH: needed to match: the blend values pass through an int and one register pointer so GCC builds them with mov and steps the address */
	value = 0x3f9e;
	blend = &REG_BLDCNT;
	*blend = value;
	value = 16;
	*++blend = value;
	value = 31;
	*++blend = value;
	fill = 0;
	Dma_Set((const void *)&fill, work, 0x85000007, REG_DMA3);
	if (object == NULL)
		object = ObjectTable_Get(gGameState.selected_actor);
	work[6] = (u32)object;
}


void Object_EnableEffectSpawnCallback(void)
{
	Scheduler_EnableCallbacks(Object_EffectSpawnCallback);
}

void Object_DisableEffectSpawnCallback(void)
{
	Scheduler_DisableCallbacks(Object_EffectSpawnCallback);
}

struct BattleEffectEntry *BattleFx_FindDefinition(u32 id)
{
	struct BattleEffectEntry *entry = BattleFx_DefinitionTable;
	u32						  index = 0;

	if (entry->id != id)
	{
		do
		{
			index += 1;
			entry++;
			if (index > 0x81)
				break;
		} while (entry->id != id);
	}
	return entry;
}

s32 BattleFx_GetAnimationValue(void)
{
	struct BattleRenderObject *object = ObjectTable_Get();

	if (object->kind != 1 || object->animation == NULL || object->animation->value_28 == NULL)
	{
		return 0;
	}
	return *object->animation->value_28;
}

s32 BattleFx_GetResourceId(u32 id)
{
	u8 value;

	if (gGameState.speech == 0 || (value = BattleFx_FindDefinition(id)->value) == 0xFF)
	{
		return 0;
	}
	return value + 0x100;
}

u8 BattleFx_GetFlags(void)
{
	return BattleFx_FindDefinition(BattleFx_GetAnimationValue())->flags;
}

void Battle_UpdateModeFromShoulderButtons(void)
{
	struct BattleRuntime *runtime = (struct BattleRuntime *)gEventWork;

	if (*(volatile u8 *)&gDebugMode != 0)
	{
		if (gKeyState & KEY_L)
		{
			runtime->mode_1cc = 0;
		}
		if (gKeyState & KEY_R)
		{
			runtime->mode_1cc = -1;
		}
	}
}

void Battle_WaitMode0(s32 frames)
{
	if (((struct BattleRuntime *)gEventWork)->mode_1cc == 0 && frames != 0)
	{
		WaitFrames(frames);
	}
}

void Battle_InitializeRenderObject(void)
{
	struct BattleRenderObject *object;

	object = ObjectTable_Get(gGameState.selected_actor);
	object->unknown_30 = 0x10000;
	object->unknown_34 = 0x8000;
	object->unknown_38 = 0x80000000;
	object->unknown_40 = 0x80000000;
	object->unknown_24 = 0;
	object->unknown_2c = 0;
	if (gGameState.movement_mode == 1)
	{
		ObjectDispatch_ApplyArgumentToChildrenFar((struct ObjectRuntime *)object, 0xC);
		return;
	}
	ObjectDispatch_ApplyArgumentToChildrenFar((struct ObjectRuntime *)object, 1);
}

void Battle_ResetEffectCounter(void);

void Battle_Reset(void)
{
	struct BattleRuntime *runtime = (struct BattleRuntime *)gEventWork;

	UiTimedNotice_CloseIfActiveFar();
	Battle_InitializeRenderObject();
	if (runtime->unknown_cb6 != 0)
	{
		Battle_ResetEffectCounter();
	}
	{
		s32 zero = 0;
		runtime->unknown_cc2 = zero;
		runtime->unknown_cc4 = zero;
		runtime->unknown_1c8 = 0x10;
		runtime->mode_1cc = zero;
		runtime->unknown_1da = 0xFFFF;
		runtime->unknown_1dc = -1;
		runtime->unknown_1de = -1;
		Scheduler_AddOrUpdateCallback((const void *)Battle_UpdateModeFromShoulderButtons, 0xC80);
		GameFlag_ClearBitFar(0x132);
		runtime->object_id = gGameState.selected_actor;
		runtime->unknown_1f8 = zero;
	}
}

void BattleFx_FinishAction(void)
{
	Scheduler_RemoveCallback(Battle_UpdateModeFromShoulderButtons);
	Object_AttachWorkTargetToObject(gGameState.selected_actor, 1);
	GameFlag_RefreshLureCapFar();
}

s32 Party_AddActiveOwnerFar(s32 event_id);

void Event_NoOpHookA(void)
{
}

void Event_NoOpHook(void)
{
}

void Event_RunObjectHookAndWait(struct ScenePlacement *table)
{
    Event_NoOpHook();
    ObjectTable_ResetForObject(table);
    WaitFrames(1);
    ObjectTable_Get(gGameState.selected_actor);
}

void Event_CallWithLastActiveObjectId(struct ScenePlacement *table)
{
    Event_SpawnObjectTable(table, ObjectTable_FindLastActiveId());
}

void Event_SetSceneEvents(s32 value)
{
    struct FieldStepWork *work = (struct FieldStepWork *)gEventWork;

    work->events = (const struct SceneEvent *)value;
}

void Event_PrepareObjectAndApplyValue(s32 event_id, s32 value)
{
    /* FAKEMATCH: UnitGetFar is called with no argument set up; r0 still holds event_id */
    ((s32 (*)(void))UnitGetFar)();
    Party_AddActiveOwnerFar(event_id);
    if (value != 0) {
        Party_ShowJoinedMessageFar(event_id, value);
    }
}

void Event_PrepareTwoObjectsAndApply(s32 first, s32 second)
{
    Party_AddActiveOwnerFar(first);
    Party_AddActiveOwnerFar(second);
    Party_ShowPairJoinedMessageFar(first, second);
}

s32 Event_ValidatePackedId(u32 packed_id)
{
    s32 group = (packed_id >> 10) & 0xf;
    s32 entry = packed_id & 0x3ff;

    if (group > 7)
        return -1;
    if (GameFlag_TestFar(group) == 0)
        return -2;
    if (Equipment_HasValueFar(group, entry) == 0)
        return -3;
    return 0;
}

void Event_ClearInvalidPackedValues(void)
{
    if (Event_ValidatePackedId(gGameState.first_shortcut) != 0)
        gGameState.first_shortcut = 0;
    if (Event_ValidatePackedId(gGameState.second_shortcut) != 0)
        gGameState.second_shortcut = 0;
}


static __inline__ s32 Vitals_Ratio(s32 value, s32 max)
{
    s32 ratio = (value << 14) / max;
    s32 clamped = 0x4000;

    if (ratio <= 0x4000) {
        clamped = 0;
        if (ratio >= 0)
            clamped = ratio;
    }
    return clamped;
}

void Party_RemoveOwnerRestored(s32 owner)
{
    struct BtlUnit *vitals;
    s32 i;
    s32 count;
    s32 standing;

    Party_RemoveActiveOwnerFar(owner);
    Event_ClearInvalidPackedValues();
    vitals = UnitGetFar(owner);
    Unit_Recover(vitals);
    vitals->hp_gauge = Vitals_Ratio(vitals->hp, vitals->max_hp);
    if (vitals->hp_gauge == 0 && vitals->hp != 0)
        vitals->hp_gauge = 1;
    vitals->ep_gauge = Vitals_Ratio(vitals->ep, vitals->max_ep);
    if (vitals->ep_gauge == 0 && vitals->ep != 0)
        vitals->ep_gauge = 1;
    vitals->doku = 0;
    standing = 0;
    count = Party_CountActiveOwnersFar();
    for (i = 0; i < count; i++) {
        vitals = UnitGetFar(gGameState.active_owners[i]);
        if (vitals->hp != 0)
            standing++;
    }
    if (standing == 0) {
        vitals = UnitGetFar(gGameState.selected_actor);
        vitals->hp = 1;
        vitals->hp_gauge = Vitals_Ratio(vitals->hp, vitals->max_hp);
        if (vitals->hp_gauge == 0 && vitals->hp != 0)
            vitals->hp_gauge = 1;
        vitals->ep_gauge = Vitals_Ratio(vitals->ep, vitals->max_ep);
        if (vitals->ep_gauge == 0 && vitals->ep != 0)
            vitals->ep_gauge = 1;
    }
}

struct ItemDefinition *Item_GetDirectFar(s32 item);

extern void UiWork_PushValueSlotFar(s32, s32);
extern void UiText_ShowPositionedMessageAndWaitFar(void *, s32);

s32 Party_CheckMemberValueTotal(s32 id)
{
    s32 count;
    s32 sum = 0;
    s32 i;

    count = Party_CountActiveOwnersFar(id);
    for (i = 0; i < count; i++)
        sum += Inventory_CountItemFar(gGameState.active_owners[i], id);

    if (sum >= count * 30) {
        UiWork_PushValueSlotFar(id, 2);
        UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryMore, 1);
        UiWork_PushValueSlotFar(id, 2);
        UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryMore + 1, 1);
        return -1;
    }
    return 0;
}

void UiWork_FinalizePendingCoreFar(void);
void AudioCommand_PlayFar(s32 cue);

s32 PartyInventory_GiveItem(s32 item, s32 unused)
{
    struct EventRuntime *work;
    s32 saved;
    s32 owner;
    s32 result;
    s32 count;
    s32 i;
    s32 member;
    s32 slot;

    work = (struct EventRuntime *)gEventWork;
    saved = work->message;
    owner = PartyInventory_AddFar(item);
    if (owner == -1) {
#if defined(TBS_EDITION_DE)
        if (item == ITEM_BLACK_ORB)
            UiText_ShowPositionedMessageAndWaitFar(MsgGotBlackOrb, 1);
        else {
            UiWork_PushValueSlotFar(item, 2);
            UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem, 1);
        }
#else
        UiWork_PushValueSlotFar(item, 2);
        UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem, 1);
#endif
        UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryAnyMore, 1);
    retry:
        UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop, 1);
        result = Shop_PickUnitItemFar(&member, &slot);
        if (result == -1) {
            if (Item_GetDirectFar(item)->flags & ITEM_INDISPENSABLE) {
                UiWork_PushValueSlotFar(item, 2);
                UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 4, 1);
                goto retry;
            }
            UiWork_PushValueSlotFar(item, 2);
            UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 1, 5);
            result = Object_CallSpawnRoutineAtOrigin(1);
            UiWork_FinalizePendingCoreFar();
            if (result != 0)
                goto retry;
            Item_AdjustCounterFar(item, 1);
            UiWork_PushValueSlotFar(item, 2);
            UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 2, 1);
            work->message = saved;
            return owner;
        } else {
            UnitGetFar(member);
            {
                /* FAKEMATCH: the ROM loads the slot argument before the member. Only the English build, without the thirty-item check below, needs the copy; the direct call matches the rest. */
                s32 s = slot;

                count = Shop_GetSelectionState(member, s);
            }
#if !defined(TBS_EDITION_EN)
            if (Inventory_CountItemFar(member, item) > 29) {
                UiWork_PushValueSlotFar(member, 1);
                UiWork_PushValueSlotFar(item, 2);
                UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 7, 1);
                goto retry;
            }
#endif
            for (i = 0; i < count; i++)
                Inventory_DiscardFar(member, slot);
            owner = PartyInventory_AddFar(item);
            AudioCommand_PlayFar(83);
            if (owner == gGameState.selected_actor) {
#if defined(TBS_EDITION_DE)
                if (item == ITEM_BLACK_ORB)
                    UiText_ShowPositionedMessageAndWaitFar(MsgGotBlackOrb, 3);
                else {
                    UiWork_PushValueSlotFar(item, 2);
                    UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem, 3);
                }
#else
                UiWork_PushValueSlotFar(item, 2);
                UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem, 3);
#endif
            } else {
                UiWork_PushValueSlotFar(item, 2);
                UiWork_PushValueSlotFar(owner, 1);
                UiText_ShowPositionedMessageAndWaitFar(MsgGaveItemToMember, 3);
            }
            work->message = saved;
            return owner;
        }
    } else {
        AudioCommand_PlayFar(83);
#if defined(TBS_EDITION_DE)
        if (item == ITEM_BLACK_ORB)
            UiText_ShowPositionedMessageAndWaitFar(MsgGotBlackOrb, 3);
        else {
            UiWork_PushValueSlotFar(item, 2);
            UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem, 3);
        }
#else
        UiWork_PushValueSlotFar(item, 2);
        UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem, 3);
#endif
        if (owner != gGameState.selected_actor) {
            UiWork_PushValueSlotFar(item, 2);
            UiWork_PushValueSlotFar(owner, 1);
#if defined(TBS_EDITION_DE)
            UiText_ShowPositionedMessageAndWaitFar(MsgGaveItemToMember, 1);
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || \
    defined(TBS_EDITION_IT)
            UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem + 1, 1);
#else
            UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem + 1, 3);
#endif
        }
        work->message = saved;
    }
    return owner;
}

//--------------------------------------------------------------------------

// try_add_and_return_owner.c

// 持ち物：アイテムを持たせて、持ち主を返す

//--------------------------------------------------------------------------

s32 Inventory_AddItemFar(s32, s32);

//--------------------------------------------------------------------------
// アイテムを持たせる
// 持てればキャラクター番号を、持ち物がいっぱいなら -1 を返す
// item   : アイテム番号
// unused : 使わない
// owner  : 持たせるキャラクター番号
//--------------------------------------------------------------------------
s32 Inventory_TryAddAndReturnOwner(s32 item, s32 unused, s32 owner)
{
	if (Inventory_AddItemFar(owner, item) >= 0)
		return owner;
	return -1;
}

//--------------------------------------------------------------------------
// 何もしない
//--------------------------------------------------------------------------
void Inventory_NoOpCallback(void)
{
}

s32 Menu_RunConfirmSelectionFar(s32, s32, s32, s32);

struct EventPromptWork {
    struct EventRuntime event;
    u8 unknown_200[0xac2];
    s16 resource_base;
    s16 width;
};

struct ActionDescriptor;

void Inventory_ReservedNoOp(void)
{
}

void Object_WaitUntilChildValueDiffers(s32 object_id, s32 value)
{
    struct ObjectRuntime *obj = ObjectTable_Get(object_id);

    if (obj != 0 && obj->animation_kind == 1) {
        s32 cnt = 0;
        u8 *p = &((struct AnimationObject *)obj->animation)->last_no;

        while (cnt <= 89) {
            WaitFrames(1);
            if (value != *p) {
                break;
            }
            cnt++;
        }
    }
}

s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 force_bottom)
{
    struct EventPromptWork *work = (struct EventPromptWork *)gEventWork;
    s32 sprite = ((struct ScenePlacement *)BattleAction_FindDescriptor(work->event.speaker))->sprite;
    struct UiWindow *window = (struct UiWindow *)work->event.message_window;
    struct UiWindow *side = (struct UiWindow *)work->event.side_window;
    s32 use_bottom = 1;
    s32 answer;

    while (gKeyState != 0)
        WaitFrames(1);

    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);

    WaitFrames(3);

    if (force_bottom == 0) {
        s32 bottom = window->y + window->height;

        if (side != 0) {
            s32 side_bottom = side->y + side->height;
            if (bottom < side_bottom)
                bottom = side_bottom;
        }

        if (bottom > 15)
            use_bottom = 0;
    }

    answer = Menu_RunConfirmSelectionFar(use_bottom, work->resource_base, work->width, 0);
    if (answer != 0) {
        Object_SetModeById(actor, 4);
        UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
        UiWork_FinalizePendingCoreFar();
        Object_WaitUntilChildValueDiffers(actor, 4);
    } else {
        Object_SetModeById(actor, 3);
        UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
        UiWork_FinalizePendingCoreFar();
        Object_WaitUntilChildValueDiffers(actor, 3);
    }

    return answer;
}

s32 Object_CallSpawnRoutineAtOrigin(s32 value)
{
    return Menu_RunConfirmSelectionFar(value, 0, 0, 0);
}

//--------------------------------------------------------------------------

// draw_draw_quantity_pair_with_cue.c

// メッセージ：わざを覚えさせて「覚えた」と出す

//--------------------------------------------------------------------------

void OwnerAction_AddFar(void);
#if GAME_TLA
void Audio_PlayCue(s32);
void UiText_DrawQuantity(s32, s32);
#else
#endif

//--------------------------------------------------------------------------
// わざを覚えさせ、効果音とメッセージを出して、送られるまで待つ
// unit : キャラクター番号（メッセージの名前 1）
// abil : わざの番号（メッセージのわざ 4）
//--------------------------------------------------------------------------
void UiText_DrawQuantityPairWithCue(s32 unit, s32 abil)
{
	// 引数なしの宣言だが、r0・r1 の unit・abil がそのまま渡る
	OwnerAction_AddFar();
#if GAME_TLA
	Audio_PlayCue(83);
	UiText_DrawQuantity(unit, 1);
	UiText_DrawQuantity(abil, 4);
#else
	AudioCommand_PlayFar(83);
	UiWork_PushValueSlotFar(unit, 1);
	UiWork_PushValueSlotFar(abil, 4);
#endif
	UiText_ShowPositionedMessageAndWaitFar(MsgAbilityLearned, 3);
}

void Event_StartScreenTransition(void)
{
    struct EventRuntime *runtime = (struct EventRuntime *)gEventWork;

    DisplayTransition_Start(runtime->value_1c0, runtime->value_1c8);
    runtime->status_1c6 = 1;
}

void Event_FinishScreenTransition(void)
{
    struct EventRuntime *runtime = (struct EventRuntime *)gEventWork;

    DisplayTransition_Finish(runtime->value_1c0, runtime->value_1c8);
    runtime->status_1c6 = 0;
}

void Event_WaitTransitionFrames(void)
{
    WaitFrames(((struct EventRuntime *)gEventWork)->value_1c8);
}

void Event_SetDestination(u16 first, u16 second)
{
    ((struct EventRuntime *)gEventWork)->value_170 = 999;
    gGameState.scene = first;
    gGameState.entrance = second;
}

void Event_SetSavedDestination(u16 first, u16 second)
{
    struct EventRuntime *state = (struct EventRuntime *)gEventWork;
    state->value_170 = 999;
    gGameState.saved_scene = first;
    gGameState.saved_entrance = second;
}

void Event_SetExitRequest(u16 value)
{
    struct EventRuntime *state = (struct EventRuntime *)gEventWork;
    state->value_170 = value;
}

//--------------------------------------------------------------------------
// result_request.c
// バトル効果：イベントから決まった敵との戦闘を用意する
//--------------------------------------------------------------------------


#define CHOJO 98	  // 頂上の嵐の背景で戦うエンカウント表
#define TOBANAI 0x800 // ジンを跳ねさせない
#define JIN_0 300	  // ジン 0 番の pending_djinn_event

//--------------------------------------------------------------------------
// エンカウント表 zone の slot 番目の敵グループとの戦闘を用意する
// ワールドマップ（mode 3）では、選択中の者の足もとの地形も引く
//--------------------------------------------------------------------------
void BattleFx_SetWeightedResult(s32 zone_no, s32 slot_no)
{
	register s32				  zone = zone_no;
	register s32				  slot = slot_no;
	register struct EventRuntime *ev = (struct EventRuntime *)gEventWork;

	ev->value_17c = BattleFx_GetWeightedResult(zone, slot); // 敵グループ
	if (zone == CHOJO && slot == 0)
		gGameState.special = (s32)&ResourceId_VinasuChojoStormBackdrop;
	if (ev->mode_19e == 3)
		BattleFx_LookupResult((u8 *)ObjectTable_Get(gGameState.selected_actor) + 8); // 位置
	BattleFx_SelectBattleCue(zone, slot);
}

//--------------------------------------------------------------------------
// ジン no 番との戦闘を用意する
// flag の下 8 ビットはジンのオブジェクトで、TOBANAI がなければ跳ねさせる
// no は属性ごとに 20 ずつ（Djinn_ResolvePendingEvent）
//--------------------------------------------------------------------------
void BattleFx_SetPhaseRequest(s32 flag, s32 no)
{
	struct EventRuntime *ev;
	struct GameState	*gs;
	s32					 skip;

	ev = (struct EventRuntime *)gEventWork;
	skip = flag & TOBANAI;
	flag &= 0xff;

	if (skip == 0)
		EventActor_RunHopAndLeapSequence(flag);

	gs = &gGameState;
	gs->pending_djinn_event = (no + JIN_0) | skip;
	/* FAKEMATCH: the result is read as s16 here; the shared prototype returns unsigned char */
	ev->value_17c = ((s16 (*)(s32))BattleFx_GetPhaseResult)(no); // ジンの敵グループ
	if (ev->mode_19e == 3)
	{
		void *obj = ObjectTable_Get(gs->selected_actor);

		BattleFx_LookupResult((u8 *)obj + 8); // 位置
	}
	BattleFx_SelectBattleCue(0, 0);
}

void Party_SetFields1ceAnd1d0(u16 first, u16 second)
{
    gGameState.next_scene = first;
    gGameState.next_entrance = second;
}

void Event_SetDefeatDestination(u16 first, u16 second)
{
    gGameState.defeat_scene = first;
    gGameState.defeat_entrance = second;
}

void Event_RequestDestination(u16 first, u16 second)
{
    struct EventRuntime *work = (struct EventRuntime *)gEventWork;
    gGameState.scene = first;
    gGameState.entrance = second;
    work->value_170 = 999;
}


struct FieldCueWork {
    u8 padding[0xcc8];
    s16 queued_sound;
};
void ObjectMotion_SetHorizontalPositionWithTerrain(u32, s32, s32);
void Battle_WaitMode0(s32);
void ObjectDispatch_InitFromTable6Far(struct ObjectRuntime *);
void Script_WaitForEventTimeoutFar(struct ObjectRuntime *);

struct BurstParticle {
    u8 pad_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad_14[28];
    s32 velocity_x;
    s32 velocity_z;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unknown_44[12];
    struct FieldSprite *child;
    u8 pad_54;
    u8 flags;
    u8 pad_56[14];
    u16 unknown_64;
    u8 pad_66[6];
    void (*callback_6c)(struct BurstParticle *);
};
void ObjectMotion_ArmCallback(s32 arg0, s32 arg1, s32 arg2);
void BattleFx_PlayQueuedSound(void);

void BattleFx_SetQueuedSoundAndPlay(s32 sound_id)
{
    ((struct FieldCueWork *)gEventWork)->queued_sound = sound_id;
    if ((s16)sound_id == -1) {
        sound_id = 0x121;
    }
    AudioCommand_PlayFar(0x12a);
    AudioCommand_PlayFar(sound_id);
}

void BattleFx_PlayQueuedSound(void)
{
    s16 sound_id = ((struct FieldCueWork *)gEventWork)->queued_sound;

    if (sound_id != -1)
        AudioCommand_PlayFar(sound_id);
}

struct FieldActor *Object_GetById(s32 object_id)
{
    struct FieldActor *object = (struct FieldActor *)ObjectTable_Get(object_id);

    if (object == NULL)
        return NULL;
    return object;
}

void ObjectMotion_SetSpeedParameters(u32 object_id, s32 speed_limit, s32 acceleration)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->acceleration = acceleration;
        object->speed_limit = speed_limit;
    }
}

void ObjectMotion_EnableActionAndSetCallback(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        s32 value = 1;
        value |= object->action_flags;
        object->action_flags = value;
        ObjectMotion_SetActionCallback(object, action);
    }
}

void ObjectMotion_EnableActionAndResetMotion(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);
    u8 enabled;

    if (object != NULL) {
        enabled = 1;
        object->action_flags = enabled | object->action_flags;
        ObjectDispatch_InitFromTable6Far(object);
    }
}

void Object_LinkObjectAndSetCallback(u32 object_id, u32 linked_object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->linked_object = (struct ObjectRuntime *)Object_GetById(linked_object_id);
        ObjectMotion_SetActionCallback(object, (s32)ObjectMotion_LinkedActionScript);
    }
}

void Object_RefreshSelectorById(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL)
        ObjectDispatch_WaitForValue16Far(object);
}

void Object_SetActionCallbackAndRefreshById(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        s32 value = 1;
        value |= object->action_flags;
        object->action_flags = value;
        ObjectMotion_SetActionCallback(object, action);
        ObjectDispatch_WaitForValue16Far(object);
    }
}

void ObjectMotion_ResetAndSetPosition(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        ObjectDispatch_InitFromTable6Far(object);
        Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
    }
}

void ObjectMotion_SetPositionAndCommit(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        ObjectDispatch_InitFromTable6Far(object);
        Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
        Script_WaitForEventTimeoutFar(object);
    }
}

void ObjectMotion_ResetAndSetPositionInMode2(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        ObjectDispatch_InitFromTable6Far(object);
        ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
        Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
    }
}

void ObjectMotion_SetPositionAndReset(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        ObjectDispatch_InitFromTable6Far(object);
        ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
        Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
        Script_WaitForEventTimeoutFar(object);
        ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
    }
}

void ObjectMotion_CenterOnColumn(u32 object_id, s32 action, s32 z_offset)
{
    struct ObjectRuntime *object;
    s16 current_angle;
    s32 snapped_angle;
    s16 angle_remainder;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        current_angle = ((s16 *)&object->x)[1];
        snapped_angle = current_angle;
        if (current_angle < 0) {
            snapped_angle += 15;
        }
        snapped_angle >>= 4;
        snapped_angle *= 16;
        angle_remainder = current_angle - snapped_angle;
        object->movement_state = 0;
        ObjectDispatch_InitFromTable6Far(object);
        ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
        Object_SetMoveTargetFar(object,
            object->x + ((8 - angle_remainder) << 16),
            object->y, object->z);
        Script_WaitForEventTimeoutFar(object);
        ObjectMotion_SetActionVariant(object_id, action);
        Object_SetMoveTargetFar(object, object->x, object->y,
            object->z + (z_offset << 16));
    }
}

void ObjectMotion_OffsetPositionAndResetMotion(u32 object_id, s32 x_offset, s32 z_offset)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        ObjectDispatch_InitFromTable6Far(object);
        Object_SetMoveTargetFar(object, object->x + (x_offset << 16),
            object->y, object->z + (z_offset << 16));
    }
}

void ObjectMotion_OffsetPositionAndReset(u32 object_id, s32 x_offset, s32 z_offset)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->movement_state = 0;
        ObjectDispatch_InitFromTable6Far(object);
        ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
        Object_SetMoveTargetFar(object, object->x + (x_offset << 16),
            object->y, object->z + (z_offset << 16));
    }
}

void ObjectMotion_CommitPositionAndActivate(u32 object_id, s32 x_offset, s32 z_offset)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    ObjectMotion_OffsetPositionAndReset(object_id, x_offset, z_offset);
    if (object != NULL) {
        Script_WaitForEventTimeoutFar(object);
        ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
    }
}

void Motion_LaunchFromFocusedObject(u32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct ObjectRuntime *object = ObjectTable_Get(arg0);

    if (object != NULL) {
        struct ObjectRuntime *other;

        ObjectMotion_SetSpeedParameters(arg0, 0x9999, 0x4CCC);
        other = (struct ObjectRuntime *)Object_GetById(gGameState.selected_actor);
        if (other != NULL)
            ObjectMotion_SetHorizontalPositionWithTerrain(arg0, other->x, other->z);
        object->movement_state = 0;
        ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
        ObjectMotion_OffsetPositionAndResetMotion(arg0, arg1, arg2);
        ObjectDispatch_InitializeFar((struct DispatchObject *)object, (const s32 *)&ObjectMotion_LaunchScript);
        object->action = arg3;
    }
}

void ObjectMotion_CommitCurrentPositionAndActivate(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        Script_WaitForEventTimeoutFar(object);
        ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
    }
}

void ObjectMotion_SetHorizontalPositionWithTerrain(u32 object_id, s32 x, s32 z)
{
    struct ObjectRuntime *object;
    s32 terrain_height;
    s32 tile_x;
    s32 tile_z;
    s32 terrain_id;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        ObjectDispatch_InitFromTable6Far(object);
        object->velocity_x = 0;
        object->velocity_y = 0;
        object->velocity_z = 0;
        object->target_y = 0x80000000;
        object->target_x = 0x80000000;
        object->x = x;
        object->z = z;
        if (1 & object->flags) {
            terrain_id = object->terrain_id;
            tile_x = x;
            if (tile_x < 0) {
                tile_x += 0xFFFF;
            }
            tile_x = tile_x >> 0x10;
            tile_z = z;
            if (tile_z < 0) {
                tile_z += 0xFFFF;
            }
            tile_z = tile_z >> 0x10;
            terrain_height = Map_GetTerrainHeightFar(terrain_id, tile_x, tile_z) << 0x10;
            object->y = (object->y - object->terrain_height) + terrain_height;
            object->terrain_height = terrain_height;
        }
    }
}

void ObjectMotion_SetPositionWithTerrain(u32 object_id, s32 x, s32 y, s32 z)
{
    struct ObjectRuntime *object;
    s32 terrain_height;
    s32 tile_x;
    s32 tile_z;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        ObjectDispatch_InitFromTable6Far(object);
        object->velocity_x = 0;
        object->velocity_y = 0;
        object->velocity_z = 0;
        object->target_y = 0x80000000;
        object->target_x = 0x80000000;
        object->x = x;
        object->y = y;
        object->z = z;
        if (1 & object->flags) {
            s32 terrain_id = object->terrain_id;
            tile_x = x;
            if (tile_x < 0) {
                tile_x += 0xFFFF;
            }
            tile_x >>= 0x10;
            tile_z = z;
            if (tile_z < 0) {
                tile_z += 0xFFFF;
            }
            terrain_height =
                Map_GetTerrainHeightFar(terrain_id, tile_x, tile_z >> 0x10) << 0x10;
            object->y = (object->y - object->terrain_height) + terrain_height;
            object->terrain_height = terrain_height;
        }
    }
}

void Object_SetModeById(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL)
        ObjectDispatch_ApplyArgumentToChildrenFar(object, action);
}

void Object_SetActionById(u32 object_id, s32 action)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL)
        ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)object, action);
}

void ObjectMotion_WaitForAnimationChange(u32 object_id)
{
    struct ObjectRuntime *object;
    struct AnimationObject *sprite;
    volatile s32 saved;
    s32 i;

    object = ObjectTable_Get(object_id);
    if (object != NULL && object->animation_kind == 1) {
        sprite = object->animation;
        saved = sprite->last_no;
        for (i = 0; i <= 89; i++) {
            WaitFrames(1);
            if (saved != sprite->last_no) {
                break;
            }
        }
    }
}

void Motion_SetModeAndWaitAnimation(u32 object_id, s32 action)
{
    Object_SetModeById(object_id, action);
    ObjectMotion_WaitForAnimationChange(object_id);
}

void ObjectMotion_NoOp(void)
{
}

void ObjectMotion_Launch(u32 object_id, s32 speed, s32 event_id)
{
    struct ObjectRuntime *object;
    u8 *object_flags;
    s32 updated_flags;
    s32 vertical_velocity;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        object_flags = &object->flags;
        updated_flags = *object_flags | 2;
        vertical_velocity = speed << 16;
        *object_flags = updated_flags;
        object->velocity_y = vertical_velocity;
        if (speed > 5) {
            AudioCommand_PlayFar(0x99);
        } else {
            AudioCommand_PlayFar(0x98);
        }
        Battle_WaitMode0(event_id);
    }
}

void ObjectMotion_SetVariantCallback(u32 object_id, s32 variant)
{
    struct ObjectRuntime *object;

    object = ObjectTable_Get(object_id);
    if (object != NULL && variant > 0) {
        if (variant > 3) {
            variant = 3;
        }
        ObjectDispatch_InitializeFar((struct DispatchObject *)object,
            (const s32 *)(ObjectMotion_VariantScripts + ((3 - variant) << 7)));
    }
}

void Motion_SetVarCbAndRefresh(u32 object_id, s32 variant)
{
    ObjectMotion_SetVariantCallback(object_id, variant);
    Object_RefreshSelectorById(object_id);
}

void BattleFx_UpdateParticleLinearMotion(struct BurstParticle *particle)
{
  s32 velocity_x;
  s32 x;
  s32 velocity_z;
  s32 z;
  s32 velocity_y;
  s32 vz;
  velocity_x = particle->velocity_x;
  x = (particle->x) + velocity_x;
  particle->x = x;
  particle->target_x = x;
  velocity_z = particle->velocity_z;
  z = (particle->z) + velocity_z;
  particle->z = z;
  particle->target_z = z;
  velocity_y = (particle->y) + 0x400;
  particle->y = velocity_y;
  particle->target_y = velocity_y;
  particle->velocity_x =
      (s32)(velocity_x - velocity_x / 0x12);

  vz = velocity_z;
  if (velocity_z < 0) {
    vz += 0xF;
  }
  particle->velocity_z =
      (s32)(velocity_z - (vz >> 4));

}

void BattleFx_SpawnBurstParticle(struct BurstParticle *source, s32 optional)
{
    struct BurstParticle *object;
    struct FieldSprite *child;
    s32 value;

    object = (struct BurstParticle *)Object_CreateFar(222, source->x, source->y, source->z);
    if (object != 0) {
        child = object->child;
        switch (Random16() & 1) {
        case 1:
            ObjectDispatch_ApplyArgumentToChildrenFar((struct ObjectRuntime *)object, 2);
            ObjectDispatch_InitializeFar((struct DispatchObject *)object, (const s32 *)BattleFx_BurstParticleScriptA);
            break;
        default:
            ObjectDispatch_ApplyArgumentToChildrenFar((struct ObjectRuntime *)object, 1);
            ObjectDispatch_InitializeFar((struct DispatchObject *)object, (const s32 *)BattleFx_BurstParticleScriptB);
            break;
        }

        if (optional != 0)
            ObjectGroup_SetChildValue((struct FieldActor *)object, optional);

        object->flags = 0;
        value = Random16() % 10 + 5;
        object->velocity_z = -0x1999 * value;
        value = Random16() % 15 - 7;
        value <<= 1;
        object->velocity_x = 0x1999 * value;
        object->unknown_64 = 0;
        object->callback_6c = BattleFx_UpdateParticleLinearMotion;
        child->flags = 0;
        child->priority = source->child->priority;
    }
}

void BattleFx_RunRisingObjectSequence(s32 sequence_arg, s32 mode_or_frame, s32 optional_action)
{
    s32 next_y;
    s32 base_z;
    struct ObjectRuntime *object;
    u8 *object_flags;

    object = (struct ObjectRuntime *)Object_GetById(sequence_arg);
    base_z = object->z;
    if (object != 0) {
        AudioCommand_PlayFar(0x121);
        ObjectDispatch_ApplyArgumentToChildrenFar(object, mode_or_frame);
        WaitFrames(10);
        object_flags = &object->flags;
        ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
        {
            u8 flags = 2;
            flags |= *object_flags;
            *object_flags = flags;
        }
        object->velocity_y = 0x40000;
        Object_SetMoveTargetFar(object, object->x, object->y,
            base_z + 0xC0000);
        WaitFrames(6);
        AudioCommand_PlayFar(0xD9);
        mode_or_frame = 0;
        ObjectMotion_ArmCallback(sequence_arg, 0x5000, 0);
        *object_flags = 0;
        do {
            next_y = object->y + 0xFFFE0000;
            object->y = next_y;
            object->target_y = next_y;
            WaitFrames(1);
            if ((optional_action != -1) && (mode_or_frame & 1)) {
                BattleFx_SpawnBurstParticle((struct BurstParticle *)object, optional_action);
            }
            mode_or_frame++;
        } while ((u32)mode_or_frame <= 0xD);
        *object_flags = 3;
        object->velocity_y = 0x30000;
        Object_SetMoveTargetFar(object, object->x, object->y,
            base_z + 0x100000);
        Script_WaitForEventTimeoutFar(object);
        mode_or_frame = 0;
        if (object->y > object->terrain_height) {
wait_for_target_y:
            WaitFrames(1);
            mode_or_frame++;
            if ((u32)mode_or_frame <= 0xB3) {
                if (object->y > object->terrain_height) {
                    goto wait_for_target_y;
                }
            }
        }
        WaitFrames(2);
        BattleFx_PlayQueuedSound();
    }
}


void ObjectMotion_SetAngleToward(s32 first_id, s32 second_id, s32 wait)
{
    struct ObjectRuntime *first;
    struct ObjectRuntime *second;

    first = ObjectTable_Get(first_id);
    second = ObjectTable_Get(second_id);
    if (first != 0 && second != 0) {
        first->angle = ArcTan2(second->z - first->z,
            second->x - first->x);
        Battle_WaitMode0(wait);
    }
}

void Object_LinkPair(s32 first_id, s32 second_id, s32 wait)
{
    void *first = ObjectTable_Get(first_id);
    void *second = ObjectTable_Get(second_id);

    if (first != NULL && second != NULL) {
        FacingObject_TurnPairToFaceEachOther(first, second);
        Battle_WaitMode0(wait);
    }
}

void FacingObject_TurnPairToFaceEachOther(struct FacingObject *raw_first, struct FacingObject *raw_second)
{
    struct ObjectRuntime *first = (struct ObjectRuntime *)raw_first;
    struct ObjectRuntime *second = (struct ObjectRuntime *)raw_second;
    s32 toward;
    s32 away;
    s32 frame;
    s32 turning;
    s32 delta;

    if (first == NULL || second == NULL)
        return;
    toward = (u16)ArcTan2(second->z - first->z,
        second->x - first->x);
    away = toward + 0x8000;
    for (frame = 0; frame < 60; frame++) {
        turning = 2;
        delta = (s16)(toward - first->angle);
        if (delta != 0) {
            if (delta > 0x1000)
                delta = 0x1000;
            if (delta < -0x1000)
                delta = -0x1000;
            first->angle += delta;
        } else {
            turning = 1;
        }
        delta = (s16)(away - second->angle);
        if (delta != 0) {
            if (delta > 0x1000)
                delta = 0x1000;
            if (delta < -0x1000)
                delta = -0x1000;
            second->angle += delta;
        } else {
            turning--;
        }
        if (turning == 0)
            break;
        WaitFrames(1);
    }
}

void ObjectTable_DestroyById(s32 index)
{
    void *object = ObjectTable_Get(index);
    struct ObjectSlotTable *table = (struct ObjectSlotTable *)gEventWork;

    if (object != 0) {
        ObjectDispatch_ReleaseFar(object);
        table->slots[index] = NULL;
    }
}

void ObjectTable_DestroyNoOp(void)
{
}

void ObjectGroup_ConfigureChildValue(s32 object_id, s32 value)
{
    s32 flags;
    struct DispatchObject *object;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        flags = 0x100 & value;
        if (flags != 0) {
            ((struct ScriptMotionObject *)object)->hook =
                (void (*)(struct ScriptMotionObject *))ObjectGroup_ApplyIndexedChildValue;
            return;
        }
        ((struct ScriptMotionObject *)object)->hook = NULL;
        ObjectGroup_SetChildValue((struct FieldActor *)object, value);
    }
}

void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *object)
{
    if ((object->kind & 0xf) == 1) {
        u8 child_value;
        struct AnimationObject *container;
        u8 child_count;

        child_value = ObjectGroup_BlinkChildValues[(gFrameCount >> 1) & 3];
        container = object->target.child;
        child_count = container->count;
        if (child_count != 0) {
            struct AnimationEntry **entries = container->entries;
            s32 remaining = child_count;
            do {
                struct AnimationEntry *entry = *entries++;
                if (entry != 0 && entry->script != 0) {
                    entry->param = child_value;
                }
                remaining--;
            } while (remaining != 0);
        }
        container->dirty = 1;
    }
}

void ObjectGroup_SetChildValue(struct FieldActor *actor, s32 value)
{
    struct DispatchObject *object = (struct DispatchObject *)actor;

    if ((object->kind & 0xf) == 1) {
        struct AnimationObject *container = object->target.child;
        u8 raw_count = container->count;

        if (raw_count != 0) {
            struct AnimationEntry **entry = container->entries;
            u32 count = raw_count;
            do {
                struct AnimationEntry *item = *entry++;
                if (item != NULL && item->script != 0) {
                    item->param = value;
                }
                count--;
            } while (count != 0);
        }
        container->dirty = 1;
    }
}


struct UiWindow *UiText_OpenMessageAtObject(s32 arg);
extern const u8 ObjectMotion_StepAngleScript[];

void Object_SetTargetAndCallback(u32 object_id, s32 target_id, const void *callback)
{
    struct ObjectRuntime *first = ObjectTable_Get(object_id);
    struct ObjectRuntime *second = ObjectTable_Get(target_id & 0xff);

    if (first != NULL && second != NULL) {
        first->linked_object = second;
        if (!(target_id & 0x10000)) {
            first->action = 40;
            first->acceleration = second->acceleration * 2;
            first->speed_limit = second->speed_limit;
            first->unknown_59 = 0;
        }
        ObjectDispatch_InitializeFar((struct DispatchObject *)first, (const s32 *)callback);
    }
}

s32 ObjectMotion_StepAngle(struct ObjectRuntime *object)
{
    s32 delta = 0;

    if (object != NULL) {
        s32 target_angle = (u16)object->action;
        s32 current_angle = object->angle;
        delta = (s16)(target_angle - current_angle);
        if (delta != 0) {
            if (delta > 4096)
                delta = 2048;
            if (delta < -4096)
                delta = -2048;
            object->angle = current_angle + delta;
        }
    }
    return delta;
}

void Object_ResetTargetAndSetMode1(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->target_x = 0x80000000;
        object->target_y = 0x80000000;
        object->target_z = 0x80000000;
        ObjectDispatch_InitFromTable6Far(object);
        ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
    }
}

void ObjectMotion_ArmCallback(s32 object_id, s32 angle, s32 wait)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->action = angle;
        ObjectDispatch_InitializeFar((struct DispatchObject *)object, (const s32 *)ObjectMotion_StepAngleScript);
        Battle_WaitMode0(wait);
    }
}

void ObjectMotion_SetActionVariant(s32 object_id, s32 priority)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL && (object->animation_kind & 0xF) == 1) {
        struct FieldSprite *sprite = object->animation;

        sprite->priority = priority;
        sprite->second_priority = priority;
        object->unknown_23 &= ~ACTOR_PRIORITY_AUTOMATIC;
    }
}

void ObjectVisual_CopyAttributes(u32 target_id, u32 source_id)
{
    void *p;
    u8 flags;
    u32 shape;

    p = Object_GetById(source_id);
    p = ((struct ObjectRuntime *)p)->animation;
    flags = ((struct FieldSprite *)p)->vram_block;
    shape = ((struct FieldSprite *)p)->tile;

    p = Object_GetById(target_id);
    p = ((struct ObjectRuntime *)p)->animation;
    ((struct FieldSprite *)p)->vram_block = flags;
    ((struct FieldSprite *)p)->tile = shape;
}

void ObjectVisual_ReservedNoOp(void)
{
}


void Event_SetMessageId(s16 value)
{
    ((struct EventRuntime *)gEventWork)->message = value;
}

s32 ObjectTable_ReadActiveValue(s32 key)
{
    struct FieldStepWork *state = (struct FieldStepWork *)gEventWork;
    s32 result = -1;
    struct ObjectRuntime *entry = state->actors[key & 0x0fff];

    if (entry != 0 && entry->animation_kind == 1)
        result = ((struct AnimationObject *)entry->animation)->entries[0]->anim_id;
    return result;
}

s32 ObjectTable_FindActiveByValue(s32 value)
{
    struct FieldStepWork *state = (struct FieldStepWork *)gEventWork;
    s32 result = -1;
    s32 index = 8;
    struct ObjectRuntime *object = state->actors[index];

    if (object != 0 && object->animation_kind == 1 && ((struct AnimationObject *)object->animation)->entries[0]->anim_id == value) {
        result = index;
    } else {
    next:
        index++;
        if (index <= 65) {
            object = state->actors[index];
            if (object == 0 || object->animation_kind != 1 ||
                ((struct AnimationObject *)object->animation)->entries[0]->anim_id != value)
                goto next;
            result = index;
        }
    }
    return result;
}

struct UiWindow *UiText_OpenMessageAtObject(s32 arg)
{
    struct UiRenderWork *win;
    struct EventRuntime *work;
    s32 speaker;
    s32 extra;
    s32 tail;
    s32 margin;
    s32 height;
    s32 width;
    s32 top;
    s32 left;
    s32 pos[3];
    struct UiWindow *handle;
    s32 side;
    s32 flags;
    s32 message;
    struct ObjectRuntime *object;
    s32 x;
    s32 y;
    s32 ret;
    s32 column;
#if !EDITION_INTERNATIONAL
    s32 none;
#endif

    win = (struct UiRenderWork *)gWindowWork[0];
    work = ((struct WorkPointers *)gWindowWork)->event;
    handle = 0;
    side = 0;
    speaker = ObjectTable_ReadActiveValue(arg);
    extra = 0;
    tail = 0;
    margin = 4;
    flags = arg & 0xf000;
    message = work->message;
    arg &= 0xfff;
    x = 0;
    object = ObjectTable_Get(arg);
    work->speaker = arg;
    y = 0;
    ret = 0;
    if (work->message_busy == 0) {
        if (object != 0) {
            if (work->mode_19e == 3) {
                Render_ProjectPoint(&object->x, pos);
                x = pos[0] >> 3;
                y = pos[1] >> 3;
                ret = 1;
                y -= 2;
            } else {
                if (Object_GetScreenPosition(arg, pos) != -1)
                    ret = 1;
                else
                    ret = 0;
                x = pos[0] >> 3;
                y = pos[1] >> 3;
            }
        } else if (arg <= 7) {
            speaker = arg;
            object = ObjectTable_Get(gGameState.selected_actor);
            if (work->mode_19e == 3) {
                Render_ProjectPoint(&object->x, pos);
                x = pos[0] >> 3;
                y = pos[1] >> 3;
                ret = 1;
            } else {
                if (Object_GetScreenPosition(gGameState.selected_actor, pos) != -1)
                    ret = 1;
                else
                    ret = 0;
                x = pos[0] >> 3;
                y = pos[1] >> 3;
            }
        }
        if (!ret) {
            left = 15;
            top = 10;
        } else {
            left = 0;
            top = 0;
            UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
            left = x - width / 2;
            if (flags & 0x4000)
                top = y - height - 1;
            else if (!(flags & 0x8000) && y <= 8)
                top = y - height - 1;
            else
                top = y + 4;
        }
        if (win->mode != 0)
            margin = 5;
        column = x;
        if (flags & 0x1000) {
            column = x - margin - 2;
            if (column < 0)
                column = 0;
        } else if (flags & 0x2000) {
            column += 2;
            if (column + margin > 29)
                column = 29 - margin;
        } else if (column <= 15) {
            column = column - margin - 2;
            if (column < 0)
                column = x + 2;
        } else {
            column += 2;
            if (column + margin > 29)
                column = x - margin - 2;
        }
#if EDITION_INTERNATIONAL
        ret = Localization_LookupEntryIdFar(speaker);
        if (ret != -1) {
            UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
            message = -1;
            if (top > y)
                tail = top - 5;
            else
                tail = top + height;
            if (tail < 0)
                tail = top + height;
            else if (tail + 5 > 19)
                tail = top - 5;
            if (top < tail) {
                s32 lines = height;

                UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
                message = -1;
                extra = lines - height + 1;
            }
        } else if (top < y) {
            s32 lines = height;

            UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
            extra = lines - height + 1;
            message = ret;
        }
#else
        none = Localization_LookupEntryIdFar(speaker);
        ret = -1;
        if (none != ret) {
            UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
            message = ret;
            if (top > y)
                tail = top - 5;
            else
                tail = top + height;
            if (tail < 0)
                tail = top + height;
            else if (tail + 5 > 19)
                tail = top - 5;
        }
        if (top < y) {
            s32 lines = height;

            UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
            message = -1;
            extra = lines - height + 1;
        }
#endif
        if (column < 0)
            column = 0;
        else if (column + margin > 29)
            column = 29 - margin;
        if (win->mode != 0) {
            WaitFrames(8);
            if (extra != 0)
                handle = (struct UiWindow *)UiText_OpenMessageWindowFar(message, left, top + extra - 1, 18);
            else
                handle = (struct UiWindow *)UiText_OpenMessageWindowFar(message, left, top, 2);
        } else {
            s32 icon = BattleFx_GetResourceId(speaker);

            if (extra != 0)
                handle = (struct UiWindow *)UiText_OpenMessageWindowFar(message, left, top + extra - 1,
                                                     (icon << 16) | 17);
            else
                handle = (struct UiWindow *)UiText_OpenMessageWindowFar(message, left, top, (icon << 16) | 1);
        }
        if (win->mode != 0)
            side = UiWindow_CreateWithSideObjectFar(speaker, 0, column, tail);
        else
            side = UiWindow_CreateWithSideObjectFar(speaker, 0, column, tail);
        while (!UiWork_IsCompleteFar())
            WaitFrames(1);
    }
    work->message_window = (s32)handle;
    work->side_window = side;
    work->message++;
    return handle;
}


void						Battle_WaitMode0(s32 mode);
s32							Inventory_PromptAndSetObjectMode(s32 actor, s32 force);

void BattleEv_RunWait(s32 action, s32 flag)
{
	s32					 masked_action;
	struct EventRuntime *runtime = (struct EventRuntime *)gEventWork;
	/* FAKEMATCH: called with a second argument the function does not take */
	struct UiWindow		*window = ((struct UiWindow *(*)(s32, s32))UiText_OpenMessageAtObject)(action, flag);
	s32					 message_id;
	u32					 frames;

	frames = 0;
	WaitFrames(1);
	message_id = ObjectTable_ReadActiveValue(action);
	if (action <= 7)
	{
		masked_action = action & 0x0fff;

		if (BattleAction_FindDescriptor(masked_action) == 0)
		{
			message_id = masked_action;
		}
	}
	UiWork_FinalizeEntityMatchingLocalizedIdFar(message_id);

	if (runtime->message_busy == 0)
	{
		while (UiWork_IsIdleFar(window) == 0)
		{
			WaitFrames(1);
			frames++;
			if (frames > 600
				|| ((gKeysHeld & KEY_SELECT) && (gKeysHeld & KEY_R) && (gKeysHeld & KEY_L)
					&& (gKeysHeld & KEY_A)))
			{
				UiWork_FinalizePendingCoreFar();
			}
		}
	}

	WaitFrames(1);
}

void BattlePres_RunActionThenWaitIfModeZero(s32 first, s32 second, s32 value)
{
	BattleEv_RunWait(first, second);
	Battle_WaitMode0(value);
}

s32 BattleEventRuntime_ProcessAction(s32 object_id, s32 action_id)
{
	s32					 result;
	struct EventRuntime *runtime;
	struct GameState	*save;

	/* FAKEMATCH: called with a second argument the function does not take */
	((struct UiWindow *(*)(s32, s32))UiText_OpenMessageAtObject)(object_id, action_id);
	save = &gGameState;
	result = Inventory_PromptAndSetObjectMode(save->selected_actor, 0);
	if (result == 0)
	{
		BattleEv_RunWait(object_id, action_id);
		runtime = (struct EventRuntime *)gEventWork;
		runtime->message += 1;
	}
	else
	{
		runtime = (struct EventRuntime *)gEventWork;
		runtime->message += 1;
		BattleEv_RunWait(object_id, action_id);
	}
	return result;
}

void BattleEventRuntime_EmptyCallback(void)
{
}

void Event_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y);

void Event_ShowSpeakerMessage(s32 speaker)
{
    // 位置は未初期化のまま読まれる (original bug: the position is never set)
    struct { s32 x, y; } pos;
    struct EventRuntime *state = (struct EventRuntime *)gEventWork;

    speaker &= 0xfff;
    ObjectTable_Get(speaker);
    state->speaker = speaker;
    if (state->message_busy == 0) {
        s32 px = pos.x;
        s32 py = pos.y;
        s32 win;

        if (py > 119)
            py += 32;
        else
            py -= 32;
        px = px < 8 ? 8 : px;
        if (px > 312)
            px = 312;
        py = py < 20 ? 20 : py;
        if (py > 220)
            py = 220;
        win = UiText_OpenMessageWindowFar(state->message, px, py, 1);
        state->message_window = win;
        while (UiWork_IsIdleFar((struct UiWindow *)win) == 0)
            WaitFrames(1);
    }
    state->message++;
}

void ObjectTable_CallRefreshHook(void)
{
    /* FAKEMATCH: called with no arguments set up */
    ((void (*)(void))Event_ShowMessageAtPosition)();
}

void Event_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y)
{
    struct EventRuntime *state = (struct EventRuntime *)gEventWork;
    s32 py = y;
    s32 px = x;
    s32 min_x = 8;
    s32 min_y = 20;
    s32 ret;

    if (py > 119)
        py += 32;
    else
        py -= 32;

    if (x < min_x)
        px = min_x;
    if (px > 312)
        px = 312;
    if (py < min_y)
        py = min_y;
    if (py > 220)
        py = 220;

    ret = UiText_OpenMessageWindowFar(state->message, px, py, 1);
    while (UiWork_IsIdleFar((struct UiWindow *)ret) == 0)
        WaitFrames(1);
    state->message++;
}

//--------------------------------------------------------------------------

// table_run_if_active.c

// フィールド：話し手の表示の後しまつ

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// アクターに絵があれば、その絵番号を話し手としてウィンドウの表示を終える
// id : アクター番号
//--------------------------------------------------------------------------
void ObjectTable_RunIfActive(s32 id)
{
	s32 no;

	no = ObjectTable_ReadActiveValue(id);
	if (no != -1)
		UiWork_FinalizeEntityMatchingLocalizedIdFar(no);
}

void Battle_ShowPairedUnitWorkAndWait(s32 first, s32 first_x, s32 first_y, s32 first_arg,
	s32 first_extra, s32 second, s32 second_x, s32 second_y, s32 second_arg, s32 second_extra)
{
	struct EventRuntime *rt = (struct EventRuntime *)gEventWork;
	s32					 id0 = ObjectTable_ReadActiveValue(first);
	s32					 id1 = ObjectTable_ReadActiveValue(second);
	s32					 h0;
	s32					 h1;

	h0 = UiText_OpenMessageWindowFar(
		rt->message++, first_x, first_y, BattleFx_GetResourceId(id0) << 16);
	UiWindow_CreateWithSideObjectFar(id0, 0, first_arg, first_extra);

	h1 = UiText_OpenMessageWindowFar(
		rt->message++, second_x, second_y, BattleFx_GetResourceId(id1) << 16);
	UiWindow_CreateWithSideObjectFar(id1, 0, second_arg, second_extra);

	while (UiWork_IsCompleteFar() == 0)
		WaitFrames(1);

	WaitFrames(1);
	while ((gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0)
		WaitFrames(1);

	WaitFrames(1);
	UiWork_FinalizeEntityMatchingLocalizedIdFar(id0);
	UiWork_FinalizeEntityMatchingLocalizedIdFar(id1);
	UiWork_FinalizePendingCoreFar();
	WaitFrames(1);

	while (UiWork_IsIdleFar((struct UiWindow *)h0) == 0)
		WaitFrames(1);
	while (UiWork_IsIdleFar((struct UiWindow *)h1) == 0)
		WaitFrames(1);

	WaitFrames(1);
}

void Ui_SetRenderResultFromObject(s32 arg0)
{
	struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];
	u8					 value;
	s32					 ret;

	if (arg0 == 0x80000000)
	{
		*(s16 *)&state->result[0] = (value = 0);
		*(s16 *)&state->result[1] = value;
	}
	else
	{
		ret = BattleFx_GetResourceId(ObjectTable_ReadActiveValue(arg0));
		value = Ui_RenderResultValues[gGameState.unknown_207[5]];
		*(s16 *)&state->result[0] = ret;
		*(s16 *)&state->result[1] = value;
	}
}
