/* Draft, not exact (2026-09-24): 138 of 174 halfwords differ at equal length. Control flow,
   pool and loop shapes match; the residual is allocation: the reference spills both work and
   value to the stack (136-byte frame) and keeps 1 - value in r7 with the mask 7 in r4, while
   this keeps work in fp and value alone on the stack. Declaration order moves nothing. */
#include "TYPES.H"

extern u8 *Data_03001eec;
extern u8 *Data_03001ef0;

u32 Random16(void);
void WaitFrames(s32 frames);

/* Dissolve a 128x192 4bpp-as-8bpp tile canvas with a ragged wipe: each of
   the 128 lanes gets a random 0..63 delay, and the front advances faster
   every frame (horizontally when mode is 1, vertically otherwise), writing
   1 - value into every pixel it passes. */
void Unnamed_080cd104(s32 mode, s32 value)
{
    u8 *work;
    u8 *canvas;
    u8 delay[128];
    u8 *p;
    s32 front;
    s32 speed;
    s32 line;
    s32 lane;
    s32 pos;

    canvas = Data_03001ef0;
    work = Data_03001eec;
    for (lane = 0; lane != 128; lane++)
        delay[lane] = Random16() & 0x3f;

    if (mode == 1) {
        front = 0;
        speed = 1;
        line = 0;
        do {
            front += speed;
            speed++;
            for (; line != front; line++) {
                p = delay;
                for (lane = 0; lane != 128; lane++) {
                    pos = line - *p++;
                    if (pos >= 0) if (pos <= 127)
                        canvas[(((lane / 8) * 16 + pos / 8) * 8 + (lane & 7)) * 8 + (pos & 7)] = 1 - value;
                }
            }
            *(s32 *)(work + 0x7824) = 1;
            WaitFrames(1);
        } while (front <= 256);
    } else {
        front = 0;
        speed = 1;
        line = 0;
        do {
            front += speed / 2;
            speed += 4;
            for (; line != front; line++) {
                p = delay;
                for (lane = 0; lane != 128; lane++) {
                    pos = line - *p++;
                    if (pos >= 0) if (pos <= 127)
                        canvas[(((pos / 8) * 16 + lane / 8) * 8 + (pos & 7)) * 8 + (lane & 7)] = 1 - value;
                }
            }
            *(s32 *)(work + 0x7824) = 1;
            WaitFrames(1);
        } while (front <= 191);
    }
}
