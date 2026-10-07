#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RUNTIME_MEM.H"
#include "GLYPH.H"
#include "RESOURCE.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "SELECT.H"

extern u8 Tile_Decompress4bppCode[];
extern u8 Tile_ExpandMaskedCode[];
extern u8 Tile_ExpandOpaqueCode[];
extern u8 Tile_Decompress4bppCodeSize[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern u8 Tile_ExpandOpaqueCodeSize[];

void UiWork_ReservedNoOp(void)
{
}
