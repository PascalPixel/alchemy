/* Pin-free Map_CopyCellIndexRect trial: 142 native and 142 built
 * instructions, 140 common, differ 4. Reusing bytes for the second
 * tile word reduces the plain unbound trial from differ 72 (weighted
 * 595) to differ 24 (weighted 300). Moving the initial scale into the
 * base-address block still differs 72. Reusing the dead offset for the
 * second word then reduces it to differ 4 (weighted 10): its last load
 * and store use r4 instead of r3. Other dead temporaries, signed types
 * and dropping register qualifiers do not close those two operands.
 * Reusing cell at the final store
 * instead scores weighted 195 but differs 50; neither is adopted.
 * The linked function retains its r3 binding until the whole ROM matches. */
#include "edition.h"
#include "mapcopy.h"
#include "map_scroll.h"
#include "ram_buffer.h"

#define MAP_CELL_TILE_MASK 0x00000fff
#define MAP_CELL_ATTRIBUTE_MASK 0xfffff000

struct TilePos
{
	s32 x, y;
};

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
				volatile u32 *dest;
				u32			  n;
				register u32 *tiles;
				u32			  tmp6;
				u32			  tmp12;
				u32			  tmp13;

				cell = val & MAP_CELL_TILE_MASK;
				dst[0] = (dst[0] & MAP_CELL_ATTRIBUTE_MASK) | cell;
				tmp12 = x & 15;
				tmp6 = tmp12;
				dst += 1;
				tmp3 = y & 15;
				tmp13 = tmp3 << 5;
				pos = view;
				i = 0;
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
						u32 *tmp;
						u32	 tmp2;
						u32	 tmp4;
						u32 *tmp7;
						u32	 tmp10;
						u32	 tmp14;
						u32	 tmp18;
						u32	 tmp21;
						// Pin-free byte offset; allocation still differs from the native.
						u32 bytes = cell * 8;
						u32			 tmp16;

						{
							tmp2 = 0x06002800;
							/* FAKEMATCH: the consumed-base barrier preserves two register choices and one load order without changing instruction count or frame. */
							__asm__ __volatile__("" : : "l"(tmp2));
							tmp = (u32 *)Ram_MapBlocks;
							tiles = tmp;
							tmp16 = offset + tmp2;
						}
						tmp21 = tmp16;
						tmp18 = tmp21;
						tmp2 = tmp18;
						tmp = (u32 *)tmp2;
						dest = tmp;
						tiles = (u32 *)((u8 *)tiles + bytes);
						tmp14 = *tiles;
						tmp10 = tmp14;
						*dest = tmp10;
						tmp7 = (u32 *)(Ram_MapBlocks + 4);
						tiles = tmp7;
						tiles = (u32 *)((u8 *)tiles + bytes);
						tmp16 = 0x06002840 + offset;
						tmp18 = tmp16;
						tmp14 = tmp18;
						/* FAKEMATCH: reusing both consumed offsets for the
						 * second word changes allocation, leaving differ 4. */
						bytes = (offset = *tiles);
						tmp21 = bytes;
						tmp10 = tmp21;
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
