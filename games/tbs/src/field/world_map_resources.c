#include "field.h"
#include "resource_ids.h"
#include "types.h"

// World map graphics: a palette and five tile sets for each of its two looks.

// Each set is a palette and five tile sets; the loader takes the second set
// when the cell it loads at has terrain kind 21. Not const: the loader reads
// it through a u32 *.
u32 gWorldMap_GraphicsResources[2][6] = {
    {
        (u32)&ResourceId_WorldMapPaletteA,
        (u32)&ResourceId_WorldMapTilesA,
        (u32)&ResourceId_WorldMapTilesB,
        (u32)&ResourceId_WorldMapTilesC,
        (u32)&ResourceId_WorldMapTilesD,
        (u32)&ResourceId_WorldMapTilesE,
    },
    {
        (u32)&ResourceId_WorldMapPaletteB,
        (u32)&ResourceId_WorldMapTilesF,
        (u32)&ResourceId_WorldMapTilesG,
        (u32)&ResourceId_WorldMapTilesH,
        (u32)&ResourceId_WorldMapTilesI,
        (u32)&ResourceId_WorldMapTilesJ,
    },
};
