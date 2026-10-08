//--------------------------------------------------------------------------
// result_request.c
// バトル効果：イベントから決まった敵との戦闘を用意する
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
#include "event_runtime.h"
#include "object_lookup.h"
#include "resource_ids.h"
#include "game_state.h"

#define BattleFx_GetPhaseResult ((s16 (*)(s32))BattleFx_GetPhaseResult)
extern struct EventRuntime *gEventWork;

extern struct GameState gGameState;

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
