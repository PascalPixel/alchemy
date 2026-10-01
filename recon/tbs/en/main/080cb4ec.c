/* Battle effect: a rain of blades. Sixty-four blades are seeded at random
   columns with a sideways drift set by the column; each of 120 frames draws
   sixteen of them, falling once their turn comes and then shrinking, while
   the screen shakes and the first target is hit every fourth frame from
   frame 23 to 87.

   2026-10-01 slice-6: rewritten from the listing after its matched sibling
   BattleFx_RunTwoResource (PRESENT3.C), 27 instructions off (was 106), all
   in the two draw calls of the inner loop and all one rotation of three
   registers: the reference holds the drift in r6, the cell in r4, the
   width in r5 and the height in r4; this draft the drift in r4, the cell
   in r5, the width in r6 and the height in r5. The allocator ranks the
   drift first here (18 weighted references over 10 instructions) because
   the blitter index, drift >> 31, is computed before the call's operands;
   the reference computes it after them, which keeps the drift alive past
   the width and height loads and ranks it below both. Loading the operands
   in statements before the call does that but moves 62 other registers;
   the permuter (60 s) found nothing. The width and height must each be
   loaded once (assigned inside the operand list), or 132 instructions
   differ.
   Data_080edf7f, Data_080edf83 and Data_080edf88 (cell widths, heights and
   sheet offsets) are labelled in the English scaffold only; the other five
   editions need the same three labels when this is adopted. Resource 0x78
   is ResourceId_SwordSlashSheet. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"

extern u8 gBattleFxWork[];
/* Per drift step 0..3: the cell's width and height and its offset in the
   sheet. */
extern const u8 Data_080edf7f[];
extern const u8 Data_080edf83[];
extern const u16 Data_080edf88[];

void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void ObjectGroup_TickMemberTimers(void);
void Camera_ApplyShake(s32 x, s32 y);

void Unnamed_080cb4ec(struct BattleEffectArgument *efx)
{
    struct EffectPosition position;
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle *blit;
    struct EffectStep *blade;
    s32 frame;
    s32 i;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = efx;
    BattleFx_BeginCanvasLayer(1);
    *(u16 *)0x04000020 = 0x100;
    *(u16 *)0x04000052 = 0x1000;
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    draw[0] = (DrawRectangle)heap_cache[46 - 39];
    BattleEffect_LoadWork(47, 7, 7, 7, 1);
    draw[1] = (DrawRectangle)heap_cache[47 - 39];
    blit = draw;
    Resource_LoadAndDecompress((s32)&ResourceId_SwordSlashSheet, work, 1, 1);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &position);
    *(s32 *)0x04000028 = (64 - position.x) << 8;
    for (i = 0; i != 64; i++) {
        s32 column;

        blade = &work->particles[i];
        column = Random16() % 96 + 16;
        blade->x = column;
        blade->y = (24 - i / 4) << 16;
        if (column <= 43)
            blade->velocity_x = 3;
        else if (column <= 51)
            blade->velocity_x = 2;
        else if (column <= 59)
            blade->velocity_x = 1;
        else if (column <= 67)
            blade->velocity_x = 0;
        else if (column <= 75)
            blade->velocity_x = -1;
        else if (column <= 83)
            blade->velocity_x = -2;
        else
            blade->velocity_x = -3;
        blade->velocity_x <<= 17;
        blade->velocity_y = 0x80000;
        blade->x <<= 16;
    }
    Audio_PlayCue(212);
    for (frame = 0; frame != 120; frame++) {
        if (frame <= 16) {
            *(u16 *)0x04000052 = frame | 0x1000;
            if (frame == 16)
                *(u16 *)0x04000050 = 0;
        }
        if (frame > 103) {
            *(u16 *)0x04000052 = (0x78 - frame) | 0x1000;
            if (frame == 104)
                *(u16 *)0x04000050 = 0x3f44;
        }
        for (i = 15; i != -1; i--) {
            s32 drift;
            s32 cell;
            u32 wide;
            u32 high;

            blade = &work->particles[i];
            drift = blade->velocity_x;
            cell = (drift < 0 ? -drift : drift) >> 17;
            if (frame < i * 4 + 25) {
                blit[(u32)drift >> 31](canvas, (u8 *)work + Data_080edf88[cell],
                    (blade->x >> 16) - ((wide = Data_080edf7f[cell]) >> 1),
                    (blade->y >> 16) - ((high = Data_080edf83[cell]) >> 1),
                    wide, high);
                if (frame >= i * 4 + 16) {
                    blade->x += blade->velocity_x;
                    blade->y += blade->velocity_y;
                }
            } else {
                blit[(u32)drift >> 31](canvas, (u8 *)work + Data_080edf88[cell],
                    (blade->x >> 16) - ((wide = Data_080edf7f[cell]) >> 1),
                    (blade->y >> 16) - ((high = Data_080edf83[cell]) >> 1),
                    wide, high - 4);
            }
        }
        if (frame >= 23 && frame <= 87 && (frame & 3) == 0) {
            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 2);
            work->shake_frames = 1;
            if ((frame & 7) == 0)
                Audio_PlayCue(133);
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
