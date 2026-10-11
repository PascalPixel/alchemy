//--------------------------------------------------------------------------
// mapcopy.c
// マップ：セルの矩形の写し
//--------------------------------------------------------------------------

#include "edition.h"
#include "mapcopy.h"

#if GAME_TLA

//--------------------------------------------------------------------------
// セルの矩形を、ブロック番号と属性ごと写す
// src_x, src_y  : 写すもとの左上（セル）
// width, height : 大きさ（セル）
// dst_x, dst_y  : 写す先の左上（セル）
//--------------------------------------------------------------------------
void Map_CopyCellsRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
	Map_CopyMetatileIndicesRect(src_x, src_y, dst_x, dst_y, width, height);
	Map_CopyCellAttributeRect(src_x, src_y, width, height, dst_x, dst_y);
}
#else
#include "map_scroll.h"
#include "ram_buffer.h"

extern u8 gMapCellBuffer[];

#define MAP_CELLS ((u32 *)gMapCellBuffer)
#define MAP_CELL_TILE_MASK 0x00000fff	   // ブロック番号
#define MAP_CELL_ATTRIBUTE_MASK 0xfffff000 // 属性

// レイヤーの表示範囲の左上（セル）
struct TilePos
{
	s32 x, y;
};

//--------------------------------------------------------------------------
// セル（16×16 ドット）の矩形をそのまま写す
// 写した先が BG レイヤーの表示範囲（16×12 セル）にあれば、
// VRAM の BG マップにもブロックの 2×2 キャラを書く
// src_x, src_y  : 写すもとの左上（セル）
// width, height : 大きさ（セル）
// dst_x, dst_y  : 写す先の左上（セル）
//--------------------------------------------------------------------------
void Map_CopyMetatileCellsRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
	s32					   y = dst_y;
	u32					  *src = (u32 *)Ram_MapCellBuffer + (src_y * 128 + src_x);
	u32					  *dst = &((u32 *)Ram_MapCellBuffer)[y * 128 + dst_x];
	struct MapLayerScroll *layer = ((struct MapScrollWork *)*gMapWork)->layers;
	struct TilePos		   view[3];
	s32					   i;
	struct TilePos		  *pos = view;
	register s32		   x;

	// 3 つのレイヤーの表示範囲（スクロール位置をセル単位に）
	i = 2;
	while (i >= 0)
	{
		register s32 tmp5;

		tmp5 = layer->x >> 20;
		pos->x = tmp5;
		tmp5 = (*layer).y;
		pos->y = tmp5 >> 20;
		pos++;
		layer++;
		i--;
	}
	if (y < dst_y + height)
	{
		do
		{
			for (x = dst_x; x < dst_x + width; x++)
			{
				u32			  val = *src++;
				u32			  cell;
				register u32  offset;
				u32			  tmp3;
				u32			 *dest;
				u32			  n;
				register u32 *tiles;
				u32			  tmp6;
				u32			  tmp12;
				u32			  tmp13;

				dst[0] = val;
				cell = val & 0xfff;
				tmp12 = x & 15;
				tmp6 = tmp12;
				dst += 1;
				tmp3 = y & 15;
				tmp13 = tmp3 << 5;
				pos = view;
				tmp12 = tmp13;
				n = tmp12;
				n += tmp6;
				tmp13 = n * 4;
				i = 0;
				tmp6 = tmp13;
				offset = tmp6;
				// 表示中のレイヤーなら、VRAM の BG マップ（レイヤーごとに 0x800）へ
				for (; i < 3; i += 1)
				{
					if (pos->x <= x && 16 + (*pos).x > x && pos->y <= y && pos->y + 12 > y)
					{
						// ブロック（8 バイト）の上の段の 2 キャラと、下の段（0x40 先）の 2 キャラ
						u32 *tmp;
						u32	 tmp2;
						u32	 tmp4;
						u32 *tmp7;
						u32	 tmp10;
						u32	 tmp14;
						u32	 tmp16;
						/* FAKEMATCH: restoring the unscaled cell keeps loop.c's
						 * shift inside the successful layer branch. The restoration
						 * folds away, preserving the native 36-byte frame and registers. */
						register u32 bytes = cell * 8;
						u32			 tmp8;
						register u32 tmp17;

						cell = bytes / 8;
						tmp = (u32 *)Ram_MapBlocks;
						tmp8 = 0x06002800 + offset;
						tiles = tmp;
						tmp2 = tmp8;
						tmp = (u32 *)tmp2;
						dest = tmp;
						tiles = (u32 *)((u8 *)tiles + bytes);
						tmp16 = *tiles;
						tmp14 = tmp16;
						tmp17 = 0x06002840 + offset;
						tmp10 = tmp14;
						*dest = tmp10;
						tmp7 = (u32 *)(Ram_MapBlocks + 4);
						tiles = tmp7;
						tiles = (u32 *)((u8 *)tiles + bytes);
						tmp8 = tmp17;
						tmp14 = tmp8;
						tmp17 = tiles[0];
						tmp16 = tmp17;
						tmp10 = tmp16;
						tmp4 = tmp10;
						tmp2 = tmp4;
						tmp4 = tmp14;
						tmp7 = (u32 *)tmp4;
						dest = tmp7;
						*dest = tmp2;
						break;
					}
					n = 0x800; // つぎのレイヤーの BG マップ
					offset += n;
					pos++;
				}
			}
			src = src + (128 - width);
			dst = 128 - width + dst;
			y++;
		} while (y < dst_y + height);
	}
}

//--------------------------------------------------------------------------
// セルの矩形の属性だけを写す。写す先のブロック番号はそのまま
// src_x, src_y  : 写すもとの左上（セル）
// width, height : 大きさ（セル）
// dst_x, dst_y  : 写す先の左上（セル）
//--------------------------------------------------------------------------
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
	u32 *dst = MAP_CELLS;
	u32 *src = dst;
	u16	 row;
	u16	 col;

	dst += (dst_y << 7) + dst_x;
	src += (src_y << 7) + src_x;

	for (row = 0; row < height; row++)
	{
		u32 *dst_cell = dst + (row << 7);
		u32 *src_cell = src + (row << 7);

		for (col = 0; col < width; col++)
		{
			*dst_cell = (*dst_cell & MAP_CELL_TILE_MASK) | (*src_cell++ & MAP_CELL_ATTRIBUTE_MASK);
			dst_cell++;
		}
	}
}

//--------------------------------------------------------------------------
// セルの矩形のブロック番号だけを写す。写す先の属性はそのまま
// 表示範囲にあれば VRAM の BG マップも書きかえる
// src_x, src_y  : 写すもとの左上（セル）
// width, height : 大きさ（セル）
// dst_x, dst_y  : 写す先の左上（セル）
//--------------------------------------------------------------------------
void Map_CopyCellIndexRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
	/* FAKEMATCH: needed to match: word/index temporaries preserve register allocation; the plain body allocates differently and uses a 40-byte frame. */
	s32					   y;
	u32					  *src = (u32 *)Ram_MapCellBuffer + (src_y * 128 + src_x);
	u32					  *dst = &((u32 *)Ram_MapCellBuffer)[dst_y * 128 + dst_x];
	struct MapLayerScroll *layer = ((struct MapScrollWork *)*gMapWork)->layers;
	struct TilePos		   view[3];
	s32					   i;
	struct TilePos		  *pos = view;
	register s32		   x;

	// 3 つのレイヤーの表示範囲（スクロール位置をセル単位に）
	i = 2;
	while (i >= 0)
	{
		register s32 tmp5;

		tmp5 = layer->x >> 20;
		pos->x = tmp5;
		tmp5 = (*layer).y;
		pos->y = tmp5 >> 20;
		pos++;
		layer++;
		i--;
	}
	y = dst_y;
	if (y < dst_y + height)
	{
		do
		{
			for (x = dst_x; x < dst_x + width; x++)
			{
				u32			  val = *src++;
				u32			  cell;
				register u32  offset;
				u32			  tmp3;
				u32			  n;
				u32			  tmp6;
				u32			  tmp12;
				u32			  tmp13;

				cell = val & MAP_CELL_TILE_MASK;
				dst[0] = (dst[0] & MAP_CELL_ATTRIBUTE_MASK) | cell;
				tmp12 = x & 15;
				tmp6 = tmp12;
				dst += 1;
				/* FAKEMATCH: the r3 pin on the scaled cell and the consumed-base
				 * barrier are replaced by three measured forms.
				 * 1. i = 0 here, before the offset: i's live length (58 -> 60
				 *    insns, 22 refs) drops its global-allocation priority below
				 *    cell's (15 refs over 30), so cell keeps r5 and i r6 (i = 0
				 *    after pos = view: differ 38, cell in r6).
				 * 2. The scale goes into the x-loop scratch tmp13 inside the layer
				 *    loop: its first reference is outside that loop, so loop.c
				 *    (reg_in_basic_block_p under maybe_never) leaves the shift in
				 *    the conditional block instead of hoisting the invariant
				 *    before the window tests; being block-local and untied, it
				 *    takes r3 like the pinned form (scale into n: differ 52).
				 * 3. The scale comes after the first destination: cell's r5 is
				 *    still live when reload materializes 0x6002800 for the folded
				 *    add, so reload picks r2 instead of its round-robin r5 (scale
				 *    first: differ 54).
				 * The scr/blk variables and the dest = (u8 *)scr / tiles = (u8 *)blk
				 * copies keep the layer loop above loop.c's threshold of 30 real
				 * insns, so the four address constants stay in the loop.
				 * 4. The two word copies read and write through volatile casts: the
				 *    VRAM store is hardware, and the volatile load from the block
				 *    buffer keeps its add apart from the load (a non-volatile load
				 *    combines into ldr [r, r]); these four volatiles replaced two pins
				 *    and two barriers and raised the volatile count (2026-10-11). */
				i = 0;
				tmp3 = y & 15;
				tmp13 = tmp3 << 5;
				pos = view;
				tmp12 = tmp13;
				n = tmp12;
				n += tmp6;
				tmp13 = n * 4;
				tmp6 = tmp13;
				offset = tmp6;
				// 表示中のレイヤーなら、VRAM の BG マップ（レイヤーごとに 0x800）へ
				for (; i < 3; i += 1)
				{
					if (pos->x <= x && 16 + (*pos).x > x && pos->y <= y && pos->y + 12 > y)
					{
						// ブロック（8 バイト）の上の段の 2 キャラと、下の段（0x40 先）の 2 キャラ
						u32 scr, blk;
						u8 *dest, *tiles;

						scr = 0x06002800;
						dest = (u8 *)scr;
						dest += offset;
						tmp13 = cell * 8;
						blk = (u32)Ram_MapBlocks;
						tiles = (u8 *)blk;
						tiles += tmp13;
						*(volatile u32 *)dest = *(volatile u32 *)tiles;

						blk = (u32)Ram_MapBlocks + 4;
						tiles = (u8 *)blk;
						tiles += tmp13;
						scr = 0x06002840;
						dest = (u8 *)scr;
						dest += offset;
						*(volatile u32 *)dest = *(volatile u32 *)tiles;
						break;
					}
					n = 0x800; // つぎのレイヤーの BG マップ
					offset += n;
					pos++;
				}
			}
			src = src + (128 - width);
			dst = 128 - width + dst;
			y++;
		} while (y < dst_y + height);
	}
}
#endif
