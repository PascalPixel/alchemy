#include "types.h"
#include "dma.h"
#include "io_reg.h"

struct OamEntry {
    u32 y : 8;
    u32 affine : 2;
    u32 mode : 2;
    u32 mosaic : 1;
    u32 colors256 : 1;
    u32 shape : 2;
    u32 x : 9;
    u32 matrix : 5;
    u32 size : 2;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u16 unused;
};

void Title_PlaceLogoPiece(s32 tile, s32 x, u32 y)
{
    struct OamEntry oam;

    Dma_Fill32(0, &oam, sizeof(oam));
    oam.shape = 0;
    oam.size = 3;
    oam.colors256 = 1;
    oam.mode = 1;
    oam.affine = 3;
    oam.tile = tile;
    oam.x = x - 32;
    oam.y = y + 224;
    Dma_Set(&oam, (void *)(0x07000000 + ((u32)tile >> 4)), 0x84000002, REG_DMA3);
}
