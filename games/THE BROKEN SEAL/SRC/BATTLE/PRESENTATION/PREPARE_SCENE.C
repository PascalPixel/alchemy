#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "RESOURCE_IDS.H"
void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_RunTwoResource(s32, s32);
s32 Runtime_AllocateHeapBlock(s32 kind, s32 size);
s32 Scheduler_AddOrUpdateCallback(s32 callback, s32 order);

/* battle/presentation/misc/prepare.c */
/*
 * Battle presentation setup at 0x080ccaec.  Allocate the kind-39 and kind-40
 * work blocks, reset the presentation, write the BG2PA identity scale and the
 * blend coefficients, stream the palette selected by the caller's kind into
 * palette RAM through the IWRAM word-copy kernel, seed three work-block
 * fields, then schedule two frame callbacks.  The work-block offsets are
 * taken by position and are not verified.
 */

/*
 * ResourceId_ rows are small resource numbers the reference loads from its
 * literal pool rather than materializing, so they must not be folded into
 * constants.  Battle_Apply is declared as returning s32 although the result
 * is unused: the value-returning call form keeps the call the last setter of
 * r0, which is what places each callback's pool load after the order
 * argument's shift.
 */


void *Resource_GetTableEntry(s32 id);
void BattlePresentation_DrawStreaks(void);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);

void BattlePresentation_PrepareScene(s32 kind)
{
    u8 *work;
    void *palette;
    s32 id;

    work = (u8 *)Runtime_AllocateHeapBlock(39, 0x782c);
    Runtime_AllocateHeapBlock(40, 0x4000);
    BattleFx_BeginCanvasLayer(0);
    *(s32 *)(work + 0x77b4) = 24;
    *(s16 *)0x04000020 = 0x100;
    *(s16 *)0x04000052 = 0x1010;
    switch (kind) {
    case 0:
        id = (s32)&ResourceId_YellowPaletteB;
        break;
    case 1:
        id = (s32)&ResourceId_CrystalSheet;
        break;
    case 2:
        id = (s32)&ResourceId_EmberStreakSheet;
        break;
    case 3:
        id = (s32)&ResourceId_VioletPaletteD;
        break;
    case 4:
    default:
        id = (s32)&ResourceId_DustPuffSheet;
        break;
    }
    palette = Resource_GetTableEntry(id);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    *(s32 *)(work + 0x778c) = 0;
    *(s32 *)(work + 0x7780) = 3;
    *(s32 *)(work + 0x7784) = 0x06060606;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_DrawStreaks, 0xC80);
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
}

/* battle/effects/runtime/misc/schedule_callbacks_and_release_blocks.c */
extern void Scheduler_RemoveCallback(void (*)(void));

extern u8 Palette_StepFadeTransfer;

void BattleFx_ScheduleCallbacksAndReleaseBlocks(void)
{
    Scheduler_RemoveCallback((void (*)(void))&BattlePresentation_DrawStreaks);
    Scheduler_RemoveCallback((void (*)(void))&BattlePresentation_ProcessPendingGraphicsTransfer);
    {
        s32 (*transfer)(void *, s32) = Iwram_ClearWords;

        transfer((void *)0x06004000, 0x4000);
    }
    Scheduler_RemoveCallback((void (*)(void))&Palette_StepFadeTransfer);
    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
}

/* battle/effects/two_resource/run_mode0.c */
void BattleFx_RunTwoResourceMode0(s32 arg0)
{
    BattleFx_RunTwoResource(arg0, 0);
}

/* battle/effects/two_resource/run_mode1.c */
void BattleFx_RunTwoResourceMode1(s32 arg0)
{
    BattleFx_RunTwoResource(arg0, 1);
}
