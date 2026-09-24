#include "TYPES.H"

/* battle/effects/scene_transition/finish_and_release_heap_block.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Animation_ApplyChildPalette(void *a, s32 b);
void Runtime_ReleaseHeapBlock(s32 a);
void Ui_SetBank15PaletteAndClearRenderMode(void);
void Scheduler_RemoveCallback(s32);
void *Object_GetById(u32);
void BattleFx_PrepareBufferInterpolation(void);
extern u8 RomBytes_08097645;

void BattleFx_FinishSceneAndReleaseHeapBlock(void)
{
    void *work;

    work = *(void **)ADDR_03001EA8;
    Ui_SetBank15PaletteAndClearRenderMode();
    Scheduler_RemoveCallback((s32)&RomBytes_08097645);
    Animation_ApplyChildPalette(Object_GetById(FIELD_AT_OFFSET(work, u16, 0x290)), 1);
    BattleFx_PrepareBufferInterpolation();
    Runtime_ReleaseHeapBlock(0x16);
}
