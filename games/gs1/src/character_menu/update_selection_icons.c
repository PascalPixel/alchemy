#include "types.h"
#include "scene.h"
#include "abi/character_menu/update_selection_icons.h"

struct Entry_080a9dc4 {
    u8 padding[14];
    u8 value;
};

struct State_080a9dc4 {
    u8 padding[200];
    struct Entry_080a9dc4 *entries[5];
};

extern struct State_080a9dc4 *gIw;

s32 CharacterMenu_UpdateSelectionIcons(const u8 *enabled)
{
    struct State_080a9dc4 *state = gIw;
    s32 index = 0;

    do {
        if (enabled[index] != 0) {
            s32 kind;
            switch (index) {
            case 0: kind = 16; break;
            case 1: kind = 1; break;
            case 2: kind = 2; break;
            case 3: kind = 15; break;
            case 4: kind = 7; break;
            default: kind = 0; break;
            }
            Sys_SetMode(8, kind, state->entries[index]->value, 0);
        }
        index++;
    } while (index <= 4);
    return 1;
}
