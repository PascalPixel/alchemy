//--------------------------------------------------------------------------
// MAPFX.C
// マップの表示範囲とズームの移り変わり、ほかのオブジェクトについて回るオブジェクト
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
#include "MEM.H"
#include "ANIMSPR.H"
#include "FIELDOBJ.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "METADATA_LOOKUP.H"
#include "OBJDISP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "IWRAM_CALL.H"
#include "FIXED_MATH.H"
#include "BATTLE_EFFECT_RUNTIME.H"

#include "MAP_SCROLL.H"
#include "HEAP_STATE.H"

#define WaitFrames ((s32 (*)(s32))WaitFrames)

#define gScanlinePitch ((u8 *)&gScanlinePitch)

// スプライトの OAM 属性2 の上位バイト (bit 2〜3 が表示優先度)
#define ATTR2_HI(spr) (((u8 *)&((struct AnimationObject *)(spr))->part[0])[9])

s16 *BattleAction_FindDescriptor(s16 action);

struct FieldActor *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void			   ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 mode);
void			   AudioCommand_PlayFar(s32 cue);
void			   Battle_WaitMode0(s32 state);

//--------------------------------------------------------------------------
// マップの表示範囲を決める
//--------------------------------------------------------------------------
void Map_SetViewBounds(s32 min_x, s32 min_y, s32 max_x, s32 max_y)
{
	struct MapScrollWork *work = gMapWork[0];

	work->min_x = min_x;
	work->min_y = min_y;
	work->max_x = max_x;
	work->max_y = max_y;
}

//--------------------------------------------------------------------------
// 毎フレームの処理 : ズームを前の値から目標へ 1 段すすめる
// 画面中心のオブジェクトが止まっているあいだは待つ
//--------------------------------------------------------------------------
s32 BattleFx_StepRatioTransition(void)
{
	struct PerspectiveWork *work;
	s16					   *frames;
	s32					   *from;
	s16					   *step;
	s32						zoom;
	s32						delta;

	work = gMapWork[0];
	if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))->view_center->paused
		!= 0)
		return;
	frames = (s16 *)&work->zoom_transition_frames;
	if (*frames == 0)
		return;
	from = &work->previous_zoom;
	delta = work->zoom - *from;
	step = &work->zoom_transition_step;
	(*step)++;
	zoom = *from + __divsi3(delta * *step, *frames);
	work->distance = Iwram_MulQ16(work->far_plane, zoom);
	*(u32 *)gScanlinePitch = work->pitch + 1;
	if (*step == *frames)
	{
		*frames = 0;
		Scheduler_RemoveCallback((SchedulerCallback)BattleFx_StepRatioTransition);
	}
}

//--------------------------------------------------------------------------
// ズームの移り変わりを始める (イベントの mode が 3 のときだけ)
// zoom   : 倍率 (16.16 固定小数)
// frames : かけるフレーム数
//--------------------------------------------------------------------------
void BattleFx_ScheduleRatioTransition(s32 zoom, s32 frames)
{
	struct PerspectiveWork *work = gMapWork[0];
	s32						inv;

	if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))->mode != 3)
		return;
	{
		s32 (*ratio)(s32, s32) = Iwram_RatioMulQ14;
		inv = ratio(zoom, 0x10000); // 1 / zoom
	}
	work->previous_zoom = work->zoom;
	work->zoom = inv;
	work->zoom_transition_frames = frames;
	work->zoom_transition_step = 0;
	Scheduler_AddOrUpdateCallback((SchedulerCallback)BattleFx_StepRatioTransition, 0xc94);
}

//--------------------------------------------------------------------------
// ズームの移り変わりが終わるまで待つ (イベントの mode が 3 のとき、最長 300 フレーム)
//--------------------------------------------------------------------------
void Event_WaitForZoomTransition(void)
{
	s32						frames;
	struct PerspectiveWork *work = gMapWork[0];

	if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))->mode == 3)
	{
		frames = 0;
		if (*(s16 *)&work->zoom_transition_frames != 0)
		{
			do
			{
				WaitFrames(1);
				frames++;
			} while (frames < 300 && *(s16 *)&work->zoom_transition_frames != 0);
		}
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理 : 付いていく相手の位置に置き、y には絵の box_x と 8 ドットを足す
//--------------------------------------------------------------------------
s32 BattleFx_CopyLinkedObjectPosition(void *self)
{
	struct FieldActor *obj = self;
	struct FieldActor *link;

	link = *(struct FieldActor **)obj->unknown_68;
	if (link != NULL)
	{
		obj->motion_flags = 0;
		obj->x.fixed = link->x.fixed;
		obj->y.fixed =
			link->y.fixed
			+ ((s8)Resource_GetMetadataRecordFar(*BattleAction_FindDescriptor(*(s16 *)&obj->work))
					->box_x
				<< 16)
			+ (8 << 16);
		obj->terrain_height = link->terrain_height;
		obj->z.fixed = link->z.fixed;
	}
	return 0;
}

//--------------------------------------------------------------------------
// id のオブジェクトについて回るオブジェクト (種類 21) を出し、wait フレーム待つ
// mode : 下位 4 ビットはオブジェクトの mode、下位 8 ビットが 6 なら音 110、
//        0x100 で表示優先度 1 (なければ相手と同じ優先度)
//--------------------------------------------------------------------------
void BattleFx_SpawnLinked(s32 id, s32 mode, s32 wait)
{
	struct FieldActor *target;

	if ((mode & 0xff) == 6)
	{
		AudioCommand_PlayFar(110);
	}

	target = ObjectTable_Get(id);
	if (target != 0)
	{
		struct FieldActor *obj =
			Object_CreateFar(21, target->x.fixed, target->y.fixed, target->z.fixed);

		if (obj != 0)
		{
			ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_LinkedObjectScript);
			ObjectDispatch_ApplyArgumentToChildrenFar(obj, mode & 15);
			obj->motion_flags = 0;
			obj->timer = 0;
			obj->work = id;
			obj->update = (void (*)(union FieldObject *))BattleFx_CopyLinkedObjectPosition;
			obj->sprite->flags = 0;
			*(struct FieldActor **)&obj->unknown_68[0] = target;

			if ((mode & 0x100) != 0)
			{
				s32 mask = 13;
				u8	attr = ATTR2_HI(obj->sprite);

				mask = -mask;
				mask &= attr;
				mask |= 4;
				ATTR2_HI(obj->sprite) = mask;
			}
			else
			{
				s32 pri = 12;
				u8	src = ATTR2_HI(target->sprite);
				u8	dst;
				s32 mask = 13;

				pri &= src;
				dst = ATTR2_HI(obj->sprite);
				mask = -mask;
				mask &= dst;
				mask |= pri;
				ATTR2_HI(obj->sprite) = mask;
			}
		}
		Battle_WaitMode0(wait);
	}
}
