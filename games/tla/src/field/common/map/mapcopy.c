//--------------------------------------------------------------------------
// mapcopy.c
// マップ：セルの矩形の写し
//--------------------------------------------------------------------------

#include "edition.h"
#include "mapcopy.h"

#if GAME_TLA

//--------------------------------------------------------------------------
// セルの矩形を、ブロック番号と属性ごと写す
// srcX, srcY  : 写すもとの左上（セル）
// width, height : 大きさ（セル）
// dst_x, dst_y  : 写す先の左上（セル）
//--------------------------------------------------------------------------
void Map_CopyCellsRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                       s32 dstY) {
  Map_CopyMetatileIndicesRect(srcX, srcY, dstX, dstY, width, height);
  Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
}
#endif
