#include "TYPES.H"
#include "DMA.H"
#include "IWRAM_CALL.H"
#include "RESOURCE.H"

extern struct GameState gGameState;
extern u8 Data_03001cb4[];

/* runtime/blank_display_load_value_and_run.c */
s32 Audio_PlayCue(s32);
void ReelGame_Run(void);

static __inline__ void FillWords(void *dst, s32 size, s32 value)
{
    /* FAKEMATCH: a direct call changes BattlePres_ProcessPendingTileTransfer from mov r0, r4 to lsl r1, r1, #8 (86/86 assembly lines). */
    Iwram_FillWords(dst, size, value);
}

extern u8 *gBattleFxWork[2];
void ColorBuffer_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes);
void ColorBuffer_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup, u32 bytes);

#define ABS(v) ((v) < 0 ? -(v) : (v))

s32 Runtime_BlankDisplayLoadValueAndRun(void)
{
  s32 *p;
  u8 *src;
  if (1)
  {
    *((s16 *) 0x04000000) = 0x40;
    src = (u8 *)((void *) &gGameState);
    p = (s32 *)((u32)&Data_03001cb4);
    *p = *((s32 *)(src + 4));
  }
  Audio_PlayCue(9);
  ReelGame_Run();
  return 0;
}

/* graphics/color/Palette_ScaleRgb555.c */
s32 Graphics_ScaleRgb555(
    u16 *source,
    u16 *destination,
    s32 scale,
    s32 count)
{
    s32 remaining;
    u32 red_mask;
    u32 green_mask;
    u32 blue_mask;
    u32 pixel;
    u32 red;
    u32 green;
    u32 blue;

    if (count > 0) {
        red_mask = 0x1f;
        green_mask = 0x3e0;
        blue_mask = 0x7c00;
        remaining = count;
        do {
            pixel = *source;
            red = pixel & red_mask;
            green = pixel & green_mask;
            blue = blue_mask & pixel;
            red *= scale;
            green *= scale;
            blue *= scale;
            pixel = ((red >> 16) & red_mask) | ((green >> 16) & green_mask);
            pixel |= (blue >> 16) & blue_mask;
            *destination = pixel;
            source++;
            destination++;
            remaining--;
        } while (remaining != 0);
    }
    return 0;
}

/* Runs the pending BG tile transfer once the effect requests it: a plain
   copy to 0x06003500 followed by a refill, or the halved or three-quarter
   colour backup; otherwise counts the frames since the last transfer. */
void BattlePres_ProcessPendingTileTransfer(void)
{
    u8 *work;
    u8 *buffer;

    work = gBattleFxWork[0];
    if (*(s32 *)(work + 0x7824) == 1) {
        buffer = gBattleFxWork[1];
        switch (*(s32 *)(work + 0x7780)) {
        case 1:
            Dma_Set(buffer, (void *)0x06003500, 0x84002000, (volatile u32 *)0x040000d4);
            FillWords(buffer, 0x8000, *(s32 *)(work + 0x7784));
            break;
        case 2:
            if (*(s32 *)(work + 0x7784) == 50)
                ColorBuffer_BackupAndHalveNonzero(buffer, (u8 *)0x06003500, 0x8000);
            else
                ColorBuffer_BackupAndScaleNonzeroThreeQuarters(buffer, (u8 *)0x06003500, 0x8000);
            break;
        }
        *(s32 *)(work + 0x7824) = 0;
        *(s32 *)(work + 0x7820) = 1;
    } else {
        (*(s32 *)(work + 0x7820))++;
    }
}

/* Darken background entries 160-175 and object entries 1-239 by one step
   per channel, stopping each 5-bit channel at 0. */
void Palette_DarkenSceneStep(void)
{
    u16 *color;
    s32 i;

    color = (u16 *)0x05000140;
    for (i = 0; i != 16; i++) {
        s32 blue = (*color >> 10) & 0x1f;
        s32 green = (*color >> 5) & 0x1f;
        s32 red = *color & 0x1f;

        blue--;
        green--;
        red--;

        if (blue < 0)
            blue = 0;
        if (green < 0)
            green = 0;
        if (red < 0)
            red = 0;
        *color++ = (blue << 10) | (green << 5) | red;
    }
    color = (u16 *)0x05000202;
    for (i = 0; i != 239; i++) {
        s32 blue = (*color >> 10) & 0x1f;
        s32 green = (*color >> 5) & 0x1f;
        s32 red = *color & 0x1f;

        blue--;
        green--;
        red--;

        if (blue < 0)
            blue = 0;
        if (green < 0)
            green = 0;
        if (red < 0)
            red = 0;
        *color++ = (blue << 10) | (green << 5) | red;
    }
}

/* Step background colours 1-63 one unit per channel towards the palette of
   resource_id. */
void Palette_StepTowardResource(s32 resource_id)
{
    u16 target[64];
    u16 *color = (u16 *)0x05000000;
    s32 i;

    Dma_Set(Resource_GetTableEntry(resource_id), target, 0x84000020, (volatile u32 *)0x040000d4);
    for (i = 0; i != 64; i++, color++) {
        s32 red = *color & 0x1f;
        s32 green = (*color >> 5) & 0x1f;
        s32 blue = (*color >> 10) & 0x1f;
        s32 target_red = target[i] & 0x1f;
        s32 target_green = (target[i] >> 5) & 0x1f;
        s32 target_blue = (target[i] >> 10) & 0x1f;

        if (red < target_red)
            red++;
        else if (red > target_red)
            red--;
        if (green < target_green)
            green++;
        else if (green > target_green)
            green--;
        if (blue < target_blue)
            blue++;
        else if (blue > target_blue)
            blue--;
        target[i] = (blue << 10) | (green << 5) | red;
    }
    Dma_Set(target + 1, (void *)0x05000002, 0x8000003f, (volatile u32 *)0x040000d4);
}

/* Draw a line into the effect canvas (8bpp tiles, 32 tiles to a row) with
   an 8.8 fraction, keeping the larger of the existing and new colour. The
   major axis is walked from its lower end. */
void BattleFx_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color)
{
    s32 dx = x1 - x0;
    s32 dy = y1 - y0;
    s32 frac = 0x80;
    u8 *canvas = ((u8 **)gBattleFxWork)[1];
    s32 step;
    s32 i;
    s32 x;
    s32 y;
    s32 t;
    u32 off;

    if (ABS(dx) < ABS(dy)) {
        if (dy < 0) {
            t = x0; x0 = x1; x1 = t;
            t = y0; y0 = y1; y1 = t;
            dx = x1 - x0;
            dy = y1 - y0;
        }
        step = (ABS(x1 - x0) << 8) / ABS(y1 - y0);
        x = x0;
        for (i = y0; i != y1; i++) {
            off = (((((u32)i >> 3) << 5) + ((u32)x >> 3)) << 3) + (i & 7);
            off = (off << 3) + (x & 7);
            if (canvas[off] < color)
                canvas[off] = color;
            frac += step;
            if (frac & 0x100) {
                if (dx > 0)
                    x++;
                else
                    x--;
                frac &= ~0x100;
            }
        }
    } else {
        if (dx < 0) {
            t = x0; x0 = x1; x1 = t;
            t = y0; y0 = y1; y1 = t;
            dx = x1 - x0;
            dy = y1 - y0;
        }
        step = (ABS(y1 - y0) << 8) / ABS(x1 - x0);
        y = y0;
        for (i = x0; i != x1; i++) {
            off = (((((u32)y >> 3) << 5) + ((u32)i >> 3)) << 3) + (y & 7);
            off = (off << 3) + (i & 7);
            if (canvas[off] < color)
                canvas[off] = color;
            frac += step;
            if (frac & 0x100) {
                if (dy > 0)
                    y++;
                else
                    y--;
                frac &= ~0x100;
            }
        }
    }
}
