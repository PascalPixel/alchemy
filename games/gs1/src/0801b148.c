#include "types.h"

struct Node_0801b148 {
    u32 value0;
    struct Node_0801b148 *next;
    u16 value8;
    u16 active;
    u16 handle;
};

struct Work;

extern u8 *Data_03001e98;

void Func_0801a97c(void);
void UiWork_Finalize(struct Work *work, s32 release);
void WaitFrames(u32 value);
s32 Resource_ResetEntry(u32 index);
void Func_0801c21c(void);
void Func_08002dd8(u32 value);

void Func_0801b148(void)
{
    u8 *state = Data_03001e98;
    struct Node_0801b148 *node;

    Func_0801a97c();
    UiWork_Finalize(*(struct Work **)(state + 0x350), 2);
    WaitFrames(1);
    node = *(struct Node_0801b148 **)(state + 0x348);
    while (node != 0) {
        if (node->active != 0) {
            Resource_ResetEntry(node->handle);
            node->active = 0;
        }
        node = node->next;
    }
    node = *(struct Node_0801b148 **)(state + 0x34c);
    while (node != 0) {
        if (node->active != 0) {
            Resource_ResetEntry(node->handle);
            node->active = 0;
        }
        node = node->next;
    }
    Func_0801c21c();
    if (*(s16 *)(state + 18) != 0) {
        Resource_ResetEntry(*(u16 *)(state + 12));
        if (*(s16 *)(state + 18) != 0) {
            Resource_ResetEntry(*(u16 *)(state + 64));
        }
    }
    Resource_ResetEntry(*(u16 *)(state + 0x2e4));
    Func_08002dd8(18);
}
