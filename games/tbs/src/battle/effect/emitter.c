//--------------------------------------------------------------------------
// emitter.c
// フィールド効果：手に入れたアイテムの表示ときらめき、近くの印、ウィンドウの HBlank DMA
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
#include "system.h"
#include "object_lookup.h"
#include "scene.h"
#include "global_cells.h"
#include "effect_runtime.h"
#include "dma.h"
#include "io_reg.h"
#include "disptran.h"
#include "btlsys.h"
#include "layout_guard.h"
void Ui_PrepareTransferForItemFar(s32 item);

#define gOverlayArea (*(struct FieldMapState *)&gOverlayArea)

#define EffectRuntime_GetCurrentObject ((s32 (*)(s32))EffectRuntime_GetCurrentObject)
typedef struct MapEventEntry *(*EventListFn)(s32);
#define _call_via_r0 ((EventListFn)_call_via_r0)

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

extern u8	  *gEventWork;
struct EfxObj *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
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
	struct ParticleCell	 *cell = (struct ParticleCell *)(gEventWork + 0x11c);
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
	Vector_AddPolarOffset(16 << 16, ((struct EfxSrc *)src_z)->ang, &pos);
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
	obj = Object_CreateFar(22, x, 16 << 16, src_z);
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
		Object_CreateFar(22, actor->x, actor->y + (36 << 16), actor->z);

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
#define BATTLE_ACTIVE_OFS 0xcb8
#define MENU_OPEN_OFS 0xcca // gEventWork：メニューを開いていれば 0 以外

struct ParticleSlot
{
	s32 script;
	u8	pad[0x6c];
};

extern u8 *gEventWork;

void ObjectDispatch_ReleaseFar(void *object);

#define ParticlePool (*(struct ParticleSlot **)((u8 *)&gEventWork - 88))

//--------------------------------------------------------------------------
// 効果が動いていれば、きらめきのオブジェクトをすべて消す
//--------------------------------------------------------------------------
void BattleFx_ClearRandomParticles(void)
{
	if (*(s16 *)(gEventWork + BATTLE_ACTIVE_OFS) != 0)
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
	if (*(s8 *)(gEventWork + MENU_OPEN_OFS) != 0)
		return;
#endif
	if ((100 * Random16() >> 16) > 9)
		return;

	pos.first = source->values.first;
	pos.second = source->values.second;
	pos.third = source->values.third;
	rnd = Random16();
	Vector_AddPolarOffset(rnd << 4, Random16(), &pos);
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
	struct EffectRuntime *runtime = gEventWork;

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
			if (EffectRuntime_GetCurrentObject(id) != 0)
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
			entry = _call_via_r0(gOverlayArea.events);
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
