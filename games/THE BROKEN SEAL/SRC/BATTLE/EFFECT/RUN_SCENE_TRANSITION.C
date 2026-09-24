#include "DMA.H"

#define Runtime_AllocateBlock Func_080048f4
#define BattleEffect_InitializeSharedScene Func_08097384
#define BattleFx_AdvanceHueCycle Func_080978c4
#define BattleFx_ApplyColorToTargetBuffer Func_08091200
#define BattleFx_StartBufferInterpolation Func_08091254
#define Ui_FillBank15PaletteGrey Func_08097a7c
#define Scheduler_AddOrUpdateCallback Func_080041d8
#define RunSceneTransitionEffect Func_08097540

void *Runtime_AllocateBlock(s32 slot, s32 size);
void BattleEffect_InitializeSharedScene(void);
s32 Func_08002304(s32, s32);
void BattleFx_AdvanceHueCycle(void);
void BattleFx_ApplyColorToTargetBuffer(s32 color, s32 mode);
void BattleFx_StartBufferInterpolation(s32 frames);
void Ui_FillBank15PaletteGrey(void);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);

/* Allocates and clears the 0x298-byte transition work, tints the target
   buffer with the current hue, records the origin and starts the
   transition task. */
void RunSceneTransitionEffect(s32 x, s32 y)
{
    u8 *work;
    volatile u32 zero;

    work = Runtime_AllocateBlock(22, 0x298);
    BattleEffect_InitializeSharedScene();
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x850000a6, (volatile u32 *)0x040000d4);
    *(u16 *)(work + 0x28e) = Func_08002304(*(s32 *)0x03001e40 << 1, 360);
    BattleFx_AdvanceHueCycle();
    BattleFx_ApplyColorToTargetBuffer(((s8)work[0x28d] << 10) | ((s8)work[0x28c] << 5) |
                                      (s8)work[0x28b] | 0x200000, 1);
    BattleFx_StartBufferInterpolation(8);
    *(u16 *)(work + 0x290) = x;
    *(u16 *)(work + 0x292) = y;
    work[0x294] = 8;
    Ui_FillBank15PaletteGrey();
    Scheduler_AddOrUpdateCallback((void (*)(void))0x08097645, 0xc80);
}
