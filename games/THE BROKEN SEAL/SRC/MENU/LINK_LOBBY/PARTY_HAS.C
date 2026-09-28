#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Party_CountActiveOwners(void);


s32 LinkLobby_PartyContains(s32 id)
{
    s32 count;
    s32 max;
    s32 i;

    count = Party_CountActiveOwners();
    max = 3;
    if (Engine_GameFlagIsSet(0x172) == 0) {
        max = 4;
    }
    if (count > max) {
        count = max;
    }
    for (i = 0; i < count; i++) {
        if (gGameState.unknown_1f8[i] == 0xff) {
            return 0;
        }
        if (gGameState.unknown_1f8[i] == id) {
            return 1;
        }
    }
    return 0;
}
