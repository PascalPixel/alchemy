#include "types.h"

struct GaiaRockInsideNode
{
    u8 unknown_0[2];
    u8 next[4];
    u8 unknown_6[2];
};

struct GaiaRockInsidePath
{
    s32              current;
    struct GaiaRockInsideNode nodes[1];
};

extern u8 Data_020023c4[];

#define GAIA_ROCK_INSIDE_PATH ((struct GaiaRockInsidePath *)(Data_020023c4 + 0x288))

// The direction from the current node whose link is 251 or the next node:
// 0x4000, 0x8000, 0 or 0xc000 for the four links in turn, or -1 if none is.
s32 Func_02004904(void)
{
    struct GaiaRockInsidePath *path = GAIA_ROCK_INSIDE_PATH;
    s32               current = path->current;
    /* FAKEMATCH: the node is pinned to r3 and each link to r2: the native keeps the node pointer in r3 and loads the links into r2, where plainly the two trade places (differ 26; the node pinned alone, 11). */
    register struct GaiaRockInsideNode *node asm("r3") = &path->nodes[current];
    s32                        next = current + 1;
    register u32               link asm("r2");

    link = node->next[0];
    if (link == 251 || link == next)
        return 0x4000;
    link = node->next[1];
    if (link == 251 || link == next)
        return 0x8000;
    link = node->next[2];
    if (link == 251 || link == next)
        return 0;
    link = node->next[3];
    if (link == 251 || link == next)
        return 0xc000;
    return -1;
}
