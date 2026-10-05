#include "TYPES.H"
#include "PARTY_STATE.H"
#include "FIELD_SCENE.H"

extern u8 gOverlayArea[];

void Resource_LoadCode(u32 index, void *destination);

/* Loads the current scene's overlay code into the overlay area. */
void BattleFx_LoadResourceGroup(void)
{
    const struct SceneRecord *table = Field_SceneTable;
    s16 scene = gPartyState.scene;

    Resource_LoadCode(table[scene].resource_id, gOverlayArea);
}
