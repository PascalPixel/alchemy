#ifndef ALCHEMY_UNIT_FIELD_MAPLAYER_H
#define ALCHEMY_UNIT_FIELD_MAPLAYER_H

#include "types.h"

void Runtime_RemapBytesByTable(u8 *buf, s32 cnt);
void Tilemap_DecodeStagedBuffer(s32 size);
void Graphics_RenumberFillerEntries(void);
void Tilemap_ConvertBuffer(void);
s32 Map_LoadLayeredScene(s32 index);
void Map_ApplyWorkOriginAndSpan(void);
void Map_RenderMetatileRow(u32 a0, s32 a1, s32 a2);
void Map_RenderPaletteMappedBlock(u32 a0, s32 a1, s32 a2);
void Map_UpdateLayerScroll(void);
void Map_SetCameraCenter(s32 x, s32 y);
void Map_CopyMetatileIndicesRect(s32 src_x, s32 src_y, u32 dst_x, u32 dst_y, u32 width, u32 height);
void Map_PlayMetatileCopySequence(u16 *command, s32 destination_x, u32 destination_y);
void Map_CopyMetatileCellsRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y);
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y);
void Map_CopyCellIndexRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y);
void Map_SetWorkFlagBits9To11(u32 v);

#endif
