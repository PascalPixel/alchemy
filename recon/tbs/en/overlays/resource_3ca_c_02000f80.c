/* NONMATCHING: 340 of 340 bytes, 120 differing halfwords, 97 halfword edits
 * (2026-09-26). Loop bodies
 * match. Remaining: global allocation keeps scroll_y in fp and rematerialises
 * the 0xff mask inside each loop, where the reference keeps 0xff in fp and
 * spills scroll_y (as y << 16) to the stack; every register after that shifts
 * by one. A shared mask shortened the owner to 336 bytes; separate loop
 * locals did not move the baseline. Packed scroll arithmetic shortened it
 * to 316 bytes and 64 edits, losing the required signed load and spill. */
#include "TYPES.H"
#include "IWRAM_CALL.H"

struct WaveLine {
    u16 x;
    u16 y;
};

struct WaveState {
    struct WaveLine pages[2][480];
    u8 page;
    u8 unknown_f01;
    u16 phase_x;
    u16 phase_y;
    u8 unknown_f06[2];
    s32 frequency_x;
    s32 frequency_y;
    s32 step_x;
    s32 step_y;
    s32 amplitude_x;
    s32 amplitude_y;
};

struct BgScroll {
    u16 unknown_00[6];
    u16 x;
    s16 y;
};

extern struct WaveState *Data_03001ed8;
extern struct BgScroll Data_03001ad0;
extern s16 Data_020094c8[];

void Local_02000f80(void)
{
    struct WaveState *state;
    struct WaveLine *line;
    u16 scroll_y;
    u16 base;
    s32 acc;
    s32 step;
    s32 amplitude;

    state = Data_03001ed8;
    scroll_y = Data_03001ad0.y;
    line = state->pages[state->page ^ 1];
    step = state->step_x;
    acc = state->frequency_x * (state->phase_x + scroll_y);
    amplitude = state->amplitude_x;
    base = Data_03001ad0.x;
    {
        s32 i;
        u16 off;

        for (i = 0; i != 160; i++) {
            off = Iwram_MulQ16(Data_020094c8[(acc >> 16) & 0xff], amplitude) / 256;
            line->x = off + base;
            acc += step;
            line++;
        }
    }
    line = state->pages[state->page ^ 1];
    step = state->step_y;
    acc = state->frequency_y * (state->phase_y + scroll_y);
    amplitude = state->amplitude_y;
    base = scroll_y;
    {
        s32 i;
        u16 off;

        for (i = 0; i != 160; i++) {
            off = Iwram_MulQ16(Data_020094c8[(acc >> 16) & 0xff], amplitude) / 256;
            line->y = off + base;
            acc += step;
            line++;
        }
    }
    state->phase_y++;
    state->page ^= 1;
}
