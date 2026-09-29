#include "TYPES.H"
#include "RESOURCE_IDS.H"

extern u16 gGameState[];

/* Picks the next battle's backdrop for a kind of terrain: grassland, forest,
   desert, beach or snowfield, else the Sol Sanctum's. Halfword 235 of the
   game state is the backdrop, which battle setup hands to
   BattleBackground_LoadFar. */
void BattleFx_SelectResultPointer(s32 arg0)
{
    u16 value;

    switch (arg0 - 1) {
    case 0:
        value = (u16)(u32)&ResourceId_GrasslandBackdrop;
        break;
    case 1:
        value = (u16)(u32)&ResourceId_ForestBackdrop;
        break;
    case 2:
        value = (u16)(u32)&ResourceId_DesertBackdrop;
        break;
    case 3:
    case 6:
        value = (u16)(u32)&ResourceId_BeachBackdrop;
        break;
    case 4:
    case 5:
        value = (u16)(u32)&ResourceId_SnowfieldBackdrop;
        break;
    default:
        value = (u16)(u32)&ResourceId_SoruShindenBackdrop;
        break;
    }
    gGameState[235] = value;
}
