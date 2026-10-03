#include "GAME_STATE.H"
#include "WORKSPACE_OPTIONS.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "FIXED_MATH.H"

struct MenuWorkspaceEntry {
    u8 unknown_00[10];
    u16 state;
    u8 unknown_0c[40];
};

u16 Palette_ScaleTintChannels(struct WorkspaceWork *work, s32 red_scale, s32 green_scale, s32 blue_scale);

extern const u8 PaletteGlow_WaveTable[];
#define GLOW_PALETTE ((u16 *)0x050001e8)

u16 Color_ScaleComponents(s16 *rgb, s32 red_scale, s32 green_scale, s32 blue_scale);

/* Clear the seven workspace entries, page, tint and cursor frame. The
   entries' remaining fields are not established. */
void Menu_ResetWorkspaceSelection(struct WorkspaceWork *work)
{
    struct MenuWorkspaceEntry *entry = (struct MenuWorkspaceEntry *)((u8 *)work + 0x400);
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
    work->page = zero;
    work->tint[0] = zero;
    work->tint[1] = zero;
    work->tint[2] = zero;
    work->cursor_frame = zero;
}

/* Palette tint: three BGR channels kept at work + 0x576, driven by the
   day counters in the game state and scaled into palette bank 15. */
void GraphicsPalette_SetTintChannelsFromCounters(struct WorkspaceWork *work)
{
    s16 phase;
    s32 bias;
    s32 blue;
    s32 red;
    s32 green;

    phase = (gGameState.palette_glow[0] + 12) % 24 * 4;
    bias = gGameState.palette_glow[1] - 7;
    red = PaletteGlow_WaveTable[(s16)(phase % 96)];
    green = PaletteGlow_WaveTable[(phase + 32) % 96];
    blue = PaletteGlow_WaveTable[(phase + 64) % 96];
    red += bias;
    green += bias;
    blue += bias;
    if (red < 0)
        red = 0;
    if (green < 0)
        green = 0;
    if (blue < 0)
        blue = 0;
    if (red > 31)
        red = 31;
    if (green > 31)
        green = 31;
    if (blue > 31)
        blue = 31;
    work->tint[0] = red;
    work->tint[1] = green;
    work->tint[2] = blue;
}

void Palette_WriteBlendedBank15Entries(struct WorkspaceWork *work)
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
u16 Palette_ScaleTintChannels(struct WorkspaceWork *work, s32 scale_r, s32 scale_g, s32 scale_b)
{
    s32 r;
    s32 g;
    s32 b;

    r = Iwram_MulQ16(work->tint[0] << 16, scale_r) >> 16;
    g = Iwram_MulQ16(work->tint[1] << 16, scale_g) >> 16;
    b = Iwram_MulQ16(work->tint[2] << 16, scale_b) >> 16;
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

void GraphicsPalette_DecrementSelectionWrap(struct WorkspaceWork *work)
{
    u16 page = work->page;

    if (page == 0)
        page = 2;
    else
        page--;
    work->page = page;
}

void Menu_AdvanceWorkspaceIndexModulo3(struct WorkspaceWork *work)
{
    u16 page = work->page + 1;

    work->page = page;
    if (page > 2)
        work->page = 0;
}

void GraphicsPalette_DecrementSelectedCounter(s32 work)
{
    u8 *p;
    u16 sel;
    s32 off;

    sel = ((struct WorkspaceWork *)work)->page;
    switch (sel) {
    case 0:
        off = 0x20C;
        p = (u8 *)&gGameState + off;
        break;
    case 1:
        p = &gGameState.palette_glow[0];
        break;
    case 2:
        p = &gGameState.palette_glow[1];
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

    sel = ((struct WorkspaceWork *)arg0)->page;
    switch (sel) {
    case 0:
        off = 0x20C;
        sp = (u8 *)&gGameState + off;
        if (*sp <= 1) {
            break;
        }
        return;
    case 1:
        sp = &gGameState.palette_glow[0];
        if (*sp <= 23) {
            break;
        }
        return;
    case 2:
        sp = &gGameState.palette_glow[1];
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
