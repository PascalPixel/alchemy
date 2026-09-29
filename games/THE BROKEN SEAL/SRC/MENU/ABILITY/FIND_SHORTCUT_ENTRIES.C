#include "TYPES.H"

/* One entry of a collected ability list, in the packed form of a shortcut:
   the owner in the top six bits, the ability in the low ten. */
struct ShortcutListEntry {
    u16 owner;
    u16 ability;
};

/* FAKEMATCH: the first shortcut is read through a volatile field. That keeps
   its read one zero-extended ldrh from the state base plus 0x220; a plain
   read is folded into the address and combined into sign-extending shifts. */
struct ShortcutState {
    u32 unknown_000[0x220 / 4];
    volatile u16 first;
    u16 second;
};

extern struct ShortcutState gGameState;

/* Find the positions of the two Psynergy shortcuts in a 448-entry ability
   list; a shortcut that is not in the list leaves its position at 0. */
void Menu_FindShortcutEntries(u32 *first_index, u32 *second_index,
                              const struct ShortcutListEntry *entries)
{
    s32 i;
    u16 first;

    *first_index = 0;
    *second_index = 0;

    first = gGameState.first;
    for (i = 0; i <= 447; i++) {
        if (entries[i].ability == (first & 0x3ff) &&
            entries[i].owner == (first >> 10)) {
            *first_index = i;
            break;
        }
    }

    for (i = 0; i <= 447; i++) {
        if (entries[i].ability == (gGameState.second & 0x3ff) &&
            entries[i].owner == (gGameState.second >> 10)) {
            *second_index = i;
            break;
        }
    }
}
