//--------------------------------------------------------------------------
// TABLE_RUN_IF_ACTIVE.C
// フィールド：話し手の表示の後しまつ
//--------------------------------------------------------------------------

#include "TYPES.H"

s32 UiWork_FinalizeEntityMatchingLocalizedIdFar();
s32 ObjectTable_ReadActiveValue();

//--------------------------------------------------------------------------
// アクターに絵があれば、その絵番号を話し手としてウィンドウの表示を終える
// id : アクター番号
//--------------------------------------------------------------------------
void ObjectTable_RunIfActive(s32 id)
{
	s32 no;

	no = ObjectTable_ReadActiveValue(id);
	if (no != -1)
		UiWork_FinalizeEntityMatchingLocalizedIdFar(no);
}
