#include "types.h"

#define BattleEffect_InitializeObjectPool Func_0808fe38

struct DmaChannelEffect {
    const void *source;
    void *destination;
    u32 control;
};

void BattleEffect_InitializeObjectPool(s16 mode)
{
    volatile struct DmaChannelEffect *dma =
        (volatile struct DmaChannelEffect *)0x040000d4;
    u8 *pool = Func_080048f4(31, 0x540);
    s32 zero = 0;

    dma->source = &zero;
    dma->destination = pool;
    dma->control = 0x85000150;
    *(s16 *)(pool + 0x528) = mode;
    *(s16 *)(pool + 0x52a) = 0;
    *(s16 *)(pool + 0x534) = 0x3f3f;
    *(s16 *)(pool + 0x536) = 1;
    Func_080041d8((void *)0x0808f52d, 0xc80);
    Func_080041d8((void *)0x0808f499, 0x480);
}
