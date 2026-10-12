/* Near miss, differ 26 of 41, registers only: the native keeps the node
 * pointer in r3 and loads each link byte into r2; the draft uses r2 for the
 * pointer and r3 for the bytes. One id local shared by the four tests is
 * worse (differ 34). */
#include "types.h"

struct Ov681Node {
    u8 unknown_0[2];
    u8 next[4];
    u8 unknown_6[2];
};

struct Ov681Path {
    s32 current;
    struct Ov681Node nodes[1];
};

extern u8 Data_020023c4[];

#define OV681_PATH ((struct Ov681Path *)(Data_020023c4 + 0x288))

s32 Func_02004904(void)
{
    struct Ov681Path *path = OV681_PATH;
    s32 current = path->current;
    struct Ov681Node *node = &path->nodes[current];
    s32 next = current + 1;

    if (node->next[0] == 251 || node->next[0] == next)
        return 0x4000;
    if (node->next[1] == 251 || node->next[1] == next)
        return 0x8000;
    if (node->next[2] == 251 || node->next[2] == next)
        return 0;
    if (node->next[3] == 251 || node->next[3] == next)
        return 0xc000;
    return -1;
}
