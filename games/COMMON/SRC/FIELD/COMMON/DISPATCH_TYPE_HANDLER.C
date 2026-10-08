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
// kind に応じた演出の処理を呼ぶ
// kind : 0 対象を指定せずに実行、1 指定の対象 (second_object_id) に実行、2 終わり、3 種類ごとの処理
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
