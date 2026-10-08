//--------------------------------------------------------------------------
// PATTERN.C
// バトル効果：十字キーの入力を向きに変える
//--------------------------------------------------------------------------

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"

extern u32 gKeysHeld;

//--------------------------------------------------------------------------
// 押している十字キーの向き（0x10000 で 1 周）を返す
// 右 0、下 0x4000、左 0x8000、上 0xc000
// 何も押していないとき、斜めのときは 0xffff
//--------------------------------------------------------------------------
u16 BattleFx_GetCycledTableWord(void)
{
	return BattleFx_CyclePatternWords[(gKeysHeld >> 4) & 15]; // 右・左・上・下の 4 ビット
}
