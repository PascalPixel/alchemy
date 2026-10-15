#include "types.h"

extern u8 gMenuWork[];

struct CharacterSelectorState {
    u8 padding000[0x24];
    s32 screenHandle;
    u8 padding028[0x0e4];
    s32 selectorWindow;
    u8 padding110[0x34];
    u16 rowPositions[8];
    u8 padding154[0x0b4];
    u16 characterIds[8];
    u8 padding218;
    u8 characterCount;
    u8 padding21a[6];
    u16 flags;
};

s32 Party_AddActiveOwnerFar(s32 character_id);
s32 Party_ListActiveOwnersFar(const u16 *characterIds);
s32 Party_RemoveActiveOwnerFar(s32 character_id);

s32 CharacterSelector_MoveEntry(s32 selectedIndex, s32 direction)
{
    struct CharacterSelectorState *state =
        *(struct CharacterSelectorState **)gMenuWork;
    u32 reordered[14];
    s32 index;

    if (state->characterCount <= 1) {
        return 0;
    }
    if (direction == 1) {
        if (selectedIndex == state->characterCount - 1) {
            return 0;
        }
    } else {
        if (selectedIndex == 0) {
            return 0;
        }
    }

    {
        u32 zero = 0;
        u32 *clear = &reordered[13];

        do {
            *clear = zero;
            clear--;
        } while ((s32)clear >= (s32)reordered);
    }
    for (index = 0; index < state->characterCount; index++) {
        reordered[index] = state->characterIds[index];
    }

    if (direction == 1) {
        index = reordered[selectedIndex];
        reordered[selectedIndex] = reordered[selectedIndex + 1];
        reordered[selectedIndex + 1] = index;
    } else {
        index = reordered[selectedIndex];
        reordered[selectedIndex] = reordered[selectedIndex - 1];
        reordered[selectedIndex - 1] = index;
    }

    for (index = 0; index < state->characterCount; index++) {
        Party_RemoveActiveOwnerFar(state->characterIds[index]);
    }
    for (index = 0; index < state->characterCount; index++) {
        Party_AddActiveOwnerFar(reordered[index]);
    }
    state->characterCount = Party_ListActiveOwnersFar(state->characterIds);
    return 1;
}
