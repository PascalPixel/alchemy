#include "types.h"

/*
 * Post a message and, unless messages are suppressed, open the window that
 * shows it and spin until the window reports itself finished.
 *
 * The message id is masked to twelve bits, announced through Func_0808ba1c
 * and recorded at context+0x1f4.  A non-zero word at context+0x1cc suppresses
 * the window entirely; only the sequence counter at context+0x1d8 is then
 * advanced, which happens on both paths.
 *
 * The window is placed at a point that is pushed away from the middle of the
 * screen - 32 further down below scanline 119, 32 further up above it - and
 * then clamped into 8..312 horizontally and 20..220 vertically.  The spin
 * calls Func_080030f8(1) once per frame until Func_08015050 returns non-zero.
 *
 * The placement reads two registers the prologue saves and never writes:
 * that is the frame-balanced live-in recorded for resource_3cd:04b0 - a close
 * without its open - and converts as uninitialised locals rather than a skip.
 *
 * The epilogue at 0x0809314a ends `pop {r0} ; bx r0`, so the owner is void.
 * No inbound call; the address is published as a Thumb pointer at
 * 0x0808a1a4, so the caller is unknown.
 */

void Func_0808ba1c(s32 message);
s32 Func_08015038(s32 sequence, s32 x, s32 y, s32 flags);
s32 Func_08015050(s32 window);
void Func_080030f8(s32 frames);

void Func_080930bc(s32 message)
{
    u8 *context;
    s32 x;
    s32 y;
    s32 window;

    context = *(u8 **)0x03001ebc;
    message &= 0xfff;
    Func_0808ba1c(message);
    *(s32 *)(context + 500) = message;

    if (*(s32 *)(context + 460) == 0) {
        if (y > 119) {
            y += 32;
        } else {
            y -= 32;
        }

        if (x < 8) {
            x = 8;
        }
        if (x > 312) {
            x = 312;
        }
        if (y < 20) {
            y = 20;
        }
        if (y > 220) {
            y = 220;
        }

        window = Func_08015038(*(s16 *)(context + 472), x, y, 1);
        *(s32 *)(context + 504) = window;

        while (Func_08015050(window) == 0) {
            Func_080030f8(1);
        }
    }

    *(u16 *)(context + 472) += 1;
}
