#include "unit/field/common/fielddat.h"
#include "types.h"

// 十字キー入力ごとの向き (0xffff = 入力なし)
const u16 WorldMap_KeyHeadings[16] =
{
	0xffff, 0x0000, 0x8000, 0xffff, 0xc000, 0xe000, 0xa000, 0xc000,
	0x4000, 0x2000, 0x6000, 0x4000, 0xffff, 0x0000, 0x8000, 0xffff,
};

// 足跡のスクリプト
const s32 WorldMap_FootprintActions[6] =
{
	0, 120, 27,
	0, 30, 27,
};

// 十字キー入力ごとの旋回量
const s32 WorldMap_KeyYaws[16] =
{
	0, -512, 512, 0,
	0, -384, 384, 0,
	0, -384, 384, 0,
	0, 0, 0, 0,
};
