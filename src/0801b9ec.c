typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct Node_0801b9ec {
    u8 filler0[4];
    struct Node_0801b9ec *next;
    u8 filler8[2];
    u16 type;
    u16 value;
    u8 filler14[18];
    u16 base;
};

struct State_0801b9ec {
    u8 filler0[0x348];
    struct Node_0801b9ec *head;
};

extern u8 Data_0000001f;
void Func_08019ee4(u32, u32, u32 *, u32 *, u32);
void Func_0801c188(void);

void Func_0801b9ec(struct State_0801b9ec *state, u32 index)
{
    struct Node_0801b9ec *node = state->head;
    u32 output;
    u32 value;

    while (index != 0) {
        index--;
        node = node->next;
    }
    if (node->type == 1 || node->type == 6) {
        u32 first = node->base - (u32)&Data_0000001f;

        value = node->value;
        Func_08019ee4(first, 0, &value, &output, 1);
        Func_0801c188();
    }
}
