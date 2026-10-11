#include "scene.h"
#include "event_runtime.h"
#include "game_state.h"
#include "types.h"
#include "disptran.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "object_lookup.h"
#include "resource_ids.h"

extern struct EventRuntime *gEventWork;

void Event_StartScreenTransition(void)
{
    struct EventRuntime *runtime = gEventWork;

    DisplayTransition_Start(runtime->value_1c0, runtime->value_1c8);
    runtime->status_1c6 = 1;
}

void Event_FinishScreenTransition(void)
{
    struct EventRuntime *runtime = gEventWork;

    DisplayTransition_Finish(runtime->value_1c0, runtime->value_1c8);
    runtime->status_1c6 = 0;
}

void Event_WaitTransitionFrames(void)
{
    WaitFrames(gEventWork->value_1c8);
}

void Event_SetDestination(u16 first, u16 second)
{
    gEventWork->value_170 = 999;
    gGameState.scene = first;
    gGameState.entrance = second;
}

void Event_SetSavedDestination(u16 first, u16 second)
{
    struct EventRuntime *state = gEventWork;
    state->value_170 = 999;
    gGameState.saved_scene = first;
    gGameState.saved_entrance = second;
}

void Event_SetExitRequest(u16 value)
{
    struct EventRuntime *state = gEventWork;
    state->value_170 = value;
}

//--------------------------------------------------------------------------

// result_request.c

// バトル効果：イベントから決まった敵との戦闘を用意する

//--------------------------------------------------------------------------

#define BattleFx_GetPhaseResult ((s16 (*)(s32))BattleFx_GetPhaseResult)

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
	register struct EventRuntime *ev = gEventWork;

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

	ev = gEventWork;
	skip = flag & TOBANAI;
	flag &= 0xff;

	if (skip == 0)
		EventActor_RunHopAndLeapSequence(flag);

	gs = &gGameState;
	gs->pending_djinn_event = (no + JIN_0) | skip;
	ev->value_17c = BattleFx_GetPhaseResult(no); // ジンの敵グループ
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
    struct EventRuntime *work = gEventWork;
    gGameState.scene = first;
    gGameState.entrance = second;
    work->value_170 = 999;
}
