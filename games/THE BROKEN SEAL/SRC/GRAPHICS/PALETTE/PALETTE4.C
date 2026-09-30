#include "TYPES.H"

/* Scales each 5-bit component by its own Q16 factor and packs BGR555. */
u16 Color_ScaleComponents(const s16 *rgb, s32 red, s32 green, s32 blue);
extern const u8 PaletteGlow_WaveTable[];
#define GLOW_PALETTE ((u16 *)0x050001e8)

extern u8 gGameState[];

/* Cycles a base color around the wave table and writes seven shades of it
   to OBJ palette 15, entries 4 to 10. */
void PaletteGlow_Update(s32 phase, s32 brightness)
{
    s16 rgb[3];
    s16 step;
    s16 red;
    s16 green;
    s16 blue;
    s16 offset;

    step = ((phase + 12) % 24) * 4;
    offset = brightness - 7;
    red = PaletteGlow_WaveTable[(s16)(step % 96)] + offset;
    green = PaletteGlow_WaveTable[(step + 32) % 96] + offset;
    blue = PaletteGlow_WaveTable[(step + 64) % 96] + offset;
    if (red < 0)
        red = 0;
    if (red > 31)
        red = 31;
    if (green < 0)
        green = 0;
    if (green > 31)
        green = 31;
    if (blue < 0)
        blue = 0;
    if (blue > 31)
        blue = 31;
    rgb[0] = red;
    rgb[1] = green;
    rgb[2] = blue;
    GLOW_PALETTE[0] = Color_ScaleComponents(rgb, 0xeeee, 0xcccc, 0x11110);
    GLOW_PALETTE[1] = Color_ScaleComponents(rgb, 0xd555, 0xbbbb, 0xeeee);
    GLOW_PALETTE[2] = Color_ScaleComponents(rgb, 0xbbbb, 0xaaaa, 0xcccc);
    GLOW_PALETTE[3] = Color_ScaleComponents(rgb, 0xa221, 0x9999, 0xaaaa);
    GLOW_PALETTE[4] = Color_ScaleComponents(rgb, 0x10888, 0xdddd, 0x13333);
    GLOW_PALETTE[5] = Color_ScaleComponents(rgb, 0x12221, 0xeeee, 0x15555);
    GLOW_PALETTE[6] = Color_ScaleComponents(rgb, 0x13bbb, 0x10000, 0x17777);
}

void GraphicsPalette_DecrementSelectionWrap(void *base)
{
    s32 v;
    u16 t;
    s32 cur;

    base = (u8 *)base + 0x574;
    v = *(u16 *)base;
    t = v;
    cur = t;

    if (cur == 0) {
        cur = 2;
    } else {
        cur = v + 0xFFFF;
    }
    *(u16 *)base = cur;
}

void Menu_AdvanceWorkspaceIndexModulo3(void *arg0)
{
  unsigned int zero;
  unsigned long cnt;
  cnt = 1 + (*((u16 *)(0x574 + ((u8 *)arg0))));
  zero = 0U;
  *((u16 *)(((u8 *)arg0) + 0x574)) = cnt;
  if (((u32)(cnt << 0x10)) >= (((unsigned long) 0x20000U) + 1))
  {
    *((u16 *)(((u8 *)arg0) + 0x574)) = zero;
  }
}

void GraphicsPalette_DecrementSelectedCounter(s32 work)
{
    u8 *p;
    u16 sel;
    s32 off;

    sel = *(u16 *)((u8 *)work + 0x574);
    switch (sel) {
    case 0:
        off = 0x20C;
        p = &gGameState[off];
        break;
    case 1:
        off = 0x205;
        p = &gGameState[off];
        break;
    case 2:
        off = 0x206;
        p = &gGameState[off];
        break;
    default:
        return;
    }
    if (*p) {
        (*p)--;
    }
}

void GraphicsPalette_AdjustSelectionCounter(s32 arg0)
{
    u8 *sp;
    u16 sel;
    s32 off;

    sel = *(u16 *)((u8 *)arg0 + 0x574);
    switch (sel) {
    case 0:
        off = 0x20C;
        sp = &gGameState[off];
        if (*sp <= 1) {
            break;
        }
        return;
    case 1:
        off = 0x205;
        sp = &gGameState[off];
        if (*sp <= 23) {
            break;
        }
        return;
    case 2:
        off = 0x206;
        sp = &gGameState[off];
        if (*sp <= 14) {
            break;
        }
        return;
    default:
        return;
    }
    (*sp)++;
}

void GraphicsPalette_SelectionNoOp(void)
{
}
