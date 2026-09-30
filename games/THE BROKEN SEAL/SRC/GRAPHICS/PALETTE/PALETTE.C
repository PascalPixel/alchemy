#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "FIXED_MATH.H"

struct MenuWorkspaceEntry {
    u8 unknown_00[10];
    u16 state;
    u8 unknown_0c[40];
};

struct MenuWorkspace {
    u8 unknown_000[0x400];
    struct MenuWorkspaceEntry entries[7];
    u8 unknown_56c[8];
    u16 selection[4];
    u16 cursor;
};

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
extern u8 gGameState[];
u16 Palette_ScaleTintChannels(u8 *work, s32 scale_r, s32 scale_g, s32 scale_b);

extern const u8 PaletteGlow_WaveTable[];
#define GLOW_PALETTE ((u16 *)0x050001e8)

u16 Color_ScaleComponents(s16 *rgb, s32 red_scale, s32 green_scale, s32 blue_scale);

/* Clears the state of the seven workspace entries, then the four selected
   entries and the cursor that RUN_WORKSPACE_SELECTION_LOOP.C reads. */
void Menu_ResetWorkspaceSelection(struct MenuWorkspace *work)
{
    struct MenuWorkspaceEntry *entry = work->entries;
    u16 zero = 0;

    entry->state = zero;
    entry++;
    entry->state = zero;
    entry++;
    entry->state = zero;
    entry++;
    entry->state = zero;
    entry++;
    entry->state = zero;
    entry++;
    entry->state = zero;
    entry++;
    entry->state = zero;
    work->selection[0] = zero;
    work->selection[1] = zero;
    work->selection[2] = zero;
    work->selection[3] = zero;
    work->cursor = zero;
}

/* Palette tint: three BGR channels kept at work + 0x576, driven by the
   day counters in the game state and scaled into palette bank 15. */
void GraphicsPalette_SetTintChannelsFromCounters(void *work)
{
    s16 phase; s32 bias; s32 c2, c0, c1;
    phase = Math_Mod(gGameState[0x205] + 0xC, 0x18) * 4;
    bias = gGameState[0x206] - 7;
    c0 = PaletteGlow_WaveTable[(s16)Math_Mod(phase, 0x60)];
    c1 = PaletteGlow_WaveTable[Math_Mod(phase + 0x20, 0x60)];
    c2 = PaletteGlow_WaveTable[Math_Mod(phase + 0x40, 0x60)];
    c0 += bias; c1 += bias; c2 += bias;
    if (c0 < 0) c0 = 0; if (c1 < 0) c1 = 0; if (c2 < 0) c2 = 0;
    if (c0 > 0x1F) c0 = 0x1F; if (c1 > 0x1F) c1 = 0x1F; if (c2 > 0x1F) c2 = 0x1F;
    FIELD_AT_OFFSET(work, s16 *, 0x576) = (s16)c0;
    FIELD_AT_OFFSET(work, s16 *, 0x578) = (s16)c1;
    FIELD_AT_OFFSET(work, s16 *, 0x57A) = (s16)c2;
}

void Palette_WriteBlendedBank15Entries(u8 *work)
{
    *(s16 *)0x050001E8 = Palette_ScaleTintChannels(work, 0xEEEE, 0xCCCC, 0x11110);
    *(s16 *)0x050001EA = Palette_ScaleTintChannels(work, 0xD555, 0xBBBB, 0xEEEE);
    *(s16 *)0x050001EC = Palette_ScaleTintChannels(work, 0xBBBB, 0xAAAA, 0xCCCC);
    *(s16 *)0x050001EE = Palette_ScaleTintChannels(work, 0xA221, 0x9999, 0xAAAA);
    *(s16 *)0x050001F0 = Palette_ScaleTintChannels(work, 0x10888, 0xDDDD, 0x13333);
    *(s16 *)0x050001F2 = Palette_ScaleTintChannels(work, 0x12221, 0xEEEE, 0x15555);
    *(s16 *)0x050001F4 = Palette_ScaleTintChannels(work, 0x13BBB, 0x10000, 0x17777);
}

/* Scales the three tint channels kept at work + 0x576 by Q16 factors and
   packs them, clamped to 0-31, into a BGR555 colour. */
u16 Palette_ScaleTintChannels(u8 *work, s32 scale_r, s32 scale_g, s32 scale_b)
{
    s32 r;
    s32 g;
    s32 b;

    r = Iwram_MulQ16(*(u16 *)(work + 0x576) << 16, scale_r) >> 16;
    g = Iwram_MulQ16(*(u16 *)(work + 0x578) << 16, scale_g) >> 16;
    b = Iwram_MulQ16(*(u16 *)(work + 0x57a) << 16, scale_b) >> 16;
    if (r < 0)
        r = 0;
    if (g < 0)
        g = 0;
    if (b < 0)
        b = 0;
    if (r > 31)
        r = 31;
    if (g > 31)
        g = 31;
    if (b > 31)
        b = 31;
    return r + ((b << 10) + (g << 5));
}

u16 Color_ScaleComponents(s16 *rgb, s32 red_scale, s32 green_scale, s32 blue_scale)
{
    s32 red;
    s32 green;
    s32 blue;

    red = Iwram_MulQ16(rgb[0] << 16, red_scale) >> 16;
    green = Iwram_MulQ16(rgb[1] << 16, green_scale) >> 16;
    blue = Iwram_MulQ16(rgb[2] << 16, blue_scale) >> 16;
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
    return red + ((blue << 10) + (green << 5));
}

/* Scales each 5-bit component by its own Q16 factor and packs BGR555. */
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
