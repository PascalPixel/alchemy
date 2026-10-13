#include "unit/field/objdata.h"
#include "unit/field/field.h"
#include "unit/field/fldobj.h"
#include "unit/field/script.h"
#include "unit/field/operands.h"
#include "unit/field/player.h"
#include "unit/field/mapcell.h"
#include "unit/runtime/runtime.h"
#include "unit/runtime/objvram.h"
#include "unit/runtime/sched.h"
#include "unit/runtime/math.h"
#include "unit/runtime/heap.h"
#include "unit/runtime/matrix.h"
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
#include "types.h"
#include "animspr.h"
#include "global_cells.h"
#include "dma.h"
#include "ramcode.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "iowrite.h"
#include "heap_state.h"
#include "affine.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "object_dispatch.h"
#include "metadata_lookup.h"
#include "edition.h"
#include "objdisp.h"
#include "object_runtime.h"
#include "btlsys.h"
#include "transform.h"
#include "script.h"
#include "script_motion.h"
#include "script_object_entry.h"
#include "script_object_runtime.h"
#include "fieldobj.h"
#include "fixed_math.h"
#include "keystate.h"
#include "fieldrun.h"
#include "event_runtime.h"
#include "game_state.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "resource_ids.h"
#include "flag_ids.h"
#include "mapcopy.h"
#include "fixed_point_position.h"
#include "mapframe.h"
#include "btldisp.h"
#include "project.h"
#include "frame.h"
#include "palqueue.h"
#include "curve.h"
#include "btlpty.h"
#include "b5_context.h"
#include "vram_tab.h"
#include "vram_block.h"

//--------------------------------------------------------------------------
// オブジェクトの画面上の位置（ドット）を out[0], out[1] に返す
// 画面のまわりの範囲にいれば 0、外なら 0, 0 として -1
// obj : オブジェクト
// out : x と y
//--------------------------------------------------------------------------
s32 Map_GetScreenRelativePosition(struct ObjectRuntime *obj, s32 *out)
{
	struct MapScrollWork *map = gMapWork[0];
	s32					 *org = &map->view_x;
	s32					  ox;
	s32					  oz;
	s32					  x;
	s32					  y;

	ox = org[0] & 0xffff0000;
	oz = org[1] & 0xffff0000;
	x = obj->x - ox;
	y = obj->z - oz;
	if ((x >= -0x1fffff && x <= 0x10fffff) && y > 0 && y < 0xe00000)
	{
		*out++ = x >> 16;
		*out = y >> 16;
		return 0;
	}
	*out++ = 0;
	*out = 0;
	return -1;
}

//--------------------------------------------------------------------------
// オブジェクトのしくみを用意する
// 64 個のオブジェクトの領域をとって 0 でうめ、毎フレームの処理を登録する
// mode : 4 なら動きの処理を Object_UpdateAllMotion で、3 と 4 は固定カメラ
//--------------------------------------------------------------------------
void ObjectSystem_Initialize(s32 mode)
{
	struct ObjectSystemWork *sys;
	struct ObjectRuntime	*objs;
	volatile u32			 fill;

	sys = Runtime_AllocateBlock(6, sizeof(*sys));
	objs = Runtime_AllocateBlock(5, 64 * sizeof(*objs));
	ObjectSystem_Configure(mode);
	Dma_Fill32Cell(fill, 0, objs, 64 * sizeof(*objs));
	Dma_Fill32Cell(fill, 0, sys, sizeof(*sys));
	if (mode == 4)
		Scheduler_AddOrUpdateCallback(Object_UpdateAllMotion, 0xc8a);
	else
		Scheduler_AddOrUpdateCallback(Object_UpdateAllThumb, 0xc8a);
	if (mode >= 3 && mode <= 4)
	{
		Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCameraFixed, 0xc80);
	}
	else
	{
		Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCamera, 0xc80);
		Iwram_Data2 = 0;
		Iwram_Data1 = 0;
	}
	sys->edge = 15;
	sys->fill = 0;
}

//--------------------------------------------------------------------------
// 何もしない
//--------------------------------------------------------------------------
void ResourceTable_ReservedNoOpC0C4(void)
{
}

//--------------------------------------------------------------------------
// 何もしない
//--------------------------------------------------------------------------
void ResourceTable_ReservedNoOp2(void)
{
}

//--------------------------------------------------------------------------
// 64 個のうち、使われていない（スクリプトのない）オブジェクトを返す
// 空きがなければ 0
//--------------------------------------------------------------------------
void *ObjectDispatch_FindFreeObject(void)
{
	struct ObjectRuntime *obj = gObjectSlots;
	void				 *ret = 0;
	s32					  n = 0;

	while (n <= 63)
	{
		if (obj->script == 0)
		{
			ret = obj;
			break;
		}
		n++;
		obj++;
	}
	return ret;
}

//--------------------------------------------------------------------------
// オブジェクトの絵を解放し、オブジェクトを 0 でうめて空きにする
// obj : オブジェクト
//--------------------------------------------------------------------------
void ObjectDispatch_Release(struct DispatchObject *obj)
{
	volatile u32				zero;
	s32							n;
	struct ResourceObjectWork **child;

	if (obj)
	{
		switch (obj->kind & 15)
		{
		case 1: // 絵がひとつ
			ResourceObject_Release(obj->target.child);
			break;
		case 2: // 絵が 4 つまで
			child = (struct ResourceObjectWork **)obj->target.children;
			n = 3;
			do
			{
				struct ResourceObjectWork *res = *child++;
				if (res)
					ResourceObject_Release(res);
			} while (--n >= 0);
			break;
		}
		Dma_Fill32Cell(zero, 0, obj, sizeof(struct ObjectRuntime));
	}
}

//--------------------------------------------------------------------------
// フィールドのオブジェクトを作る。空きがなければ 0
// id      : 4096 で割った商が種類（0 絵ひとつ、2 絵ふたつ）、下の 12 ビットが絵
// x, y, z : 位置（16.16 固定小数）
//--------------------------------------------------------------------------
struct ObjectRuntime *FieldObject_Create(s32 id, s32 x, s32 y, s32 z)
{
	struct ObjectRuntime	   *obj;
	struct FieldActor		   *act;
	struct ScriptMotionObject  *mot;
	struct ScriptObjectRuntime *sc;
	void					   *spr;
	s32							kind;
	void					  **ent;

	ObjectDispatch_FindFreeObject();
	kind = id / 4096;
	id &= 0xfff;
	obj = ObjectDispatch_FindFreeObject();
	if (obj != NULL)
	{
		act = (struct FieldActor *)obj;
		act->radius = 16;
		switch (kind)
		{
		case 0:
			spr = ResourceObject_Create(id);
			if (spr != NULL)
			{
				obj->animation_kind = 1;
				obj->animation = spr;
				act->radius = Resource_GetMetadataRecordFar(id)->box_y >> 1;
			}
			else
			{
				obj->animation_kind = 0;
			}
			break;
		case 2:
			// 絵の並び（4 つ分）をとって 0 でうめ、id と id + 1 の絵を入れる
			ent = (void **)((u32 *)gMenuCtrlWork + gMenuCtrlWork->count++ + 2);
			obj->animation_kind = kind;
			obj->animation = ent;
			Dma_Fill32(0, ent, 0x10);
			spr = ResourceObject_Create(id);
			if (spr != NULL)
			{
				act->radius = Resource_GetMetadataRecordFar(id)->box_y >> 1;
				*ent++ = spr;
			}
			spr = ResourceObject_Create(id + 1);
			if (spr != NULL)
				*ent = spr;
			break;
		}
	}
	if (obj != NULL)
	{
		mot = (struct ScriptMotionObject *)obj;
		sc = (struct ScriptObjectRuntime *)obj;
		Object_SetPositionAndResetMotion(obj, x, y, z);
		obj->script = (s32 *)ObjectDispatch_DefaultScript;
		obj->speed_limit = 0x20000;
		obj->step = 0;
		act->scale_x = 0x10000;
		act->scale_y = 0x10000;
		obj->acceleration = 0x10000;
		obj->flags = 3;
		mot->gravity = 0x10000;
		mot->vertical.bounce = 0x4000;
		obj->unknown_59 = 0;
		obj->action_flags = 1;
		act->rise_base_y = 0;
		obj->angle = 0x4000;
		sc->home_x = x / 65536; // もとの位置（ドット）
		sc->home_z = z / 65536;
	}
	return obj;
}

//--------------------------------------------------------------------------
// オブジェクトにスクリプトを与え、はじめから動かす
// obj    : オブジェクト
// script : スクリプト
//--------------------------------------------------------------------------
void ObjectDispatch_Initialize(struct DispatchObject *obj, const s32 *script)
{
	if (obj != 0)
	{
		obj->value_04 = 0;
		obj->value_00 = (u32)script;
		obj->value_5b = 0;
		obj->value_5d = 0;
		obj->value_57 = 0;
	}
}

//--------------------------------------------------------------------------
// 絵がひとつならその絵に、絵の組ならある絵すべてに apply(絵, value) を呼ぶ
//--------------------------------------------------------------------------
#define DEFINE_APPLY_TO_CHILDREN(name, object_type, apply) \
	void name(object_type raw, s32 value)                  \
	{                                                      \
		struct DispatchObject *obj;                        \
		void				 **list;                       \
		s32					   n;                          \
		void				  *spr;                        \
                                                           \
		obj = raw;                                         \
		if (obj != 0)                                      \
		{                                                  \
			switch (obj->kind & 0xf)                       \
			{                                              \
			case 1:                                        \
				apply(obj->target.child, value);           \
				break;                                     \
			case 2:                                        \
				list = obj->target.children;               \
				n = 3;                                     \
				do                                         \
				{                                          \
					spr = *list++;                         \
					if (spr != 0)                          \
						apply(spr, value);                 \
					n--;                                   \
				} while (n >= 0);                          \
				break;                                     \
			}                                              \
		}                                                  \
	}

// オブジェクトの絵（すべて）のアニメを no にする
DEFINE_APPLY_TO_CHILDREN(ObjectDispatch_ApplyArgumentToChildren, void *,
	AnimationObjects_SelectAnimation)

// オブジェクトの絵（すべて）の AnimationObjects_SetActiveTimerSteps を呼ぶ
DEFINE_APPLY_TO_CHILDREN(ObjectDispatch_ApplyValueToChildren, struct DispatchObject *,
	AnimationObjects_SetActiveTimerSteps)

//--------------------------------------------------------------------------
// 上のふたつをいっしょに：アニメを no にし、steps をわたす
//--------------------------------------------------------------------------
void ObjectDispatch_ApplyPairToChildren(struct DispatchObject *obj, s32 no, s32 steps)
{
	void **list;
	void  *spr;
	s32	   n;

	if (obj != 0)
	{
		switch (obj->kind & 15)
		{
		case 1:
			AnimationObjects_SelectAnimation(obj->target.child, no);
			AnimationObjects_SetActiveTimerSteps(obj->target.child, steps);
			break;
		case 2:
			list = obj->target.children;
			for (n = 3; n >= 0; n--)
			{
				spr = *list++;
				if (spr != 0)
				{
					AnimationObjects_SelectAnimation(spr, no);
					AnimationObjects_SetActiveTimerSteps(spr, steps);
				}
			}
			break;
		}
	}
}

//--------------------------------------------------------------------------
// 絵がひとつのオブジェクトなら、絵の回転を設定する
//--------------------------------------------------------------------------
void ObjectDispatch_SetChildRotation(struct DispatchObject *obj, u32 rot)
{
	if (obj != 0 && (obj->kind & 0xf) == 1)
		((struct FieldSprite *)obj->target.child)->rotation = rot;
}

//--------------------------------------------------------------------------
// 絵がひとつのオブジェクトなら、アニメ番号を no にして初めから動かす
// no が負ならなにもしない
//--------------------------------------------------------------------------
void Animation_SetIndexAndInitObjects(void *raw, s32 no)
{
	struct DispatchObject  *obj = raw;
	struct AnimationObject *spr;

	if (obj != NULL && (obj->kind & 0xf) == 1)
	{
		spr = obj->target.child;
		if (no >= 0)
		{
			spr->entries[0]->anim_id = no;
			Animation_InitializeObjects(spr);
		}
	}
}

//--------------------------------------------------------------------------
// 絵がひとつのオブジェクトなら、その絵に no の情報を登録する
// no が負ならなにもしない
//--------------------------------------------------------------------------
void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *obj, s32 no)
{
	if (obj != 0 && (obj->kind & 0xf) == 1)
	{
		struct AnimationObject *spr = obj->target.child;
		if (no >= 0)
			ResourceMetadata_Register(spr, no);
	}
}

//--------------------------------------------------------------------------
// 決まったスクリプトでオブジェクトを動かしはじめる
//--------------------------------------------------------------------------
void ObjectDispatch_InitFromTable5WithArgument(struct DispatchObject *obj, s32 arg)
{
	ObjectDispatch_Initialize(obj, ObjectDispatch_Table5Script);
	obj->argument = arg;
}

void ObjectDispatch_InitFromTable0(struct DispatchObject *obj)
{
	ObjectDispatch_Initialize(obj, ObjectDispatch_Table0Script);
}

void ObjectDispatch_InitFromTable1(struct DispatchObject *obj)
{
	ObjectDispatch_Initialize(obj, ObjectDispatch_Table1Script);
}

void ObjectDispatch_InitFromTable2(struct DispatchObject *obj)
{
	ObjectDispatch_Initialize(obj, ObjectDispatch_Table2Script);
}

void ObjectDispatch_InitFromTable3(struct DispatchObject *obj)
{
	ObjectDispatch_Initialize(obj, ObjectDispatch_Table3Script);
}

void ObjectDispatch_InitFromTable6(struct DispatchObject *obj)
{
	ObjectDispatch_Initialize(obj, ObjectDispatch_Table6Script);
}

void ObjectDispatch_InitFromTable4WithArgument(struct DispatchObject *obj, s32 arg)
{
	ObjectDispatch_Initialize(obj, ObjectDispatch_Table4Script);
	if (arg != 0)
	{
		obj->value_34 = 0x8000;
		obj->value_30 = 0x40000;
		obj->argument = arg;
		obj->value_64 = 0;
	}
}

//--------------------------------------------------------------------------
// スクリプトが命令 0x10 に来るまで待つ（300 フレームまで）
//--------------------------------------------------------------------------
void ObjectDispatch_WaitForValue16(void *raw)
{
	struct ScriptInterpreter *sc = raw;
	s32						  cnt;

	cnt = 0;
	if (sc->script[sc->cursor] != 0x10)
	{
		do
		{
			WaitFrames(1);
			cnt++;
			if (cnt > 299)
				break;
		} while (sc->script[sc->cursor] != 0x10);
	}
}

//--------------------------------------------------------------------------
// 絵がひとつのオブジェクトなら、絵の flags を設定する
//--------------------------------------------------------------------------
void ObjectDispatch_SetSingleChildFlags(struct DispatchObject *obj, u32 flags)
{
	if (obj != NULL)
	{
		if ((obj->kind & 0xf) == 1)
			((struct AnimationObject *)obj->target.child)->flags = flags;
	}
}

//--------------------------------------------------------------------------
// 絵がひとつのオブジェクトなら、絵の半透明のしかたを設定する
//--------------------------------------------------------------------------
void ObjectDispatch_SetChildBlendMode(struct DispatchObject *obj, u32 mode)
{
	if (obj != 0 && obj->kind == 1)
	{
		struct FieldSprite *spr = obj->target.child;
		spr->blend_mode = mode;
	}
}

//--------------------------------------------------------------------------
// 絵がひとつのオブジェクトなら、絵の display_flags の bit1 を設定する
//--------------------------------------------------------------------------
void ObjectDispatch_SetChildDisplayFlag1(struct DispatchObject *obj, u32 on)
{
	if (obj != 0 && obj->kind == 1)
	{
		struct FieldSprite *spr = obj->target.child;
		spr->unknown_1d_1 = on;
	}
}

//--------------------------------------------------------------------------
// 絵がひとつのオブジェクトなら、ObjectGroup_SetChildValueUnlessFifteen を呼ぶ
//--------------------------------------------------------------------------
void Animation_ApplyChildValues(struct DispatchObject *obj, u32 val)
{
	if (obj != NULL && obj->kind == 1)
		ObjectGroup_SetChildValueUnlessFifteen(obj->target.child, val);
}

//--------------------------------------------------------------------------
// カメラの処理を動かし、BG1-3 を消して OBJ を出す
//--------------------------------------------------------------------------
void Graphics_EnableObjLayerAndCallbacks(void)
{
	Scheduler_EnableCallbacks(ObjectSystem_UpdateCamera);
	Scheduler_EnableCallbacks(ObjectSystem_UpdateCameraFixed);
	BattleFx_ApplyColorToTargetBufferFar(0x10000, 1);
	BattleFx_StartBufferInterpolationFar(1);
	WaitFrames(1);
	REG_DISPCNT = (REG_DISPCNT & 0xf1ff) | DISPCNT_OBJ_ON;
}

//--------------------------------------------------------------------------
// カメラの処理を止め、BG1-3 と OBJ を消す
//--------------------------------------------------------------------------
void ObjectDispatch_StopCallbacksAndHideLayers(void)
{
	Scheduler_DisableCallbacks(ObjectSystem_UpdateCamera);
	Scheduler_DisableCallbacks(ObjectSystem_UpdateCameraFixed);
	REG_DISPCNT &= 0xe1ff;
}

s32 ObjectDispatch_ReturnTrue(void)
{
	return 1;
}

// マップのセル
struct CameraTile
{
	u32 unk_00 : 12;
	u32 layer : 2;	  // 0 でなければ、上にいるオブジェクトのレイヤー + 1
	u32 priority : 2; // 0 でなければ、上にいるオブジェクトの表示の優先度
	u32 unk_10 : 16;
};

// オブジェクトの絵（OAM の属性をふくむ）
struct CameraSprite
{
	u8	unknown_00[4];
	u16 y : 8;
	u16 affine : 2;
	u16 blend_mode : 2;
	u16 mosaic : 1;
	u16 full_color : 1;
	u16 shape : 2;
	u16 x : 9;
	u16 affine_index : 5;
	u16 flip_x : 1;
	u16 flip_y : 1;
	u16 tile : 10;
	u16 priority : 2;
	u16 palette : 4;
	u8	unknown_0a[0x0a];
	u16 second_tile : 10;
	u16 second_priority : 2;
	u16 second_palette : 4;
	u8	unknown_16[2];
	s32 scale;
	u8	resource; // VRAM ブロックの番号
	u8	flags_1d;
	u8	unk_1e[7];
	u8	activated;
};

// オブジェクト
struct CameraObject
{
	u32	  active;
	u16	  unk_04;
	u16	  angle;
	s32	  x;
	s32	  y;
	s32	  z;
	s32	  height;
	s32	  scale_x;
	s32	  scale_y;
	u8	  unk_20[2];
	u8	  layer;
	u8	  flags;
	u8	  unk_24[0x2c];
	void *sprite;
	u8	  kind; // 1 絵ひとつ、2 絵が 4 つまで
	u8	  unk_55[7];
	u8	  held;
	u8	  unk_5d[0x13];
};

struct CameraLayer
{
	struct CameraTile *tiles;
	u8				   unk_04[44];
};

// マップ
struct CameraState
{
	u8				   unk_000[0xe4];
	s32				   x; // 見ている位置
	s32				   z;
	u8				   unk_0ec[0x44];
	struct CameraLayer layers[1];
};

struct CameraPoint
{
	s32 x;
	s32 y;
	s32 z;
};

// 固定カメラ
struct FixedCamera
{
	struct CameraPoint	eye;
	struct CameraPoint	target;
	struct CameraPoint *eye_override;	 // あればこちらの視点を使う
	struct CameraPoint *target_override; // あればこちらの注視点を使う
};

//--------------------------------------------------------------------------
// 毎フレーム：オブジェクトの絵を画面に置く（ふつうのカメラ）
// 画面のまわりにいるオブジェクトは、足もとのセルからレイヤーと優先度を決めて
// 置く。外にいれば絵の VRAM ブロックを手放す
//--------------------------------------------------------------------------
void ObjectSystem_UpdateCamera(void)
{
	/* FAKEMATCH: frame local: unused; the native frame keeps room for
	   three points above the position. Measured (2026-10-13): without it
	   the frame is 44 bytes for 80 (differ 4, the two sp adjustments). */
	struct CameraPoint		 spare[3];
	s32						 cnt;
	struct CameraState		*map;
	s32						 cam_x;
	s32						 cam_z;
	struct ObjectSystemWork *sync;
	struct CameraObject		*obj;
	struct CameraSprite		*spr;
	s32						*cam;
	u32						 kind;
	s32						 dx;
	s32						 dz;
	s32						 top;
	struct CameraTile		*tile;
	u32						 bits;
	s32						 scale[2];
	s32						 pos[4];
	u32						 bits_val;

	map = *(struct CameraState **)(((u8 *)&gObjectSlots) + 12);
	cam = &map->x;
	cam_x = cam[0] & 0xffff0000;
	cam_z = cam[1] & 0xffff0000;
	sync = *(struct ObjectSystemWork **)(((u8 *)&gObjectSlots) + 4);
	// 絵を置く処理（Render_DecodeFrame）を作業領域へ写す
	RamCode_LoadSlot(52, Render_DecodeFrame, Render_DecodeFrameCodeSize);
	sync->used = 0;
	obj = *(struct CameraObject **)((u8 *)&gObjectSlots);
	for (cnt = 0; cnt < 64; cnt++, obj++)
	{
		if (obj->active == 0)
			continue;
		if (obj->x != 0 || obj->z != 0)
		{
			kind = obj->kind & 15;
			if (kind == 0)
				continue;
			if (kind != 1)
				continue;
			if (sync->suspended != 0 && obj->held == 0)
			{
				struct CameraSprite *frozen = obj->sprite;

				Resource_ActivateEntry(frozen->resource);
				frozen->activated = kind;
				continue;
			}
			dx = obj->x - cam_x;
			dz = obj->z - cam_z;
			top = dz - obj->y;
			spr = obj->sprite;
			if (dx > -0x200000 && dx < 0x1100000 && top > -0x200000 && top < 0xe00000)
			{
				// 足もとのセルの情報
				tile = &map->layers[obj->layer].tiles[(obj->x >> 20) + ((obj->z >> 20) << 7)];
				if (obj->flags & 1)
				{
					bits = tile->priority;
					if (bits != 0)
					{
						spr->priority = bits;
						spr->second_priority = bits;
					}
				}
				bits_val = tile->layer;
				if (bits_val != 0)
					obj->layer = bits_val - 1;
				scale[0] = Iwram_MulQ16(obj->scale_x, spr->scale);
				scale[1] = Iwram_MulQ16(obj->scale_y, spr->scale);
				pos[0] = dx;
				pos[1] = obj->y;
				pos[2] = dz;
				pos[3] = obj->height;
				if (obj->flags & 2)
				{
					pos[1] += 0xfec00000; // -20 ドット
					pos[2] += 0xfec00000;
					pos[3] += 0xfec00000;
				}
				if (obj->flags & 4)
				{
					pos[1] += 0x1400000; // +20 ドット
					pos[2] += 0x1400000;
					pos[3] += 0x1400000;
				}
				Render_ApplyProjectedPlacement(spr, pos, scale, obj->angle);
				continue;
			}
			if (obj->held == 0)
			{
				if (!(spr->flags_1d & 1))
				{
					Resource_ActivateEntry(spr->resource);
					spr->activated = 1;
				}
			}
		}
		else
		{
			kind = obj->kind & 15;
			if (kind == 1)
			{
				spr = obj->sprite;
				if (obj->held == 0 && !(spr->flags_1d & 1))
				{
					Resource_ActivateEntry(spr->resource);
					spr->activated = kind;
				}
			}
		}
	}
	Runtime_ReleaseHeapBlock(52);
}

s32 ObjectCamera_ReturnTrue(void)
{
	return 1;
}

//--------------------------------------------------------------------------
// 毎フレーム：オブジェクトの絵を画面に置く（固定カメラ）
// 視点と注視点から向きを決め、64 番目のオブジェクトから順に置く
// コマンドを選んでいるあいだ（フラグ 363）は、決まった向きの行列を使い
// 大きさを 1 倍にする
//--------------------------------------------------------------------------
void ObjectSystem_UpdateCameraFixed(void)
{
	s32						 cnt;
	struct FixedCamera		*cam;
	struct ObjectSystemWork *sync;
	struct CameraPoint		*eye;
	struct CameraPoint		*tgt;
	struct CameraObject		*obj;
	s32						 angle;
	s32						 part;
	s32						 kind;
	s32						*pos;
	s32						 one;
	void				   **parts;
	void					*spr;
	s32						 scale2[2];
	s32						 scale[2];
	/* FAKEMATCH: frame local: unused; the native frame keeps room for two
	   points above the scales. Measured (2026-10-13): without it the frame
	   is 28 bytes for 52 (differ 4, the two sp adjustments). */
	struct CameraPoint spare[2];

	cam = *(struct FixedCamera **)(((u8 *)&gObjectSlots) + 0x1c);
	sync = *(struct ObjectSystemWork **)(((u8 *)&gObjectSlots) + 4);
	RamCode_LoadSlot(52, Render_DecodeFrame, Render_DecodeFrameCodeSize);
	eye = &cam->eye;
	tgt = &cam->target;
	if (cam->eye_override != NULL)
		eye = cam->eye_override;
	if (cam->target_override != NULL)
		tgt = cam->target_override;
	angle = (s16)ArcTan2((eye->x - tgt->x) >> 16, (eye->z - tgt->z) >> 16);
	sync->used = 0;
	Render_ResetTransformState();
	if (GameFlag_TestFar(363))
	{
		angle += 0xffffe000;
		Iwram_TransformMatrix(Camera_FixedViewMatrix);
		Graphics_PrepareTransferAndRun(eye, tgt);
	}
	else
	{
		Graphics_PrepareTransferInIwramWork(eye, tgt);
	}
	obj = *(struct CameraObject **)((u8 *)&gObjectSlots);
	obj += 63;
	one = 0x10000;
	for (cnt = 63; cnt >= 0; cnt--, obj--)
	{
		if (obj->active == 0)
			continue;
		kind = obj->kind & 15;
		switch (kind)
		{
		case 1:
			pos = &obj->x;
			scale[0] = obj->scale_x;
			scale[1] = obj->scale_y;
			spr = obj->sprite;
			if (GameFlag_TestFar(363))
			{
				scale[0] = one;
				scale[1] = one;
			}
			Render_PlaceProjectedSprite(spr, pos, scale, obj->angle + angle, obj->layer);
			break;
		case 2:
			scale2[0] = obj->scale_x;
			scale2[1] = obj->scale_y;
			if (GameFlag_TestFar(363))
			{
				scale2[0] = one;
				scale2[1] = one;
			}
			parts = obj->sprite;
			for (part = 3; part >= 0; part--)
			{
				spr = *parts++;
				if (spr != NULL)
					Render_PlaceProjectedSprite(
						spr, &obj->x, scale2, obj->angle + angle, obj->layer);
			}
			break;
		case 0:
			break;
		}
	}
	Runtime_ReleaseHeapBlock(52);
}

//--------------------------------------------------------------------------

// script.c

// オブジェクトスクリプト：位置の移動と byte_54 の命令

//--------------------------------------------------------------------------
