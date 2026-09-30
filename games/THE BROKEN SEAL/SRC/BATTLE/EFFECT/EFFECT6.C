#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "RAM_BUFFER.H"

extern u8 gMapCellBuffer[];
extern u8 gBattleFxWork[];
void BattleFx_ArmBg2AffineHBlankDma(void);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
extern u8 *gBattleWork;

/* The two seven-byte-stride rectangle tables are plain, non-const arrays:
 * their element loads are ordered against the outgoing-argument stores at
 * each blit call site, which a const spelling would let float away. */
extern u8 CounterReveal_PanelX[];
extern u8 CounterReveal_PanelY[];
s32 BattleFx_BeginTiledCanvas(s32 mode);
void *Resource_GetTableEntry(s32 id);
u32 Resource_DecodeType01(const void *source, void *destination);
s32 BattleFx_EndCanvasLayer(void);
void **GetBattleObjectSlotFar(s32 member_id);
void EffectPosition_ApplyStepAndYOffset(s32 member_id, void *out);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Audio_PlayCue(s32 cue);
void Object_SetPosition(void *object, s32 a, s32 b, s32 c);
void BattleBackground_LoadFar(s32 a, s32 b, s32 c);

typedef s32 (*WordCopy)(void *, const void *, s32);

static __inline__ void CopyWords(WordCopy copy, void *destination,
                                 const void *source, s32 size)
{
    copy(destination, source, size);
}

extern u8 gWorkSlot[];
#define PIXEL_BUFFER Ram_MapCellBuffer

/*
 * Battle-presentation sub-effect at 0x080cfef4, structurally related to the
 * "0x03001eec battle work" family already recovered in
 * games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/MEMBER_ORBIT.C (owner 080ce85c) and
 * recon/tbs/en/main/080e7404.c: same heap_cache/work/canvas prologue,
 * the same raw-offset field-access spelling, and the same
 * BG2-affine + rectangle-blit (BattleEffect_LoadWork heap kinds 46/47) setup.
 *
 * Unlike the member-orbit effect this owner drives a single fixed party
 * member (its slot index lives at object+0x24, not an iterated array from
 * +0x24), runs a fixed 132-frame count rather than member_count*16+48, loads
 * a second graphics resource (ResourceId_RedCrescentSheetA) straight into OBJ VRAM at
 * 0x02010000, and drives BG2PC (0x04000052) directly rather than the BG2
 * reference-point registers.  Frames 88-99 additionally reveal a run of
 * glyph-style rectangles (width/height pairs 57x98, 99x69, 128x91, 128x59,
 * 122x29, 76x25) read from two seven-byte-stride tables at 0x080ee10c and
 * 0x080ee11a, selected by the same object+4 "kind" field used for the
 * rectangle-routine pick -- most plausibly a spinning number/counter reveal
 * rather than the orbiting-member sprite loop of the sibling effect.
 *
 * `status` and `_call_via_r3`/`Func_080072f4` follow the established
 * sibling reading: both addresses are `_call_via_rN` thunk slots
 * (the container-built bank at 0x080072e4) -- r3 for _call_via_r3, r4 for Func_080072f4
 * -- so each call is a genuine indirect call through a traced function
 * pointer, not a call to a real symbol at that address.
 */
void BattleFx_RunCounterReveal(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    void *palette;
    void *sprite_vram;
    u32 status;
    void *rectangle[2];
    void *second_rectangle;
    void *member_object;
    s32 pos[6];
    s32 curve[2];
    s32 draw_enabled;
    s32 idx_a;
    s32 idx_b;
    s32 zero_val;
    s32 frame;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    zero_val = 0;
    (*(void **)((u8 *)(work) + (0x7828))) = object;
    BattleFx_BeginTiledCanvas(0);
    (*(s16 *)((u8 *)((void *)0x04000020) + (0))) = 0x100;
    (*(s16 *)((u8 *)((void *)0x04000020) + (0x32))) = 0x1010;
    palette = Resource_GetTableEntry((s32)&ResourceId_CounterRevealSheet);
    status = Iwram_CopyWords((void *)0x05000000, palette, 128);
    palette = (u8 *)palette + 128;
    status = Resource_DecodeType01(palette, work);
    sprite_vram = Ram_MapCellBuffer;
    palette = Resource_GetTableEntry((s32)&ResourceId_RedCrescentSheetA);
    palette = (u8 *)palette + 128;
    status = Resource_DecodeType01(palette, sprite_vram);
    status = BattleEffect_LoadWork(46, 7, 7, 3, 1);
    rectangle[0] = heap_cache[7];
    status = BattleEffect_LoadWork(47, 7, 7, 7, 1);
    second_rectangle = heap_cache[8];
    rectangle[1] = second_rectangle;
    Scheduler_AddOrUpdateCallback((s32)BattleFx_ArmBg2AffineHBlankDma, 0x480);
    (*(s32 *)((u8 *)(work) + (0x7780))) = 1;
    (*(s32 *)((u8 *)(work) + (0x7784))) = zero_val;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    draw_enabled = 1;
    if ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4))) == 1) {
        curve[0] = -0x500000;
    } else {
        curve[0] = 0x700000;
    }
    curve[1] = -0x200000;
    for (frame = 0; frame != 132; frame++) {
        s32 spin;
        s32 screen_x;
        s32 screen_y;
        s32 amp;
        s32 row_base;
        s32 angle;
        s32 i;
        s32 *scanline;

        spin = frame << 9;
        screen_x = (curve[0] >> 16) + ((Trig_Sin(spin) << 4) >> 16) + 48;
        screen_y = (curve[1] >> 16) + ((Trig_Cos(spin) << 2) >> 16) + 16;
        if (frame == 88) {
            Audio_PlayCue(134);
        }
        if (frame == 32) {
            if ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4))) == 1) {
                curve[0] = -0x200000;
            } else {
                curve[0] = 0x480000;
            }
            curve[1] = 0x180000;
            draw_enabled = 0;
        }
        if (frame == 33) {
            (*(s16 *)((u8 *)((void *)0x04000052) + (0))) = 0x1010;
            draw_enabled = 1;
        }
        if (frame == 64) {
            EffectPosition_ApplyStepAndYOffset(
                (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x24))),
                pos);
            if ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4))) == 1) {
                curve[0] = (pos[0] - 128) << 16;
            } else {
                curve[0] = (pos[0] - 64) << 16;
            }
            curve[1] = 0;
            draw_enabled = 0;
        }
        if (frame == 65) {
            (*(s16 *)((u8 *)((void *)0x04000052) + (0))) = 0x1010;
            draw_enabled = 1;
        }
        scanline = (s32 *)((u8 *)work + 0x6980);
        amp = 0;
        if (frame <= 31) {
            if (frame > 15) {
                amp = (frame * 2) - 32;
                (*(s16 *)((u8 *)((void *)0x04000052) + (0))) =
                    (s16)((31 - frame) | 0x1000);
            }
        } else if (frame <= 63) {
            if (frame > 47) {
                amp = (frame * 2) - 96;
                (*(s16 *)((u8 *)((void *)0x04000052) + (0))) =
                    (s16)((63 - frame) | 0x1000);
            }
        }
        if (amp < 0) {
            amp = 0;
        }
        /* The counter is live from here, so it shares no register with
         * screen_x, which dies in the row-base expression below. */
        i = 0;
        row_base = (6 - screen_x) << 8;
        angle = frame << 11;
        for (; i != 160; i++) {
            *scanline++ =
                row_base - ((Trig_Sin(angle) * amp) >> 10);
            angle += 0x800;
        }
        if (draw_enabled != 0) {
            if ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4))) == 0) {
                idx_a = 0;
                idx_b = 0;
            } else {
                idx_a = 1;
                idx_b = i >> 31;
            }
            if (frame <= 87) {
                ((DrawRectangleFn)rectangle[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4)))])(
                    canvas, work, CounterReveal_PanelX[idx_a * 7],
                    CounterReveal_PanelY[idx_b * 7] + screen_y, 57, 98);
            } else {
                if (frame <= 91) {
                    ((DrawRectangleFn)rectangle[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4)))])(
                        canvas, work, CounterReveal_PanelX[idx_a * 7],
                        CounterReveal_PanelY[idx_b * 7] + screen_y, 57, 98);
                }
                ((DrawRectangleFn)rectangle[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4)))])(
                    canvas, (u8 *)work + 0x15D2, CounterReveal_PanelX[idx_a * 7 + 1],
                    CounterReveal_PanelY[idx_b * 7 + 1] + screen_y, 99, 69);
                if ((u32)(frame - 88) <= 1U) {
                    status = Iwram_FillWords(canvas, 0x4000, 0x3F3F3F3F);
                }
                if ((u32)(frame - 90) <= 1U) {
                    ((DrawRectangleFn)rectangle[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4)))])(
                        canvas, (u8 *)work + 0x3081,
                        CounterReveal_PanelX[idx_a * 7 + 2],
                        CounterReveal_PanelY[idx_b * 7 + 2] + screen_y, 128, 91);
                }
                if ((u32)(frame - 92) <= 1U) {
                    ((DrawRectangleFn)rectangle[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4)))])(
                        canvas, (void *)gMapCellBuffer,
                        CounterReveal_PanelX[idx_a * 7 + 3],
                        CounterReveal_PanelY[idx_b * 7 + 3] + screen_y, 128, 91);
                }
                if ((u32)(frame - 94) <= 1U) {
                    ((DrawRectangleFn)rectangle[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4)))])(
                        canvas, gMapCellBuffer + 0x2d80,
                        CounterReveal_PanelX[idx_a * 7 + 4],
                        CounterReveal_PanelY[idx_b * 7 + 4] + screen_y, 128, 59);
                }
                if ((u32)(frame - 96) <= 1U) {
                    ((DrawRectangleFn)rectangle[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4)))])(
                        canvas, gMapCellBuffer + 0x4b00,
                        CounterReveal_PanelX[idx_a * 7 + 5],
                        CounterReveal_PanelY[idx_b * 7 + 5] + screen_y, 122, 29);
                }
                if ((u32)(frame - 98) <= 1U) {
                    ((DrawRectangleFn)rectangle[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4)))])(
                        canvas, gMapCellBuffer + 0x58d2,
                        CounterReveal_PanelX[idx_a * 7 + 6],
                        CounterReveal_PanelY[idx_b * 7 + 6] + screen_y, 76, 25);
                }
            }
        }
        if (frame == 88) {
            member_object = *GetBattleObjectSlotFar((*(s16 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x24))));
            (*(s32 *)((u8 *)(member_object) + (0x28))) = 0x10000;
            (*(s32 *)((u8 *)(member_object) + (0x34))) = 0x20000;
            (*(s32 *)((u8 *)(member_object) + (0x30))) = 0x20000;
            (*(s32 *)((u8 *)(member_object) + (0x48))) = 0;
            (*(s8 *)((u8 *)(member_object) + (0x5A))) = 0;
            (*(s8 *)((u8 *)(member_object) + (0x58))) = 0;
            Object_SetPosition(member_object,
                (*(s32 *)((u8 *)(member_object) + (8))) << 1, 0,
                (*(s32 *)((u8 *)(member_object) + (16))));
            ObjectGroup_UpdateMembers(
                (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x24))), -1,
                5, -1, 0);
        }
        if (frame == 120) {
            (*(s32 *)((u8 *)(*GetBattleObjectSlotFar((*(s16 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x24))))) + (0x48))) = 0xAB85;
        }
        (*(s32 *)((u8 *)(work) + (0x7824))) = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((void *)BattlePresentation_ProcessPendingGraphicsTransfer);
    Scheduler_RemoveCallback((void *)BattleFx_ArmBg2AffineHBlankDma);
    BattleBackground_LoadFar(1, (*(u16 *)((u8 *)(gBattleWork) + (0x648))), 24);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

/* Convert the tiled background through a blue palette ramp. Each screen
 * column begins after its random delay; the row budget grows each frame.
 * The second palette bank preserves the converted pixels after the sweep.
 */
void Graphics_ConvertBackgroundToBlueRamp(void)
{
    s32 i, x, y, row, end, speed;
    s32 red, green, blue;
    u16 color;
    u8 *delays, *p, *pixel;
    u16 *palette;
    u32 delay_mask;
    palette = (u16 *)0x05000040;
    i = 0;
    do {
        s32 half;
        half = i / 2;
        *palette++ = (i << 10) | (half << 5) | half;
        i++;
    } while (i != 32);
    delays = *(u8 **)(gWorkSlot + 41 * 4);
    end = 0;
    speed = 16;
    CopyWords(Iwram_CopyWords, PIXEL_BUFFER, (void *)0x06008000, 0x7800);
    delay_mask = 63;
    p = delays;
    do {
        *p++ = Random16() & delay_mask;
    } while (p != delays + 256);
    row = 0;
    i = 0;
    do {
        end += speed / 4;
        speed++;
        while (row != end) {
            x = 0;
            do {
                y = row - delays[x];
                if (y < 0) goto next_pixel;
                if (y > 119) goto next_pixel;
                {
                    pixel = PIXEL_BUFFER + ((x & 7) + ((x / 8) << 6) + ((y & 7) << 3) + ((y / 8) << 11));
                    color = ((s16 *)0x05000000)[*pixel];
                    red = color & 31;
                    green = (color >> 5) & 31;
                    blue = (color >> 10) & 31;
                    if (red < green) red = green;
                    if (red < blue) red = blue;
                    *pixel = 63 - red;
                }
next_pixel:
                x++;
            } while (x != 256);
            row++;
        }
        Iwram_CopyWords((void *)0x06008000, PIXEL_BUFFER, 0x7800);
        WaitFrames(1);
        if (end > 248) break;
        i++;
    } while (i != 27);
    palette = (u16 *)0x050000c0;
    i = 0;
    do {
        s32 half;
        half = i / 2;
        *palette++ = (i << 10) | (half << 5) | half;
        i++;
    } while (i != 32);
    i = 0;
    do {
        PIXEL_BUFFER[i] += 64;
        i++;
    } while (i != 0x7800);
    CopyWords(Iwram_CopyWords, (void *)0x06008000, PIXEL_BUFFER, 0x7800);
    WaitFrames(1);
}
