#include "PROTO.H"
#include "TYPES.H"
#include "PARTY_STATE.H"
#include "FIELD_SCENE.H"

extern u8 gOverlayArea[];


void BattleFx_LoadResourceGroup(void)
{
    const struct SceneRecord *table = Field_SceneTable;
    s16 scene = gPartyState.scene;

    Resource_LoadCode(table[scene].resource_id, gOverlayArea);
}
