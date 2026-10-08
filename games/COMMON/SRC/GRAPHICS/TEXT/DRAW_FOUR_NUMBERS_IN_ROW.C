//--------------------------------------------------------------------------
// DRAW_FOUR_NUMBERS_IN_ROW.C
// セーブ：記録のジンの数を属性ごとに横に並べて書く
//--------------------------------------------------------------------------

#include "TEXT_RENDER_RUNTIME.H"
#include "TYPES.H"

// セーブの記録（SaveSummary）のうち、ここで読むところ
struct NumberRow
{
	u8 unknown_00[0x28];
	s8 values[4]; // 属性ごとのジンの数
};

//--------------------------------------------------------------------------
// 4 つの数を 2 けたで、24 ドットおきに y 16 に書く
// win : 書き込み先（0 なら何もしない）
//--------------------------------------------------------------------------
void UiText_DrawFourNumbersInRow(struct RenderInput *win, struct NumberRow *row)
{
	s32 i;

	if (win != 0)
	{
		for (i = 0; i < 4; i++)
			UiText_DrawNumberAtOffset(row->values[i], 2, win, i * 24, 16);
	}
}
