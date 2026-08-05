#include "types.h"

struct DmaTransfer {
    u32 source;
    u32 destination;
    u32 control;
};

extern void Func_0808a330(s32 value, s32 mode);
extern void Func_02001878(void);

/* Load the selected saved palette, fade it in, then refresh the work buffer. */
void Func_020018b8(s32 useAlternate)
{
    volatile struct DmaTransfer *const dma3 =
        (volatile struct DmaTransfer *)0x040000d4;

    dma3->destination = *(volatile u32 *)0x03001ed0;
    dma3->source = useAlternate != 0 ? 0x0200a4e0 : 0x02009de0;
    dma3->control = 0x840000e0;

    Func_0808a330(0x10000, 0);
    Func_02001878();
}
