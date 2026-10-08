//--------------------------------------------------------------------------
// CREATE_OUTPUT_FROM_RESOURCE_SLOT.C
// 表示：わざのアイコンをスプライトとしてウィンドウに置く
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
#include "RENDER_INPUT.H"
#include "TYPES.H"
#include "RESOURCE.H"
#include "VRAM_TAB.H"

//--------------------------------------------------------------------------
// あいている VRAM キャッシュにアイコンを読み、16x16 のスプライトで出す
// （あきがなければ何もしない）
// input : 置く先のウィンドウ
// x, y  : ウィンドウの中の位置（ドット）
// abil  : わざの番号
//--------------------------------------------------------------------------
s32 Ui_CreateOutputFromResourceSlot(struct RenderInput *input, s32 x, s32 y, s32 abil)
{
	s32 slot;

	slot = Resource_FindFreeEntry();
	if (slot != VRAM_CACHE_ENTRY_COUNT)
	{
		Resource_LoadIndexedEntryToBuffer(abil, slot);
		RenderOutput_Create(slot, 0x40000000, input, x, y); // OBJ サイズ 16x16
	}
}
