#include "TYPES.H"
#include "IWRAM_CALL.H"

/* The hblank scroll work: two pages of BG3 offsets, a pair for each of the
 * 160 lines, the page the DMA reads, the wave's phase, and per axis the
 * wave's frequency, its step per line and its amplitude. */
struct ScrollWork {
    u16 pages[2][0x3c0];
    u8 page;
    u8 mode;
    u16 phase;
    u8 unknown_f04[4];
    s32 frequency[2];
    s32 step[2];
    s32 amplitude[2];
};

extern struct ScrollWork *gHBlankScrollWork;
extern s16 gBgScroll[];
extern const s16 BabiFune_WaveSine[256];

/* One line's offset: the scroll swayed by the wave at this angle. */
static __inline__ s32 Wave_Offset(s32 angle, s32 amplitude, u16 base)
{
    return (u16)(Iwram_MulQ16(BabiFune_WaveSine[(angle >> 16) & 0xff], amplitude) / 256) + base;
}

/* Babi Fune: fill the page the DMA is not reading with this frame's wave,
 * each line's horizontal and vertical BG3 offset swayed from the scroll by
 * a sine of the line, then flip pages and advance the phase. */
void Engine_BuildScrollPage(void)
{
    struct ScrollWork *work;
    u16 *line;
    s32 angle;
    u16 base;
    s32 amplitude;
    s32 step;
    u16 y;
    s32 i;

    work = gHBlankScrollWork;
    y = gBgScroll[7];
    line = work->pages[work->page ^ 1];
    step = work->step[0];
    angle = (work->phase + y) * work->frequency[0];
    amplitude = work->amplitude[0];
    base = gBgScroll[6];
    for (i = 0; i != 160; i++) {
        *line = Wave_Offset(angle, amplitude, base);
        angle += step;
        line += 2;
    }
    line = work->pages[work->page ^ 1] + 1;
    step = work->step[1];
    angle = (work->phase + y) * work->frequency[1];
    amplitude = work->amplitude[1];
    for (i = 0; i != 160; i++) {
        *line = Wave_Offset(angle, amplitude, y);
        angle += step;
        line += 2;
    }
    work->phase++;
    work->page ^= 1;
}
