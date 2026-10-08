//--------------------------------------------------------------------------
// DISPATCH_TYPE_HANDLER.C
// フィールド：演出処理の呼び分け
//--------------------------------------------------------------------------

#include "TYPES.H"
#include "BATTLE.H"
#include "FIELD.H"

s32 BattleFx_Run(void);
s32 BattleFx_DispatchRequestKind(void);

//--------------------------------------------------------------------------
// 今の演出の段階 kind に応じた処理を呼ぶ
// kind : 0 開始、1 対象への処理、2 終わり、3 種類ごとの処理
//--------------------------------------------------------------------------
void Field_DispatchTypeHandler(s32 kind)
{
	if (kind == 0)
		BattleFx_Run();
	else if (kind == 1)
		BattleFx_DispatchRequestKind();
	else if (kind == 2)
		BattleFx_ClearChildValueOnMismatch();
	else if (kind == 3)
		FieldEvent_RunTypeHandler();
}
