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
#endif
