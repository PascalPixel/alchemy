#include "types.h"

s32 Func_080072f0(void *, u32, u32, void *);

extern char Value_06000290;
extern char Value_06000280;
extern char Value_0600028c;
extern char Value_03000164;

#define START_BATTLE_DMA(source_, destination_, control_) do { \
    u32 dma_destination = (u32)(destination_); \
    u32 dma_source = (u32)(source_); \
    u32 dma_control = (control_); \
    u32 *dma = (u32 *)0x040000d4; \
    dma[0] = dma_source; \
    dma[1] = dma_destination; \
    dma[2] = dma_control; \
} while (0)

#define BattlePresentation_InitializeTilePattern Func_080b5ad4

s32 BattlePresentation_InitializeTilePattern(void)
{
    u32 control = 0x80000008;

    START_BATTLE_DMA(&Value_06000290, &Value_06000280, control);
    return Func_080072f0(&Value_0600028c, 20, control, &Value_03000164);
}

/*
 * This 12-byte local body is independently byte-exact.  It remains part of
 * the assembly owner until Func_080b5ad4 shrinks by four bytes and restores
 * its original 0x080b5b08 address.
 */
void Func_080b5b08(void)
{
    u16 zero = 0;

    *(u16 *)0x04000012 = zero;
}
