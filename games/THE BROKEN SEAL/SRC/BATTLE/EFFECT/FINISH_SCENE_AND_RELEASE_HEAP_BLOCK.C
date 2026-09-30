#include "TYPES.H"
extern u8 gBattleBgFxWork[];

/* battle/effects/scene_transition/finish_and_release_heap_block.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Animation_ApplyChildPalette(void *a, s32 b);
void Runtime_ReleaseHeapBlock(s32 a);
void Ui_SetBank15PaletteAndClearRenderMode(void);
void Scheduler_RemoveCallback(s32);
void *Object_GetById(u32);
void BattleFx_PrepareBufferInterpolation(void);
extern u8 Func_08097644;

void BattleFx_FinishSceneAndReleaseHeapBlock(void)
{
    void *work;

    work = *(void **)((u32)&gBattleBgFxWork);
    Ui_SetBank15PaletteAndClearRenderMode();
    Scheduler_RemoveCallback((s32)&Func_08097644);
    Animation_ApplyChildPalette(Object_GetById(FIELD_AT_OFFSET(work, u16, 0x290)), 1);
    BattleFx_PrepareBufferInterpolation();
    Runtime_ReleaseHeapBlock(0x16);
}
