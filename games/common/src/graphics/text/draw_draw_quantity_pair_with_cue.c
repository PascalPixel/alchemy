//--------------------------------------------------------------------------
// draw_draw_quantity_pair_with_cue.c
// メッセージ：わざを覚えさせて「覚えた」と出す
//--------------------------------------------------------------------------

#include "types.h"
#include "scene.h"
#include "text/msg_ids.h"

void OwnerAction_AddFar(void);
void Audio_PlayCue(s32);
void UiText_DrawQuantity(s32, s32);
void UiText_ShowPositionedMessageAndWaitFar(void *, s32);

//--------------------------------------------------------------------------
// わざを覚えさせ、効果音とメッセージを出して、送られるまで待つ
// unit : キャラクター番号（メッセージの名前 1）
// abil : わざの番号（メッセージのわざ 4）
//--------------------------------------------------------------------------
void UiText_DrawQuantityPairWithCue(s32 unit, s32 abil)
{
	// 引数なしの宣言だが、r0・r1 の unit・abil がそのまま渡る
	OwnerAction_AddFar();
	Audio_PlayCue(83);
	UiText_DrawQuantity(unit, 1);
	UiText_DrawQuantity(abil, 4);
	UiText_ShowPositionedMessageAndWaitFar(MsgAbilityLearned, 3);
}
