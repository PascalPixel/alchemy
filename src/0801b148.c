typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;

struct Node_0801b148 {
    u32 value0;
    struct Node_0801b148 *next;
    u16 value8;
    u16 active;
    u16 handle;
};

extern u8 *Data_03001e98;

void Func_0801a97c(void);
void Func_08016418(void *value, u32 mode);
void Func_080030f8(u32 value);
void Func_08003f3c(u16 handle);
void Func_0801c21c(void);
void Func_08002dd8(u32 value);

void Func_0801b148(void)
{
    u8 *state = Data_03001e98;
    struct Node_0801b148 *node;

    Func_0801a97c();
    Func_08016418(*(void **)(state + 0x350), 2);
    Func_080030f8(1);
    node = *(struct Node_0801b148 **)(state + 0x348);
    while (node != 0) {
        if (node->active != 0) {
            Func_08003f3c(node->handle);
            node->active = 0;
        }
        node = node->next;
    }
    node = *(struct Node_0801b148 **)(state + 0x34c);
    while (node != 0) {
        if (node->active != 0) {
            Func_08003f3c(node->handle);
            node->active = 0;
        }
        node = node->next;
    }
    Func_0801c21c();
    if (*(s16 *)(state + 18) != 0) {
        Func_08003f3c(*(u16 *)(state + 12));
        if (*(s16 *)(state + 18) != 0) {
            Func_08003f3c(*(u16 *)(state + 64));
        }
    }
    Func_08003f3c(*(u16 *)(state + 0x2e4));
    Func_08002dd8(18);
}
