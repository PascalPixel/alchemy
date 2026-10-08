//--------------------------------------------------------------------------
// HEAVY_IMPACT_FINISH.C
// フィールド効果：ジンが仲間になる演出 (属性3)
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
#include "TYPES.H"
#include "FX_SCENE.H"
#include "GAME_STATE.H"
#include "FIXED_MATH.H"
#include "MOTOBJ.H"

extern struct BattleFxScene *gEffectWork;

struct MotionObject *Object_GetById(s32 id);

int BattleFx_ClearActiveSlotsAndScheduleUpdates();
int ObjectMotion_ArmCallback();
int ObjectMotion_Launch();

//--------------------------------------------------------------------------
// ジンが跳ねて光の粒になり、操作キャラへ集まる
// jin_id : ジンのオブジェクト番号
//--------------------------------------------------------------------------
void BattleFx_FinishHeavyImpact(s32 jin_id)
{
	struct
	{
		s32 x;
		s32 y;
		s32 z;
	} pos;
	struct MotionObject	 *jin;
	struct BattleFxScene *scene;
	struct EffectSlot	 *slot;
	s32					  scale;
	s32					  cnt;

	jin = Object_GetById(jin_id);
	/* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling */
	do
	{
		if (jin == NULL)
			return;
		BattleFx_InitializeSlots();
		scene = gEffectWork;
		Shop_RestoreSceneTilesFar(0x20118c);
		Audio_PlayCue(173);
		Motion_SetVarCbAndRefresh(jin_id, 1);
	} while (0);
	Audio_PlayCue(174);
	Motion_SetVarCbAndRefresh(jin_id, 1);
	/* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling */
	do
	{
		Audio_PlayCue(175);
		Motion_SetVarCbAndRefresh(jin_id, 1);
		WaitFrames(20);

		// 回転してから高く跳ねる
		Audio_PlayCue(140);
		jin->update = BattleFx_AdvanceSpinAngle;
		WaitFrames(40);
		Audio_PlayCue(153);
		ObjectMotion_Launch(jin_id, 12, 22);

		// ジンの画面位置から光の粒を 24 個出す
		pos.x = jin->x;
		pos.y = jin->y;
		pos.z = jin->z;
		Camera_WorldToScreen(&pos);
		ObjectDispatch_ReleaseFar(jin);
		Audio_PlayCue(164);
		slot = scene->slots;
		cnt = 23;
		do
		{
			EffectSlot_Initialize(slot, 284, pos.x, pos.z);
			EffectSlot_SetCallback(
				slot, BattleFx_RunAngledApproachPhases); // 回ってから操作キャラへ
			EffectSlot_SetObjectMode(slot, 7);
			ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, (u32)(Random16() * 7) >> 16);
			scale = (u32)Random16() / 3 + 0x10000; // 1.0〜1.33 倍
			slot->scale_y = scale;
			slot->scale_x = scale;
			cnt--;
			WaitFrames(1);
			slot++;
		} while (cnt >= 0);
	} while (0);

	// 操作キャラを 0x4000 (90°) の向きへ回し、動作 28 にする
	WaitFrames(60);
	ObjectMotion_ArmCallback(gGameState.selected_actor, 0x4000, 0);
	WaitFrames(20);
	Object_SetMode(Object_GetById(gGameState.selected_actor), 28);
	WaitFrames(40);
	Audio_PlayCue(164);
	WaitFrames(100);
	Shop_InitEffectFar();
	BattleFx_ClearActiveSlotsAndScheduleUpdates();
}
