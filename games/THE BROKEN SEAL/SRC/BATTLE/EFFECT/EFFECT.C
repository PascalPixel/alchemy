//--------------------------------------------------------------------------
// EFFECT.C
// フィールド効果：種類ごとの振り分けと、効果のイベント処理
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
#include "BATTLE_EFFECT_RUNTIME.H"
#include "HEAP_STATE.H"
#include "ANIMSPR.H"
#include "OBJECT_RUNTIME.H"
#include "GAME_STATE.H"
#include "FX_SCENE.H"
#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "BTLSYS.H"
#include "text/MSG_IDS.H"

#define gFrameCount (*(volatile s32 *)&gFrameCount)

#define BattleFx_MarkChildAndRunFallbackTransition \
	((void (*)(s32))BattleFx_MarkChildAndRunFallbackTransition)
#define BattleEffect_RunTargetedItemBreak ((void (*)(s32))BattleEffect_RunTargetedItemBreak)
#define BattleFx_CallEffect04 ((void (*)(s32))BattleFx_CallEffect04)
#define BattleFx_CallEffect05 ((void (*)(s32))BattleFx_CallEffect05)
#define BattleFx_StartOrbitingParticles ((void (*)(s32))BattleFx_StartOrbitingParticles)
#define BattleFx_RunBurstParticleMainObject ((void (*)(s32))BattleFx_RunBurstParticleMainObject)
#define BattleFx_CallEffect03AndStop ((void (*)(s32))BattleFx_CallEffect03AndStop)
#define BattleFx_RunEffect13Hook ((void (*)(s32))BattleFx_RunEffect13Hook)
#define BattleFx_ResumeObject ((void (*)(s32))BattleFx_ResumeObject)

#define FX_SLOT 56 // gWorkSlot[56] は gEffectWork

void BattleEffect_RunFallbackObjectTransition(void);

extern struct BattleFxScene *gEffectWork;
void						 RunSceneTransitionEffect(s32 source_id, s32 target_id);
void						 RunBattleEffect04(void);
void						 RunBattleEffect14(void);
void						 BattleEffect_PauseObject(s32 obj_id);

void Event_SetMessageId(s32);
void BattleFx_FinishAction();
void _call_via_r3(s32, s32);
void UiText_ShowPositionedMessageAndWaitFar(s32, s32);
void Battle_Reset();

void Vector_AddPolarOffset(s32, s32, s32 *);
void Object_SetMoveTargetFar(struct ObjectRuntime *, s32, s32, s32);

//--------------------------------------------------------------------------
// 効果の種類 (animation) ごとに本体を走らせる
//--------------------------------------------------------------------------
void BattleFx_Run(void)
{
	struct BattleFxScene *scene;
	struct BattleRuntime *runtime;
	s32					  kind;
	s32					  target_id;
	s32					  obj_id;

	scene = *(gWorkSlot + FX_SLOT);
	runtime = *(gWorkSlot + HEAP_SLOT_EVENT);
	kind = scene->animation;
	target_id = scene->second_object_id;

	switch (kind)
	{
	case 1:
		BattleFx_RunItemBreakSequence();
		return;
	case 7:
		RunBattleEffect07();
		return;
	case 11:
		RunBattleEffect11();
		return;
	case 4:
		RunBattleEffect04();
		return;
	case 5:
		RunBattleEffect05();
		return;
	case 14:
		RunBattleEffect14();
		return;
	case 6:
		BattleFx_RunOrbitingParticles();
		return;
	case 3:
		RunBattleEffect03();
		return;
	case 12:
		BattleFx_RunBurstParticles();
		return;
	case 13:
		RunBattleEffect13();
		return;
	case 9: // 前に止めた対象を戻し、近くの対象を止める
		if (gGameState.paused_object != -1)
		{
			BattleFx_ResumeObject(gGameState.paused_object);
			gGameState.paused_object = -1;
		}

		obj_id = BattleEffect_SelectNearbyTargetObject(gGameState.selected_actor, kind);
		obj_id = BattleFx_FilterObjectIdByFlags(obj_id);
		if (BattleFx_FindDescriptorWithOverride(obj_id) != 0)
		{
			BattleFx_SetupObjectPair(gGameState.selected_actor, obj_id);
			BattleFx_MarkChildAndRunFallbackTransition(obj_id);
			BattleEffect_PauseObject(obj_id);
			gGameState.paused_object = obj_id;
		}
		else
		{
			BattleEffect_RunFallbackObjectTransition();
		}
		return;
	case 2: // 波と色の効果 (種類 8 の効果が動いていれば先に止める)
		if (*(s16 *)&runtime->unknown_cb8[0] != 0)
			ResetSceneTransitionEffect();
		RunSceneTransitionEffect(scene->first_object_id, target_id);
		return;
	case 8:
		RunBattleEffect08();
		return;
	case 10:
		BattleFx_RunFlashingCallbackSequence();
		return;
	case 15:
		BattleFx_RunEffect15();
		return;
	case 16:
		RunBattleEffect16();
		return;
	}
}

//--------------------------------------------------------------------------
// 対象が決まっているときの振り分け
//--------------------------------------------------------------------------
void BattleFx_DispatchRequestKind(void)
{
	struct BattleFxScene *scene = *(gWorkSlot + FX_SLOT);
	struct BattleRuntime *runtime = *(gWorkSlot + HEAP_SLOT_EVENT);
	s32					  kind = scene->animation;
	s32					  target_id = scene->second_object_id;

	scene->enabled = 0;
	switch (kind)
	{
	case 2:
		if (*(s16 *)&runtime->unknown_cb8[0] != 0)
			ResetSceneTransitionEffect();
		if (gGameState.paused_object != scene->second_object_id)
			((struct ObjectRuntime *)scene->child)->movement_state = 1;
		RunSceneTransitionEffect(scene->first_object_id, target_id);
		break;
	case 1: // 方向キーで対象を押す
		BattleFx_SteerPushedTarget(target_id);
		break;
	case 7:
		BattleEffect_RunTargetedItemBreak(target_id);
		break;
	case 11:
		BattleFx_SteerLiftedTarget(target_id);
		break;
	case 4:
		BattleFx_CallEffect04(target_id);
		break;
	case 5:
		BattleFx_CallEffect05(target_id);
		break;
	case 6:
		BattleFx_StartOrbitingParticles(target_id);
		break;
	case 12:
		BattleFx_RunBurstParticleMainObject(target_id);
		break;
	case 9:
		if (gGameState.paused_object != -1)
		{
			BattleFx_ResumeObject(gGameState.paused_object);
			gGameState.paused_object = -1;
		}
		BattleEffect_PauseObject(target_id);
		gGameState.paused_object = target_id;
		BattleFx_MarkChildAndRunFallbackTransition(target_id);
		break;
	case 3:
		BattleFx_CallEffect03AndStop(target_id);
		break;
	case 14:
		BattleFx_CallEffect14(target_id);
		break;
	case 13:
		BattleFx_RunEffect13Hook(target_id);
		break;
	case 8:
		RunBattleEffect08();
		break;
	case 10:
		BattleFx_RunFlashingCallbackSequence();
		break;
	case 15:
		BattleFx_RunEffect15();
		break;
	case 16:
		RunBattleEffect16();
		break;
	}
}

//--------------------------------------------------------------------------
// 種類 2 の後始末：効果を終え、止めていない対象の movement_state を戻す
//--------------------------------------------------------------------------
void BattleFx_ClearChildValueOnMismatch(void)
{
	struct BattleFxScene *scene = gEffectWork;

	if (scene->animation == 2)
	{
		BattleFx_FinishSceneAndReleaseHeapBlock();
		if (gGameState.paused_object != scene->second_object_id)
		{
			((struct ObjectRuntime *)scene->child)->movement_state = 0;
		}
	}
}

//--------------------------------------------------------------------------
// 種類 8, 10, 16 の追加処理
//--------------------------------------------------------------------------
void FieldEvent_RunTypeHandler(void)
{
	u32 kind;

	kind = gEffectWork->animation;
	switch (kind)
	{
	case 8:
		ResetSceneTransitionEffect();
		return;
	case 10:
		MapEvent_RunTileTriggerSequence();
		return;
	case 16:
		FieldEvent_ShowStatusMessage();
		return;
	}
}

//--------------------------------------------------------------------------
// 効果に結びついたイベントを実行する
// event  : オフセット 8 の値が 0x10000 未満ならメッセージ番号、それ以上なら処理の番地
// param  : 処理へ渡す値 (使った者など)
// target : 対象
// フラグ 322 が立っていれば「何も起こらない」を出す
//--------------------------------------------------------------------------
s32 BattleFx_RunEventAction(void *event, s32 param, s32 target)
{
	s32 script;

	if (event != NULL)
	{
		script = FIELD_AT_OFFSET(event, s32 *, 8);
		if (script != 0)
		{
			if (script < 0x10000)
			{
				Battle_Reset();
				Event_SetMessageId(FIELD_AT_OFFSET(event, s32 *, 8));
				BattleEv_RunWait(target, 0);
				BattleFx_FinishAction();
			}
			else
			{
				_call_via_r3(param, target);
			}
		}
		if (GameFlag_TestFar(322) != 0)
		{
			Battle_Reset();
			UiText_ShowPositionedMessageAndWaitFar((s32)MsgNothingHappens, 1);
			BattleFx_FinishAction();
		}
	}
	return 0;
}

//--------------------------------------------------------------------------
// 各パーツのパレットをフレーム数 % 6 にする (毎フレーム呼ぶと色が巡る)
//--------------------------------------------------------------------------
void ObjectGroup_ApplyRandomChildValues(void *owner)
{
	s32			  kind;
	void		 *anim;
	s32			  initial_count;
	s32			  cnt;
	void		**entry;
	void		 *part;
	volatile s32 *frame_count;
	s32			  value;

	kind = ((struct ObjectRuntime *)owner)->animation_kind;
	if (kind == 1)
	{
		anim = ((struct ObjectRuntime *)owner)->animation;
		if (anim != 0 && (((struct AnimationObject *)anim)->display_flags & kind) == 0)
		{
			initial_count = ((struct AnimationObject *)anim)->count;
			if (initial_count != 0)
			{
				frame_count = &gFrameCount;
				entry = (void **)((struct AnimationObject *)anim)->entries;
				cnt = initial_count;
				do
				{
					part = *entry++;
					value = (u32)*frame_count % 6;
					cnt--;
					((struct AnimationEntry *)part)->param = value;
				} while (cnt != 0);
			}
			((struct AnimationObject *)anim)->dirty = 1;
		}
	}
}

//--------------------------------------------------------------------------
// オブジェクトを向き angle へ magnitude だけずらした位置に置く
//--------------------------------------------------------------------------
void Motion_SetTargetPositionFromMagnitudeAngle(
	struct ObjectRuntime *object, s32 magnitude, s32 angle)
{
	s32 pos[3];

	if (object != 0)
	{
		pos[0] = object->x;
		pos[1] = object->y;
		pos[2] = object->z;
		Vector_AddPolarOffset(magnitude, angle, pos);
		Object_SetMoveTargetFar(object, pos[0], pos[1], pos[2]);
	}
}
