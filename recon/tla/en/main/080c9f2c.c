#include "types.h"
#include "party_state.h"

struct TerrainResult {
    s16 terrain;
    s16 behavior;
    s16 flag;
    s16 result;
};

extern struct TerrainResult Data_080eedbc[];
extern struct TerrainResult Data_080eef34[];
s32 GetWorldMapTerrainBehaviorFar(void *position, s32 *terrain);
s32 GameFlag_Test(s32 flag);
void Func_080ca514(s32 behavior);

s32 BattleFx_LookupResult(void *position)
{
    struct TerrainResult *entry = Data_080eedbc;
    s32 behavior;
    u32 terrain;
    s32 result;

    if (gPartyState.render_mode == 8)
        return 0;
    if (gPartyState.render_mode == 7)
        entry = Data_080eef34;
    behavior = GetWorldMapTerrainBehaviorFar(position, (s32 *)&terrain);
    terrain %= 20;
    result = 0;
    for (; entry->terrain != -1; entry++) {
        if (entry->terrain != terrain)
            continue;
        if (entry->behavior != -1 && entry->behavior != behavior)
            continue;
        if (entry->flag != -1 && GameFlag_Test(entry->flag) == 0)
            continue;
        result = entry->result;
        break;
    }
    Func_080ca514(behavior);
    return result;
}
