//--------------------------------------------------------------------------
// COPY_LIST_ORDER.C
// メニュー：並べかえの順番の表を写す
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
#include "TYPES.H"

//--------------------------------------------------------------------------
// mode 0〜2 の順番の表（それ以外は標準の表）を order に写す
// 表は 0xff で終わる。写すのは 0xff を含めて 32 バイトまで
//--------------------------------------------------------------------------
void Menu_CopyListOrder(s32 mode, u8 *order)
{
	u8 *src;
	s32 n;

	src = Menu_ListOrderDefault;
	switch (mode)
	{
	case 0:
		src = Menu_ListOrderMode0;
		break;
	case 1:
		src = Menu_ListOrderMode1;
		break;
	case 2:
		src = Menu_ListOrderMode2;
		break;
	}
	*order = *src;
	n = 0;
	if (*order != 0xff)
	{
		do
		{
			if (++n > 31)
				break;
			src++;
			order++;
			*order = *src;
		} while (*order != 0xff);
	}
}
