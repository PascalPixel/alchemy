#include "types.h"

struct State_0801b9a8;
struct State_0801b9ec;

void Func_0801b9a8(struct State_0801b9a8 *state, u32 index);
void Func_0801b9ec(struct State_0801b9ec *state, u32 index);
void Func_0801ba68(void *state, u32 mode);
void Func_0801b010(u16 type, u32 value);
void Func_080030f8(u32 value);

void Func_0801b4ec(u8 *state)
{
    u16 *selection = (u16 *)(state + 0x39c);
    u16 *index = (u16 *)(state + 0x39e);
    u16 *count;
    u32 end = *selection + *index + 1;

    count = (u16 *)(state + 0x394);
    if (end != *count) {
        Func_0801b9a8(state, *index);
        {
            u16 *status = (u16 *)(state + 0x3a2);
            u32 value = 33;

            *status = value;
        }
        Func_080030f8(1);
        *index += 1;
        if (*index == 4 && end + 1 < *count) {
            *index += 0xffff;
            {
                u32 value = 8;

                *(u16 *)(state + 60) = value;
            }
            *selection += 1;
            Func_0801ba68(state, 1);
            if (*selection + *index + 2 == *count) {
                u32 value = 0;

                *(u16 *)(state + 62) = value;
            }
            {
                u32 value = 1;

                *(u16 *)(state + 10) = value;
            }
        }
        {
            u16 *status = (u16 *)(state + 0x3a2);
            u32 value = 1;

            *status = value;
            Func_0801b9ec(state, *(u16 *)(state + 0x39e));
        }
        Func_080030f8(1);
        Func_0801b010(*(u16 *)(*(u8 **)(state + 0x348) + 10), 0);
        Func_080030f8(1);
    }
}
