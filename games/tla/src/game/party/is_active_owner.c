#include "types.h"
#include "party_state.h"

extern char ResourceId_GrasslandBBackdrop;
extern char ResourceId_RockyBeachBackdrop;
extern char ResourceId_DenseForestBackdrop;
extern char ResourceId_ShipDeckBackdrop;
extern char ResourceId_SnowfieldBBackdrop;
extern char ResourceId_BlizzardBackdrop;
extern char ResourceId_DesertBackdrop;

s32 Party_CountActiveOwnersFar(void);

// Chooses the battle backdrop for a terrain; the ship deck's is the default.
void Func_080ca514(s32 terrain)
{
    u16 value;

    switch (terrain - 1)
    {
    case 0:
        value = (u32)&ResourceId_GrasslandBBackdrop;
        break;
    case 1:
        value = (u32)&ResourceId_DenseForestBackdrop;
        break;
    case 2:
        value = (u32)&ResourceId_DesertBackdrop;
        break;
    case 3:
    case 6:
        value = (u32)&ResourceId_RockyBeachBackdrop;
        break;
    case 4:
    case 5:
        value = (u32)&ResourceId_SnowfieldBBackdrop;
        break;
    case 10:
    case 11:
        value = (u32)&ResourceId_BlizzardBackdrop;
        break;
    default:
        value = (u32)&ResourceId_ShipDeckBackdrop;
        break;
    }
    gPartyState.special = value;
}

s32 Func_080ca5a0(s32 owner)
{
    s32 count = Party_CountActiveOwnersFar();
    s32 i;

    if (count > 4)
        count = 4;
    for (i = 0; i < count; i++)
    {
        if (gPartyState.activeOwners[i] == owner)
            return 1;
    }
    return 0;
}
