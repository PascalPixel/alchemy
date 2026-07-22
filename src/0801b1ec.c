typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct Node_0801b1ec {
    u8 filler0[4];
    struct Node_0801b1ec *next;
    u8 filler8[8];
    u16 first1;
    u16 second1;
    u8 filler14[4];
    u16 first2;
    u16 second2;
};

struct State_0801b1ec {
    u8 filler0[0x348];
    struct Node_0801b1ec *head;
    u8 filler34c[0x4a];
    u16 first;
    u16 second;
};

extern struct State_0801b1ec *Data_03001e98;

void Func_0801b1ec(u32 first, u32 second)
{
    struct State_0801b1ec *state = Data_03001e98;
    struct Node_0801b1ec *node;

    state->first = first;
    state->second = second;
    node = state->head;
    while (node != 0) {
        node->first1 = first;
        node->first2 = first;
        node->second1 = second;
        node->second2 = second;
        node = node->next;
        first += 16;
    }
}
