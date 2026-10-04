#include "TYPES.H"
#include "PARTY_STATE.H"

/* A scene's row: its overlay's resource id, group, variant and map. */
struct SceneRecord {
    s16 resource_id;
    s8 group;
    s8 variant;
    u16 map_index;
    u16 reserved;
};

extern const struct SceneRecord Field_SceneTable[];
extern u8 gOverlayArea[];

void Resource_LoadCode(u32 index, void *destination);

/* Loads the current scene's overlay code into the overlay area. */
void BattleFx_LoadResourceGroup(void)
{
    const struct SceneRecord *table = Field_SceneTable;
    s16 scene = gPartyState.scene;

    Resource_LoadCode(table[scene].resource_id, gOverlayArea);
}
