typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct Node {
    u8 unknown[4];
    struct Node *next;
};

struct State {
    u8 unknown_000[840];
    struct Node *node;
    u8 unknown_34c[82];
    u16 count;
};

struct Node *Func_0801b36c(struct State *state)
{
    struct Node *node = state->node;
    s32 index;

    for (index = 0; index != state->count; ++index) {
        node = node->next;
    }
    return node;
}
