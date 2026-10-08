//--------------------------------------------------------------------------
// DRAW_NUMBER_RIGHT_ALIGNED_FAR.C
// 文字：数を右そろえで書く（海外版）
//--------------------------------------------------------------------------

#include "EDITION.H"
#if EDITION_INTERNATIONAL
#include "TYPES.H"
#include "FIXED_MATH.H"

void UiText_DrawNumberAtOffsetFar(s32 value, s32 digits, s32 layer, s32 x, s32 y);

//--------------------------------------------------------------------------
// num のけた数を数え、右はしが x になるように 1 けた 8 ドットで書く
// （1 けたの数も 2 けた分の幅をとる）
// win  : 書き込み先のウィンドウ
//--------------------------------------------------------------------------
void UiText_DrawNumberRightAlignedFar(s32 num, s32 win, s32 x, s32 y)
{
	s32 v = num;
	s32 keta = 1;

	while (keta <= 15)
	{
		v = v / 10;
		if (v <= 9)
		{
			break;
		}
		keta++;
	}

	keta++;
	x -= keta * 8;
	UiText_DrawNumberAtOffsetFar(num, keta, win, x, y);
}

#endif
