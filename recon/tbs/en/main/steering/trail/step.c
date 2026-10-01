/* NONMATCHING: Func_080cc5d8, measured 2026-10-01.
 * Source hypothesis: Advance the frame before the independent wait, retaining both persistent carriers.
 * EN: 916/904 complete linked bytes, 794 differing byte positions, first +0x12, all literal pools included.
 * Six ordinary TBS targets compile successfully; text extents in bytes:
 * JA 916; EN 916; DE 916; ES 916; FR 916; IT 916.
 * Other editions have no complete linked proof here: their still-raw
 * physical table owners need source labels before this C can be linked.
 * Existing source names replace the stale palette/callback numeric aliases.
 * This attempt is retained under S4 and earns no credit.
 */
#include "TYPES.H"
#include "IO_REG.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"
#include "BATTLE_PRESENTATION.H"


#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

extern void *gWorkSlot[];

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);

extern const u8 Data_080ee058[4];
extern const u8 Data_080ee05c[4];
extern const u8 Data_080ee060[4];
extern const u16 BattleFx6_FlareCells[];

void *Runtime_AllocateHeapBlock(s32 kind, s32 size);
void BattleFx_BeginCanvasLayer(s32 mode);
void *Resource_GetTableEntry(s32 id);
u32 Random16(void);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
void Scheduler_RemoveCallback(void *callback);
void Audio_PlayCue(s32 id);
void EffectPosition_ApplyStepAndYOffset(s32 source, void *screen);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);
void Runtime_ReleaseHeapBlock(s32 id);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
s32 BattleFx_EndCanvasLayer(void);

void Func_080cc5d8(void *object)
{
    /* FAKEMATCH: Preserve the persistent heap-work pointer in the native high-register carrier across all draw callbacks. */
    register void *work asm("r9");
    void *canvas;
    DrawRectangleFn rectangle[2];
    void *trail_source;
    void *palette;
    s32 status;
    s32 palette_id;
    u8 *star;
    s32 i;
    /* FAKEMATCH: Preserve the native high-register frame carrier beside the per-sheet offset. */
    register s32 frame asm("r10");
    s32 screen[3];
    WordCopyFn copy;
    s32 callback_interval;
    void *display_base;

    work = Runtime_AllocateHeapBlock(39, 0x782c);
    canvas = Runtime_AllocateHeapBlock(40, 0x4000);
    trail_source = Runtime_AllocateHeapBlock(41, 0x60e);
    M2C_FIELD(work, void **, 0x7828) = object;
    BattleFx_BeginCanvasLayer(0);

    M2C_FIELD(work, s32 *, 0x77b4) = 24;
    M2C_FIELD(work, s32 *, 0x77b8) = 0;
    REG_BLDALPHA = 0x100c;
    REG_BG2PA = 0x100;

    Resource_LoadAndDecompress((s32)&ResourceId_CyanSparkSheet, work, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, trail_source, 0, 0);

    switch (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0)) {
    case 0:
        palette_id = (s32)&ResourceId_YellowPaletteA;
        break;
    case 1:
        palette_id = (s32)&ResourceId_IceTileSheet;
        break;
    case 2:
        palette_id = (s32)&ResourceId_RedPaletteA;
        break;
    default:
        palette_id = (s32)&ResourceId_VioletPaletteA;
        break;
    }
    palette = Resource_GetTableEntry(palette_id);
    copy = (WordCopyFn)Iwram_CopyWords;
    status = copy((void *)0x05000000, palette, 128);

    star = Ram_MapCellBuffer;
    for (i = 0; i != 128; i++) {
        M2C_FIELD(star, s32 *, 4) = 0x800000;
        M2C_FIELD(star, s32 *, 0) = (s32)(Random16() & 0xFFFF);
        M2C_FIELD(star, s32 *, 8) = (s32)(Random16() & 0x1FF) + 1024;
        M2C_FIELD(star, s32 *, 24) = -i;
        star += 28;
    }

    for (i = 0; i != 64; i++) {
        u8 *slot = (u8 *)work + 0x7080 + i * 28;
        M2C_FIELD(slot, s32 *, 0) = (s32)(Random16() & 0xFFFF);
        M2C_FIELD(slot, s32 *, 4) = (s32)(Random16() & 31) + 16;
        M2C_FIELD(slot, s32 *, 24) = (i & 15) + 16;
    }

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    callback_interval = 0x480;
    Scheduler_AddOrUpdateCallback((void *)BattlePresentation_ProcessPendingGraphicsTransfer, callback_interval);

    status = BattleEffect_LoadWork(46, 7, 7, 7, 3);
    rectangle[0] = (DrawRectangleFn)gWorkSlot[46];
    Audio_PlayCue(140);

    for (frame = 0; frame != 56;) {
        {
            register s32 *pos asm("r5") = screen; /* FAKEMATCH: the reference reads the projection through r5 */
            EffectPosition_ApplyStepAndYOffset(M2C_FIELD(object, s32 *, 8), screen);
            REG_BG2X = (64 - pos[0]) << 8;
            if (frame > 49)
                REG_BLDALPHA = (112 - frame * 2) | 0x1000;
        }

        if (frame == 26) {
            Audio_PlayCue(212);
            ObjectGroup_UpdateMembers(
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36),
                7, -1, 0, 20);
        }

        if ((u32)(frame - 28) <= 20) {
            s32 sprite_frame = __divsi3(frame - 28, 3);

            rectangle[0](
                canvas, (u8 *)work + 0x1400 + sprite_frame * 0x900,
                40, screen[1] - 24, 48, 48);
        }

        if ((u32)frame <= 14) {
            /* FAKEMATCH: Probe the sheet-offset carrier shared across the four draw calls in the native high register. */
            register s32 offset asm("r8") = (__modsi3(__divsi3(frame, 3), 5)) << 10;

            for (i = 0; i != 4; i++) {
                s32 x;
                s32 y;

                /* FAKEMATCH: Prevent the sheet address sum from being hoisted out of the native four-sheet loop; the offset itself keeps its C value. */
                asm("" : "+r"(offset));

                status = BattleEffect_LoadWork(47, 7, 7, Data_080ee060[i] | 3, 2);
                x = (s8)Data_080ee058[i] + 32;
                y = (screen[1] + (s8)Data_080ee05c[i]) - 32;
                rectangle[1] = (DrawRectangleFn)gWorkSlot[47];
                rectangle[1](canvas, (u8 *)work + offset, x, y, 32, 32);
                Runtime_ReleaseHeapBlock(47);
            }
        }

        if (frame >= 0) {
            u8 *slot = (u8 *)work + 0x7080;

            for (i = 0; i != 64; i++) {
                if (M2C_FIELD(slot, s32 *, 24) >= 0
                        && M2C_FIELD(slot, s32 *, 4) > 0) {
                    s32 radius_x;
                    s32 radius_y;
                    s32 half;
                    s32 full;

                    half = (M2C_FIELD(slot, s32 *, 24) >> 3) + 1;
                    radius_x = ((M2C_FIELD(slot, s32 *, 4)
                        * Trig_Sin(M2C_FIELD(slot, s32 *, 0))) >> 16)
                        + 64;
                    radius_y = ((M2C_FIELD(slot, s32 *, 4)
                        * Trig_Cos(M2C_FIELD(slot, s32 *, 0))) >> 16)
                        + screen[1];
                    if (half <= 0) {
                        half = 1;
                    }
                    full = half << 1;
                    rectangle[0](
                        canvas, trail_source + BattleFx6_FlareCells[half - 1],
                        radius_x - half, radius_y - half, full, full);
                    M2C_FIELD(slot, s32 *, 4) -= 2;
                    M2C_FIELD(slot, s32 *, 24) -= 1;
                }
                slot += 28;
            }
        }

        ObjectGroup_TickMemberTimers();
        do { M2C_FIELD(work, s32 *, 0x7824) = 1; } while (0);
        frame++;
        WaitFrames(1);
    }

    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback((void *)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
    Runtime_ReleaseHeapBlock(41);
    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
}
