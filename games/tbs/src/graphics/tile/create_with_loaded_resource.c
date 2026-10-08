#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "resource.h"
#include "render_input.h"


s32 UiIcon_CreateWithLoadedResource(struct RenderInput *window, s32 x, s32 y, s32 resource_id)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    if (slot != 0x60) {
        UiIcon_LoadResourceIntoSlot(resource_id, slot);
        RenderOutput_CreateFar(slot, 0x40000000, window, x, y);
    }
}
