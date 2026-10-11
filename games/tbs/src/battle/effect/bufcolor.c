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

//--------------------------------------------------------------------------
// emitter.c
// フィールド効果：手に入れたアイテムの表示ときらめき、近くの印、ウィンドウの HBlank DMA
//--------------------------------------------------------------------------
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
#define gEventWork (*(u8 *(*))&gEventWork)
#define Object_CreateFar ((struct EfxObj *(*)(s32 kind, s32 x, s32 y, s32 z))Object_CreateFar)
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

#undef gEventWork
#undef Object_CreateFar

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
                Scheduler_RemoveCallback((u32)BattleFx_StartWindowHBlankDma);
                Scheduler_RemoveCallback((u32)DisplayTransition_UpdateScanlineTable);
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
    Dma_Set(&clear_value, state, 0x85000000 | (sizeof(*state) / 4), REG_DMA3);
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
    Dma_Set(&fill, BG_SCREEN_BLOCK(4), 0x85000140, REG_DMA3);
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
    Dma_Set(&zero, state, 0x85000000 | (sizeof(*state) / 4), REG_DMA3);
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
	Dma_Set(&zero, buf, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | (sizeof(*buf) >> 2), REG_DMA3);
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

#define ObjectTable_Get ((void *(*)(u32 object))ObjectTable_Get)

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

#undef ObjectTable_Get

#define ObjectDispatch_ApplyArgumentToChildrenFar ((void (*)(struct FieldActor *object, s32 animation))ObjectDispatch_ApplyArgumentToChildrenFar)

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
#define gDebugMode (*(volatile u8 (*))&gDebugMode)

void Battle_UpdateModeFromShoulderButtons(void)
{
	struct BattleRuntime *runtime = (struct BattleRuntime *)gEventWork;

	if (gDebugMode != 0)
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
		ObjectDispatch_ApplyArgumentToChildrenFar((s32)object, 0xC);
		return;
	}
	ObjectDispatch_ApplyArgumentToChildrenFar((s32)object, 1);
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

#undef ObjectDispatch_ApplyArgumentToChildrenFar
#undef gDebugMode
