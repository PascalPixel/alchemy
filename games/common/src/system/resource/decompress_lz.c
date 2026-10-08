//--------------------------------------------------------------------------
// decompress_lz.c
// リソース：LZ 圧縮の展開
//--------------------------------------------------------------------------

#include "types.h"
#include "scene.h"

//--------------------------------------------------------------------------
// source を destination に展開して destination を返す
// source[0・1] : コマンドの列までのオフセット（0 なら圧縮なしで source + 2 を返す）
// source + 2   : そのまま写すバイトの列
// コマンドの列 : 制御バイトを下位ビットから 1 ビットずつ読む
//   1 : そのまま写すバイトを 1 つ写す
//   0 : 2 バイト（上位・下位）を読む。0 なら終わり。
//       下位 12 ビットが距離、上位 4 ビット + 2 が長さ（4 ビットが 0 なら次のバイト + 18）
//       source + 2 から距離だけ前のところから写す
//--------------------------------------------------------------------------
u8 *Resource_DecompressLz(const u8 *source, u8 *destination)
{
	s32		  token;
	u32		  length;
	u32		  sentinel;
	u32		  control;
	u8		  byte;
	u8		 *start;
	const u8 *base;
	const u8 *command;
	const u8 *copy;
	const u8 *literal;

	token = source[0] | (source[1] << 8);
	literal = source + 2;
	if (token == 0)
		return (u8 *)literal;
	command = source + token;
	control = *command;
	sentinel = 128;
	sentinel <<= 1; // 8 ビット読み終えたしるし
	base = literal;
	start = destination;
	command++;
	control |= sentinel;
	goto dispatch;

back_reference:
	length = token >> 12;
	if (length == 0)
	{
		length = *command + 16;
		command++;
	}
	length += 2;
	copy = base - (token & 0x0fff);
	while (length != 0)
	{
		*destination++ = *copy++;
		length--;
	}

shift:
	control >>= 1;
	if (control == 0)
	{
		control = *command;
		sentinel = 128;
		sentinel <<= 1;
		command++;
		control |= sentinel;
	}

dispatch:
	if ((control & 1) != 0)
	{
		if (control != 1) // 残っているのがしるしだけなら次の制御バイトへ
			*destination++ = *literal++;
		goto shift;
	}

	byte = *command++;
	token = byte << 8;
	token |= *command++;
	if (token != 0)
		goto back_reference;
	return start;
}
