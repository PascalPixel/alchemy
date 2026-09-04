#include "types.h"

#define Menu_EnableAllItemObjects Func_080ad69c

/*
 * This owner's view of Data_03001f2c. 62 owners reach that global and declare
 * it eight different ways; this one needs an item table and a count, and claims
 * nothing about the rest.
 */
struct Work_080ad69c {
    u8 unknown_000[276];
    void *items[64];
    u8 unknown_214[5];
    u8 count;
};

extern struct Work_080ad69c *Data_03001f2c;

void Object_InitializeMode(void *item, s32 enabled);

void Menu_EnableAllItemObjects(void)
{
    struct Work_080ad69c *state = Data_03001f2c;
    s32 index;

    for (index = 0; index < state->count; ++index) {
        Object_InitializeMode(state->items[index], 1);
    }
}
