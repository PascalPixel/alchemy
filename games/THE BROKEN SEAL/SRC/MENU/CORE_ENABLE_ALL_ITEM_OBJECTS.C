#include "TYPES.H"
#include "SCENE.H"

/*
 * This owner's view of gMenuWork. 62 owners reach that global and declare
 * it eight different ways; this one needs an item table and a count, and claims
 * nothing about the rest.
 */
struct Work_080ad69c {
    u8 unknown_000[276];
    void *items[64];
    u8 unknown_214[5];
    u8 count;
};

extern struct Work_080ad69c *gMenuWork;

void AnimationObjects_SelectAnimationFar(void *item, s32 enabled);

void Menu_EnableAllItemObjects(void)
{
    struct Work_080ad69c *state = gMenuWork;
    s32 index;

    for (index = 0; index < state->count; ++index) {
        AnimationObjects_SelectAnimationFar(state->items[index], 1);
    }
}
