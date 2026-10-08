//--------------------------------------------------------------------------
// release_battle_object_records.c
// バトル：オブジェクトの絵の解放
//--------------------------------------------------------------------------

#include "types.h"
#include "motobj.h"
#include "resource.h"

//--------------------------------------------------------------------------
// オブジェクトが持つ絵をすべて解放し、絵なしにもどす
// no : オブジェクト番号
//--------------------------------------------------------------------------
void ReleaseBattleObjectRecords(s32 no)
{
	struct BtlObj		*slot;
	struct MotionObject *obj;
	s32					 i;
	void				*rec;

	slot = BtlObjGet(no);
	if (slot != NULL)
	{
		obj = slot->object;
		if (obj != NULL)
		{
			slot->animation_entry = 0;
			slot->effect_entry = 0;
			i = 0;
			while ((rec = GetMotionRecord(obj, i)) != NULL)
			{
				ResourceObject_ReleaseFar(rec);
				i += 1;
			}
			obj->record_storage_kind = 0; // 絵なし
			obj->records = NULL;
		}
	}
}
