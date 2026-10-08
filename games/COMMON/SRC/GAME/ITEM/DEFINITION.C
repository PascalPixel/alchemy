//--------------------------------------------------------------------------
// DEFINITION.C
// アイテム：定義データの参照と装備できるかの判定
//--------------------------------------------------------------------------

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "ITEM.H"
#include "EDITION.H"

//--------------------------------------------------------------------------
// アイテムの定義データを返す
// item : アイテム番号（持ち物の値なら上位の装備・個数ビットを無視する）
//--------------------------------------------------------------------------
struct ItemDefinition *Item_GetDirect(s32 item)
{
	return Item_DefinitionTable + (item & ITEM_ID_MASK);
}
#include "OWNER_STATE.H"
#include "BATTLE_UNIT.H"

//--------------------------------------------------------------------------
// キャラクターがアイテムを装備できるか
// equip_mask の class_id 番目のビットで決まり、class_id が 8 以上なら 0
//--------------------------------------------------------------------------
s32 Item_CanOwnerEquipDirect(s32 unit, s32 item)
{
#if GAME_TLA
	struct OwnerInventoryState *st = UnitGet(unit);
#else
	struct BtlUnit *st = UnitGet(unit);
#endif
	struct ItemDefinition *def = Item_GetDirect(item);
	s32					   mask = def->equip_mask;

	if (st->class_id >= 8)
	{
		return 0;
	}
	return (mask >> st->class_id) & 1;
}

//--------------------------------------------------------------------------
// 装備品でなければ 1、装備品ならキャラクターが装備できるかを返す
//--------------------------------------------------------------------------
s32 Item_IsCompatibleWithOwner(s32 unit, s32 item)
{
	if (Item_GetEquipmentGroup(item) == 0)
	{
		return 1;
	}
	return Item_CanOwnerEquipDirect(unit, item);
}
