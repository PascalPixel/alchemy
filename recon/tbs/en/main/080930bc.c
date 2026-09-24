/* 2026-09-24: hand-written after Event_ShowValue1d8AtPosition (08093168).
   41 differing halfwords. The reference reads two uninitialised locals
   (caller r6 and r8 through r7) for the window position and keeps the event
   work in r8; here both undefined locals share one register and the work
   pointer takes r6. */
#include "TYPES.H"
#include "SYSTEM.H"

struct EventWork {
    u8 unknown_000[0x1cc];
    s32 silent;
    u8 unknown_1d0[8];
    s16 message;
    u8 unknown_1da[0x1a];
    s32 speaker;
    s32 window;
};

extern struct EventWork *Data_03001ebc;
void *ObjectTable_Get(s32 id);
s32 UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y, s32 mode);
s32 UiWork_IsIdleFar(s32 window);

void Func_080930bc(s32 speaker)
{
    s32 x;
    s32 y;
    struct EventWork *state = Data_03001ebc;

    speaker &= 0xfff;
    ObjectTable_Get(speaker);
    state->speaker = speaker;
    if (state->silent == 0) {
        s32 py = y; s32 px = x; s32 min_x = 8; s32 min_y = 20; if (py > 119) py += 32; else py -= 32; if (px < min_x) px = min_x; if (px > 312) px = 312; if (py < min_y) py = min_y; if (py > 220) py = 220;
        state->window = UiText_OpenMessageWindowFar(state->message, px, py, 1);
        while (UiWork_IsIdleFar(state->window) == 0)
            WaitFrames(1);
    }
    state->message++;
}
