#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "party_state.h"
#include "field_scene.h"

extern u8 gOverlayArea[];


void BattleFx_LoadResourceGroup(void)
{
    const struct SceneRecord *table = Field_SceneTable;
    s16 scene = gPartyState.scene;

    Resource_LoadCode(table[scene].resourceId, gOverlayArea);
}
