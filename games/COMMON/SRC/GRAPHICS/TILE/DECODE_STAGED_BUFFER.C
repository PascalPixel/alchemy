//--------------------------------------------------------------------------
// DECODE_STAGED_BUFFER.C
// マップ：展開したブロックのデータを gMapBlocks にほどく
//--------------------------------------------------------------------------

#include "TYPES.H"
extern u32 gMapBlocks[];
#include "RAM_BUFFER.H"
extern u8 gMapCellBuffer[];

//--------------------------------------------------------------------------
// 展開バッファの [1] が方式、[2] からが 16 ビットのデータ
// size : 方式のバイトを含む長さ（偶数ならデータなし）
//   方式 0 : そのまま写す
//   方式 1 : 上位バイトの列のあとに下位バイトの列。前の値と XOR して戻す
//   方式 2 : 前の値と XOR して戻す
//--------------------------------------------------------------------------
void Tilemap_DecodeStagedBuffer(s32 size)
{
	u16 *dst;
	u16 *src;
	s32	 n;
	s32	 i;

	src = (u16 *)(Ram_MapCellBuffer + 2);
	dst = (u16 *)gMapBlocks;
	n = (size - 1) / 2;
	if (size & 1)
	{
		switch (((u8 *)gMapCellBuffer)[1])
		{
		case 0:
			for (i = 0; i < n; i++)
				*dst++ = *src++;
			break;
		case 1:
		{
			u32 prev = 0;
			u8 *hi = Ram_MapCellBuffer + 2;
			u8 *lo = hi + n;

			for (i = 0; i < n; i++)
			{
				u32 v = ((*hi++ << 8) | *lo++) ^ prev;
				*dst++ = v;
				prev = v;
			}
			break;
		}
		case 2:
		{
			s32 prev = 0;

			for (i = 0; i < n; i++)
			{
				s32 v = *src++ ^ prev;
				*dst++ = v;
				prev = v;
			}
			break;
		}
		}
	}
}
