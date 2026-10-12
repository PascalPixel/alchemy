#ifndef ALCHEMY_UNIT_FIELD_COMMON_SCRIPT_FIELD7_H
#define ALCHEMY_UNIT_FIELD_COMMON_SCRIPT_FIELD7_H

#include "types.h"

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);
s32 Map_InitializePerspectiveScene(void);
void Map_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py);
void WorldMap_LoadGraphics(s32 x, s32 z);
void MapAnimation_ApplyAffineFrame(void);
void Map_RenderPaletteMappedRow(u32 value);
void Map_RenderPaletteMappedColumn(u32 value);
void WorldMap_UpdateView(void);
void Map_UpdateCurrentTileBlock(void);
void Map_UpdateCurrentTileBlockUntilBlocked(void);
void MapAnimation_PresentFrame(void);
void MapAnimation_Start(void);
void Map_ShowBg1FromBuffer(void);
void Map_LoadAreaGraphics(void);
void Map_LoadDefaultCellsAndUpdateBlock(void);
void MapAnimation_Update(void);
void Map_ClearLayerEntryFlag(u32 no);
void Map_SetLayerEntryFlag(u32 no);
void MapAnimation_StartChannels(const u16 *script);
void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);
void DisplayBlend_RunScript(void);
void DisplayBlend_StartScript(u16 *script);
void DisplayBlend_EnableRunScript(void);
void DisplayBlend_DisableRunScript(void);

#endif
