/* Near miss, differ 8 of 41: the native adds the cell index into the cell
   pointer (adds r3, r3, r0), reads the leader height before it builds
   0x100000, and reloads the party state address for the clearing store before
   its offset; this draft has each pair the other way round. */
#include "../../../../../games/tla/src/field/overlay_661/scene.h"

struct Cell { u8 a, b, c, d; };

void Func_020020e4(void)
{
    struct FieldActor *leader = Object_GetById(gPartyState.current_owner);
    u8 *map = (u8 *)Ram_HeapSlots->map_work;
    s32 index = Func_020041e0(leader);

    if ((*(struct Cell **)(map + 312))[index].c == 20 && leader->y > 0x100000)
        gPartyState.unknown_2c3 = 1;
    else
        gPartyState.unknown_2c3 = 0;
}
