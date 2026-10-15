#include "types.h"

extern char ResourceId_WorldMapPaletteA;
extern char ResourceId_DefaultMapAnimation;
extern char ResourceId_DefaultMetatileAttributes;
extern char ResourceId_WorldMapTilesA1;
extern char ResourceId_WorldMapTilesA2;
extern char ResourceId_WorldMapTilesA3;
extern char ResourceId_WorldMapTilesA4;
extern char ResourceId_WorldMapTilesA5;
extern char ResourceId_WorldMapTilesA6;
extern char ResourceId_WorldMapPaletteC;
extern char ResourceId_Data1AD;
extern char ResourceId_Data1AE;
extern char ResourceId_WorldMapTilesC1;
extern char ResourceId_WorldMapTilesC2;
extern char ResourceId_WorldMapTilesC3;
extern char ResourceId_WorldMapTilesC4;
extern char ResourceId_WorldMapTilesC5;
extern char ResourceId_WorldMapTilesC6;
extern char ResourceId_WorldMapPaletteB;
extern char ResourceId_Data1A4;
extern char ResourceId_Data1A5;
extern char ResourceId_WorldMapTilesB1;
extern char ResourceId_WorldMapTilesB2;
extern char ResourceId_WorldMapTilesB3;
extern char ResourceId_WorldMapTilesB4;
extern char ResourceId_WorldMapTilesB5;
extern char ResourceId_WorldMapTilesB6;
extern char ResourceId_WorldMapPaletteD;
extern char ResourceId_Data1B6;
extern char ResourceId_Data1B7;
extern char ResourceId_WorldMapTilesD1;
extern char ResourceId_WorldMapTilesD2;
extern char ResourceId_WorldMapTilesD3;
extern char ResourceId_WorldMapTilesD4;
extern char ResourceId_WorldMapTilesD5;
extern char ResourceId_WorldMapTilesD6;

//--------------------------------------------------------------------------
// World map graphics in four sets, each a palette, two more resources and
// six tile sets, by resource id.
//--------------------------------------------------------------------------

const u32 Data_0802ed34[4][9] =
{
    {
        (u32)&ResourceId_WorldMapPaletteA,
        (u32)&ResourceId_DefaultMapAnimation,
        (u32)&ResourceId_DefaultMetatileAttributes,
        (u32)&ResourceId_WorldMapTilesA1,
        (u32)&ResourceId_WorldMapTilesA2,
        (u32)&ResourceId_WorldMapTilesA3,
        (u32)&ResourceId_WorldMapTilesA4,
        (u32)&ResourceId_WorldMapTilesA5,
        (u32)&ResourceId_WorldMapTilesA6,
    },
    {
        (u32)&ResourceId_WorldMapPaletteC,
        (u32)&ResourceId_Data1AD,
        (u32)&ResourceId_Data1AE,
        (u32)&ResourceId_WorldMapTilesC1,
        (u32)&ResourceId_WorldMapTilesC2,
        (u32)&ResourceId_WorldMapTilesC3,
        (u32)&ResourceId_WorldMapTilesC4,
        (u32)&ResourceId_WorldMapTilesC5,
        (u32)&ResourceId_WorldMapTilesC6,
    },
    {
        (u32)&ResourceId_WorldMapPaletteB,
        (u32)&ResourceId_Data1A4,
        (u32)&ResourceId_Data1A5,
        (u32)&ResourceId_WorldMapTilesB1,
        (u32)&ResourceId_WorldMapTilesB2,
        (u32)&ResourceId_WorldMapTilesB3,
        (u32)&ResourceId_WorldMapTilesB4,
        (u32)&ResourceId_WorldMapTilesB5,
        (u32)&ResourceId_WorldMapTilesB6,
    },
    {
        (u32)&ResourceId_WorldMapPaletteD,
        (u32)&ResourceId_Data1B6,
        (u32)&ResourceId_Data1B7,
        (u32)&ResourceId_WorldMapTilesD1,
        (u32)&ResourceId_WorldMapTilesD2,
        (u32)&ResourceId_WorldMapTilesD3,
        (u32)&ResourceId_WorldMapTilesD4,
        (u32)&ResourceId_WorldMapTilesD5,
        (u32)&ResourceId_WorldMapTilesD6,
    },
};
