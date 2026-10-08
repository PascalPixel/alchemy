//--------------------------------------------------------------------------
// POSITION_OWNER_ENTRY.C
// アイテムメニュー：持ち物のアイコンを格子の位置に置く
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
#include "M7_INTERFACES.H"

//--------------------------------------------------------------------------
// i 番目（15 より大きければ 0）を、横 cols 個の格子（1 つ 24x16 ドット）の
// 位置に置く
// x0, y0 : 格子の左上
//--------------------------------------------------------------------------
void ItemMenu_PosOwner(struct RenderOutput **slot, s32 i, s32 x0, s32 y0, s32 cols)
{
	struct RenderOutput *obj;

	if (i > 15)
		i = 0;
	obj = *slot;
	obj->y = i / cols * 16 + y0;
	obj->x = i % cols * 24 + x0;
	UiIcon_PrepareObject(obj);
}
