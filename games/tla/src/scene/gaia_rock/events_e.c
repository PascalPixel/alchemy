#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "map.h"
#include "scene_ids.h"

s32 Func_02000a68(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov680Room2 && (u16)(gPartyState.entrance - 3) <= 1) {
        s32 *layer = (s32 *)&Ram_HeapSlots->map_work->layers[1];

        layer[2] = -0x800000;
        layer[3] = 0x4480000;
    }
    return 0;
}
