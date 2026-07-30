#include "types.h"

struct DmaChannel_080b010c {
    const void *source;
    void *destination;
    u32 control;
};

u8 *Func_080048b0(s32 resource, s32 size);
void Func_0808a460(void);
s8 Func_08077158(s32);
s32 Func_08004080(void);
void Func_08003fa4(s32 index, s32 count, s32 source);
void Func_080041d8(s32 message, s32 code);

void Func_080b010c(void)
{
    u8 *state = Func_080048b0(0x37, 0xa70);
    volatile struct DmaChannel_080b010c *dma =
        (volatile struct DmaChannel_080b010c *)0x040000d4;
    u32 zero = 0;
    s32 allocation;

    Func_0808a460();
    dma->source = &zero;
    dma->destination = state;
    dma->control = 0x8500029c;

    state[0x3a8] = 0x0c;
    state[0x3a7] = Func_08077158((s32)(state + 0x36e));

    allocation = Func_08004080();
    *(u16 *)(state + 0x390) = allocation;
    Func_08003fa4(allocation, 0x80, 0x080b3940);

    allocation = Func_08004080();
    *(u16 *)(state + 0x392) = allocation;
    Func_08003fa4(allocation, 0x80, 0x080b3b40);

    allocation = Func_08004080();
    *(u16 *)(state + 0x394) = allocation;
    Func_08003fa4(allocation, 0x80, 0x080b3bc0);

    allocation = Func_08004080();
    *(u16 *)(state + 0x396) = allocation;
    Func_08003fa4(allocation, 0x80, 0x080b39c0);

    allocation = Func_08004080();
    *(u16 *)(state + 0x39a) = allocation;
    Func_08003fa4(allocation, 0x80, 0x080b3a40);

    allocation = Func_08004080();
    *(u16 *)(state + 0x398) = allocation;
    Func_08003fa4(allocation, 0x80, 0x080b3ac0);

    Func_080041d8(0x080b00f5, 0xc80);
}
