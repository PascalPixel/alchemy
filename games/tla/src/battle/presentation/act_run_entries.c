//--------------------------------------------------------------------------
// act_run_entries.c
// バトル表示：行動の効果を対象ごとに出す
//--------------------------------------------------------------------------

#include "types.h"
#include "battle.h"
#include "system.h"
#include "battle_event.h"
#include "battle_display.h"
#include "battle_type.h"

void BattlePresentation_WaitForAdvance(void);

//--------------------------------------------------------------------------
// ハイライトを消し、対象ごとに効果をかけてイベントを流す
// 対象がいなければ、メッセージの送りを待つだけ
// plan : 行動と対象の情報
//--------------------------------------------------------------------------
s32 BattlePres_RunActorEntries(struct BattlePlan *plan, s32 unused)
{
	s32 i;

	BattlePres_SetActorModes(0, 0);
	if (plan->target_count == 0)
	{
		BattlePresentation_WaitForAdvance();
	}
	else
	{
		for (i = 0; i < plan->target_count; i++)
		{
			BattleTargetExec(plan, i);
			BattleEv_DispatchQueued();
		}
	}
	WaitFrames(1);
}
