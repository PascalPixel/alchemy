#include "TYPES.H"
#include "IWRAM_CALL.H"

/* Palette tint: three BGR channels kept at work + 0x576, driven by the
   day counters in the game state and scaled into palette bank 15. */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
s32 Math_Mod(s32, s32);
extern u8 gGameState[];
#define PARTY_STATE gGameState
extern u8 PaletteGlow_WaveTable[];
void GraphicsPalette_SetTintChannelsFromCounters(void *work)
{
    s16 phase; s32 bias; s32 c2, c0, c1;
    phase = Math_Mod(PARTY_STATE[0x205] + 0xC, 0x18) * 4;
    bias = PARTY_STATE[0x206] - 7;
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

u16 Palette_ScaleTintChannels(u8 *work, s32 scale_r, s32 scale_g, s32 scale_b);

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
