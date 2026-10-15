/* Near miss, differ 2: the listing loads the party state's address and then
 * builds the render mode's offset (ldr r3; movs r1, #128; lsls; adds); this
 * draft starts the offset first (movs r1; ldr r3). sched2 gives the offset's
 * first insn priority 7 and the pool load 6 (alchemy diff --rtl); the listing
 * needs the load at 7 or more. Tried: the party state as a pointer local
 * (same), behind a volatile "+l" barrier (the frame and the table load move
 * ahead of the load, differ 4), in a one-pass block (sub sp lands between the
 * load and the offset, differ 2), behind a plain "+l" asm (the load moves
 * after the shift, differ 2). No barrier fits: sub sp must stay after the
 * mode's load. */
#include "types.h"
#include "party_state.h"

struct TerrainResult {
    s16 terrain;
    s16 behavior;
    s16 flag;
    s16 result;
};

extern struct TerrainResult gData_080eedbc[];
extern struct TerrainResult gData_080eef34[];
s32 GetWorldMapTerrainBehaviorFar(void *position, s32 *terrain);
s32 GameFlag_Test(s32 flag);
void Func_080ca514(s32 behavior);

s32 BattleFx_LookupResult(void *position)
{
    struct TerrainResult *entry = gData_080eedbc;
    s32 behavior;
    u32 terrain;
    s32 result;

    if (gPartyState.renderMode == 8)
        return 0;
    if (gPartyState.renderMode == 7)
        entry = gData_080eef34;
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
