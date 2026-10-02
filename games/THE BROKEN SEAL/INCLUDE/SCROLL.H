#ifndef ALCHEMY_SCROLL_H
#define ALCHEMY_SCROLL_H

#include "MAP_SCROLL.H"
#include "LAYOUT_GUARD.H"

/* The idle scanline page is built while H-blank DMA reads the other page. */
struct DisplayScrollWork {
    struct BgScroll rows[2][160][3];
    u8 page;
    u8 mode;
    u16 frame;
    u8 unknown_f04[4];
    s32 freq_x;
    s32 freq_y;
    s32 step_x;
    s32 step_y;
    s32 amp_x;
    s32 amp_y;
};

LAYOUT_SIZE_GUARD(DisplayScrollWork_Size, struct DisplayScrollWork, 0xf20);
LAYOUT_OFFSET_GUARD(DisplayScrollWork_Page, struct DisplayScrollWork, page, 0xf00);
LAYOUT_OFFSET_GUARD(DisplayScrollWork_Frequency, struct DisplayScrollWork, freq_x, 0xf08);

extern struct DisplayScrollWork *gHBlankScrollWork;
extern const s16 DisplayScroll_WaveSine[];

void DisplayScroll_InitializeHBlankDma(
    s32 mode, s32 freq_x, s32 step_x, s32 amp_x,
    s32 freq_y, s32 step_y, s32 amp_y);
void DisplayScroll_BuildAndSwapHBlankPage(void);
void DisplayScroll_ArmHBlankDma(void);
void DisplayScroll_DisarmHBlankDma(void);

#endif
