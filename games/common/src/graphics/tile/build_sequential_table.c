//--------------------------------------------------------------------------
// build_sequential_table.c
// タイル：1 バイトずつ連番のマップを作る
//--------------------------------------------------------------------------

#include "types.h"
#include "iwram_call.h"
#include "scene.h"

//--------------------------------------------------------------------------
// 0〜255 と 0〜223 の連番のバイトを続けて書き、
// 残りの 544 バイトを 0xff で埋める（あわせて 1024 バイト）
//--------------------------------------------------------------------------
void Graphics_BuildSequentialTileTable(s32 *dst)
{
	s32 v;
	s32 n;
	s32 (*fill)(void *, s32, u32);

	// 4 バイトずつ書くので 1 回に 4 つ進める
	v = 0x03020100;
	n = 0;
	do
	{
		n += 1;
		*dst++ = v;
		v += 0x04040404;
	} while (n >= 0 && n <= 63);

	v = 0x03020100;
	n = 0;
	do
	{
		n += 1;
		*dst++ = v;
		v += 0x04040404;
	} while (n >= 0 && n <= 55);

	/* FAKEMATCH: the routine is called through a local pointer to its fixed entry; a plain call matches The Broken Seal through its IWRAM call layer but not The Lost Age, whose header calls the entry directly (all six of its editions differ). */
	fill = Iwram_FillWordsEntry;
	fill(dst, 544, -1);
}
