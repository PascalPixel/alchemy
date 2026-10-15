//--------------------------------------------------------------------------
// create_output_from_resource_slot.c
// 表示：わざのアイコンをスプライトとしてウィンドウに置く
//--------------------------------------------------------------------------

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "render_input.h"
#include "types.h"
#include "resource.h"
#include "vram_tab.h"

//--------------------------------------------------------------------------
// あいている VRAM キャッシュにアイコンを読み、16x16 のスプライトで出す
// （あきがなければ何もしない）
// input : 置く先のウィンドウ
// x, y  : ウィンドウの中の位置（ドット）
// ability  : わざの番号
//--------------------------------------------------------------------------
s32 Ui_CreateOutputFromResourceSlot(struct RenderInput *input, s32 x, s32 y, s32 ability)
{
	s32 slot;

	slot = Resource_FindFreeEntry();
	if (slot != VRAM_CACHE_ENTRY_COUNT)
	{
		Resource_LoadIndexedEntryToBuffer(ability, slot);
		RenderOutput_Create(slot, 0x40000000, input, x, y); // OBJ サイズ 16x16
	}
}
