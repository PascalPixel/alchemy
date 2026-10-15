//--------------------------------------------------------------------------
// classify_use_ability.c
// アイテム：召喚えらびのメニューと、戦闘でアイテムを使えるかの判定
//--------------------------------------------------------------------------

#include "btlsys.h"
#include "item.h"
#include "types.h"
#include "scene.h"
#include "edition.h"
#include "graphics.h"
#include "uiwindow.h"

#define BATTLE_OK 0x80 // tgt_flg：戦闘で使える

//--------------------------------------------------------------------------
// 戦闘でアイテムを使えるか
// 戻り値 : 0 使える / 1 使えない / 2 戦闘では使えない
// unit   : キャラクター番号
// item   : 持ち物の値
//--------------------------------------------------------------------------
s32 Item_ClassifyUseAbility(s32 unit, s32 item)
{
	s32					   ret;
	struct ItemDefinition *def;

	if (item == 0)
	{
		return 1;
	}
#if GAME_TLA
	def = Item_Get(item);
#endif
	ret = 1;
	if (def->use_type == 3) // エナジーを授ける装備
	{
		return ret;
	}
	if (def->action_id == 0)
	{
		return ret;
	}
	if ((def->type != 0) && (Item_CanOwnerEquip(unit, item) == 0))
	{
		return ret;
	}
	if ((BATTLE_OK & BtlActGet(def->action_id)->tgt_flg) == 0)
	{
		return 2;
	}
	return 0;
}
