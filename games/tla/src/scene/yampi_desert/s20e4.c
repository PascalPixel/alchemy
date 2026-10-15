#include "scene.h"
#include "map.h"

void Func_020020e4(void)
{
    struct FieldActor *leader = Object_GetById(gPartyState.currentOwner);
    struct MapWork *map = Ram_HeapSlots->mapWork;
    s32 index = Func_020041e0(leader);
    struct MapCell *cell = map->layers[0].cells + index;
    struct PartyState *party;
    s32 y;

    if (cell->collisionCode == 20) {
        y = leader->y;
        asm volatile(""); /* FAKEMATCH: ⚓️ reads the height before it builds 0x100000 */
        if (y > 0x100000) {
            gPartyState.unk2c3 = 1;
            return;
        }
    }
    party = &gPartyState;
    asm volatile(""); /* FAKEMATCH: ⚓️ loads the party state address before it builds the field offset */
    party->unk2c3 = 0;
}
